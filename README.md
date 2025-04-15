# 50051 Programming Language Concepts  
# Image to ASCII Line ART

# To Run

Run `cleanRebuild.cmd` (for Windows).

This script cleans up built directories and rebuilds the project. It also runs tests under `./tests`.

The output executable binary file will be located in the folder `./build/build`. The project uses CMake to simplify the building process and organise the code.

# On Adding Features

1. Add a new folder with the feature in src:
   Create a folder such as `./src/newFeatureFolder`.

2. Update `./src/main.c`:  
   Write your desired code and logic here. Remember to import the headers for your feature code.

3. Update `./src/CMakeLists.txt`:
   - Add your library:  
     ```cmake
     add_library(newFeatureNameLibrary newFeatureFolder/newFeature.c)
     ```
     The add_library command tells CMake to compile a set of source files into a library target named `newFeatureNameLibrary`.

   - Specify include directories:  
     ```cmake
     target_include_directories(newFeatureNameLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/newFeatureFolder)
     ```  
     This tells CMake that the target `newFeatureNameLibrary` depends on the path `${CMAKE_CURRENT_SOURCE_DIR}/newFeatureFolder`.

   - Link your library to the main executable:  
     ```cmake
     target_link_libraries(ImageToASCIILineArt PUBLIC newFeatureNameLibrary)
     ```  
     This links the `newFeatureNameLibrary` to the main executable "ImageToASCIILineArt".

4. Run `cleanRebuild.cmd`
--- 