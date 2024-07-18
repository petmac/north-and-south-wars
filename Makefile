TEMP_DIR := temp
OUT_DIR := $(TEMP_DIR)/game/main
DATA_DIR := $(OUT_DIR)/data

TMX := assets/map.tmx
TSX := assets/tiles.tsx

MAP := $(DATA_DIR)/map.map
TILES_BPL := $(DATA_DIR)/tiles.bpl

TMX2MAP := tools/target/release/tmx2map
TSX2BPL := tools/target/release/tsx2bpl

ASEPRITE := "$(HOME)/Library/Application Support/Steam/steamapps/common/Aseprite/Aseprite.app/Contents/MacOS/aseprite"

FREEIMAGE_PREFIX := $(shell brew --prefix freeimage)
KINGCON := external/kingcon/build/kingcon

.PHONY: all
all: cmake-build $(DATA_DIR)/small_font.BPL $(DATA_DIR)/small_font.PAL $(MAP) $(TILES_BPL)

.PHONY: clean
clean:
	rm -rf $(TEMP_DIR)
	cd tools && cargo clean

.PHONY: cmake-build
cmake-build: $(TEMP_DIR)/build.ninja
	cmake --build --preset amiga

$(TEMP_DIR)/build.ninja: CMakeLists.txt CMakePresets.json
	cmake --preset amiga

$(MAP): $(TMX) $(TMX2MAP)
	mkdir -p $(dir $@)
	$(TMX2MAP) $< $@

$(TILES_BPL): $(TSX) $(TSX2BPL) assets/tiles.png
	mkdir -p $(dir $@)
	$(TSX2BPL) $< $@

$(TMX2MAP) $(TSX2BPL): tools

.PHONY: tools
tools:
	cd tools && cargo build --release

# Convert from .aseprite to .png
$(TEMP_DIR)/assets/%.png: assets/%.aseprite
	$(ASEPRITE) --batch $< --save-as $@

# Convert from .png to .BPL and PAL
# https://github.com/grahambates/kingcon/blob/master/README.md#image-conversion-output-format
$(DATA_DIR)/%.BPL $(DATA_DIR)/%.PAL: $(TEMP_DIR)/assets/%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -RawPalette -Interleaved -Mask=32

# Kingcon
$(KINGCON):
	CPATH="$(FREEIMAGE_PREFIX)/include" LIBRARY_PATH="$(FREEIMAGE_PREFIX)/lib" $(MAKE) --directory external/kingcon
