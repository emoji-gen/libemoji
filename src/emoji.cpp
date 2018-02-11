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

void emoji_generate(const EgGenerateParams* params, EgGeneratorResult* result) {
    EgGenerator generator;
    generator.setText(params->fText);
    generator.setWidth(params->fWidth);
    generator.setHeight(params->fHeight);
    generator.setTextAlign(convertToSkAlign(params->fTextAlign));
    generator.setTypefaceFromName(params->fTypefaceName);
    generator.setColor(params->fColor);
    generator.setBackgroundColor(params->fBackgroundColor);
    generator.setFormat(convertToSkFormat(params->fFormat));
    generator.setQuality(params->fQuality);

    sk_sp<SkData> data(generator.generate());
    unsigned char* buf = new unsigned char[data->size()];
    data->copyRange(0, data->size(), buf);

    result->fSize = data->size();
    result->fData = buf;
}

