#include "imageWriter.h"

int main(int argc, char *argv[]) {
    int imgWidth, imgHeight, imgBitDepth;
    unsigned char imgHeader[BMP_HEADER_SIZE];
    unsigned char imgColorTable[BMP_COLOR_TABLE_SIZE];
    unsigned char imgBuffer[CUSTOM_IMG_SIZE];

    const char imageName[] = "images/bridge.bmp";
    const char newImgName[] = "images/bridge_copy.bmp";
    const char matrixFileName[] = "images/image_matrix.txt"; 

    // Reading the image
    imageReader(imageName, &imgWidth, &imgHeight, &imgBitDepth, &imgHeader[0], &imgColorTable[0], &imgBuffer[0]);
    
    // Writing the image
    imageWriter(newImgName, imgHeader, imgColorTable, imgBuffer, imgWidth, imgHeight, imgBitDepth);

    // Getting the image matrix 
    getImageMatrix(imgBuffer, imgWidth, imgHeight, imgBitDepth, matrixFileName);

    printf("==== SUCCESS ====\n");
    return 0;
}