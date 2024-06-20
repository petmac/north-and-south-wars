add_library(
    amiga
    OBJECT
    include/amiga/bitmap.h
    include/amiga/custom.h
    include/amiga/libs.h
    include/amiga/mouse.h
    include/amiga/system.h
    src/libs.cpp
    src/mouse.cpp
    src/system.cpp
    )

target_include_directories(
    amiga
    PUBLIC
    include
    )

target_link_libraries(
    amiga
    PUBLIC
    util
    PRIVATE
    support
    )
