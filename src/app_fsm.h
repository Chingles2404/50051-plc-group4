#ifndef APP_FSM_H
#define APP_FSM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap_parser.h"
#include "edge_detection.h"
#include "ascii_main.h"

typedef enum {
    STATE_MAIN_MENU,
    STATE_LOAD_IMAGE,
    STATE_VALIDATE_FILE,
    STATE_PARSE_BMP,
    STATE_IMAGE_LOADED,
    STATE_CONFIG_SETTINGS,
    STATE_SET_RESOLUTION,
    STATE_SET_CHARACTER_SET,
    STATE_SET_COLOR_MODE,
    STATE_PROCESSING_MENU,
    STATE_GRAYSCALE_CONVERSION,
    STATE_EDGE_DETECTION,
    STATE_BRIGHTNESS_MAPPING,
    STATE_ADJUST_THRESHOLD,
    STATE_CHUNK_IMAGE,
    STATE_CHARACTER_MAPPING,
    STATE_QUICK_PREVIEW,
    STATE_ADJUST_MAPPING,
    STATE_FINAL_PREVIEW,
    STATE_SAVE_OUTPUT,
    STATE_DISPLAY_TERMINAL,
    STATE_OUTPUT_COMPLETE,
    STATE_ERROR,
    STATE_EXIT
} AppState;

typedef struct {
    AppState state;
    char* filename;
    BitmapParser* parser;
    Matrix* image;
    Matrix* edges;
    Matrix** chunks;
    char* asciiArt;
    int chunkSize;         /* Resolution setting */
    int characterSet;      /* ASCII set selection */
    int colorMode;         /* Color mode flag */
    int totalChunks;
    int chunkRows;
    int chunkCols;
    char* errorMessage;
} AppContext;

AppContext* createAppContext();
void freeAppContext(AppContext* ctx);
AppState processState(AppContext* ctx);

/* state handlers, i stopped here, need to keep adding */
AppState handleMainMenu(AppContext* ctx);
AppState handleLoadImage(AppContext* ctx);
AppState handleValidateFile(AppContext* ctx);




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

#endif