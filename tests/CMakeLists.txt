add_executable(test_main main.c utils/test_utils.c)

target_include_directories(test_main PUBLIC 
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

target_link_libraries(test_main PUBLIC utils)

add_test(NAME test_main COMMAND test_main)