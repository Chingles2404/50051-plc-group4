#include <stdio.h>
#include <stdlib.h>
#include "bitmap_utils.h"

void handleMemory(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
}