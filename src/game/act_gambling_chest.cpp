// Actor GMTB ("Gambling shop chest" in docs/names/actors.tsv): the spawner
// allocates 0x258 bytes for it and calls fn_80200904, which stores the
// vtable 0x804AAAE8. Its tile is rewritten through lbl_8053AC98.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/BgRewrite.h"
#include "game/act/act_gambling_chest.h"

GamblingShopChest_804AAAE8::~GamblingShopChest_804AAAE8() {}

void GamblingShopChest_804AAAE8::fn_80310E48() {
    lbl_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x10);
    field_0x234 = -1;
    mState = 0;
}

void GamblingShopChest_804AAAE8::fn_80310F50(s32 a, s32 b, s32 serial) {
    lbl_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0x30);
    field_0x234 = a;
    field_0x238 = b;
    field_0x23c = serial;
    mState = 1;
}
