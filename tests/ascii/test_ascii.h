#ifndef TEST_ASCII_H
#define TEST_ASCII_H

typedef struct {
    const char* label;
    const int values[9];
    char expected_character; // Only used if `is_expected` is 1
    int is_expected; // 1 if this character is expected to match. else 0
} TestCase;

void test_ascii_needs_normalisation_check();
void test_ascii_does_not_need_normalisation_check();
void test_normalise_edge_matrix();
void test_compute_mse();
void test_ascii_template_single();

void run_test_cases(const char *label, const int values[9], char expected_character, int is_expected_to_match_character);

void test_ascii_template_multiple();


#endif