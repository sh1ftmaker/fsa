#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameState.h"
#include "game/GameGlobals.h"
#include "game/act/act_ganon.h"
#include "game/act/act_life_meter.h"
#include "game/act/tu_801F9484.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/StageFlag.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/tu_80379F84.h"

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8019C4F4(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// FIXME: 89.43. Scheduling and registers: the original keeps the layer in r31,
// the player index in r29 and the position pointer in r30; ours has the layer
// in r29, the index in r30 and the pointer in r31. The .sdata2 literal pairs
// once the unit's literals are laid out.
void cls_804A2944::fn_8019C500(s32 keepY) {
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i)) {
            s32 layer = field_0x4;
            if (layer == getPlayerLayer_8023DE58(i)) {
                JGeometry::TVec3f pos(field_0x2a4[i].x, field_0x2a4[i].y, field_0x2a4[i].z);
                if (!keepY) {
                    pos.y = 32.0f + mPos.y;
                }
                fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            }
        }
    }
}

// .sbss flags of the ZLD2 actor (cls_804A2944).
u8 lbl_8053AB98;
u8 lbl_8053ABA8;

cls_804A2944::~cls_804A2944() {
    for (s32 i = 0; i < 13; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
    // field_0x90 is u32, but the original tests it with a signed cmpwi.
    s32 param = field_0x90;
    if (param == 0) {
        getGameView_802D6A9C()->fn_802D7440(true);
        lbl_8053AB98 = 0;
        lbl_8053ABA8 = 0;
    }
}

void fn_8019CD9C(s32 v) {
    gGameRoot_8053AAF8->fn_8013670C(v);
}

void fn_8019CDC4(s32 v) {
    GameRoot_8049EF20* root = gGameRoot_8053AAF8;
    root->field_0x85 = 1;
    root->field_0x74 = 0;
    root->field_0x78 = 0;
    root->field_0x54 = v;
}

void fn_8019CDE4(s32 stage, s32 sub) {
    if (stage < 0 || stage > 7) {
        return;
    }
    if (sub < 0 || sub > 3) {
        return;
    }
    lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x28 |= 1 << (stage * 4 + sub);
}

bool fn_8019CE40(s32 stage, s32 sub) {
    return isStageFlagSet(stage, sub);
}

void ActorBase_804A5770::fn_8019CEAC(s32 v) {
    field_0xb0 = v;
}

s32 ActorBase_804A5770::fn_8019CEB4() const {
    return field_0xb0;
}

void Ganon_804A25F0::fn_8019CEBC(s32 value) {
    field_0x28c = value;
}

void Ganon_804A25F0::fn_8019CEC4(s32 value) {
    field_0x2f8 = value;
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8019CECC(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

void fn_8019CEE8(u8 v) {
    lbl_80529DEC.field_0x24d = v;
}

void LifeMeter_804B174C::fn_8019CEF8(s32 serial, f32 yOffset, s32 index) {
    field_0x250 = serial;
    field_0x254 = yOffset;
    field_0x24c = index;
}
