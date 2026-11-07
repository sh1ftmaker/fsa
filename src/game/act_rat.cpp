// Actor RAT ("Rat" in docs/names/actors.tsv): the spawner (spawnActor_801F9484)
// allocates 0x248 bytes for it (0x801FBE30) and calls its constructor
// fn_80202EEC, which stores the vtable 0x8049C400.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/act_rat.h"

// The step of the walk cycle, per direction frame (x, y).
static const f32 lbl_8049C3E0[4][2] = {{2.0f, 0.0f}, {-2.0f, 0.0f}, {0.0f, 2.0f}, {0.0f, -2.0f}};

// Frame offsets per animation step.
static u8 lbl_80539E68[] = {0, 3, 1, 2};

// FIXME(shotgun): 100 % (written in the shotgun pass)
void Rat_8049C400::init() {
    mBox.i.set(-8.0f, -8.0f);
    mBox.f.set(8.0f, 8.0f);
    field_0x80.i.set(-10.0f, -10.0f);
    field_0x80.f.set(10.0f, 10.0f);
    field_0xb0 = 0x11101;
    mFrameBase = 0;
    mStep = 0;
    field_0x230 = 1;
    field_0x238 = 0;
    field_0x23c = 0;
    mDir = 0;
    field_0x244 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x32;
    field_0x90 &= 1;
}

// Wanders in the four directions (states 0, 1, 3 and the chase 2: a player nearer than 120 turns it his way).
// FIXME(shotgun): 98.26 %, different + reloc-name (one instruction-order change around the chase flag)

void Rat_8049C400::update() {
    if (fn_801F3D94(true) || fn_801F305C()) {
        fn_801F2618(NULL, false);
        return;
    }
    field_0x238++;
    if (field_0x238 == 4) {
        mFrameBase++;
        if (mFrameBase == 2) {
            mFrameBase = 0;
        }
        field_0x238 = 0;
    }
    bool bumped;
    if (field_0x90 != 0) {
        bumped = fn_801F2B7C(field_0x114, -1, 4) != 0;
    } else {
        bumped = fn_801F2B7C(field_0x114, -1, 1) != 0;
    }
    u8 hit = fn_801F2618(NULL, false);
    if (hit) {
        if (ActorBase_804A5770::vfunc_0x4C(field_0x4, &mPos) == 1) {
            vfunc_0x24();
            return;
        }
    }
    switch (field_0x230) {
    case 0:
    case 3: {
        mDir = 0;
        if (hit) {
            mStep++;
        }
        mStep &= 3;
        bool chasing = false;
        if (field_0x90 != 0 && field_0x230 == 0) {
            u16 angle = 0;
            f32 dist = 0.0f;
            s32 dir = 0;
            s32 player = 0;
            fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
            if (player != -1 && dist < 120.0f) {
                field_0x230 = 2;
                field_0x23c = 0;
                for (u8 i = 0; i < 4; i++) {
                    if (dir == lbl_80539E68[i]) {
                        mStep = i;
                        break;
                    }
                }
                chasing = true;
            }
        }
        if (chasing) {
            return;
        }
        mPos.x += lbl_8049C3E0[lbl_80539E68[mStep]][0];
        mPos.y += lbl_8049C3E0[lbl_80539E68[mStep]][1];
        field_0x23c++;
        if (field_0x23c == field_0x244) {
            field_0x244 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x32;
            field_0x230 = 1;
            field_0x23c = 0;
        }
        break;
    }
    case 1:
        mDir = 1;
        field_0x23c++;
        if (field_0x23c == field_0x244) {
            field_0x244 = (s32)(80.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
            mStep = (u8)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            field_0x230 = 0;
            startSound_801F0E34(0x400A6, 0);
            field_0x23c = 0;
        }
        break;
    case 2:
        mStep &= 3;
        mPos.x += lbl_8049C3E0[lbl_80539E68[mStep]][0];
        mPos.y += lbl_8049C3E0[lbl_80539E68[mStep]][1];
        if (!(field_0x23c & 0x1111)) {
            u16 angle = 0;
            f32 dist = 0.0f;
            s32 dir = 0;
            s32 player = 0;
            fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
            if (player != -1 && dist < 120.0f) {
                for (u8 i = 0; i < 4; i++) {
                    if (dir == lbl_80539E68[i]) {
                        mStep = i;
                        break;
                    }
                }
            }
        }
        if (bumped == 1) {
            field_0x244 = 100;
            field_0x230 = 3;
            field_0x23c = 0;
        } else {
            field_0x23c++;
            if (field_0x23c == 100 || hit) {
                field_0x244 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x32;
                field_0x230 = 1;
                field_0x23c = 0;
            }
        }
        break;
    }
}

// Drawn 1.2 times larger when the spawn parameter (0x90) is nonzero.
void Rat_8049C400::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x90 != 0) {
        mtx[0][0] = 1.2f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.2f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.2f;
        params.mMtx = mtx;
    }
    // The direction's block, then the walk cycle's frame in it.
    drawSprite_801F06F0("enemy_gc_sob.bin", mFrameBase + mDir * 2 + (lbl_80539E68[mStep] * 4 + 0xA1),
                &params, 0, 1.0f, 1.0f);
}

Rat_8049C400::~Rat_8049C400() {}
