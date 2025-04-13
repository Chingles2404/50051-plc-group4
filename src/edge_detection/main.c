#include <stdio.h>
#include "utils.h"

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

    freeMatrix(originalMatrix);
    freeMatrix(paddedMatrix);
    freeMatrix(removedPaddingMatrix);

    /* Just for testing - END */

    printf("Press enter to exit...");
    getchar();

    return 0;
}
