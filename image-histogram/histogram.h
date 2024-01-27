#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define HEADER_SIZE      54
#define COLOR_TABLE_SIZE 1024
#define CUSTOM_IMG_SIZE  512*512

float IMG_HIST [255];

void imageReader(const char *imgName, int *height, int *width, unsigned char *header, unsigned char colorTable, unsigned char *buf);
void imageWriter(const char *imgName, unsigned char *header, unsigned char *colorTable, unsigned char *buf, int bitDepth);
void imageHistogram();