#include "copy.h"

void copyImage() {
    FILE *streamIn = fopen("images/cameraman.bmp", "rb");
    FILE *fout = fopen("images/cameraman_copy.bmp", "wb");

    if (streamIn == NULL || fout == NULL) {
        printf("Unable to open file\n");
        return;
    }

    unsigned char header[54];
    unsigned char colorTable[1024];

    // Extracting the image header
    fread(header, sizeof(unsigned char), 54, streamIn);

    // Getting the width, height, and bit depth from the image header
    int width = *(int *)(&header[18]);
    int height = *(int *)(&header[22]);
    int bitDepth = *(int *)(&header[28]);

    if (bitDepth <= 8) {
        fread(colorTable, sizeof(unsigned char), 1024, streamIn);
    }

    fwrite(header, sizeof(unsigned char), 54, fout);
    // Buffer to store the image data
    // Dinamically allocated because C is funny (I LOVE C)
    unsigned char *buff = (unsigned char *)malloc(height * width * sizeof(unsigned char));

    if (buff == NULL) {
        printf("Memory allocation failed\n");
        return;
    }


    // Read image data
    fread(buff, sizeof(unsigned char), (height * width), streamIn);

    if (bitDepth <= 8) {
        fwrite(colorTable, sizeof(unsigned char), 1024, fout);
    }

    fwrite(buff,sizeof(unsigned char), (height*width), fout);
    // Close the files
    fclose(streamIn);
    fclose(fout);

    printf("Process... done\n");
    printf("Width: %d \n", width);
    printf("Height: %d \n", height);
}
