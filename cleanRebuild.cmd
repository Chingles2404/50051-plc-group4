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

REM Detect compiler and build tool
where make >nul 2>&1
if %errorlevel%==0 (
    set "MAKE_TOOL=make"
    echo Using make
) else (
    where mingw32-make >nul 2>&1
    if %errorlevel%==0 (
        set "MAKE_TOOL=mingw32-make"
        echo Using mingw32-make
    ) else (
        echo No suitable make tool found (neither make nor mingw32-make)
        exit /b 1
    )
)

REM Configure the project with CMake using MinGW Makefiles
cmake -S . -B build -G "MinGW Makefiles"
if errorlevel 1 (
    echo CMake configuration failed.
    exit /b 1
)

REM Build
cd build
%MAKE_TOOL%
if errorlevel 1 (
    echo [ERROR] Build failed.
    exit /b 1
)
cd ..


set INPUT_BMP=sample_inputs\example.bmp
set EXE_PATH=build\ImageToASCIILineArt.exe
set OUTPUT_TXT=build\ImageOutput.txt

REM Assumption is that we are in /build directory. So the path is relative
set TESTS_EXE=tests\runAllTests.exe

set RUN_TESTS=0

if "%RUN_TESTS%" == "1" (
    echo Running tests...
    "%TESTS_EXE%"
)



REM Run the compiled ASCII converter with an input BMP

REM Run the program
if exist %INPUT_BMP% (
    echo Converting image to ASCII art...
    echo ASCII art output will be located at "%OUTPUT_BMP%".


    @REM %EXE_PATH% %INPUT_BMP%
    @REM if errorlevel 1 (
    @REM     echo Conversion failed.
    @REM ) else (
    @REM     echo Conversion done.
    @REM )
    
    
    %EXE_PATH%
) else (
    echo sample_inputs\example.bmp cannot be found!
)


@echo off
endlocal
exit /b 0