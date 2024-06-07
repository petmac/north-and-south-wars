cmake_minimum_required(VERSION 3.23.0)
project("AmiGameJam 2024" ASM C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/AmigaCMakeCrossToolchains/Modules/m68k-amigaos")
enable_language(ASM_VASM)

add_executable(
    game.elf
    hacks.c
    main.cpp
    support/depacker_doynax_vasm.asm
    support/depacker_doynax.s
    support/gcc8_a_support.s
    support/gcc8_c_support.c
    support/gcc8_c_support.h
    )

add_custom_command(
    TARGET game.elf POST_BUILD
    COMMAND ${ELF2HUNK} game.elf game.exe
    )
