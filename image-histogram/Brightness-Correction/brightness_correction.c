#include "imageWriter.h"

// Function to read an image file
void imageReader(const char* imageName, int *_height, int *_width, int *_bitDepth ,unsigned char *_header, unsigned char *_colorTable, unsigned char *_buffer){
    FILE *streamIn;
    streamIn = fopen(imageName, "rb");

    if(streamIn == NULL){
        printf("Unable to read the image\n");
    }

    size_t bytesRead = fread(_header, sizeof(unsigned char), 54, streamIn);
    if (bytesRead != 54) {
        printf("Error reading the image header.\n");
        fclose(streamIn);
        return;
    }

    // Read the header of the image
    for(int i = 0; i < 54; i++){
        _header[i] = getc(streamIn);
    }

    // Extract image width, height, and bit depth from the header
    *_width  = * (int*)&_header[18];
    *_height= * (int*)&_header[22];
    *_bitDepth = *(int*)&_header[28];

    // If the image has a bit depth less than or equal to 8, read the color table
    if(*_bitDepth <= 8){
        fread(_colorTable, sizeof(unsigned char), 1024, streamIn);
    }

    // Read the image data into the buffer
    fread(_buffer, sizeof(unsigned), CUSTOM_IMG_SIZE, streamIn);
    fclose(streamIn);
}

// Function to write an image file
void imageWriter(const char* imageName, unsigned char *header, unsigned char *colorTable, unsigned char* buffer, int bitDepth){
    FILE *fo = fopen(imageName, "wb");

    // Write the header of the image 
    fwrite(header, sizeof(unsigned char), 54, fo);

    // If the image has a bit depth less than or equal to 8, write the color table
    if(bitDepth <= 8){
        fwrite(colorTable, sizeof(unsigned char), 1024, fo);
    }

    // Write the image data from the buffer
    fwrite(buffer, sizeof(unsigned char), CUSTOM_IMG_SIZE, fo);
    fclose(fo);
}

void correctBrightness (unsigned char *_input_image_data, unsigned char *_output_image_data, int img_rows, int img_cols, int brightness){
    int i;

    for(int y = 0; y < img_rows; y++){
        for(int x = 0; x < img_cols; x++){
            i = *(_input_image_data + x + y * img_cols) + brightness;

            if(i > 255) i = 255;
            if(i < 0) i = 0;
            *(_output_image_data + x + y * img_cols) = i;
        }
    }
}