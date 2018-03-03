#include "SkPaint.h"
#include "SkRect.h"

#include "EgLine.h"

EgLine::EgLine(
    const std::string& text,
    SkScalar width,
    SkScalar lineHeight,
    sk_sp<SkTypeface> typeface
) : fText(text), fWidth(width), fLineHeight(lineHeight), fTypeface(typeface) {
}

EgLine::MeasureSpec EgLine::measure(SkScalar textSize) {
    SkPaint paint;
    paint.setColor(SK_ColorBLACK);
    paint.setAntiAlias(true);
    paint.setTypeface(fTypeface);
    paint.setTextAlign(SkPaint::kLeft_Align);

    SkRect bounds;
    MeasureSpec measureSpec;

    // フォントサイズ固定モード
    if (!SkScalarIsNaN(textSize)) {
        paint.setTextSize(textSize);
        paint.measureText(fText.c_str(), fText.length(), &bounds);

        measureSpec.fTextSize = textSize;
        measureSpec.fBounds = bounds;
        measureSpec.fTextScaleX = bounds.width() > fWidth ? fWidth / bounds.width() : 1;
        return measureSpec;
    }

    SkScalar minTextSize = fLineHeight * SkDoubleToScalar(0.9);
    SkScalar maxTextSize = fLineHeight * SkIntToScalar(10);

    SkRect prevBounds = SkRect::MakeEmpty();
    SkScalar prevTextSize = SK_ScalarNaN;

    if (!fText.empty()) {
        for (SkScalar i = minTextSize; i < maxTextSize; i += 0.5) {

            paint.setTextSize(i);
            paint.measureText(fText.c_str(), fText.length(), &bounds);

            if (bounds.height() > fLineHeight) break;

            prevTextSize = i;
            prevBounds = bounds;
        }
    }

    measureSpec.fTextSize = prevTextSize;
    measureSpec.fBounds = prevBounds;
    measureSpec.fTextScaleX = prevBounds.width() > fWidth ? fWidth / prevBounds.width() : 1;

    return measureSpec;
}

void EgLine::draw(SkCanvas* canvas, int y, MeasureSpec spec) {
    SkPaint paint;
    paint.setColor(fColor);
    paint.setAntiAlias(true);
    paint.setTypeface(fTypeface);
    paint.setTextAlign(SkPaint::kLeft_Align);
    paint.setTextSize(spec.fTextSize);

    SkScalar x;
    switch (fTextAlign) {
    case SkPaint::kLeft_Align:
        x = -spec.fBounds.fLeft;
        break;
    case SkPaint::kCenter_Align:
        if (spec.fTextScaleX < SkIntToScalar(1)) {
            x = -spec.fBounds.fLeft;
        } else {
            x = (fWidth - spec.fBounds.width()) / SkIntToScalar(2) - spec.fBounds.fLeft;
        }
        break;
    case SkPaint::kRight_Align:
        if (spec.fTextScaleX < SkIntToScalar(1)) {
            x = -spec.fBounds.fLeft;
        } else {
            x = fWidth - spec.fBounds.width() - spec.fBounds.fLeft;
        }
        break;
    }

    paint.setTextScaleX(spec.fTextScaleX);
    canvas->drawString(fText.c_str(), x, y - spec.fBounds.fTop, paint);
}
