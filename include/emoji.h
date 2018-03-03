#ifndef INCLUDE_EMOJI_H_
#define INCLUDE_EMOJI_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Error Codes
typedef int EgError;

#define EG_OK 0
#define EG_INVALID_PARAMETER -1

// Align
//~~~~~~~~~
typedef enum {
    kLeft_Align,
    kCenter_Align,
    kRight_Align,
} EgAlign;


// Format
typedef enum {
    kPNG_Format,
    kWEBP_Format,
} EgFormat;


// Generate Params
//~~~~~~~~~~~~~~~~~~
typedef struct {
    // Text
    const char* fText;

    // Sizes
    uint32_t fWidth;
    uint32_t fHeight;

    // Color
    uint32_t fColor;
    uint32_t fBackgroundColor;

    // Style
    EgAlign fTextAlign;
    bool fTextSizeFixed;

    // Font
    const char* fTypefaceFile;
    const char* fTypefaceName;

    // Image
    EgFormat fFormat;
    int fQuality;
} EgGenerateParams;


// Generate Result
//~~~~~~~~~~~~~~~~~~
typedef struct {
    size_t fSize;
    void* fData;
} EgGenerateResult;

EgError emoji_generate(const EgGenerateParams* params, EgGenerateResult* result);
void emoji_free(EgGenerateResult* const result);

#ifdef __cplusplus
}
#endif

#endif
