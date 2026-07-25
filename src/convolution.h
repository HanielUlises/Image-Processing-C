#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <stdio.h>
#include <stdlib.h>

struct kernel {
    int rows;
    int columns;
    int *data;
};

/*
 * Spatial convolution over a tightly packed 24bpp BGR buffer.
 *
 * divisor : the kernel weights are summed and then divided by this value
 *           (use 1 for kernels that already sum to one, 9 for a box blur, ...).
 *           A divisor of 0 is treated as 1 to avoid a division by zero.
 * bias    : added after the division, useful to re-center signed results
 *           such as embossing or Sobel gradients around mid gray (128).
 */
void convolve_rgb(
    int width,
    int height,
    struct kernel *mask,
    unsigned char *input_buffer,
    unsigned char *output_buffer,
    int divisor,
    int bias
);

#endif
