#!/usr/bin/env python3
"""Convert optional region_map_{2,3,4}.png files into assets the region-map engine can draw.

Engine format (verified from src/region_map.c + src/bg.c):
  DISPCNT mode 1, BG2 affine, 8bpp tiles, screenSize 2 = 64x64 8-bit tilemap.
  Tile indices are one byte (0-255). Charblock holds at most 256 8x8 8bpp tiles.
  Palette is loaded at BG slot 7 for 48 colors, so tile pixels must be 112-159.
  Logical cursor grid is 28x15 tiles inset at (1, 2); that is NOT the tilemap size.

Vanilla Hoenn (map.png / map.bin / map.pal) is left alone.
This script only processes region_map_{2,3,4}.png.

Input modes:
  1. Map image: 224x120 PNG (28x15 tiles). Unique 8x8 tiles are packed and a
     64x64 affine tilemap is generated, placed at the cursor inset (1, 2).
  2. Tileset + layout: any other PNG whose size is a multiple of 8, plus an
     existing region_map_N.bin that is 28x15 (8-bit or Tilemap Studio 16-bit
     GBA regular-BG entries) or an already-converted 64x64 affine map.
     16-bit entries are packed (tile ID in bits 0-9, H/V flip in bits 10-11,
     palette bank in bits 12-15). Only the tile ID is used as the source index;
     flips are baked into extra 8x8 tiles because affine maps have no flip bits.
"""

from __future__ import annotations

import os
import struct
import sys
import zlib
from datetime import datetime

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GFX_DIR = os.path.join(ROOT, "graphics", "pokenav", "region_map")
DEFAULT_HEADER = os.path.join(ROOT, "src", "data", "region_map", "region_map_extra_assets.h")
CONVERT_LOG = os.path.join(ROOT, "build", "region_map_convert.log")

AFFINE_MAP_W = 64
AFFINE_MAP_H = 64
MAX_TILES = 256
MAX_COLORS = 48
PALETTE_BASE = 7 * 16  # BG_PLTT_ID(7)
TILE_PX = 8
CURSOR_GRID_W = 28
CURSOR_GRID_H = 15
CURSOR_X_MIN = 1
CURSOR_Y_MIN = 2
MAP_IMAGE_W = CURSOR_GRID_W * TILE_PX
MAP_IMAGE_H = CURSOR_GRID_H * TILE_PX
LAYOUT_ENTRIES = CURSOR_GRID_W * CURSOR_GRID_H
AFFINE_BYTES = AFFINE_MAP_W * AFFINE_MAP_H


class ConvertError(Exception):
    pass


def append_convert_log(text):
    os.makedirs(os.path.dirname(CONVERT_LOG), exist_ok=True)
    stamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    block = "========== %s ==========\n%s" % (stamp, text)
    if not block.endswith("\n"):
        block += "\n"
    with open(CONVERT_LOG, "a", encoding="utf-8") as f:
        f.write(block)
        f.write("\n")


def paeth_predictor(a, b, c):
    p = a + b - c
    pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
    if pa <= pb and pa <= pc:
        return a
    if pb <= pc:
        return b
    return c


def unfilter_scanlines(data, width, bytes_per_pixel, height):
    stride = width * bytes_per_pixel
    out = bytearray(stride * height)
    i = 0
    prev = bytearray(stride)
    for y in range(height):
        ftype = data[i]
        i += 1
        row = bytearray(data[i:i + stride])
        i += stride
        if ftype == 0:
            pass
        elif ftype == 1:
            for x in range(stride):
                left = row[x - bytes_per_pixel] if x >= bytes_per_pixel else 0
                row[x] = (row[x] + left) & 0xFF
        elif ftype == 2:
            for x in range(stride):
                row[x] = (row[x] + prev[x]) & 0xFF
        elif ftype == 3:
            for x in range(stride):
                left = row[x - bytes_per_pixel] if x >= bytes_per_pixel else 0
                row[x] = (row[x] + ((left + prev[x]) // 2)) & 0xFF
        elif ftype == 4:
            for x in range(stride):
                left = row[x - bytes_per_pixel] if x >= bytes_per_pixel else 0
                up = prev[x]
                up_left = prev[x - bytes_per_pixel] if x >= bytes_per_pixel else 0
                row[x] = (row[x] + paeth_predictor(left, up, up_left)) & 0xFF
        else:
            raise ConvertError("unsupported PNG filter %d" % ftype)
        out[y * stride:(y + 1) * stride] = row
        prev = row
    return out


def read_png(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ConvertError("%s is not a PNG" % path)

    pos = 8
    ihdr = None
    plte = None
    idat = bytearray()
    while pos + 8 <= len(data):
        length = struct.unpack(">I", data[pos:pos + 4])[0]
        ctype = data[pos + 4:pos + 8]
        chunk = data[pos + 8:pos + 8 + length]
        pos += 12 + length
        if ctype == b"IHDR":
            ihdr = chunk
        elif ctype == b"PLTE":
            plte = chunk
        elif ctype == b"IDAT":
            idat.extend(chunk)
        elif ctype == b"IEND":
            break

    if ihdr is None or len(ihdr) < 13:
        raise ConvertError("%s missing IHDR" % path)

    width, height, bit_depth, color_type, compression, filt, interlace = struct.unpack(">IIBBBBB", ihdr)
    if compression != 0 or filt != 0:
        raise ConvertError("%s uses unsupported PNG compression/filter method" % path)
    if interlace != 0:
        raise ConvertError("%s is interlaced; save a non-interlaced PNG" % path)
    if width == 0 or height == 0:
        raise ConvertError("%s has invalid dimensions %dx%d" % (path, width, height))

    raw = zlib.decompress(bytes(idat))
    pixels = []  # list of (r, g, b)

    if color_type == 2 and bit_depth == 8:
        bpp = 3
        raw = unfilter_scanlines(raw, width, bpp, height)
        for i in range(0, len(raw), 3):
            pixels.append((raw[i], raw[i + 1], raw[i + 2]))
    elif color_type == 6 and bit_depth == 8:
        bpp = 4
        raw = unfilter_scanlines(raw, width, bpp, height)
        for i in range(0, len(raw), 4):
            pixels.append((raw[i], raw[i + 1], raw[i + 2]))
    elif color_type == 3:
        if plte is None:
            raise ConvertError("%s is indexed but has no PLTE" % path)
        palette = [(plte[i], plte[i + 1], plte[i + 2]) for i in range(0, len(plte), 3)]
        if bit_depth == 8:
            raw = unfilter_scanlines(raw, width, 1, height)
            for idx in raw:
                pixels.append(palette[idx] if idx < len(palette) else (0, 0, 0))
        elif bit_depth == 4:
            stride = (width + 1) // 2
            raw = unfilter_scanlines(raw, stride, 1, height)
            for y in range(height):
                row = raw[y * stride:(y + 1) * stride]
                for x in range(width):
                    byte = row[x // 2]
                    idx = (byte >> 4) if (x % 2) == 0 else (byte & 0x0F)
                    pixels.append(palette[idx] if idx < len(palette) else (0, 0, 0))
        else:
            raise ConvertError("%s indexed bit depth %d is not supported" % (path, bit_depth))
    else:
        raise ConvertError("%s color type %d / bit depth %d is not supported" % (path, color_type, bit_depth))

    unique_rgb = len(set(pixels))
    return width, height, pixels, unique_rgb


def to_rgb555(rgb):
    r, g, b = rgb
    return (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10)


def from_rgb555(c):
    r = (c & 31) << 3
    g = ((c >> 5) & 31) << 3
    b = ((c >> 10) & 31) << 3
    return (r, g, b)


def color_dist2(a, b):
    return (a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2 + (a[2] - b[2]) ** 2


def box_channel_range(box, channel):
    return max(c[2][channel] for c in box) - min(c[2][channel] for c in box)


def median_cut(colors, n_colors):
    """colors: list of (count, rgb555, rgb888)."""
    boxes = [list(colors)]
    while len(boxes) < n_colors:
        box = max(boxes, key=lambda b: max(box_channel_range(b, ch) for ch in range(3)))
        channel = max(range(3), key=lambda ch: box_channel_range(box, ch))
        if box_channel_range(box, channel) == 0:
            break
        box.sort(key=lambda c: c[2][channel])
        mid = max(1, len(box) // 2)
        if mid >= len(box):
            break
        boxes.remove(box)
        boxes.append(box[:mid])
        boxes.append(box[mid:])

    palette = []
    for box in boxes:
        tw = sum(c[0] for c in box)
        if tw == 0:
            continue
        r = sum(c[2][0] * c[0] for c in box) // tw
        g = sum(c[2][1] * c[0] for c in box) // tw
        b = sum(c[2][2] * c[0] for c in box) // tw
        palette.append(to_rgb555((r, g, b)))
    unique = []
    seen = set()
    for c in palette:
        if c not in seen:
            seen.add(c)
            unique.append(c)
    return unique


def build_palette(pixels):
    counts = {}
    for rgb in pixels:
        c555 = to_rgb555(rgb)
        counts[c555] = counts.get(c555, 0) + 1
    unique_555 = len(counts)
    entries = [(n, c, from_rgb555(c)) for c, n in counts.items()]
    if unique_555 <= MAX_COLORS:
        palette = [c for _, c, _ in sorted(entries, key=lambda e: -e[0])]
        quantized = False
    else:
        palette = median_cut(entries, MAX_COLORS)
        quantized = True
    while len(palette) < MAX_COLORS:
        palette.append(0)
    palette = palette[:MAX_COLORS]
    pal_rgb = [from_rgb555(c) for c in palette]
    return palette, pal_rgb, unique_555, quantized


def map_pixels_to_pal(pixels, pal_rgb):
    cache = {}
    indices = []
    for rgb in pixels:
        key = to_rgb555(rgb)
        if key not in cache:
            best_i = 0
            best_d = 10 ** 9
            src = from_rgb555(key)
            for i, dest in enumerate(pal_rgb):
                d = color_dist2(src, dest)
                if d < best_d:
                    best_d = d
                    best_i = i
            cache[key] = PALETTE_BASE + best_i
        indices.append(cache[key])
    return indices


def copy_tile(indices, width, tx, ty):
    buf = bytearray(TILE_PX * TILE_PX)
    p = 0
    for py in range(TILE_PX):
        src = ((ty * TILE_PX + py) * width) + (tx * TILE_PX)
        buf[p:p + TILE_PX] = bytes(indices[src:src + TILE_PX])
        p += TILE_PX
    return bytes(buf)


def extract_tiles_deduped(indices, width, height):
    tw = width // TILE_PX
    th = height // TILE_PX
    tiles = []
    tile_map = []
    lookup = {}
    for ty in range(th):
        row = []
        for tx in range(tw):
            key = copy_tile(indices, width, tx, ty)
            idx = lookup.get(key)
            if idx is None:
                idx = len(tiles)
                if idx >= MAX_TILES:
                    raise ConvertError(
                        "unique 8x8 tiles exceed %d (affine 8bpp charblock limit). "
                        "Simplify the art so identical 8x8 cells can be reused, or supply "
                        "a tileset PNG with <=256 tiles plus a 28x15 layout .bin." % MAX_TILES
                    )
                lookup[key] = idx
                tiles.append(key)
            row.append(idx)
        tile_map.append(row)
    return tiles, tile_map, tw, th


def extract_tiles_sequential(indices, width, height):
    tw = width // TILE_PX
    th = height // TILE_PX
    tiles = []
    for ty in range(th):
        for tx in range(tw):
            tiles.append(copy_tile(indices, width, tx, ty))
    if len(tiles) > MAX_TILES:
        raise ConvertError(
            "tileset has %d 8x8 tiles; affine 8bpp charblock limit is %d. "
            "Reduce the tileset so it contains at most %d tiles."
            % (len(tiles), MAX_TILES, MAX_TILES)
        )
    if not tiles:
        raise ConvertError("tileset is empty")
    return tiles, tw, th


def place_generated_tilemap(tile_map, tw, th):
    affine = bytearray(AFFINE_BYTES)
    if tw == CURSOR_GRID_W and th == CURSOR_GRID_H:
        ox, oy = CURSOR_X_MIN, CURSOR_Y_MIN
        placed = "cursor grid inset (%d, %d)" % (ox, oy)
    else:
        ox, oy = 0, 0
        placed = "top-left (0, 0)"
    if ox + tw > AFFINE_MAP_W or oy + th > AFFINE_MAP_H:
        raise ConvertError(
            "image is %dx%d tiles; affine tilemap is %dx%d. Shrink the PNG."
            % (tw, th, AFFINE_MAP_W, AFFINE_MAP_H)
        )
    for y, row in enumerate(tile_map):
        dest = (oy + y) * AFFINE_MAP_W + ox
        affine[dest:dest + tw] = bytes(row)
    return affine, placed


GBA_TILE_NUM_MASK = 0x03FF
GBA_HFLIP_BIT = 0x0400
GBA_VFLIP_BIT = 0x0800
GBA_PALETTE_SHIFT = 12


def decode_gba_regular_entry(entry):
    """Unpack a Tilemap Studio / GBA regular-BG 16-bit tilemap entry."""
    tile = entry & GBA_TILE_NUM_MASK
    hflip = bool(entry & GBA_HFLIP_BIT)
    vflip = bool(entry & GBA_VFLIP_BIT)
    pal = (entry >> GBA_PALETTE_SHIFT) & 0xF
    return tile, hflip, vflip, pal


def flip_tile_bytes(tile, hflip, vflip):
    rows = [tile[i * TILE_PX:(i + 1) * TILE_PX] for i in range(TILE_PX)]
    if hflip:
        rows = [bytes(reversed(row)) for row in rows]
    if vflip:
        rows = list(reversed(rows))
    return b"".join(rows)


def bake_regular_bg_entries(tiles, raw_entries, label):
    """Turn packed 16-bit regular-BG entries into affine 8-bit indices.

    Affine tilemaps cannot store flip/palette bits, so H/V flips are resolved
    by appending flipped copies of the source 8x8 tiles.
    """
    source_count = len(tiles)
    out_tiles = list(tiles)
    by_pixels = {tile: i for i, tile in enumerate(out_tiles)}
    variants = {}
    affine_entries = []
    baked_flips = 0

    for packed in raw_entries:
        tid, hflip, vflip, _pal = decode_gba_regular_entry(packed)
        if tid >= source_count:
            raise ConvertError(
                "%s tile ID %d (decoded from packed entry 0x%04X) is out of range "
                "for a tileset with %d tiles"
                % (label, tid, packed, source_count)
            )
        key = (tid, hflip, vflip)
        idx = variants.get(key)
        if idx is None:
            tile = out_tiles[tid]
            if hflip or vflip:
                tile = flip_tile_bytes(tile, hflip, vflip)
                idx = by_pixels.get(tile)
                if idx is None:
                    idx = len(out_tiles)
                    if idx >= MAX_TILES:
                        raise ConvertError(
                            "baking H/V flips from %s produced more than %d unique tiles "
                            "(affine 8bpp charblock limit)"
                            % (label, MAX_TILES)
                        )
                    out_tiles.append(tile)
                    by_pixels[tile] = idx
                    baked_flips += 1
            else:
                idx = tid
            variants[key] = idx
        affine_entries.append(idx)

    decoded_ids = [decode_gba_regular_entry(e)[0] for e in raw_entries]
    return out_tiles, affine_entries, max(decoded_ids) if decoded_ids else 0, baked_flips


def validate_tile_indices(entries, num_tiles, label):
    if not entries:
        raise ConvertError("%s is empty" % label)
    max_index = max(entries)
    min_index = min(entries)
    if min_index < 0:
        raise ConvertError("%s has negative tile index %d" % (label, min_index))
    if max_index > 255:
        raise ConvertError(
            "%s tile index %d exceeds the affine 8-bit limit 255. "
            "The tileset must have at most 256 unique 8x8 tiles."
            % (label, max_index)
        )
    if max_index >= num_tiles:
        raise ConvertError(
            "%s tile index %d is out of range for a tileset with %d tiles"
            % (label, max_index, num_tiles)
        )
    return max_index


def choose_layout_pad_tile(entries):
    """Pick the ocean/background tile from the 28x15 border (most common edge tile)."""
    counts = {}

    def add(index):
        tile = entries[index]
        counts[tile] = counts.get(tile, 0) + 1

    for x in range(CURSOR_GRID_W):
        add(x)
        add((CURSOR_GRID_H - 1) * CURSOR_GRID_W + x)
    for y in range(CURSOR_GRID_H):
        add(y * CURSOR_GRID_W)
        add(y * CURSOR_GRID_W + (CURSOR_GRID_W - 1))
    if not counts:
        return 0
    return max(counts, key=counts.get)


def extract_inset_entries(affine):
    entries = []
    for y in range(CURSOR_GRID_H):
        src = (CURSOR_Y_MIN + y) * AFFINE_MAP_W + CURSOR_X_MIN
        entries.extend(affine[src:src + CURSOR_GRID_W])
    return entries


def fill_affine_padding(affine, pad_tile):
    for y in range(AFFINE_MAP_H):
        row = y * AFFINE_MAP_W
        for x in range(AFFINE_MAP_W):
            if x < CURSOR_X_MIN or x >= CURSOR_X_MIN + CURSOR_GRID_W or y < CURSOR_Y_MIN or y >= CURSOR_Y_MIN + CURSOR_GRID_H:
                affine[row + x] = pad_tile


def affine_padding_is_uniform(affine):
    first = None
    for y in range(AFFINE_MAP_H):
        row = y * AFFINE_MAP_W
        for x in range(AFFINE_MAP_W):
            if CURSOR_X_MIN <= x < CURSOR_X_MIN + CURSOR_GRID_W and CURSOR_Y_MIN <= y < CURSOR_Y_MIN + CURSOR_GRID_H:
                continue
            tile = affine[row + x]
            if first is None:
                first = tile
            elif tile != first:
                return False
    return True


def place_layout_entries(entries):
    pad_tile = choose_layout_pad_tile(entries)
    affine = bytearray([pad_tile]) * AFFINE_BYTES
    i = 0
    for y in range(CURSOR_GRID_H):
        dest = (CURSOR_Y_MIN + y) * AFFINE_MAP_W + CURSOR_X_MIN
        affine[dest:dest + CURSOR_GRID_W] = bytes(entries[i:i + CURSOR_GRID_W])
        i += CURSOR_GRID_W
    return affine, pad_tile


def load_existing_layout(bin_path, tiles):
    if not os.path.exists(bin_path):
        return None
    with open(bin_path, "rb") as f:
        data = f.read()
    label = os.path.basename(bin_path)
    num_tiles = len(tiles)

    if len(data) == LAYOUT_ENTRIES * 2:
        raw = struct.unpack("<%dH" % LAYOUT_ENTRIES, data)
        tiles, entries, decoded_max, baked_flips = bake_regular_bg_entries(tiles, raw, label)
        max_index = validate_tile_indices(entries, len(tiles), label)
        affine, pad_tile = place_layout_entries(entries)
        placed = (
            "28x15 Tilemap Studio 16-bit (tile ID bits 0-9, H/V flip baked) "
            "-> affine 64x64 8-bit inset (%d, %d); decoded tile IDs 0-%d, %d flip variant(s), pad tile %d"
            % (CURSOR_X_MIN, CURSOR_Y_MIN, decoded_max, baked_flips, pad_tile)
        )
        return tiles, affine, placed, max_index

    if len(data) == LAYOUT_ENTRIES:
        entries = list(data)
        max_index = validate_tile_indices(entries, num_tiles, label)
        affine, pad_tile = place_layout_entries(entries)
        placed = "28x15 8-bit -> affine 64x64 8-bit inset (%d, %d), pad tile %d" % (CURSOR_X_MIN, CURSOR_Y_MIN, pad_tile)
        return tiles, affine, placed, max_index

    if len(data) == AFFINE_BYTES:
        affine = bytearray(data)
        max_index = validate_tile_indices(affine, num_tiles, label)
        placed = "affine 64x64 8-bit (existing)"
        if affine_padding_is_uniform(affine):
            pad_tile = choose_layout_pad_tile(extract_inset_entries(affine))
            fill_affine_padding(affine, pad_tile)
            placed += ", padded unused cells with tile %d" % pad_tile
        return tiles, affine, placed, max_index

    raise ConvertError(
        "%s is %d bytes; expected %d (8-bit 28x15), %d (16-bit 28x15), or %d (affine 64x64 8-bit)"
        % (label, len(data), LAYOUT_ENTRIES, LAYOUT_ENTRIES * 2, AFFINE_BYTES)
    )


def write_jasc_pal(path, pal_rgb):
    lines = ["JASC-PAL", "0100", str(MAX_COLORS)]
    for r, g, b in pal_rgb:
        lines.append("%d %d %d" % (r, g, b))
    with open(path, "w", encoding="ascii", newline="\n") as f:
        f.write("\n".join(lines) + "\n")


def convert_slot(n, root=ROOT):
    gfx_dir = os.path.join(root, "graphics", "pokenav", "region_map")
    png_path = os.path.join(gfx_dir, "region_map_%d.png" % n)
    bpp_path = os.path.join(gfx_dir, "region_map_%d.8bpp" % n)
    bin_path = os.path.join(gfx_dir, "region_map_%d.bin" % n)
    pal_path = os.path.join(gfx_dir, "region_map_%d.pal" % n)

    if not os.path.exists(png_path):
        raise ConvertError("missing %s" % png_path)

    width, height, pixels, unique_rgb = read_png(png_path)
    if width % TILE_PX or height % TILE_PX:
        raise ConvertError(
            "region_map_%d.png is %dx%d; both sides must be multiples of %d"
            % (n, width, height, TILE_PX)
        )

    palette_555, pal_rgb, unique_555, quantized = build_palette(pixels)
    indices = map_pixels_to_pal(pixels, pal_rgb)
    unique_mapped = sorted(set(indices))
    if not unique_mapped:
        raise ConvertError("region_map_%d.png produced no pixels" % n)
    if unique_mapped[0] < PALETTE_BASE or unique_mapped[-1] >= PALETTE_BASE + MAX_COLORS:
        raise ConvertError(
            "internal palette remap produced pixel indices %d-%d; expected %d-%d"
            % (unique_mapped[0], unique_mapped[-1], PALETTE_BASE, PALETTE_BASE + MAX_COLORS - 1)
        )

    is_map_image = width == MAP_IMAGE_W and height == MAP_IMAGE_H
    skip_gfx = False
    if is_map_image:
        tiles, tile_map, tw, th = extract_tiles_deduped(indices, width, height)
        affine, placed = place_generated_tilemap(tile_map, tw, th)
        mode = "map image (deduped 8x8 tiles, generated affine tilemap)"
        layout_max = max(max(row) for row in tile_map) if tile_map else 0
        max_tile_index = len(tiles) - 1
    else:
        existing_bin = b""
        if os.path.exists(bin_path):
            with open(bin_path, "rb") as f:
                existing_bin = f.read()
        # Already-converted affine maps: only fix padding. Do not rebuild .8bpp/.pal.
        # Baked flip tiles live in the existing .8bpp, so never re-index from the PNG sheet.
        if len(existing_bin) == AFFINE_BYTES:
            if not os.path.exists(bpp_path):
                raise ConvertError(
                    "region_map_%d.bin is a 64x64 affine tilemap but region_map_%d.8bpp is missing"
                    % (n, n)
                )
            affine = bytearray(existing_bin)
            placed = "affine 64x64 8-bit (existing)"
            # Always fill cells outside the 28x15 inset. If the existing padding is
            # mixed (vanilla-style overflow art), keep those extra tiles and only
            # replace a uniform leftover pad color.
            pad_tile = choose_layout_pad_tile(extract_inset_entries(affine))
            if affine_padding_is_uniform(affine):
                fill_affine_padding(affine, pad_tile)
                placed += ", padded unused cells with tile %d" % pad_tile
            else:
                placed += ", kept mixed overflow tiles outside 28x15 (pad tile would be %d)" % pad_tile
            tw, th = width // TILE_PX, height // TILE_PX
            ntiles = os.path.getsize(bpp_path) // (TILE_PX * TILE_PX)
            max_tile_index = ntiles - 1
            layout_max = max(affine) if affine else 0
            mode = "tileset sheet + layout .bin (tilemap padding only)"
            skip_gfx = True
            tiles = []
        else:
            tiles, tw, th = extract_tiles_sequential(indices, width, height)
            loaded = load_existing_layout(bin_path, tiles)
            if loaded is None:
                raise ConvertError(
                    "region_map_%d.png is %dx%d (%d x %d tiles), not a 224x120 map image. "
                    "Provide region_map_%d.bin as a 28x15 tilemap (8-bit or Tilemap Studio 16-bit entries) "
                    "or save the PNG as 224x120 (28x15 tiles)."
                    % (n, width, height, tw, th, n)
                )
            tiles, affine, placed, layout_max = loaded
            mode = "tileset sheet + layout .bin"
            max_tile_index = len(tiles) - 1

    if not skip_gfx:
        with open(bpp_path, "wb") as f:
            f.write(b"".join(tiles))
        write_jasc_pal(pal_path, pal_rgb)
    with open(bin_path, "wb") as f:
        f.write(affine)

    wrote = [os.path.basename(bin_path)]
    if not skip_gfx:
        wrote = [os.path.basename(bpp_path), os.path.basename(bin_path), os.path.basename(pal_path)]

    summary = "\n".join([
        "PASS region_map_%d" % n,
        "region_map_%d conversion summary:" % n,
        "  source PNG           : %dx%d (%d x %d tiles)" % (width, height, tw, th),
        "  input mode           : %s" % mode,
        "  unique source colors : %d RGB / %d RGB555" % (unique_rgb, unique_555),
        "  palette mode         : 8bpp affine, 48 colors at BG indices %d-%d (slot 7)" % (PALETTE_BASE, PALETTE_BASE + MAX_COLORS - 1),
        "  palette strategy     : remap generated PNG colors into vanilla LoadPalette range",
        "  quantized            : %s" % ("yes, to 48 GBA colors" if quantized else "no"),
        "  unique tiles         : %d / %d" % (max_tile_index + 1, MAX_TILES),
        "  maximum tile index   : %d (layout max %d)" % (max_tile_index, layout_max),
        "  tilemap              : affine 8-bit %dx%d (%d bytes), %s" % (AFFINE_MAP_W, AFFINE_MAP_H, len(affine), placed),
        "  pixel index range    : %d-%d" % (unique_mapped[0], unique_mapped[-1]),
        "  wrote                : %s" % ", ".join(wrote),
        "",
    ])
    append_convert_log(summary)
    print("region_map_%d: pass" % n)
    return True


def convert_slot_checked(n, root=ROOT):
    try:
        convert_slot(n, root)
        return 0
    except ConvertError as e:
        append_convert_log("FAIL region_map_%d\nerror: %s\n" % (n, e))
        print("region_map_%d: fail" % n)
        return 1


def emit_slot_header(n, root=ROOT):
    gfx_dir = os.path.join(root, "graphics", "pokenav", "region_map")
    png = os.path.join(gfx_dir, "region_map_%d.png" % n)
    rel = "graphics/pokenav/region_map"
    lines = []
    if os.path.exists(png):
        lines.append('static const u16 sRegionMap%d_Pal[] = INCBIN_U16("%s/region_map_%d.gbapal");' % (n, rel, n))
        lines.append('static const u32 sRegionMap%d_GfxLZ[] = INCBIN_U32("%s/region_map_%d.8bpp.lz");' % (n, rel, n))
        lines.append('static const u32 sRegionMap%d_TilemapLZ[] = INCBIN_U32("%s/region_map_%d.bin.lz");' % (n, rel, n))
        lines.append("#define REGION_MAP_%d_PAL sRegionMap%d_Pal" % (n, n))
        lines.append("#define REGION_MAP_%d_GFX sRegionMap%d_GfxLZ" % (n, n))
        lines.append("#define REGION_MAP_%d_TILEMAP sRegionMap%d_TilemapLZ" % (n, n))
    else:
        lines.append("#define REGION_MAP_%d_PAL sRegionMapBg_Pal" % n)
        lines.append("#define REGION_MAP_%d_GFX sRegionMapBg_GfxLZ" % n)
        lines.append("#define REGION_MAP_%d_TILEMAP sRegionMapBg_TilemapLZ" % n)
    return "\n".join(lines)


def write_header(out_path, root=ROOT):
    chunks = [
        "// Generated by tools/generate_region_map_assets.py",
        "// Optional maps: graphics/pokenav/region_map/region_map_{2,3,4}.png",
        "// Custom maps are converted to affine 8bpp, 64x64 8-bit tilemaps, and",
        "// 48 colors at BG palette slot 7 (pixel indices 112-159).",
        "#ifndef GUARD_REGION_MAP_EXTRA_ASSETS_H",
        "#define GUARD_REGION_MAP_EXTRA_ASSETS_H",
        "",
        emit_slot_header(2, root),
        emit_slot_header(3, root),
        emit_slot_header(4, root),
        "",
        "#endif // GUARD_REGION_MAP_EXTRA_ASSETS_H",
        "",
    ]
    text = "\n".join(chunks)
    old = None
    if os.path.exists(out_path):
        with open(out_path, "r", encoding="utf-8") as f:
            old = f.read()
    if old != text:
        os.makedirs(os.path.dirname(out_path), exist_ok=True)
        with open(out_path, "w", encoding="utf-8", newline="\n") as f:
            f.write(text)


def usage():
    print(
        "usage: generate_region_map_assets.py [--convert N | --header-only [out.h] | [out.h]]",
        file=sys.stderr,
    )


def main(argv=None):
    argv = list(sys.argv[1:] if argv is None else argv)
    root = ROOT
    if argv[:1] == ["--convert"]:
        if len(argv) < 2:
            usage()
            return 2
        return convert_slot_checked(int(argv[1]), root)

    header_only = argv[:1] == ["--header-only"]
    if header_only:
        argv = argv[1:]

    out_path = argv[0] if argv else DEFAULT_HEADER
    if not os.path.isabs(out_path):
        out_path = os.path.join(root, out_path)

    if not header_only:
        for n in (2, 3, 4):
            png = os.path.join(root, "graphics", "pokenav", "region_map", "region_map_%d.png" % n)
            if os.path.exists(png):
                if convert_slot_checked(n, root) != 0:
                    return 1
    write_header(out_path, root)
    return 0


if __name__ == "__main__":
    sys.exit(main())
