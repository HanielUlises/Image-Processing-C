#include "imageBrightness.h"
#include "imageWriter.h"
#include <stdlib.h>

#define BRIGHTNESS_FACTOR 100
#define DARKNESS_FACTOR   50
#define MAXIMUM_COLOR     255
#define MINIMUM_COLOR     0

void increaseBrightness(const char* inputPath, const char* outputPath){

    unsigned char header[54];
    unsigned char colorTable[1024];
    int height, width, bitDepth;

    FILE* temp = fopen(inputPath, "rb");
    fseek(temp, 0, SEEK_END);
    int fileSize = ftell(temp);
    rewind(temp);
    fclose(temp);

    unsigned char* buffer = (unsigned char*)malloc(fileSize);

    imageReader(inputPath, &height, &width, &bitDepth,
                header, colorTable, buffer);

    int imageSize = width * height * (bitDepth / 8);

    for(int i = 0; i < imageSize; i++){
        int tempVal = buffer[i] + BRIGHTNESS_FACTOR;
        buffer[i] = (tempVal > MAXIMUM_COLOR) ? MAXIMUM_COLOR : tempVal;
    }

    imageWriter(outputPath, header, colorTable,
                buffer, width, height, bitDepth);

    free(buffer);
}

void reduceBrightness(const char* inputPath, const char* outputPath){

    unsigned char header[54];
    unsigned char colorTable[1024];
    int height, width, bitDepth;

    FILE* temp = fopen(inputPath, "rb");
    fseek(temp, 0, SEEK_END);
    int fileSize = ftell(temp);
    rewind(temp);
    fclose(temp);

    unsigned char* buffer = (unsigned char*)malloc(fileSize);

    imageReader(inputPath, &height, &width, &bitDepth,
                header, colorTable, buffer);

    int imageSize = width * height * (bitDepth / 8);

    for(int i = 0; i < imageSize; i++){
        int tempVal = buffer[i] - DARKNESS_FACTOR;
        buffer[i] = (tempVal < MINIMUM_COLOR) ? MINIMUM_COLOR : tempVal;
    }

    imageWriter(outputPath, header, colorTable,
                buffer, width, height, bitDepth);

    free(buffer);
}