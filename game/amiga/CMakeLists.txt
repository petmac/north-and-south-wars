add_library(
    amiga
    INTERFACE
    include/amiga/custom.h
    )

target_include_directories(
    amiga
    INTERFACE
    include
    )
