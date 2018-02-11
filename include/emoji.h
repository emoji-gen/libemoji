#ifndef INCLUDE_EMOJI_H_
#define INCLUDE_EMOJI_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


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

    // Font
    const char* fTypefacePath;
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

void emoji_generate(const EgGenerateParams* params, EgGenerateResult* result);
void emoji_free(EgGenerateResult* const result);

#ifdef __cplusplus
}
#endif

#endif
