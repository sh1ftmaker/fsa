#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/DrawShadow.h"
#include "game/act/act_flame.h"

// FIXME(shotgun): not written (effort): init (0x8015EDF4, size 0x3A8)
// FIXME(shotgun): not written (size 0x3C8): fn_8015EA2C
// FIXME(shotgun): not written (size 0xAB0): fn_8015DF7C

Flame_804A0330::~Flame_804A0330() {
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// FIXME(shotgun): 96.4 %, missing/extra
void Flame_804A0330::update() {
    fn_8015EA2C();
    if (mState != 5) {
        fn_8015DF7C();
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
}

static inline void drawFlameShadow(s32 layer, const JGeometry::TVec3f* pos, f32 scale) {
    gGameRoot2_8053AB10->field_0x2c->drawShadow_80158B5C(layer, pos, JUtility::TColor(0, 0, 0, 0x80), scale);
}

void Flame_804A0330::draw() {
    if (field_0x254 == 0 && field_0x94 != 4 && field_0x94 != 5) {
        drawFlameShadow(field_0x4, &mPos, 60.0f);
    }
    if (field_0x240 != 0 && (field_0x94 == 4 || field_0x94 == 5)) {
        f32 scale = 46.2f;
        if (field_0x94 == 5) {
            scale = 140.0f;
        }
        drawFlameShadow(field_0x4, &mPos, scale);
    }
}
