TMX := assets/map.tmx
TSX := assets/tiles.tsx
MAP := out/data/map
TMX2MAP := tools/target/release/tmx2map

.PHONY: all
all: build/build.ninja $(MAP)
	cmake --build --preset amiga

.PHONY: clean
clean:
	rm -rf out/
	cd tools && cargo clean

build/build.ninja: CMakeLists.txt CMakePresets.json
	cmake --preset amiga

$(MAP): $(TMX2MAP) $(TMX) $(TSX)
	mkdir -p $(dir $@)
	$(TMX2MAP) $(TMX) $@

$(TMX2MAP): tools

.PHONY: tools
tools:
	cd tools && cargo build --release
