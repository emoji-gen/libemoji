#include <iostream>
#include <string>
#include <vector>

#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkSurface.h"
#include "SkTypeface.h"

#include "emoji.h"


void cutEffectiveRange(
        sk_sp<SkTypeface> typeface,
        const std::string& line,
        int lineHeight
) {
    SkPaint paint;
    SkRect bounds;

    paint.setColor(SK_ColorBLACK);
    paint.setAntiAlias(true);
    paint.setTypeface(typeface);
    paint.setTextAlign(SkPaint::kCenter_Align);

    SkScalar textSize = SK_ScalarNaN;
    SkScalar textY = SK_ScalarNaN;
    SkScalar maxTextSize = lineHeight * SkIntToScalar(10);

    for (SkScalar i = lineHeight; i < maxTextSize; i += 0.5) {
        paint.setTextSize(i);
        paint.measureText(line.c_str(), line.length(), &bounds);

        SkScalar measuredWidth = bounds.fRight - bounds.fLeft;
        SkScalar measuredHeight = bounds.fBottom - bounds.fTop;

        if (measuredHeight > lineHeight) break;

        textSize = i;
        textY = -bounds.fTop;
    }

    /* sk_sp<SkSurface> surface = SkSurface::MakeRasterN32Premul(SkScalarCeilToInt(128), SkScalarCeilToInt(64)); */
    /* SkCanvas* canvas = surface->getCanvas(); */

    /* paint.setTextSize(textSize); */
    /* canvas->drawString(line.c_str(), 128 / SkIntToScalar(2), textY, paint); */

    /* sk_sp<SkImage> image(surface->makeImageSnapshot()); */
    /* sk_sp<SkData> data(image->encodeToData(SkEncodedImageFormat::kPNG, 100)); */
    /* SkFILEWStream fh((std::string("./_") + line + "_.png").c_str()); */
    /* (void)fh.write(data->data(), data->size()); */
}

void emoji_generate() {
    std::vector<std::string> lines;
    lines.push_back("良さ");
    lines.push_back("そう");

    // -----

    sk_sp<SkTypeface> typeface = SkTypeface::MakeFromName("Noto Sans Mono CJK JP Bold", SkFontStyle());

    for (auto& line : lines) {
        /* sk_sp<SkSurface> surface = SkSurface::MakeRasterN32Premul(line.length() * 255 / lines.size(), 128); */
        /* SkCanvas* canvas = surface->getCanvas(); */
        /* canvas->clear(SK_ColorTRANSPARENT); */

        cutEffectiveRange(typeface, line, 128 / lines.size());

        std::cout << line << std::endl;
    }

    /* SkPaint paint; */




}

