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


REM Run tests using CTest
ctest --output-on-failure


cd ..