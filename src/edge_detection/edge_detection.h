#ifndef EDGE_DETECTION_UTILS_H
#define EDGE_DETECTION_UTILS_H

int ** create2DArray(int numberRows, int numberCols);


typedef struct matrix {
    int size;
    int numberRows;
    int numberCols;
    int **data;
} Matrix;

Matrix* createMatrix(int numberRows, int numberCols);

Matrix* createMatrixFromFlatArray(int rows, int cols, const int values[]);

void setMatrixElement(Matrix *matrix, int row, int col, int value);
int getMatrixElement(Matrix *matrix, int row, int col);

Matrix *createPaddedMatrixWithZeros(Matrix *originalMatrix, int paddingAmount);
Matrix * createMatrixWithRemovedPadding(Matrix *originalMatrix, int paddingAmountToRemove);
int matricesAreEqual(Matrix *firstMatrix, Matrix *secondMatrix);

void free2DArray(int ** array, int numberRows);
void freeMatrix(Matrix *matrix);

Matrix * getSurroundingElements(Matrix * matrix, int targetRow, int targetCol, int kernelSize);
int matrixDotProduct(Matrix * matrix1, Matrix * matrix2, int kernelSize);
Matrix * applyKernel(Matrix * matrix, Matrix * kernel);
int pythagoreanAddition(int x, int y);

Matrix * getGradientOfPixel(Matrix * target, Matrix * xKernel, Matrix * yKernel);

Matrix * gradientPipeline(Matrix * target);

Matrix** chunkImage(Matrix* image, int chunkSize);

#endif