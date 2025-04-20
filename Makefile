# Compiler and Flags
COMPILER = gcc

COMPILER_FLAGS = -std=c99 -Wall -Werror -ansi -pedantic \
	-I./src \
	-I./src/utilities \
	-I./src/ascii \
	-I./src/edge_detection \
	-I./src/process_bitmap \
	-I./tests/utilities \
	-I./tests/ascii

# Source Directories
APPLICATION_SOURCE_DIRECTORY = src
UNIT_TEST_SOURCE_DIRECTORY = tests

# Application Source Files
APPLICATION_SOURCE_FILES = \
	$(APPLICATION_SOURCE_DIRECTORY)/main.c \
	$(APPLICATION_SOURCE_DIRECTORY)/app_fsm.c \
	$(APPLICATION_SOURCE_DIRECTORY)/utilities/fsm_actions.c \
	$(APPLICATION_SOURCE_DIRECTORY)/utilities/matrices.c \
	$(APPLICATION_SOURCE_DIRECTORY)/ascii/ascii.c \
	$(APPLICATION_SOURCE_DIRECTORY)/edge_detection/edge_detection.c \
	$(APPLICATION_SOURCE_DIRECTORY)/process_bitmap/bitmap_parser.c \
	$(APPLICATION_SOURCE_DIRECTORY)/utilities/string_helpers.c

APPLICATION_OBJECT_FILES = $(APPLICATION_SOURCE_FILES:.c=.o)

# Unit Test Source Files
UNIT_TEST_FILES = \
	$(UNIT_TEST_SOURCE_DIRECTORY)/test_main.c \
	$(UNIT_TEST_SOURCE_DIRECTORY)/ascii/test_ascii.c \
	$(UNIT_TEST_SOURCE_DIRECTORY)/utilities/test_matrices.c

UNIT_TEST_OBJECT_FILES = $(UNIT_TEST_FILES:.c=.o)

ifeq ($(OS),Windows_NT)
    EXECUTABLE_EXTENSION = .exe # Windows has .exe
else
    EXECUTABLE_EXTENSION = # Linux is empty
endif

# Executables
APPLICATION_EXECUTABLE = ImageToASCIILineArt$(EXECUTABLE_EXTENSION)
UNIT_TEST_EXECUTABLE = runAllTests$(EXECUTABLE_EXTENSION)

# Targets
.PHONY: all clean run test

all: $(APPLICATION_EXECUTABLE) $(UNIT_TEST_EXECUTABLE)

$(APPLICATION_EXECUTABLE): $(APPLICATION_OBJECT_FILES)
	@$(COMPILER) $^ -o $@ -lm

APPLICATION_OBJECT_FILES_WITHOUT_MAIN = $(filter-out src/main.o, $(APPLICATION_OBJECT_FILES))

$(UNIT_TEST_EXECUTABLE): $(APPLICATION_OBJECT_FILES_WITHOUT_MAIN) $(UNIT_TEST_OBJECT_FILES)
	@$(COMPILER) $^ -o $@ -lm

%.o: %.c
	@$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

run: $(APPLICATION_EXECUTABLE)
	@./$(APPLICATION_EXECUTABLE)

test: $(UNIT_TEST_EXECUTABLE)
	@./$(UNIT_TEST_EXECUTABLE)

clean:
	@echo Cleaning up object files, executable files, and build directory...
ifeq ($(OS),Windows_NT)
	@del /q /f ImageToASCIILineArt runAllTests 2>nul || rem
	@del /q /f *.exe 2>nul || rem
	@for /r %%d in (.) do @del /q /f "%%d\\*.o" 2>nul || rem
	@if exist build rmdir /s /q build
else
	@find . -type f -name "*.o" -delete
	@find . -type f -name "*.exe" -delete
	@rm -f ImageToASCIILineArt runAllTests
	@rm -rf build
endif