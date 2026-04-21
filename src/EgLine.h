#ifndef SRC_EG_LINE_BOX_H_
#define SRC_EG_LINE_BOX_H_

#include <string>

#include "core/SkCanvas.h"
#include "core/SkColor.h"
#include "core/SkFont.h"
#include "core/SkPaint.h"
#include "core/SkRect.h"
#include "core/SkScalar.h"
#include "core/SkTypeface.h"
#include "utils/SkTextUtils.h"

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
