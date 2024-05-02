#include "convolution.h"

// Two dimensional (2D) discrete convolution
void convolve (int img_rows, int img_cols, struct kernel *mask, unsigned char *input_buffer, unsigned char *output_buffer){
    long i, j, m, n, idx, jdx;
    int ms, im, val;
    unsigned char *temp;

    // Iterate over all rows of the input image
    for(i = 0; i < img_rows; i++){
        // Iterate over all columns of the input image
        for(j = 0; j < img_cols; j++){
            // Initializing convolution result for current pixel
            val = 0; 
            // Iterate over all rows and columns of the mask
            for(m = 0; m < mask->rows; m++){
                for(n = 0; n < mask->columns; n++){
                    ms = (unsigned char) *(mask->Data + m*mask->rows + n); // Retrieve mask value
                    idx = i - m; // Compute row index for input image under the mask
                    jdx = j - n; // Compute column index for input image under the mask
                    
                    // Ensure indices are within the image bounds
                    if(idx >= 0 && jdx >= 0){
                        // Image value
                        im = *(input_buffer + idx + img_rows + jdx); 
                    }
                    // Convolution result
                    val += ms * im; 
                }
            }
            // Clip value to 8-bit range
            if(val > 255) val = 255;
            if(val < 0) val = 0;

            // Write result to the output buffer
            temp = output_buffer + i * img_rows + j;
            *temp = (unsigned char) val;
        }
    }
}