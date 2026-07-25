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

    int channels  = bitDepth / 8;
    int imageSize = width * height * channels;

    if (channels >= 3) {
        /* Buffer is stored bottom-up as B, G, R triples. Weighted luminance
           (ITU-R BT.601) written back to all three channels. */
        for (int i = 0; i + 2 < imageSize; i += channels) {
            unsigned char b = buffer[i];
            unsigned char g = buffer[i + 1];
            unsigned char r = buffer[i + 2];

            unsigned char gray = (unsigned char)(
                0.299 * r + 0.587 * g + 0.114 * b
            );

            buffer[i]     = gray;
            buffer[i + 1] = gray;
            buffer[i + 2] = gray;
        }
    }
    /* 8bpp (or lower) images are already single channel: nothing to do. */

    imageWriter(outputPath, header, colorTable,
                buffer, width, height, bitDepth);

    free(buffer);
}
