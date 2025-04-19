cmake_minimum_required(VERSION 3.19)

project(ImageToASCIILineArt)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directory for the binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_compile_options(-Wall -Werror -pedantic)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
