#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "edge_detection.h"

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
    Matrix * xKernel;
    Matrix * yKernel;
    Matrix * gradient;

    xKernel = createMatrix(3, 3);
    setMatrixElement(xKernel, 0, 0, -1);
    setMatrixElement(xKernel, 0, 1, 0);
    setMatrixElement(xKernel, 0, 2, 1);
    setMatrixElement(xKernel, 1, 0, -2);
    setMatrixElement(xKernel, 1, 1, 0);
    setMatrixElement(xKernel, 1, 2, 2);
    setMatrixElement(xKernel, 2, 0, -1);
    setMatrixElement(xKernel, 2, 1, 0);
    setMatrixElement(xKernel, 2, 2, 1);

    yKernel = createMatrix(3, 3);
    setMatrixElement(yKernel, 0, 0, -1);
    setMatrixElement(yKernel, 0, 1, -2);
    setMatrixElement(yKernel, 0, 2, -1);
    setMatrixElement(yKernel, 1, 0, 0);
    setMatrixElement(yKernel, 1, 1, 0);
    setMatrixElement(yKernel, 1, 2, 0);
    setMatrixElement(yKernel, 2, 0, 1);
    setMatrixElement(yKernel, 2, 1, 2);
    setMatrixElement(yKernel, 2, 2, 1);

    gradient = getGradientOfPixel(target, xKernel, yKernel);

    freeMatrix(xKernel);
    freeMatrix(yKernel);

    return gradient;
}


Matrix** chunkImage(Matrix* image, int chunkSize) {

    int rows;
    int cols;

    int chunkRows;
    int chunkCols;

    Matrix** chunks;

    int chunkIndex;

    int r;
    int c;

    int y;
    int x;
    
    int srcY;
    int srcX;
    int val;

    rows = image->numberRows;
    cols = image->numberCols;
    
    /* Calculate how many complete chunks we can fit */
    chunkRows = rows / chunkSize;
    chunkCols = cols / chunkSize;
    
    /* Allocate array of Matrix pointers */
    chunks = (Matrix**)malloc(chunkRows * chunkCols * sizeof(Matrix*));
    if (chunks == NULL) return NULL;
    
    chunkIndex = 0;
    for (r = 0; r < chunkRows; r++) {
        for (c = 0; c < chunkCols; c++) {
            /* Create a new matrix for this chunk */
            chunks[chunkIndex] = createMatrix(chunkSize, chunkSize);
            
            /* Fill with data from the original image */
            for (y = 0; y < chunkSize; y++) {
                for (x = 0; x < chunkSize; x++) {
                    srcY = r * chunkSize + y;
                    srcX = c * chunkSize + x;
                    val = getMatrixElement(image, srcY, srcX);
                    setMatrixElement(chunks[chunkIndex], y, x, val);
                }
            }
            
            chunkIndex++;
        }
    }
    
    return chunks;
}