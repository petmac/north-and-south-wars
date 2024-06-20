cmake_minimum_required(VERSION 3.23.0)
project("AmiGameJam 2024" ASM C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/AmigaCMakeCrossToolchains/Modules/m68k-amigaos")
enable_language(ASM_VASM)

add_subdirectory(game)
