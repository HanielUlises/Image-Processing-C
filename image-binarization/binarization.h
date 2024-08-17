#ifndef BINARIZATION_H
#define BINARIZATION_H

#include <stdio.h>
#include <stdlib.h>

#define WHITE 255
#define BLACK 0

void adaptive_binarize(const char* inputPath, const char* outputPath, int windowSize, double C);

#endif
