#include "imageWriter.h"

// Function to read an image file
void imageReader(const char* imageName, int *height, int *width, int *bitDepth, unsigned char *header, unsigned char *colorTable, unsigned char *buffer){
    FILE *streamIn = fopen(imageName, "rb");
    if(streamIn == NULL){
        printf("Unable to open the file: %s\n", imageName);
        return;
    }

    // Read the header of the image
    if(fread(header, 1, 54, streamIn) != 54){
        printf("Error reading the image header.\n");
        fclose(streamIn);
        return;
    }

    // Extract image width, height, and bit depth from the header
    *width = *(int*)&header[18];
    *height = *(int*)&header[22];
    *bitDepth = *(int*)&header[28];

    int colorTableSize = (*bitDepth <= 8) ? 1024 : 0;
    int imageSize = (*width) * (*height) * (*bitDepth / 8);

    // If the image has a bit depth less than or equal to 8, read the color table
    if(colorTableSize > 0){
        if(fread(colorTable, 1, colorTableSize, streamIn) != colorTableSize){
            printf("Error reading color table.\n");
            fclose(streamIn);
            return;
        }
    }

    // Read the image data into the buffer
    if(fread(buffer, 1, imageSize, streamIn) != imageSize){
        printf("Error reading image data.\n");
        fclose(streamIn);
        return;
    }

    fclose(streamIn);
}

// Function to write an image file
void imageWriter(const char* imageName, unsigned char *header, unsigned char *colorTable, unsigned char* buffer, int width, int height, int bitDepth){
    FILE *fo = fopen(imageName, "wb");
    if(fo == NULL){
        printf("Unable to create the file: %s\n", imageName);
        return;
    }

    fwrite(header, 1, 54, fo);

    int colorTableSize = (bitDepth <= 8) ? 1024 : 0;
    int imageSize = width * height * (bitDepth / 8);

    // If the image has a bit depth less than or equal to 8, write the color table
    if(colorTableSize > 0){
        fwrite(colorTable, 1, colorTableSize, fo);
    }

    // Write the image data from the buffer
    fwrite(buffer, 1, imageSize, fo);
    fclose(fo);
}

void getImageMatrix(unsigned char *buffer, int width, int height, int bitDepth, const char* filename) {
    if (bitDepth > 8) {
        printf("Only 8-bit or lower images are supported for printing.\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    int bytesPerPixel = bitDepth / 8; 

    // Iterate over each pixel in reverse!! (Important, otherwise the matrix will be 180 degrees flipped)
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            int pos = i * width + j * bytesPerPixel;
            fprintf(file, "%3d ", buffer[pos]); 
        }
        fprintf(file, "\n");
    }


    fclose(file);
}