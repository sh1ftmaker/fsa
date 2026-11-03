#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/cls_8031709C.h"
#include "game/act/act_tingle_first.h"

cls_804AADD8::cls_804AADD8() {
    field_0x24c = new cls_8031709C;
}

cls_804AADD8::~cls_804AADD8() {
    if (field_0x24c != NULL) {
        delete field_0x24c;
        field_0x24c = NULL;
    }
}

// FIXME: 99.32%. Only the float constants differ: the target loads -8, -24 and 24 (lbl_805418E4/E8/EC)
// and -20 and 20 (lbl_805418F0/F4) from .sdata2 by name; ours are @NNN literals, since splits.txt
// has no .sdata2 for this unit.
void cls_804AADD8::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -24.0f;
    mBox.f.x = 24.0f;
    mBox.f.y = 24.0f;
    field_0x230 = 0;
    field_0x238 = 50;
    field_0x234 = -1;
    field_0x23c = 29;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    s32 layer = field_0x4;
    JGeometry::TVec3f pos(mPos);
    field_0x24c->fn_80316E50(layer, pos);
    field_0x260 = -20.0f;
    field_0x264 = -20.0f;
    field_0x268 = 20.0f;
    field_0x26c = 20.0f;
    field_0x270 = 0;
}

// FIXME: 99.62%. Only the literal labels differ: 1.0f is lbl_805418C4 in the target (.sdata2) and
// the string is lbl_80471620 (.rodata); ours are @NNN, since .sdata2 is not split for this unit.
void cls_804AADD8::draw() {
    if (field_0x230 == 0xC || field_0x230 == 0xB) {
        return;
    }
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x23c + 0xC0, false, 2, -1, 0, 0, 0);
    fn_801F06F0("enemy_both2_sob.bin", field_0x23c + 0xC0, NULL, 7, 1.0f, 1.0f);
}
