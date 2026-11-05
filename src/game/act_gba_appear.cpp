#include "game/GameAudio.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/RotMatrix.h"
#include "game/act/act_gba_appear.h"
#include "JSystem/JUtility/JUTVideo.h"

// The images drawn for state 0 and state 1.
static const char* lbl_804B16C0[] = {
    "kk_gbaOut1P_01.bti", "kk_gbaOut2P_01.bti", "kk_gbaOut3P_01.bti", "kk_gbaOut4P_01.bti",
};
static const char* lbl_804B16D0[] = {
    "kk_gbaOut1P_02.bti", "kk_gbaOut2P_02.bti", "kk_gbaOut3P_02.bti", "kk_gbaOut4P_02.bti",
};

// The effect ids, one per player.
static const u16 lbl_80543C00[] = {0x15, 0x17, 0x14, 0x16};

// FIXME: 96.7%. The original also computes `addi r4, r27, 0x1c4` (the position argument) later,
// after the params stores, not before the camera/params setup as here. regalloc-only: the original keeps the scale in f29, cos x / cos y / cos z in
// f31 / f30 / f3 and sin x / y / z in f28 / f27 / f26; ours has the scale in f31 and the sines
// and cosines in other registers (the same shape as act_hardhat_beetle's setRotScaleMtx draw).
void GBAAppear_804B16E0::draw() {
    for (s32 i = 0; i < 4; i++) {
        u8 bit = 1 << i;
        bool active = (lbl_80529DEC.field_0x236 & bit) ? true : false; // the original materialises the bool
        if (active) {
            SpriteParams_801F5AE0 params;
            Mtx mtx;
            params.field_0x1d = 2;
            params.field_0x1c = 2;
            params.field_0x2d = 1;
            JGeometry::TVec3f rot(0.0f, 0.0f, 0.0f);
            setRotScaleMtx(mtx, rot, lbl_8050DB0C.field_0x30[0]);
            mtx[0][3] = 0.0f;
            mtx[1][3] = 0.0f;
            mtx[2][3] = 0.0f;
            params.mMtx = mtx;
            if (mState[i] == 0) {
                void* res = fn_804030C4('TIMG', lbl_804B16C0[i]);
                if (field_0x4 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&lbl_80529DEC.field_0x1c4[i], res,
                                                         &params);
                }
            } else if (mState[i] == 1) {
                void* res = fn_804030C4('TIMG', lbl_804B16D0[i]);
                if (field_0x4 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&lbl_80529DEC.field_0x1c4[i], res,
                                                         &params);
                }
            }
            lbl_80529DEC.field_0x236 &= ~bit;
        } else {
            mState[i] = 0xFF;
        }
    }
}

// FIXME: ~86%. The original's frame is 0x80: it makes two extra by-value TVec3f stack copies
// (slots 0x14 and 0x2C, psq_st of pos; its member-wise store into field_0x1c4[i] reads x/y from
// the first) and two more for the fn_8043E7CC argument (0x20 and 0x8); and the pos copy
// (psq_l / psq_st into the local before the call) is scheduled before the camera load
// (lwz gGameRoot2_8053AB10 -> 0x24) there, after the z load (lfs 0x1CC) here.
void GBAAppear_804B16E0::update() {
    JGeometry::TVec3f* player = getPlayerPos_8023E724(0);
    mPos.x = player->x;
    mPos.y = player->y;
    mPos.z = player->z;
    for (s32 i = 0; i < 4; i++) {
        u8 bit = 1 << i;
        if (lbl_80529DEC.field_0x236 & bit) {
            JGeometry::TVec3f pos = lbl_80529DEC.field_0x1c4[i];
            JGeometry::TVec3f screen;
            gGameRoot2_8053AB10->field_0x24->fn_8030C5F0(&pos, false, false, &screen);
            if (mState[i] == 0xFF) {
                if (screen.y > 0.5f * (s32)JUTGetVideoManager()->getEfbHeight()) {
                    mState[i] = 0;
                    pos.y += lbl_8050DB0C.field_0x30[1];
                } else {
                    mState[i] = 1;
                    pos.y -= lbl_8050DB0C.field_0x30[1];
                }
            } else {
                if (screen.y > (1.0f / 3.0f) * (s32)JUTGetVideoManager()->getEfbHeight()) {
                    mState[i] = 0;
                    pos.y += lbl_8050DB0C.field_0x30[1];
                } else if (screen.y < (2.0f / 3.0f) * (s32)JUTGetVideoManager()->getEfbHeight()) {
                    mState[i] = 1;
                    pos.y -= lbl_8050DB0C.field_0x30[1];
                }
            }
            lbl_80529DEC.field_0x1c4[i].x = pos.x;
            lbl_80529DEC.field_0x1c4[i].y = pos.y;
            lbl_80529DEC.field_0x1c4[i].z = pos.z;
            if (field_0x4 < 8) {
                gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&lbl_80529DEC.field_0x1c4[i]);
            }
            if (mEmitters[i] == NULL) {
                mEmitters[i] = spawnEmitter_8013CC50(field_0x4, &lbl_80529DEC.field_0x1c4[i],
                                           lbl_80543C00[i], 1, 1, NULL, NULL, true,
                                           lbl_8050DB0C.field_0x30[0]);
                mEmitters[i]->setStatus(0x40);
            } else {
                setEmitterPos_8013CB44(mEmitters[i], &lbl_80529DEC.field_0x1c4[i], 1, 1, true);
            }
            lbl_8053AED0->fn_8043E7CC(lbl_80529DEC.field_0x1c4[i]);
        } else {
            deleteEmitter(&mEmitters[i]);
        }
    }
}

void GBAAppear_804B16E0::init() {
    for (s32 i = 0; i < 4; i++) {
        mEmitters[i] = NULL;
        mState[i] = 0xFF;
    }
    field_0x11e = 1;
    field_0x11f = 1;
}

GBAAppear_804B16E0::~GBAAppear_804B16E0() {
    for (s32 i = 0; i < 4; i++) {
        deleteEmitter(&mEmitters[i]);
    }
}
