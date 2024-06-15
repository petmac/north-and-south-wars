add_executable(
    a.elf
    custom.h
    libs.cpp
    libs.h
    main.cpp
    )

target_link_libraries(a.elf PRIVATE support)

add_custom_command(
    TARGET a.elf POST_BUILD
    COMMAND ${ELF2HUNK} a.elf a.exe
    )
