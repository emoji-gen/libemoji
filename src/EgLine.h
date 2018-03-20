#ifndef SRC_EG_LINE_BOX_H_
#define SRC_EG_LINE_BOX_H_

#include <string>

#include "SkCanvas.h"
#include "SkColor.h"
#include "SkRect.h"
#include "SkScalar.h"
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

    void setText(const std::string& text) { fText = text; }
    void setWidth(SkScalar width) { fWidth = width; }
    void setLineHeight(SkScalar lineHeight) { fLineHeight = lineHeight; }
    void setTypeface(sk_sp<SkTypeface> typeface) { fTypeface = typeface; }
    void setTextAlign(SkPaint::Align textAlign) { fTextAlign = textAlign; }
    void setColor(SkColor color) { fColor = color; }
    void setDisableStretch(bool disableStretch) { fDisableStretch = disableStretch; }

    MeasureSpec measure(SkScalar textSize = SK_ScalarNaN);
    void draw(SkCanvas* canvas, std::size_t y, const MeasureSpec& spec);

private:
    std::string fText = "";
    SkScalar fWidth = SkIntToScalar(128);
    SkScalar fLineHeight = SkIntToScalar(128);
    SkPaint::Align fTextAlign;
    sk_sp<SkTypeface> fTypeface;
    SkColor fColor;
    bool fDisableStretch;
};

#endif
