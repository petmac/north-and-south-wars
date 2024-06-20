add_library(
    amiga
    OBJECT
    include/amiga/bitmap.h
    include/amiga/custom.h
    include/amiga/mouse.h
    src/mouse.cpp
    )

target_include_directories(
    amiga
    PUBLIC
    include
    )

target_link_libraries(
    amiga
    PRIVATE
    util
    )
