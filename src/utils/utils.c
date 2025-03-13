#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int ** create2DArray(int numberRows, int numberCols) {
    int rowIndex;
    int** array;
    array = (int **) malloc(sizeof(int *) * numberRows);
    for (rowIndex = 0; rowIndex < numberRows; rowIndex++) {
        array[rowIndex] = (int *) malloc(sizeof(int) * numberCols);
    }
    return array;
}

Matrix* createMatrix(int numberRows, int numberCols) {
    Matrix* matrix = (Matrix*) malloc(sizeof(Matrix));

    matrix->numberCols = numberCols;
    matrix->numberRows = numberRows;
    matrix->size = numberRows * numberCols;
    matrix->data = create2DArray(numberRows, numberCols);

    return matrix;
}


void setMatrixElement(Matrix *matrix, int row, int col, int value) {
    matrix->data[row][col] = value;

}

int getMatrixElement(Matrix *matrix, int row, int col) {
    return matrix -> data[row][col];
}

Matrix *createPaddedMatrixWithZeros(Matrix *originalMatrix, int paddingAmount) {
    /* If paddingAmount is 1, that means we surround the original matrix with 1 layer of zero
    Creates a new matrix */
    Matrix *paddedMatrix;
    int newNumberOfRows;
    int newNumberOfCols;
    int rowIndex;
    int colIndex;

    newNumberOfRows = originalMatrix->numberRows + paddingAmount*2;
    newNumberOfCols = originalMatrix->numberCols + paddingAmount*2;

    paddedMatrix = createMatrix(newNumberOfRows, newNumberOfCols);
    

    for (rowIndex = 0; rowIndex < newNumberOfRows; rowIndex++) {
        for (colIndex = 0; colIndex < newNumberOfCols; colIndex++) {
            setMatrixElement(paddedMatrix, rowIndex, colIndex, 0);
        }
    }

    for (rowIndex = paddingAmount; rowIndex < newNumberOfRows - paddingAmount; rowIndex++) {
        for (colIndex = paddingAmount; colIndex < newNumberOfCols - paddingAmount; colIndex++) {
            setMatrixElement(paddedMatrix, rowIndex, colIndex, getMatrixElement(originalMatrix, rowIndex-paddingAmount, colIndex-paddingAmount));
        }
    }

    return paddedMatrix;


}


void free2DArray(int ** array, int numberRows) {
    int row;
    for (row = 0; row < numberRows; row++) {
        free(array[row]);
    }
    free(array);
}

void freeMatrix(Matrix *matrix) {
    free2DArray(matrix->data, matrix->numberRows);
    free(matrix);
}
