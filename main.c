#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "libpatch/src/patch.h"

void show_help() {
    printf("usage: ppatch [-h] [-o OUT_FILE] ROM_FILE PATCH_FILE\n"
           "  -o  Override default output file path: patch_name.[gba|gbc|gb]\n"
           "  -h  Show this message and exit\n");
}

uint8_t* apply_patch(
    const uint8_t* src_data, size_t src_size,
    const uint8_t* patch_data, size_t patch_size,
    size_t out_size
) {
    enum PatchType type;
    if (PatchError_OK != patch_get_type(&type, patch_data, patch_size)) {
        printf("error: could not identify patch type\n");
        return NULL;
    }

    if (PatchError_OK != patch_get_size(type, &out_size, src_size, patch_data,
                                        patch_size)) {
        printf("error: bad patch size\n");
        return NULL;
    }

    uint8_t* out = malloc(out_size);
    if (!out) {
        printf("error: apply_patch malloc\n");
        return NULL;
    }

    if (PatchError_OK != patch_apply(type, out, out_size, src_data, src_size,
                                     patch_data, patch_size)) {
        // free(out);
        printf("error: patching failed or bad patch type\n");
        return NULL;
    }

    // patch worked, out is now pointing to allocated data.
    // remember to call free when done!
    return out;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        show_help();
        return 1;
    }

    char* src_path = NULL;
    char* patch_path = NULL;
    // TODO: properly make a pretty outpath if -o not supplied
    char* out_path = "patched.gba";
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            show_help();
            return 0;
        } else if (strcmp(argv[i], "-o") == 0) {
            i++;
            if (i == argc) {
                printf("error: -o requires a file path\n");
                return 1;
            }
            out_path = argv[i];
        } else {
            if (src_path == NULL) {
                src_path = argv[i];
            } else if (patch_path == NULL) {
                patch_path = argv[i];
            } else {
                break;
            }
        }
    }
    if (!src_path || !patch_path) {
        printf("error: rom path and patch path is required\n");
        return 1;
    }
    
    // Read base game
    FILE *fsrc = fopen(src_path, "rb");
    if (fsrc == NULL) {
        printf("error opening %s\n", src_path);
        return 1;
    } 
    
    fseek(fsrc, 0L, SEEK_END);
    const size_t src_size = ftell(fsrc);
    rewind(fsrc);
    uint8_t *src_buf = malloc(src_size);
    fread(src_buf, sizeof(src_buf), src_size, fsrc);
    fclose(fsrc);

    // Read patch
    FILE *fpatch = fopen(patch_path, "rb");
    if (fpatch == NULL) {
        printf("error opening %s\n", patch_path);
        return 1;
    } 
    
    fseek(fpatch, 0L, SEEK_END);
    const size_t patch_size = ftell(fpatch);
    rewind(fpatch);
    uint8_t *patch_buf = malloc(patch_size);
    fread(patch_buf, sizeof(patch_buf), patch_size, fpatch);
    fclose(fpatch);

    // Apply patch
    FILE *fout = fopen(out_path, "wb");
    if (fout == NULL) {
        printf("error opening %s\n", out_path);
        return 1;
    } 

    uint8_t* out_buf = apply_patch(src_buf, src_size, patch_buf,
                                   patch_size, src_size);
    if (out_buf == NULL) {
        return 1;
    }
    
    fwrite(out_buf, sizeof(out_buf), src_size, fout);
    fclose(fout);
    printf("Successfully created %s\n", out_path);

    free(src_buf);
    free(patch_buf);
    free(out_buf);
    return 0;
}
