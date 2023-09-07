#include "imageWriter.h"


void imageReader(const char* imageName, int *_height, int *_width, int *_bitDepth ,unsigned char *_header, unsigned char *_colorTable, unsigned char *_buffer){
    FILE *streamIn;
    streamIn =fopen(imageName, "rb");

    if(streamIn == (FILE *) 0){
        printf("Unable to read the image\n");
    }

    for(int i = 0; i < 54; i++){
        _header[i] = getc(streamIn);
    }

    *_width  = * (int*)&_header[18];
    *_height= * (int*)&_header[22];
    *_bitDepth = *(int*)&_header[28];

    if(*_bitDepth <= 8){
        fread(_colorTable, sizeof(unsigned char), 1024, streamIn);;
    }

    fread(_buffer, sizeof(unsigned), CUSTOM_IMG_SIZE, streamIn);
    fclose(streamIn);
}

void imageWriter(const char* imageName, unsigned char *header, unsigned char *colorTable, unsigned char* buffer, int bitDepth){
    FILE *fo = fopen(imageName, "wb");
    fwrite(header, sizeof(unsigned char),54, fo);
    if(bitDepth <= 8){
        fwrite(colorTable, sizeof(unsigned char), 1024, fo);
    }
    fwrite(buffer, sizeof(unsigned char), CUSTOM_IMG_SIZE, fo);
    fclose(fo);
}