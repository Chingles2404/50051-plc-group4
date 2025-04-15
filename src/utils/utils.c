#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

Matrix * createMatrixWithRemovedPadding(Matrix *originalMatrix, int paddingAmountToRemove) {
    /* If paddingAmountToRemove is 1, that means we have to remove the original matrix with 1 layer of zero
    Creates a new matrix */
    Matrix *removedPaddingMatrix;
    int newNumberOfRows;
    int newNumberOfCols;
    int rowIndex;
    int colIndex;

    if (paddingAmountToRemove*2 >= originalMatrix->numberRows || paddingAmountToRemove*2 >= originalMatrix->numberCols) {
        return NULL; /* NULL means error */
    }

    newNumberOfRows = originalMatrix->numberRows - paddingAmountToRemove*2;
    newNumberOfCols = originalMatrix->numberCols - paddingAmountToRemove*2;

    removedPaddingMatrix = createMatrix(newNumberOfRows, newNumberOfCols);
    

    for (rowIndex = 0; rowIndex < newNumberOfRows; rowIndex++) {
        for (colIndex = 0; colIndex < newNumberOfCols; colIndex++) {
            setMatrixElement(removedPaddingMatrix, rowIndex, colIndex, 
                getMatrixElement(originalMatrix, rowIndex + paddingAmountToRemove, colIndex+paddingAmountToRemove));
        }
    }

    return removedPaddingMatrix;

}

int matricesAreEqual(Matrix *firstMatrix, Matrix *secondMatrix) {

    int rowIndex;
    int columnIndex;

    /* Firstly, check the metadata of the matrices 
    */
    if (firstMatrix->numberRows != secondMatrix->numberRows ||
        firstMatrix->numberCols != secondMatrix->numberCols ||
        firstMatrix->size != secondMatrix->size
    ) {
        return 0; /* 0 for false */
    }
    
    /* Secondly, check the data of the matrices. As from the above's check, 
    their dimensions (number of rows and columns) are the same, we can use one of the matrices' dimensions
    */
    for (rowIndex = 0; rowIndex < firstMatrix->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < secondMatrix->numberCols; columnIndex++) {
            if (getMatrixElement(firstMatrix, rowIndex, columnIndex) != getMatrixElement(secondMatrix, rowIndex, columnIndex)) {
                return 0;
            }
        }
    }

    return 1;

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

Matrix * addPaddingToMatrix(Matrix * matrix, int padding) {
    int newNumberRows = matrix->numberRows + 2 * padding;
    int newNumberCols = matrix->numberCols + 2 * padding;
    Matrix * newMatrix = createMatrix(newNumberRows, newNumberCols);

    int row, col;
    for (row = 0; row < newNumberRows; row++) {
        for (col = 0; col < newNumberCols; col++) {
            if (row < padding || row >= newNumberRows - padding || col < padding || col >= newNumberCols - padding) {
                setMatrixElement(newMatrix, row, col, 0);
            } else {
                setMatrixElement(newMatrix, row, col, getMatrixElement(matrix, row - padding, col - padding));
            }
        }
    }
    return newMatrix;
}

Matrix * removePaddingFromMatrix(Matrix * matrix, int padding) {
    int newNumberRows = matrix->numberRows - 2 * padding;
    int newNumberCols = matrix->numberCols - 2 * padding;
    Matrix * newMatrix = createMatrix(newNumberRows, newNumberCols);

    int row, col;
    for (row = 0; row < newNumberRows; row++) {
        for (col = 0; col < newNumberCols; col++) {
            setMatrixElement(newMatrix, row, col, getMatrixElement(matrix, row + padding, col + padding));
        }
    }
    return newMatrix;
}

Matrix * getSurroundingElements(Matrix * matrix, int targetRow, int targetCol, int kernelSize) {
    Matrix * newMatrix = createMatrix(kernelSize, kernelSize);
    int radius = kernelSize / 2;
    int temp;

    int row, col;
    for (row = 0; row < kernelSize; row++) {
        for (col = 0; col < kernelSize; col++) {
            temp = getMatrixElement(matrix, row + row - radius, col + col - radius);
            setMatrixElement(newMatrix, row, col, temp);
        }
    }
    return newMatrix;
}

int matrixDotProduct(Matrix * matrix1, Matrix * matrix2, int kernelSize) {
    int sum = 0;
    int row, col;
    int temp1, temp2;
    for (row = 0; row < kernelSize; row++) {
        for (col = 0; col < kernelSize; col++) {
            temp1 = getMatrixElement(matrix1, row, col);
            temp2 = getMatrixElement(matrix2, row, col);
            sum += temp1 * temp2;
        }
    }
    return sum;
}

Matrix * applyKernel(Matrix * matrix, Matrix * kernel) {
    Matrix * newMatrix, * paddedMatrix, * surroundingElements;
    int temp, radius;
    int row, col;
    if (kernel->numberRows % 2 == 0 && kernel->numberRows != kernel->numberCols) {
        printf("wrong size");
        return matrix;
    }

    newMatrix = createMatrix(matrix->numberRows, matrix->numberCols);
    paddedMatrix = addPaddingToMatrix(matrix, kernel->numberRows / 2);
    radius = kernel->numberRows / 2;
    
    for (row = radius; row < matrix->numberRows + radius; row++) {
        for (col = radius; col < matrix->numberCols + radius; col++) {
            surroundingElements = getSurroundingElements(paddedMatrix, row, col, kernel->numberRows);
            temp = matrixDotProduct(surroundingElements, kernel, kernel->numberRows);
            setMatrixElement(newMatrix, row - radius, col - radius, temp);
            freeMatrix(surroundingElements);
        }
    }
    freeMatrix(paddedMatrix);
    return newMatrix;
}

int pythagoreanAddition(int x, int y) {
    return (int) sqrt(x * x + y * y);
}

Matrix * getGradientOfPixel(Matrix * target, Matrix * xKernel, Matrix * yKernel) {
    Matrix * gx = applyKernel(target, xKernel);
    Matrix * gy = applyKernel(target, yKernel);
    Matrix * g = createMatrix(target->numberRows, target->numberCols);

    int row, col;
    for (row = 0; row < g->numberRows; row++) {
        for (col = 0; col < g->numberCols; col++) {
            int x = getMatrixElement(gx, row, col);
            int y = getMatrixElement(gy, row, col);
            int gradient = pythagoreanAddition(x, y);
            setMatrixElement(g, row, col, gradient);
        }
    }
    freeMatrix(gx);
    freeMatrix(gy);

    return g;
}