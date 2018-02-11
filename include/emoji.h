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
} EgGenerateParams;


// Generate Result
//~~~~~~~~~~~~~~~~~~
typedef struct {
    size_t size;
    void* data;
} EgGenerateResult;

void emoji_generate(const EgGenerateParams* params);
/* void emoji_make(const EmojiParams* params, EmojiResult* const result); */
/* void emoji_free(EmojiResult* const result); */

#ifdef __cplusplus
}
#endif

#endif
