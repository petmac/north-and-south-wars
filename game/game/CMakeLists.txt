add_library(
    game
    OBJECT
    include/game/callbacks.h
    include/game/game.h
    src/game.cpp
    )

target_include_directories(
    game
    PUBLIC
    include
    )

target_link_libraries(
    game
    PUBLIC
    util
    )
