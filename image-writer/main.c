#include "imageWriter.h"

int main(int argc, char *argv){
    int imgWidth, imgHeight, imgBitDepth;
    unsigned char imgHeader[BMP_HEADER_SIZE];
    unsigned char imgColorTable[BMP_COLOR_TABLE_SIZE];
    unsigned char imgBuffer[CUSTOM_IMG_SIZE];

    const char imageName[] = "images/bridge.bmp";
    const char newImgName = "images/bride_copy.bmp";

    imageReader(imageName, &imgWidth, &imgHeight, &imgBitDepth, &imgHeader[0], &imgColorTable[0], &imgBuffer[0]);
    imageWriter(newImgName, imgHeader, imgColorTable,imgBuffer, imgBitDepth);

    printf("==== SUCESS ====\n");
    return 0; 
}