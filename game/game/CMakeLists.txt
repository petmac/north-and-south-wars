add_library(
    game
    OBJECT
    include/game/callbacks.h
    include/game/game.h
    include/game/intro.h
    include/game/title.h
    src/game.cpp
    src/intro.cpp
    src/intro.h
    src/title.cpp
    src/title.h
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
