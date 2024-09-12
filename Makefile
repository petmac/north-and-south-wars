TEMP_DIR := temp
OUT_DIR := $(TEMP_DIR)/game/main
DATA_DIR := $(OUT_DIR)/data

TMX := assets/mission/map.tmx
TSX := assets/mission/tiles.tsx

MAP := $(DATA_DIR)/mission/map.map
BPL_ASSET_NAMES := \
	mission/attack/bg_bridge \
	mission/attack/bg_mountain \
	mission/attack/bg_plain \
	mission/attack/bg_road \
	mission/attack/bg_woods \
	mission/attack/units \
	mission/arrows \
	mission/menu \
	mission/units \
	small_font
TILES_BPL := $(DATA_DIR)/mission/tiles.bpl

TMX2MAP := tools/target/release/tmx2map
TSX2BPL := tools/target/release/tsx2bpl

ASEPRITE := "$(HOME)/Library/Application Support/Steam/steamapps/common/Aseprite/Aseprite.app/Contents/MacOS/aseprite"

FREEIMAGE_PREFIX := $(shell brew --prefix freeimage)
KINGCON := external/kingcon/build/kingcon

.PHONY: all
all: cmake-build $(DATA_DIR)/mouse.SPR $(DATA_DIR)/palette.PAL $(TILES_BPL) $(foreach name,$(BPL_ASSET_NAMES),$(DATA_DIR)/$(name).BPL) $(MAP)

.PHONY: clean
clean:
	rm -rf $(TEMP_DIR)
	cd tools && cargo clean

.PHONY: cmake-build
cmake-build: $(TEMP_DIR)/build.ninja
	cmake --build --preset amiga

$(TEMP_DIR)/build.ninja: CMakeLists.txt CMakePresets.json
	cmake --preset amiga

$(TILES_BPL): $(TSX) $(TSX2BPL) assets/mission/tiles.png
	mkdir -p $(dir $@)
	$(TSX2BPL) $< $@

$(MAP): $(TMX) $(TMX2MAP)
	mkdir -p $(dir $@)
	$(TMX2MAP) $< $@

$(TMX2MAP) $(TSX2BPL): tools

.PHONY: tools
tools:
	cd tools && cargo build --release

# Convert background image from .png to .BPL
# https://github.com/grahambates/kingcon/blob/master/README.md#image-conversion-output-format
$(DATA_DIR)/mission/attack/bg_%.BPL: $(TEMP_DIR)/assets/mission/attack/bg_%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved

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
