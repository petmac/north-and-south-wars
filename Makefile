TEMP_DIR := temp
OUT_DIR := $(TEMP_DIR)/game/main
DATA_DIR := $(OUT_DIR)/data

TMX := assets/mission/map.tmx
TSX := assets/mission/tiles.tsx

MAP := $(DATA_DIR)/mission/map.map
BPL_ASSET_NAMES := small_font

TMX2MAP := tools/target/release/tmx2map
TSX2BPL := tools/target/release/tsx2bpl

ASEPRITE := "$(HOME)/Library/Application Support/Steam/steamapps/common/Aseprite/Aseprite.app/Contents/MacOS/aseprite"

FREEIMAGE_PREFIX := $(shell brew --prefix freeimage)
KINGCON := external/kingcon/build/kingcon

.PHONY: all
all: cmake-build $(DATA_DIR)/mouse.SPR $(DATA_DIR)/palette.PAL $(foreach name,$(BPL_ASSET_NAMES),$(DATA_DIR)/$(name).BPL) $(MAP) $(DATA_DIR)/mission.chip

.PHONY: clean
clean:
	rm -rf $(TEMP_DIR)
	cd tools && cargo clean

.PHONY: cmake-build
cmake-build: $(TEMP_DIR)/build.ninja
	cmake --build --preset amiga

$(TEMP_DIR)/build.ninja: CMakeLists.txt CMakePresets.json
	cmake --preset amiga

$(TEMP_DIR)/assets/mission/tiles.bpl: $(TSX) $(TSX2BPL) assets/mission/tiles.png
	mkdir -p $(dir $@)
	$(TSX2BPL) $< $@

$(MAP): $(TMX) $(TMX2MAP)
	mkdir -p $(dir $@)
	$(TMX2MAP) $< $@

$(TMX2MAP) $(TSX2BPL): tools

.PHONY: tools
tools:
	cd tools && cargo build --release

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

$(DATA_DIR)/mission.chip: $(foreach name,$(MISSION_CHIP_ASSET_NAMES),$(TEMP_DIR)/assets/mission/$(name))
	cat $^ >$@

# Convert background image from .png to .BPL
# https://github.com/grahambates/kingcon/blob/master/README.md#image-conversion-output-format
$(TEMP_DIR)/assets/mission/encounter/bg_%.BPL: $(TEMP_DIR)/assets/mission/encounter/bg_%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved

# Convert image from .png to .BPL
# https://github.com/grahambates/kingcon/blob/master/README.md#image-conversion-output-format
%.BPL: %.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved -Mask

# Convert image from .png to .BPL
# https://github.com/grahambates/kingcon/blob/master/README.md#image-conversion-output-format
$(DATA_DIR)/%.BPL: $(TEMP_DIR)/assets/%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved -Mask

# Convert palette from .png to .PAL
# https://github.com/grahambates/kingcon/blob/master/README.md#image-conversion-output-format
$(DATA_DIR)/palette.PAL: $(TEMP_DIR)/assets/palette.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -RawPalette

# Convert image from .png to .SPR
# https://github.com/grahambates/kingcon/blob/master/README.md#image-conversion-output-format
$(DATA_DIR)/%.SPR: $(TEMP_DIR)/assets/%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=s16

# Convert image from .aseprite to .png
$(TEMP_DIR)/assets/%.png: assets/%.aseprite
	$(ASEPRITE) --batch $< --save-as $@

# Kingcon
$(KINGCON):
	CPATH="$(FREEIMAGE_PREFIX)/include" LIBRARY_PATH="$(FREEIMAGE_PREFIX)/lib" $(MAKE) --directory external/kingcon
