#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imageGrayscale.h"
#include "imageBrightness.h"
#include "binarization.h"
#include "convolution.h"
#include "imageWriter.h"
#include "histogramEqualization.h"

static void write_temp_file(const char *path, const unsigned char *data, int len) {
    FILE *f = fopen(path, "wb");
    fwrite(data, 1, len, f);
    fclose(f);
}

static unsigned char *read_temp_file(const char *path, int *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) { *out_len = 0; return NULL; }
    fseek(f, 0, SEEK_END);
    *out_len = (int)ftell(f);
    rewind(f);
    unsigned char *buf = (unsigned char *)malloc(*out_len);
    fread(buf, 1, *out_len, f);
    fclose(f);
    return buf;
}

static unsigned char *g_output_buf = NULL;
static int g_output_len = 0;

EMSCRIPTEN_KEEPALIVE
unsigned char *get_output_ptr() { return g_output_buf; }

EMSCRIPTEN_KEEPALIVE
int get_output_len() { return g_output_len; }

EMSCRIPTEN_KEEPALIVE
void free_output() {
    if (g_output_buf) {
        free(g_output_buf);
        g_output_buf = NULL;
    }
    g_output_len = 0;
}

EMSCRIPTEN_KEEPALIVE
void wasm_grayscale(const unsigned char *input_bmp, int input_len) {
    free_output();
    EM_ASM({ try { FS.mkdir('/tmp'); } catch(e) {} });
    write_temp_file("/tmp/input.bmp", input_bmp, input_len);
    toGrayScale("/tmp/input.bmp", "/tmp/output.bmp");
    g_output_buf = read_temp_file("/tmp/output.bmp", &g_output_len);
}

EMSCRIPTEN_KEEPALIVE
void wasm_increase_brightness(const unsigned char *input_bmp, int input_len) {
    free_output();
    EM_ASM({ try { FS.mkdir('/tmp'); } catch(e) {} });
    write_temp_file("/tmp/input.bmp", input_bmp, input_len);
    increaseBrightness("/tmp/input.bmp", "/tmp/output.bmp");
    g_output_buf = read_temp_file("/tmp/output.bmp", &g_output_len);
}

EMSCRIPTEN_KEEPALIVE
void wasm_reduce_brightness(const unsigned char *input_bmp, int input_len) {
    free_output();
    EM_ASM({ try { FS.mkdir('/tmp'); } catch(e) {} });
    write_temp_file("/tmp/input.bmp", input_bmp, input_len);
    reduceBrightness("/tmp/input.bmp", "/tmp/output.bmp");
    g_output_buf = read_temp_file("/tmp/output.bmp", &g_output_len);
}

EMSCRIPTEN_KEEPALIVE
void wasm_binarize(const unsigned char *input_bmp, int input_len,
                   int window_size, double C) {
    free_output();
    EM_ASM({ try { FS.mkdir('/tmp'); } catch(e) {} });
    write_temp_file("/tmp/input.bmp", input_bmp, input_len);
    adaptive_binarize("/tmp/input.bmp", "/tmp/output.bmp",
                      window_size, C);
    g_output_buf = read_temp_file("/tmp/output.bmp", &g_output_len);
}

EMSCRIPTEN_KEEPALIVE
void wasm_convolve(
    unsigned char *header,
    int header_len,
    unsigned char *pixels,
    int pixels_len,
    int height,
    int width,
    unsigned char *kernel_bytes,
    int k_rows,
    int k_cols
) {
    free_output();

    int *kdata = (int*)malloc(k_rows * k_cols * sizeof(int));
    for (int i = 0; i < k_rows * k_cols; i++) {
        kdata[i] = (signed char)kernel_bytes[i];
    }

    struct kernel k;
    k.rows = k_rows;
    k.columns = k_cols;
    k.data = kdata;

    unsigned char *out_pixels =
        (unsigned char*)malloc(pixels_len);

    convolve_rgb(width, height, &k, pixels, out_pixels);

    g_output_len = header_len + pixels_len;
    g_output_buf = (unsigned char*)malloc(g_output_len);

    memcpy(g_output_buf, header, header_len);
    memcpy(g_output_buf + header_len,
           out_pixels, pixels_len);

    free(out_pixels);
    free(kdata);
}

EMSCRIPTEN_KEEPALIVE
void wasm_correct_brightness(const unsigned char *input_bmp, int input_len) {
    free_output();

    EM_ASM({ try { FS.mkdir('/tmp'); } catch(e) {} });

    write_temp_file("/tmp/input.bmp", input_bmp, input_len);

    equalizeHistogramRGB("/tmp/input.bmp", "/tmp/output.bmp");

    g_output_buf = read_temp_file("/tmp/output.bmp", &g_output_len);
}

EMSCRIPTEN_KEEPALIVE
unsigned char *alloc_input(int size) {
    return (unsigned char *)malloc(size);
}

EMSCRIPTEN_KEEPALIVE
void free_input(unsigned char *ptr) {
    free(ptr);
}