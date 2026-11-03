#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_cloud.h"

// Name of each state's sprite, four per state (the state is field_0x36c).
static const char* lbl_8049CBB0[3][4] = {
    {"lift_s_a.bti", "lift_s_b.bti", "lift_s_c.bti", "lift_s_d.bti"},
    {"lift_l_a.bti", "lift_l_b.bti", "lift_l_c.bti", "lift_l_d.bti"},
    {"said_a.bti", "said_b.bti", "said_c.bti", "said_d.bti"},
};

// One animation: (cel, duration) pairs, ended by a cel of -1.
static const s16 lbl_80463FA0[] = {0, 10, 1, 10, 2, 10, 3, 10, -1, 0};
static const s16* lbl_80539EC0[] = {lbl_80463FA0, NULL};

Cloud_8049CBE0::~Cloud_8049CBE0() {}

static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

// Whether `player` is in the cloud: on the cloud's layer, either its box
// (moved to the cloud's position) overlaps the player's, or one of eight
// points around the player falls in mBox.
// FIXME: 94.45%. In the layer branch the original keeps box.i.x and mPos.x in
// other FPRs and reloads box.i.y and box.f.y from the stack, where ours
// reuses the registers; the eight-point loop matches.
bool Cloud_8049CBE0::fn_801053E8(s32 player) {
    if (player < 0) {
        return false;
    }
    if (field_0x4 != fn_8023DE58(player)) {
        return false;
    }
    JGeometry::TBox2f box(mBox);
    if (viewLayerFlag(field_0x4)) {
        box.i.x += mPos.x;
        box.f.x += mPos.x;
        box.i.y += mPos.y;
        box.f.y += mPos.y;
        box.i.y -= 2.0f;
        if (fn_8023B0F4(player, &box, field_0x4, false, 0.0f)) {
            fn_8022DF48(player);
            return true;
        }
        return false;
    }
    for (s32 i = 0; i < 8; i++) {
        JGeometry::TVec3f v;
        fn_8022E088(player, i, &v);
        v -= mPos;
        // The angle is in s16 units (32768 = 180 degrees); JMASinShort and
        // JMACosShort index the sin/cos table with it.
        s32 angle = (s32)(JMath::atanTable_.atan2_(v.y, v.x) *
                          (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        f32 len = JGeometry::TVec2<f32>(v.x, v.y).length();
        v.x = len * JMACosShort(angle);
        v.y = len * JMASinShort(angle);
        v.z = 0.0f;
        if (contains(box, v)) {
            fn_8022DF48(player);
            return true;
        }
    }
    return false;
}

void Cloud_8049CBE0::draw() {
    ActorBase_804A5770::draw();
    mParams.field_0x1d = -1;
    s32 key = mSprite.mKey;
    JGeometry::TVec3f pos = mPos;
    if (field_0x36c == 2) {
        pos.y -= 4.0f;
    }
    void* res = field_0x2f8[key];
    if (field_0x4 < 8) {
        lbl_8053AB10->field_0x20->fn_802F745C(&pos, res, &mParams);
    }
}

void Cloud_8049CBE0::init() {
    ActorBase_804A5770::init();
    field_0x354 = field_0x90 & 0xFF;
    field_0x358 = field_0x90 >> 12 & 0x1F;
    field_0x36c = field_0x90 >> 17 & 0x1F;
    field_0x364 = field_0x90 >> 22 & 0x1F;
    field_0x368 = field_0x90 >> 22 & 0x1F;
    field_0x35c = field_0x90 >> 27;
    for (s32 i = 0; i < 4; i++) {
        field_0x2f8[i] = lbl_8053AAF8->field_0xa4->getResource('????', lbl_8049CBB0[field_0x36c][i]);
    }
    // FIXME: 88.48%. The original's dispatch is `cmpwi 1; beq state1; bge L;
    // b default; L: cmpwi 3; bge default; b state2`, with the blocks laid out
    // state1, state2, default. This switch and an if/else chain both compare
    // against 1 again; the 0.0f/-38.0f literals also sit at other offsets.
    switch (field_0x36c) {
    case 1:
        mBox.i.x = -38.0f;
        mBox.i.y = -38.0f;
        mBox.f.x = 38.0f;
        mBox.f.y = 38.0f;
        break;
    case 2:
        mBox.i.x = -32.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 32.0f;
        mBox.f.y = 12.0f;
        break;
    default:
        mBox.i.x = -22.0f;
        mBox.i.y = -22.0f;
        mBox.f.x = 22.0f;
        mBox.f.y = 22.0f;
        break;
    }
    mBox2 = mBox;
    field_0x80 = mBox;
    mSprite.fn_801D2608(lbl_80539EC0, 1);
    mSprite.fn_801D22DC(0, 2, false);
    mSprite.fn_801D1F14(0.0f);
}

// FIXME: 99.95%. Everything matches except the 0.0f literal: this unit's
// .sdata2 literal order puts it at another offset than lbl_8053C228.
Cloud_8049CBE0::Cloud_8049CBE0()
    : field_0x344(0.0f), field_0x348(0.0f), field_0x34c(0.0f), field_0x350(0.0f), field_0x354(0),
      field_0x358(0), field_0x35c(-1), field_0x360(0), field_0x364(0), field_0x368(0),
      field_0x36c(0), field_0x370(0) {
    for (s32 i = 0; i < 4; i++) {
        field_0x2f8[i] = NULL;
    }
}
