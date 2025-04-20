#include "app_fsm.h"
#include "fsm_actions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrices.h"

/* Transition table */
static Transition transitions[] = {
    /* Main Menu transitions */
    {STATE_INPUT_MainMenu, ACTION_INPUT, NULL, STATE_GOTO_LoadImage, STATE_INPUT_MainMenu},
    
    /* Load Image path */
    {STATE_GOTO_LoadImage, ACTION_GOTO, NULL, STATE_INPUT_FilePath, STATE_INPUT_MainMenu},
    {STATE_INPUT_FilePath, ACTION_INPUT, NULL, STATE_VALIDATE_Path, STATE_INPUT_FilePath},
    {STATE_VALIDATE_Path, ACTION_CHECK, NULL, STATE_TRANSFORM_Parse, STATE_INPUT_FilePath},
    
    /* Image processing flow */
    {STATE_TRANSFORM_Parse, ACTION_TRANSFORM, NULL, STATE_TRANSFORM_Grayscale, STATE_ERROR_Generic},
    {STATE_TRANSFORM_Grayscale, ACTION_TRANSFORM, NULL, STATE_TRANSFORM_Edge, STATE_ERROR_Generic},
    {STATE_TRANSFORM_Edge, ACTION_TRANSFORM, NULL, STATE_TRANSFORM_Chunk, STATE_ERROR_Generic},
    {STATE_TRANSFORM_Chunk, ACTION_TRANSFORM, NULL, STATE_MAP_ASCII, STATE_ERROR_Generic},
    {STATE_MAP_ASCII, ACTION_MAP, NULL, STATE_DISPLAY_Preview, STATE_ERROR_Generic},
    
    /* Preview and output */
    {STATE_DISPLAY_Preview, ACTION_DISPLAY, NULL, STATE_VALIDATE_Adjust, STATE_ERROR_Generic},
    {STATE_VALIDATE_Adjust, ACTION_CHECK, NULL, STATE_DISPLAY_Adjust, STATE_GOTO_FinalPreview},
    {STATE_DISPLAY_Adjust, ACTION_DISPLAY, NULL, STATE_GOTO_Config, STATE_ERROR_Generic},
    
    /* Configuration menu */
    {STATE_GOTO_Config, ACTION_GOTO, NULL, STATE_INPUT_Config, STATE_INPUT_MainMenu},
    {STATE_INPUT_Config, ACTION_INPUT, NULL, STATE_VALIDATE_Config, STATE_INPUT_Config},
    {STATE_VALIDATE_Config, ACTION_CHECK, NULL, STATE_INPUT_MainMenu, STATE_INPUT_Config},
    
    /* Final output */
    {STATE_GOTO_FinalPreview, ACTION_GOTO, NULL, STATE_OUTPUT_File, STATE_DISPLAY_Terminal},
    {STATE_OUTPUT_File, ACTION_EMIT, NULL, STATE_INPUT_MainMenu, STATE_ERROR_Generic},
    {STATE_DISPLAY_Terminal, ACTION_DISPLAY, NULL, STATE_INPUT_MainMenu, STATE_ERROR_Generic},
    
    /* Error handling */
    {STATE_ERROR_Generic, ACTION_GOTO, NULL, STATE_INPUT_MainMenu, STATE_INPUT_MainMenu}
};


static const int numTransitions = sizeof(transitions) / sizeof(Transition);

void initialiseFSM() {
    for (int i = 0; i < numTransitions; i++) {
        switch (transitions[i].current) {
            case STATE_INPUT_MainMenu:
                transitions[i].actionParam = malloc(sizeof(int));
                break;
                
            case STATE_INPUT_FilePath:
                transitions[i].actionParam = malloc(sizeof(int));
                break;
                
            case STATE_INPUT_Config:
                transitions[i].actionParam = malloc(sizeof(int));
                break;
                
            case STATE_VALIDATE_Adjust:
                transitions[i].actionParam = malloc(sizeof(int));
                break;
                
            case STATE_DISPLAY_Preview:
                transitions[i].actionParam = malloc(sizeof(int));
                break;
                
            case STATE_GOTO_FinalPreview:
                transitions[i].actionParam = malloc(sizeof(int));
                break;
                
            case STATE_OUTPUT_File:
                transitions[i].actionParam = malloc(256 * sizeof(char));
                break;
                
            default:
               
                break;
        }
    }
}

void cleanupFSM() {
    for (int i = 0; i < numTransitions; i++) {
        if (transitions[i].actionParam != NULL) {
            free(transitions[i].actionParam);
        }
    }
}

/* find transition for curr state */
Transition* findTransition(AppState state) {
    for (int i = 0; i < numTransitions; i++) {
        if (transitions[i].current == state) {
            return &transitions[i];
        }
    }
    /* if no transition found, return NULL */
    return NULL;
}

/* execute action for a transition */
ActionStatus executeAction(ActionType actionType, void* param, AppContext* ctx) {
    /* Special case for GOTO action - always succeeds */
    if (actionType == ACTION_GOTO) {
        return ACTION_SUCCESS;
    }
    
    /* get correct action function based on state and action type */
    ActionFunction actionFunc = NULL;
    
    switch (ctx->state) {
        /* Input states */
        case STATE_INPUT_MainMenu:
            actionFunc = actionMainMenu;
            break;
            
        case STATE_INPUT_FilePath:
            actionFunc = actionFilePath;
            break;
            
        case STATE_INPUT_Config:
            actionFunc = actionConfigMenu;
            break;
            
        /* Validation states */
        case STATE_VALIDATE_Path:
            actionFunc = actionValidateFilePath;
            break;
            
        case STATE_VALIDATE_Adjust:
            actionFunc = actionPreviewChoice;
            break;
            
        case STATE_VALIDATE_Config:
            /* Special logic to handle config menu options */
            {
                int choice = *(int*)param;
                if (choice == 1) {
                    actionFunc = actionResolutionConfig;
                } else if (choice == 2) {
                    actionFunc = actionColorModeConfig;
                } else if (choice == 3) {
                    /* Return to main menu */
                    return ACTION_SUCCESS;
                } else {
                    return ACTION_FAILURE;
                }
            }
            break;
            
        /* Transform states */
        case STATE_TRANSFORM_Parse:
            actionFunc = actionParseBitmap;
            break;
            
        case STATE_TRANSFORM_Grayscale:
            actionFunc = actionConvertToGrayscale;
            break;
            
        case STATE_TRANSFORM_Edge:
            actionFunc = actionDetectEdges;
            break;
            
        case STATE_TRANSFORM_Chunk:
            actionFunc = actionChunkImage;
            break;
            
        /* Mapping states */
        case STATE_MAP_ASCII:
            actionFunc = actionMapToAscii;
            break;
            
        /* Display states */
        case STATE_DISPLAY_Preview:
            actionFunc = actionDisplayPreview;
            break;
            
        case STATE_DISPLAY_Adjust:
            /* Redirect to config menu */
            return ACTION_SUCCESS;
            
        case STATE_DISPLAY_Terminal:
            actionFunc = actionDisplayInTerminal;
            break;
            
        /* Output states */
        case STATE_OUTPUT_File:
            actionFunc = actionOutputFileName;
            if (actionFunc(ctx, param) == ACTION_SUCCESS) {
                /* if filename input succeeded, save to that file */
                return actionSaveToFile(ctx, param);
            } else {
                return ACTION_FAILURE;
            }
            break;
            
        /* GOTO states */
        case STATE_GOTO_FinalPreview:
            actionFunc = actionFinalOutputChoice;
            break;
            
        /* Error state */
        case STATE_ERROR_Generic:
            printf("Error: %s\n", ctx->errorMessage ? ctx->errorMessage : "Unknown error");
            printf("Press Enter to return to main menu...");
            clearInputBuffer();
            getchar(); /* wait for user to Enter */
            return ACTION_SUCCESS;
            
        default:
            printf("Error: Unknown state %d\n", ctx->state);
            return ACTION_FAILURE;
    }
    
    if (actionFunc != NULL) {
        return actionFunc(ctx, param);
    } else {
        printf("Error: No action function for state %d\n", ctx->state);
        return ACTION_FAILURE;
    }
}

AppState processState(AppContext* ctx) {
    /*exit state */
    if (ctx->state == STATE_EXIT) {
        return STATE_EXIT;
    }
    
    Transition* transition = findTransition(ctx->state);
    if (transition == NULL) {
        printf("Error: No transition found for state %d\n", ctx->state);
        if (ctx->errorMessage) free(ctx->errorMessage);
        ctx->errorMessage = strdup("No transition found for current state");
        return STATE_ERROR_Generic;
    }
    
    /* main menu */
    if (ctx->state == STATE_INPUT_MainMenu) {
        ActionStatus status = executeAction(transition->action, transition->actionParam, ctx);
        int choice = *(int*)transition->actionParam;
        
        if (status == ACTION_SUCCESS) {
            if (choice == 1) {
                return STATE_GOTO_LoadImage;
            } else if (choice == 2) {
                return STATE_GOTO_Config;
            } else if (choice == 3) {
                return STATE_EXIT;
            } else {
                return STATE_INPUT_MainMenu;
            }
        } else {
            return STATE_INPUT_MainMenu;
        }
    }
    
    /* file path input - handle 'back' command */
    if (ctx->state == STATE_INPUT_FilePath) {
        ActionStatus status = executeAction(transition->action, transition->actionParam, ctx);
        int choice = *(int*)transition->actionParam;
        
        if (status == ACTION_SUCCESS && choice == -1) {
            /* user entered 'back' command */
            return STATE_INPUT_MainMenu;
        } else if (status == ACTION_SUCCESS) {
            return transition->success;
        } else {
            return transition->failure;
        }
    }
    
    /* validate adjust */
    if (ctx->state == STATE_VALIDATE_Adjust) {
        ActionStatus status = executeAction(transition->action, transition->actionParam, ctx);
        int choice = *(int*)transition->actionParam;
        
        if (status == ACTION_SUCCESS) {
            if (choice == 1) {
                return STATE_DISPLAY_Adjust;
            } else {
                return STATE_GOTO_FinalPreview;
            }
        } else {
            return STATE_VALIDATE_Adjust;
        }
    }
    
    /* final preview */
    if (ctx->state == STATE_GOTO_FinalPreview) {
        ActionStatus status = executeAction(transition->action, transition->actionParam, ctx);
        int choice = *(int*)transition->actionParam;
        
        if (status == ACTION_SUCCESS) {
            if (choice == 1) {
                return STATE_OUTPUT_File;
            } else if (choice == 2) {
                return STATE_DISPLAY_Terminal;
            } else if (choice == 3) {
                /* first save, then display */
                AppState nextState = STATE_OUTPUT_File;
                ctx->state = nextState;
                nextState = processState(ctx);
                if (nextState == STATE_ERROR_Generic) {
                    return nextState;
                }
                return STATE_DISPLAY_Terminal;
            } else {
                return STATE_GOTO_FinalPreview;
            }
        } else {
            return STATE_GOTO_FinalPreview;
        }
    }
    
    ActionStatus status = executeAction(transition->action, transition->actionParam, ctx);
    return (status == ACTION_SUCCESS) ? transition->success : transition->failure;
}

AppContext* createAppContext() {
    AppContext* ctx = (AppContext*)malloc(sizeof(AppContext));
    if (ctx == NULL) return NULL;
    
    ctx->state = STATE_INPUT_MainMenu;
    ctx->filename = NULL;
    ctx->parser = NULL;
    ctx->image = NULL;
    ctx->edges = NULL;
    ctx->chunks = NULL;
    ctx->asciiArt = NULL;
    ctx->chunkSize = 3;      /* Default 3x3 resolution */
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