# Quest menu graphics

Place the following files in this folder. The build converts them automatically.

| Source file | Built as | Purpose |
|-------------|----------|---------|
| `menu.png` | `menu.4bpp.lz` | Background tileset (256×256, 4bpp indexed) |
| `menu_pal.pal` | `menu_pal.gbapal` | Background palette (included) |
| `tilemap.bin` | `tilemap.bin.lz` | BG1 tilemap for the menu layout |
| `main_window.pal` | `main_window.gbapal` | Window/text palette (included) |

Reference assets from the pret quest menu port:
https://github.com/pret/pokeemerald/tree/master/graphics/quest_menu

Optional FRLG-style window tiles (not required; standard Emerald frames are used by default):
- `../text_window/fr_std0.png`
- `../text_window/fr_message_box.png`

After replacing `menu.png` or `tilemap.bin`, run `make clean` (or delete the matching `.4bpp`/`.lz` files) so gbagfx rebuilds them.
