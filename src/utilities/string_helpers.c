#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_helpers.h"

char* stringDuplicate(const char* source) {
    size_t length;
    char* copy;

    if (source == NULL) return NULL;

    length = strlen(source) + 1; /* +1 for null terminator */
    
    copy = (char*)malloc(length);
    if (copy != NULL) {
        memcpy(copy, source, length);
    }
    return copy;
}