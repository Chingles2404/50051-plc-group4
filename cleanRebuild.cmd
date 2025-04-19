@echo off
setlocal

@echo off
REM Remove the existing build directory
if exist build (
    rmdir /s /q build
)
echo Build directory removed.

REM Remove the existing Testing directory
if exist Testing (
    rmdir /s /q Testing
)
echo Testing directory removed.

REM Configure the project with CMake using MinGW Makefiles
cmake -S . -B build -G "MinGW Makefiles"
if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

REM Build the project (assuming you have mingw32-make installed)
cd build
make


set INPUT_BMP=sample_inputs\example.bmp
set EXE_PATH=build\ImageToASCIILineArt.exe
set OUTPUT_BMP=build\ImageOutput.txt

REM Assumption is that we are in /build directory. So the path is relative
set TESTS_EXE=tests\runAllTests.exe

set RUN_TESTS=1

if "%RUN_TESTS%" == "1" (
    echo Running tests...
    "%TESTS_EXE%"
)



REM Run the compiled ASCII converter with an input BMP
cd ..

REM Run the program
if exist %INPUT_BMP% (
    echo Converting image to ASCII art...
    echo ASCII art output will be located at "%OUTPUT_BMP%".
    %EXE_PATH% %INPUT_BMP%

    if errorlevel 1 (
        echo Conversion failed.
    ) else (
        echo Conversion done.
    )
) else (
    echo Input BMP file "%INPUT_BMP%" not found.
)


@echo off
endlocal