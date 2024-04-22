#include "histogram_equalization.h"

int main (){
    int imgWidth, imgHeight, imgBitDepth;
    unsigned char *imgHeader = (unsigned char *) malloc (HEADER_SIZE * sizeof(unsigned char));
    unsigned char *imgColorTable = (unsigned char *) malloc(COLOR_TABLE_SIZE * sizeof(unsigned char));
    unsigned char *imgBuffer = (unsigned char *) malloc(CUSTOM_IMG_SIZE * sizeof(unsigned char));

    const char* imagePath = "images/houses.bmp";
    imageReader(imagePath, &imgHeight, &imgWidth, &imgBitDepth, &imgHeader[0], &imgColorTable[0], &imgBuffer[0]);
    imageHistogram(&imgBuffer[0], imgHeight, imgWidth, &IMG_HIST[0]);

    return 0;
}