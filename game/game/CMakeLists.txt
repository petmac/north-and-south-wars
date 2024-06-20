add_executable(
    a.elf
    chip.cpp
    chip.h
    copper.h
    doynax.cpp
    doynax.h
    frame_chip.h
    game.cpp
    game.h
    loader.cpp
    loader.h
    main.cpp
    )

target_link_libraries(
    a.elf
    PRIVATE
    amiga
    support
    )

add_custom_command(
    TARGET a.elf POST_BUILD
    COMMAND ${ELF2HUNK} a.elf a.exe
    )
