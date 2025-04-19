CC = gcc
CFLAGS = -Wall -Werror -I.        # Include current directory (for utils.h)
SRC = main.c utils.c
OUT = edge_detection

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) -lm

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
