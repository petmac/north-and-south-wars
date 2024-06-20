add_library(
    support
    OBJECT
    depacker_doynax_vasm.asm
    depacker_doynax.s
    gcc8_a_support.s
    gcc8_c_support.c
    gcc8_c_support.h
    )

target_include_directories(
    support
    PUBLIC
    .
    )
