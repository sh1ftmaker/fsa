#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Unk_8045A080.h"
#include "game/act/tu_8044B80C.h"
#include "game/act/tu_802E9428.h"

// FIXME(shotgun): not written (size 0x4A4): fn_802EAD78
// FIXME(shotgun): not written (size 0x115C): update (0x802E9B6C)
// FIXME(shotgun): not written (size 0x2E8): fn_802E9884
// FIXME(shotgun): not written (size 0x390): fn_802E94F4

// FIXME(shotgun): 99.7 %, operands + reloc-name
void UnknownENDM_804AA0FC::init() {
    field_0x284 = getGameView_802D6A9C()->field_0x4;
    field_0x280 = getGameView_802D6A9C()->field_0x8;
    field_0x23c = 0;
    field_0x230 = 0;
    field_0x28d = 0;
    field_0x298 = 0;
    field_0x240 = 0;
    field_0x294 = 0;
    field_0x295 = 0;
    field_0x299 = 1;
    field_0x270 = 300.0f;
    field_0x274 = 300.0f;
    field_0x278 = 0.0f;
    gAudioMgr_8053AF80->fn_80458FF0(0x100028, true, 0);
    field_0x234 = 0;
    field_0x231 = 0;
    field_0x29a = lbl_8053AF30->fn_8044B80C(field_0x294);
}

// FIXME(shotgun): 93.7 %, missing/extra
bool UnknownENDM_804AA0FC::fn_802E948C(f32 target, s32 frames) {
    if (field_0x270 == target) {
        return false;
    }
    f32 step = fabs(field_0x270 - target) / (f32)frames;
    if (field_0x270 < target) {
        field_0x278 = step;
    } else {
        field_0x278 = -step;
    }
    field_0x274 = target;
    return true;
}

void UnknownENDM_804AA0FC::draw() {}

UnknownENDM_804AA0FC::~UnknownENDM_804AA0FC() {}
