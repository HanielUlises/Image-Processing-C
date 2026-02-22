#include "binarization.h"

void adaptive_binarize(const char* inputPath, const char* outputPath, int windowSize, double C) {
    FILE *fIn = fopen(inputPath, "rb");
    FILE *fOut = fopen(outputPath, "wb");

    if (!fIn) {
        printf("Unable to open image\n");
        return;
    }

    unsigned char imgHeader[54];
    unsigned char colorTable[1024];

    fread(imgHeader, sizeof(unsigned char), 54, fIn);
    fwrite(imgHeader, sizeof(unsigned char), 54, fOut);

    int width = *(int*)&imgHeader[18];
    int height = *(int*)&imgHeader[22];
    int bitDepth = *(int*)&imgHeader[28];
    int imgSize = height * width;

    // Read the color table if bit depth is less than or equal to 8
    if (bitDepth <= 8) {
        fread(colorTable, sizeof(unsigned char), 1024, fIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
    }

    // Allocate memory for the image data
    unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * imgSize);
    fread(buffer, sizeof(unsigned char), imgSize, fIn);

    // Adaptive thresholding
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int sum = 0;
            int count = 0;

            // Calculate the mean of the neighboring pixels
            for (int j = -windowSize / 2; j <= windowSize / 2; j++) {
                for (int i = -windowSize / 2; i <= windowSize / 2; i++) {
                    int neighborX = x + i;
                    int neighborY = y + j;
                    if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                        sum += buffer[neighborY * width + neighborX];
                        count++;
                    }
                }
            }

            // Calculate the threshold for the current pixel
            int mean = sum / count;
            int threshold = mean - C;

            // Applying the threshold
            buffer[y * width + x] = (buffer[y * width + x] > threshold) ? WHITE : BLACK;
        }
    }

    fwrite(buffer, sizeof(unsigned char), imgSize, fOut);

    free(buffer);
    fclose(fIn);
    fclose(fOut);
}
