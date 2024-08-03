cmake_minimum_required(VERSION 3.14)

add_subdirectory(bsp bsp)

include(bsp/toolchain.cmake)

project(hello)

add_executable(hello.elf)

target_sources(hello.elf
    PRIVATE
        hello.c
)

target_include_directories(hello.elf
    PRIVATE
    PUBLIC
)

target_link_libraries(hello.elf
    PRIVATE
        -T "${BspLinkerScript}" -nostdlib
        "${ExtraArchiveLibraries}"
        -Wl,--start-group "${BspLibraryName}" -lc -lstdc++ -lgcc -lm -Wl,--end-group
)

# Create objdump from ELF.
set(objdump hello.elf.objdump)
add_custom_command(
    OUTPUT "${objdump}"
    DEPENDS hello.elf
    COMMAND "${ToolchainObjdump}" "${ToolchainObjdumpFlags}" hello.elf >
            "${objdump}"
    COMMENT "Creating ${objdump}."
    VERBATIM
)
add_custom_target(create-objdump ALL DEPENDS "${objdump}")
