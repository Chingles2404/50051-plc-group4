#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app_fsm.h"
#include "fsm_actions.h"
#include "bitmap_parser.h"
#include "edge_detection.h"
#include "ascii.h"
#include "matrices.h"

int main(int argc, char **argv) {
    initialiseFSM();
    
    /* Create application context */
    AppContext* ctx = createAppContext();
    if (ctx == NULL) {
        fprintf(stderr, "Failed to create application context\n");
        cleanupFSM();
        return EXIT_FAILURE;
    }
    
    /* If command line argument provided, use it as filename */
    if (argc > 1) {
        ctx->filename = strdup(argv[1]);
        ctx->state = STATE_VALIDATE_Path;
    } else {
        ctx->state = STATE_INPUT_MainMenu;
    }
    
    int retryCount = 0;

    while (ctx->state != STATE_EXIT) {
        printf("--- Main loop: current state is %d ---\n", ctx->state);
        AppState newState = processState(ctx);
        printf("--- Main loop: new state is %d ---\n", newState);

        if (newState == ctx->state) {
            retryCount++;
            if (retryCount >= 3) {
                printf("Warning: State did not change after 3 attempts. Exiting to avoid infinite loop.\n");
                break;
            }
        } else {
            retryCount = 0;
        }

        ctx->state = newState;
    }
    
    printf("Exiting ASCII Art Generator. Goodbye!\n");
    
    freeAppContext(ctx);
    cleanupFSM();
    
    return EXIT_SUCCESS;
}