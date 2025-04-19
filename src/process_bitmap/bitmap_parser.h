#pragma once
#include <stdio.h>
#include <stdbool.h>

#pragma pack(push, 1)
typedef struct {
    unsigned short type;       /* Identifier: BM */
    unsigned int size;         /* File size in bytes */
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;       /* Offset to image data in bytes */
} FILEHEADER;

typedef struct {
    unsigned int size;               /* Header size in bytes */
    int width, height;               /* Width and height of image */
    unsigned short planes;           /* Number of color planes */
    unsigned short bits;             /* Bits per pixel */
    unsigned int compression;        /* Compression type */
    unsigned int imagesize;          /* Image size in bytes */
    int xresolution, yresolution;    /* Pixels per meter */
    unsigned int ncolours;           /* Number of colors */
    unsigned int importantcolours;   /* Important colors */
} INFOHEADER;
#pragma pack(pop)

/* Parser States */
typedef enum {
    STATE_INIT,                /* Initial state */
    STATE_READING_FILE_HEADER, /* Reading BMP file header */
    STATE_READING_INFO_HEADER, /* Reading BMP info header */
    STATE_READING_PIXEL_DATA,  /* Reading pixel data */
    STATE_COMPLETED,           /* Parsing completed successfully */
    STATE_ERROR                /* Error state */
} ParserState;

/* Error Codes */
typedef enum {
    ERROR_NONE,               /* No error */
    ERROR_FILE_NOT_FOUND,     /* File not found or cannot be opened */
    ERROR_INVALID_BMP,        /* Not a valid BMP file */
    ERROR_UNSUPPORTED_FORMAT, /* Unsupported BMP format */
    ERROR_MEMORY_ALLOCATION,  /* Memory allocation failed */
    ERROR_READING_FILE,       /* Error reading from file */
    ERROR_UNEXPECTED_EOF      /* Unexpected end of file */
} ErrorCode;

/* Parser Context Structure */
typedef struct {
    FILE* file;                /* File pointer */
    ParserState state;         /* Current state */
    ErrorCode error;           /* Error code if any */
    char* error_message;       /* Detailed error message */
    FILEHEADER fileHeader;     /* BMP file header */
    INFOHEADER infoHeader;     /* BMP info header */
    unsigned char* pixelData;  /* Pixel data (grayscale) */
    int current_position;      /* Current position in file */
} BitmapParser;


BitmapParser* createBitmapParser();
void freeBitmapParser(BitmapParser* parser);
bool parseFile(BitmapParser* parser, const char* filename);
ParserState processState(BitmapParser* parser);


ParserState handleInitState(BitmapParser* parser, const char* filename);
ParserState handleFileHeaderState(BitmapParser* parser);
ParserState handleInfoHeaderState(BitmapParser* parser);
ParserState handlePixelDataState(BitmapParser* parser);
ParserState handleErrorState(BitmapParser* parser);

bool isGrayscale(INFOHEADER* infoHeader);
void convertToGrayscale(unsigned char* pixels, int width, int height, int bytesPerPixel);
void setError(BitmapParser* parser, ErrorCode code, const char* message);