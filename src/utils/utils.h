#ifndef UTILS_H
#define UTILS_H

int ** create2DArray(int numberRows, int numberCols);


typedef struct matrix {
    int size;
    int numberRows;
    int numberCols;
    int **data;
} Matrix;

Matrix* createMatrix(int numberRows, int numberCols);
void setMatrixElement(Matrix *matrix, int row, int col, int value);
int getMatrixElement(Matrix *matrix, int row, int col);

Matrix *createPaddedMatrixWithZeros(Matrix *originalMatrix, int paddingAmount);

void free2DArray(int ** array, int numberRows);
void freeMatrix(Matrix *matrix);

#endif