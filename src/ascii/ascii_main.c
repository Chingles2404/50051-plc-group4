#include <stdio.h>
#include <math.h>
#include <float.h>
#include "ascii_main.h"
#include <assert.h>


const AsciiTemplate ASCII_TEMPLATES[] = {

  /* All-heavy */
  {'@', (int[]){1,1,1, 1,1,1, 1,1,1}, 3, 3},

  /* Original 6 templates */
  {'/', (int[]){0,0,1, 0,1,0, 1,0,0}, 3, 3},
  {'\\', (int[]){1,0,0, 0,1,0, 0,0,1}, 3, 3},
  {'|', (int[]){0,1,0, 0,1,0, 0,1,0}, 3, 3},
  {'-', (int[]){0,0,0, 1,1,1, 0,0,0}, 3, 3},
  {'X', (int[]){1,0,1, 0,1,0, 1,0,1}, 3, 3},
  {'+', (int[]){0,1,0, 1,1,1, 0,1,0}, 3, 3},

  /* Horizontal and vertical variants */
  {'-', (int[]){1,1,1, 0,0,0, 0,0,0}, 3, 3},
  {'_', (int[]){0,0,0, 0,0,0, 1,1,1}, 3, 3},
  {'=', (int[]){1,1,1, 0,0,0, 1,1,1}, 3, 3},
  {'=', (int[]){1,1,1, 1,1,1, 0,0,0}, 3, 3},
  {'=', (int[]){0,0,0, 1,1,1, 1,1,1}, 3, 3},
  {'|', (int[]){1,0,0, 1,0,0, 1,0,0}, 3, 3},
  {'|', (int[]){0,0,1, 0,0,1, 0,0,1}, 3, 3},

  /* Edges */
  {'L', (int[]){1,0,0, 1,0,0, 1,1,1}, 3, 3},
  {'J', (int[]){1,1,1, 0,0,1, 0,0,1}, 3, 3},
  {'r', (int[]){1,1,1, 1,0,0, 1,0,0}, 3, 3},
  {'T', (int[]){1,1,1, 0,1,0, 0,1,0}, 3, 3},
  {'7', (int[]){1,1,1, 0,1,0, 1,0,0}, 3, 3},
  {'[', (int[]){1,1,0, 1,0,0, 1,1,0}, 3, 3},
  {'[', (int[]){0,1,1, 0,1,0, 0,1,1}, 3, 3},
  {']', (int[]){1,1,0, 0,1,0, 1,1,0}, 3, 3},
  {']', (int[]){0,1,1, 0,0,1, 0,1,1}, 3, 3},
  {'O', (int[]){1,1,1, 1,0,1, 1,1,1}, 3, 3},
  {'F', (int[]){1,1,1, 1,1,1, 1,0,0}, 3, 3},

  /* Dot variants */
  {'`', (int[]){1,0,0, 0,0,0, 0,0,0}, 3, 3},
  {'`', (int[]){0,1,0, 0,0,0, 0,0,0}, 3, 3},
  {'`', (int[]){0,0,1, 0,0,0, 0,0,0}, 3, 3},
  {'.', (int[]){0,0,0, 1,0,0, 0,0,0}, 3, 3},
  {'.', (int[]){0,0,0, 0,0,1, 0,0,0}, 3, 3},
  {'.', (int[]){0,0,0, 0,0,0, 1,0,0}, 3, 3},
  {'.', (int[]){0,0,0, 0,0,0, 0,1,0}, 3, 3},
  {'.', (int[]){0,0,0, 0,0,0, 0,0,1}, 3, 3},

  /* Dot variants */
  {':', (int[]){1,0,0, 1,0,0, 0,0,0}, 3, 3},
  {':', (int[]){0,0,0, 1,0,0, 1,0,0}, 3, 3},
  {':', (int[]){0,1,0, 0,1,0, 0,0,0}, 3, 3},
  {':', (int[]){0,0,0, 0,1,0, 0,1,0}, 3, 3},
  {':', (int[]){0,0,1, 0,0,1, 0,0,0}, 3, 3},
  {':', (int[]){0,0,0, 0,0,1, 0,0,1}, 3, 3},
};


#define TEMPLATE_COUNT (sizeof(ASCII_TEMPLATES) / sizeof(ASCII_TEMPLATES[0]))

/* Determine the weight based on the ASCII character type */
double get_weight(char ascii_char) {
  switch (ascii_char) {
      case '/': case '\\': return 0.9;
      case '|':            return 1.1;
      case '-':            return 1.2;
      case 'X':            return 1.3;
      default:             return 1.0;
  }
}

int needs_normalisation(Matrix *matrix) {
  for (int i = 0; i < matrix->numberRows; i++) {
      for (int j = 0; j < matrix->numberCols; j++) {
          if (getMatrixElement(matrix, i, j) > 1) {
              return 1;
          }
      }
  }
  return 0;
}

/* Normalise edge values into binary (0 or 1) */
void normalise_edge_matrix(Matrix *edge_matrix, Matrix *binary_matrix) {

  int rows = edge_matrix->numberRows;

  int cols = edge_matrix->numberCols;

  for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
          // Convert grayscale edge values to binary: thresholding at 128
          int value = getMatrixElement(edge_matrix, i, j);

          int binary_value = (value > 128) ? 1 : 0;
          setMatrixElement(binary_matrix, i, j, binary_value);
      }
  }
}


/* Sum of squared differences between matrices with weight for penalising mismatches */
double compute_mse(Matrix *edge_matrix, Matrix *ascii_template, char ascii_char) {

  int rows = edge_matrix->numberRows;
  int cols = edge_matrix->numberCols;

  double mse = 0.0;
  double weight = get_weight(ascii_char);

  for (int i = 0; i < rows; i++) {

      for (int j = 0; j < cols; j++) {

          double diff = getMatrixElement(edge_matrix, i, j) - getMatrixElement(ascii_template, i, j);
          mse += (diff * diff) * weight;  // Apply weight to penalise mismatches
      }
  }

  return mse / (rows * cols);  // Normalise by total number of elements
}


/* Finds the best matching ASCII character for a given 3x3 edge matrix */
char find_best_ascii(Matrix *edge_matrix) {
  int i;
  double min_mse = DBL_MAX;
  char best_match = ' ';

  /* printf("\nChecking Weighted MSE for different ASCII templates:\n"); */

  /* Compare the input matrix with each ASCII template */
  for (i = 0; i < TEMPLATE_COUNT; i++) {

    const int* flat_values = (const int*)ASCII_TEMPLATES[i].ascii_template;

    Matrix* template_matrix = createMatrixFromFlatArray(3, 3, flat_values);

    double mse = compute_mse(edge_matrix, template_matrix, ASCII_TEMPLATES[i].character);
    /* printf("Weighted MSE with '%c': %.5f\n", ASCII_TEMPLATES[i].character, mse); */

    /* Select the ASCII character with the lowest MSE (best match) */
    if (mse < min_mse) {
        min_mse = mse;
        best_match = ASCII_TEMPLATES[i].character;
    }
  }

  printf("\nBest match: %c with Weighted MSE: %.5f\n\n", best_match, min_mse);
  return best_match;
}

void print_matrix(Matrix *matrix) {
  int i, j;
  printf("Matrix (%d x %d):\n", matrix->numberRows, matrix->numberCols);
  for (i = 0; i < matrix->numberRows; i++) {
      for (j = 0; j < matrix->numberCols; j++) {
          printf("%d ", getMatrixElement(matrix, i, j));
      }
      printf("\n");
  }
}


/* === Test Case Runner === */
void run_test_case(const char *label, Matrix *matrix) {
  Matrix* binary_matrix = createMatrix(3, 3);

  printf("========== %s ==========\n", label);
  print_matrix(matrix);

  if (needs_normalisation(matrix)) {
      normalise_edge_matrix(matrix, binary_matrix);
      printf("\nNormalised Binary Matrix:\n");
      print_matrix(binary_matrix);
      printf("ASCII character selected: %c\n\n", find_best_ascii(binary_matrix));
  } else {
      printf("ASCII character selected: %c\n\n", find_best_ascii(matrix));
  }

  freeMatrix(matrix);
}

void test_ascii_template_full_block() {
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

  // You can now test matching logic, e.g.:
  char bestChar = find_best_ascii(m);
  assert(bestChar == '@'); 

  freeMatrix(m);
  printf("test_ascii_template_full_block passed\n");
}

// /* TODO: remove in future. Used for testing for now */
// int main() {

//   const TestCase test_cases[] = {
//       // === Direct Matches ===
//       {"Direct match: '@'", {1,1,1,1,1,1,1,1,1}},
//       {"Direct match: '/'", {0,0,1,0,1,0,1,0,0}},
//       {"Direct match: '\\'", {1,0,0,0,1,0,0,0,1}},
//       {"Direct match: '|'", {0,1,0,0,1,0,0,1,0}},
//       {"Direct match: '-'", {0,0,0,1,1,1,0,0,0}},
//       {"Direct match: 'X'", {1,0,1,0,1,0,1,0,1}},
//       {"Direct match: '+'", {0,1,0,1,1,1,0,1,0}},
//       {"Direct match: '-' (top row)", {1,1,1,0,0,0,0,0,0}},
//       {"Direct match: '_'", {0,0,0,0,0,0,1,1,1}},
//       {"Direct match: '=' (sandwich)", {1,1,1,0,0,0,1,1,1}},
//       {"Direct match: '=' (top-heavy)", {1,1,1,1,1,1,0,0,0}},
//       {"Direct match: '=' (bottom-heavy)", {0,0,0,1,1,1,1,1,1}},
//       {"Direct match: '|' (left)", {1,0,0,1,0,0,1,0,0}},
//       {"Direct match: '|' (right)", {0,0,1,0,0,1,0,0,1}},
//       {"Direct match: 'L'", {1,0,0,1,0,0,1,1,1}},
//       {"Direct match: 'J'", {1,1,1,0,0,1,0,0,1}},
//       {"Direct match: 'r'", {1,1,1,1,0,0,1,0,0}},
//       {"Direct match: 'T'", {1,1,1,0,1,0,0,1,0}},
//       {"Direct match: '7'", {1,1,1,0,1,0,1,0,0}},
//       {"Direct match: '['", {1,1,0,1,0,0,1,1,0}},
//       {"Direct match: '[' alt", {0,1,1,0,1,0,0,1,1}},
//       {"Direct match: ']'", {1,1,0,0,1,0,1,1,0}},
//       {"Direct match: ']' alt", {0,1,1,0,0,1,0,1,1}},
//       {"Direct match: 'O'", {1,1,1,1,0,1,1,1,1}},
//       {"Direct match: 'F'", {1,1,1,1,1,1,1,0,0}},

//       // === Dots and Colons ===
//       {"Direct match: '`' (TL)", {1,0,0,0,0,0,0,0,0}},
//       {"Direct match: '`' (TC)", {0,1,0,0,0,0,0,0,0}},
//       {"Direct match: '`' (TR)", {0,0,1,0,0,0,0,0,0}},
//       {"Direct match: '.' (ML)", {0,0,0,1,0,0,0,0,0}},
//       {"Direct match: '.' (MR)", {0,0,0,0,0,1,0,0,0}},
//       {"Direct match: '.' (BL)", {0,0,0,0,0,0,1,0,0}},
//       {"Direct match: '.' (BC)", {0,0,0,0,0,0,0,1,0}},
//       {"Direct match: '.' (BR)", {0,0,0,0,0,0,0,0,1}},
//       {"Direct match: ':' (T-L)", {1,0,0,1,0,0,0,0,0}},
//       {"Direct match: ':' (B-L)", {0,0,0,1,0,0,1,0,0}},
//       {"Direct match: ':' (T-C)", {0,1,0,0,1,0,0,0,0}},
//       {"Direct match: ':' (B-C)", {0,0,0,0,1,0,0,1,0}},
//       {"Direct match: ':' (T-R)", {0,0,1,0,0,1,0,0,0}},
//       {"Direct match: ':' (B-R)", {0,0,0,0,0,1,0,0,1}},

//       // === Unmatched Patterns ===
//       {"Non-direct match: L-corner", {1,1,1,1,0,0,0,0,0}},
//       {"Non-direct match: Bottom-left block", {0,0,0,0,0,0,1,1,0}},
//       {"Non-direct match: T top-heavy", {0,1,0,1,1,1,0,0,0}},
//       {"Non-direct match: Top-right box", {0,1,1,0,1,1,0,0,0}},
//       {"Non-direct match: double-vertical", {1,0,1,1,0,1,1,0,1}},
//       {"Non-direct match: top & bottom vertical dots", {0,1,0,0,0,0,0,1,0}},
//       {"Non-direct match: Grayscale - Diagonal /", {0,0,255,0,200,0,255,0,0}},
//       {"Non-direct match: Grayscale - Diagonal \\", {255,0,0,0,200,0,0,0,255}},
//       {"Non-direct match: Grayscale - Vertical-ish", {50,180,50,30,200,30,70,255,60}},
//       {"Non-direct match: Grayscale - Horizontal-ish", {5,10,20,240,250,245,10,5,0}},
//       {"Non-direct match: Grayscale - X-like", {200,0,200,0,180,0,200,0,200}},
//       {"Non-direct match: Grayscale - Plus-like", {50,160,60,220,255,210,80,170,90}},
//       {"Non-direct match: Grayscale - Blurry noise", {120,130,140,130,125,120,140,110,100}}
//   };
  
//   int total = sizeof(test_cases) / sizeof(test_cases[0]);
//   for (int i = 0; i < total; ++i) {
//       Matrix* m = createMatrixFromFlatArray(3, 3, test_cases[i].values);
//       run_test_case(test_cases[i].label, m);
//       freeMatrix(m);
//   }

//   return 0;
// }