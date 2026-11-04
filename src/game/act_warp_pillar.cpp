#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/act/act_warp_pillar.h"

void WarpPillar_804AC0A8::draw() {}

void WarpPillar_804AC0A8::init() {
    field_0x94 = field_0x90 & 0xF;
    mLife = 10000;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x238 = (field_0x90 >> 4) & 0xF;
    field_0x23c = (field_0x90 >> 8) & 0xF;
    field_0x258 = 0.0f;
    mEmitter264 = NULL;
    mEmitter268 = NULL;
    field_0x25c[0] = false;
    mEmitter26c[0] = NULL;
    field_0x248[0] = 0;
    field_0x260[0] = false;
    field_0x25c[1] = false;
    mEmitter26c[1] = NULL;
    field_0x248[1] = 0;
    field_0x260[1] = false;
    field_0x25c[2] = false;
    mEmitter26c[2] = NULL;
    field_0x248[2] = 0;
    field_0x260[2] = false;
    field_0x25c[3] = false;
    mEmitter26c[3] = NULL;
    field_0x248[3] = 0;
    field_0x260[3] = false;

    if (mEmitter264 == NULL) {
        JGeometry::TVec3f pos(mPos.x, 16.0f + mPos.y, mPos.z);
        mEmitter264 = fn_8013CC50(field_0x4, &pos, 0x4E0, 1, 2, NULL, NULL, true, 1.0f);
    }
    if (mEmitter268 == NULL) {
        JGeometry::TVec3f pos(mPos.x, 16.0f + mPos.y, mPos.z);
        mEmitter268 = fn_8013CC50(field_0x4, &pos, 0x4E1, 1, 2, NULL, NULL, true, 1.0f);
    }

    mBox.i.x = -47.0f;
    mBox.i.y = -47.0f;
    mBox.f.x = 47.0f;
    mBox.f.y = 15.0f;
}

WarpPillar_804AC0A8::~WarpPillar_804AC0A8() {
    fn_801EE434(mEmitter264);
    fn_801EE434(mEmitter268);
    for (s32 i = 0; i < 4; i++) {
        fn_801EE434(mEmitter26c[i]);
    }
}
