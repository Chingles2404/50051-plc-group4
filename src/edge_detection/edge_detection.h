#ifndef EDGE_DETECTION_UTILS_H
#define EDGE_DETECTION_UTILS_H
#include "matrices.h"

Matrix * getSurroundingElements(Matrix * matrix, int targetRow, int targetCol, int kernelSize);
int matrixDotProduct(Matrix * matrix1, Matrix * matrix2, int kernelSize);
Matrix * applyKernel(Matrix * matrix, Matrix * kernel);
int pythagoreanAddition(int x, int y);

Matrix * getGradientOfPixel(Matrix * target, Matrix * xKernel, Matrix * yKernel);

Matrix * gradientPipeline(Matrix * target);

Matrix** chunkImage(Matrix* image, int chunkSize);

#endif