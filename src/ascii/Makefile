CC = gcc
CFLAGS = -Wall -Werror -I.        # Include current directory for headers
SRC = main.c
OUT = ascii_matcher

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) -lm

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
