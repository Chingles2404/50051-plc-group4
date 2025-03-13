#include <stdio.h>
#include "test_utils.h"

int main(int argc, char **argv) {
    test_create_and_free_2DArray();
    test_create_and_free_matrix();
    test_create_and_free_padded_matrix();
    printf("All tests passed!\n");
    return 0;
}
