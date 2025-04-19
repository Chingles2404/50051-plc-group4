#include <stdio.h>
#include <stdlib.h> 
#include "bitmap.h"

FILE *openBitmapFile(const char *filename) {
    FILE *fp = fopen(filename, "rb"); /* Open file in binary mode */
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;
    }
    return fp;
}

int validateBitmap(FILEHEADER *fileHeader) {
    if (fileHeader->type != 0x4D42) {  /* Check 'BM' in header for BMP files */
        printf("Not a valid BMP file.\n");
        return 0;
    }
    return 1;
}

void readBitmapHeaders(FILE *fp, FILEHEADER *fileHeader, INFOHEADER *infoHeader) {
    fread(fileHeader, sizeof(FILEHEADER), 1, fp);
    fread(infoHeader, sizeof(INFOHEADER), 1, fp);
}

void readPixelData(FILE *fp, INFOHEADER *infoHeader, unsigned char **data) {
    int width = infoHeader->width;
    int height = infoHeader->height;
    int bytesPerPixel = infoHeader->bits / 8;
    int row_padded = (width * bytesPerPixel + 3) & (~3);
    unsigned char *row = malloc(row_padded);
    unsigned char *img = malloc(width * height);

    fseek(fp, sizeof(FILEHEADER) + sizeof(INFOHEADER), SEEK_SET);
    for (int i = 0; i < height; i++) {
        fread(row, sizeof(unsigned char), row_padded, fp);
        for (int j = 0; j < width * bytesPerPixel; j += bytesPerPixel) {
            int gray = (row[j] * 0.11) + (row[j + 1] * 0.59) + (row[j + 2] * 0.3);   /* translating the RGB color values of each pixel into a single grayscale value using a luminance formula */
            img[(i * width) + j / bytesPerPixel] = gray;
        }
    }
    *data = img;
    free(row);
}
