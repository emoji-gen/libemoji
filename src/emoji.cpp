#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "SkData.h"
#include "SkEncodedImageFormat.h"
#include "SkPaint.h"

#include "EgGenerator.h"
#include "emoji.h"

/**
 * Convert EgAlign -> SkPaint::Align
 */
static bool convertToSkAlign(EgAlign src, SkPaint::Align &dest) {
    switch (src) {
    case kLeft_Align:
        dest = SkPaint::kLeft_Align;
        return true;
    case kCenter_Align:
        dest = SkPaint::kCenter_Align;
        return true;
    case kRight_Align:
        dest = SkPaint::kRight_Align;
        return true;
    }
    return false;
}

/**
 * Convert EgFormat -> SkEncodedImageFormat
 */
static bool convertToSkFormat(EgFormat src, SkEncodedImageFormat &dest) {
    switch (src) {
    case kPNG_Format:
        dest = SkEncodedImageFormat::kPNG;
        return true;
    case kWEBP_Format:
        dest = SkEncodedImageFormat::kWEBP;
        return true;
    }
    return false;
}

EgError emoji_generate(const EgGenerateParams *params,
                       EgGenerateResult *result) {
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
    SkPaint::Align align;
    if (!convertToSkAlign(params->fTextAlign, align)) {
        return EG_INVALID_PARAMETER;
    }
    generator.setTextAlign(align);
    generator.setTextSizeFixed(params->fTextSizeFixed);
    generator.setDisableStretch(params->fDisableStretch);

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
    SkEncodedImageFormat format;
    if (!convertToSkFormat(params->fFormat, format)) {
        return EG_INVALID_PARAMETER;
    }
    generator.setFormat(format);
    if (params->fQuality < 0 || params->fQuality > 100) {
        return EG_INVALID_PARAMETER;
    }
    generator.setQuality(params->fQuality);

    // Generate
    sk_sp<SkData> data(generator.generate());
    unsigned char *buf = new unsigned char[data->size()];
    data->copyRange(0, data->size(), buf);

    result->fSize = data->size();
    result->fData = buf;

    return EG_OK;
}

void emoji_free(EgGenerateResult *result) {
    if (result != nullptr) {
        delete[] reinterpret_cast<unsigned char *>(result->fData);
        result->fData = nullptr;
        result->fSize = 0;
    }
}
