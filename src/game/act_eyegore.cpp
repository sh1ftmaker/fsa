#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "game/act/act_eyegore.h"

Eyegore_804A3548::~Eyegore_804A3548() {}

// TIMG names indexed by field_0x250. Not const: the table sits in .data.
static const char* lbl_804A34F8[] = {
    "e_igol_yoko_0.bti", "e_igol_yoko_1.bti", "e_igol_yoko_2.bti", "e_igol_yoko_3.bti",
    "e_igol_yoko_0.bti", "e_igol_yoko_1.bti", "e_igol_yoko_2.bti", "e_igol_yoko_3.bti",
    "e_igol_mae_0.bti",  "e_igol_mae_1.bti",  "e_igol_mae_2.bti",  "e_igol_mae_3.bti",
    "e_igol_ushiro_0.bti", "e_igol_ushiro_1.bti", "e_igol_ushiro_2.bti", "e_igol_ushiro_3.bti",
    "e_igol_yoko_toji.bti", "e_igol_yoko_toji.bti", "e_igol_mae_toji.bti", "e_igol_ushiro_0.bti",
};

void Eyegore_804A3548::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    f32 flip = 1.0f;
    if (field_0x98 == 0) {
        flip = -1.0f;
    }
    mtx[0][0] = flip;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.0f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.0f;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.mMtx = mtx;
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804A34F8[field_0x250]);
    fn_801EFD34(res, &params, 3, 0.0f, -9.0f, 0.0f);
}

// FIXME: 99.71 %. The .sdata2 literals (lbl_8053E898 and the others after it)
// are @NNN here: the unit's .sdata2 split doesn't cover them. In the else
// branch of case 5 the original has the zero in r4 and the mRandom pointer in
// r5; ours the other way round (regalloc-only, as in vfunc_0x44).
void Eyegore_804A3548::update() {
    s32 dir = -1;
    f32 dist;

    if (fn_801F3D94(1) != 0) {
        fn_801F2618(NULL, false);
        return;
    }
    if (mLife < 0x3E8) {
        vfunc_0x24();
        field_0xb0 = 0;
        return;
    }

    fn_801EF510();
    addCalc2_801F71A4(&field_0x3c.x, field_0x25c.x, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x25c.y, 0.3f, 1.0f);
    mPos += field_0x3c;

    for (s32 i = 0; i < 4; i++) {
        s32 player = fn_801F285C(NULL, NULL, false, i);
        if (player < 0) {
            continue;
        }
        JGeometry::TBox2f box(mBox);
        JGeometry::TVec2f pos(mPos.x, mPos.y);
        JGeometry::TVec2f center;
        box.addPos(pos);
        center.x = box.f.x + 0.5f * (box.i.x - box.f.x);
        center.y = box.f.y + 0.5f * (box.i.y - box.f.y);
        if (fn_80237948(player, &center)) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x400D1, 0);
        }
    }

    fn_801F2B7C(-100, -1, 1);
    fn_801EEC98(mSerial, NULL, &dir, &dist, NULL, 0, 0);
    bool hit = fn_801F2618(NULL, false);

    switch (field_0x230) {
    case 0:
        field_0x25c.z = 0.0f;
        field_0x25c.y = 0.0f;
        field_0x25c.x = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0xb0 = 0x0A040004;
        field_0x98 = 2;
        field_0x250 = field_0x98 * 4;
        mTimers[0] = 12;
        field_0xbc = 4;
        field_0x230 = 1;
        // fall through
    case 1:
        if (mTimers[0] == 0) {
            field_0x250 = field_0x98 + 0x10;
        }
        if (mTimers[2] != 0) {
            break;
        }
        if (dir < 0 || dist > 64.0f) {
            break;
        }
        field_0x244 = 0;
        field_0x248 = 0;
        mTimers[0] = 0;
        field_0x230 = 2;
        // fall through
    case 2:
        if (mTimers[0] != 0) {
            break;
        }
        if (field_0x244 == 0) {
            field_0x250 = field_0x98 * 4;
        } else {
            field_0x250 = field_0x98 + 0x10;
            field_0x248++;
            if (field_0x248 > 2) {
                field_0x250 = field_0x98 * 4;
                field_0x230 = 3;
                break;
            }
        }
        mTimers[0] = 12;
        field_0x244++;
        field_0x244 &= 1;
        break;
    case 3:
        field_0x24c++;
        if (field_0x24c > 30) {
            field_0x244 = 0;
            mTimers[0] = 0;
            field_0x24c = 0;
            mTimers[2] = (s32)(140.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 140;
            field_0x230 = 4;
        }
        break;
    case 4:
        if (mTimers[3] == 0 && dir >= 0) {
            if (field_0x98 != dir) {
                mTimers[0] = 0;
                field_0x244 = 0;
            }
            field_0x98 = dir;
        }
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        switch (field_0x98) {
        case 0:
            field_0x25c.x = 1.4f;
            field_0x25c.y = 0.0f;
            field_0x25c.z = 0.0f;
            break;
        case 1:
            field_0x25c.x = -1.4f;
            field_0x25c.y = 0.0f;
            field_0x25c.z = 0.0f;
            break;
        case 2:
            field_0x25c.x = 0.0f;
            field_0x25c.y = 1.4f;
            field_0x25c.z = 0.0f;
            break;
        case 3:
            field_0x25c.x = 0.0f;
            field_0x25c.y = -1.4f;
            field_0x25c.z = 0.0f;
            break;
        }
        field_0xb0 = 0x0A040010;
        field_0xbc = 3;
        mTimers[1] = (s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 20;
        field_0x230 = 5;
        // fall through
    case 5:
        if (hit) {
            field_0x98 ^= 2;
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                field_0x98 ^= 1;
            }
            f32 savedX = mPos.x;
            f32 savedY = mPos.y;
            f32 savedZ = mPos.z;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            switch (field_0x98) {
            case 0:
                field_0x25c.x = 16.0f;
                field_0x25c.y = 0.0f;
                field_0x25c.z = 0.0f;
                break;
            case 1:
                field_0x25c.x = -16.0f;
                field_0x25c.y = 0.0f;
                field_0x25c.z = 0.0f;
                break;
            case 2:
                field_0x25c.x = 0.0f;
                field_0x25c.y = 16.0f;
                field_0x25c.z = 0.0f;
                break;
            case 3:
                field_0x25c.x = 0.0f;
                field_0x25c.y = -16.0f;
                field_0x25c.z = 0.0f;
                break;
            }
            mPos += field_0x25c;
            if (fn_801F2618(NULL, false)) {
                field_0x98 ^= 1;
            }
            mPos.x = savedX;
            mPos.y = savedY;
            mPos.z = savedZ;
            field_0x25c.z = 0.0f;
            field_0x25c.y = 0.0f;
            field_0x25c.x = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            mTimers[3] = (s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 20;
            mTimers[0] = 0;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            switch (field_0x98) {
            case 0:
                field_0x25c.x = 1.4f;
                field_0x25c.y = 0.0f;
                field_0x25c.z = 0.0f;
                break;
            case 1:
                field_0x25c.x = -1.4f;
                field_0x25c.y = 0.0f;
                field_0x25c.z = 0.0f;
                break;
            case 2:
                field_0x25c.x = 0.0f;
                field_0x25c.y = 1.4f;
                field_0x25c.z = 0.0f;
                break;
            case 3:
                field_0x25c.x = 0.0f;
                field_0x25c.y = -1.4f;
                field_0x25c.z = 0.0f;
                break;
            }
        } else {
            if (mTimers[2] == 0) {
                mTimers[0] = 0;
                field_0x244 = 0;
                field_0x250 = field_0x98 + 0x10;
                mTimers[2] = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 60;
                field_0x230 = 0;
            } else {
                if (mTimers[0] == 0) {
                    field_0x250 = field_0x98 * 4 + field_0x244;
                    mTimers[0] = 6;
                    field_0x244++;
                    field_0x244 &= 3;
                    if (field_0x244 & 1) {
                        startSound_801F0E34(0x5007C, 0);
                    }
                }
                if (mTimers[1] == 0) {
                    field_0x230 = 4;
                }
            }
            break;
        }
        break;
    }

    for (s32 i = 0; i < 4; i++) {
        if (mTimers[i] > 0) {
            mTimers[i]--;
        }
    }
}

// Runs only when mLife has changed since the last call.
// FIXME: 99.04 %. regalloc-only: the zero for mTimers[0] and field_0x244 is
// in r6 and the gGameRoot_8053AAF8 pointer in r5 here, the other way round in the
// original; the 1.0f and 60.0f literals are @NNN, not lbl_8053E888 /
// lbl_8053E8BC (the unit's .sdata2 is not split yet).
void Eyegore_804A3548::vfunc_0x44() {
    if (field_0x254 != mLife) {
        field_0x254 = mLife;
        if (field_0x258 > 0) {
            field_0x258--;
        }
        if (field_0x258 <= 0) {
            field_0xb0 = 0;
        }
        mTimers[0] = 0;
        field_0x244 = 0;
        field_0x250 = field_0x98 + 0x10;
        mTimers[2] = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x3C;
        field_0x230 = 0;
    }
}

// FIXME: 99.12 %. Only the .sdata2 literal names differ: lbl_8053E8C0,
// lbl_8053E8C4, ... are @NNN in ours (the unit's .sdata2 split doesn't cover them).
void Eyegore_804A3548::init() {
    mBox.i.x = -12.0f;
    mBox.i.y = -22.0f;
    mBox.f.x = 12.0f;
    mBox.f.y = 2.0f;
    field_0x80.i.x = -12.0f;
    field_0x80.i.y = -22.0f;
    field_0x80.f.x = 12.0f;
    field_0x80.f.y = 8.0f;
    mBox2.i.x = -16.0f;
    mBox2.i.y = -24.0f;
    mBox2.f.x = 16.0f;
    mBox2.f.y = 6.0f;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    mTimers[0] = 0;
    mTimers[1] = 0;
    mTimers[2] = 0;
    mTimers[3] = 0;
    field_0xb8 = 1;
    field_0x98 = 2;
    field_0x250 = 0x12;
    field_0xbc = 4;
    field_0x25c.z = 0.0f;
    field_0x25c.y = 0.0f;
    field_0x25c.x = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    mLife = 0x3EF;
    field_0x258 = 2;
    field_0x254 = mLife;
    field_0x230 = 0;
}
