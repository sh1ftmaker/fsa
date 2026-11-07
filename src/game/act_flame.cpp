#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/DrawShadow.h"
#include "game/Effect.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorManager.h"
#include "game/Map.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAMath.h"
#include "game/act/act_flame.h"

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

// FIXME(shotgun): tables not written out (gate.py literal tripwire); extern-declared, sizes
// from symbols.txt. By direction (field_0x98).
struct Tables_80466110 {
    /* 0x00 */ s32 mRotY[4];                // the emitters' y rotation
    /* 0x10 */ u8 field_0x10[0x10];         // not used here
    /* 0x20 */ JGeometry::TBox2f mBoxes[4]; // mBox while burning
    /* 0x60 */ JGeometry::TBox2f mHitBoxes[4]; // field_0x80
}; // Size: 0xA0
extern const Tables_80466110 lbl_80466110;
extern JGeometry::TVec2f lbl_804A0310[4]; // the emitter offset from mPos

// 0x8015DF7C.
// FIXME(shotgun): 0 %, new
void Flame_804A0330::fn_8015DF7C() {
    JGeometry::TBox2f box;
    box.set(mBox);
    box.addPos(mPos.x, mPos.y);
    if (field_0x254 != 0) {
        s32 player = fn_8023B004(&box, field_0x4);
        if (player >= 0) {
            if (field_0x244 == 0) {
                fn_801F0D20(3);
                fn_80230FD0(player, 0x30009);
                field_0x244 = 1;
            }
        } else {
            field_0x244 = 0;
        }
        return;
    }
    if (field_0x240 != 0) {
        f32 target = 1.0f;
        if (field_0x94 == 4) {
            target = 0.33f;
        }
        addCalc2_801F71A4(&field_0x260, target, 0.1f, 1.0f);
        if (field_0x94 != 4 && field_0x94 != 5) {
            mBox.set(lbl_80466110.mBoxes[field_0x98]);
            if (field_0x258 == 1) {
                mBox.i.x *= 1.5f;
                mBox.i.y *= 1.5f;
                mBox.f.x *= 1.5f;
                mBox.f.y *= 1.5f;
            }
            if (field_0x260 > 0.7f) {
                fn_801F2B20(5);
                if (!fn_801F2B7C(field_0x114, -1, 9) && gGameRoot_8053AAF8->field_0x7f == 1) {
                    field_0x80.set(lbl_80466110.mHitBoxes[field_0x98]);
                    if (field_0x258 == 1) {
                        field_0x80.i.x *= 1.5f;
                        field_0x80.i.y *= 1.5f;
                        field_0x80.f.x *= 1.5f;
                        field_0x80.f.y *= 1.5f;
                    }
                    box = field_0x80;
                    box.addPos(mPos.x, mPos.y);
                    fn_80239914(&box, field_0x4, mSerial);
                }
            }
        } else {
            if (field_0x94 == 5) {
                mBox.set(-40.0f, -50.0f, 40.0f, 44.0f);
                field_0x80.set(-36.0f, -45.0f, 36.0f, 40.0f);
            } else {
                mBox.set(-18.0f, -18.0f, 18.0f, 24.0f);
                field_0x80.set(-14.0f, -14.0f, 14.0f, 20.0f);
            }
            box = mBox;
            box.addPos(mPos.x, mPos.y);
            fn_801F2B20(5);
            if (!fn_801F2B7C(field_0x114, -1, 9)) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8023F264(i) && getPlayerLayer_8023DE58(i) != 8) {
                        JGeometry::TVec3f* ppos = getPlayerPos_8023E724(i);
                        JGeometry::TVec2f d(ppos->x, ppos->y);
                        d.x -= mPos.x;
                        d.y -= mPos.y;
                        f32 sq = d.x * d.x + d.y * d.y;
                        if (sq < 900.0f) {
                            if (sq > JGeometry::TUtil<f32>::epsilon()) {
                                f32 inv = JGeometry::TUtil<f32>::inv_sqrt(sq);
                                d.x *= inv;
                                d.y *= inv;
                            }
                            if (d.x * d.x + d.y * d.y > JGeometry::TUtil<f32>::epsilon()) {
                                d.x *= 16.0f;
                                d.y *= 16.0f;
                            }
                            JGeometry::TVec3f push(d.x, d.y, 0.0f);
                            fn_8023DFF4(i, &push);
                        }
                    }
                }
            }
        }
    } else {
        addCalc2_801F71A4(&field_0x260, 0.0f, 0.1f, 1.0f);
        mBox.set(1.0f, -1.0f, 1.0f, -1.0f);
        field_0x80.set(1.0f, -1.0f, 1.0f, -1.0f);
    }
    if (field_0x94 != 4 && field_0x94 != 5) {
        JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
        pos.x = mPos.x + lbl_804A0310[field_0x98].x;
        pos.y = mPos.y + lbl_804A0310[field_0x98].y;
        if (field_0x260 < 0.1f) {
            if (mEmitters[0] != NULL) {
                mEmitters[0]->becomeInvalidEmitter();
            }
            if (mEmitters[1] == NULL) {
                u16 id;
                if (mEmitters[0] != NULL) {
                    id = 0x1AD;
                    if (field_0x25c == 1) {
                        id = 0x253;
                    }
                } else {
                    id = 0x1AE;
                    if (field_0x25c == 1) {
                        id = 0x253;
                    }
                }
                mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &pos, id, 1, 1, NULL, NULL, true,
                                                     0.5f * field_0x258 + 1.0f);
            }
            mEmitters[0] = NULL;
            if (mEmitters[1] != NULL) {
                s16 rx = 0;
                if (field_0x98 == 2) {
                    rx = 0x4000;
                }
                setEmitterPos_8013CB44(mEmitters[1], &pos, 1, 1, true);
                JGeometry::TVec3<s16> rot;
                rot.x = rx;
                rot.y = lbl_80466110.mRotY[field_0x98];
                rot.z = -0x8000;
                mEmitters[1]->setGlobalRotation(rot);
            }
        } else {
            if (mEmitters[1] != NULL) {
                mEmitters[1]->becomeInvalidEmitter();
                mEmitters[1] = NULL;
            }
            if (mEmitters[0] == NULL) {
                u16 id = 0x1AC;
                if (field_0x25c == 1) {
                    id = 0x252;
                }
                mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &pos, id, 1, 1, NULL, NULL, true,
                                                     0.5f * field_0x258 + 1.0f);
            }
            if (mEmitters[0] != NULL) {
                s16 rx = 0;
                if (field_0x98 == 2) {
                    rx = 0x4000;
                }
                setEmitterPos_8013CB44(mEmitters[0], &pos, 1, 1, true);
                JGeometry::TVec3<s16> rot;
                rot.x = rx;
                rot.y = lbl_80466110.mRotY[field_0x98];
                rot.z = -0x8000;
                mEmitters[0]->setGlobalRotation(rot);
                if (mBox.i.x != 1.0f && mBox.f.x != -1.0f) {
                    fn_8020D58C()->addBox(mSerial);
                }
                startSound_801F0E34(0x30011, 0);
            }
        }
        field_0x24c++;
        if (field_0x24c > 10) {
            s32 count = 5;
            if (field_0x260 < 0.1f) {
                count = 1;
            }
            for (s32 i = 0; i < count; i++) {
                JGeometry::TVec3f spark(mPos.x, mPos.y, mPos.z);
                f32 r = 16.0f * (f32)i;
                spark.x = r * JMath::sincosTable_.sinShort(fn_801F6874(field_0x98)) + spark.x;
                spark.y = r * JMath::sincosTable_.cosShort(fn_801F6874(field_0x98)) + spark.y;
                spawnEmitter_8013CC50(field_0x4, &spark, 0x54D, 1, 1, NULL, NULL, true, 1.0f);
            }
            field_0x24c = 0;
        }
        return;
    }
    if (mEmitters[0] == NULL) {
        return;
    }
    mEmitters[0]->setGlobalAlpha(255.0f * field_0x260);
    mEmitters[1]->setGlobalParticleScale(field_0x260, field_0x260);
    mEmitters[2]->setGlobalParticleScale(field_0x260, field_0x260);
    if (field_0x260 > 0.1f) {
        field_0x24c++;
        if (field_0x24c > 10) {
            u16 angle = 0;
            for (s32 i = 0; i < 8; i++) {
                JGeometry::TVec3f spark(mPos.x, mPos.y, mPos.z);
                f32 r;
                if (field_0x94 == 5) {
                    r = 40.0f;
                } else {
                    r = 20.0f;
                }
                spark.x = r * JMath::sincosTable_.sinShort(angle) + spark.x;
                spark.y = r * JMath::sincosTable_.cosShort(angle) + spark.y;
                spawnEmitter_8013CC50(field_0x4, &spark, 0x54D, 1, 1, NULL, NULL, true, 1.0f);
                angle += 0x2000;
            }
            field_0x24c = 0;
        }
        if (mBox.i.x != 1.0f && mBox.f.x != -1.0f) {
            fn_8020D58C()->addBox(mSerial);
        }
        startSound_801F0E34(0x30011, 0);
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
