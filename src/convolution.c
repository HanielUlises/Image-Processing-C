#include "convolution.h"

static int clamp(int v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return v;
}

void convolve_rgb(
    int width,
    int height,
    struct kernel *mask,
    unsigned char *input,
    unsigned char *output
) {
    int kHalfR = mask->rows / 2;
    int kHalfC = mask->columns / 2;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            for (int m = 0; m < mask->rows; m++) {
                for (int n = 0; n < mask->columns; n++) {

                    int yy = y + (m - kHalfR);
                    int xx = x + (n - kHalfC);

                    if (yy >= 0 && yy < height &&
                        xx >= 0 && xx < width) {

                        int imgIdx = (yy * width + xx) * 3;
                        int kVal   = mask->data[m * mask->columns + n];

                        sumB += input[imgIdx]     * kVal;
                        sumG += input[imgIdx + 1] * kVal;
                        sumR += input[imgIdx + 2] * kVal;
                    }
                }
            }

            int outIdx = (y * width + x) * 3;

            output[outIdx]     = (unsigned char)clamp(sumB);
            output[outIdx + 1] = (unsigned char)clamp(sumG);
            output[outIdx + 2] = (unsigned char)clamp(sumR);
        }
    }
}