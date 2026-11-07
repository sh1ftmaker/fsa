#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "JSystem/JMath/JMath.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/act/tu_80166BD0.h"

// Two tables in .data, read by update() (not written yet).
s32 lbl_804A0638[6] = {0, 1, 2, 3, 2, 1};
s32 lbl_804A0650[6] = {12, 8, 8, 12, 8, 8};

MojamojaUnidentifiedEnemy_804A0668::~MojamojaUnidentifiedEnemy_804A0668() {
    releaseEmitter_801EE434(mEmitter26c);
}

void MojamojaUnidentifiedEnemy_804A0668::vfunc_0x44() {
    if (mEmitter26c != NULL) {
        mEmitter26c->becomeInvalidEmitter();
        mEmitter26c = NULL;
    }
}

// FIXME: 99.6 %. Only the .sdata2 literals: not split (lbl_8053D838.. vs @NNN).
void MojamojaUnidentifiedEnemy_804A0668::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    mLife = 4;
    field_0x250 = 0;
    field_0x25c = 0.0f;
    field_0x258 = 1.0f;
    field_0x260 = 0.0f;
    field_0x264 = 0.0f;
    field_0x268 = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x254 = 255.0f;
    mEmitter26c = NULL;
    field_0xb0 = 0x11101;
    field_0x230 = 0;
}

// FIXME(shotgun): 94.59 %, missing/extra + reloc-name
void MojamojaUnidentifiedEnemy_804A0668::update() {
    if (mEmitter26c != NULL) {
        addCalc2_801F71A4(&field_0x254, 1.0f, 10.0f, 0.0f);
        mEmitter26c->setGlobalAlpha((s32)field_0x254);
    }
    if (fn_801F3D94(1) != 0 || fn_801F305C()) {
        if (mEmitter26c != NULL) {
            mEmitter26c->becomeInvalidEmitter();
            mEmitter26c = NULL;
        }
        fn_801F2618(NULL, false);
        return;
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
    addCalc2_801F71A4(&field_0x3c.x, field_0x260, 0.5f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x264, 0.5f, 1.0f);
    mPos.x += field_0x3c.x;
    mPos.y += field_0x3c.y;
    mPos.z += field_0x3c.z;
    bool hitWall = false;
    if (field_0x230 != 3) {
        if (fn_8022461C(&mPos, field_0x4) == 0xE) {
            startSound_801F0E34(0x4001A, 0);
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x260 = 0.0f;
            field_0x264 = 0.0f;
            field_0x268 = 0.0f;
            field_0x230 = 3;
        } else {
            fn_801F2B7C(-100, -1, 1);
            hitWall = fn_801F2618(NULL, false);
        }
    }
    switch (field_0x230) {
    case 0:
        if (mEmitter26c != NULL) {
            mEmitter26c->becomeInvalidEmitter();
            mEmitter26c = NULL;
        }
        field_0x244 = (s32)(5.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        field_0x248 = 0;
        field_0x238 = (s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x14;
        field_0x23c = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x64;
        field_0x230 = 1;
        // fall through
    case 1:
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            if (field_0x238 == 0) {
                field_0x260 = JMath::sincosTable_.sinShort((s16)(s32)(65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
                field_0x264 = JMath::sincosTable_.cosShort((s16)(s32)(65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
                field_0x3c.z = 5.0f;
            } else {
                if (field_0x260 != 0.0f || field_0x264 != 0.0f) {
                    startSound_801F0E34(0x40018, 0);
                }
                field_0x260 = 0.0f;
                field_0x264 = 0.0f;
                field_0x268 = 0.0f;
            }
            if (field_0x23c == 0) {
                JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
                pos.y -= 8.0f;
                mEmitter26c = spawnEmitter_8013CC50(field_0x4, &pos, 0x170, 0, 1, NULL, NULL, true, 0.3f);
                field_0x254 = 255.0f;
                startSound_801F0E34(0x40019, 0);
                field_0x244 = 0;
                field_0x248 = (s32)(2.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 1;
                field_0x260 = 0.0f;
                field_0x264 = 0.0f;
                field_0x268 = 0.0f;
                field_0x230 = 2;
                break;
            }
        } else {
            field_0x3c.z -= 1.0f;
            field_0x238 = 0x14;
        }
        if (hitWall) {
            field_0x260 *= -1.0f;
            field_0x264 *= -1.0f;
        }
        if (field_0x234 == 0) {
            field_0x234 = lbl_804A0650[field_0x244];
            field_0x250 = lbl_804A0638[field_0x244];
            field_0x244++;
            if (field_0x244 > 5) {
                field_0x244 = 0;
            }
        }
        break;
    case 2:
        if (mPos.z <= 0.0f) {
            field_0x3c.z = 0.0f;
        } else {
            field_0x3c.z -= 1.0f;
        }
        if (field_0x234 == 0) {
            field_0x234 = 4;
            field_0x250 = lbl_804A0638[field_0x244];
            field_0x244++;
            if (field_0x244 > 5) {
                field_0x244 = 0;
                field_0x248--;
                if (field_0x248 <= 0) {
                    field_0x230 = 0;
                }
            }
        }
        break;
    case 3:
        field_0x25c += 0.1f;
        field_0x258 -= 0.02f;
        if (field_0x258 < 0.1f) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, -1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x5005F, 0);
            fn_801F3668();
        }
        break;
    }
}

// FIXME: 99.0 %. Register allocation only: the scale (0x258) sits in f31 and
// the first sin result in f26 where the original has f29 and f28.
void MojamojaUnidentifiedEnemy_804A0668::draw() {
    if (field_0x230 == 3) {
        SpriteParams_801F5AE0 params;
        Mtx mtx;
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        JGeometry::TVec3f rot(0.0f, field_0x25c, 0.0f);
        setRotScaleMtx(mtx, rot, field_0x258);
        params.mMtx = mtx;
        s32 frame = field_0x250 + 0x9d;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
    } else {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x250 + 0x9d, NULL, 0, 1.0f, 1.0f);
    }
}
