#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app_fsm.h"
#include "fsm_actions.h"
#include "bitmap_parser.h"
#include "edge_detection.h"
#include "ascii.h"
#include "matrices.h"
#include "string_helpers.h"

int main(int argc, char **argv) {
    AppContext* context;
    initialiseFSM();
    
    /* Create application context */
    context = createAppContext();
    if (context == NULL) {
        fprintf(stderr, "Failed to create application context\n");
        cleanupFSM();
        return EXIT_FAILURE;
    }
    
    /* If command line argument provided, use it as filename */
    if (argc > 1) {
        context->filename = stringDuplicate(argv[1]);
        context->state = STATE_VALIDATE_Path;
    } else {
        context->state = STATE_INPUT_MainMenu;
    }
    
    /* Main FSM loop */
    while (context->state != STATE_EXIT) {
        context->state = processState(context);
    }
    
    printf("Exiting ASCII Art Generator. Goodbye!\n");
    
    freeAppContext(context);
    cleanupFSM();
    
    return EXIT_SUCCESS;
}