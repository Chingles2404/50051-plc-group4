#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "edge_detection.h"

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

Matrix * getSurroundingElements(Matrix * matrix, int targetRow, int targetCol, int kernelSize) {
    Matrix * newMatrix = createMatrix(kernelSize, kernelSize);
    int radius = kernelSize / 2;
    int temp;

    if (targetRow < radius || targetRow >= matrix->numberRows - radius ||
        targetCol < radius || targetCol >= matrix->numberCols - radius) {
        return newMatrix;
    }

    int row, col;
    for (row = 0; row < kernelSize; row++) {
        for (col = 0; col < kernelSize; col++) {
            temp = getMatrixElement(matrix, targetRow + row - radius, targetCol + col - radius);
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
    if (kernel->numberRows % 2 == 0 || kernel->numberRows != kernel->numberCols) {
        printf("wrong size");
        return matrix;
    }

    newMatrix = createMatrix(matrix->numberRows, matrix->numberCols);
    paddedMatrix = createPaddedMatrixWithZeros(matrix, kernel->numberRows / 2);
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

Matrix * gradientPipeline(Matrix * target) {
    Matrix * xKernel = createMatrix(3, 3);
    setMatrixElement(xKernel, 0, 0, -1);
    setMatrixElement(xKernel, 0, 1, 0);
    setMatrixElement(xKernel, 0, 2, 1);
    setMatrixElement(xKernel, 1, 0, -2);
    setMatrixElement(xKernel, 1, 1, 0);
    setMatrixElement(xKernel, 1, 2, 2);
    setMatrixElement(xKernel, 2, 0, -1);
    setMatrixElement(xKernel, 2, 1, 0);
    setMatrixElement(xKernel, 2, 2, 1);

    Matrix * yKernel = createMatrix(3, 3);
    setMatrixElement(yKernel, 0, 0, -1);
    setMatrixElement(yKernel, 0, 1, -2);
    setMatrixElement(yKernel, 0, 2, -1);
    setMatrixElement(yKernel, 1, 0, 0);
    setMatrixElement(yKernel, 1, 1, 0);
    setMatrixElement(yKernel, 1, 2, 0);
    setMatrixElement(yKernel, 2, 0, 1);
    setMatrixElement(yKernel, 2, 1, 2);
    setMatrixElement(yKernel, 2, 2, 1);

    Matrix * gradient = getGradientOfPixel(target, xKernel, yKernel);

    freeMatrix(xKernel);
    freeMatrix(yKernel);

    return gradient;
}


Matrix** chunkImage(Matrix* image, int chunkSize) {
    int rows = image->numberRows;
    int cols = image->numberCols;
    
    /* Calculate how many complete chunks we can fit */
    int chunkRows = rows / chunkSize;
    int chunkCols = cols / chunkSize;
    
    /* Allocate array of Matrix pointers */
    Matrix** chunks = (Matrix**)malloc(chunkRows * chunkCols * sizeof(Matrix*));
    if (chunks == NULL) return NULL;
    
    int chunkIndex = 0;
    for (int r = 0; r < chunkRows; r++) {
        for (int c = 0; c < chunkCols; c++) {
            /* Create a new matrix for this chunk */
            chunks[chunkIndex] = createMatrix(chunkSize, chunkSize);
            
            /* Fill with data from the original image */
            for (int y = 0; y < chunkSize; y++) {
                for (int x = 0; x < chunkSize; x++) {
                    int srcY = r * chunkSize + y;
                    int srcX = c * chunkSize + x;
                    int val = getMatrixElement(image, srcY, srcX);
                    setMatrixElement(chunks[chunkIndex], y, x, val);
                }
            }
            
            chunkIndex++;
        }
    }
    
    return chunks;
}