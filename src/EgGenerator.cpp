#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#include "SkCanvas.h"
#include "SkImage.h"
#include "SkPaint.h"
#include "SkSurface.h"
#include "SkTypeface.h"

#include "EgGenerator.h"
#include "EgLine.h"

EgGenerator::EgGenerator() {
}

void EgGenerator::setTypefaceFromName(const char* familyName) {
    fTypeface = SkTypeface::MakeFromName(familyName, SkFontStyle());
}

void EgGenerator::setText(const char* text) {
    std::istringstream stream(text);
    std::string line;
    std::vector<std::string> texts;

    while (std::getline(stream, line)) {
        texts.push_back(line);
    }

    fTexts = std::move(texts);
}

void EgGenerator::generate() {
    sk_sp<SkSurface> surface = SkSurface::MakeRasterN32Premul(fWidth, fHeight);
    SkCanvas* canvas = surface->getCanvas();
    canvas->clear(fBackgroundColor);

    std::vector<EgLine::MeasureSpec> specs;
    SkScalar lineHeight = fHeight / SkIntToScalar(fTexts.size());

    // 行ボックスを作成
    std::vector<EgLine> lines;
    for (auto& text : fTexts) {
        EgLine line(text, fWidth, lineHeight, fTypeface);
        line.setTextAlign(fTextAlign);
        line.setColor(fColor);
        lines.push_back(std::move(line));
    }

    // 高さ・幅を計算
    for (auto& line : lines) {
        specs.push_back(line.measure());
    }

    // テキストを描画
    for (std::size_t i = 0; i < lines.size(); ++i) {
        lines[i].draw(canvas,  lineHeight * i, specs[i]);
    }

    sk_sp<SkImage> image(surface->makeImageSnapshot());
    sk_sp<SkData> data(image->encodeToData(SkEncodedImageFormat::kPNG, 100));
    SkFILEWStream fh((std::string("./emoji.png").c_str()));
    (void)fh.write(data->data(), data->size());
}

