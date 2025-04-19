# Combine all tests into one executable
add_executable(runAllTests
    ${CMAKE_SOURCE_DIR}/tests/test_main.c

    ${CMAKE_SOURCE_DIR}/src/utilities/matrices.c
    ${CMAKE_SOURCE_DIR}/tests/utilities/test_matrices.c


    ${CMAKE_SOURCE_DIR}/src/ascii/ascii.c
    ${CMAKE_SOURCE_DIR}/tests/ascii/test_ascii.c
)

target_include_directories(runAllTests PRIVATE

    ${CMAKE_SOURCE_DIR}/src/utilities
    ${CMAKE_SOURCE_DIR}/tests/utilities

    ${CMAKE_SOURCE_DIR}/src/ascii
    ${CMAKE_SOURCE_DIR}/tests/ascii
)

target_link_libraries(runAllTests PRIVATE ascii edgeDetection utilities)

add_test(NAME runAllTests COMMAND runAllTests)