#[[
The add_library command tells CMake to compile a set of source files into a library target.

In our case, utils is the library. utils.c is the source file.

As utils.c has its own inclusion of headers in the same directory (utils.h), we can opt it out here
]]
add_library(utils utils/utils.c)

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
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils)

# Assuming our main application file is main.c, it is compiled to create the ImageToASCIILineArt executable.
add_executable(ImageToASCIILineArt main.c)

# This links the utils library you just added above (add_library()) to your executable you just created above (add_executable())
target_link_libraries(ImageToASCIILineArt PUBLIC utils)