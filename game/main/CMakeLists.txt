add_executable(
    a.elf
    blit.h
    chip.cpp
    chip.h
    copper.h
    frame_chip.h
    loader.cpp
    main.cpp
    palette.h
    )

target_link_libraries(
    a.elf
    PRIVATE
    amiga
    game
    support
    )

add_custom_command(
    TARGET a.elf POST_BUILD
    COMMAND ${ELF2HUNK} a.elf a.exe
    )
