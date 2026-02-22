#include "imageGrayscale.h"
#include "imageWriter.h"
#include <stdlib.h>

void toGrayScale(const char* inputPath, const char* outputPath){

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
        buffer[i] = (unsigned char)(
            0.3 * buffer[i] +
            0.59 * buffer[i] +
            0.11 * buffer[i]
        );
    }

    imageWriter(outputPath, header, colorTable,
                buffer, width, height, bitDepth);

    free(buffer);
}