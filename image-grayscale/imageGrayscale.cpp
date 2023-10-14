#include "imageGrayscale.h"


void toGrayScale (){
    FILE *fIn = fopen("images/lena_color.bmp", "rb");
    FILE *fOut = fopen("images/lena_gray.bmp", "wb");

    unsigned char imgHeader [54];

    if(fIn == NULL)
        printf("Unable to open the image\n");

    for(int i = 0; i < 54; i++){
        imgHeader[i] = getc(fIn);
    }

    fwrite(imgHeader, sizeof(unsigned char),54, fOut);

    int height = *(int*)&imgHeader[22];
    int width  = *(int*)&imgHeader[18];
    int depth  = *(int*)&imgHeader[28];

    const int imgSize = height * width;

    unsigned char (*colorBuffer)[3] = (unsigned char (*)[3]) malloc(imgSize * sizeof(unsigned char[3]));
    if(colorBuffer == NULL){
        printf("Memory allocation failed\n");
    }
}