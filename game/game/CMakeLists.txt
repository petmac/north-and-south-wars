add_library(
    game
    OBJECT
    include/game/callbacks.h
    include/game/game.h
    include/game/intro.h
    src/game.cpp
    src/intro.cpp
    src/intro.h
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
