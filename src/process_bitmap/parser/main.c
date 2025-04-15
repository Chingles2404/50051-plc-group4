#include <stdio.h>
#include <stdlib.h>
#include "bitmap_parser.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <bitmap_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    
    BitmapParser* parser = createBitmapParser();
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
        int width = parser->infoHeader.width;
        int height = parser->infoHeader.height;
        
        printf("First 10 pixels (grayscale values): ");
        for (int i = 0; i < 10 && i < width * height; i++) {
            printf("%d ", parser->pixelData[i]);
        }
        printf("\n");
        
        /* i think here we would process the grayscale pixel data to create ASCII art */
        /* and implement the edge detection and ASCII mapping */
        
        /* this i just did to test */
        FILE* outFile = fopen("output.txt", "w");
        if (outFile != NULL) {
            for (int y = 0; y < height; y += 2) {  /* Skip rows to maintain aspect ratio */
                for (int x = 0; x < width; x++) {
                    int idx = y * width + x;
                    if (idx < width * height) {
                        /* simple mapping grayscale values to simple ASCII chars */
                        char symbol;
                        int gray = parser->pixelData[idx];
                        
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
            printf("test ASCII representation saved to output.txt\n");
        }
    }
    
    freeBitmapParser(parser);
    return EXIT_SUCCESS;
}