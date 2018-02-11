#include <stdio.h>

#include "emoji.h"

int main() {
    EgGenerateParams params;
    params.fText = "確実に\n痩せる";
    params.fWidth = 1024;
    params.fHeight = 1024;
    params.fColor = 0xFFEC71A1;
    params.fBackgroundColor = 0x00FFFFFF;
    params.fTextAlign = kLeft_Align;
    params.fTypefaceName = "Noto Sans Mono CJK JP Bold";
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
