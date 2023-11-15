#include "imageBrightness.h"

void increaseBrightness(){
    FILE * fIn = fopen("images/lena512.bmp", "rb");
    FILE * fOut = fopen("images/lena_increased.bmp", "wb");

    unsigned char imgHeader[54];
    unsigned char colorTable[1024];

    for (int i = 0; i < 54; i++){
        imgHeader[i] = getc(fIn);
    }
    fwrite(imgHeader, sizeof(unsigned char), 54, fOut);

    int height = *(int *) &imgHeader[22];
    int width = *(int *) &imgHeader[18];
    int bitDepth = *(int *) & imgHeader[28];

    int imageSize = height * width;

    if (bitDepth <= 8){
        fread(colorTable, sizeof(unsigned char), 1024, fIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
    }

    // Buffer to store the pixel data
    unsigned char* buffer = (unsigned char*) malloc(sizeof(unsigned char) * imageSize);
    
    fread(buffer, sizeof(unsigned char), imageSize, fIn);

    int temp;
    for(int i = 0; i<imageSize; i++){
        temp = buffer[i] + BRIGHTNESS_FACTOR;
        buffer[i] = (temp > MAXIMUM_COLOR) ? MAXIMUM_COLOR : temp;
    }

    fwrite(buffer, sizeof(unsigned char), imageSize, fOut);
    fclose(fIn);
    fclose(fOut);

}

void reduceBrightness(){
    FILE * fIn = fopen("images/lena512.bmp", "rb");
    FILE * fOut = fopen("images/lena_decreased.bmp", "wb");

    unsigned char imgHeader[54];
    unsigned char colorTable[1024];

    for (int i = 0; i < 54; i++){
        imgHeader[i] = getc(fIn);
    }
    fwrite(imgHeader, sizeof(unsigned char), 54, fOut);

    int height = *(int *) &imgHeader[22];
    int width = *(int *) &imgHeader[18];
    int bitDepth = *(int *) & imgHeader[28];

    int imageSize = height * width;

    if (bitDepth <= 8){
        fread(colorTable, sizeof(unsigned char), 1024, fIn);
        fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
    }

    // Buffer to store the pixel data
    unsigned char* buffer = (unsigned char*) malloc(sizeof(unsigned char) * imageSize);
    
    fread(buffer, sizeof(unsigned char), imageSize, fIn);

    int temp;
    for(int i = 0; i<imageSize; i++){
        temp = buffer[i] - DARKNESS_FACTOR;
        buffer[i] = (temp < MINIMUM_COLOR) ? MINIMUM_COLOR : temp;
    }

    fwrite(buffer, sizeof(unsigned char), imageSize, fOut);
    fclose(fIn);
    fclose(fOut);
}