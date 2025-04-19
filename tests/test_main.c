#include <stdio.h>

#include "matrices.h"
#include "test_matrices.h"

#include "ascii.h"
#include "test_ascii.h"

int main(int argc, char **argv) {
    
    printf("--------------------------------------------------\n");
    printf(" Running Matrix Tests\n");
    printf("--------------------------------------------------\n");

    test_create_and_free_2DArray();
    test_create_and_free_matrix();
    test_create_and_free_padded_matrix();
    test_matrices_are_same();
    test_removal_of_padding_from_matrix();
    
    printf("--------------------------------------------------\n");
    printf(" Running ASCII-Substitution Tests\n");
    printf("--------------------------------------------------\n");

    test_ascii_needs_normalisation_check();
    test_ascii_does_not_need_normalisation_check();
    test_normalise_edge_matrix();
    test_compute_mse();
    test_ascii_template_single();
    test_ascii_template_multiple();
    
    printf("--------------------------------------------------\n");
    printf(" All tests passed successfully!\n");
    printf("--------------------------------------------------\n\n");
    return 0;
}
