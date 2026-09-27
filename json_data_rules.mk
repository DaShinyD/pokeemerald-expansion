# JSON files are run through jsonproc, which is a tool that converts JSON data to an output file
# based on an Inja template. https://github.com/pantor/inja

AUTO_GEN_TARGETS += $(DATA_SRC_SUBDIR)/wild_encounters.h
$(DATA_SRC_SUBDIR)/wild_encounters.h: $(DATA_SRC_SUBDIR)/wild_encounters.json $(DATA_SRC_SUBDIR)/wild_encounters.json.txt
	$(JSONPROC) $^ $@

$(C_BUILDDIR)/wild_encounter.o: c_dep += $(DATA_SRC_SUBDIR)/wild_encounters.h

AUTO_GEN_TARGETS += $(DATA_SRC_SUBDIR)/region_map/region_map_entries.h
$(DATA_SRC_SUBDIR)/region_map/region_map_entries.h: $(DATA_SRC_SUBDIR)/region_map/region_map_sections.json $(DATA_SRC_SUBDIR)/region_map/region_map_sections.json.txt
	$(JSONPROC) $^ $@

$(C_BUILDDIR)/region_map.o: c_dep += $(DATA_SRC_SUBDIR)/region_map/region_map_entries.h

# Optional extra region maps. PNGs are converted to affine 8bpp + 64x64 8-bit
# tilemap + 48-color pal by tools/generate_region_map_assets.py (not gbagfx).
# Do not add gbagfx %.8bpp rules for region_map_{2,3,4}.png; they would emit
# unmapped 0-255 pixel indices and skip the affine tilemap rewrite.
AUTO_GEN_TARGETS += $(DATA_SRC_SUBDIR)/region_map/region_map_extra_assets.h
$(DATA_SRC_SUBDIR)/region_map/region_map_extra_assets.h: tools/generate_region_map_assets.py $(wildcard graphics/pokenav/region_map/region_map_2.png) $(wildcard graphics/pokenav/region_map/region_map_3.png) $(wildcard graphics/pokenav/region_map/region_map_4.png)
	python3 tools/generate_region_map_assets.py --header-only $@

$(C_BUILDDIR)/region_map.o: $(DATA_SRC_SUBDIR)/region_map/region_map_extra_assets.h

ifneq ($(wildcard graphics/pokenav/region_map/region_map_2.png),)
graphics/pokenav/region_map/region_map_2.8bpp graphics/pokenav/region_map/region_map_2.bin graphics/pokenav/region_map/region_map_2.pal &: graphics/pokenav/region_map/region_map_2.png tools/generate_region_map_assets.py
	python3 tools/generate_region_map_assets.py --convert 2
$(C_BUILDDIR)/region_map.o: graphics/pokenav/region_map/region_map_2.8bpp.lz graphics/pokenav/region_map/region_map_2.gbapal graphics/pokenav/region_map/region_map_2.bin.lz
endif
ifneq ($(wildcard graphics/pokenav/region_map/region_map_3.png),)
graphics/pokenav/region_map/region_map_3.8bpp graphics/pokenav/region_map/region_map_3.bin graphics/pokenav/region_map/region_map_3.pal &: graphics/pokenav/region_map/region_map_3.png tools/generate_region_map_assets.py
	python3 tools/generate_region_map_assets.py --convert 3
$(C_BUILDDIR)/region_map.o: graphics/pokenav/region_map/region_map_3.8bpp.lz graphics/pokenav/region_map/region_map_3.gbapal graphics/pokenav/region_map/region_map_3.bin.lz
endif
ifneq ($(wildcard graphics/pokenav/region_map/region_map_4.png),)
graphics/pokenav/region_map/region_map_4.8bpp graphics/pokenav/region_map/region_map_4.bin graphics/pokenav/region_map/region_map_4.pal &: graphics/pokenav/region_map/region_map_4.png tools/generate_region_map_assets.py
	python3 tools/generate_region_map_assets.py --convert 4
$(C_BUILDDIR)/region_map.o: graphics/pokenav/region_map/region_map_4.8bpp.lz graphics/pokenav/region_map/region_map_4.gbapal graphics/pokenav/region_map/region_map_4.bin.lz
endif
