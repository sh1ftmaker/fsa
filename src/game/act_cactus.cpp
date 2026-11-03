#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/act/act_cactus.h"

// Sprite frames of the hit animation (0, 1, 2, 3, 4, 3, 4).
static const s32 lbl_804718E8[7] = {0, 1, 2, 3, 4, 3, 4};

Cactus_804AB580::~Cactus_804AB580() {}

void Cactus_804AB580::draw() {
    fn_801F06F0("enemy_gc_sob.bin", lbl_804718E8[field_0x23c] + 0x12F, NULL, 0, 1.0f, 1.0f);
}

void Cactus_804AB580::update() {
    if ((field_0x90 & 1) == 1 && vfunc_0x34() == 1) {
        fn_8013CC50(field_0x4, &mPos, 0x503, 1, 0, NULL, NULL, true, 1.0f);
        mAlive = false;
        return;
    }
    JGeometry::TBox2f hitBox(mBox2);
    JGeometry::TBox2f box(-4.0f, -4.0f, 4.0f, 4.0f);
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    hitBox.addPos(pos);
    box.addPos(pos);
    switch (field_0x230) {
    case 0:
    case 1: {
        fn_80239914(&box, field_0x4, mSerial);
        s32 hit = fn_801F2B7C(field_0x114, -1, 1);
        s32 player = fn_801F285C(NULL, NULL, true, -1);
        if (hit > 0 || player >= 0 || field_0x9c >= 0 || fn_8023BB50(&hitBox, field_0x4, &pos, true) >= 0) {
            if (field_0x230 == 0) {
                fn_801F0E34(0x50080, 0);
            }
            if (player >= 0) {
                fn_802379E4(player, &mPos);
            }
            field_0x230 = 1;
            field_0x23c = 0;
        }
        break;
    }
    }
    if (field_0x230 == 1) {
        fn_80239638(mSerial);
        field_0x23c++;
        if (field_0x23c >= 7) {
            field_0x23c = 0;
            field_0x230 = 0;
        }
    }
}

void Cactus_804AB580::init() {
    mLife = 10000;
    field_0x234 = 0;
    field_0x23c = 0;
    field_0x238 = 0;
    field_0x230 = 0;
    field_0x114 = 2;
    field_0xb8 = 2;
    field_0xb0 = 0x1042304;
    field_0x80.set(-5.0f, -5.0f, 5.0f, 5.0f);
    mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
    mBox2.set(-9.0f, -9.0f, 9.0f, 9.0f);
}
