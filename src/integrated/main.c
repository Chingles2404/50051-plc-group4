#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../process_bitmap/bitmap_parser.h"
#include "../edge_detection/utils.h"
#include "ascii.h"

void convertToAsciiMatrix(struct matrix* edgeMatrix, Matrix asciiMatrix) {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
           /*  Get value from edge detection matrix */
            int val = getMatrixElement(edgeMatrix, y, x);
            
            /*  Normalize to binary (0 or 1) for ASCII matching */
            /* We need to adjust this threshold based on the edge detection output */
            asciiMatrix[y][x] = (val > 128) ? 1 : 0;
        }
    }
}

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
    
    /* at this point, parsing is successful */
    printf("Successfully parsed bitmap file\n");
    printf("Dimensions: %d x %d\n", parser->infoHeader.width, parser->infoHeader.height);
    
    if (parser->pixelData != NULL) {
        int width = parser->infoHeader.width;
        int height = parser->infoHeader.height;
        
        /* convert pizel data to matrix form for edge dectecionn */
        Matrix* image = createMatrix(height, width);
        if (image == NULL) {
            fprintf(stderr, "Failed to create image matrix\n");
            freeBitmapParser(parser);
            return EXIT_FAILURE;
        }
        
        /* fill the matrix with grayscale pixel data */
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int idx = y * width + x;
                setMatrixElement(image, y, x, parser->pixelData[idx]);
            }
        }
        
        /* apply edge detection */
        Matrix* edges = gradientPipeline(image);
        if (edges == NULL) {
            fprintf(stderr, "Edge detection failed\n");
            freeMatrix(image);
            freeBitmapParser(parser);
            return EXIT_FAILURE;
        }
        
        printf("Edge detection completed\n");
     
        int chunkSize = 3; /* For 3x3 ASCII templates */
        int chunkRows = edges->numberRows / chunkSize;
        int chunkCols = edges->numberCols / chunkSize;
        int totalChunks = chunkRows * chunkCols;
    
        Matrix** chunks = chunkImage(edges, chunkSize);
        if (chunks == NULL) {
            fprintf(stderr, "Image chunking failed\n");
            freeMatrix(edges);
            freeMatrix(image);
            freeBitmapParser(parser);
            return EXIT_FAILURE;
        }
    
        printf("Image divided into %d chunks (%d x %d)\n", totalChunks, chunkRows, chunkCols);
        
        char* asciiArt = (char*)malloc((chunkRows * (chunkCols + 1) + 1) * sizeof(char));
        if (asciiArt == NULL) {
            fprintf(stderr, "Failed to allocate memory for ASCII art\n");
            
           /*  Free all resources */
            for (int i = 0; i < totalChunks; i++) {
                freeMatrix(chunks[i]);
            }
            free(chunks);
            freeMatrix(edges);
            freeMatrix(image);
            freeBitmapParser(parser);
            return EXIT_FAILURE;
        }
        
        /* Map each chunk to an ASCII character */
        for (int r = 0; r < chunkRows; r++) {
            for (int c = 0; c < chunkCols; c++) {
                int chunkIndex = r * chunkCols + c;
                
                // /* Convert from edge detection Matrix to ASCII Matrix */
            Matrix asciiMatrix;
            convertToAsciiMatrix(chunks[chunkIndex], asciiMatrix);
            
            /* Call your ASCII mapping function */
            char ascii = find_best_ascii(asciiMatrix);
            
            /*  Store in output array */
            asciiArt[r * (chunkCols + 1) + c] = ascii;
        }
        /* Add newline at the end of each row */
        asciiArt[r * (chunkCols + 1) + chunkCols] = '\n';
    }
    /* added null terminator */
    asciiArt[chunkRows * (chunkCols + 1)] = '\0';
    

    printf("ASCII Art:\n%s\n", asciiArt);
    

    FILE* outFile = fopen("output.txt", "w");
    if (outFile != NULL) {
        fprintf(outFile, "%s", asciiArt);
        fclose(outFile);
        printf("ASCII art saved to output.txt\n");
    } else {
        fprintf(stderr, "Failed to open output file\n");
    }
    
    /* Free all resources */
    free(asciiArt);
    for (int i = 0; i < totalChunks; i++) {
        freeMatrix(chunks[i]);
    }
    free(chunks);
    freeMatrix(edges);
    freeMatrix(image);
    freeBitmapParser(parser);
    
    return EXIT_SUCCESS;
}