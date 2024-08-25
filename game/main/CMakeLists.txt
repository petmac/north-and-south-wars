add_executable(
    a.elf
    assets/arrows.h
    assets/menu_bitmaps.h
    assets/palette.h
    assets/small_font.h
    assets/sprite_images.h
    assets/tileset.h
    assets/unit_bitmaps.h
    background.h
    blit.h
    chip.cpp
    chip.h
    copper.h
    draw_campaign_map.cpp
    draw_campaign_map.h
    draw_intro.cpp
    draw_intro.h
    draw_mission.cpp
    draw_mission.h
    draw_title.cpp
    draw_title.h
    draw.cpp
    draw.h
    fast.cpp
    fast.h
    frame_chip.h
    frame_fast.h
    loader.cpp
    main.cpp
    playfield.h
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
