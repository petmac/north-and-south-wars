TEMP_DIR := temp
TEMP_ASSETS_DIR := $(TEMP_DIR)/assets
OUT_DIR := $(TEMP_DIR)/game/main
DATA_DIR := $(OUT_DIR)/data

# TOP LEVEL

BPL_ASSET_NAMES := small_font
MAP := $(DATA_DIR)/mission/map.map

.SECONDARY:

.PHONY: all
all: $(DATA_DIR)/mouse.SPR $(DATA_DIR)/palette.PAL $(foreach name,$(BPL_ASSET_NAMES),$(DATA_DIR)/$(name).BPL) $(MAP) $(DATA_DIR)/mission.chip.lz

.PHONY: clean
clean:
	rm -rf $(TEMP_DIR)
	cd tools && cargo clean
	ninja clean

# DATA

TMX := assets/mission/map.tmx
TSX := assets/mission/tiles.tsx

TMX2MAP := tools/target/release/tmx2map
TSX2BPL := tools/target/release/tsx2bpl

ASEPRITE := "$(HOME)/Library/Application Support/Steam/steamapps/common/Aseprite/Aseprite.app/Contents/MacOS/aseprite"

FREEIMAGE_PREFIX := $(shell brew --prefix freeimage)
KINGCON := external/kingcon/build/kingcon

LZ := temp/lz/lz

# Convert tileset image from TSX and .png to .bpl
$(TEMP_ASSETS_DIR)/mission/tiles.bpl: $(TSX) $(TSX2BPL) assets/mission/tiles.png
	mkdir -p $(dir $@)
	$(TSX2BPL) $< $@

# Convert map from Tiled .tmx to .map
$(MAP): $(TMX) $(TMX2MAP)
	mkdir -p $(dir $@)
	$(TMX2MAP) $< $@

# Copy file from temp assets to data
$(DATA_DIR)/%: $(TEMP_ASSETS_DIR)/%
	mkdir -p $(dir $@)
	cp $< $@

# Pack file
$(TEMP_ASSETS_DIR)/%.lz: $(TEMP_ASSETS_DIR)/% $(LZ)
	$(LZ) -o $@ $<

# Pack mission data
MISSION_CHIP_ASSET_NAMES := \
	arrows.BPL \
	tiles.bpl \
	units.BPL \
	menu.BPL \
	encounter/bg_bridge.BPL \
	encounter/bg_mountain.BPL \
	encounter/bg_plain.BPL \
	encounter/bg_road.BPL \
	encounter/bg_woods.BPL \
	encounter/units.BPL
$(TEMP_ASSETS_DIR)/mission.chip: $(foreach name,$(MISSION_CHIP_ASSET_NAMES),$(TEMP_ASSETS_DIR)/mission/$(name))
	cat $^ >$@

# Convert background image from .png to .BPL
$(TEMP_ASSETS_DIR)/mission/encounter/bg_%.BPL: $(TEMP_ASSETS_DIR)/mission/encounter/bg_%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved

# Convert image from .png to .BPL
$(TEMP_ASSETS_DIR)/%.BPL: $(TEMP_ASSETS_DIR)/%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved -Mask

# Convert palette from .png to .PAL
$(TEMP_ASSETS_DIR)/%.PAL: $(TEMP_ASSETS_DIR)/%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -RawPalette

# Convert image from .png to .SPR
$(TEMP_ASSETS_DIR)/%.SPR: $(TEMP_ASSETS_DIR)/%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=s16

# Convert image from .aseprite to .png
$(TEMP_ASSETS_DIR)/%.png: assets/%.aseprite
	$(ASEPRITE) --batch $< --save-as $@

# DISTRIBUTION

TITLE := NorthAndSouthWars
DIST_ZIP := $(TEMP_DIR)/dist/$(TITLE).zip
BUTLER := external/butler-darwin-amd64/butler

.PHONY: itch
itch: $(DIST_ZIP)
	$(BUTLER) push $< petmac/north-and-south-wars:dev

# TOOLS

# Rust tools
$(TMX2MAP) $(TSX2BPL): tools

.PHONY: tools
tools:
	cd tools && cargo build --release

# Kingcon
$(KINGCON):
	CPATH="$(FREEIMAGE_PREFIX)/include" LIBRARY_PATH="$(FREEIMAGE_PREFIX)/lib" $(MAKE) --directory external/kingcon

# Doynax packer
$(LZ): external/doynamite68k/lz.c
	mkdir -p $(dir $@)
	gcc -O2 $^ -o $@
