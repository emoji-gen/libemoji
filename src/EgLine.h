#ifndef SRC_EG_LINE_BOX_H_
#define SRC_EG_LINE_BOX_H_

#include <string>

#include "SkCanvas.h"
#include "SkColor.h"
#include "SkFont.h"
#include "SkPaint.h"
#include "SkRect.h"
#include "SkScalar.h"
#include "SkTextUtils.h"
#include "SkTypeface.h"

/**
 * 行ボックス
 */
class EgLine final {
public:
    struct MeasureSpec {
        SkScalar fTextSize;
        SkScalar fTextScaleX;
        SkRect fBounds;
    };

    EgLine();

    void setText(const std::string &text) { fText = text; }
    void setWidth(SkScalar width) noexcept { fWidth = width; }
    void setLineHeight(SkScalar lineHeight) noexcept {
        fLineHeight = lineHeight;
    }
    void setTypeface(sk_sp<SkTypeface> typeface) { fTypeface = typeface; }
    void setTextAlign(SkTextUtils::Align textAlign) noexcept {
        fTextAlign = textAlign;
    }
    void setColor(SkColor color) noexcept { fColor = color; }
    void setDisableStretch(bool disableStretch) {
        fDisableStretch = disableStretch;
    }

    MeasureSpec measure(SkScalar textSize = SK_ScalarNaN);
    void draw(SkCanvas *canvas, SkScalar y, const MeasureSpec &spec);

private:
    std::string fText = "";
    SkScalar fWidth = SkIntToScalar(128);
    SkScalar fLineHeight = SkIntToScalar(128);
    SkTextUtils::Align fTextAlign;
    sk_sp<SkTypeface> fTypeface;
    SkColor fColor;
    bool fDisableStretch;

    SkPaint preparePaintForMeasure();
    SkPaint preparePaintForDraw();
    SkFont prepareFontForMeasure();
    SkFont prepareFontForDraw(const MeasureSpec &measureSpec);
    MeasureSpec measureAdjusted();
    MeasureSpec measureSizeFixed(SkScalar textSize);
};

#endif
