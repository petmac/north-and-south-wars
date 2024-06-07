.PHONY: all
all: build/build.ninja
	cmake --build --preset amiga

.PHONY: clean
clean:
	rm -rf build/

build/build.ninja: CMakeLists.txt CMakePresets.json
	cmake --preset amiga
