cmake_minimum_required(VERSION 3.12)
set(CMAKE_BUILD_TYPE Debug)
project(app)
file(GLOB SOURCES "src/*.c")
include_directories(src/lib)
add_executable(app ${SOURCES})
add_custom_command(
    TARGET app POST_BUILD
    COMMAND touch main.img
    COMMENT "Creating main.img"
)
add_custom_command(
    TARGET app POST_BUILD
    COMMAND dd if=/dev/zero of=main.img bs=512 count=2880
    COMMENT "Filling main.img with zeros"
)
add_custom_command(
    TARGET app POST_BUILD
    COMMAND mkfs.fat -F12 main.img
    COMMENT "Formatting disk"
)
add_custom_command(
    TARGET app POST_BUILD
    COMMAND mcopy -i main.img ../src/main.c "::main.c"
    COMMENT "Adding main.c to disk"
)
add_custom_command(
    TARGET app POST_BUILD
    COMMAND mcopy -i main.img ../test.txt "::test.txt"
    COMMENT "Adding test.txt to disk"
)
add_custom_command(
    TARGET app POST_BUILD
    COMMAND mcopy -i main.img ../README.md "::readme.md"
    COMMENT "Adding README.md to disk"
)