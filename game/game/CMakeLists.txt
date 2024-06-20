add_executable(
    a.elf
    chip.cpp
    chip.h
    copper.h
    custom.h
    doynax.cpp
    doynax.h
    frame_chip.h
    game.cpp
    game.h
    libs.cpp
    libs.h
    loader.cpp
    loader.h
    main.cpp
    mouse.cpp
    mouse.h
    system.cpp
    system.h
    )

target_link_libraries(
    a.elf
    PRIVATE
    support
    util
    )

add_custom_command(
    TARGET a.elf POST_BUILD
    COMMAND ${ELF2HUNK} a.elf a.exe
    )
