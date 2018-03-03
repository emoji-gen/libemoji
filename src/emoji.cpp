#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "SkData.h"
#include "SkPaint.h"
#include "SkEncodedImageFormat.h"

#include "emoji.h"
#include "EgGenerator.h"

/**
 * Convert EgAlign -> SkPaint::Align
 */
static SkPaint::Align convertToSkAlign(EgAlign align) {
    switch (align) {
    case kLeft_Align:
        return SkPaint::kLeft_Align;
    case kCenter_Align:
        return SkPaint::kCenter_Align;
    case kRight_Align:
        return SkPaint::kRight_Align;
    }
}

/**
 * Convert EgFormat -> SkEncodedImageFormat
 */
static SkEncodedImageFormat convertToSkFormat(EgFormat format) {
    switch (format) {
    case kPNG_Format:
        return SkEncodedImageFormat::kPNG;
    case kWEBP_Format:
        return SkEncodedImageFormat::kWEBP;
    }
}

EgError emoji_generate(const EgGenerateParams* params, EgGenerateResult* result) {
    EgGenerator generator;

    // Text
    if (params->fText == nullptr) {
        return EG_INVALID_PARAMETER;
    }
    if (std::strlen(params->fText) == 0) {
        return EG_INVALID_PARAMETER;
    }
    generator.setText(params->fText);

    // Size
    generator.setWidth(params->fWidth);
    generator.setHeight(params->fHeight);

    // Color
    generator.setColor(params->fColor);
    generator.setBackgroundColor(params->fBackgroundColor);

    // Style
    generator.setTextAlign(convertToSkAlign(params->fTextAlign));

    // Font
    if (params->fTypefaceFile != nullptr && params->fTypefaceName != nullptr) {
        return EG_INVALID_PARAMETER;
    }
    if (params->fTypefaceFile != nullptr) {
        if (std::strlen(params->fTypefaceFile) == 0) {
            return EG_INVALID_PARAMETER;
        }
        generator.setTypefaceFromFile(params->fTypefaceFile);
    }
    if (params->fTypefaceName != nullptr) {
        if (std::strlen(params->fTypefaceName) == 0) {
            return EG_INVALID_PARAMETER;
        }
        generator.setTypefaceFromName(params->fTypefaceName);
    }

    // Image
    generator.setFormat(convertToSkFormat(params->fFormat));
    if (params->fQuality < 0 || params->fQuality > 100) {
        return EG_INVALID_PARAMETER;
    }
    generator.setQuality(params->fQuality);

    // Generate
    sk_sp<SkData> data(generator.generate());
    unsigned char* buf = new unsigned char[data->size()];
    data->copyRange(0, data->size(), buf);

    result->fSize = data->size();
    result->fData = buf;

    return EG_OK;
}

void emoji_free(EgGenerateResult* result) {
    if (result != nullptr) {
        delete [] reinterpret_cast<unsigned char*>(result->fData);
        result->fData = nullptr;
        result->fSize = 0;
    }
}
