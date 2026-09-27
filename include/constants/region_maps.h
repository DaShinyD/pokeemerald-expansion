#ifndef GUARD_CONSTANTS_REGION_MAPS_H
#define GUARD_CONSTANTS_REGION_MAPS_H

// Slots for the Town Map / PokéNav / Fly region pictures.
// Region 1 is vanilla Hoenn. Extra pictures are optional PNGs:
//   graphics/pokenav/region_map/region_map_{2,3,4}.png
// tools/generate_region_map_assets.py converts those PNGs into engine assets:
//   affine 8bpp BG2, 64x64 8-bit tilemap, <=256 unique 8x8 tiles,
//   48 colors loaded at BG palette slot 7 (pixel indices 112-159).
// Missing pictures reuse the Hoenn map so SELECT cycling still works.
#define REGION_MAP_1 0
#define REGION_MAP_2 1
#define REGION_MAP_3 2
#define REGION_MAP_4 3
#define REGION_MAP_COUNT 4

#define REGION_MAP_HOENN REGION_MAP_1
#define REGION_MAP_KANTO REGION_MAP_2
#define REGION_MAP_JOHTO REGION_MAP_3
#define REGION_MAP_HANKU REGION_MAP_4

#endif // GUARD_CONSTANTS_REGION_MAPS_H
