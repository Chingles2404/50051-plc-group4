# 50051 Programming Language Concepts  
# Image to ASCII Line ART

This project transforms bitmap images into ASCII line art using a configurable image-processing pipeline written in C.

It supports:
- Edge detection
- Custom chunk resolutions
- Terminal or file-based output

---

### Building the Project

You may build this project in two ways: using **Makefile** (cross-platform) or **CMake**.

---

### Building using Makefile (Cross-Platform)

In the root directory, run:

`make`

This will:
- Compile all source files in `./src`
- Compile test files in `./tests`
- Output two binaries in the root directory:
  
  1. `ImageToASCIILineArt.exe`
   
  2. `runAllTests.exe`


`ImageToASCIILineArt.exe` runs the main program.

`runAllTests.exe` runs the tests for our program.


To remove all .exe and .o files: `make clean`

Note: you may have to run `make clean` before `make`.

### Run the Executable with Makefile
After a successful build, the output binary will be located at:

`./ImageToASCIILineArt.exe`

To run the program on Windows:

`.\ImageToASCIILineArt.exe`

To run the program on macOS/Linux (or WSL):

`./ImageToASCIILineArt`


### Building using CMake

A Windows batch script, `cleanRebuild.cmd`, is provided to simplify the build process.

On **Windows** in the root directory of the project, open **Command Prompt (CMD)** and run: 

`cleanRebuild.cmd`

This will:

1. Clean any existing build files
2. Rebuild the project into a new `./build` directory
3. Automatically run tests (can be disabled in the script)
4. Run the executable (see next section) automatically with predefined files (image file paths in sample_inputs)

We used `./sample_inputs/example.bmp` as the predefined image file.

The output of the executable will be located in the `./build` directory, named `ImageOutput.txt`

### Run the Executable with CMake
After a successful build, the output binary will be located at:

`./build/ImageToASCIILineArt.exe`

To run the program on Windows:

`.\build\ImageToASCIILineArt.exe`

To run the program on macOS/Linux (or WSL):

`./build/ImageToASCIILineArt`

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