#include "app_fsm.h"
#include "fsm_actions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrices.h"

/* Transition table */
static Transition transitions[] = {
    {STATE_INPUT_MainMenu, ACTION_INPUT, NULL, STATE_GOTO_LoadImage, STATE_INPUT_MainMenu},
    {STATE_INPUT_MainMenu, ACTION_GOTO, NULL, STATE_INPUT_Config, STATE_INPUT_MainMenu},

    {STATE_GOTO_LoadImage, ACTION_GOTO, NULL, STATE_INPUT_FilePath, STATE_INPUT_MainMenu},
    {STATE_INPUT_FilePath, ACTION_INPUT, NULL, STATE_VALIDATE_Path, STATE_INPUT_FilePath},
    {STATE_VALIDATE_Path, ACTION_CHECK, NULL, STATE_TRANSFORM_Parse, STATE_INPUT_FilePath},

    {STATE_TRANSFORM_Parse, ACTION_TRANSFORM, NULL, STATE_TRANSFORM_Grayscale, STATE_ERROR_Generic},
    {STATE_TRANSFORM_Grayscale, ACTION_TRANSFORM, NULL, STATE_TRANSFORM_Edge, STATE_ERROR_Generic},
    {STATE_TRANSFORM_Edge, ACTION_TRANSFORM, NULL, STATE_TRANSFORM_Chunk, STATE_ERROR_Generic},
    {STATE_TRANSFORM_Chunk, ACTION_TRANSFORM, NULL, STATE_MAP_ASCII, STATE_ERROR_Generic},
    {STATE_MAP_ASCII, ACTION_MAP, NULL, STATE_DISPLAY_Preview, STATE_ERROR_Generic},

    {STATE_DISPLAY_Preview, ACTION_DISPLAY, NULL, STATE_VALIDATE_Adjust, STATE_ERROR_Generic},
    {STATE_VALIDATE_Adjust, ACTION_CHECK, NULL, STATE_DISPLAY_Adjust, STATE_GOTO_FinalPreview},
    {STATE_DISPLAY_Adjust, ACTION_DISPLAY, NULL, STATE_INPUT_Config, STATE_ERROR_Generic},

    {STATE_INPUT_Config, ACTION_INPUT, NULL, STATE_VALIDATE_Config, STATE_INPUT_Config},
    {STATE_VALIDATE_Config, ACTION_CHECK, NULL, STATE_INPUT_Config, STATE_INPUT_Config},

    {STATE_GOTO_FinalPreview, ACTION_GOTO, NULL, STATE_OUTPUT_File, STATE_DISPLAY_Terminal},
    {STATE_OUTPUT_File, ACTION_EMIT, NULL, STATE_INPUT_MainMenu, STATE_ERROR_Generic},
    {STATE_DISPLAY_Terminal, ACTION_DISPLAY, NULL, STATE_INPUT_MainMenu, STATE_ERROR_Generic},

    {STATE_ERROR_Generic, ACTION_GOTO, NULL, STATE_INPUT_MainMenu, STATE_INPUT_MainMenu}
};

static const int numTransitions = sizeof(transitions) / sizeof(Transition);

void initialiseFSM() {
    for (int i = 0; i < numTransitions; i++) {
        if (transitions[i].actionParam != NULL) {
            free(transitions[i].actionParam);
            transitions[i].actionParam = NULL;
        }
        switch (transitions[i].current) {
            case STATE_INPUT_MainMenu:
            case STATE_INPUT_FilePath:
            case STATE_INPUT_Config:
            case STATE_VALIDATE_Adjust:
            case STATE_DISPLAY_Preview:
            case STATE_VALIDATE_Config:
                transitions[i].actionParam = malloc(sizeof(int));
                break;
            case STATE_GOTO_FinalPreview:
                transitions[i].actionParam = malloc(sizeof(int));
                if (transitions[i].actionParam)
                    *(int*)transitions[i].actionParam = 0;
                break;
            case STATE_OUTPUT_File:
                transitions[i].actionParam = malloc(256 * sizeof(char));
                if (transitions[i].actionParam)
                    ((char*)transitions[i].actionParam)[0] = '\0';
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

Transition* findTransition(AppState state) {
    for (int i = 0; i < numTransitions; i++) {
        if (transitions[i].current == state) {
            return &transitions[i];
        }
    }
    return NULL;
}

ActionStatus executeAction(ActionType actionType, void* param, AppContext* ctx) {
    printf("Executing action for state %d\n", ctx->state);

    if (actionType == ACTION_GOTO) return ACTION_SUCCESS;

    ActionFunction actionFunc = NULL;

    if (ctx->state == STATE_VALIDATE_Config) {
        int choice = *(int*)param;
        printf("Handling config validation for choice: %d\n", choice);
        if (choice == 1) return actionResolutionConfig(ctx, param);
        else if (choice == 2) return actionColorModeConfig(ctx, param);
        else if (choice == 3) return ACTION_SUCCESS;
        else return ACTION_FAILURE;
    }

    switch (ctx->state) {
        case STATE_INPUT_MainMenu: actionFunc = actionMainMenu; break;
        case STATE_INPUT_FilePath: actionFunc = actionFilePath; break;
        case STATE_INPUT_Config: actionFunc = actionConfigMenu; break;
        case STATE_VALIDATE_Path: actionFunc = actionValidateFilePath; break;
        case STATE_VALIDATE_Adjust: actionFunc = actionPreviewChoice; break;
        case STATE_TRANSFORM_Parse: actionFunc = actionParseBitmap; break;
        case STATE_TRANSFORM_Grayscale: actionFunc = actionConvertToGrayscale; break;
        case STATE_TRANSFORM_Edge: actionFunc = actionDetectEdges; break;
        case STATE_TRANSFORM_Chunk: actionFunc = actionChunkImage; break;
        case STATE_MAP_ASCII: actionFunc = actionMapToAscii; break;
        case STATE_DISPLAY_Preview: actionFunc = actionDisplayPreview; break;
        case STATE_DISPLAY_Adjust: return ACTION_SUCCESS;
        case STATE_DISPLAY_Terminal: actionFunc = actionDisplayInTerminal; break;
        case STATE_OUTPUT_File:
            if (actionOutputFileName(ctx, param) == ACTION_SUCCESS)
                return actionSaveToFile(ctx, param);
            else return ACTION_FAILURE;
        case STATE_GOTO_FinalPreview: actionFunc = actionFinalOutputChoice; break;
        case STATE_ERROR_Generic:
            printf("Error: %s\n", ctx->errorMessage ? ctx->errorMessage : "Unknown error");
            printf("Press Enter to return to main menu...");
            clearInputBuffer(); getchar();
            return ACTION_SUCCESS;
        default:
            printf("Error: No action function for state %d\n", ctx->state);
            return ACTION_FAILURE;
    }

    if (actionFunc != NULL) {
        ActionStatus result = actionFunc(ctx, param);
        printf("Action function for state %d result: %s\n", ctx->state, result == ACTION_SUCCESS ? "success" : "failure");
        return result;
    }
    return ACTION_FAILURE;
}

AppState processState(AppContext* ctx) {
    if (ctx->state == STATE_EXIT) return STATE_EXIT;

    AppState currentState = ctx->state;
    Transition* transition = findTransition(ctx->state);
    if (!transition) return STATE_ERROR_Generic;

    ActionStatus status = executeAction(transition->action, transition->actionParam, ctx);
    if (currentState != ctx->state) return ctx->state;

    if (ctx->state == STATE_INPUT_MainMenu) {
        int choice = *(int*)transition->actionParam;
        if (status == ACTION_SUCCESS) {
            if (choice == 1) return STATE_GOTO_LoadImage;
            else if (choice == 2) return STATE_INPUT_Config;
            else if (choice == 3) return STATE_EXIT;
        }
        return STATE_INPUT_MainMenu;
    }

    if (ctx->state == STATE_INPUT_FilePath) {
        int choice = *(int*)transition->actionParam;
        if (status == ACTION_SUCCESS && choice == -1) return STATE_INPUT_MainMenu;
        else if (status == ACTION_SUCCESS) return transition->success;
        return transition->failure;
    }

    if (ctx->state == STATE_INPUT_Config) {
        int choice = *(int*)transition->actionParam;
        Transition* next = findTransition(STATE_VALIDATE_Config);
        if (next && next->actionParam) *(int*)next->actionParam = choice;
        return STATE_VALIDATE_Config;
    }

    if (ctx->state == STATE_VALIDATE_Config) {
        int choice = *(int*)transition->actionParam;
        if (status == ACTION_SUCCESS) {
            if (choice == 3) {
                // If user came from adjustment, reprocess. Otherwise, return to main.
                return (ctx->resumeAfterConfig) ? STATE_TRANSFORM_Grayscale : STATE_INPUT_MainMenu;
            } else {
                return (ctx->resumeAfterConfig) ? STATE_TRANSFORM_Grayscale : STATE_INPUT_Config;
            }
        }
        return STATE_INPUT_Config;
    }

    if (ctx->state == STATE_VALIDATE_Adjust) {
        int choice = *(int*)transition->actionParam;
        if (status == ACTION_SUCCESS) {
            if (choice == 1) {
                ctx->resumeAfterConfig = 1;  // flag to resume processing after config
                return STATE_INPUT_Config;
            } else if (choice == 2) {
                ctx->resumeAfterConfig = 0;
                return STATE_GOTO_FinalPreview;
            } else if (choice == 3) return STATE_EXIT;
        }
        return STATE_VALIDATE_Adjust;
    }

    if (ctx->state == STATE_GOTO_FinalPreview) {
        *(int*)transition->actionParam = 1;
        int choice = *(int*)transition->actionParam;
        if (choice < 1 || choice > 4) *(int*)transition->actionParam = choice = 1;
        if (status == ACTION_SUCCESS) {
            if (choice == 1) return STATE_OUTPUT_File;
            else if (choice == 2) return STATE_DISPLAY_Terminal;
            else if (choice == 3) {
                ctx->state = STATE_OUTPUT_File;
                AppState next = processState(ctx);
                return (next == STATE_ERROR_Generic) ? next : STATE_DISPLAY_Terminal;
            } else if (choice == 4) return STATE_EXIT;
        }
        return STATE_GOTO_FinalPreview;
    }

    return (status == ACTION_SUCCESS) ? transition->success : transition->failure;
}

AppContext* createAppContext() {
    AppContext* ctx = (AppContext*)malloc(sizeof(AppContext));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(AppContext));
    ctx->state = STATE_INPUT_MainMenu;
    ctx->chunkSize = 3;
    ctx->colorMode = 0;
    ctx->resumeAfterConfig = 0;
    return ctx;
}

void freeAppContext(AppContext* ctx) {
    if (!ctx) return;
    if (ctx->filename) free(ctx->filename);
    if (ctx->parser) freeBitmapParser(ctx->parser);
    if (ctx->image) freeMatrix(ctx->image);
    if (ctx->edges) freeMatrix(ctx->edges);
    if (ctx->chunks) {
        for (int i = 0; i < ctx->totalChunks; i++)
            if (ctx->chunks[i]) freeMatrix(ctx->chunks[i]);
        free(ctx->chunks);
    }
    if (ctx->asciiArt) free(ctx->asciiArt);
    if (ctx->errorMessage) free(ctx->errorMessage);
    free(ctx);
}
