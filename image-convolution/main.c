#include "convolution.h"
#include "../imageWriter/image-writer.h"

int main(int argc, char* argv[]) {
    const char * path_img = "../imageWriter/images";  
    int imgWidth, imgHeight, imgBitDepth;
    unsigned char imgHeader[BMP_HEADER_SIZE];
    unsigned char imgColorTable[BMP_COLOR_TABLE_SIZE];
    unsigned char imgBuffer[CUSTOM_IMG_SIZE];
    unsigned char outputBuffer[CUSTOM_IMG_SIZE];

    imageReader(path_img, &imgWidth, &imgHeight, &imgBitDepth, &imgHeader[0], &imgColorTable[0], &imgBuffer[0]);

    struct kernel mask;
    mask.rows = 3;
    mask.columns = 3;
    unsigned char maskData[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    mask.Data = maskData;

    convolve(imgHeight, imgWidth, &mask, imgBuffer, outputBuffer);
    imageWriter("output_image.bmp", imgHeader, imgColorTable, outputBuffer, imgWidth, imgHeight, imgBitDepth);

    return 0;
}
