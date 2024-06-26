#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BMP_HEADER_SIZE 54
#define BMP_COLOR_TABLE_SIZE 1024
#define CUSTOM_IMG_SIZE 1024*1024

void imageReader(const char* imageName, int *_height, int *_width, int *_bitDepth ,unsigned char *_header, unsigned char *_colorTable, unsigned char *_buffer);
void imageWriter(const char* imageName, unsigned char *header, unsigned char *colorTable, unsigned char* buffer, int bitDepth);
void correctBrightness (unsigned char *_input_image_data, unsigned char *_output_image_data, int img_rows, int img_cols, int brightness);