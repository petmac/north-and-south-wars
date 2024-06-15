add_executable(
    a.elf
    copper.cpp
    copper.h
    custom.h
    libs.cpp
    libs.h
    main.cpp
    system.cpp
    system.h
    )

target_link_libraries(a.elf PRIVATE support)

add_custom_command(
    TARGET a.elf POST_BUILD
    COMMAND ${ELF2HUNK} a.elf a.exe
    )
