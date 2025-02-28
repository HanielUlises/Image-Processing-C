#include "convolution.h"

// Perform 2D discrete convolution on an image using a given mask
void convolve(int img_rows, int img_cols, struct kernel *mask, unsigned char *input_buffer, unsigned char *output_buffer) {
    long i, j, m, n, idx, jdx;
    int ms, im, val;
    unsigned char *temp;

    for(i = 0; i < img_rows; i++) {
        // Iterate over all columns of the input image
        for(j = 0; j < img_cols; j++) {
            val = 0;  

            for(m = 0; m < mask->rows; m++) {
                for(n = 0; n < mask->columns; n++) {
                    ms = (unsigned char)*(mask->Data + m * mask->rows + n);  // Mask value
                    idx = i - m;  // Row index for input image under the mask
                    jdx = j - n;  // Column index for input image under the mask

                    if(idx >= 0 && jdx >= 0 && idx < img_rows && jdx < img_cols) {
                        // Image value
                        im = *(input_buffer + idx * img_cols + jdx);
                    } else {
                        im = 0; 
                    }
                    
                    // Convolution result
                    val += ms * im; 
                }
            }

            // 8-bit range
            if(val > 255) val = 255;
            if(val < 0) val = 0;

            temp = output_buffer + i * img_cols + j;  
            *temp = (unsigned char) val;
        }
    }
}
