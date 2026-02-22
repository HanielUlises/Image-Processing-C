#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void equalizeHistogramRGB(const char *input_path,
                          const char *output_path)
{
    FILE *in = fopen(input_path, "rb");
    if (!in) return;

    FILE *out = fopen(output_path, "wb");
    if (!out) { fclose(in); return; }

    unsigned char header[54];
    fread(header, 1, 54, in);

    int dataOffset = *(int*)&header[10];
    int width      = *(int*)&header[18];
    int height     = abs(*(int*)&header[22]);
    short bpp      = *(short*)&header[28];

    if (bpp != 24) {
        fclose(in);
        fclose(out);
        return;
    }

    int rowSize = (width * 3 + 3) & (~3);
    int imgSize = rowSize * height;

    unsigned char *pixels = (unsigned char*)malloc(imgSize);
    fseek(in, dataOffset, SEEK_SET);
    fread(pixels, 1, imgSize, in);

    int histR[256] = {0};
    int histG[256] = {0};
    int histB[256] = {0};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * rowSize + x * 3;
            histB[pixels[idx]]++;
            histG[pixels[idx + 1]]++;
            histR[pixels[idx + 2]]++;
        }
    }

    int totalPixels = width * height;

    int cdfR[256] = {0};
    int cdfG[256] = {0};
    int cdfB[256] = {0};

    cdfR[0] = histR[0];
    cdfG[0] = histG[0];
    cdfB[0] = histB[0];

    for (int i = 1; i < 256; i++) {
        cdfR[i] = cdfR[i - 1] + histR[i];
        cdfG[i] = cdfG[i - 1] + histG[i];
        cdfB[i] = cdfB[i - 1] + histB[i];
    }

    unsigned char lutR[256];
    unsigned char lutG[256];
    unsigned char lutB[256];

    for (int i = 0; i < 256; i++) {
        lutR[i] = (unsigned char)(255.0 * cdfR[i] / totalPixels);
        lutG[i] = (unsigned char)(255.0 * cdfG[i] / totalPixels);
        lutB[i] = (unsigned char)(255.0 * cdfB[i] / totalPixels);
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * rowSize + x * 3;
            pixels[idx]     = lutB[pixels[idx]];
            pixels[idx + 1] = lutG[pixels[idx + 1]];
            pixels[idx + 2] = lutR[pixels[idx + 2]];
        }
    }

    fwrite(header, 1, 54, out);
    fseek(out, dataOffset, SEEK_SET);
    fwrite(pixels, 1, imgSize, out);

    free(pixels);
    fclose(in);
    fclose(out);
}