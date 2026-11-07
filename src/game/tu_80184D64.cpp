#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/Effect.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/tu_80184D64.h"

// 0x80186D24.
// FIXME(shotgun): 95.08 %, missing/extra + reloc-name
void cls_804A18E0::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.set(-10.0f, -10.0f, 10.0f, 10.0f);
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x98 = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x272 = fn_801F6874(field_0x98);
    field_0x264 = field_0x98 * 2 + field_0x24c;
    field_0x288[2] = 0.0f;
    field_0x288[1] = 0.0f;
    field_0x288[0] = 0.0f;
    field_0x384[0] = mPos.x;
    field_0x384[1] = mPos.y;
    field_0x384[2] = mPos.z;
    field_0x270 = 0xFFFF;
    for (s32 i = 0; i < 5; i++) {
        field_0x294[i * 2].x = mPos.x;
        field_0x294[i * 2].y = mPos.y;
        field_0x294[i * 2].z = mPos.z;
        field_0x30c[i * 2].x = mPos.x;
        field_0x30c[i * 2].y = mPos.y;
        field_0x30c[i * 2].z = mPos.z;
        field_0x274[i * 2] = 0;
        field_0x294[i * 2 + 1].x = mPos.x;
        field_0x294[i * 2 + 1].y = mPos.y;
        field_0x294[i * 2 + 1].z = mPos.z;
        field_0x30c[i * 2 + 1].x = mPos.x;
        field_0x30c[i * 2 + 1].y = mPos.y;
        field_0x30c[i * 2 + 1].z = mPos.z;
        field_0x274[i * 2 + 1] = 0;
    }
    field_0x26c = 0.0f;
    field_0x268 = 3;
    field_0x260 = -1;
    field_0x118 = 4;
    field_0xb0 = 0xA040004;
    field_0x230 = 0;
}

// 0x80184FC0.
// FIXME(shotgun): 83.50 %, missing/extra + reloc-name
void cls_804A18E0::update() {
    f32 box80_ix = field_0x80.i.x;
    f32 box80_iy = field_0x80.i.y;
    f32 box80_fx = field_0x80.f.x;
    f32 box80_fy = field_0x80.f.y;
    f32 posY = mPos.y;
    f32 posX = mPos.x;
    f32 outDist = 0.0f;
    s32 outDir = -1;
    mLife = 10000;

    if (field_0x270 == 0xFFFF) {
        if (!getGameView_802D6A9C()->fn_802DCD0C()) {
            JGeometry::TVec3f p;
            p.x = field_0x384[0];
            p.y = field_0x384[1];
            field_0x270 = fn_80226998(&p, field_0x4);
        }
        return;
    }

    addCalc2_801F71A4(&field_0x3c.x, field_0x288[0], 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x288[1], 0.3f, 1.0f);
    mPos += field_0x3c;
    fn_801EF510();

    for (int i = 0; i < 4; i++) {
        s32 hitPlayer = fn_801F285C(NULL, NULL, false, i);
        if (hitPlayer >= 0 && field_0x260 != hitPlayer) {
            f32 y0 = mBox.i.y + mPos.y;
            f32 y1 = mBox.f.y + mPos.y;
            f32 x0 = mBox.i.x + mPos.x;
            f32 x1 = mBox.f.x + mPos.x;
            JGeometry::TVec2f center(0.5f * (x0 - x1) + x1, 0.5f * (y0 - y1) + y1);
            if (fn_80237948(hitPlayer, &center)) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x400D1, 0);
            }
        }
    }

    if (fn_801F2B7C(field_0x114, field_0x260, 6) && field_0x230 == 8 && field_0x238 == 0) {
        field_0x3c.x *= -1.0f;
        field_0x3c.y *= -1.0f;
        field_0x288[2] = 0.0f;
        field_0x288[1] = 0.0f;
        field_0x288[0] = 0.0f;
        field_0x3c.z = 10.0f;
        field_0x238 = 20;
        if (field_0x268 > 0) {
            field_0x268--;
        }
    }

    if (field_0x230 == 7 || field_0x230 == 8) {
        fn_801F2618(NULL, false);
    }

    if (field_0x260 < 0) {
        JGeometry::TVec2f origin(field_0x384[0], field_0x384[1]);
        JGeometry::TBox2f box(mBox2);
        box.i.x += origin.x;
        box.i.y += origin.y;
        box.f.x += origin.x;
        box.f.y += origin.y;
        field_0x260 = fn_8023AA18(&box, field_0x4, &origin);
        if (field_0x260 >= 0) {
            JGeometry::TVec3f p;
            p.x = field_0x384[0];
            p.y = field_0x384[1];
            if (field_0x270 == fn_80226998(&p, field_0x4)) {
                field_0x260 = -1;
            } else {
                field_0x240 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                field_0x230 = 6;
            }
        }
    }

    switch (field_0x230) {
    case 0:
        field_0x234 = 0;
        field_0x24c = 0;
        field_0x238 = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 30;
        field_0x26c = 8.0f;
        field_0x288[2] = 0.0f;
        field_0x288[1] = 0.0f;
        field_0x288[0] = 0.0f;
        field_0x230 = 1;
        // fallthrough
    case 1: {
        f32 dy = mPos.y - field_0x384[1];
        f32 dx = mPos.x - field_0x384[0];
        if (JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy) >= 48.0f) {
            mPos.x = field_0x294[9].x;
            mPos.y = field_0x294[9].y;
            mPos.z = field_0x294[9].z;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x288[2] = 0.0f;
            field_0x288[1] = 0.0f;
            field_0x288[0] = 0.0f;
            field_0x272 = (s16)(JMath::atanTable_.atan2_(-dx, -dy) *
                                (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
            field_0x238 = (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
        } else {
            field_0x294[9].x = mPos.x;
            field_0x294[9].y = mPos.y;
            field_0x294[9].z = mPos.z;
            field_0x30c[9].x = mPos.x;
            field_0x30c[9].y = mPos.y;
            field_0x30c[9].z = mPos.z;
        }
        if (field_0x238 == 0 && field_0x23c == 0) {
            field_0x23c = (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x288[2] = 0.0f;
            field_0x288[1] = 0.0f;
            field_0x288[0] = 0.0f;
        }
        if (field_0x23c == 1) {
            field_0x238 = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 30;
            field_0x272 += (s32)(16384.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            startSound_801F0E34(0x400B0, 0);
        }
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            if (field_0x23c == 0) {
                field_0x3c.z = 3.0f;
            }
        }
        if (field_0x23c == 0) {
            field_0x288[0] = JMath::sincosTable_.sinShort(field_0x272);
            field_0x288[1] = JMath::sincosTable_.cosShort(field_0x272);
            field_0x98 = fn_801F681C(field_0x272);
        }
        if (field_0x240 == 0 && field_0x23c != 0) {
            for (int i = 0; i < 4; i++) {
                if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                    JGeometry::TVec3f diff = *getPlayerPos_8023E724(i) - mPos;
                    if (diff.squared() < 4096.0f) {
                        fn_801EEC98(mSerial, &field_0x272, NULL, NULL, NULL, i, 2);
                        field_0x230 = 2;
                        break;
                    }
                }
            }
        }
        break;
    }
    case 2:
        field_0x288[0] = 4.0f * JMath::sincosTable_.sinShort(field_0x272);
        field_0x288[1] = 4.0f * JMath::sincosTable_.cosShort(field_0x272);
        field_0x98 = fn_801F681C(field_0x272);
        startSound_801F0E34(0x400B0, 0);
        field_0x26c = 12.0f;
        field_0x264 = field_0x98 * 2;
        if (field_0x98 != 3) {
            field_0x264++;
        }
        field_0x3c.z = 7.0f;
        field_0x230 = 3;
        break;
    case 3: {
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        }
        f32 dy = mPos.y - field_0x384[1];
        f32 dx = mPos.x - field_0x384[0];
        if (JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy) < 64.0f) {
            break;
        }
        field_0x288[2] = 0.0f;
        field_0x288[1] = 0.0f;
        field_0x288[0] = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x238 = 30;
        field_0x230 = 4;
    }
        // fallthrough
    case 4:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        }
        if (field_0x238 != 0) {
            break;
        }
        startSound_801F0E34(0x400B0, 0);
        field_0x26c = 8.0f;
        field_0x230 = 5;
        // fallthrough
    case 5: {
        f32 dx = mPos.x - field_0x384[0];
        f32 dy = mPos.y - field_0x384[1];
        field_0x272 = (s16)(JMath::atanTable_.atan2_(-dx, -dy) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        field_0x288[0] = JMath::sincosTable_.sinShort(field_0x272);
        field_0x288[1] = JMath::sincosTable_.cosShort(field_0x272);
        field_0x98 = fn_801F681C(field_0x272);
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 3.0f;
        }
        if (JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy) < 24.0f) {
            field_0x240 = (s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 20;
            field_0x230 = 0;
        }
        break;
    }
    case 6: {
        f32 dx = mPos.x - field_0x384[0];
        f32 dy = mPos.y - field_0x384[1];
        field_0x272 = (s16)(JMath::atanTable_.atan2_(-dx, -dy) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        field_0x98 = fn_801F681C(field_0x272);
        f32 distSq = dx * dx + dy * dy;
        field_0x288[0] = 2.0f * JMath::sincosTable_.sinShort(field_0x272);
        field_0x288[1] = 2.0f * JMath::sincosTable_.cosShort(field_0x272);
        if (JGeometry::TUtil<f32>::sqrt(distSq) < 16.0f) {
            field_0x288[2] = 0.0f;
            field_0x288[1] = 0.0f;
            field_0x288[0] = 0.0f;
            field_0x230 = 7;
        }
        break;
    }
    case 7:
        if (field_0x248 == 0 && gGameRoot_8053AAF8->fn_80138A30() > 1) {
            for (int i = 0; i < 4; i++) {
                if (i != field_0x260 && fn_8023F264(i) && !fn_8022F478(i) &&
                    field_0x4 == getPlayerLayer_8023DE58(i)) {
                    JGeometry::TVec3f diff = *getPlayerPos_8023E724(i) - mPos;
                    if (diff.squared() < 6400.0f) {
                        field_0x254 = i;
                        field_0x238 = 0;
                        field_0x25c = 0;
                        field_0x23c = 0;
                        field_0x258 = 0;
                        field_0x240 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                        field_0x230 = 8;
                        return;
                    }
                }
            }
        }
        if (field_0x240 == 0 && field_0x25c == 0) {
            field_0x254 = getActorManager_801F856C()->fn_801F7664(&mPos, NULL);
            if (field_0x254 >= 0 &&
                field_0x4 == getActorManager_801F856C()->findActorBySerial_801F666C(field_0x254)->field_0x4) {
                field_0x258 = 1;
                field_0x25c = 0;
                field_0x238 = 0;
                field_0x23c = 0;
                field_0x240 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                field_0x230 = 8;
                return;
            }
            s32 end = getActorManager_801F856C()->mActorEnd;
            s32 found = 0;
            for (s32 i = 0; i < end; i++) {
                ActorBase_804A5770* act = getActorManager_801F856C()->mActors[i];
                if (act != NULL && act->field_0x4 == field_0x4 && act->field_0x1a8 == true) {
                    found = 1;
                    break;
                }
            }
            if (found == 0) {
                field_0x272 = fn_801F6874(field_0x98);
                field_0x288[0] = 2.0f * JMath::sincosTable_.sinShort(field_0x272);
                field_0x288[1] = 2.0f * JMath::sincosTable_.cosShort(field_0x272);
                field_0x258 = 0;
                field_0x230 = 9;
                return;
            }
        }
        field_0x25c++;
        field_0x25c &= 0x1F;
        fn_801EEC98(mSerial, &field_0x272, &outDir, &outDist, NULL, field_0x260, 2);
        if (outDist > 48.0f) {
            field_0x288[0] = JMath::sincosTable_.sinShort(field_0x272);
            field_0x288[1] = JMath::sincosTable_.cosShort(field_0x272);
        } else if (field_0x238 == 0 && field_0x23c == 0) {
            startSound_801F0E34(0x400B0, 0);
            field_0x23c = (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
            field_0x288[2] = 0.0f;
            field_0x288[1] = 0.0f;
            field_0x288[0] = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
        }
        if (field_0x23c == 1) {
            field_0x272 += (s32)(8192.0f - 16384.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            field_0x288[0] = JMath::sincosTable_.sinShort(field_0x272);
            field_0x288[1] = JMath::sincosTable_.cosShort(field_0x272);
            field_0x238 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
        }
        if (outDir != -1) {
            field_0x98 = outDir;
        }
        break;
    case 8:
        if (field_0x238 == 0) {
            outDir = -1;
            if (field_0x258 == 0) {
                fn_801EEC98(mSerial, &field_0x272, &outDir, &outDist, NULL, field_0x254, 2);
            } else {
                ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x254);
                if (target != NULL && target->mLife > 0) {
                    f32 dx = mPos.x - target->mPos.x;
                    f32 dy = mPos.y - target->mPos.y;
                    field_0x272 = (s16)(JMath::atanTable_.atan2_(-dx, -dy) *
                                        (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
                    outDir = fn_801F681C(field_0x272);
                }
            }
            if (outDir >= 0) {
                field_0x288[0] = 3.0f * JMath::sincosTable_.sinShort(field_0x272);
                field_0x288[1] = 3.0f * JMath::sincosTable_.cosShort(field_0x272);
                field_0x98 = outDir;
            } else if (field_0x258 == 0) {
                for (int i = 0; i < 4; i++) {
                    if (i != field_0x260 && fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                        field_0x238 = 0;
                        field_0x254 = i;
                    }
                }
            } else {
                field_0x240 = 0;
                field_0x25c = 1;
            }
        }
        if (field_0x240 == 0) {
            field_0x238 = 0;
            field_0x23c = 0;
            field_0x240 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
            field_0x248 = 100;
            field_0x230 = 7;
        }
        break;
    case 9: {
        f32 x0 = box80_ix + posX;
        f32 x1 = box80_fx + posX;
        f32 y0 = box80_iy + posY;
        f32 y1 = box80_fy + posY;
        JGeometry::TVec3f center;
        center.x = 0.5f * (x0 - x1) + x1;
        center.y = 0.5f * (y0 - y1) + y1;
        if (fn_802265FC(&center, field_0x4) == 0xC || fn_801F6758(&mPos, field_0x4, -16.0f)) {
            if (field_0x258 == 0) {
                field_0x258 = 1;
                field_0x238 = 100;
                startSound_801F0E34(0x400B0, 0);
                field_0x288[2] = 0.0f;
                field_0x288[1] = 0.0f;
                field_0x288[0] = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
            }
        }
        if (field_0x258 != 0) {
            if (mPos.z > 0.0f) {
                field_0x3c.z -= 1.0f;
            }
            if (mPos.z <= 0.0f) {
                mPos.z = 0.0f;
            }
            fn_801EEC98(mSerial, NULL, &outDir, NULL, NULL, field_0x260, 2);
            if (outDir >= 0) {
                field_0x98 = outDir;
            }
            if (field_0x238 == 0) {
                field_0x258 = 2;
                field_0x288[0] = 2.0f * JMath::sincosTable_.sinShort(field_0x272);
                field_0x288[1] = 2.0f * JMath::sincosTable_.cosShort(field_0x272);
                if (outDir >= 0) {
                    field_0x98 = outDir ^ 1;
                }
            }
        }
        if (field_0x258 == 2 && fn_801F6758(&mPos, field_0x4, 16.0f)) {
            mAlive = false;
        }
        break;
    }
    }

    if (field_0x230 == 1 || (field_0x230 >= 5 && field_0x230 <= 8) || field_0x230 == 9) {
        if (field_0x23c == 0 && field_0x234 == 0) {
            if (field_0x98 == 3) {
                field_0x24c = 0;
            }
            field_0x264 = field_0x98 * 2 + field_0x24c;
            field_0x24c++;
            field_0x24c &= 1;
            field_0x234 = 6;
        }
    }

    if (field_0x230 == 6 || (field_0x230 >= 7 && field_0x230 <= 8) || field_0x230 == 9) {
        fn_801F2B20(0);
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 1.0f;
        }
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            if ((field_0x3c.x != 0.0f || field_0x3c.y != 0.0f) && field_0x23c == 0) {
                if (field_0x230 == 7) {
                    field_0x3c.z = 3.0f;
                } else {
                    field_0x3c.z = 5.0f;
                    startSound_801F0E34(0x400B0, 0);
                }
            }
        }
        if (field_0x244 == 0 && (field_0x230 != 7 || field_0x3c.x != 0.0f || field_0x3c.y != 0.0f)) {
            field_0x30c[9].x = mPos.x;
            field_0x30c[9].y = mPos.y;
        }
        field_0x30c[9].z = mPos.z;
        for (int i = 1; i < 9; i++) {
            if (field_0x244 == 0 && (field_0x230 != 7 || field_0x3c.x != 0.0f || field_0x3c.y != 0.0f)) {
                field_0x30c[i].x = field_0x30c[i + 1].x;
                field_0x30c[i].y = field_0x30c[i + 1].y;
            }
            field_0x30c[i].z = field_0x30c[i + 1].z;
        }
        if (field_0x244 == 0) {
            field_0x244 = 8;
            if (field_0x230 == 8 || field_0x230 == 9) {
                field_0x244 = 2;
            }
        }
        for (int i = 1; i < 9; i++) {
            addCalc2_801F71A4(&field_0x294[i].x, field_0x30c[i].x, 0.5f, 5.0f);
            addCalc2_801F71A4(&field_0x294[i].y, field_0x30c[i].y, 0.5f, 5.0f);
            field_0x294[i].z = field_0x30c[i].z * (0.1f * i);
        }
    }

    if (field_0x230 == 3 || field_0x230 == 4 || field_0x230 == 5) {
        field_0x294[9].x = mPos.x;
        field_0x294[9].y = mPos.y;
        field_0x294[9].z = mPos.z;
        field_0x30c[9].x = mPos.x;
        field_0x30c[9].y = mPos.y;
        field_0x30c[9].z = mPos.z;
    }

    if (field_0x260 < 0) {
        for (int i = 1; i < 9; i++) {
            field_0x30c[i - 1].x = field_0x30c[i].x;
            field_0x30c[i - 1].y = field_0x30c[i].y;
            field_0x30c[i - 1].z = field_0x30c[i].z;
        }
        for (int i = 1; i < 9; i++) {
            f32 dx = field_0x294[i - 1].x - field_0x30c[i].x;
            f32 dy = field_0x294[i - 1].y - field_0x30c[i].y;
            s16 ang = (s16)(JMath::atanTable_.atan2_(-dx, -dy) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
            f32 tx = field_0x26c * JMath::sincosTable_.sinShort(ang) + field_0x294[i - 1].x;
            if (JGeometry::TUtil<f32>::abs(field_0x294[i].x - tx) >= 4.0f) {
                addCalc2_801F71A4(&field_0x294[i].x, tx, 0.5f, 10.0f);
            }
            f32 ty = field_0x26c * JMath::sincosTable_.cosShort(ang) + field_0x294[i - 1].y;
            if (JGeometry::TUtil<f32>::abs(field_0x294[i].y - ty) >= 4.0f) {
                addCalc2_801F71A4(&field_0x294[i].y, ty, 0.5f, 10.0f);
            }
            field_0x294[i].z = field_0x30c[i].z * (0.1f * i);
        }
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
    if (field_0x240 > 0) {
        field_0x240--;
    }
    if (field_0x244 > 0) {
        field_0x244--;
    }
    if (field_0x248 > 0) {
        field_0x248--;
    }
}

// 0x80184DC4.
void cls_804A18E0::draw() {
    SpriteParams_801F5AE0 params;
    JGeometry::TVec3f savedPos = mPos;
    s32 i = 0;
    if (field_0x260 >= 0) {
        i = 1;
    }
    for (; i < 9; i++) {
        mPos.x = field_0x294[i].x;
        mPos.y = field_0x294[i].y;
        mPos.z = field_0x294[i].z;
        params.field_0x20 = 0;
        params.field_0x1d = -1;
        drawSprite_801F06F0("enemy_gc_sob.bin", 0x13B, &params, 7, 1.0f, 1.0f);
    }
    mPos.x = savedPos.x;
    mPos.y = savedPos.y;
    mPos.z = savedPos.z;
    params.field_0x20 = 0;
    params.field_0x1d = 0;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x264 + 0x134, &params, 0, 1.0f, 1.0f);
}

// Destructor at 0x80184D64.
cls_804A18E0::~cls_804A18E0() {}
