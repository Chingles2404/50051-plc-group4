CC = gcc

# -std=c99: use C99 standard
# -w: suppress all warnings
CFLAGS = -std=c99 -w \
	-I./src \
	-I./src/utilities \
	-I./src/ascii \
	-I./src/edge_detection \
	-I./src/process_bitmap \
	-I./tests/utilities \
	-I./tests/ascii

# Source directories
SOURCE_DIRECTORY = src
TEST_DIRECTORY = tests

# Application source files
SOURCE_FILES = \
	$(SOURCE_DIRECTORY)/main.c \
	$(SOURCE_DIRECTORY)/app_fsm.c \
	$(SOURCE_DIRECTORY)/utilities/fsm_actions.c \
	$(SOURCE_DIRECTORY)/utilities/matrices.c \
	$(SOURCE_DIRECTORY)/ascii/ascii.c \
	$(SOURCE_DIRECTORY)/edge_detection/edge_detection.c \
	$(SOURCE_DIRECTORY)/process_bitmap/bitmap_parser.c

OBJECT_FILES = $(SOURCE_FILES:.c=.o)

# Test source files
TEST_FILES = \
	$(TEST_DIRECTORY)/test_main.c \
	$(TEST_DIRECTORY)/ascii/test_ascii.c \
	$(TEST_DIRECTORY)/utilities/test_matrices.c

TEST_OBJECT_FILES = $(TEST_FILES:.c=.o)

# Targets
APPLICATION_EXECUTABLE = ImageToASCIILineArt.exe
TEST_EXECUTABLE = runAllTests.exe

# Declare phony targets to prevent conflicts with file names
.PHONY: all clean run test

all: $(APPLICATION_EXECUTABLE) $(TEST_EXECUTABLE)

# Link all object files to build the main application
# $^ = all dependencies, $@ = target filename
$(APPLICATION_EXECUTABLE): $(OBJECT_FILES)
	@$(CC) $^ -o $@

# Build test binary
NON_MAIN_OBJECT_FILES = $(filter-out src/main.o, $(OBJECT_FILES))

$(TEST_EXECUTABLE): $(NON_MAIN_OBJECT_FILES) $(TEST_OBJECT_FILES)
	@$(CC) $^ -o $@

# Pattern rule to compile each .c file into its .o object file
# $< = source file, $@ = object file
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Run the main application
run: $(APPLICATION_EXECUTABLE)
	@./$(APPLICATION_EXECUTABLE)

# Run the unit test executable
test: $(TEST_EXECUTABLE)
	@./$(TEST_EXECUTABLE)

# Deletes all .exe and .o files (recursive for objects)
clean:
	@echo Cleaning up object and executable files...
	@del /q /f *.exe 2>nul
	@for /r %%d in (.) do @del /q /f "%%d\*.o" 2>nul
