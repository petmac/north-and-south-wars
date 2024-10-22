TEMP_DIR := temp
OUT_DIR := $(TEMP_DIR)/game/main
DATA_DIR := $(OUT_DIR)/data

# TOP LEVEL

EXE := $(OUT_DIR)/a.exe
BPL_ASSET_NAMES := small_font
MAP := $(DATA_DIR)/mission/map.map

.PHONY: all
all: $(EXE) $(DATA_DIR)/mouse.SPR $(DATA_DIR)/palette.PAL $(foreach name,$(BPL_ASSET_NAMES),$(DATA_DIR)/$(name).BPL) $(MAP) $(DATA_DIR)/mission.chip adf

.PHONY: clean
clean:
	rm -rf $(TEMP_DIR)
	cd tools && cargo clean

# CODE

$(EXE): cmake-build

.PHONY: cmake-build
cmake-build: $(TEMP_DIR)/build.ninja
	cmake --build --preset amiga

$(TEMP_DIR)/build.ninja: CMakeLists.txt CMakePresets.json
	cmake --preset amiga

# DATA

TMX := assets/mission/map.tmx
TSX := assets/mission/tiles.tsx

TMX2MAP := tools/target/release/tmx2map
TSX2BPL := tools/target/release/tsx2bpl

ASEPRITE := "$(HOME)/Library/Application Support/Steam/steamapps/common/Aseprite/Aseprite.app/Contents/MacOS/aseprite"

FREEIMAGE_PREFIX := $(shell brew --prefix freeimage)
KINGCON := external/kingcon/build/kingcon

# Convert tileset image from TSX and .png to .bpl
$(TEMP_DIR)/assets/mission/tiles.bpl: $(TSX) $(TSX2BPL) assets/mission/tiles.png
	mkdir -p $(dir $@)
	$(TSX2BPL) $< $@

# Convert map from Tiled .tmx to .map
$(MAP): $(TMX) $(TMX2MAP)
	mkdir -p $(dir $@)
	$(TMX2MAP) $< $@

# Copy file from temp to data
$(DATA_DIR)/%: $(TEMP_DIR)/assets/%
	cp $< $@

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
$(TEMP_DIR)/assets/mission/encounter/bg_%.BPL: $(TEMP_DIR)/assets/mission/encounter/bg_%.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved

# Convert image from .png to .BPL
%.BPL: %.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved -Mask

# Convert palette from .png to .PAL
%.PAL: %.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -RawPalette

# Convert image from .png to .SPR
%.SPR: %.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=s16

# Convert image from .aseprite to .png
$(TEMP_DIR)/assets/%.png: assets/%.aseprite
	$(ASEPRITE) --batch $< --save-as $@

# ADF

TITLE := PetMacAmiGameJam24
ZIPPED_ADF := $(TEMP_DIR)/adf/$(TITLE).zip
SHRINKLED_EXE := $(TEMP_DIR)/shrinkler/exe.fast.shrinkled

.PHONY: adf
adf: $(ZIPPED_ADF)

$(ZIPPED_ADF): $(TEMP_DIR)/adf/$(TITLE).adf
	zip -9 -j -v $@ $<

%.adf: $(SHRINKLED_EXE) adf/S/startup-sequence
	mkdir -p $(dir $@)
	xdftool $@ format "$(TITLE)" + boot install
	xdftool $@ write adf/S S
	xdftool $@ write $(SHRINKLED_EXE) $(TITLE)
	xdftool $@ write $(OUT_DIR)/data
	xdftool $@ list

$(SHRINKLED_EXE): $(EXE)
	mkdir -p $(dir $@)
	Shrinkler -h -o -1 $(EXE) $@

# TOOLS

# Rust tools
$(TMX2MAP) $(TSX2BPL): tools

.PHONY: tools
tools:
	cd tools && cargo build --release

# Kingcon
$(KINGCON):
	CPATH="$(FREEIMAGE_PREFIX)/include" LIBRARY_PATH="$(FREEIMAGE_PREFIX)/lib" $(MAKE) --directory external/kingcon
