#include <stdio.h>
#include "edge_detection.h"

int main(int argc, char **argv) {

    /* Just for testing - START. Can remove this part later onwards */

    Matrix *originalMatrix;
    int value;
    int rowIndex;
    int columnIndex;
    Matrix *paddedMatrix;
    Matrix *removedPaddingMatrix;
    
    originalMatrix = createMatrix(3, 3);

    value = 1;
    for (rowIndex = 0; rowIndex < originalMatrix->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < originalMatrix->numberCols; columnIndex++) {
            setMatrixElement(originalMatrix, rowIndex, columnIndex, value++);
        }
    }

    paddedMatrix = createPaddedMatrixWithZeros(originalMatrix, 1);

    printf("Padded Matrix:\n");
    for (rowIndex = 0; rowIndex < paddedMatrix->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < paddedMatrix->numberCols; columnIndex++) {
            printf("%d ", getMatrixElement(paddedMatrix, rowIndex, columnIndex));
        }
        printf("\n");
    }
    
    removedPaddingMatrix = createMatrixWithRemovedPadding(paddedMatrix, 1);
    printf("Removed-Padding Matrix:\n");
    for (rowIndex = 0; rowIndex < removedPaddingMatrix->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < removedPaddingMatrix->numberCols; columnIndex++) {
            printf("%d ", getMatrixElement(removedPaddingMatrix, rowIndex, columnIndex));
        }
        printf("\n");
    }

    if (matricesAreEqual(originalMatrix, removedPaddingMatrix)) {
        printf("Matrices are equal\n");
    } else {
        printf("Matrices are not equal\n");
    }

    Matrix * surrounding1 = getSurroundingElements(paddedMatrix, 1, 1, 3);
    Matrix * surrounding2 = getSurroundingElements(paddedMatrix, 3, 3, 3);
    int dot = matrixDotProduct(surrounding1, surrounding2, 3);
    printf("Surrounding1:\n");
    for (rowIndex = 0; rowIndex < surrounding1->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < surrounding1->numberCols; columnIndex++) {
            printf("%d ", getMatrixElement(surrounding1, rowIndex, columnIndex));
        }
        printf("\n");
    }
    printf("Surrounding2:\n");
    for (rowIndex = 0; rowIndex < surrounding2->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < surrounding2->numberCols; columnIndex++) {
            printf("%d ", getMatrixElement(surrounding2, rowIndex, columnIndex));
        }
        printf("\n");
    }
    printf("Dot product: %d\n", dot);

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

    Matrix * gx = applyKernel(originalMatrix, xKernel);
    Matrix * gy = applyKernel(originalMatrix, yKernel);
    printf("Gx:\n");
    for (rowIndex = 0; rowIndex < gx->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < gx->numberCols; columnIndex++) {
            printf("%d ", getMatrixElement(gx, rowIndex, columnIndex));
        }
        printf("\n");
    }
    printf("Gy:\n");
    for (rowIndex = 0; rowIndex < gy->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < gy->numberCols; columnIndex++) {
            printf("%d ", getMatrixElement(gy, rowIndex, columnIndex));
        }
        printf("\n");
    }

    Matrix * gradient = getGradientOfPixel(originalMatrix, xKernel, yKernel);
    printf("Gradient:\n");
    for (rowIndex = 0; rowIndex < gradient->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < gradient->numberCols; columnIndex++) {
            printf("%d ", getMatrixElement(gradient, rowIndex, columnIndex));
        }
        printf("\n");
    }

    Matrix * gradientPipelineResult = gradientPipeline(originalMatrix);
    printf("Gradient Pipeline Result:\n");
    for (rowIndex = 0; rowIndex < gradientPipelineResult->numberRows; rowIndex++) {
        for (columnIndex = 0; columnIndex < gradientPipelineResult->numberCols; columnIndex++) {
            printf("%d ", getMatrixElement(gradientPipelineResult, rowIndex, columnIndex));
        }
        printf("\n");
    }
    if (matricesAreEqual(gradient, gradientPipelineResult)) {
        printf("Gradient and Gradient Pipeline Result are equal\n");
    } else {
        printf("Gradient and Gradient Pipeline Result are not equal\n");
    }

    freeMatrix(originalMatrix);
    freeMatrix(paddedMatrix);
    freeMatrix(removedPaddingMatrix);
    freeMatrix(surrounding1);
    freeMatrix(surrounding2);
    freeMatrix(xKernel);
    freeMatrix(yKernel);
    freeMatrix(gx);
    freeMatrix(gy);
    freeMatrix(gradient);
    freeMatrix(gradientPipelineResult);

    /* Just for testing - END */

    printf("Press enter to exit...");
    getchar();

    return 0;
}