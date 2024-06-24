TEMP_DIR := temp

TMX := assets/map.tmx
TSX := assets/tiles.tsx
MAP := $(TEMP_DIR)/data/map
TMX2MAP := tools/target/release/tmx2map

ASEPRITE := "$(HOME)/Library/Application Support/Steam/steamapps/common/Aseprite/Aseprite.app/Contents/MacOS/aseprite"

FREEIMAGE_PREFIX := $(shell brew --prefix freeimage)
KINGCON := external/kingcon/build/kingcon

.PHONY: all
all: cmake-build $(TEMP_DIR)/assets/small_font.BPL $(TEMP_DIR)/assets/small_font.PAL

.PHONY: clean
clean:
	rm -rf $(TEMP_DIR)
	cd tools && cargo clean

.PHONY: cmake-build
cmake-build: $(TEMP_DIR)/build.ninja
	cmake --build --preset amiga

$(TEMP_DIR)/build.ninja: CMakeLists.txt CMakePresets.json
	cmake --preset amiga

$(MAP): $(TMX2MAP) $(TMX) $(TSX)
	mkdir -p $(dir $@)
	$(TMX2MAP) $(TMX) $@

$(TMX2MAP): tools

.PHONY: tools
tools:
	cd tools && cargo build --release

# Convert from .aseprite to .png
$(TEMP_DIR)/assets/%.png: assets/%.aseprite
	$(ASEPRITE) --batch $< --save-as $@ 

# Convert from .png to .BPL and PAL
# https://github.com/grahambates/kingcon/blob/master/README.md#image-conversion-output-format
%.BPL %.PAL: %.png $(KINGCON)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved -RawPalette

# Kingcon
$(KINGCON):
	CPATH="$(FREEIMAGE_PREFIX)/include" LIBRARY_PATH="$(FREEIMAGE_PREFIX)/lib" $(MAKE) --directory external/kingcon
