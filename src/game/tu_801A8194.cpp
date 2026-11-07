#include "game/GameAudio.h"
#include "game/act/tu_801A8194.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

// FIXME(shotgun): 46.8 %, missing/extra: the original copies mPos into the 40-entry trail with a loop unrolled eight times (five groups of eight entries); ours is rolled
void cls_804A2EC0::init() {
    mBox.i.x = -10.0f;
    mBox.i.y = -10.0f;
    mBox.f.x = 10.0f;
    mBox.f.y = 10.0f;
    field_0x80.i.x = 0.0f;
    field_0x80.i.y = 0.0f;
    field_0x80.f.x = 0.0f;
    field_0x80.f.y = 0.0f;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x270 = 72.0f;
    field_0x11e = 1;
    field_0x26c = 1.2f;
    field_0x274 = 255.0f;
    field_0x27c = mPos;
    field_0x288 = mPos;
    for (s32 i = 0; i < 40; i++) {
        field_0x294[i] = mPos;
    }
    field_0x268 = 0;
    field_0x190 = 0;
    field_0x278 = 0.0f;
    field_0x264 = mLife;
    mLife = 10000;
    field_0x260 = 0;
    field_0x230 = 0;
}

// FIXME(shotgun): 86.0 %, missing/extra: sqrtf expansion and trail-collision loop
void cls_804A2EC0::update() {
    s32 player = -1;
    if (field_0x258 == 0 && (field_0x90 >> 27) != 0) {
        if (!vfunc_0x34()) {
            return;
        }
        field_0x258 = 1;
    }
    if (getGameView_802D6A9C()->fn_802DCD0C()) {
        if (field_0x25c == 1) {
            mAlive = false;
            return;
        }
    } else {
        field_0x25c = 1;
    }
    fn_801EEC98(mSerial, NULL, NULL, NULL, &player, 0, 1);
    if (field_0x230 == 1 && field_0x238 == 0) {
        field_0x294[0] = mPos;
        for (s32 i = 39; i > 0; i--) {
            field_0x294[i] = field_0x294[i - 1];
        }
        field_0x238 = 3;
    }
    if (field_0x264 > 0) {
        JGeometry::TVec3f saved = mPos;
        mBox.i.x = -2.0f;
        mBox.i.y = -2.0f;
        mBox.f.x = 2.0f;
        mBox.f.y = 2.0f;
        for (s32 i = 39; i > 0; i--) {
            if (player >= 0) {
                f32 dx = getPlayerPos_8023E724(player)->x - field_0x294[i].x;
                f32 dy = getPlayerPos_8023E724(player)->y - (field_0x294[i].y - field_0x294[i].z);
                field_0x288 = *getPlayerPos_8023E724(player);
                f32 dist = std::sqrtf(dx * dx + dy * dy);
                if (dist < 16.0f) {
                    mPos.x = field_0x294[i].x;
                    mPos.y = field_0x294[i].y;
                    mPos.z = 0.0f;
                    fn_801F2B7C(field_0x114, -1, 5);
                }
            }
        }
        mBox.i.x = -10.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 10.0f;
        mPos = saved;
    }
    if (field_0x230 == 2) {
        addCalc2_801F71A4(&field_0x274, 0.0f, 0.3f, 5.0f);
    }
    if (fn_801F3D94(0) != 0) {
        return;
    }
    if (field_0x264 > 0) {
        for (s32 p = 0; p < 4; p++) {
            s32 hitter = fn_801F285C(NULL, NULL, true, p);
            if (hitter < 0) {
                continue;
            }
            if (gGameRoot_8053AAF8->fn_80138A30() > 1 || fn_8022CAF8(hitter) == 1 || fn_8022CAF8(hitter) == 0) {
                if (fn_801F2FAC()) {
                    field_0x264 -= 10000 - mLife;
                    if (field_0x264 <= 0) {
                        startSound_801F0E34(0x40003, 0);
                        field_0x264 = 0;
                        field_0x230 = 2;
                        return;
                    }
                    startSound_801F0E34(0x40002, 0);
                    field_0x230 = 0;
                    return;
                }
            }
        }
    }
    if (field_0x264 > 0 && field_0x230 != 0) {
        addCalc2_801F71A4(&mPos.x, field_0x27c.x, 1.0f, field_0x278);
        addCalc2_801F71A4(&mPos.y, field_0x27c.y, 1.0f, field_0x278);
        addCalc2_801F71A4(&field_0x278, 10.0f, 0.3f, 1.0f);
        addCalc2_801F71A4(&mPos.z, field_0x27c.z, 0.3f, 1.0f);
        field_0x27c.z = 16.0f;
        fn_801F2B7C(-100, -1, 1);
        startSound_801F0E34(0x400EC, 0);
        switch (field_0x230) {
        case 0:
            startSound_801F0E34(0x4000F, 0);
            startSound_801F0E34(0x400A9, 0);
            if (player >= 0) {
                fn_801EEC98(mSerial, &field_0x268, NULL, NULL, NULL, 0, 2);
                field_0x268 += 0x8000;
            }
            field_0x230 = 1;
            // fall through
        case 1:
            if (player >= 0) {
                field_0x27c.x = getPlayerPos_8023E724(player)->x + field_0x270 * JMath::sincosTable_.sinShort(field_0x268);
                field_0x27c.y = getPlayerPos_8023E724(player)->y + field_0x270 * JMath::sincosTable_.cosShort(field_0x268);
            } else {
                field_0x27c.x = field_0x288.x + field_0x270 * JMath::sincosTable_.sinShort(field_0x268);
                field_0x27c.y = field_0x288.y + field_0x270 * JMath::sincosTable_.cosShort(field_0x268);
            }
            field_0x27c.y += 16.0f;
            field_0x268 += 0x200;
            if (field_0x234 <= 0) {
                field_0x260++;
                if (field_0x260 > 2) {
                    field_0x260 = 0;
                }
                field_0x234 = 6;
            }
            break;
        case 2:
            if (field_0x274 < 1.0f) {
                for (s32 i = 0; i < 40; i++) {
                    getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &field_0x294[i], 0);
                }
                vfunc_0x24();
            }
            break;
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
}

// FIXME(shotgun): 75.3 %, missing/extra: the trail loop and the scale/alpha updates are scheduled differently
void cls_804A2EC0::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x258 == 0 && (field_0x90 >> 27) != 0 && !vfunc_0x34()) {
        return;
    }
    f32 scale = field_0x26c;
    mtx[0][0] = scale;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = scale;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scale;
    params.mMtx = mtx;
    fn_801F02BC("enemy_both2_sob.bin", field_0x260 + 0x155, &params, 7, 0, false, 1.0f, 1.0f);
    f32 alpha = field_0x274;
    s32 frame = field_0x260;
    JGeometry::TVec3f saved = mPos;
    for (s32 i = 1; i < 40; i++) {
        if (field_0x294[i].z != 0.0f) {
            scale *= 0.95f;
            if (scale < 0.5f) {
                scale = 0.5f;
            }
            alpha -= 2.0f;
            if (alpha < 0.0f) {
                alpha = 0.0f;
            }
            params.field_0x10.a = (s32)alpha;
            mtx[0][0] = scale;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = scale;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = scale;
            frame--;
            if (frame < 0) {
                frame = 2;
            }
            mPos = field_0x294[i];
            fn_801F02BC("enemy_both2_sob.bin", frame + 0x155, &params, 7, 0, false, 1.0f, 1.0f);
        }
    }
    mPos = saved;
}

cls_804A2EC0::~cls_804A2EC0() {}
