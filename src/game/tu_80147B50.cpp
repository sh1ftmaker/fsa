#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"
#include "game/act/tu_80147B50.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Tables of the 'DGTL' actor (not written as source here): .data 0x8049F630 emitter ids per
// direction, 0x8049F640 trail indices, 0x8049F698 resource names (.rodata strings);
// .sdata 0x80539F60 a segment-sprite order.
extern const u16 lbl_8049F630[];
extern const s32 lbl_8049F640[14];
extern const char* const lbl_8049F698[];
extern const u8 lbl_80539F60[];
extern s32 lbl_80539F58;
extern s32 lbl_80539F5C;
extern s32 lbl_8053AB30;
extern s32 lbl_8053AB34;
extern s32 lbl_8053AB38;

// FIXME(shotgun): 84.18 %, missing/extra + reloc-name.
void cls_8049F6C8::init() {
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x274 = 6;
    field_0x278 = 0;
    field_0x280 = field_0x278;
    field_0x250 = 300;
    field_0x27c = field_0x278;
    for (s32 i = 0; i < 10; i++) {
        mEmitters798[i] = NULL;
    }
    mEmitters7c0[0] = NULL;
    mEmitters7c0[1] = NULL;
    field_0x98 = (s32)(15.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
    field_0x298 = field_0x98;
    field_0x29c = -1;
    field_0x294 = 255.0f;
    field_0x196 = 1;
    u16 angle;
    switch (field_0x98) {
    case 0:
        angle = 0;
        break;
    case 1:
        angle = 0x2000;
        break;
    case 2:
        angle = 0x4000;
        break;
    case 3:
        angle = 0x6000;
        break;
    case 4:
        angle = 0x8000;
        break;
    case 5:
        angle = 0xA000;
        break;
    case 6:
        angle = 0xC000;
        break;
    case 7:
        angle = 0xE000;
        break;
    }
    field_0x284 = angle;
    field_0x286 = field_0x284;
    for (s32 i = 0; i < 10; i++) {
        field_0x2c0[i].set(mPos);
        field_0x2ac[i] = field_0x284;
    }
    for (s32 i = 0; i < 80; i++) {
        field_0x3d8[i].set(mPos);
        field_0x338[i] = field_0x284;
    }
    field_0x290 = 1.3f;
    field_0x2a0.x = field_0x290 * JMath::sincosTable_.sinShort(field_0x284);
    field_0x2a0.y = field_0x290 * JMath::sincosTable_.cosShort(field_0x284);
    field_0x2a0.z = 0.0f;
    field_0x288 = 0.3f;
    field_0x28c = 1.0f;
    field_0x90 &= 3;
    field_0x94 = 0;
    field_0xbc = 2;
    field_0xb0 = 0x4000004;
    getGameView_802D6A9C()->fn_802D7440(false);
    switch (field_0x90) {
    case 0:
        lbl_8053AB30 += 1;
        mState = 0;
        break;
    case 1:
        lbl_80539F58 = -1;
        lbl_80539F5C = -1;
        lbl_8053AB34 = 0;
        lbl_8053AB38 = field_0x4;
        gAudioMgr_8053AF80->fn_80458F9C(60);
        lbl_8053AB30 = 0;
        mState = 1;
        lbl_8053AB30 += 1;
        break;
    case 2:
        lbl_8053AB38 = field_0x4;
        // fall through
    case 3:
        mBox.i.x = -10.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 10.0f;
        field_0x248 = 20;
        mState = 8;
        break;
    }
}

// FIXME(shotgun): 85.20 %, missing/extra + reloc-name.
void cls_8049F6C8::fn_8014A2CC(s32 flag) {
    s32 base = 0;
    if (field_0x274 == 1) {
        base = 7;
    }
    if (flag == 0) {
        for (s32 i = 78; i >= 0; i--) {
            field_0x3d8[i + 1].set(field_0x3d8[i]);
            field_0x338[i + 1] = field_0x338[i];
        }
    } else {
        for (s32 i = 78; i >= 0; i--) {
            field_0x3d8[i + 1].z = field_0x3d8[i].z;
        }
    }
    if (flag == 0) {
        field_0x3d8[0].set(mPos);
        field_0x338[0] = field_0x284;
    } else {
        field_0x3d8[0].z = mPos.z;
    }
    for (s32 i = 0; i < 6; i++) {
        s32 index = lbl_8049F640[base + i];
        field_0x2c0[i + 1].x = field_0x3d8[index].x;
        field_0x2c0[i + 1].y = field_0x3d8[index].y;
        field_0x2c0[i + 1].z = field_0x3d8[index].z;
        field_0x2ac[i + 1] = field_0x338[index];
        if (flag != 0) {
            field_0x2c0[i + 1].z = field_0x3d8[(s32)(0.25f * lbl_8049F640[i])].z;
        }
        if (i < 5) {
            if (mEmitters798[i] == NULL) {
                mEmitters798[i] = spawnEmitter_8013CC50(field_0x4, &field_0x2c0[i + 1], 0x11C, 0, 0, NULL, NULL, true,
                                                        1.0f - 0.08f * i);
            } else {
                mEmitters798[i]->setGlobalAlpha((u8)(s32)field_0x294);
                setEmitterPos_8013CB44(mEmitters798[i], &field_0x2c0[i + 1], 0, 0, true);
            }
        }
    }
    s32 index = lbl_8049F640[6 + base];
    field_0x2c0[7].x = field_0x3d8[index].x;
    field_0x2c0[7].y = field_0x3d8[index].y;
    field_0x2c0[7].z = field_0x3d8[index].z;
    field_0x2ac[7] = field_0x338[index];
    for (s32 i = 0; i < 2; i++) {
        setEmitterPos_8013CB44(mEmitters7c0[i], &field_0x2c0[7], 0, 0, true);
    }
    if (flag != 0) {
        field_0x2c0[7].z = field_0x3d8[(s32)(0.25f * lbl_8049F640[6])].z;
    }
    field_0x2c0[0].x = field_0x3d8[0].x;
    field_0x2c0[0].y = field_0x3d8[0].y;
    field_0x2c0[0].z = field_0x3d8[0].z;
}

// FIXME(shotgun): 83.42 %, missing/extra + reloc-name.
bool cls_8049F6C8::fn_80149FDC() {
    if (field_0x250 == 0) {
        return true;
    }
    if (field_0x24c == 0) {
        field_0x24c = 300 - field_0x250;
        field_0x24c = (s32)(0.2f * field_0x24c);
        if (field_0x24c < 8) {
            field_0x24c = 8;
        }
        field_0x278 = (field_0x278 + 1) & 3;
        if (mState != 1 && mState != 2) {
            switch (field_0x278) {
            case 0:
                startSound_801F0E34(0x400BB, 0);
                break;
            case 1:
                startSound_801F0E34(0x400BC, 0);
                break;
            case 2:
                startSound_801F0E34(0x400BD, 0);
                break;
            case 3:
                startSound_801F0E34(0x400BE, 0);
                break;
            }
        }
    }
    if (field_0x250 != 1) {
        return false;
    }
    field_0x278 = field_0x27c;
    s32 tries = 0;
    while (field_0x27c == field_0x278 && tries < 100) {
        tries++;
        field_0x278 = (s32)(4.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
        field_0x278 &= 3;
    }
    field_0x27c = field_0x278;
    for (s32 i = 0; i < 2; i++) {
        deleteEmitter(&mEmitters7c0[i]);
    }
    const u16* ids = &lbl_8049F630[field_0x278 * 2];
    mEmitters7c0[0] = spawnEmitter_8013CC50(field_0x4, &mPos, ids[0], 0, 0, NULL, NULL, true, 1.0f);
    mEmitters7c0[1] = spawnEmitter_8013CC50(field_0x4, &mPos, ids[1], 0, 0, NULL, NULL, true, 1.0f);
    if (mState != 1 && mState != 2) {
        startSound_801F0E34(0x400BF, 0);
    }
    field_0x254 = 0xE10;
    return false;
}

// FIXME(shotgun): 97.37 %, missing/extra + reloc-name.
bool cls_8049F6C8::fn_80149AEC() {
    u8 hit = 0;
    if (field_0x240 != 0 || field_0x24c != 0 || field_0x250 != 0) {
        return false;
    }
    f32 x = mPos.x;
    f32 y = mPos.y;
    f32 z = mPos.z;
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mPos.x = field_0x2c0[7].x;
    mPos.y = field_0x2c0[7].y;
    mPos.z = field_0x2c0[7].z;
    field_0xb0 = 0x4000000;
    for (s32 i = 0; i < 4; i++) {
        s32 player = fn_801F285C(NULL, NULL, true, i);
        if (player >= 0) {
            if (player == field_0x278 || fn_8022EA0C(player) == field_0x278) {
                if (fn_801F2FAC() && mLife != 10000) {
                    field_0x270 = 0;
                    hit = 1;
                    mPos.z = 0.0f;
                    field_0x3c.z = 0.0f;
                }
            }
        }
    }
    mPos.x = x;
    mPos.y = y;
    mPos.z = z;
    if (!hit) {
        return false;
    }
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x2a0.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    for (s32 i = 0; i < 2; i++) {
        deleteEmitter(&mEmitters7c0[i]);
    }
    field_0x274 -= 1;
    if (field_0x274 <= 0) {
        field_0xd8 = 0;
        field_0xdc = 0;
        if (field_0x90 == 1) {
            lbl_80539F58 = -1;
        }
        if (field_0x90 == 0) {
            lbl_80539F5C = -1;
        }
        field_0x234 = 0;
        field_0x238 = 0;
        field_0x23c = 0;
        field_0x240 = 0;
        field_0x244 = 0;
        field_0x248 = 0;
        field_0x24c = 0;
        field_0x250 = 0;
        field_0x254 = 0;
        field_0x23c = 0xA8;
        for (s32 i = 0; i < 10; i++) {
            if (mEmitters798[i] != NULL) {
                mEmitters798[i]->becomeInvalidEmitter();
                mEmitters798[i] = NULL;
            }
        }
        gGameRoot_8053AAF8->fn_80134DE4(0xA8, 0);
        mState = 6;
    } else {
        if (field_0x274 == 1) {
            field_0x290 = 2.0f;
        }
        field_0x25c = 4;
        if (field_0x274 == 2) {
            if (field_0x90 == 1) {
                getActorManager_801F856C()->spawnActor_801F9484('DGTL', field_0x4, &mPos, 0, -1, -1);
                s32 serial = lbl_80539F58;
                if (serial >= 0) {
                    ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (other != NULL) {
                        lbl_80539F5C = getActorManager_801F856C()->spawnActor_801F9484(
                            'DGTL', other->field_0x4, &other->mPos, 3, -1, -1);
                    }
                }
                startSound_801F0E34(0x40023, 0);
            } else {
                startSound_801F0E34(0x40005, 0);
            }
        } else {
            startSound_801F0E34(0x40005, 0);
        }
        for (s32 i = 0; i < 80; i++) {
            field_0x3d8[i].set(mPos);
        }
    }
    return true;
}

// FIXME(shotgun): 97.82 %, different.
cls_8049F6C8::~cls_8049F6C8() {
    for (s32 i = 0; i < 10; i++) {
        releaseEmitter_801EE434(mEmitters798[i]);
    }
    for (s32 i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters7c0[i]);
    }
    if (field_0x90 == 1) {
        lbl_80539F58 = -1;
    }
    if (field_0x90 == 0) {
        lbl_80539F5C = -1;
    }
}

// FIXME(shotgun): 88.64 %, missing/extra + reloc-name.
void cls_8049F6C8::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1d = 1;
    if (field_0x90 == 2 || field_0x90 == 3) {
        if (lbl_8053AB34 == 1) {
            fn_801F02BC("enemy_both2_sob.bin", 0x176, NULL, -1, 0, true, 1.0f, 1.0f);
        }
        return;
    }
    JGeometry::TVec3f saved = mPos;
    mPos.x = field_0x2c0[7].x;
    mPos.y = field_0x2c0[7].y;
    mPos.z = field_0x2c0[7].z;
    setRotScaleMtx(mtx, JGeometry::TVec3f(0.0f, field_0x2ac[7] / -10430.219f, 0.0f), 1.0f);
    params.mMtx = mtx;
    fn_801EFD34(fn_804032E8('DEGT', lbl_8049F698[field_0x278 + 8]), &params, -1, 0.0f, 0.0f, 0.0f);
    for (s32 i = 4; i >= 0; i--) {
        params.field_0x1d = 6 - i;
        mPos.x = field_0x2c0[i + 1].x;
        mPos.y = field_0x2c0[i + 1].y;
        mPos.z = field_0x2c0[i + 1].z;
        f32 scale = 1.0f - 0.08f * i;
        setRotScaleMtx(mtx, JGeometry::TVec3f(0.0f, field_0x2ac[i + 1] / -10430.219f, 0.0f), scale);
        params.mMtx = mtx;
        fn_801EFD34(fn_804032E8('DEGT', lbl_8049F698[field_0x25c + 3]), &params, -1, 0.0f, 0.0f, 0.0f);
    }
    mPos.set(saved);
    params.field_0x1d = 7;
    setRotScaleMtx(mtx, JGeometry::TVec3f(0.0f, field_0x284 / -10430.219f, 0.0f), 1.0f);
    params.mMtx = mtx;
    fn_801EFD34(fn_804032E8('DEGT', lbl_8049F698[lbl_80539F60[field_0x258]]), &params, -1, 0.0f, 0.0f, 0.0f);
}

// FIXME(shotgun): update (fn_8014821C, 0x17F4 bytes) is not written.
