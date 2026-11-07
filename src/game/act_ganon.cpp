#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/act/act_ganon.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

// .bss tables indexed by player (the unwritten update / init use them; the __sinit constructs
// the two vector tables).
s32 lbl_804C1BC0[4];
JGeometry::TVec3f lbl_804C1BD0[4];
JGeometry::TVec3f lbl_804C1C00[4];

// Animation tables (.data 0x804A1FF8, not written as source here): per animation, a first
// frame, and for each step the frame offset (0xFF ends the animation) and its length, for the
// body (field_0x25c) and the second layer (field_0x260).
extern const s16 lbl_804A1FF8[16];
extern const u8* const lbl_804A2118[16];
extern const u8* const lbl_804A2158[16];
extern const u8* const lbl_804A2298[16];
extern const u8* const lbl_804A22D8[16];
extern u8 lbl_8053AB90;
extern u8 lbl_8053AB91;
extern s32 lbl_80539FE8;

void fn_80196E00() {
    lbl_8053AED0->field_0xdc |= 0x10;
}

u8 fn_80196DF0() {
    return lbl_80529DEC.field_0x24f;
}

void fn_80196D8C(s32 index, bool flag, f32 a, f32 b) {
    getGameView_802D6A9C()->fn_802D79E8(index, a, b, flag);
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_80196D70(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_80196D60(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

void fn_80196D30(s32 a, s32 b) {
    gGameRoot_8053AAF8->fn_80134DE4(a, b);
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_80196D24(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// Sets the emitter's particle scale (0xB0, 0xB4).
void fn_80196D18(JPABaseEmitter* emitter, f32 x, f32 y) {
    emitter->mGlobalPScl.set(x, y);
}

// Out-of-line copy of JPABaseEmitter::setGlobalScale(const TVec3f&).
void fn_80196CF4(JPABaseEmitter* emitter, const JGeometry::TVec3f* scale) {
    emitter->setGlobalScale(*scale);
}

// FIXME(shotgun): 88.03 %, different + reloc-name.
void Ganon_804A25F0::init() {
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x274 = 0;
    field_0x278 = 0;
    field_0x27c = 0;
    field_0x280 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x318[i] = -1;
    }
    field_0x94 = 0;
    field_0x196 = 1;
    for (s32 i = 0; i < 4; i++) {
        field_0x308[i] = -1;
    }
    field_0x284 = 0;
    field_0x288 = 0;
    field_0x2b4 = 0;
    field_0x29c = 0;
    field_0x28c = 0;
    field_0x290 = 0;
    field_0x2b8.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x298 = 0x1E0;
    field_0x2a0 = 0;
    field_0x2a8 = 0.0f;
    field_0x294 = -1;
    field_0x2f8 = -1;
    field_0x2ac = 20.0f;
    field_0x2b0 = 20.0f;
    field_0x2a5 = 0;
    field_0x2b6 = (s16)(65536.0f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
    field_0x2dc.set(mPos);
    field_0x2e8.set(mPos);
    field_0x54.set(1.0f, 1.0f, 1.0f);
    field_0x2d0.set(1.0f, 1.0f, 1.0f);
    field_0x2f4 = 0.0f;
    field_0x300 = 0;
    field_0x2fc = -1;
    field_0x304 = 0;
    for (s32 i = 0; i < 5; i++) {
        mEmitters[i] = NULL;
    }
    field_0x2a4 = 0;
    field_0x2a6 = 1;
    field_0x118 = 2;
    if (field_0x90 == 0) {
        field_0xb0 = 0xA040004;
        lbl_8053AB91 = 0;
        lbl_80539FE8 = field_0x4;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                lbl_804C1C00[i].set(*getPlayerPos_8023E724(i));
            }
        }
        lbl_8053AB90 = 0;
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x80.i.x = -16.0f;
        field_0x80.i.y = -16.0f;
        field_0x80.f.x = 16.0f;
        field_0x80.f.y = 16.0f;
        mState = 0;
    }
    if (field_0x90 == 10 || (field_0x90 - 11) <= 1u || field_0x90 == 13) {
        mBox.i.x = -24.0f;
        mBox.i.y = -24.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 24.0f;
        field_0x80.i.x = -24.0f;
        field_0x80.i.y = -24.0f;
        field_0x80.f.x = 24.0f;
        field_0x80.f.y = 24.0f;
        mState = 0x1F;
    }
    if (field_0x90 == 20 || (field_0x90 - 21) <= 1u || field_0x90 == 23) {
        mBox.i.x = -32.0f;
        mBox.i.y = -32.0f;
        mBox.f.x = 32.0f;
        mBox.f.y = 32.0f;
        field_0x80.i.x = -32.0f;
        field_0x80.i.y = -32.0f;
        field_0x80.f.x = 32.0f;
        field_0x80.f.y = 32.0f;
        field_0x114 = 2;
        mState = 0x23;
    }
    if (field_0x90 == 1 || (field_0x90 - 2) <= 1u || field_0x90 == 4) {
        field_0x25c = field_0x90 - 1;
        lbl_804C1BC0[field_0x25c] = field_0x4;
        lbl_804C1BD0[field_0x25c].set(mPos);
        lbl_804C1C00[field_0x25c].y += 16.0f;
        mState = 0x28;
    }
    if (field_0x90 == 31) {
        field_0xb0 = 0x5000000;
        mState = 0x2C;
    }
    if (field_0x90 == 30) {
        field_0xb0 = 0xA040004;
        field_0x114 = 4;
        mBox.i.x = -48.0f;
        mBox.i.y = -48.0f;
        mBox.f.x = 48.0f;
        mBox.f.y = 48.0f;
        mState = 0x2A;
    }
}

Ganon_804A25F0::~Ganon_804A25F0() {
    for (s32 i = 0; i < 5; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// Steps the body animation (field_0x25c / field_0x234) and the second layer (field_0x260 /
// field_0x238) through the frame tables.
// FIXME(shotgun): 89.95 %, missing/extra.
void Ganon_804A25F0::fn_801964AC() {
    if (field_0x28c != field_0x290) {
        field_0x290 = field_0x28c;
        field_0x2a4 = 0;
        field_0x234 = 0;
        field_0x238 = 0;
        field_0x25c = 0;
        field_0x260 = 0;
    }
    if (field_0x234 == 0) {
        field_0x284 = lbl_804A2118[field_0x28c][field_0x25c];
        field_0x284 += lbl_804A1FF8[field_0x28c];
        field_0x234 = lbl_804A2158[field_0x28c][field_0x25c];
        field_0x25c++;
        if (field_0x28c == 8) {
            if (field_0x264 == 0) {
                if (field_0x25c > 2) {
                    field_0x25c = 0;
                }
            } else {
                field_0x2a5 = 0;
            }
        }
        if (lbl_804A2118[field_0x28c][field_0x25c] == 0xFF) {
            s32 anim = field_0x28c;
            if ((u32)anim <= 1 || anim == 5 || anim == 3 || anim == 10 || anim == 12) {
                field_0x25c = 0;
                if (field_0x28c == 10) {
                    if (field_0x23c != 0 || mState != 0xB) {
                        startSound_801F0E34(0x50006, 0);
                    }
                }
            } else if (anim == 6) {
                field_0x25c = 4;
            } else {
                field_0x25c--;
            }
            field_0x2a4 = 1;
            if (field_0x28c == 2) {
                field_0x28c = 3;
                field_0x25c = 0;
            }
            if (field_0x28c == 13 || field_0x28c == 14) {
                field_0x2a4 = 0;
                field_0x25c = 0;
                field_0x28c = 9;
            }
            if (field_0x28c == 9) {
                field_0x28c = 10;
                startSound_801F0E34(0x50006, 0);
                field_0x25c = 0;
            }
        }
    }
    if (field_0x238 == 0) {
        field_0x288 = lbl_804A2298[field_0x28c][field_0x260];
        field_0x288 += lbl_804A1FF8[field_0x28c];
        field_0x238 = lbl_804A22D8[field_0x28c][field_0x260];
        field_0x260++;
        if (field_0x28c == 8) {
            if (field_0x264 == 0) {
                if (field_0x260 > 2) {
                    field_0x260 = 0;
                }
            }
        }
        if (lbl_804A2298[field_0x28c][field_0x260] == 0xFF) {
            s32 anim = field_0x28c;
            if ((u32)anim <= 1 || anim == 5 || anim == 3 || anim == 10 || anim == 12) {
                field_0x260 = 0;
            } else if (anim == 6) {
                field_0x260 = 4;
            } else {
                field_0x260--;
            }
            if (field_0x28c == 2) {
                field_0x28c = 3;
                field_0x260 = 0;
            }
            if (field_0x28c == 13 || field_0x28c == 14) {
                field_0x260 = 0;
            }
            if (field_0x28c == 9) {
                field_0x28c = 10;
                field_0x260 = 0;
            }
        }
    }
}

// Picks the player (of those on this layer) with the most hearts (mode 0) or most of the other
// stat (mode 1), the nearest on ties by a coin flip; stores its distance, angle and index.
// FIXME(shotgun): 92.69 %, missing/extra + reloc-name.
void Ganon_804A25F0::fn_80196208(s32 mode) {
    field_0x2fc = -1;
    s32 best = -1;
    for (s32 i = 0; i < 4; i++) {
        if (!fn_8023F264(i)) {
            continue;
        }
        if (field_0x4 != getPlayerLayer_8023DE58(i)) {
            continue;
        }
        if (fn_80230188(i)) {
            continue;
        }
        if (fn_8022DCD8(i)) {
            continue;
        }
        if (!((mode == 0 && fn_80237774(i) >= best) || (mode == 1 && fn_80236228(i) >= best))) {
            continue;
        }
        if ((mode == 0 && best == fn_80237774(i)) || (mode == 1 && best == fn_80236228(i))) {
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f < 0.5f) {
                continue;
            }
        }
        if (mode == 0) {
            best = fn_80237774(i);
        }
        if (mode == 1) {
            best = fn_80236228(i);
        }
        f32 dx = getPlayerPos_8023E724(i)->x - mPos.x;
        f32 dy = getPlayerPos_8023E724(i)->y - mPos.y;
        field_0x2f4 = std::sqrtf(dx * dx + dy * dy);
        field_0x304 = (s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f);
        field_0x2fc = i;
    }
}

// FIXME(shotgun): 99.88 %, reloc-name.
void Ganon_804A25F0::fn_80196020(s32 player) {
    if (player < 0) {
        return;
    }
    if (field_0x268 < 0) {
        for (s32 i = 0; i < 5; i++) {
            if (mEmitters[i] != NULL) {
                mEmitters[i]->becomeInvalidEmitter();
                mEmitters[i] = NULL;
            }
        }
        mEmitters[0] = spawnEmitter_8013CC50(field_0x4, getPlayerPos_8023E724(player), 0x2A6, 0, 0, NULL, NULL, true, 1.0f);
        mEmitters[1] = spawnEmitter_8013CC50(field_0x4, getPlayerPos_8023E724(player), 0x2A7, 0, 0, NULL, NULL, true, 1.0f);
    }
    if (fn_8022F3DC(player) >= 0) {
        player = fn_8022F3DC(player);
    }
    fn_8022CA6C(player, 5, false);
    if (fn_802317F0(player, lbl_804C1BC0[player], getPlayerPos_8023E724(player), &lbl_804C1BD0[player], 6)) {
        field_0x2a8 = 1.0f;
        field_0x234 = 100;
        mState = 0x22;
    } else {
        mState = 0x21;
    }
}

// FIXME(shotgun): 89.51 %, missing/extra + reloc-name.
void Ganon_804A25F0::fn_80192BD8() {
    switch (mState) {
    case 0x2C:
        field_0x234 = 50;
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x359, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x35A, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x35B, 0, 0, NULL, NULL, true, 1.0f);
        mState = 0x2D;
        // fall through
    case 0x2D:
        if (field_0x234 == 0) {
            mAlive = false;
        }
        break;
    }
    fn_801F2B7C(-100, -1, true);
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
    if (field_0x24c > 0) {
        field_0x24c--;
    }
    if (field_0x250 > 0) {
        field_0x250--;
    }
}

void Ganon_804A25F0::vfunc_0x24() {
    startSound_801F0E34(0x40006, 0);
    vfunc_0x28(2, 0.0f, 0.0f);
}

// FIXME(shotgun): draw (0x80192090, 0xB48 bytes) and update (0x80192D98, 0x3288 bytes) are not written.
