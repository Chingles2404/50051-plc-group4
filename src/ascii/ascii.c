#include <stdio.h>
#include <math.h>
#include <float.h>
#include "ascii.h"
#include <assert.h>

static int template_at[]        = {1,1,1, 1,1,1, 1,1,1};
static int template_slash[]     = {0,0,1, 0,1,0, 1,0,0};
static int template_backslash[] = {1,0,0, 0,1,0, 0,0,1};
static int template_pipe[]      = {0,1,0, 0,1,0, 0,1,0};
static int template_dash[]      = {0,0,0, 1,1,1, 0,0,0};
static int template_x[]         = {1,0,1, 0,1,0, 1,0,1};
static int template_plus[]      = {0,1,0, 1,1,1, 0,1,0};

static int template_dash_top[]      = {1,1,1, 0,0,0, 0,0,0};
static int template_dash_middle[]   = {0,0,0, 1,1,1, 0,0,0};
static int template_underscore[]    = {0,0,0, 0,0,0, 1,1,1};

static int template_equal_topbottom[]  = {1,1,1, 0,0,0, 1,1,1};
static int template_equal_topmiddle[]  = {1,1,1, 1,1,1, 0,0,0};
static int template_equal_midbottom[]  = {0,0,0, 1,1,1, 1,1,1};

static int template_bar_left[]     = {1,0,0, 1,0,0, 1,0,0};
static int template_bar_middle[]   = {0,1,0, 0,1,0, 0,1,0};
static int template_bar_right[]    = {0,0,1, 0,0,1, 0,0,1};

static int template_L[]       = {1,0,0, 1,0,0, 1,1,1};
static int template_J[]       = {1,1,1, 0,0,1, 0,0,1};
static int template_r[]       = {1,1,1, 1,0,0, 1,0,0};
static int template_T[]       = {1,1,1, 0,1,0, 0,1,0};
static int template_7[]       = {1,1,1, 0,1,0, 1,0,0};

static int template_bracketL1[] = {1,1,0, 1,0,0, 1,1,0};
static int template_bracketL2[] = {0,1,1, 0,1,0, 0,1,1};
static int template_bracketR1[] = {1,1,0, 0,1,0, 1,1,0};
static int template_bracketR2[] = {0,1,1, 0,0,1, 0,1,1};

static int template_O[]       = {1,1,1, 1,0,1, 1,1,1};
static int template_F[]       = {1,1,1, 1,1,1, 1,0,0};

/* Dot and Backtick patterns */
static int template_dot1[] = {1,0,0, 0,0,0, 0,0,0};
static int template_dot2[] = {0,1,0, 0,0,0, 0,0,0};
static int template_dot3[] = {0,0,1, 0,0,0, 0,0,0};
static int template_dot4[] = {0,0,0, 1,0,0, 0,0,0};
static int template_dot5[] = {0,0,0, 0,1,0, 0,0,0};
static int template_dot6[] = {0,0,0, 0,0,1, 0,0,0};
static int template_dot7[] = {0,0,0, 0,0,0, 1,0,0};
static int template_dot8[] = {0,0,0, 0,0,0, 0,1,0};
static int template_dot9[] = {0,0,0, 0,0,0, 0,0,1};

/* Colon patterns */
static int template_colon1[] = {1,0,0, 1,0,0, 0,0,0};
static int template_colon2[] = {0,0,0, 1,0,0, 1,0,0};
static int template_colon3[] = {0,1,0, 0,1,0, 0,0,0};
static int template_colon4[] = {0,0,0, 0,1,0, 0,1,0};
static int template_colon5[] = {0,0,1, 0,0,1, 0,0,0};
static int template_colon6[] = {0,0,0, 0,0,1, 0,0,1};

const AsciiTemplate ASCII_TEMPLATES[] = {

  {'@', template_at,        3, 3},
  {'/', template_slash,     3, 3},
  {'\\', template_backslash, 3, 3},
  {'|', template_pipe,      3, 3},
  {'-', template_dash,      3, 3},
  {'X', template_x,         3, 3},
  {'+', template_plus,      3, 3},


  /* Horizontal and vertical variants */
  {'-', template_dash_top,       3, 3},
  {'-', template_dash_middle,    3, 3},
  {'_', template_underscore,     3, 3},

  {'=', template_equal_topbottom, 3, 3},
  {'=', template_equal_topmiddle, 3, 3},
  {'=', template_equal_midbottom, 3, 3},

  {'|', template_bar_left,       3, 3},
  {'|', template_bar_middle,     3, 3},
  {'|', template_bar_right,      3, 3},

  /* Edges */

  {'L', template_L, 3, 3},
  {'J', template_J, 3, 3},
  {'r', template_r, 3, 3},
  {'T', template_T, 3, 3},
  {'7', template_7, 3, 3},

  {'[', template_bracketL1, 3, 3},
  {'[', template_bracketL2, 3, 3},
  {']', template_bracketR1, 3, 3},
  {']', template_bracketR2, 3, 3},

  {'O', template_O, 3, 3},
  {'F', template_F, 3, 3},

  /* Dot variants */
  {'`', template_dot1, 3, 3},
  {'`', template_dot2, 3, 3},
  {'`', template_dot3, 3, 3},

  {'.', template_dot4, 3, 3},
  {'.', template_dot5, 3, 3},
  {'.', template_dot6, 3, 3},
  {'.', template_dot7, 3, 3},
  {'.', template_dot8, 3, 3},
  {'.', template_dot9, 3, 3},

  /* Colon variants */
  {':', template_colon1, 3, 3},
  {':', template_colon2, 3, 3},
  {':', template_colon3, 3, 3},
  {':', template_colon4, 3, 3},
  {':', template_colon5, 3, 3},
  {':', template_colon6, 3, 3}
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
  int i;
  int j;

  for (i = 0; i < matrix->numberRows; i++) {
      for (j = 0; j < matrix->numberCols; j++) {
          if (getMatrixElement(matrix, i, j) > 1) {
              return 1;
          }
      }
  }
  return 0;
}

/* Normalise edge values into binary (0 or 1) */
void normalise_edge_matrix(Matrix *edge_matrix, Matrix *binary_matrix) {
  int rows;
  int cols;
  int i;
  int j;

  rows = edge_matrix->numberRows;

  cols = edge_matrix->numberCols;

  for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        int value;
        int binary_value;
        /* Convert grayscale edge values to binary: thresholding at 128 */
        value = getMatrixElement(edge_matrix, i, j);

        binary_value = (value > 128) ? 1 : 0;
        setMatrixElement(binary_matrix, i, j, binary_value);
      }
  }
}


/* Sum of squared differences between matrices with weight for penalising mismatches */
double compute_mse(Matrix *edge_matrix, Matrix *ascii_template, char ascii_char) {
  int rows;
  int cols;
  double mse;
  double weight;
  int i;
  int j;

  rows = edge_matrix->numberRows;
  cols = edge_matrix->numberCols;

  mse = 0.0;
  weight = get_weight(ascii_char);

  for (i = 0; i < rows; i++) {

    for (j = 0; j < cols; j++) {
      double diff;

      diff = getMatrixElement(edge_matrix, i, j) - getMatrixElement(ascii_template, i, j);
      mse += (diff * diff) * weight; /* Apply weight to penalise mismatches */
    }
  }

  return mse / (rows * cols); /* Normalise by total number of elements  */
}


/* Finds the best matching ASCII character for a given 3x3 edge matrix */
char find_best_ascii(Matrix *edge_matrix) {
  int i;
  double min_mse;
  char best_match;

  Matrix* template_matrix;
  double mse;

  min_mse = DBL_MAX;
  best_match = ' ';

  /* printf("\nChecking Weighted MSE for different ASCII templates:\n"); */

  /* Compare the input matrix with each ASCII template */
  for (i = 0; i < TEMPLATE_COUNT; i++) {

    const int* flat_values = (const int*)ASCII_TEMPLATES[i].ascii_template;


    template_matrix = createMatrixFromFlatArray(3, 3, flat_values);

    mse = compute_mse(edge_matrix, template_matrix, ASCII_TEMPLATES[i].character);
    /* printf("Weighted MSE with '%c': %.5f\n", ASCII_TEMPLATES[i].character, mse); */

    /* Select the ASCII character with the lowest MSE (best match) */
    if (mse < min_mse) {
        min_mse = mse;
        best_match = ASCII_TEMPLATES[i].character;
    }
  }

  /* printf("\nBest match: %c with Weighted MSE: %.5f\n\n", best_match, min_mse); */
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
