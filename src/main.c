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
    
    /* Main FSM loop */
    while (ctx->state != STATE_EXIT) {
        ctx->state = processState(ctx);
    }
    
    printf("Exiting ASCII Art Generator. Goodbye!\n");
    
    freeAppContext(ctx);
    cleanupFSM();
    
    return EXIT_SUCCESS;
}