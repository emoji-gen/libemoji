#include <stdio.h>
#include <string.h>

#include "emoji.h"

int main() {
    EgGenerateParams params;
    memset(&params, 0, sizeof(params));

    params.fText = "絵文\n.";
    params.fWidth = 256;
    params.fHeight = 256;
    params.fColor = 0xFF000000;
    params.fBackgroundColor = 0x00FFFFFF;
    params.fTextAlign = kCenter_Align;
    params.fTextSizeFixed = true;
    params.fTypefaceFile = "./example/NotoSansMonoCJKjp-Bold.otf";
    params.fFormat = kPNG_Format;
    params.fQuality = 100;

    EgError err;
    EgGenerateResult result;
    if ((err = emoji_generate(&params, &result)) != EG_OK) {
        printf("ERROR: %d\n", err);
        return 1;
    }

    FILE *fp = fopen("./example/emoji.png", "w");
    fwrite(result.fData, result.fSize, 1, fp);
    fclose(fp);

    emoji_free(&result);

    return 0;
}
