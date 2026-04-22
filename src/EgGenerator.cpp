#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#include "core/SkCanvas.h"
#include "core/SkImage.h"
#include "core/SkImageInfo.h"
#include "core/SkPaint.h"
#include "core/SkSurface.h"
#include "core/SkTypeface.h"
#include "encode/SkPngEncoder.h"
#include "encode/SkWebpEncoder.h"

// SkFontMgr
#include "core/SkFontMgr.h"
#ifdef EG_OS_MAC
#include "ports/SkFontMgr_mac_ct.h"
#endif
#ifdef EG_OS_LINUX
#include "ports/SkFontMgr_empty.h"
#endif

#include "EgGenerator.h"
#include "EgLine.h"

EgGenerator::EgGenerator() {
#ifdef EG_OS_MAC
    fFontMgr = SkFontMgr_New_CoreText(nullptr);
#endif
#ifdef EG_OS_LINUX
    // TODO: Use SkFontMgr_New_FontConfig(...) instead
    fFontMgr = SkFontMgr_New_Custom_Empty();
#endif
}

void EgGenerator::setTypefaceFromName(const char *familyName) {
    // Note: Currently only works on macOS
    fTypeface = fFontMgr->matchFamilyStyle(familyName, SkFontStyle());
}

void EgGenerator::setTypefaceFromFile(const char *path) {
    fTypeface = fFontMgr->makeFromFile(path);
}

void EgGenerator::setText(const char *text) {
    std::istringstream stream(text);
    std::string line;
    std::vector<std::string> texts;

    while (std::getline(stream, line)) {
        texts.push_back(line);
    }

    fTexts = std::move(texts);
}

sk_sp<SkData> EgGenerator::generate() {
    SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(fWidth, fHeight);
    sk_sp<SkSurface> surface = SkSurfaces::Raster(imageInfo);
    SkCanvas *canvas = surface->getCanvas();
    canvas->clear(fBackgroundColor);

    std::vector<EgLine::MeasureSpec> specs;
    SkScalar lineHeight = fHeight / SkIntToScalar(fTexts.size());

    // 行ボックスを作成
    std::vector<EgLine> lines;
    for (auto &text : fTexts) {
        EgLine line;
        line.setWidth(fWidth);
        line.setLineHeight(lineHeight);
        line.setText(text);
        line.setTypeface(fTypeface);
        line.setTextAlign(fTextAlign);
        line.setColor(fColor);
        line.setDisableStretch(fDisableStretch);
        lines.push_back(std::move(line));
    }

    // 高さ・幅を計算
    for (auto &line : lines) {
        specs.push_back(line.measure());
    }

    // サイズ固定モード: 最小テキストサイズで再計算
    if (fTextSizeFixed) {
        auto foundSpecItr = std::min_element(
            specs.begin(), specs.end(),
            [](EgLine::MeasureSpec lhs, EgLine::MeasureSpec rhs) {
                return lhs.fTextSize < rhs.fTextSize;
            });

        if (foundSpecItr != specs.end()) {
            SkScalar minTextSize = foundSpecItr->fTextSize;
            for (std::size_t i = 0; i < lines.size(); ++i) {
                specs[i] = lines[i].measure(minTextSize);
            }
        }
    }

    // テキストを描画
    for (std::uint32_t i = 0; i < lines.size(); ++i) {
        lines[i].draw(canvas, lineHeight * i, specs[i]);
    }

    // エンコード
    sk_sp<SkImage> snapshot(surface->makeImageSnapshot());
    if (fFormat == kPNG_Format) {
        return SkPngEncoder::Encode(nullptr, snapshot.get(), SkPngEncoder::Options());
    } else if (fFormat == kWEBP_Format) {
        SkWebpEncoder::Options options { .fQuality = static_cast<float>(fQuality) };
        return SkWebpEncoder::Encode(nullptr, snapshot.get(), options);
    }

    // 生成失敗
    return sk_sp<SkData>(); // nullptr
}
