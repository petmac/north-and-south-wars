add_executable(
    a.elf
    blit.h
    chip.cpp
    chip.h
    copper.h
    draw_intro.cpp
    draw_intro.h
    draw_title.cpp
    draw_title.h
    draw.cpp
    draw.h
    fast.cpp
    fast.h
    frame_chip.h
    frame_fast.h
    input.cpp
    loader.cpp
    main.cpp
    palette.h
    text.h
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
