#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

#include "core/SkData.h"
#include "core/SkPaint.h"
#include "utils/SkTextUtils.h"

#include "EgGenerator.h"
#include "emoji.h"

/**
 * Convert EgAlign -> SkTextUtils::Align
 */
static bool convertToSkAlign(EgAlign src, SkTextUtils::Align &dest) {
    switch (src) {
    case kLeft_Align:
        dest = SkTextUtils::kLeft_Align;
        return true;
    case kCenter_Align:
        dest = SkTextUtils::kCenter_Align;
        return true;
    case kRight_Align:
        dest = SkTextUtils::kRight_Align;
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
    SkTextUtils::Align align;
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
    generator.setFormat(params->fFormat);
    if (params->fQuality < 0 || params->fQuality > 100) {
        return EG_INVALID_PARAMETER;
    }
    generator.setQuality(params->fQuality);

    // Generate
    sk_sp<SkData> data(generator.generate());
    if (data == nullptr) {
        return EG_GENERATION_FAILED; // 生成失敗
    }

    std::uint8_t *buf = new std::uint8_t[data->size()];
    data->copyRange(0, data->size(), buf);

    result->fSize = data->size();
    result->fData = buf;

    return EG_OK;
}

void emoji_free(EgGenerateResult *result) {
    if (result == nullptr) return;
    if (result->fData == nullptr) return;

    delete[] reinterpret_cast<std::uint8_t *>(result->fData);
    result->fData = nullptr;
    result->fSize = 0;
}
