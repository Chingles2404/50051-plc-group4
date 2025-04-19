# Matching 3x3 Matrix with ASCII Template

(The `ascii/` module compiles to a binary named **`ascii_matcher`** when built using `make`.)

This module matches a 3x3 edge matrix to the most visually similar ASCII character using a weighted Mean Squared Error (MSE) comparison against predefined templates.

## Algorithm

1. **Edge Matrix Input**  
   Each input is a 3x3 matrix representing detected edges. Values may be binary (0 or 1) or grayscale (0–255).

2. **Normalization (If Grayscale)**  
   If the input is grayscale, values are normalized to binary using a threshold (values > 128 become `1`, else `0`).

3. **Template Matching**  
   The input is compared against predefined ASCII templates (like `'/'`, `'\\'`, `'|'`, `'-'`, `'X'`, `'+'`, etc).

4. **Mean Squared Error (MSE) Calculation**  
    For each template:
    - Compute the **MSE** between the input and the template.
    - A **weight** is applied as penalty based on the character type for visual mismatches.

5. **Best Match Selection**  
   The template with the **lowest weighted MSE** is selected as the most visually similar ASCII character.



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
