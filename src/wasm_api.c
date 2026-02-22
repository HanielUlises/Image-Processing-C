/**
 * wasm_api.c
 * 
 * Bridge layer para WebAssembly.
 * Incluye los .c originales SIN MODIFICARLOS y expone funciones
 * que operan en memoria (en vez de archivos en disco).
 * 
 * Los originales usan fopen/fwrite sobre archivos — aquí usamos
 * la FS virtual de Emscripten (MEMFS) para que funcionen igual:
 * escribimos el BMP a un archivo temporal en MEMFS, llamamos la
 * función original, y leemos el resultado de vuelta a memoria.
 */

#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Incluir los originales ─────────────────────────────────────── */
#include "imageGrayscale.h"
#include "imageBrightness.h"
#include "binarization.h"
#include "convolution.h"
#include "imageWriter.h"

/* ── Helpers internos para MEMFS ─────────────────────────────────── */

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

/* ── Allocator de salida expuesto a JS ───────────────────────────── */

static unsigned char *g_output_buf = NULL;
static int            g_output_len = 0;

EMSCRIPTEN_KEEPALIVE
unsigned char *get_output_ptr() { return g_output_buf; }

EMSCRIPTEN_KEEPALIVE
int get_output_len() { return g_output_len; }

EMSCRIPTEN_KEEPALIVE
void free_output() {
    if (g_output_buf) { free(g_output_buf); g_output_buf = NULL; }
    g_output_len = 0;
}

/* ── 1. Grayscale ────────────────────────────────────────────────── *
 * El original (imageGrayscale.c) abre hardcoded "images/lena_color.bmp"
 * y escribe "images/lena_gray.bmp". Creamos esos paths en MEMFS.
 */
EMSCRIPTEN_KEEPALIVE
void wasm_grayscale(const unsigned char *input_bmp, int input_len) {
    free_output();

    /* Crear directorio virtual */
    EM_ASM({ FS.mkdir('/images'); }, 0);

    write_temp_file("/images/lena_color.bmp", input_bmp, input_len);

    /* Llamar función original sin cambios */
    toGrayScale();

    g_output_buf = read_temp_file("/images/lena_gray.bmp", &g_output_len);
}

/* ── 2. Increase Brightness ──────────────────────────────────────── *
 * Original abre "images/lena512.bmp" → "images/lena_increased.bmp"
 */
EMSCRIPTEN_KEEPALIVE
void wasm_increase_brightness(const unsigned char *input_bmp, int input_len) {
    free_output();
    EM_ASM({ try { FS.mkdir('/images'); } catch(e) {} });
    write_temp_file("/images/lena512.bmp", input_bmp, input_len);
    increaseBrightness();
    g_output_buf = read_temp_file("/images/lena_increased.bmp", &g_output_len);
}

/* ── 3. Reduce Brightness ────────────────────────────────────────── */
EMSCRIPTEN_KEEPALIVE
void wasm_reduce_brightness(const unsigned char *input_bmp, int input_len) {
    free_output();
    EM_ASM({ try { FS.mkdir('/images'); } catch(e) {} });
    write_temp_file("/images/lena512.bmp", input_bmp, input_len);
    reduceBrightness();
    g_output_buf = read_temp_file("/images/lena_decreased.bmp", &g_output_len);
}

/* ── 4. Adaptive Binarize ────────────────────────────────────────── *
 * Esta función ya acepta paths como parámetros → más limpio.
 */
EMSCRIPTEN_KEEPALIVE
void wasm_binarize(const unsigned char *input_bmp, int input_len,
                   int window_size, double C) {
    free_output();
    EM_ASM({ try { FS.mkdir('/images'); } catch(e) {} });
    write_temp_file("/images/input_bin.bmp", input_bmp, input_len);
    adaptive_binarize("/images/input_bin.bmp", "/images/output_bin.bmp",
                      window_size, C);
    g_output_buf = read_temp_file("/images/output_bin.bmp", &g_output_len);
}

/* ── 5. Convolution ─────────────────────────────────────────────── *
 * convolve() ya opera sobre buffers en memoria → llamamos directo.
 * El caller pasa el pixel buffer (sin header BMP), ancho, alto,
 * y el kernel como array plano + dimensiones.
 */
EMSCRIPTEN_KEEPALIVE
void wasm_convolve(const unsigned char *header,    int header_len,
                   const unsigned char *pixels,    int pixels_len,
                   int img_rows,  int img_cols,
                   const unsigned char *kernel_data,
                   int k_rows,    int k_cols) {
    free_output();

    struct kernel mask;
    mask.rows    = k_rows;
    mask.columns = k_cols;
    mask.Data    = (unsigned char *)kernel_data;

    unsigned char *out_pixels = (unsigned char *)malloc(pixels_len);

    /* Llamar función original sin cambios */
    convolve(img_rows, img_cols, &mask, (unsigned char *)pixels, out_pixels);

    /* Devolver header + pixels procesados */
    g_output_len = header_len + pixels_len;
    g_output_buf = (unsigned char *)malloc(g_output_len);
    memcpy(g_output_buf, header, header_len);
    memcpy(g_output_buf + header_len, out_pixels, pixels_len);
    free(out_pixels);
}

/* ── 6. Brightness Correction (imageWriter) ──────────────────────── *
 * correctBrightness() opera en buffers → llamamos directo.
 */
EMSCRIPTEN_KEEPALIVE
void wasm_correct_brightness(const unsigned char *header,  int header_len,
                              const unsigned char *pixels,  int pixels_len,
                              int img_rows, int img_cols,
                              int brightness_val) {
    free_output();

    unsigned char *out_pixels = (unsigned char *)malloc(pixels_len);

    /* Llamar función original sin cambios */
    correctBrightness((unsigned char *)pixels, out_pixels,
                      img_rows, img_cols, brightness_val);

    g_output_len = header_len + pixels_len;
    g_output_buf = (unsigned char *)malloc(g_output_len);
    memcpy(g_output_buf, header, header_len);
    memcpy(g_output_buf + header_len, out_pixels, pixels_len);
    free(out_pixels);
}

/* ── Allocar buffer de entrada (para JS → C transfer) ───────────── */
EMSCRIPTEN_KEEPALIVE
unsigned char *alloc_input(int size) {
    return (unsigned char *)malloc(size);
}

EMSCRIPTEN_KEEPALIVE
void free_input(unsigned char *ptr) {
    free(ptr);
}
