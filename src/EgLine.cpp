#include "SkPaint.h"
#include "SkRect.h"

#include "EgLine.h"

EgLine::EgLine() {}

EgLine::MeasureSpec EgLine::measure(SkScalar textSize) {
    return SkScalarIsNaN(textSize) ? measureAdjusted() : measureSizeFixed(textSize);
}

/**
 * テキストサイズ可変モード
 */
EgLine::MeasureSpec EgLine::measureAdjusted() {
    SkPaint paint = preparePaintForMeasure();
    SkRect bounds;
    MeasureSpec spec;

    SkScalar minTextSize = fLineHeight * SkDoubleToScalar(0.9);
    SkScalar maxTextSize = fLineHeight * SkIntToScalar(10);

    SkRect prevBounds = SkRect::MakeEmpty();
    SkScalar prevTextSize = SK_ScalarNaN;

    if (!fText.empty()) {
        // 非伸縮モード: 初期フォントサイズを調整
        if (fDisableStretch) {
            for (SkScalar i = minTextSize; i > SkIntToScalar(0);
                 i -= SkDoubleToScalar(0.5)) {
                paint.setTextSize(i);
                paint.measureText(fText.c_str(), fText.length(), &bounds);
                if (bounds.width() < fWidth) {
                    minTextSize = i;
                    break;
                }
            }
        }

        for (SkScalar i = minTextSize; i < maxTextSize;
             i += SkDoubleToScalar(0.5)) {
            paint.setTextSize(i);
            paint.measureText(fText.c_str(), fText.length(), &bounds);

            if (bounds.height() > fLineHeight) break;
            if (fDisableStretch && bounds.width() > fWidth) break;

            prevTextSize = i;
            prevBounds = bounds;
        }
    }

    spec.fTextSize = prevTextSize;
    spec.fBounds = prevBounds;
    spec.fTextScaleX = SkIntToScalar(1);

    // 横方向圧縮が必要な場合: 圧縮率の調整
    if (prevBounds.width() > fWidth) {
        paint.setTextSize(prevTextSize);

        for (SkScalar i = fWidth / prevBounds.width(); i > SkDoubleToScalar(0);
             i -= SkDoubleToScalar(0.0001)) {
            paint.setTextScaleX(i);
            paint.measureText(fText.c_str(), fText.length(), &bounds);
            if (bounds.width() <= fWidth) {
                spec.fBounds = bounds;
                spec.fTextScaleX = i;
                break;
            }
        }
    }

    return spec;
}

/**
 * テキストサイズ固定モード
 *
 * 一度可変モードで測定後、最も小さいフォントサイズで固定し再度計測する
 */
EgLine::MeasureSpec EgLine::measureSizeFixed(SkScalar textSize) {
    SkPaint paint = preparePaintForMeasure();
    SkRect bounds;
    MeasureSpec spec;

    paint.setTextSize(textSize);
    paint.measureText(fText.c_str(), fText.length(), &bounds);

    spec.fTextSize = textSize;
    spec.fBounds = bounds;
    spec.fTextScaleX = SkIntToScalar(1);

    // 横方向圧縮が必要な場合: 圧縮率の調整
    if (bounds.width() > fWidth) {
        for (SkScalar i = fWidth / bounds.width(); i > SkDoubleToScalar(0);
             i -= SkDoubleToScalar(0.0001)) {
            paint.setTextScaleX(i);
            paint.measureText(fText.c_str(), fText.length(), &bounds);
            if (bounds.width() <= fWidth) {
                spec.fBounds = bounds;
                spec.fTextScaleX = i;
                break;
            }
        }
    }

    return spec;
}

void EgLine::draw(SkCanvas *canvas, SkScalar y, const MeasureSpec &spec) {
    SkPaint paint = preparePaintForDraw(spec.fTextSize);

    // for X-axis
    SkScalar x;
    switch (fTextAlign) {
    case SkPaint::kLeft_Align:
        x = -spec.fBounds.fLeft;
        break;
    case SkPaint::kCenter_Align:
        if (spec.fTextScaleX < SkIntToScalar(1)) {
            x = -spec.fBounds.fLeft;
        } else {
            x = (fWidth - spec.fBounds.width()) / SkIntToScalar(2) -
                spec.fBounds.fLeft;
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
    canvas->drawString(fText.c_str(), x, y - spec.fBounds.fTop + offsetY,
                       paint);
}

SkPaint EgLine::preparePaintForMeasure() {
    SkPaint paint;
    paint.setColor(SK_ColorBLACK);
    paint.setAntiAlias(true);
    paint.setTextAlign(SkPaint::kLeft_Align);

    if (fTypeface != nullptr) {
        paint.setTypeface(fTypeface);
    }

    return paint;
}

SkPaint EgLine::preparePaintForDraw(SkScalar textSize) {
    SkPaint paint;
    paint.setColor(fColor);
    paint.setAntiAlias(true);
    paint.setTextAlign(SkPaint::kLeft_Align);
    paint.setTextSize(textSize);

    if (fTypeface != nullptr) {
        paint.setTypeface(fTypeface);
    }

    return paint;
}
