#include <iostream>
#include <string>
#include <vector>

#include "SkPaint.h"

#include "emoji.h"
#include "EgGenerator.h"

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

void emoji_generate(const EgGenerateParams* params) {
    EgGenerator generator;
    generator.setText(params->fText);
    generator.setWidth(params->fWidth);
    generator.setHeight(params->fHeight);
    generator.setTextAlign(convertToSkAlign(params->fTextAlign));
    generator.setTypefaceFromName(params->fTypefaceName);
    generator.setColor(params->fColor);
    generator.setBackgroundColor(params->fBackgroundColor);
    generator.generate();
}

