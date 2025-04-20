#include "app_fsm.h"
#include "fsm_actions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrices.h"
#include "string_helpers.h"

/* Transition table */
static Transition transitions[] = {
    

    /* Main Menu transitions */
    {STATE_INPUT_MainMenu, ACTION_INPUT, NULL, STATE_GOTO_LoadImage, STATE_INPUT_MainMenu},
    {STATE_INPUT_MainMenu, ACTION_GOTO, NULL, STATE_INPUT_Config, STATE_INPUT_MainMenu},
    
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
    int i;

    for (i = 0; i < numTransitions; i++) {
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
    int i;
    for (i = 0; i < numTransitions; i++) {
        if (transitions[i].actionParam != NULL) {
            free(transitions[i].actionParam);
        }
    }
}

/* find transition for curr state */
Transition* findTransition(AppState state) {
    int i;
    for (i = 0; i < numTransitions; i++) {
        if (transitions[i].current == state) {
            return &transitions[i];
        }
    }
    /* if no transition found, return NULL */
    return NULL;
}

/* execute action for a transition */
ActionStatus executeAction(ActionType actionType, void* param, AppContext* context) {
    ActionFunction actionFunc;

    /* Special case for GOTO action - always succeeds */
    if (actionType == ACTION_GOTO) {
        return ACTION_SUCCESS;
    }
    
    /* get correct action function based on state and action type */
    actionFunc = NULL;
    
    switch (context->state) {
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
            if (actionFunc(context, param) == ACTION_SUCCESS) {
                /* if filename input succeeded, save to that file */
                return actionSaveToFile(context, param);
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
            printf("Error: %s\n", context->errorMessage ? context->errorMessage : "Unknown error");
            printf("Press Enter to return to main menu...");
            clearInputBuffer();
            getchar(); /* wait for user to Enter */
            return ACTION_SUCCESS;
            
        default:
            printf("Error: Unknown state %d\n", context->state);
            return ACTION_FAILURE;
    }
    
    if (actionFunc != NULL) {
        return actionFunc(context, param);
    } else {
        printf("Error: No action function for state %d\n", context->state);
        return ACTION_FAILURE;
    }
}

AppState processState(AppContext* context) {
    Transition* transition;
    ActionStatus status;
    /*exit state */
    if (context->state == STATE_EXIT) {
        return STATE_EXIT;
    }
    
    transition = findTransition(context->state);
    if (transition == NULL) {
        printf("Error: No transition found for state %d\n", context->state);
        if (context->errorMessage) free(context->errorMessage);
        context->errorMessage = stringDuplicate("No transition found for current state");
        
        return STATE_ERROR_Generic;
    }
    
    /* main menu */
    if (context->state == STATE_INPUT_MainMenu) {
        int choice;
        status = executeAction(transition->action, transition->actionParam, context);
        choice = *(int*)transition->actionParam;
        
        if (status == ACTION_SUCCESS) {
            if (choice == 1) {
                return STATE_GOTO_LoadImage;
            } else if (choice == 2) {

                printf("Selected 2");
                return STATE_GOTO_Config;
            } else if (choice == 3) {
                return STATE_EXIT;
            } else {
                return STATE_INPUT_MainMenu;
            }
        }
    }
    
    /* file path input - handle 'back' command */
    if (context->state == STATE_INPUT_FilePath) {
        int choice;
        status = executeAction(transition->action, transition->actionParam, context);
        choice = *(int*)transition->actionParam;
        
        if (status == ACTION_SUCCESS && choice == -1) {
            /* user entered 'back' command */
            return STATE_INPUT_MainMenu;
        } else if (status == ACTION_SUCCESS) {
            return transition->success;
        } else {
            return transition->failure;
        }
    }
    /* file path input - handle 'back' command */
    if (context->state == STATE_INPUT_Config) {
        int choice;
        status = executeAction(transition->action, transition->actionParam, context);
        choice = *(int*)transition->actionParam;
        
        if (status == ACTION_SUCCESS) {

            /**
            printf("1. Set Resolution (current: %dx%d)\n", ctx->chunkSize, ctx->chunkSize);
            printf("2. Set Color Mode (current: %s)\n", 
                ctx->colorMode == 0 ? "Grayscale" : "Color");
            printf("3. Return to Main Menu\n");
             */
            
            switch (choice) {
                case 1:
                    return STATE_VALIDATE_Config;
                case 2:
                    return STATE_DISPLAY_Adjust;
                case 3:
                    return STATE_INPUT_MainMenu;
                default:
                    printf("Invalid choice. Returning to adjustment menu.\n");
                    return STATE_VALIDATE_Adjust;
            }
        } else {
        }
    }

    /* validate adjust */
    if (context->state == STATE_VALIDATE_Adjust) {
        int choice;
        status = executeAction(transition->action, transition->actionParam, context);
        choice = *(int*)transition->actionParam;
        
        if (status == ACTION_SUCCESS) {

            switch (choice) {
                case 1:
                    return STATE_GOTO_Config;
                case 2:
                    return STATE_GOTO_FinalPreview;
                case 3:
                    return STATE_EXIT;
                default:
                    printf("Invalid choice. Returning to adjustment menu.\n");
                    return STATE_VALIDATE_Adjust;
            }
        } else {
            return STATE_VALIDATE_Adjust;
        }
    }
    
    /* final preview */
    if (context->state == STATE_GOTO_FinalPreview) {
        int choice;
        status = executeAction(transition->action, transition->actionParam, context);
        choice = *(int*)transition->actionParam;
        
        if (status == ACTION_SUCCESS) {
            
            switch (choice) {
                case 1:
                    return STATE_OUTPUT_File;
                case 2:
                    return STATE_DISPLAY_Terminal;
                case 3: {
                    /* first save, then display */
                    AppState nextState = STATE_OUTPUT_File;
                    context->state = nextState;
                    nextState = processState(context);
                    if (nextState == STATE_ERROR_Generic) {
                        return nextState;
                    }
                    return STATE_DISPLAY_Terminal;
                }
                default:
                    return STATE_GOTO_FinalPreview;
            }
        } else {
            return STATE_GOTO_FinalPreview;
        }
    }
    status = executeAction(transition->action, transition->actionParam, context);
    if (status == ACTION_SUCCESS) {
        return transition->success;
    } else {
        if (context->errorMessage) {
            printf("Error: %s\n", context->errorMessage);
        }
        return transition->failure;
    }
}

AppContext* createAppContext() {
    AppContext* context;
    context = (AppContext*)malloc(sizeof(AppContext));
    if (context == NULL) return NULL;
    
    context->state = STATE_INPUT_MainMenu;
    context->filename = NULL;
    context->parser = NULL;
    context->image = NULL;
    context->edges = NULL;
    context->chunks = NULL;
    context->asciiArt = NULL;
    context->chunkSize = 3;      /* Default 3x3 resolution */
    context->colorMode = 0;      /* Default grayscale */
    context->totalChunks = 0;
    context->chunkRows = 0;
    context->chunkCols = 0;
    context->errorMessage = NULL;
    
    return context;
}

void freeAppContext(AppContext* context) {
    int i;
    if (context == NULL) return;
    
    if (context->filename) free(context->filename);
    if (context->parser) freeBitmapParser(context->parser);
    if (context->image) freeMatrix(context->image);
    if (context->edges) freeMatrix(context->edges);
    
    if (context->chunks) {
        for (i = 0; i < context->totalChunks; i++) {
            if (context->chunks[i]) freeMatrix(context->chunks[i]);
        }
        free(context->chunks);
    }
    
    if (context->asciiArt) free(context->asciiArt);
    if (context->errorMessage) free(context->errorMessage);
    
    free(context);
}