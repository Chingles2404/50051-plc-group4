
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsm_actions.h"

#include "ascii.h"
#include "edge_detection.h"
#include "bitmap_parser.h"
#include "matrices.h"

#include "string_helpers.h"


ActionStatus actionMainMenu(AppContext* ctx, void* param) {
    int choice;
    printf("\n==== ASCII Art Generator ====\n");
    printf("1. Load Image\n");
    printf("2. Change Resolution\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter a number.\n");
        return ACTION_FAILURE;
    }
    clearInputBuffer();
    
    /* store the user's choice in the context */
    *(int*)param = choice;
    
    if (choice >= 1 && choice <= 3) {
        return ACTION_SUCCESS;
    } else {
        printf("Invalid choice. Please try again.\n");
        return ACTION_FAILURE;
    }
}

ActionStatus actionFilePath(AppContext* ctx, void* param) {
    char buffer[256];
    if (ctx->filename) {
        free(ctx->filename);
        ctx->filename = NULL;
    }
    
    printf("Enter image file path (or type '4' to return to main menu): ");
    if (scanf("%255s", buffer) != 1) {
        clearInputBuffer();
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("Failed to read file path");
        return ACTION_FAILURE;
    }
    clearInputBuffer();
    
    /* check if user wants to go back */
    if (strcmp(buffer, "4") == 0) {
        *(int*)param = -1; 
        return ACTION_SUCCESS;
    }
    
    ctx->filename = stringDuplicate(buffer);
    if (ctx->filename == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("Memory allocation failed");
        return ACTION_FAILURE;
    }
    
    return ACTION_SUCCESS;
}

ActionStatus actionConfigMenu(AppContext* ctx, void* param) {
    int choice;
    int* choiceParam = (int*)param;
    
    printf("\n==== Configuration Menu ====\n");
    printf("1. Set Resolution (current: %dx%d)\n", ctx->chunkSize, ctx->chunkSize);
    printf("2. Return to Main Menu\n");
    printf("Enter your choice: ");
    
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter a number.\n");
        return ACTION_FAILURE;
    }
    clearInputBuffer();
    
    *choiceParam = choice;
    
    if (choice >= 1 && choice <= 2) {
        return ACTION_SUCCESS;
    } else {
        printf("Invalid choice. Please try again.\n");
        return ACTION_FAILURE;
    }
}

ActionStatus actionResolutionConfig(AppContext* ctx, void* param) {
    int size;
    printf("\nSet Resolution\n");
    printf("Enter chunk size (3, 5, or 7): ");
    
    if (scanf("%d", &size) != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter a number.\n");
        return ACTION_FAILURE;
    }
    clearInputBuffer();
    
    if (size != 3 && size != 5 && size != 7) {
        printf("Invalid chunk size. Please choose 3, 5, or 7.\n");
        return ACTION_FAILURE;
    }
    
    ctx->chunkSize = size;
    printf("Resolution set to %dx%d\n", size, size);
    return ACTION_SUCCESS;
}



ActionStatus actionPreviewChoice(AppContext* ctx, void* param) {
    int choice;
    printf("\nPreview of ASCII Art\n");
    printf("1. Adjust settings\n");
    printf("2. Proceed with current result\n");
    printf("3. Quit\n");
    printf("Enter your choice: ");
    
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter a number.\n");
        return ACTION_FAILURE;
    }
    clearInputBuffer();
    
    *(int*)param = choice;
    
    if (choice == 1 || choice == 2 || choice == 3) {
        return ACTION_SUCCESS;
    } else {
        printf("Invalid choice. Please try again.\n");
        return ACTION_FAILURE;
    }
}

ActionStatus actionFinalOutputChoice(AppContext* ctx, void* param) {
    int choice;
    printf("\nOutput Options\n");
    printf("1. Save to file\n");
    printf("2. Display in terminal\n");
    printf("3. Both save and display\n");
    printf("4. Quit\n");
    printf("Enter your choice: ");
    
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input. Please enter a number.\n");
        return ACTION_FAILURE;
    }
    clearInputBuffer();
    
    *(int*)param = choice;
    
    if (choice >= 1 && choice <= 4) {
        return ACTION_SUCCESS;
    } else {
        printf("Invalid choice. Please try again.\n");
        return ACTION_FAILURE;
    }
}

ActionStatus actionOutputFileName(AppContext* ctx, void* param) {
    char buffer[256];
    size_t len;
    char* filename;
    printf("\nEnter filename to save ASCII art (default: output.txt): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("Failed to read filename");
        return ACTION_FAILURE;
    }
    
    len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    /* use default if empty */
    if (buffer[0] == '\0') {
        strcpy(buffer, "output.txt");
    }
    
    /* store the filename in param */
    filename = (char*)param;
    strcpy(filename, buffer);
    
    return ACTION_SUCCESS;
}

/* validation actions */

ActionStatus actionValidateFilePath(AppContext* ctx, void* param) {
    FILE* file;
    if (ctx->filename == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("No file specified");
        return ACTION_FAILURE;
    }
    
    file = fopen(ctx->filename, "rb");
    if (file == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("File not found or cannot be opened");
        return ACTION_FAILURE;
    }
    
    fclose(file);
    return ACTION_SUCCESS;
}

/* Transform Actions */

ActionStatus actionParseBitmap(AppContext* ctx, void* param) {
    if (ctx->parser != NULL) {
        freeBitmapParser(ctx->parser);
    }
    
    ctx->parser = createBitmapParser();
    if (ctx->parser == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("Failed to create bitmap parser");
        return ACTION_FAILURE;
    }
    
    if (!parseFile(ctx->parser, ctx->filename)) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate(ctx->parser->error_message ? 
        ctx->parser->error_message : "Unknown parsing error");
        return ACTION_FAILURE;
    }
    
    return ACTION_SUCCESS;
}

ActionStatus actionConvertToGrayscale(AppContext* ctx, void* param) {
    /* Grayscale conversion is already done in the parser */
    /* But we need to convert it to our matrix format */

    int width;
    int height;
    int x;
    int y;
    int idx;
    
    if (ctx->parser == NULL || ctx->parser->pixelData == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("No bitmap data available");
        return ACTION_FAILURE;
    }
    
    width = ctx->parser->infoHeader.width;
    height = ctx->parser->infoHeader.height;
    
 
    if (ctx->image != NULL) {
        freeMatrix(ctx->image);
    }
    
    /* creating new matrix for the image */
    ctx->image = createMatrix(height, width);
    if (ctx->image == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("Failed to allocate memory for image matrix");
        return ACTION_FAILURE;
    }
    
    /* copy grayscale data to the matrix */
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            idx = y * width + x;
            setMatrixElement(ctx->image, y, x, ctx->parser->pixelData[idx]);
        }
    }
    
    return ACTION_SUCCESS;
}

ActionStatus actionDetectEdges(AppContext* ctx, void* param) {
    if (ctx->image == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("No image data available");
        return ACTION_FAILURE;
    }
    
    if (ctx->edges != NULL) {
        freeMatrix(ctx->edges);
    }
    
    /* Apply edge detection */
    ctx->edges = gradientPipeline(ctx->image);
    if (ctx->edges == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("Edge detection failed");
        return ACTION_FAILURE;
    }
    
    printf("Edge detection completed. Proceeding to image chunking...\n");
    return ACTION_SUCCESS;
}

ActionStatus actionChunkImage(AppContext* ctx, void* param) {
    int i;
    if (ctx->edges == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("No edge data available");
        return ACTION_FAILURE;
    }
    
    /* Calculate chunks */
    ctx->chunkRows = ctx->edges->numberRows / ctx->chunkSize;
    ctx->chunkCols = ctx->edges->numberCols / ctx->chunkSize;
    ctx->totalChunks = ctx->chunkRows * ctx->chunkCols;
    
    if (ctx->chunks != NULL) {
        for (i = 0; i < ctx->totalChunks; i++) {
            if (ctx->chunks[i]) freeMatrix(ctx->chunks[i]);
        }
        free(ctx->chunks);
        ctx->chunks = NULL;
    }
    
    /* Create chunks */
    ctx->chunks = chunkImage(ctx->edges, ctx->chunkSize);
    if (ctx->chunks == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("Failed to chunk image");
        return ACTION_FAILURE;
    }
    
    return ACTION_SUCCESS;
}

/* Map Actions */

ActionStatus actionMapToAscii(AppContext* ctx, void* param) {
    int r;
    int c;
    char ascii;
    if (ctx->chunks == NULL || ctx->totalChunks <= 0) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("No image chunks available");
        return ACTION_FAILURE;
    }
    
    if (ctx->asciiArt != NULL) {
        free(ctx->asciiArt);
    }
    
    /* allocating memory for ASCII art string */
    /* +1 for newline after each row, +1 for final null terminator */
    ctx->asciiArt = (char*)malloc((ctx->chunkRows * (ctx->chunkCols + 1) + 1) * sizeof(char));
    if (ctx->asciiArt == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("Failed to allocate memory for ASCII art");
        return ACTION_FAILURE;
    }
    
    /* mapping each chunk to an ASCII character */
    for (r = 0; r < ctx->chunkRows; r++) {
        for (c = 0; c < ctx->chunkCols; c++) {
            int chunkIndex = r * ctx->chunkCols + c;
            
            Matrix* asciiMatrix = convertToAsciiMatrix(ctx->chunks[chunkIndex]);
            if (asciiMatrix == NULL) {
                ctx->asciiArt[r * (ctx->chunkCols + 1) + c] = '?';
                continue;
            }
            
            ascii = find_best_ascii(asciiMatrix);
            ctx->asciiArt[r * (ctx->chunkCols + 1) + c] = ascii;
            freeMatrix(asciiMatrix);
        }
   
        ctx->asciiArt[r * (ctx->chunkCols + 1) + ctx->chunkCols] = '\n';
    }
    ctx->asciiArt[ctx->chunkRows * (ctx->chunkCols + 1)] = '\0';
    
    return ACTION_SUCCESS;
}

/* Display Actions */

ActionStatus actionDisplayPreview(AppContext* ctx, void* param) {
    if (ctx->asciiArt == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("No ASCII art available");
        return ACTION_FAILURE;
    }
    
    printf("\nASCII Art Preview:\n");
    printf("%s\n", ctx->asciiArt);
    
    return ACTION_SUCCESS;
}

ActionStatus actionDisplayInTerminal(AppContext* ctx, void* param) {

    if (ctx->asciiArt == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("No ASCII art available to display");
        return ACTION_FAILURE;
    }
    
    printf("\n==== ASCII Art ====\n\n");
    printf("%s", ctx->asciiArt);
    
    printf("\nPress Enter to continue...");
    clearInputBuffer();
    getchar(); /* wait for user to Enter */
    
    return ACTION_SUCCESS;
}

/* Output Actions */

ActionStatus actionSaveToFile(AppContext* ctx, void* param) {

    const char* filename = (const char*)param;
    FILE* outFile;

    if (ctx->asciiArt == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("No ASCII art available to save");
        return ACTION_FAILURE;
    }
    
    outFile = fopen(filename, "w");
    if (outFile == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = stringDuplicate("Failed to open output file");
        return ACTION_FAILURE;
    }
    
    fprintf(outFile, "%s", ctx->asciiArt);
    fclose(outFile);
    
    printf("ASCII art saved to %s\n", filename);
    return ACTION_SUCCESS;
}



void clearInputBuffer() {
    /* Since only numeric characters are read, this discards all non-numerics */
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}