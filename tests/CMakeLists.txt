add_executable(test_utils
    test_utils_main.c
    utils/test_utils.c
    ${CMAKE_SOURCE_DIR}/src/edge_detection/edge_detection_utils.c
)

target_include_directories(test_utils PRIVATE
    ${CMAKE_SOURCE_DIR}/src/edge_detection
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

add_test(NAME utils COMMAND test_utils)