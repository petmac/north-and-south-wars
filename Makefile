TEMP_DIR := temp

TMX := assets/map.tmx
TSX := assets/tiles.tsx
MAP := $(TEMP_DIR)/data/map
TMX2MAP := tools/target/release/tmx2map

.PHONY: all
all: cmake-build

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
