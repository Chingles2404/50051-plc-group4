# Matching 3x3 Matrix with ASCII Template

## How It Works

1. **Edge Matrix Input**  
   Each input is a 3x3 matrix representing detected edges. Values may be binary (0 or 1) or grayscale (0–255).

2. **Normalization (Optional)**  
   If the input is grayscale, values are normalized to binary using a threshold (values > 128 become `1`, else `0`).

3. **Template Matching**  
   The input is compared against predefined ASCII templates (like `'/'`, `'\\'`, `'|'`, `'-'`, `'X'`, `'+'`).

4. **MSE Calculation**  
   For each template:
   - Compute the **Mean Squared Error (MSE)** between the input and the template.
   - Apply character-specific weights to penalize or favor certain matches.

5. **Best Match Selection**  
   The template with the **lowest weighted MSE** is selected as the most visually similar ASCII character.
