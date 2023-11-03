#include "binarization.h"

void binarize(){
    FILE *fIn = ("images/lighthouse.bmp","rb");
    FILE *fOut = ("images/lighthouse_bw.bmp","wb");

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
    
}