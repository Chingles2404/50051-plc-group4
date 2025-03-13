add_executable(test_utils test_utils_main.c utils/test_utils.c)

target_include_directories(test_utils PUBLIC 
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

target_link_libraries(test_utils PUBLIC utils)

add_test(NAME utils COMMAND test_utils)