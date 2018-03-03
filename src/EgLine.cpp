#include "SkPaint.h"
#include "SkRect.h"

#include "EgLine.h"

EgLine::EgLine(
    const std::string& text,
    SkScalar width,
    SkScalar lineHeight
) : fText(text), fWidth(width), fLineHeight(lineHeight) {
}

EgLine::MeasureSpec EgLine::measure(SkScalar textSize) {
    SkPaint paint;
    paint.setColor(SK_ColorBLACK);
    paint.setAntiAlias(true);
    if (fTypeface != nullptr) paint.setTypeface(fTypeface);
    paint.setTextAlign(SkPaint::kLeft_Align);

    SkRect bounds;
    MeasureSpec measureSpec;

    // テキストサイズ固定モード: 2回目の測定処理
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
        // 非伸縮モード: 初期フォントサイズを調整
        if (fDisableStretch) {
            for (SkScalar i = minTextSize; i > 0; i -= 0.5) {
                paint.setTextSize(i);
                paint.measureText(fText.c_str(), fText.length(), &bounds);
                if (bounds.width() < fWidth) {
                    minTextSize = i;
                    break;
                }
            }
        }

        for (SkScalar i = minTextSize; i < maxTextSize; i += 0.5) {
            paint.setTextSize(i);
            paint.measureText(fText.c_str(), fText.length(), &bounds);

            if (bounds.height() > fLineHeight) break;
            if (fDisableStretch && bounds.width() > fWidth) break;

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

    // for X-axis
    SkScalar x;
    switch (fTextAlign) {
    case SkPaint::kLeft_Align:
        x = -spec.fBounds.fLeft;
        break;
    case SkPaint::kCenter_Align:
        if (spec.fTextScaleX < SkIntToScalar(1)) {
            x = -spec.fBounds.fLeft * spec.fTextScaleX;
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

    // for Y-axis
    SkScalar offsetY = (fLineHeight - spec.fBounds.height()) / SkIntToScalar(2);

    paint.setTextScaleX(spec.fTextScaleX);
    canvas->drawString(fText.c_str(), x, y - spec.fBounds.fTop + offsetY, paint);
}
