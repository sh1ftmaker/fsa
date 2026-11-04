#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameState.h"
#include "game/GameGlobals.h"

void ActorBase_804A5770::fn_8019CEAC(s32 v) {
    field_0xb0 = v;
}

s32 ActorBase_804A5770::fn_8019CEB4() const {
    return field_0xb0;
}

void fn_8019CDC4(s32 v) {
    GameRoot_8049EF20* root = lbl_8053AAF8;
    root->field_0x85 = 1;
    root->field_0x74 = 0;
    root->field_0x78 = 0;
    root->field_0x54 = v;
}

void fn_8019CEE8(u8 v) {
    lbl_80529DEC.field_0x24d = v;
}

void fn_8019CD9C(s32 v) {
    lbl_8053AAF8->fn_8013670C(v);
}
