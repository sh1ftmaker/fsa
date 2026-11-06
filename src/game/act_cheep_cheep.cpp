#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_cheep_cheep.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

// FIXME: 98.19%. scheduling+regalloc: the 0x19660d constant and li 0 use swapped registers (r5/r6), and fcmpu cr0, f1, f0 loads 0.0f into f1 instead of f0.
void CheepCheep_804A11B8::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x258 = field_0x90 & 1;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x25c = 0;
    field_0x260 = 8.0f * (f32)((field_0x90 >> 8) & 0xFF);
    field_0x26c = 0.0f;
    field_0x268 = 0.0f;
    field_0x264 = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x270.x = mPos.x;
    field_0x270.y = mPos.y;
    field_0x270.z = mPos.z;
    field_0x98 = (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    if (field_0x258 == 1) {
        field_0x254 = (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        field_0x114 = 4;
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x80.i.x = -16.0f;
        field_0x80.i.y = -10.0f;
        field_0x80.f.x = 16.0f;
        field_0x80.f.y = 10.0f;
    }
    field_0x230 = 0;
    if ((field_0x90 >> 4) & 0xFF) {
        if (field_0x260 == 0.0f) {
            field_0x260 = 16.0f;
        }
        field_0x98 = (field_0x90 >> 4) & 0xF;
        field_0x98--;
        field_0x98 &= 3;
        field_0x230 = 4;
    }
}

bool CheepCheep_804A11B8::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 tile = fn_8022461C(pos, layer);
    if (tile != 0x3B && tile != 0 && tile != 2 && tile != 3) {
        return true;
    }
    return false;
}

// FIXME: 98.81%. different (2 opcodes): fcmpu cr0, f1, f0 loads 0.0f into f1 and hitVec.y into f0 (swapped vs. target), plus float register/stack slot swaps in case 1 and std::sqrtf and argument load scheduling in fn_801F6758.
void CheepCheep_804A11B8::update() {
    JGeometry::TVec3f hitVec;
    u8 r31 = 0;
    f32 outDist = 0.0f;
    s32 outPlayer = -1;
    addCalc2_801F71A4(&field_0x3c.x, field_0x264, 0.1f, 0.1f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x268, 0.1f, 0.1f);
    mPos += field_0x3c;
    hitVec.z = 0.0f;
    hitVec.y = 0.0f;
    hitVec.x = 0.0f;
    u8 hit = fn_801F2618(&hitVec, false);
    if (field_0x258 == 1 && hitVec.y != 0.0f) {
        hit = 0;
    }
    if (fn_801F2B7C(field_0x114, -1, 1)) {
        if (field_0x258 == 1 && field_0x230 == 3) {
            r31 = 1;
        }
    }
    switch (field_0x230) {
    case 0:
        if (field_0x258 == 1) {
            switch (field_0x98) {
            case 0:
                field_0x264 = 0.9f;
                field_0x268 = 0.0f;
                field_0x26c = 0.0f;
                break;
            case 1:
                field_0x264 = -0.9f;
                field_0x268 = 0.0f;
                field_0x26c = 0.0f;
                break;
            }
        } else {
            switch (field_0x98) {
            case 0:
                field_0x264 = 0.4f;
                field_0x268 = 0.0f;
                field_0x26c = 0.0f;
                break;
            case 1:
                field_0x264 = -0.4f;
                field_0x268 = 0.0f;
                field_0x26c = 0.0f;
                break;
            }
        }
        field_0x230 = 1;
        // fall through
    case 1: {
        field_0x268 = 0.3f * JMASinShort(field_0x25c);
        if (FABS(field_0x270.x - mPos.x) > 16.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 96.0f) {
            r31 = 1;
        }
        field_0x25c += 300;
        if (field_0x238 == 0 || hit) {
            if (hit || fn_801F6758(&mPos, field_0x4, 16.0f) || r31 == 1) {
                s32 oldDir = field_0x98;
                field_0x98 = 0;
                if (field_0x270.x < mPos.x) {
                    field_0x98 = 1;
                }
                if (oldDir != field_0x98) {
                    field_0x26c = 0.0f;
                    field_0x268 = 0.0f;
                    field_0x264 = 0.0f;
                    field_0x3c.z = 0.0f;
                    field_0x3c.y = 0.0f;
                    field_0x3c.x = 0.0f;
                }
                field_0x238 = 20;
                field_0x230 = 0;
            }
        }
        if (field_0x23c == 0) {
            s32 outDir;
            fn_801EEC98(mSerial, NULL, &outDir, &outDist, &outPlayer, 0, 0);
            if (outPlayer >= 0 && field_0x98 == outDir) {
                if (field_0x258 == 0) {
                    if (outDist < 96.0f) {
                        if (FABS(getPlayerPos_8023E724(outPlayer)->y - mPos.y) < 32.0f) {
                            field_0x230 = 2;
                        }
                    }
                } else {
                    if (outDist < 128.0f) {
                        if (FABS(getPlayerPos_8023E724(outPlayer)->y - mPos.y) < 32.0f) {
                            field_0x230 = 2;
                        }
                    }
                }
            }
        }
        break;
    }
    case 2:
        field_0x26c = 0.0f;
        field_0x268 = 0.0f;
        field_0x264 = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        if (field_0x258 == 1) {
            switch (field_0x98) {
            case 0:
                field_0x264 = 1.7f;
                field_0x268 = 0.0f;
                field_0x26c = 0.0f;
                break;
            case 1:
                field_0x264 = -1.7f;
                field_0x268 = 0.0f;
                field_0x26c = 0.0f;
                break;
            }
        } else {
            switch (field_0x98) {
            case 0:
                field_0x264 = 0.9f;
                field_0x268 = 0.0f;
                field_0x26c = 0.0f;
                break;
            case 1:
                field_0x264 = -0.9f;
                field_0x268 = 0.0f;
                field_0x26c = 0.0f;
                break;
            }
        }
        field_0x23c = 0;
        field_0x238 = 350;
        field_0x230 = 3;
        // fall through
    case 3: {
        field_0x268 = 0.2f * JMASinShort(field_0x25c);
        f32 margin = 16.0f;
        field_0x25c += 300;
        if (field_0x258 == 1) {
            margin = -24.0f;
            hit = r31;
        }
        if (hit || fn_801F6758(&mPos, field_0x4, margin) || field_0x238 == 0) {
            field_0x98 ^= 1;
            field_0x26c = 0.0f;
            field_0x268 = 0.0f;
            field_0x264 = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x23c = 160;
            field_0x238 = 20;
            field_0x230 = 0;
        }
        break;
    }
    case 4:
        switch (field_0x98) {
        case 0:
            field_0x264 = 0.4f;
            field_0x268 = 0.0f;
            field_0x26c = 0.0f;
            break;
        case 1:
            field_0x264 = -0.4f;
            field_0x268 = 0.0f;
            field_0x26c = 0.0f;
            break;
        case 2:
            field_0x264 = 0.0f;
            field_0x268 = 0.4f;
            field_0x26c = 0.0f;
            break;
        case 3:
            field_0x264 = 0.0f;
            field_0x268 = -0.4f;
            field_0x26c = 0.0f;
            break;
        }
        field_0x238 = 10;
        field_0x230 = 5;
        break;
    case 5: {
        f32 dx = field_0x270.x - mPos.x;
        f32 dy = field_0x270.y - mPos.y;
        if (field_0x238 == 0) {
            if (hit || std::sqrtf(dx * dx + dy * dy) > field_0x260) {
                field_0x98 ^= 1;
                field_0x26c = 0.0f;
                field_0x268 = 0.0f;
                field_0x264 = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                field_0x230 = 4;
            }
        }
        break;
    }
    }
    if (field_0x234 == 0) {
        field_0x234 = 8;
        if (field_0x230 == 3) {
            field_0x234 = 4;
        }
        field_0x244++;
        field_0x244 &= 1;
        field_0x250 = ((field_0x98 & 1) << 1) + field_0x244;
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
}

void CheepCheep_804A11B8::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    s32 hasMtx = 0;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x258 == 1) {
        hasMtx = 1;
        mtx[0][0] = 2.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 2.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 2.0f;
        params.mMtx = mtx;
    }
    s32 frame = field_0x250 + 0x143;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x250 + 0x143, hasMtx != 0, 2, -1, false, 0, 0);
}

CheepCheep_804A11B8::~CheepCheep_804A11B8() {}
