#include <stdio.h>
#include <assert.h>
#include "matrices.h"
#include "test_matrices.h"

void test_create_and_free_2DArray() {

    /* Test for 5 by 10 array - normal case */
    int rows;
    int cols;
    int **array;

    rows = 5;
    cols = 10;

    array = create2DArray(rows, cols);
    assert(array != NULL);

    array[0][0] = 3;
    array[4][9] = 99;

    assert(array[0][0] == 3);
    assert(array[4][9] == 99);
    
    free2DArray(array, rows);
    
    printf("test_create_and_free_2DArray passed\n");
}

void test_create_and_free_matrix() {

    /* Test for 1 by 1 matrix - edge case */
    Matrix* m = createMatrix(1, 1);
    assert(m != NULL);
    
    assert(m->numberRows == 1);
    assert(m->numberCols == 1);
    assert(m->size == m->numberRows * m->numberCols);

    setMatrixElement(m, 0, 0, 7);
    assert(getMatrixElement(m, 0, 0) == 7);
    
    freeMatrix(m);

    /* Test for n by n matrix - large values to simulate image pixel matrix */
    m = createMatrix(1000, 1000);
    assert(m != NULL);

    assert(m->numberRows == 1000);
    assert(m->numberCols == 1000);
    assert(m->size == m->numberRows * m->numberCols);

    setMatrixElement(m, 0, 0, 7);
    assert(getMatrixElement(m, 0, 0) == 7);
    setMatrixElement(m, 999, 999, 3);
    assert(getMatrixElement(m, 999, 999) == 3);

    freeMatrix(m);

    /* Test for the 3 by 3 matrix - for kernel purposes */
    m = createMatrix(3, 3);
    assert(m != NULL);
    
    assert(m->numberRows == 3);
    assert(m->numberCols == 3);
    assert(m->size == m->numberRows * m->numberCols);

    setMatrixElement(m, 0, 0, 7);
    assert(getMatrixElement(m, 0, 0) == 7);

    freeMatrix(m);

    printf("test_create_and_free_matrix passed\n");
}

void test_create_and_free_padded_matrix() {

    /* Test for 1 by 1 matrix - edge case
    Assumes that the createMatrix() is correct
    Padding amount is set to 1
    */
    Matrix* m;
    Matrix* paddedM;
    int paddingAmount;
    
    m = createMatrix(1, 1);
    paddingAmount = 1;
    paddedM = createPaddedMatrixWithZeros(m, paddingAmount);

    assert(m != NULL);
    assert(paddedM != NULL);

    assert(paddedM->numberRows == m->numberRows + paddingAmount*2);
    assert(paddedM->numberCols == m->numberCols + paddingAmount*2);
    assert(paddedM->size == paddedM->numberRows * paddedM->numberCols);

    /* Top left of padded matrix */
    assert(getMatrixElement(paddedM, 0, 0) == 0);
    
    /* Top right of padded matrix */
    assert(getMatrixElement(paddedM, 2, 0) == 0);
    
    /* Bottom left of padded matrix */
    assert(getMatrixElement(paddedM, 0, 2) == 0);
    
    /* Bottom right of padded matrix */
    assert(getMatrixElement(paddedM, 2, 2) == 0);
    
    setMatrixElement(paddedM, 0, 0, 7);
    assert(getMatrixElement(paddedM, 0, 0) == 7);

    setMatrixElement(paddedM, 1, 1, 5);
    assert(getMatrixElement(paddedM, 1, 1) == 5);

    setMatrixElement(paddedM, 2, 2, 3);
    assert(getMatrixElement(paddedM, 2, 2) == 3);

    freeMatrix(m);
    freeMatrix(paddedM);



    /* Test for n by n matrix - normal matrix with padding amount set to 3
    Assumes that the createMatrix() is correct
    Padding amount is set to 3
    */
    m = createMatrix(1000, 1000);
    paddingAmount = 3;
    paddedM = createPaddedMatrixWithZeros(m, paddingAmount);


    assert(m != NULL);
    assert(paddedM != NULL);

    assert(paddedM->numberRows == m->numberRows + paddingAmount*2);
    assert(paddedM->numberCols == m->numberCols + paddingAmount*2);
    assert(paddedM->size == paddedM->numberRows * paddedM->numberCols);

    /* Top left of padded matrix */
    assert(getMatrixElement(paddedM, 0, 0) == 0);
    assert(getMatrixElement(paddedM, 2, 2) == 0);

    /* Top right of padded matrix */
    assert(getMatrixElement(paddedM, 0, 1005) == 0);
    assert(getMatrixElement(paddedM, 2, 1005) == 0);

    /* Bottom left of padded matrix */
    assert(getMatrixElement(paddedM, 1003, 2) == 0);
    assert(getMatrixElement(paddedM, 1005, 0) == 0);

    /* Bottom right of padded matrix */
    assert(getMatrixElement(paddedM, 1003, 1003) == 0);
    assert(getMatrixElement(paddedM, 1005, 1005) == 0);

    /* Bottom right padded matrix value set check */
    setMatrixElement(paddedM, 0, 0, 7);
    assert(getMatrixElement(paddedM, 0, 0) == 7);

    /* Normal padded matrix value set check */
    setMatrixElement(paddedM, 500, 500, 3);
    assert(getMatrixElement(paddedM, 500, 500) == 3);
    
    /* Bottom right padded matrix value set check */
    setMatrixElement(paddedM, 1003, 1003, 9);
    assert(getMatrixElement(paddedM, 1003, 1003) == 9);

    setMatrixElement(paddedM, 1005, 1005, 5);
    assert(getMatrixElement(paddedM, 1005, 1005) == 5);

    freeMatrix(m);
    freeMatrix(paddedM);

    printf("test_create_and_free_padded_matrix passed\n");
}


void test_matrices_are_same() {

    Matrix* A;
    Matrix* B;
    
    /* Same element values test */
    A = createMatrix(2, 2);
    B = createMatrix(2, 2);

    setMatrixElement(A, 0, 0, 1);
    setMatrixElement(A, 0, 1, 2);
    setMatrixElement(A, 1, 0, 3);
    setMatrixElement(A, 1, 1, 4);

    setMatrixElement(B, 0, 0, 1);
    setMatrixElement(B, 0, 1, 2);
    setMatrixElement(B, 1, 0, 3);
    setMatrixElement(B, 1, 1, 4);

    assert(matricesAreEqual(A, B) == 1);

    freeMatrix(A);
    freeMatrix(B);



    /* Different element values test */
    A = createMatrix(2, 2);
    B = createMatrix(2, 2);

    setMatrixElement(A, 0, 0, 1);
    setMatrixElement(A, 0, 1, 2);
    setMatrixElement(A, 1, 0, 3);
    setMatrixElement(A, 1, 1, 4);

    setMatrixElement(B, 0, 0, 1);
    setMatrixElement(B, 0, 1, 2);
    setMatrixElement(B, 1, 0, 99); /* Different value */
    setMatrixElement(B, 1, 1, 4);

    assert(matricesAreEqual(A, B) == 0);

    freeMatrix(A);
    freeMatrix(B);



    /* Different dimensions test */
    A = createMatrix(2, 2);
    B = createMatrix(3, 3);

    assert(matricesAreEqual(A, B) == 0);

    freeMatrix(A);
    freeMatrix(B);

    printf("test_matrices_are_same passed\n");
}


void test_removal_of_padding_from_matrix() {
    
    /* Test for 1 by 1 matrix - edge case
    Assumes that the createMatrix() is correct
    Assumes that createMatrixWithRemovedPadding() is correct
    Assumes that matricesAreEqual() is correct
    Padding amount is set to 1
    */

    Matrix* m;
    Matrix* paddedM;
    Matrix* removedPaddingM;
    int paddingAmount;
    
    m = createMatrix(1, 1);
    paddingAmount = 1;
    paddedM = createPaddedMatrixWithZeros(m, paddingAmount);
    removedPaddingM = createMatrixWithRemovedPadding(paddedM, paddingAmount);
    
    assert(matricesAreEqual(paddedM, removedPaddingM) == 0);
    assert(matricesAreEqual(m, removedPaddingM) == 1);

    freeMatrix(m);
    freeMatrix(paddedM);
    freeMatrix(removedPaddingM);



    /* Test for n by n matrix - normal matrix with padding amount set to 3
    Assumes that the createMatrix() is correct
    Assumes that createMatrixWithRemovedPadding() is correct
    Assumes that matricesAreEqual() is correct
    Padding amount is set to 3
    */
    m = createMatrix(1000, 1000);
    paddingAmount = 3;
    paddedM = createPaddedMatrixWithZeros(m, paddingAmount);
    removedPaddingM = createMatrixWithRemovedPadding(paddedM, paddingAmount);
    
    assert(matricesAreEqual(paddedM, removedPaddingM) == 0);
    assert(matricesAreEqual(m, removedPaddingM) == 1);

    freeMatrix(m);
    freeMatrix(paddedM);
    freeMatrix(removedPaddingM);

    printf("test_removal_of_padding_from_matrix passed\n");
}