#pragma once
#include <stdio.h>

#pragma pack(push, 1)
typedef struct {
    unsigned short type;       /* Identifier: 0x4d42 */
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
    int xresolution, yresolution;    /* /Pixels per meter */
    unsigned int ncolours;           /*  Number of colors */
    unsigned int importantcolours;   /* Important colors */
} INFOHEADER;
#pragma pack(pop)  /* I learnt this from stackoverflow, honestly don't fully understand it myself */

void readBitmapHeaders(FILE *fp, FILEHEADER *fileHeader, INFOHEADER *infoHeader);
void readPixelData(FILE *fp, INFOHEADER *infoHeader, unsigned char **data);
FILE *openBitmapFile(const char *filename);
int validateBitmap(FILEHEADER *fileHeader);