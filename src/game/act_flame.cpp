#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/DrawShadow.h"
#include "game/Effect.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_flame.h"

// FIXME(shotgun): not written (size 0xAB0): fn_8015DF7C

Flame_804A0330::~Flame_804A0330() {
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// FIXME(shotgun): 83.7 %, missing/extra + reloc-name
void Flame_804A0330::init() {
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    mEmitters[3] = NULL;
    field_0x254 = 0;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x118 = 4;
    field_0x258 = (field_0x90 >> 18) & 3;
    field_0x25c = (field_0x90 >> 20) & 1;
    if ((field_0x90 & 0xFF00) == 0) {
        field_0x94 = field_0x90 & 0xFF;
        field_0x98 = (field_0x90 >> 16) & 3;
        if (field_0x94 == 4 || field_0x94 == 5) {
            field_0x98 = 3;
        } else {
            field_0x114 = 4;
        }
        field_0x260 = 0.0f;
        if (field_0x94 >= 9) {
            field_0x94 = 0;
        }
        switch (field_0x94) {
        case 3:
            mState = 3;
            break;
        case 1:
            mState = 1;
            break;
        case 2:
            mState = 2;
            break;
        case 0:
        case 6:
            field_0x260 = 1.0f;
            field_0x240 = 1;
            field_0x234 = 8;
            mState = 0;
            break;
        case 4:
        case 5:
            if (field_0x94 == 5) {
                field_0x260 = 1.0f;
            } else {
                field_0x260 = 0.33f;
            }
            mEmitters[3] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x3E, 0, -1, NULL, NULL, true, field_0x260);
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x3B, 0, -1, NULL, NULL, true, field_0x260);
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x3C, 1, 1, NULL, NULL, true, field_0x260);
            mEmitters[2] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D, 1, 1, NULL, NULL, true, field_0x260);
            field_0x240 = 1;
            field_0x234 = 8;
            mState = 0;
            break;
        }
        switch (field_0x98) {
        case 0:
            field_0x3c.x = 3.0f;
            field_0x3c.y = 0.5f;
            field_0x3c.z = 0.0f;
            break;
        case 1:
            field_0x3c.x = -3.0f;
            field_0x3c.y = 0.5f;
            field_0x3c.z = 0.0f;
            break;
        case 2:
            field_0x3c.x = 0.0f;
            field_0x3c.y = 3.0f;
            field_0x3c.z = 0.0f;
            break;
        case 3:
            field_0x3c.x = 0.0f;
            field_0x3c.y = -3.0f;
            field_0x3c.z = 0.0f;
            break;
        }
    } else {
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        field_0x94 = (field_0x90 >> 8) & 0xFF;
        if (field_0x94 >= 9) {
            field_0x94 = 1;
        }
        field_0x254 = 1;
        switch (field_0x94) {
        case 1:
            mState = 6;
            break;
        case 2:
            mState = 7;
            break;
        case 3:
            mState = 8;
            break;
        }
    }
    field_0x250 = 0;
    fn_8015EA2C();
    if (field_0x240 == 0) {
        field_0x260 = 0.0f;
    }
    if (mState != 5 && mState != 4) {
        fn_8015DF7C();
    }
}

static u16 lbl_80539F70[4] = {904, 903, 902, 905};

// FIXME(shotgun): 98.1 %, different + reloc-name
void Flame_804A0330::fn_8015EA2C() {
    f32 dist = 0.0f;
    switch (mState) {
    case 0:
        if (vfunc_0x34()) {
            if (field_0x240 != 0 && (field_0x94 == 4 || field_0x94 == 5)) {
                fn_801F0D20(3);
            }
            field_0x240 = 0;
            if (field_0x94 == 6) {
                mState = 4;
            }
        } else {
            field_0x240 = 1;
        }
        break;
    case 1:
        if (vfunc_0x34()) {
            field_0x240 = 1;
        } else {
            field_0x240 = 0;
        }
        break;
    case 2:
        if (vfunc_0x34()) {
            field_0x240 = 0;
            mState = 4;
        } else {
            s32 dir;
            fn_801EEC98(mSerial, NULL, &dir, &dist, NULL, 0, 0);
            if (field_0x240 == 0) {
                if (dir >= 0 && dist <= 80.0f) {
                    field_0x240 = 1;
                }
            } else if (dir >= 0 && dist >= 96.0f) {
                field_0x234 = 8;
                field_0x240 = 0;
            }
        }
        break;
    case 3:
        if (vfunc_0x34()) {
            field_0x240 = 0;
            mState = 4;
        } else if (field_0x238 == 0) {
            if (field_0x23c == 0) {
                field_0x238 = 0x78;
                field_0x240 = 1;
            }
        } else if (field_0x238 == 1) {
            field_0x238 = 0;
            field_0x240 = 0;
            field_0x23c = 0xB4;
        }
        break;
    case 4: {
        field_0x240 = 0;
        if (field_0x260 < 0.1f) {
            bool played = false;
            for (s32 i = 0; i < 4; i++) {
                if (mEmitters[i] != NULL) {
                    mEmitters[i]->becomeInvalidEmitter();
                    mEmitters[i] = NULL;
                    if (!played) {
                        startSound_801F0E34(0x30061, 0);
                        played = true;
                    }
                }
            }
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80539F70[field_0x98], 1, 0, NULL, NULL, true, 1.0f);
            mState = 5;
        }
        break;
    }
    case 5:
        break;
    case 6:
        if (field_0x244 != 0) {
            vfunc_0x30(0xFF, true);
            mAlive = false;
        }
        break;
    case 7:
        if (field_0x244 != 0) {
            vfunc_0x30(0xFF, true);
            mAlive = false;
        }
        break;
    case 8:
        if (field_0x234 == 0) {
            if (field_0x248 == 0) {
                if (field_0x244 != 0) {
                    vfunc_0x30(0xFF, true);
                    field_0x234 = 8;
                    field_0x248 = 1;
                }
            } else if (field_0x244 == 0) {
                vfunc_0x30(0xFF, true);
                field_0x234 = 8;
                field_0x248 = 0;
            }
        }
        break;
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
