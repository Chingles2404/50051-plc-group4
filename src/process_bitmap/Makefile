CC = gcc
CFLAGS = -Wall -Werror -I.        # Includes current folder (where bitmap.h is)

SRC = main.c bitmap.c utils.c
OUT = bitmap_processor

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT) example.bmp

clean:
	rm -f $(OUT)
