# Edge Detection Module

(The `edge_detection/` module compiles to a binary named **`edge_detection`** when built using `make`.)

This module performs 2D matrix-based edge detection using **Sobel-like kernels**. It supports padding, kernel application, and gradient magnitude computation using convolution.



## Files

- `main.c`  
  Testing harness for validating core matrix operations and edge detection logic.

- `utils.c` / `utils.h`  
  Core matrix operations and gradient algorithms:
  - Matrix creation, padding, and removal
  - Kernel application via convolution
  - Gradient magnitude calculation
  - Dot product and neighborhood extraction



## Algorithm

### Steps:

1. **Matrix Initialization**  
   Matrix is created using `createMatrix()` and filled with test values to simulate pixel brightness. Each number is a pixel's brightness.

2. **Padding & Depadding**  
    To process the edges properly, matrix is padded with zeros. This allows us to safely apply 3×3 filters without going out of bounds. After filtering, the padding is removed.

3. **Neighborhood Extraction**  
   For each pixel, a k x k (usually 3x3) square of surrounding values is extracted.

4. **Dot Product with Kernels for Detecting Edges**  
    Two Sobel-like filters (xKernel and yKernel) are used. These filters are applied to each 3×3 surrounding area via dot product to detect changes in brightness horizontally (Gx) and vertically (Gy).  

    The purpose of using the dot product is to measure how closely the neighborhood matches the kernel’s pattern, which helps us detect horizontal and vertical edges.

5. **Gradient Computation**  
   At each point, the edge strength is calculated using the Pythagorean theorem:
   ```bash
   G = sqrt(Gx² + Gy²)
   ```
   This combines the horizontal and vertical edge strengths into one number. The number represents how strong the edge is at each pixel.



## How to Run

### Compile the code with `make`
```bash
make
``` 

### Run the program
```bash
make run
```

### Clean the build
```bash
make clean
```
