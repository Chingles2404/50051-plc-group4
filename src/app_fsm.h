#ifndef APP_FSM_H
#define APP_FSM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap_parser.h"
#include "edge_detection.h"
#include "ascii.h"

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


/* state handlers, i stopped here, need to keep adding */
AppState handleMainMenu(AppContext* ctx);
AppState handleLoadImage(AppContext* ctx);
AppState handleValidateFile(AppContext* ctx);


AppContext* createAppContext();

void freeAppContext(AppContext* ctx);

#endif