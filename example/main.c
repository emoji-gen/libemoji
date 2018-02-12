#include <stdio.h>

#include "emoji.h"

int main() {
    EgGenerateParams params;
    params.fText = "lib\nemoji";
    params.fWidth = 256;
    params.fHeight = 256;
    params.fColor = 0xFF000000;
    params.fBackgroundColor = 0x00FFFFFF;
    params.fTextAlign = kLeft_Align;
    params.fTypefaceName = "Linux Libertine";
    params.fFormat = kPNG_Format;
    params.fQuality = 100;

    EgGenerateResult result;
    emoji_generate(&params, &result);

    FILE *fp = fopen("./emoji.png", "w");
    fwrite(result.fData, result.fSize, 1, fp);
    fclose(fp);

    emoji_free(&result);

    return 0;
}
