#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "bitmap_utils.h"

int main(int argc, char **argv) {
    const char *filename;
    FILE *fp;
    FILEHEADER fileHeader;
    INFOHEADER infoHeader;
    unsigned char *pixelData;
    int i;

    if (argc < 2) {
        printf("Usage: %s <bitmap_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    filename = argv[1];
    fp = openBitmapFile(filename);
    if (fp == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    readBitmapHeaders(fp, &fileHeader, &infoHeader);
    if (!validateBitmap(&fileHeader)) {
        printf("File is not a valid BMP.\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    printf("Bitmap dimensions: %dx%d\n", infoHeader.width, infoHeader.height);
    printf("Bits per pixel: %d\n", infoHeader.bits);

    pixelData = NULL;
    readPixelData(fp, &infoHeader, &pixelData);

    if (pixelData == NULL) {
        printf("Failed to read pixel data.\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    /* I'm just testing here */
    printf("First 10 pixels (grayscale values): ");
    for (i = 0; i < 10 && i < infoHeader.width * infoHeader.height; i++) {
        printf("%d ", pixelData[i]);
    }
    printf("\n");

    free(pixelData);
    fclose(fp);
    return EXIT_SUCCESS;
}
