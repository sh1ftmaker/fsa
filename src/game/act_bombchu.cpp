#include "game/GameAudio.h"
#include "game/BgRewrite.h"
#include "game/act/act_bombchu.h"

Bombchu_804AB464::~Bombchu_804AB464() {}

void Bombchu_804AB464::vfunc_0x48() {
    vfunc_0x54();
    JGeometry::TVec3f result;
    gBgRewrite_8053AC98->fn_8029EDB8(&mPos, 0, field_0x4, &result);
    field_0x230 = 3;
}
