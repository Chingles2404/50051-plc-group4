#include "app_fsm.h"

AppContext* createAppContext() {
    AppContext* ctx = (AppContext*)malloc(sizeof(AppContext));
    if (ctx == NULL) return NULL;
    
    ctx->state = STATE_MAIN_MENU;
    ctx->filename = NULL;
    ctx->parser = NULL;
    ctx->image = NULL;
    ctx->edges = NULL;
    ctx->chunks = NULL;
    ctx->asciiArt = NULL;
    ctx->chunkSize = 3;      /* Default 3x3 resolution */
    ctx->characterSet = 0;   /* Default ASCII set */
    ctx->colorMode = 0;      /* Default grayscale */
    ctx->totalChunks = 0;
    ctx->chunkRows = 0;
    ctx->chunkCols = 0;
    ctx->errorMessage = NULL;
    
    return ctx;
}

void freeAppContext(AppContext* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->filename) free(ctx->filename);
    if (ctx->parser) freeBitmapParser(ctx->parser);
    if (ctx->image) freeMatrix(ctx->image);
    if (ctx->edges) freeMatrix(ctx->edges);
    
    if (ctx->chunks) {
        for (int i = 0; i < ctx->totalChunks; i++) {
            if (ctx->chunks[i]) freeMatrix(ctx->chunks[i]);
        }
        free(ctx->chunks);
    }
    
    if (ctx->asciiArt) free(ctx->asciiArt);
    if (ctx->errorMessage) free(ctx->errorMessage);
    
    free(ctx);
}

AppState processState(AppContext* ctx) {
    switch (ctx->state) {
        case STATE_MAIN_MENU:
            return handleMainMenu(ctx);
        
        case STATE_LOAD_IMAGE:
            return handleLoadImage(ctx);
        
        case STATE_VALIDATE_FILE:
            return handleValidateFile(ctx);
        
        case STATE_PARSE_BMP:
            return handleParseBMP(ctx);
        
        case STATE_IMAGE_LOADED:
            return handleImageLoaded(ctx);
        
        case STATE_CONFIG_SETTINGS:
            return handleConfigSettings(ctx);
        
        case STATE_SET_RESOLUTION:
            return handleSetResolution(ctx);
        
        case STATE_SET_CHARACTER_SET:
            return handleSetCharacterSet(ctx);
        
        case STATE_SET_COLOR_MODE:
            return handleSetColorMode(ctx);
        
        case STATE_PROCESSING_MENU:
            return handleProcessingMenu(ctx);
        
        case STATE_GRAYSCALE_CONVERSION:
            return handleGrayscaleConversion(ctx);
        
        case STATE_EDGE_DETECTION:
            return handleEdgeDetection(ctx);
        
        case STATE_BRIGHTNESS_MAPPING:
            return handleBrightnessMapping(ctx);
        
        case STATE_ADJUST_THRESHOLD:
            return handleAdjustThreshold(ctx);
        
        case STATE_CHUNK_IMAGE:
            return handleChunkImage(ctx);
        
        case STATE_CHARACTER_MAPPING:
            return handleCharacterMapping(ctx);
        
        case STATE_QUICK_PREVIEW:
            return handleQuickPreview(ctx);
        
        case STATE_ADJUST_MAPPING:
            return handleAdjustMapping(ctx);
        
        case STATE_FINAL_PREVIEW:
            return handleFinalPreview(ctx);
        
        case STATE_SAVE_OUTPUT:
            return handleSaveOutput(ctx);
        
        case STATE_DISPLAY_TERMINAL:
            return handleDisplayTerminal(ctx);
        
        case STATE_OUTPUT_COMPLETE:
            return handleOutputComplete(ctx);
        
        case STATE_ERROR:
            return handleError(ctx);
        
        case STATE_EXIT:
            return STATE_EXIT;
        
        default:
            if (ctx->errorMessage) free(ctx->errorMessage);
            ctx->errorMessage = strdup("Unknown state");
            return STATE_ERROR;
    }
}


AppState handleMainMenu(AppContext* ctx) {
    printf("\n==== ASCII Art Generator ====\n");
    printf("1. Load Image\n");
    printf("2. Configure Settings\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return STATE_MAIN_MENU;
    }
    
    switch (choice) {
        case 1:
            return STATE_LOAD_IMAGE;
        case 2:
            return STATE_CONFIG_SETTINGS;
        case 3:
            return STATE_EXIT;
        default:
            printf("Invalid choice. Please try again.\n");
            return STATE_MAIN_MENU;
    }
}

AppState handleLoadImage(AppContext* ctx) {
    if (ctx->filename) {
        free(ctx->filename);
        ctx->filename = NULL;
    }
    
    printf("Enter image file path: ");
    char buffer[256];
    if (scanf("%255s", buffer) != 1) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = strdup("Failed to read file path");
        return STATE_ERROR;
    }
    
    ctx->filename = strdup(buffer);
    if (ctx->filename == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = strdup("Memory allocation failed");
        return STATE_ERROR;
    }
    
    return STATE_VALIDATE_FILE;
}

AppState handleValidateFile(AppContext* ctx) {
    if (ctx->filename == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = strdup("No file specified");
        return STATE_ERROR;
    }
    
    FILE* file = fopen(ctx->filename, "rb");
    if (file == NULL) {
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = strdup("File not found or cannot be opened");
        return STATE_ERROR;
    }
    
    fclose(file);
    return STATE_PARSE_BMP;
}
