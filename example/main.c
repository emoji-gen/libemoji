#include "emoji.h"

#include <stdio.h>

int main() {
    EgGenerateParams params;
    params.fText = "確実に\n糞";
    params.fWidth = 1024;
    params.fHeight = 1024;
    params.fColor = 0xFFEC71A1;
    params.fBackgroundColor = 0xFFFFFFFF;
    params.fTextAlign = kCenter_Align;
    params.fTypefaceName = "Noto Sans Mono CJK JP Bold";

    emoji_generate(&params);
    return 0;
}
