#include <stdio.h>
#include <assert.h>

#include "matrices.h"

#include "ascii.h"
#include "test_ascii.h"

void test_ascii_needs_normalisation_check() {

    Matrix *m;

    // Requires normalisation (grayscale values > 128)
        
    m = createMatrix(3, 3);
    setMatrixElement(m, 0, 0, 255);
    setMatrixElement(m, 0, 1, 130);
    setMatrixElement(m, 0, 2, 0);
    setMatrixElement(m, 1, 0, 0);
    setMatrixElement(m, 1, 1, 200);
    setMatrixElement(m, 1, 2, 0);
    setMatrixElement(m, 2, 0, 0);
    setMatrixElement(m, 2, 1, 180);
    setMatrixElement(m, 2, 2, 0);

    assert(needs_normalisation(m) == 1);

    printf("test_ascii_needs_normalisation_check: Normalisation required (is grayscale) and ran successfully. Test passed\n");
    freeMatrix(m);

}

void test_ascii_does_not_need_normalisation_check() {

    Matrix *m;

    // Already in binary 0 and 1
    m = createMatrix(3, 3);
    setMatrixElement(m, 0, 0, 0);
    setMatrixElement(m, 0, 1, 1);
    setMatrixElement(m, 0, 2, 0);
    setMatrixElement(m, 1, 0, 0);
    setMatrixElement(m, 1, 1, 1);
    setMatrixElement(m, 1, 2, 0);
    setMatrixElement(m, 2, 0, 0);
    setMatrixElement(m, 2, 1, 1);
    setMatrixElement(m, 2, 2, 0);

    assert(needs_normalisation(m) == 0);

    printf("test_ascii_does_not_need_normalisation_check: No normalisation required (is already binary) and ran successfully. Test passed\n");
    freeMatrix(m);

}

void test_normalise_edge_matrix() {
    Matrix *input = createMatrix(3, 3);
    Matrix *binary = createMatrix(3, 3);

    // Input has values above and below 128
    setMatrixElement(input, 0, 0, 255);
    setMatrixElement(input, 0, 1, 127);
    setMatrixElement(input, 0, 2, 129);
    setMatrixElement(input, 1, 0, 100);
    setMatrixElement(input, 1, 1, 200);
    setMatrixElement(input, 1, 2, 128);
    setMatrixElement(input, 2, 0, 0);
    setMatrixElement(input, 2, 1, 129);
    setMatrixElement(input, 2, 2, 50);

    normalise_edge_matrix(input, binary);

    // Check values were correctly thresholded at 128
    assert(getMatrixElement(binary, 0, 0) == 1);
    assert(getMatrixElement(binary, 0, 1) == 0);
    assert(getMatrixElement(binary, 0, 2) == 1);
    assert(getMatrixElement(binary, 1, 0) == 0);
    assert(getMatrixElement(binary, 1, 1) == 1);
    assert(getMatrixElement(binary, 1, 2) == 0);
    assert(getMatrixElement(binary, 2, 0) == 0);
    assert(getMatrixElement(binary, 2, 1) == 1);
    assert(getMatrixElement(binary, 2, 2) == 0);

    printf("test_normalise_edge_matrix passed\n");

    freeMatrix(input);
    freeMatrix(binary);
}

void test_compute_mse() {
    Matrix *A = createMatrix(3, 3);
    Matrix *B = createMatrix(3, 3);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            setMatrixElement(A, i, j, 1);
            setMatrixElement(B, i, j, 0);
        }
    }

    char test_character = '#';
    double weight = get_weight(test_character);
    double expected_mse = (1.0 * 1.0 * weight * 9) / 9.0;  // 9 elements, diff^2 = 1

    double mse = compute_mse(A, B, test_character);

    assert(mse == expected_mse);
    printf("test_compute_mse passed\n");

    freeMatrix(A);
    freeMatrix(B);
}


void test_ascii_template_single() {
    Matrix* m = createMatrix(3, 3);
    assert(m != NULL);

    int values[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            setMatrixElement(m, i, j, values[i][j]);

    char bestChar = find_best_ascii(m);
    assert(bestChar == '@'); 

    freeMatrix(m);
    printf("test_ascii_template_full_block passed\n");
}




// Helper function to consecutively test multiple ascii_templates in one go
void run_test_cases(const char *label, const int values[9], char expected_character, int is_expected_to_match_character) {
    Matrix* m = createMatrixFromFlatArray(3, 3, values);
    Matrix* binary_matrix = createMatrix(3, 3);

    printf("%s\n", label);
    print_matrix(m);

    char result;

    if (needs_normalisation(m)) {
        normalise_edge_matrix(m, binary_matrix);
        printf("\nNormalised Binary Matrix:\n");
        print_matrix(binary_matrix);
        result = find_best_ascii(binary_matrix);
    } else {
        result = find_best_ascii(m);
    }

    printf("ASCII character chosen: %c", result);
    if (is_expected_to_match_character) {
        printf(" | Expected: %c", expected_character);
        assert(result == expected_character);
    }
    printf("\n\n");

    freeMatrix(m);
    freeMatrix(binary_matrix);
}

const TestCase test_cases[] = {
    
    {"Match this character: '@'", {1,1,1, 1,1,1, 1,1,1}, '@', 1},
    {"Match this character: '/'", {0,0,1, 0,1,0, 1,0,0}, '/', 1},
    {"Match this character: '\\'", {1,0,0, 0,1,0, 0,0,1}, '\\', 1},
    {"Match this character: '|'", {0,1,0, 0,1,0, 0,1,0}, '|', 1},
    {"Match this character: '-'", {0,0,0, 1,1,1, 0,0,0}, '-', 1},
    {"Match this character: 'X'", {1,0,1, 0,1,0, 1,0,1}, 'X', 1},
    {"Match this character: '+'", {0,1,0, 1,1,1, 0,1,0}, '+', 1},
    {"Match this character: '-' (top)", {1,1,1, 0,0,0, 0,0,0}, '-', 1},
    {"Match this character: '-' (middle)", {0,0,0, 1,1,1, 0,0,0}, '-', 1},
    {"Match this character: '_'", {0,0,0, 0,0,0, 1,1,1}, '_', 1},
    {"Match this character: '=' (sandwich)", {1,1,1, 0,0,0, 1,1,1}, '=', 1},
    {"Match this character: '=' (top-heavy)", {1,1,1, 1,1,1, 0,0,0}, '=', 1},
    {"Match this character: '=' (bottom-heavy)", {0,0,0, 1,1,1, 1,1,1}, '=', 1},
    {"Match this character: '|' (left)", {1,0,0, 1,0,0, 1,0,0}, '|', 1},
    {"Match this character: '|' (middle)", {0,1,0, 0,1,0, 0,1,0}, '|', 1},
    {"Match this character: '|' (right)", {0,0,1, 0,0,1, 0,0,1}, '|', 1},
    {"Match this character: 'L'", {1,0,0, 1,0,0, 1,1,1}, 'L', 1},
    {"Match this character: 'J'", {1,1,1, 0,0,1, 0,0,1}, 'J', 1},
    {"Match this character: 'r'", {1,1,1, 1,0,0, 1,0,0}, 'r', 1},
    {"Match this character: 'T'", {1,1,1, 0,1,0, 0,1,0}, 'T', 1},
    {"Match this character: '7'", {1,1,1, 0,1,0, 1,0,0}, '7', 1},
    {"Match this character: '['", {1,1,0, 1,0,0, 1,1,0}, '[', 1},
    {"Match this character: '[' alt", {0,1,1, 0,1,0, 0,1,1}, '[', 1},
    {"Match this character: ']'", {1,1,0, 0,1,0, 1,1,0}, ']', 1},
    {"Match this character: ']' alt", {0,1,1, 0,0,1, 0,1,1}, ']', 1},
    {"Match this character: 'O'", {1,1,1, 1,0,1, 1,1,1}, 'O', 1},
    {"Match this character: 'F'", {1,1,1, 1,1,1, 1,0,0}, 'F', 1},
    {"Match this character: 'F'", {1,1,1, 1,1,1, 1,0,0}, 'F', 1},
    {"Match this character: 'F'", {1,1,1, 1,1,1, 1,0,0}, 'F', 1},
    {"Match this character: 'F'", {1,1,1, 1,1,1, 1,0,0}, 'F', 1},
    {"Match this character: 'F'", {1,1,1, 1,1,1, 1,0,0}, 'F', 1},
    {"Match this character: 'F'", {1,1,1, 1,1,1, 1,0,0}, 'F', 1},
    {"Match this character: 'F'", {1,1,1, 1,1,1, 1,0,0}, 'F', 1},
    {"Match this character: 'F'", {1,1,1, 1,1,1, 1,0,0}, 'F', 1},
    {"Match this character: 'F'", {1,1,1, 1,1,1, 1,0,0}, 'F', 1},
    
    // Dot variants
    {"Match this character: '`' (TC)", {0,1,0, 0,0,0, 0,0,0}, '`', 1},
    {"Match this character: '`' (TR)", {0,0,1, 0,0,0, 0,0,0}, '`', 1},
    {"Match this character: '`' (TL)", {1,0,0, 0,0,0, 0,0,0}, '`', 1},
    {"Match this character: '.' (ML)", {0,0,0, 1,0,0, 0,0,0}, '.', 1},
    {"Match this character: '.' (C)", {0,0,0, 0,1,0, 0,0,0}, '.', 1},
    {"Match this character: '.' (MR)", {0,0,0, 0,0,1, 0,0,0}, '.', 1},
    {"Match this character: '.' (BL)", {0,0,0, 0,0,0, 1,0,0}, '.', 1},
    {"Match this character: '.' (BC)", {0,0,0, 0,0,0, 0,1,0}, '.', 1},
    {"Match this character: '.' (BR)", {0,0,0, 0,0,0, 0,0,1}, '.', 1},

    // Colon variants
    {"Match this character: ':' (T-L)", {1,0,0, 1,0,0, 0,0,0}, ':', 1},
    {"Match this character: ':' (B-L)", {0,0,0, 1,0,0, 1,0,0}, ':', 1},
    {"Match this character: ':' (T-C)", {0,1,0, 0,1,0, 0,0,0}, ':', 1},
    {"Match this character: ':' (B-C)", {0,0,0, 0,1,0, 0,1,0}, ':', 1},
    {"Match this character: ':' (T-R)", {0,0,1, 0,0,1, 0,0,0}, ':', 1},
    {"Match this character: ':' (B-R)", {0,0,0, 0,0,1, 0,0,1}, ':', 1},

    // Uncertain
    {"Uncertain: L-corner",                {1,1,1,1,0,0,0,0,0}, '?', 0},
    {"Uncertain: Bottom-left block",       {0,0,0,0,0,0,1,1,0}, '?', 0},
    {"Uncertain: Top-right box",           {0,1,1,0,1,1,0,0,0}, '?', 0},
    {"Uncertain: double-vertical",         {1,0,1,1,0,1,1,0,1}, '?', 0},
    {"Uncertain: Plus-like pattern",       {50,160,60,220,255,210,80,170,90}, '?', 0},
    {"Uncertain: Blurry noise",            {120,130,140,130,125,120,140,110,100}, '?', 0}
};


void test_ascii_template_multiple() {
    const int total = sizeof(test_cases) / sizeof(test_cases[0]);
    for (int i = 0; i < total; ++i) {
        run_test_cases(
            test_cases[i].label,
            test_cases[i].values,
            test_cases[i].expected_character,
            test_cases[i].is_expected
        );
    }
    printf("test_ascii_template_multiple passed\n");
}