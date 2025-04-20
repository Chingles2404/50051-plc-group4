#include "bitmap_parser.h"
#include "matrices.h"

#ifndef FSM_TYPES_H
#define FSM_TYPES_H

/* Application States */
typedef enum {
    /* Input states */
    STATE_INPUT_MainMenu,
    STATE_INPUT_FilePath,
    STATE_INPUT_Config,
    
    /* Validation states */
    STATE_VALIDATE_Path,
    STATE_VALIDATE_Adjust,
    STATE_VALIDATE_Config,
    
    /* Transform states */
    STATE_TRANSFORM_Parse,
    STATE_TRANSFORM_Grayscale,
    STATE_TRANSFORM_Edge,
    STATE_TRANSFORM_Chunk,
    
    /* Mapping states */
    STATE_MAP_ASCII,
    
    /* Display states */
    STATE_DISPLAY_Preview,
    STATE_DISPLAY_Adjust,
    STATE_DISPLAY_Terminal,
    
    /* Output states */
    STATE_OUTPUT_File,
    
    /* GOTO states */
    STATE_GOTO_LoadImage,
    STATE_GOTO_Config,
    STATE_GOTO_FinalPreview,
    
    /* Error states */
    STATE_ERROR_Generic,
    
    /* Exit state */
    STATE_EXIT
} AppState;

/* Action Types */
typedef enum {
    ACTION_INPUT,     /* Get input from user */
    ACTION_CHECK,     /* Validate or make a decision */
    ACTION_TRANSFORM, /* Transform the image */
    ACTION_MAP,       /* Map (e.g., matrix to ASCII) */
    ACTION_DISPLAY,   /* Show output to user */
    ACTION_EMIT,      /* Write output to file */
    ACTION_GOTO       /* Navigate to another state */
} ActionType;

/* Action Result Status */
typedef enum {
    ACTION_SUCCESS,   /* Action completed successfully */
    ACTION_FAILURE    /* Action failed */
} ActionStatus;

/* Transition Structure */
typedef struct {
    AppState current;        /* Current state */
    ActionType action;       /* Action to perform */
    void* actionParam;       /* Parameters for the action (if any) */
    AppState success;        /* Next state if action succeeds */
    AppState failure;        /* Next state if action fails */
} Transition;

/* Application Context Structure */
typedef struct {
    AppState state;          /* Current FSM state */
    char* filename;          /* Path to bitmap file */
    BitmapParser* parser;    /* Bitmap parser instance */
    Matrix* image;           /* Grayscale image matrix */
    Matrix* edges;           /* Edge detection result matrix */
    Matrix** chunks;         /* Image chunks for ASCII mapping */
    char* asciiArt;          /* Generated ASCII art string */
    int chunkSize;           /* Resolution setting (3x3, 5x5, etc.) */
    int colorMode;           /* Color output flag */
    int totalChunks;         /* Total number of chunks */
    int chunkRows;           /* Number of chunk rows */
    int chunkCols;           /* Number of chunk columns */
    char* errorMessage;      /* Last error message */
    int resumeAfterConfig; 
} AppContext;

/* Action function type */
typedef ActionStatus (*ActionFunction)(AppContext* ctx, void* param);

#endif /* FSM_TYPES_H */