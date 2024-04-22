#include "histogram_equalization.h"

void imageReader(const char *imgName, int *height, int *width, unsigned char *header, unsigned char colorTable, unsigned char *buf, int bitDepth){
    FILE *fOut = fopen(imgName, "wb");
    fwrite(header, sizeof(unsigned char), 54, fOut);

    if(bitDepth <= 8){
        fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
    }
    fwrite(buf, sizeof(unsigned char), CUSTOM_IMG_SIZE, fOut);
    fclose(fOut);
}

void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable, unsigned char *buf, int bitDepth){
    FILE *fIn = fopen(imgName, "rb");

    if(fIn == (FILE *) 0){
        printf("Unable to read the image :(\n");

    }
    for(int i = 0; i < HEADER_SIZE; i++){
        header[i] = getc(fIn);
    }

    fread(buf, sizeof(unsigned char), CUSTOM_IMG_SIZE, fIn);
    fclose(fIn);
}

void imageHistogram (unsigned char* imageData , int imageRows, int imageColumns, float histogram[]){
    FILE* fptr;
    fptr = fopen("image_hist.txt", "w");
    int x,y;
    
    long int iHist[255];
    long int sum = 0;

    int i;
    int j;

    for(i = 0; i < 255; i++){
        iHist[i] = 0;
    }

    for(y = 0; y < imageRows; y++){
        for (x = 0; x < imageColumns; i++){
           j = *(imageData + x + (y*imageColumns));
           iHist[j] = iHist[j] + 1;
           sum++; 
        }
    }

    for(i = 0; i < 255; i++){
        histogram[i] = (float)iHist[i]/(float)sum;
    }

    for(i = 0; i < 255; i++){
        fprintf(fptr,"\n%f", histogram[i]);
    }
}

void histogram_equalization (unsigned char *input_image_data, unsigned char *output_image_data, int img_rows, int img_columns){
    int hist_eq[256];

    float hist[256];
    float sum;

    imageHistogram(&input_image_data[0], img_rows, img_columns, &output_image_data[0]);

    for(int i = 0; i < 255; i++){
        sum = 0.0;
        for(int j = 0; j < i; j++){
            sum = sum + hist[j];
        }
        hist_eq[i] = (255 * sum + 0.5);
    }

    for(int y = 0; y < img_rows; y++){
        for(int x = 0; x < img_columns; x++){
            *(output_image_data + x + y * img_columns) = hist_eq[*(input_image_data + x + y * img_columns)];
        }
    }
}