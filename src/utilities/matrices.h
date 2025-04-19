
#ifndef MATRICES_H
#define MATRICES_H

int ** create2DArray(int numberRows, int numberCols);


typedef struct matrix {
    int size;
    int numberRows;
    int numberCols;
    int **data;
} Matrix;

Matrix* createMatrix(int numberRows, int numberCols);

Matrix* createMatrixFromFlatArray(int rows, int cols, const int values[]);
Matrix* convertToAsciiMatrix(Matrix* edgeMatrix);

void setMatrixElement(Matrix *matrix, int row, int col, int value);
int getMatrixElement(Matrix *matrix, int row, int col);

Matrix *createPaddedMatrixWithZeros(Matrix *originalMatrix, int paddingAmount);
Matrix * createMatrixWithRemovedPadding(Matrix *originalMatrix, int paddingAmountToRemove);
int matricesAreEqual(Matrix *firstMatrix, Matrix *secondMatrix);

void free2DArray(int ** array, int numberRows);
void freeMatrix(Matrix *matrix);


void print_matrix(Matrix *matrix);

#endif