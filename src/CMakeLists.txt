set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#[[
The add_library command tells CMake to compile a set of source files into a library target.

In our case, utils is the library. utils.c is the source file.

As utils.c has its own inclusion of headers in the same directory (utils.h), we can opt it out here
]]
add_library(utilities utilities/matrices.c utilities/string_helpers.c)
add_library(edgeDetection edge_detection/edge_detection_main.c edge_detection/edge_detection.c)
add_library(ascii ascii/ascii.c)
add_library(processBitmap process_bitmap/bitmap_main.c process_bitmap/bitmap_parser.c)
add_library(actions utilities/fsm_actions.c)
add_library(appfsm app_fsm.c)
#[[
The following line tells CMake that the target named utils library executable depends on the path ${CMAKE_CURRENT_SOURCE_DIR}/util

Also, if utils were a library that other targets depended on, those dependent targets would also automatically 
inherit the include directories and other compile options due to the PUBLIC scope.

The PUBLIC Keyword:
There are three keywords you can use here: PRIVATE, PUBLIC, and INTERFACE.

PRIVATE: Only the target itself (here, utils) uses these include directories.
INTERFACE: Only the targets that link against this target will use these include directories.
PUBLIC: Means both the target itself and any target that links against it will use these include directories.

]]#
target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utilities)
target_include_directories(edgeDetection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/edge_detection)
target_include_directories(ascii PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ascii)
target_include_directories(processBitmap PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/process_bitmap
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities
)
target_include_directories(actions PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ascii
    ${CMAKE_CURRENT_SOURCE_DIR}/edge_detection
    ${CMAKE_CURRENT_SOURCE_DIR}/process_bitmap
    ${CMAKE_CURRENT_SOURCE_DIR}/utilities
) 

# For fsm_logic.h's import of matrices.h: We can "obtain" it as a proxy from actions.c This works because fsm_logic.h's usage begins in actions.c
target_include_directories(appfsm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Assuming our main application file is main.c, it is compiled to create the ImageToASCIILineArt executable.
add_executable(ImageToASCIILineArt main.c)

# Internal dependencies
target_link_libraries(edgeDetection PUBLIC utilities m)

# This also internally links all of edge's internal dependencies - utilities library
target_link_libraries(ascii PUBLIC edgeDetection)

# This also internally links all of ascii's internal dependencies - edgeDetection + ascii
target_link_libraries(actions PUBLIC ascii)

# This also internally links all of ascii, processBitmap and actions's internal dependencies
target_link_libraries(appfsm PUBLIC ascii processBitmap actions)

# Linking only the FSM pulls in the rest of the internal dependencies as well.
target_link_libraries(ImageToASCIILineArt PUBLIC appfsm)