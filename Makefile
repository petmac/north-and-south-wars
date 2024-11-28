TEMP_DIR := temp
TEMP_ASSETS_DIR := $(TEMP_DIR)/assets
OUT_DIR := $(TEMP_DIR)/game/main
DATA_DIR := $(OUT_DIR)/data

# TOP LEVEL

EXE := $(OUT_DIR)/a.exe
MAPS := $(foreach name,0,$(DATA_DIR)/mission/$(name).map)

.SECONDARY:

.PHONY: all
all: $(EXE) \
	$(DATA_DIR)/campaign.chip.lz \
	$(DATA_DIR)/main.chip.lz \
	$(DATA_DIR)/title.chip.lz \
	$(DATA_DIR)/palette.PAL \
	$(MAPS) \
	$(DATA_DIR)/mission.chip.lz

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

TSX := assets/mission/tiles.tsx

TMX2MAP := tools/target/release/tmx2map
TSX2BPL := tools/target/release/tsx2bpl

ASEPRITE := "$(HOME)/Library/Application Support/Steam/steamapps/common/Aseprite/Aseprite.app/Contents/MacOS/aseprite"

FREEIMAGE_PREFIX := $(shell brew --prefix freeimage)
KINGCON := external/kingcon/build/kingcon

LZ := external/doynamite68k/lz

# Convert tileset image from TSX and .png to .bpl
$(TEMP_ASSETS_DIR)/mission/tiles.bpl: $(TSX) $(TSX2BPL) assets/mission/tiles.png
	mkdir -p $(dir $@)
	$(TSX2BPL) $< $@

# Convert map from Tiled .tmx to .map
$(TEMP_ASSETS_DIR)/mission/%.map: assets/mission/%.tmx $(TMX2MAP)
	mkdir -p $(dir $@)
	$(TMX2MAP) $< $@

# Copy file from temp assets to data
$(DATA_DIR)/%: $(TEMP_ASSETS_DIR)/%
	mkdir -p $(dir $@)
	cp $< $@

# Pack file
$(TEMP_ASSETS_DIR)/%.lz: $(TEMP_ASSETS_DIR)/% $(LZ)
	$(LZ) -o $@ $<

# Pack main data
MAIN_CHIP_ASSET_NAMES := \
	mouse.SPR \
	small_font.BPL \
	sounds/cancel.raw \
	sounds/ok.raw
$(TEMP_ASSETS_DIR)/main.chip: $(foreach name,$(MAIN_CHIP_ASSET_NAMES),$(TEMP_ASSETS_DIR)/$(name))
	cat $^ >$@

# Pack title data
TITLE_CHIP_ASSET_NAMES := \
	title/title.BPL
$(TEMP_ASSETS_DIR)/title.chip: $(foreach name,$(TITLE_CHIP_ASSET_NAMES),$(TEMP_ASSETS_DIR)/$(name))
	cat $^ >$@

# Pack campaign data
CAMPAIGN_CHIP_ASSET_NAMES := \
	campaign/campaign.BPL \
	campaign/flags.BPL
$(TEMP_ASSETS_DIR)/campaign.chip: $(foreach name,$(CAMPAIGN_CHIP_ASSET_NAMES),$(TEMP_ASSETS_DIR)/$(name))
	cat $^ >$@

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
	encounter/units.BPL \
	sounds/footsteps.raw \
	sounds/zoom_in.raw \
	sounds/zoom_out.raw \
	encounter/sounds/attack/infantry.raw \
	encounter/sounds/hurt.raw
$(TEMP_ASSETS_DIR)/mission.chip: $(foreach name,$(MISSION_CHIP_ASSET_NAMES),$(TEMP_ASSETS_DIR)/mission/$(name))
	cat $^ >$@

# Convert title image from .png to .BPL
$(TEMP_ASSETS_DIR)/title/title.BPL: $(TEMP_ASSETS_DIR)/title/title.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved

# Convert campaign image from .png to .BPL
$(TEMP_ASSETS_DIR)/campaign/campaign.BPL: $(TEMP_ASSETS_DIR)/campaign/campaign.png $(KINGCON)
	mkdir -p $(dir $@)
	$(KINGCON) $< $(basename $@) -Format=5 -Interleaved

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

# Convert sound from .flac to .raw
$(TEMP_ASSETS_DIR)/%.raw: assets/%.flac
	mkdir -p $(dir $@)
	sox $< --type raw --encoding signed-integer --bits 8 --channels 1 --rate 11025 $@
	truncate -c -s %2 $@

# ADF

TITLE := NorthAndSouthWars
ADF := $(TEMP_DIR)/adf/$(TITLE).adf
ZIPPED_ADF := $(TEMP_DIR)/adf/$(TITLE).zip
SHRINKLED_EXE := $(TEMP_DIR)/shrinkler/exe.fast.shrinkled

.PHONY: adf
adf: $(ZIPPED_ADF)

$(ZIPPED_ADF): $(ADF)
	zip -9 -j -v $@ $<

$(ADF): $(SHRINKLED_EXE) adf/S/startup-sequence
	mkdir -p $(dir $@)
	xdftool $@ format "$(TITLE)" + boot install
	xdftool $@ write adf/S S
	xdftool $@ write $(SHRINKLED_EXE) $(TITLE)
	xdftool $@ write $(OUT_DIR)/data
	xdftool $@ list

$(SHRINKLED_EXE): $(EXE)
	mkdir -p $(dir $@)
	Shrinkler -h -o -1 $(EXE) $@

# DISTRIBUTION

README := dist/ReadMe.txt
DIST_ZIP := $(TEMP_DIR)/dist/$(TITLE).zip
BUTLER := external/butler-darwin-amd64/butler

.PHONY: itch
itch: $(DIST_ZIP)
	$(BUTLER) push $< petmac/north-and-south-wars:dev

$(DIST_ZIP): $(ADF) $(README)
	mkdir -p $(dir $@)
	zip -9 -j -v $@ $^

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
	gcc -O2 $^ -o $@
