CC = gcc
CFLAGS = -Wall -Werror -pedantic
OBJS = main.o bitmap_parser.o

all: bmp2ascii

bmp2ascii: $(OBJS)
	$(CC) $(CFLAGS) -o bmp2ascii.exe $(OBJS)

main.o: main.c bitmap_parser.h
	$(CC) $(CFLAGS) -c main.c

bitmap_parser.o: bitmap_parser.c bitmap_parser.h
	$(CC) $(CFLAGS) -c bitmap_parser.c

clean:
	$(RM) *.o bmp2ascii.exe output.txt