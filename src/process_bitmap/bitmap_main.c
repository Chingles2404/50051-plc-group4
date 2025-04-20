#include <stdio.h>
#include <stdlib.h>
#include "bitmap_parser.h"

int main(int argc, char **argv) {
    
    const char *filename = argv[1];
    BitmapParser* parser;

    if (argc < 2) {
        printf("Usage: %s <bitmap_file>\n", argv[0]);
        return EXIT_FAILURE;
    }


    parser = createBitmapParser();
    if (parser == NULL) {
        fprintf(stderr, "Failed to create bitmap parser\n");
        return EXIT_FAILURE;
    }
    
    printf("Parsing file: %s\n", filename);
    if (!parseFile(parser, filename)) {
        fprintf(stderr, "Error parsing bitmap: %s\n", 
                parser->error_message ? parser->error_message : "Unknown error");
        freeBitmapParser(parser);
        return EXIT_FAILURE;
    }
    
    /* if successfully parsed */
    printf("Successfully parsed bitmap file\n");
    printf("Dimensions: %d x %d\n", parser->infoHeader.width, parser->infoHeader.height);
    printf("Bits per pixel: %d\n", parser->infoHeader.bits);
    printf("File size: %u bytes\n", parser->fileHeader.size);
    
    if (parser->pixelData != NULL) {
        int width;
        int height;
        int i;
        FILE* outFile;
        int y;
        int x;
        int idx;

        width = parser->infoHeader.width;
        height = parser->infoHeader.height;
        
        printf("First 10 pixels (grayscale values): ");
        for (i = 0; i < 10 && i < width * height; i++) {
            printf("%d ", parser->pixelData[i]);
        }
        printf("\n");
        
        /* i think here we would process the grayscale pixel data to create ASCII art */
        /* and implement the edge detection and ASCII mapping */
        
        /* this i just did to test */
        outFile = fopen("ImageOutput.txt", "w");
        if (outFile != NULL) {
            for (y = 0; y < height; y += 2) {  /* Skip rows to maintain aspect ratio */
                for (x = 0; x < width; x++) {
                    idx = y * width + x;
                    if (idx < width * height) {
                        /* simple mapping grayscale values to simple ASCII chars */
                        char symbol;
                        int gray;
                        gray = parser->pixelData[idx];
                        
                        if (gray < 50) symbol = '#';         /* Very dark */
                        else if (gray < 100) symbol = '@';   /* Dark */
                        else if (gray < 150) symbol = '+';   /* Medium */
                        else if (gray < 200) symbol = '.';   /* Light */
                        else symbol = ' ';                   /* Very light */
                        
                        fprintf(outFile, "%c", symbol);
                    }
                }
                fprintf(outFile, "\n");
            }
            fclose(outFile);
            printf("test ASCII representation saved to ImageOutput.txt\n");
        }
    }
    
    freeBitmapParser(parser);
    return EXIT_SUCCESS;
}