/* roundtrip.c: read file -> ZSTD_compress(level) -> ZSTD_decompress -> compare.
   exit 0 = roundtrip OK; exit 1 = lib error or MISMATCH (oracle failure);
   crash -> signal; hang -> caller's timeout. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zstd.h"
int main(int argc, char** argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s <file> [level]\n", argv[0]); return 2; }
    int level = (argc > 2) ? atoi(argv[2]) : 3;
    FILE* f = fopen(argv[1], "rb");
    if (!f) { perror("fopen"); return 2; }
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    char* src = malloc(sz ? sz : 1);
    if (fread(src, 1, sz, f) != (size_t)sz) { fprintf(stderr, "read error\n"); return 2; }
    fclose(f);
    size_t cap = ZSTD_compressBound(sz);
    char* comp = malloc(cap);
    size_t csz = ZSTD_compress(comp, cap, src, sz, level);
    if (ZSTD_isError(csz)) { fprintf(stderr, "compress: %s\n", ZSTD_getErrorName(csz)); return 1; }
    char* dec = malloc(sz ? sz : 1);
    size_t dsz = ZSTD_decompress(dec, sz, comp, csz);
    if (ZSTD_isError(dsz)) { fprintf(stderr, "decompress: %s\n", ZSTD_getErrorName(dsz)); return 1; }
    if (dsz != (size_t)sz || memcmp(src, dec, sz) != 0) { fprintf(stderr, "ROUNDTRIP MISMATCH\n"); return 1; }
    return 0;
}
