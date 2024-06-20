add_library(
    util
    INTERFACE
    include/util/types.h
    )

target_include_directories(
    util
    INTERFACE
    include
    )
