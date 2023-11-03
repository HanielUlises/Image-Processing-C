#include "binarization.h"

void binarize(){
    FILE *fIn = fopen("images/lighthouse.bmp","rb");
    FILE *fOut = fopen("images/lighthouse_bw.bmp","wb");

    unsigned char imgHeader[54];
    unsigned char colorTable[1024];

    if(!fIn){
        printf("Unable to open image\n");
    }

    for(int i=0; i < 54; i++){
        imgHeader[i] = getc(fIn);
    }

    fwrite(imgHeader, sizeof(unsigned char),54, fOut);

    int height =  *(int*) &imgHeader[22];
    int width  =  *(int*) &imgHeader[18];
    int bitDepth = *(int*) &imgHeader[28];

    int imgSize = height * width;

    if(bitDepth >= height){
        fread(colorTable, sizeof(unsigned char), 1024, fIn); 
        fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
    }

    // Buffer to store the pixel data

    unsigned char* buffer = (unsigned char*) malloc(sizeof(unsigned char) * imgSize);
    fread(buffer, sizeof(unsigned char), imgSize, fIn);

    // Conversion to black or white
    for(int i = 0; i < imgSize; i++){
        buffer[i] = (buffer[i]>THRESHOLD) ? WHITE : BLACK;
    }

    fwrite(buffer, sizeof(unsigned char), imgSize, fOut);
    fclose(fIn);
    fclose(fOut);
}