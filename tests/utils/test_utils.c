#include <stdio.h>
#include <assert.h>
#include "utils.h"
#include "test_utils.h"

#include <stdio.h>
#include <assert.h>
#include "utils.h"
#include "test_utils.h"

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
    setMatrixElement(m, 0, 0, 7);
    assert(getMatrixElement(m, 0, 0) == 7);
    freeMatrix(m);

    /* Test for n by n matrix - large values to simulate image pixel matrix */
    m = createMatrix(1000, 1000);
    assert(m != NULL);
    setMatrixElement(m, 0, 0, 7);
    assert(getMatrixElement(m, 0, 0) == 7);
    setMatrixElement(m, 999, 999, 3);
    assert(getMatrixElement(m, 999, 999) == 3);
    freeMatrix(m);

    /* Test for the 3 by 3 matrix - for kernel purposes */
    m = createMatrix(3, 3);
    assert(m != NULL);
    setMatrixElement(m, 0, 0, 7);
    assert(getMatrixElement(m, 0, 0) == 7);
    freeMatrix(m);

    

    printf("test_create_and_free_matrix passed\n");
}