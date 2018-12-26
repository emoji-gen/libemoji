#ifndef SRC_EG_GENERATOR_H_
#define SRC_EG_GENERATOR_H_

#include <string>
#include <vector>

#include "SkCanvas.h"
#include "SkColor.h"
#include "SkScalar.h"
#include "SkTypeface.h"
#include "utils/SkTextUtils.h"

class EgGenerator final {
public:
    EgGenerator();

    sk_sp<SkData> generate();

    void setText(const char *text);
    void setWidth(uint32_t width) noexcept { fWidth = SkIntToScalar(width); }
    void setHeight(uint32_t height) noexcept {
        fHeight = SkIntToScalar(height);
    }
    void setTextAlign(SkTextUtils::Align align) noexcept { fTextAlign = align; }
    void setTextSizeFixed(bool textSizeFixed) noexcept {
        fTextSizeFixed = textSizeFixed;
    }
    void setDisableStretch(bool disableStretch) noexcept {
        fDisableStretch = disableStretch;
    }
    void setTypefaceFromName(const char *familyName);
    void setTypefaceFromFile(const char *path);
    void setColor(SkColor color) noexcept { fColor = color; }
    void setBackgroundColor(SkColor color) noexcept {
        fBackgroundColor = color;
    }
    void setFormat(SkEncodedImageFormat format) noexcept { fFormat = format; }
    void setQuality(int quality) noexcept { fQuality = quality; }

private:
    std::vector<std::string> fTexts;
    SkScalar fWidth = SkIntToScalar(128);
    SkScalar fHeight = SkIntToScalar(128);
    SkTextUtils::Align fTextAlign = SkTextUtils::kCenter_Align;
    bool fTextSizeFixed = false;
    bool fDisableStretch = false;
    sk_sp<SkTypeface> fTypeface;
    SkColor fColor = SK_ColorBLACK;
    SkColor fBackgroundColor = SK_ColorTRANSPARENT;
    SkEncodedImageFormat fFormat = SkEncodedImageFormat::kPNG;
    int fQuality = 100;
};

#endif
