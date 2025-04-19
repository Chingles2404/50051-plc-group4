# Bitmap Image Reader and Grayscale Converter

(The `process_bitmap/` module compiles to a binary named **`bitmap_processor`** when built using `make`.)

This module reads 24-bit uncompressed `.bmp` (bitmap) image files, extracts their pixel data, converts it to grayscale, and prints the first few grayscale values to verify correctness.



## Files

- **`main.c`**  
  Entry point for testing. Loads and processes a bitmap file.

- **`bitmap.c` / `bitmap.h`**  
  Contains functions for:
  - Opening a BMP file
  - Validating the BMP file format
  - Reading BMP headers
  - Reading and converting pixel data to grayscale

- **`utils.c` / `utils.h`**  
  Contains utility functions like `handleMemory` for error-safe memory allocation.

- **`example.bmp`**  
  A sample BMP image for testing. Must be a 24-bit uncompressed BMP file.

- **`Makefile`**  
  Simplifies compiling and running the project.



## Algorithm

### Steps:

1. **Open File**  
   Opens the file in binary mode (`rb`). If opening fails, the program exits.

2. **Validate Bitmap**  
   Checks if the file starts with the `BM` signature (hex `0x4D42`), which indicates a valid BMP file.

3. **Read Headers**  
   - `FILEHEADER`: Basic file info (type, size, offset to pixel data).
   - `INFOHEADER`: Image info (width, height, bit depth).

4. **Read and Convert Pixel Data**  
   - Skips any row padding to align data correctly.
   - Extracts RGB values and converts each pixel to grayscale using the **luminance formula**:
     ```
     grayscale = 0.3 * R + 0.59 * G + 0.11 * B
     ```

5. **Display Sample Output**  
   - Prints the first 10 grayscale pixel values for quick verification.



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
