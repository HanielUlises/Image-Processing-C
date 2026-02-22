#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <stdio.h>
#include <stdlib.h>

struct kernel {
    int rows;
    int columns;
    int *data;
};

void convolve_rgb(
    int width,
    int height,
    struct kernel *mask,
    unsigned char *input_buffer,
    unsigned char *output_buffer
);

#endif