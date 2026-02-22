#include "imageGrayscale.h"

void toGrayScale() {
    FILE *fIn = fopen("images/lena_color.bmp", "rb");
    FILE *fOut = fopen("images/lena_gray.bmp", "wb");

    if (fIn == NULL || fOut == NULL) {
        printf("Unable to open the image\n");
        return;
    }

    unsigned char imgHeader[54];
    unsigned char colorTable[1024];

    fread(imgHeader, sizeof(unsigned char), 54, fIn);
    fwrite(imgHeader, sizeof(unsigned char), 54, fOut);

    // Image properties
    int height = *(int *) &imgHeader[22];
    int width = *(int *) &imgHeader[18];
    int depth = *(int *) &imgHeader[28];

    if (depth <= 8) {
        fread(colorTable, sizeof(unsigned char), 1024, fIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
    }

    int imgSize = height * width;

    // Color buffer that stores the RGB from the image
    unsigned char (*colorBuffer)[3] = (unsigned char (*)[3]) malloc (imgSize * sizeof(unsigned char[3]));
    if (colorBuffer == NULL) {
        printf("Memory allocation failed\n");
        fclose(fIn);
        fclose(fOut);
        return;
    }

    // Color channels 
    for (int i = 0; i < imgSize; i++) {
        colorBuffer[i][0] = getc(fIn); // Red
        colorBuffer[i][1] = getc(fIn); // Green
        colorBuffer[i][2] = getc(fIn); // Blue

        // Temporal variable
        int temp = 0;

        temp = (colorBuffer[i][0] * 0.3) + (colorBuffer[i][1] * 0.59) + (colorBuffer[i][2] * 0.11);

        for (int j = 0; j < 3; j++) {
            putc(temp, fOut);
        }
    }

    printf("\nSuccess.\n");
    fclose(fIn);
    fclose(fOut);
    free(colorBuffer);
}
