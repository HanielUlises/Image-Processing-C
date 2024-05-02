#include <stdio.h>
#include <stdlib.h>

struct kernel{
    int rows;
    int columns;
    unsigned char *Data;
};

void convolve (int img_rows, int img_cols, struct kernel *mask, unsigned char *input_buffer, unsigned char *output_buffer);