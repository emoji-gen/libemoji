#ifndef SRC_EG_GENERATOR_H_
#define SRC_EG_GENERATOR_H_

#include <string>
#include <vector>

#include "SkCanvas.h"
#include "SkColor.h"
#include "SkTypeface.h"
#include "SkScalar.h"

class EgGenerator final {
public:
    EgGenerator();

    sk_sp<SkData> generate();

    void setText(const char* text);
    void setWidth(uint32_t width) { fWidth = SkIntToScalar(width); }
    void setHeight(uint32_t height) { fHeight = SkIntToScalar(height); }
    void setTextAlign(SkPaint::Align align) { fTextAlign = align; }
    void setTypefaceFromName(const char* familyName);
    void setTypefaceFromFile(const char* path);
    void setColor(SkColor color) { fColor = color; }
    void setBackgroundColor(SkColor color) { fBackgroundColor = color; }
    void setFormat(SkEncodedImageFormat format) { fFormat = format; }
    void setQuality(int quality) { fQuality = quality; }

private:
    std::vector<std::string> fTexts;
    SkScalar fWidth = SkIntToScalar(128);
    SkScalar fHeight = SkIntToScalar(128);
    SkPaint::Align fTextAlign = SkPaint::kCenter_Align;
    sk_sp<SkTypeface> fTypeface;
    SkColor fColor = SK_ColorBLACK;
    SkColor fBackgroundColor = SK_ColorTRANSPARENT;
    SkEncodedImageFormat fFormat = SkEncodedImageFormat::kPNG;
    int fQuality = 100;
};

#endif
