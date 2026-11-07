#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/tu_8018EB94.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/ActorManager.h"
#include "JSystem/JMath/JMath.h"

extern s32 lbl_8053AB88;
s32 lbl_80539FD8[2] = {8, 14};
s32 lbl_80539FE0[2] = {3, 5};


// Destructor at 0x8018FB24.
cls_804A1FA0::~cls_804A1FA0() {
    for (int i = 0; i < 3; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// FIXME(shotgun): 85.97 %, missing/extra + reloc-name
void cls_804A1FA0::init() {
    field_0x80.i.x = -6.0f;
    field_0x80.i.y = -6.0f;
    field_0x80.f.x = 6.0f;
    field_0x80.f.y = 6.0f;
    mBox.i.x = -6.0f;
    mBox.i.y = -6.0f;
    mBox.f.x = 6.0f;
    mBox.f.y = 6.0f;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x234[0] = 0;
    field_0x234[1] = 0;
    field_0x234[2] = 0;
    field_0x234[3] = 0;
    field_0x234[4] = 0;
    field_0x288[0].set(mPos);
    field_0x288[1].set(mPos);
    field_0x288[2].set(mPos);
    field_0x288[3].set(mPos);
    field_0x288[4].set(mPos);
    field_0x288[5].set(mPos);
    for (u8 i = 0; i < 20; i++) {
        field_0x2d0[i].set(mPos);
    }
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    field_0x98 = (s32)(15.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x25c = field_0x98;
    field_0x260 = field_0x98;
    field_0x264 = -1;
    field_0x27c = field_0x278 * JMath::sincosTable_.sinShort((s32)(4000.0f * (f32)field_0x98));
    field_0x280 = field_0x278 * JMath::sincosTable_.cosShort((s32)(4000.0f * (f32)field_0x98));
    field_0x284 = 0.0f;
    field_0x234[2] = 8;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x268 = 0.3f;
    field_0x26c = 1.0f;
    field_0x270 = 1.0f;
    field_0x274 = 0.0f;
    field_0x278 = 1.3f;
    if ((lbl_8053AB88 & 1) == 0) {
        field_0x254 = 1;
    } else {
        field_0x254 = -1;
    }
    lbl_8053AB88++;
    lbl_8053AB88 = lbl_8053AB88 & 1;
    field_0xb0 = 0x12202;
    field_0x230 = 0;
    s16 angle = (s32)(4000.0f * (f32)field_0x98);
    field_0x27c = field_0x278 * JMath::sincosTable_.sinShort(angle);
    field_0x280 = field_0x278 * JMath::sincosTable_.cosShort((s32)(4000.0f * (f32)field_0x98));
    field_0x288[4].set(mPos);
    angle = (s32)(4000.0f * (f32)((field_0x98 - 2) & 0xF));
    field_0x288[4].x += 6.0f * JMath::sincosTable_.sinShort(angle);
    field_0x288[4].y += 6.0f * JMath::sincosTable_.cosShort(angle);
    field_0x288[5].set(mPos);
    angle = (s32)(4000.0f * (f32)((field_0x98 + 2) & 0xF));
    field_0x288[5].x += 6.0f * JMath::sincosTable_.sinShort(angle);
    field_0x288[5].y += 6.0f * JMath::sincosTable_.cosShort(angle);
}

// FIXME(shotgun): 80.47 %, missing/extra + reloc-name
void cls_804A1FA0::update() {
    JGeometry::TVec3f savedPos = mPos;
    f32 z0 = mPos.z;
    JGeometry::TVec3f savedVel = field_0x3c;
    bool hit = false;
    for (u8 i = 0; i < 5; i++) {
        if (field_0x234[i] > 0) {
            field_0x234[i]--;
        }
    }
    if (field_0x230 == 0) {
        for (s32 i = 0; i < 3; i++) {
            mPos.set(field_0x288[i]);
            if (fn_801F3D94(true) != 0 || fn_801F305C()) {
                fn_801F2618(NULL, false);
                hit = true;
                break;
            }
        }
        if (!hit) {
            mPos.set(savedPos);
            mPos.z = z0;
            addCalc2_801F71A4(&field_0x3c.x, field_0x27c, field_0x268, field_0x26c);
            addCalc2_801F71A4(&field_0x3c.y, field_0x280, field_0x268, field_0x26c);
            mPos.x += field_0x3c.x;
            mPos.y += field_0x3c.y;
            mPos.z += field_0x3c.z;
            fn_801F2B7C(field_0x114, -1, 1);
            if (fn_801F2618(NULL, false) && field_0x234[1] == 0) {
                field_0x98 = (field_0x98 + 8) & 0xF;
                if (field_0x264 == field_0x98) {
                    field_0x98 = (field_0x98 + field_0x254) & 0xF;
                }
                field_0x264 = field_0x260;
                field_0x260 = field_0x98;
                field_0x25c = field_0x98;
                field_0x234[1] = 8;
                field_0x234[2] = 8;
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                    field_0x254 = -field_0x254;
                }
                startSound_801F0E34(0x4000B, 0);
                field_0x27c = field_0x278 * JMath::sincosTable_.sinShort((s32)(4000.0f * (f32)field_0x98));
                field_0x280 = field_0x278 * JMath::sincosTable_.cosShort((s32)(4000.0f * (f32)field_0x98));
                field_0x3c.x = field_0x27c;
                field_0x3c.y = field_0x280;
                field_0x3c.z = field_0x284;
            }
        }
        for (s32 i = 18; i >= 0; i--) {
            field_0x2d0[i + 1].set(field_0x2d0[i]);
            field_0x2d0[i + 1].z = z0;
        }
        mPos.z = z0;
        field_0x2d0[0].set(mPos);
        if (hit) {
            field_0x288[1].set(field_0x2d0[lbl_80539FE0[0]]);
            field_0x288[2].set(field_0x2d0[lbl_80539FE0[1]]);
            field_0x288[0].set(field_0x2d0[0]);
        } else {
            field_0x288[1].set(field_0x2d0[lbl_80539FD8[0]]);
            field_0x288[2].set(field_0x2d0[lbl_80539FD8[1]]);
            field_0x288[0].set(field_0x2d0[0]);
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &field_0x288[1], 0x11C, 0, -1, NULL, NULL, true, 0.4f);
        } else {
            setEmitterPos_8013CB44(mEmitters[1], &field_0x288[1], 0, -1, true);
        }
        if (field_0x234[2] == 0 && !hit) {
            u16 outAngle = 0;
            field_0x234[2] = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 30;
            fn_801EEC98(mSerial, &outAngle, NULL, NULL, NULL, 0, 0);
            f32 a = (f32)outAngle;
            s32 idx = 0;
            for (u8 i = 0; i < 12; i++) {
                a -= 4000.0f;
                if (a < 0.0f) {
                    break;
                }
                idx++;
            }
            field_0x25c = idx;
        }
        if (field_0x25c != field_0x98 && (field_0x234[2] & 3) == 0) {
            field_0x98 = (field_0x98 + field_0x254) & 0xF;
        }
        s16 angle = (s32)(4000.0f * (f32)field_0x98);
        field_0x27c = field_0x278 * JMath::sincosTable_.sinShort(angle);
        field_0x280 = field_0x278 * JMath::sincosTable_.cosShort((s32)(4000.0f * (f32)field_0x98));
        field_0x288[4].set(mPos);
        angle = (s32)(4000.0f * (f32)((field_0x98 - 2) & 0xF));
        field_0x288[4].x += 6.0f * JMath::sincosTable_.sinShort(angle);
        field_0x288[4].y += 6.0f * JMath::sincosTable_.cosShort(angle);
        field_0x288[5].set(mPos);
        angle = (s32)(4000.0f * (f32)((field_0x98 + 2) & 0xF));
        field_0x288[5].x += 6.0f * JMath::sincosTable_.sinShort(angle);
        field_0x288[5].y += 6.0f * JMath::sincosTable_.cosShort(angle);
        field_0x270 += 500.0f;
    }
    if (field_0x234[0] == 0) {
        field_0x234[0] = 2;
        field_0x274 += 0.5f;
    }
}

// 0x8018FAB4.
// FIXME(shotgun): 67.61 %, missing/extra
void cls_804A1FA0::vfunc_0x44() {
    if (mEmitters[0] != NULL) {
        mEmitters[0]->becomeInvalidEmitter();
        mEmitters[0] = NULL;
    }
    if (mEmitters[1] != NULL) {
        mEmitters[1]->becomeInvalidEmitter();
        mEmitters[1] = NULL;
    }
    if (mEmitters[2] != NULL) {
        mEmitters[2]->becomeInvalidEmitter();
        mEmitters[2] = NULL;
    }
}
