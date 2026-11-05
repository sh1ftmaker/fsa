#include "game/GameAudio.h"
#include <float.h>
#include <math.h>
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_rope.h"

// The four animations' keys: cel 2n for 10 frames, then 2n + 1 for 10, then the terminator.
static const s16 lbl_80469138[6] = {0, 10, 1, 10, -1, -1};
static const s16 lbl_80469144[6] = {2, 10, 3, 10, -1, -1};
static const s16 lbl_80469150[6] = {4, 10, 5, 10, -1, -1};
static const s16 lbl_8046915C[6] = {6, 10, 7, 10, -1, -1};
// The table of animations: one per direction (0 +x, 1 -x, 2 +y, 3 -y).
static const s16* lbl_804A3A58[4] = {lbl_80469138, lbl_80469144, lbl_80469150, lbl_8046915C};

Rope_804A3A68::~Rope_804A3A68() {}

void Rope_804A3A68::draw() {
    drawSprite_801F06F0("enemy_gc_sob.bin", mAnim.mCel + 0xB5, NULL, 0, 1.0f, 1.0f);
}

// FIXME: 98.8 %. Regalloc only: the original loads mSpeed into f2 and the
// 0.9090909f constant into f1 (ours the reverse), and the sdata2 literals 0.4f and
// 0.9090909f come out in the other order.
void Rope_804A3A68::vfunc_0x94() {
    mAnim.fn_801D2264(field_0x98, 2);
    f32 t = (mSpeed - 0.4f) * 0.9090909f;
    if (t > 1.0f) {
        t = 1.0f;
    } else if (t < 0.0f) {
        t = 0.0f;
    }
    mAnim.mFrameCtrl.setRate(t * 2.0f + 1.0f);
    EnemyBase_804A4808::vfunc_0x94();
}

// Inlined state starts (the original inlines them at each call).
inline void Rope_804A3A68::walk() {
    field_0x250 = fn_801CD664(0x29) + 0x3c;
    mTargetSpeed = 0.4f;
    if (mState != 2) {
        field_0x98 = fn_801CF6B8();
    }
    mStateTimer = 0;
    mState = 1;
}

inline void Rope_804A3A68::idle() {
    field_0x250 = fn_801CD664(0x3d) + 0x28;
    mTargetSpeed = 0.0f;
    mStateTimer = 0;
    mState = 0;
}

// FIXME: 97.8 %. Only the integer registers of the target search differ: the
// original keeps found in r27, dir in r28 and i in r29 (the zeros copied with
// mr from r27, and found = dir = 1 as li/mr in the last branch); ours puts
// found in r29 and i in r27.
void Rope_804A3A68::vfunc_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
    case 1: {
        bool found = false;
        if (mStateTimer >= 30) {
            s32 dir = 0;
            f32 best = FLT_MAX;
            for (s32 i = 0; i < 4; i++) {
                if (fn_801D0B44(i)) {
                    f32 dx = __fabsf(mPos.x - getPlayerPos_8023E724(i)->x);
                    f32 dy = __fabsf(mPos.y - getPlayerPos_8023E724(i)->y);
                    if (dx <= 10.0f) {
                        f32 dist = fn_801D0DA0(i, &mPos);
                        if (100.0f < dist && dist < 14400.0f && dist < best) {
                            if (mPos.y < getPlayerPos_8023E724(i)->y) {
                                if (!fn_801CFD68(2, dy)) {
                                    best = dist;
                                    dir = 2;
                                    found = true;
                                }
                            } else {
                                if (!fn_801CFD68(3, dy)) {
                                    best = dist;
                                    dir = 3;
                                    found = true;
                                }
                            }
                        }
                    } else if (dy <= 10.0f) {
                        f32 dist = fn_801D0DA0(i, &mPos);
                        if (100.0f < dist && dist < 14400.0f && dist < best) {
                            if (mPos.x < getPlayerPos_8023E724(i)->x) {
                                if (!fn_801CFD68(0, dx)) {
                                    best = dist;
                                    dir = 0;
                                    found = true;
                                }
                            } else {
                                if (!fn_801CFD68(1, dx)) {
                                    best = dist;
                                    dir = 1;
                                    found = true;
                                }
                            }
                        }
                    }
                }
            }
            if (found) {
                field_0x250 = fn_801CD664(0x29) + 0x3c;
                mTargetSpeed = 1.5f;
                mStateTimer = 0;
                mState = 2;
                field_0x98 = dir;
            }
        }
        break;
    }
    }
    switch (mState) {
    case 0:
        if (mStateTimer >= field_0x250) {
            walk();
        }
        break;
    case 1:
    case 2:
        if (mapHit) {
            field_0x98 = fn_801CF720(field_0x98);
            field_0x250 += 10;
        } else if (mStateTimer >= field_0x250) {
            if (mState == 1) {
                if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.35f) {
                    idle();
                } else {
                    walk();
                }
            } else {
                walk();
            }
        }
        break;
    case 3:
        if (mStateTimer > 1) {
            if (mPos.z <= 0.0f) {
                idle();
            }
        }
        break;
    }
}

void Rope_804A3A68::init() {
    EnemyBase_804A4808::init();
    if (mPos.z > 0.001f) {
        mState = 3;
        field_0x250 = -1;
    } else {
        mState = 0;
        field_0x250 = fn_801CD664(0x3d) + 0x28;
    }
    mStateTimer = 0;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A3A58, 4);
    mAnim.startAnim_801D22DC(field_0x98, 2, false);
    mAnim.fn_801D1D60();
}

Rope_804A3A68::Rope_804A3A68() {}
