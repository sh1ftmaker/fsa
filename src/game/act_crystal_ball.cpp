#include "game/GameAudio.h"
#include "game/act/act_crystal_ball.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/Effect.h"
#include "game/ViewLayer.h"
#include "game/RotMatrix.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Map.h"

static const s32 lbl_80471760[9] = {5, 10, 15, 20, 25, 30, 35, 40, 45};


// Pattern table: 7 entries per row.
static const s32 lbl_80471784[6][7] = {
    {2, 2, 5, 5, 6, 7, 7},
    {8, 5, 6, 5, 6, 5, 6},
    {8, 6, 8, 6, 8, 5, 7},
    {2, 8, 3, 8, 4, 8, 4},
    {2, 3, 5, 3, 5, 3, 5},
    {2, 3, 4, 8, 5, 6, 7},
};


// The item shown / dropped for ball state `item`; inlined by draw (0x80320D18) and
// vfunc_0x24 (0x80320374).
inline s32 adjustItem_80320374(CrystalBall_804AB308* ball, s32 item) {
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            if (item == 8) {
                if (!fn_802DE814() && ball->field_0x2ad == 1) {
                    item = 3;
                }
            } else if (item == 4) {
                if (!fn_802DE814() && gGameRoot_8053AAF8->fn_80138A30() == 2) {
                    item = 3;
                }
            }
        }
        if (item == 7) {
            if (fn_802DE814() == 1 || fn_80237810(fn_8022F340(ball->field_0x23c)) != 0x10) {
                item = 6;
            }
        }
        if (item == 2) {
            if (getGameView_802D6A9C()->fn_802D7948() != 0 || ball->field_0x4 >= 8) {
                item = 6;
            }
        }
    }
    return item;
}

CrystalBall_804AB308::~CrystalBall_804AB308() {
    releaseEmitter_801EE434(field_0x2c4);
    releaseEmitter_801EE434(field_0x2c8);
}

// FIXME(shotgun): 98.4 %, missing/extra: one instruction
void CrystalBall_804AB308::init() {
    field_0x94 = field_0x90 & 0xF;
    field_0x284 = (field_0x90 >> 8) & 0xF;
    field_0x23c = (field_0x90 >> 16) & 3;
    field_0x290 = 0;
    field_0x2c4 = NULL;
    field_0x2c8 = NULL;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x280 = 0;
    field_0x294 = 0;
    field_0x288 = 0;
    field_0x298 = 1;
    field_0x2a4 = 0;
    field_0x2a5 = 0;
    field_0x2a6 = 1;
    field_0x2ad = 0;
    field_0x2b4 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x274 = 0;
    field_0x278 = 900;
    field_0x27c = 0;
    field_0x234 = 1.0f;
    field_0x238 = 0.0f;
    field_0x28c = 0xFF;
    field_0x29c = -1;
    field_0x80.i.x = -4.0f;
    field_0x80.i.y = -6.0f;
    field_0x80.f.x = 4.0f;
    field_0x80.f.y = 6.0f;
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mBox2.i.x = -10.0f;
    mBox2.i.y = -14.0f;
    mBox2.f.x = 10.0f;
    mBox2.f.y = 8.0f;
    field_0x2a8 = 0;
    field_0x2ac = 0;
    field_0x2b8.set(0.0f, 0.0f, 0.0f);
    field_0x2a0 = -1;
    switch (field_0x94) {
    case 0:
    case 2:
        field_0x248 = 10;
        field_0xb0 = 0x2100A;
        field_0x230 = 0;
        break;
    case 1:
        field_0xb0 = 0;
        if (fn_8022F9E4(field_0x23c, mSerial, false, false)) {
            fn_80322F44();
        } else {
            field_0x230 = 1;
        }
        break;
    case 3:
        field_0xb0 = 0;
        field_0x294 = 2;
        field_0x264 = 2;
        if (fn_8022F9E4(field_0x23c, mSerial, false, false)) {
            field_0x230 = 4;
        } else {
            field_0x230 = 3;
        }
        break;
    }
    if (viewLayerFlag(field_0x4) == 1) {
        mPos.y -= mPos.z;
        mPos.z = 0.0f;
        field_0x3c.y -= field_0x3c.z;
        field_0x3c.z = 0.0f;
    }
    field_0x2b0 = (field_0x90 >> 12) & 0xF;
    if (field_0x2b0 != 0) {
        field_0xb0 = 0;
        field_0x230 = 7;
    }
    if (field_0x230 == 0 && vfunc_0x4C(field_0x4, &mPos) == 1) {
        mAlive = false;
    }
}

// FIXME(shotgun): 99.5 %, different: one opcode
void CrystalBall_804AB308::vfunc_0x54() {
    mPos.z = 0.0f;
    field_0xb0 = 0;
    mBox.i.x = -16.0f;
    mBox.i.y = -16.0f;
    mBox.f.x = 16.0f;
    mBox.f.y = 16.0f;
    getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
    s32 mask;
    if (!gGameRoot_8053AAF8->field_0x7f) {
        mask = fn_801F2B7C(2, -1, 1);
    } else {
        mask = fn_801F2B7C(0, -1, 1);
    }
    if (mask > 0) {
        for (s32 i = 0; i < 4; i++) {
            if ((mask >> i) & 1) {
                fn_80236074(i, 100, 1);
            }
        }
    }
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x59, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x5A, 1, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x5B, 1, 1, NULL, NULL, true, 1.0f);
    startSound_801F0E34(0x30001, 0);
}

// FIXME(shotgun): 99.6 %, regalloc-only
void CrystalBall_804AB308::fn_80322F44() {
    if (field_0x284 == 0) {
        if (!gGameRoot_8053AAF8->field_0x7e) {
            field_0x288 = 0;
        } else if (gGameRoot_8053AAF8->field_0x7f == 1) {
            s32 base = fn_80236228(field_0x23c);
            s32 lo = base;
            s32 hi = base;
            for (s32 i = 0; i < 4; i++) {
                if (!fn_8022F514(i)) {
                    if (hi < fn_80236228(i)) {
                        hi = fn_80236228(i);
                    }
                    if (lo > fn_80236228(i)) {
                        lo = fn_80236228(i);
                    }
                }
            }
            if (hi == base) {
                field_0x288 = 1;
            } else if (lo == base) {
                field_0x288 = 3;
                if (hi - base >= 500) {
                    field_0x2ad = 1;
                }
            } else {
                field_0x288 = 2;
                if (hi - base >= 500) {
                    field_0x2ad = 1;
                }
            }
        } else {
            field_0x288 = 4;
        }
    } else if (field_0x284 == 1) {
        field_0x288 = 5;
    } else {
        field_0x288 = 5;
    }
    field_0x230 = 2;
    if (field_0x284 == 0) {
        field_0x2b4 = (s32)(7.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) % 7;
    } else if (field_0x284 == 1) {
        field_0x2b4 = (s32)(7.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) % 7;
    } else {
        field_0x2b4 = field_0x284;
    }
    field_0x294 = lbl_80471784[field_0x288][field_0x2b4];
    if (!gGameRoot_8053AAF8->field_0x7f) {
        field_0x260 = 20;
    } else {
        field_0x24c = (s32)(7.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 40;
        field_0x268 = field_0x2b4 - field_0x24c % 7;
        if (field_0x268 < 0) {
            field_0x268 += 7;
        }
        field_0x264 = lbl_80471784[field_0x288][field_0x268];
        field_0x248 = 5;
        field_0x2a6 = 0;
    }
}

// FIXME(shotgun): 98.4 %, missing/extra: a couple of instructions in the player loop
void CrystalBall_804AB308::fn_80322BC4() {
    s32 rank = 0;
    s32 n = 0;
    if (!gGameRoot_8053AAF8->field_0x7e) {
        rank = 4;
    } else if (gGameRoot_8053AAF8->field_0x7f == 1) {
        s32 base = fn_80236228(field_0x23c);
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i) && !fn_80230188(i) &&
                base < fn_80236228(i)) {
                rank++;
            }
        }
    } else {
        s32 base = fn_80237774(field_0x23c);
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i) && !fn_80230188(i) &&
                base < fn_80237774(i)) {
                rank++;
            }
        }
    }
    s32 candidates[10];
    if ((field_0x90 >> 18) & 1) {
        candidates[n++] = 10;
    }
    if ((field_0x90 >> 19) & 1) {
        candidates[n++] = 3;
    }
    if ((field_0x90 >> 20) & 1) {
        candidates[n++] = 4;
    }
    if ((field_0x90 >> 21) & 1) {
        candidates[n++] = 8;
    }
    if ((field_0x90 >> 22) & 1) {
        candidates[n++] = 6;
    }
    if ((field_0x90 >> 23) & 1) {
        candidates[n++] = 5;
    }
    if ((field_0x90 >> 24) & 1) {
        candidates[n++] = 7;
    }
    if ((field_0x90 >> 25) & 1) {
        candidates[n++] = 1;
    }
    if ((field_0x90 >> 4) & 1) {
        candidates[n++] = 2;
    }
    field_0x294 = candidates[(s32)((f32)n * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) % n];
    if (field_0x294 != 2) {
        f32 chance;
        switch (rank) {
        case 0:
            chance = 0.9f;
            break;
        case 1:
            chance = 0.6f;
            break;
        case 2:
            chance = 0.4f;
            break;
        case 3:
            chance = 0.1f;
            break;
        case 4:
            chance = 0.5f;
            break;
        }
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() >= chance) {
            field_0x298 = 2;
        }
    }
    field_0x230 = 4;
}

// FIXME(shotgun): 96.6 %, missing/extra: the switch over rank is laid out differently
void CrystalBall_804AB308::fn_80322964() {
    mBox.i.x = -6.0f;
    mBox.i.y = -6.0f;
    mBox.f.x = 6.0f;
    mBox.f.y = 6.0f;
    JGeometry::TVec3f saved = mPos;
    u8 mask = fn_801F2740(-1);
    if (mask != 0) {
        u8 bit = 1;
        for (s32 i = 0; i < 4; i++) {
            if (mask & bit) {
                JGeometry::TVec3f pp = *getPlayerPos_8023E724(i);
                s32 dx = (s32)__fabs(mPos.x - pp.x);
                s32 dy = (s32)__fabs(mPos.y - pp.y);
                s32 dir = fn_80230CFC(i);
                if ((dx >= 0 && dx <= 15 && dir == 0) || dir == 1) {
                    if (mPos.x > pp.x) {
                        mPos.x += (f32)((15 - dx) >> 1);
                    } else {
                        mPos.x -= (f32)((15 - dx) >> 1);
                    }
                }
                if (!viewLayerFlag(field_0x4)) {
                    if ((dy >= 0 && dy <= 15 && dir == 3) || dir == 2) {
                        if (mPos.y > pp.y) {
                            mPos.y += (f32)((15 - dy) >> 1);
                        } else {
                            mPos.y -= (f32)((15 - dy) >> 1);
                        }
                    }
                }
            }
            bit <<= 1;
        }
    }
    if (fn_801F2618(NULL, false) == 1) {
        mPos = saved;
    }
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
}

// FIXME(shotgun): 99.9 %, reloc-name only (rodata table pooling)
void CrystalBall_804AB308::fn_803226EC() {
    if (field_0x230 == 0 || ((u32)(field_0x230 - 5) <= 3) || field_0x94 == 2) {
        if (field_0x248 <= 0) {
            field_0x264 ^= 1;
            field_0x248 = 10;
        }
    } else if (field_0x230 == 2) {
        if (field_0x2a6 == 1) {
            if (field_0x260 > 0) {
                if (field_0x248 <= 0) {
                    field_0x264 ^= 1;
                    field_0x248 = 10;
                }
                field_0x260--;
            } else {
                field_0x24c = (s32)(7.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 40;
                field_0x268 = field_0x2b4 - field_0x24c % 7;
                if (field_0x268 < 0) {
                    field_0x268 += 7;
                }
                field_0x264 = lbl_80471784[field_0x288][field_0x268];
                field_0x248 = 5;
                field_0x2a6 = 0;
            }
        } else if (field_0x248 <= 0) {
            if (field_0x24c > 7) {
                field_0x248 = 5;
            } else {
                if (field_0x24c < 0) {
                    field_0x24c = 0;
                }
                field_0x248 = lbl_80471760[7 - field_0x24c];
            }
            field_0x268++;
            field_0x268 %= 7;
            field_0x264 = lbl_80471784[field_0x288][field_0x268];
            startSound_801F0E34(0x3006A, 0);
            field_0x24c--;
        }
    }
    if (field_0x248 >= 0) {
        field_0x248--;
    }
}

// FIXME(shotgun): 82.6 %, missing/extra: the code comparison chain and the doubled fn_80226850 test are laid out differently
bool CrystalBall_804AB308::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 code = fn_8022461C(pos, layer);
    s32 tile = fn_80226850(pos, layer);
    if ((u32)(code - 0x10) <= 6 || code == 0x17) {
        field_0x274 = 1;
        return false;
    }
    if (field_0x2a8 == 0 && code == 3) {
        if (mPos.z < 0.1f) {
            field_0x2b8.x = 8.0f + (f32)((s32)pos->x & 0xFFF0);
            field_0x2b8.y = 8.0f + (f32)((s32)pos->y & 0xFFF0);
            field_0x2a8 = 1;
            mPos.z = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
        }
        return false;
    }
    if (code == 0xE && !field_0x2ac) {
        if (mPos.z < 0.1f) {
            u16 effectA;
            u16 effectB;
            if (fn_802DEB58()) {
                effectA = 0x45E;
                effectB = 0x45F;
            } else if (fn_802DE9A0()) {
                effectA = 0x2A9;
                effectB = 0x2AA;
                startSound_801F0E34(0x5005F, 0);
            } else {
                if (mPos.z < 0.1f) {
                    field_0x2b8.x = 8.0f + (f32)((s32)pos->x & 0xFFF0);
                    field_0x2b8.y = 8.0f + (f32)((s32)pos->y & 0xFFF0);
                    field_0x2a8 = 1;
                    field_0x2ac = 1;
                    mPos.z = 0.0f;
                    field_0x3c.set(0.0f, 0.0f, 0.0f);
                }
                return false;
            }
            spawnEmitter_8013CC50(field_0x4, &mPos, effectA, -1, 1, NULL, NULL, true, 2.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, effectB, 0, 0, NULL, NULL, true, 2.0f);
            field_0x2ac = 1;
            fn_801F3668();
        }
        return false;
    }
    if ((code == 0xD || code == 0x3B) && !field_0x2ac) {
        if (mPos.z < 0.1f) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 2.0f);
            fn_801F3668();
            field_0x2ac = 1;
        }
        return false;
    }
    if (code == 0 || code == 3 || code == 0xE || code == 0xD || code == 0x3B || code == 4 || code == 5 ||
        code == 6 || code == 7 || code == 2 || code == 0x50 || code == 0xC || code == 0x27 || code == 0x28 ||
        code == 0x29 || code == 0x2A || code == 0x2B || code == 0x2C || code == 0x2D || code == 0x2E ||
        code == 0x2F || code == 0x20 || code == 0x39 || code == 0x3A || code == 0x4C || code == 0x4D ||
        code == 0x4E || code == 0x4F) {
        return false;
    }
    if (fn_80226850(pos, layer) == 0x3A || fn_80226850(pos, layer) == 0x3D) {
        field_0x274 = 0;
        return false;
    }
    if (code == 0x25) {
        if (field_0x274 == 0 && !getGameView_802D6A9C()->field_0x171) {
            return true;
        }
        return false;
    }
    if (code == 0x26) {
        if (field_0x274 == 0 && getGameView_802D6A9C()->field_0x171 == 1) {
            return true;
        }
        return false;
    }
    if (field_0x274 == 0 && (code != 0x22 || getGameView_802D6A9C()->field_0x170 == 1)) {
        return true;
    }
    return false;
}

// FIXME(shotgun): 99.3 %, scheduling
void CrystalBall_804AB308::vfunc_0x40() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    if (field_0x278 <= 180) {
        field_0x278 = 182;
    }
}

// FIXME(shotgun): 53.54 %, missing/extra + reloc-name (partial)
// FIXME(shotgun): states 1, 5, 6 and 7 (0x803218B8 onward: 0x80321D14, 0x80321EE4, 0x80322010) and the
// end of state 4 (0x80321CCC onward) are not written.
void CrystalBall_804AB308::update() {
    switch (field_0x230) {
    case 1:
    case 3:
        if (fn_8022F9E4(field_0x23c, mSerial, false, false) == true) {
            if (field_0x230 == 1) {
                fn_80322F44();
            } else {
                field_0x230 = 4;
            }
        }
        return;
    }
    mPos += field_0x3c;
    if (field_0x2a8 > 0) {
        addCalc2_801F71A4(&mPos.x, field_0x2b8.x, 0.1f, 1.0f);
        addCalc2_801F71A4(&mPos.y, field_0x2b8.y, 0.1f, 1.0f);
        field_0x238 += 0.1f;
        field_0x234 -= 0.02f;
        if (field_0x234 < 0.1f) {
            fn_801F3668();
        }
        return;
    }
    switch (field_0x230) {
    case 0:
    case 8: {
        if (!viewLayerFlag(field_0x4)) {
            if (mPos.z > 0.0f) {
                if (field_0x27c == 0) {
                    startSound_801F0E34(0x30048, 0);
                    field_0x27c = 1;
                }
                field_0x3c.z -= 0.1f;
            } else if (mPos.z < 0.0f) {
                if (field_0x2a4 == 0) {
                    field_0x230 = 0;
                    fn_801F2618(NULL, false);
                    if (field_0x2a8 > 0 || field_0x2ac == 1) {
                        return;
                    }
                    startSound_801F0E34(0x30049, 0);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x3CF, 0, -1, NULL, NULL, true, 1.5f);
                    field_0x2a4 = 1;
                    field_0x3c.z = 0.5f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 0.5f;
                    field_0x3c.x = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                    field_0x3c.y = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                } else {
                    field_0x2a4 = 0;
                    mPos.z = 0.0f;
                    field_0x3c.z = 0.0f;
                    field_0x3c.y = 0.0f;
                    field_0x3c.x = 0.0f;
                }
            }
            if (field_0x230 == 0) {
                fn_801F2618(NULL, false);
            }
        } else {
            JGeometry::TVec3f hitNormal(0.0f, 0.0f, 0.0f);
            fn_801F2618(&hitNormal, false);
            if (hitNormal.y == 0.0f) {
                if (field_0x27c == 0) {
                    startSound_801F0E34(0x30048, 0);
                    field_0x27c = 1;
                }
                if (field_0x2a5 == 1) {
                    field_0x2a4 = 0;
                    field_0x2a5 = 0;
                }
                field_0x3c.y += 0.15f;
            } else if (field_0x2a4 == 0) {
                startSound_801F0E34(0x30049, 0);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3CF, 0, -1, NULL, NULL, true, 1.5f);
                field_0x2a4 = 1;
                field_0x3c.x = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                field_0x3c.y = -0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            } else {
                field_0x2a5 = 1;
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                field_0x3c.y = 1.0f;
            }
        }
        JGeometry::TBox2f box = mBox;
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        field_0x29c = -1;
        if (field_0x2a8 == 0) {
            fn_80322964();
            for (s32 i = 0; i < 4; i++) {
                if (fn_80239C00(i, &box, field_0x4) == true && getPlayerPos_8023E724(i)->z <= 0.0f) {
                    field_0x29c = i;
                    break;
                }
            }
        }
        if (field_0x29c >= 0 && !fn_8023ED8C(field_0x29c) && !fn_8023EBB8(field_0x29c)) {
            field_0x234 = 1.0f;
            field_0xb0 = 0;
            if (gGameRoot_8053AAF8->field_0x7f == 1) {
                if (fn_802308E0(field_0x29c) == true) {
                    field_0x23c = field_0x29c;
                    const JGeometry::TVec3f* playerPos = getPlayerPos_8023E724(field_0x29c);
                    JGeometry::TVec3f target(playerPos->x, playerPos->y - 12.0f, playerPos->z);
                    fn_80230498(field_0x29c, &target, 0xED, 0xFF, -0x80, -0x80, true);
                    mPos.set(target);
                    field_0x258 = 0x5A;
                    field_0x25c = 10;
                    field_0x3c.set(0.0f, 0.0f, 0.0f);
                    field_0x230 = 5;
                    fn_801F3604();
                }
            } else if (fn_8022F9E4(field_0x29c, mSerial, true, false) == true) {
                field_0x23c = field_0x29c;
                if (field_0x94 == 0) {
                    fn_80322F44();
                    if (field_0x4 >= 8 && gGameRoot_8053AAF8->field_0x7e == 1) {
                        lbl_8053AED0->fn_8043D67C(0x1A1, field_0x29c, 0);
                    }
                } else if (field_0x94 == 2) {
                    fn_80322BC4();
                }
            }
        }
        if (gGameRoot_8053AAF8->field_0x7f == 0) {
            field_0x278--;
            if (field_0x278 <= 0) {
                fn_801F3668();
            }
        }
        break;
    }
    case 2:
        if (!viewLayerFlag(field_0x4)) {
            if (field_0xc4) {
                const JGeometry::TVec3f* holder = fn_8023E7B0(field_0xc8);
                mPos.set(*holder);
                field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
                field_0x28c = fn_8023BCD4(field_0xc8);
            } else {
                field_0x3c.z += -0.1f;
                mPos += field_0x3c;
                if (mPos.z < 0.0f) {
                    vfunc_0x24();
                } else if (fn_801F2618(NULL, false)) {
                    vfunc_0x24();
                }
            }
        } else if (field_0xc4) {
            const JGeometry::TVec3f* holder = fn_8023E7B0(field_0xc8);
            mPos.set(*holder);
            field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
            field_0x28c = fn_8023BCD4(field_0xc8);
        } else {
            if (field_0x26c == 0) {
                field_0x3c.y = field_0x3c.y - field_0x3c.z;
                mPos.y = mPos.y - mPos.z;
                mPos.z = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x26c = 1;
            }
            if (field_0x26c == 1) {
                field_0x3c.y += 0.15f;
            }
            mPos += field_0x3c;
            if (fn_801F2618(NULL, false)) {
                vfunc_0x24();
            }
        }
        if (field_0x24c <= 0 && field_0x294 == field_0x264 && field_0x248 <= 1) {
            field_0x248 = 0x5A;
            field_0x230 = 4;
        }
        break;
    case 4:
        // FIXME(shotgun): the body of state 4 (0x80321AF8) is not written.
        break;
    }
}

// FIXME(shotgun): 97.0 %, missing/extra: a few instructions
void CrystalBall_804AB308::draw() {
    if (field_0x230 == 0 && field_0x278 <= 180 && (field_0x278 & 2) == 0) {
        return;
    }
    if (field_0x94 == 0 && field_0x230 == 4 && field_0xc4 == 1 && field_0x248 % 30 >= 15) {
        return;
    }
    if (field_0x2b0 >= 6 || field_0x28c == 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    bool held = false;
    s32 flags = -1;
    params.field_0x10.a = field_0x28c;
    params.field_0x2d = 1;
    params.field_0x1d = 1;
    params.field_0x1c = 1;
    Mtx mtx;
    if (field_0x2a8 == 1) {
        setRotScaleMtx(mtx, JGeometry::TVec3f(0.0f, field_0x238, 0.0f), field_0x234);
        params.mMtx = mtx;
        params.field_0x1d = 0;
        params.field_0x1c = 0;
        flags = -1;
    }
    if (field_0xc4) {
        params.field_0x20 = 0;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
        held = true;
    } else if (field_0x230 == 5 || field_0x230 == 6) {
        params.field_0x20 = 0;
        params.field_0x1d = fn_8023CB24(field_0x29c) + 1;
        params.field_0x1c = fn_8023CA88(field_0x29c);
        held = true;
    }
    s32 frame = field_0x264;
    frame = adjustItem_80320374(this, frame);
    if (frame == 2) {
        frame = 8;
    } else if (frame == 8) {
        frame = 4;
    } else if (frame == 4) {
        frame = 10;
    }
    drawSprite_801F06F0("enemy_both2_sob.bin", frame + 0x90, &params, flags, 1.0f, 1.0f);
    fn_803075AC(mSerial, field_0x4, &mPos, frame + 0x90, 0, 2, -1, held, 0, 0);
}

// FIXME(shotgun): 82.2 %, missing/extra: item switch / spawn sequences are laid out differently
void CrystalBall_804AB308::vfunc_0x24() {
    u16 code = fn_8022461C(&mPos, field_0x4);
    if (code == 3 && mPos.z < 0.1f) {
        field_0x2b8.x = 8.0f + (f32)((s32)mPos.x & 0xFFF0);
        field_0x2b8.y = 8.0f + (f32)((s32)mPos.y & 0xFFF0);
        field_0x2a8 = 1;
        mPos.z = 0.0f;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        return;
    }
    if (code == 0xE && mPos.z < 0.1f) {
        fn_802DEB58();
        if (fn_802DE9A0()) {
            startSound_801F0E34(0x5005F, 0);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 2.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, 0, 0, NULL, NULL, true, 2.0f);
            fn_801F3668();
        } else {
            field_0x2b8.x = 8.0f + (f32)((s32)mPos.x & 0xFFF0);
            field_0x2b8.y = 8.0f + (f32)((s32)mPos.y & 0xFFF0);
            field_0x2a8 = 1;
            field_0x2ac = 1;
            mPos.z = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
        }
        return;
    }
    if ((code == 0xD && mPos.z < 0.1f) || code == 0x3B) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 2.0f);
        fn_801F3668();
        return;
    }
    if (field_0x4 >= 8) {
        JGeometry::TVec3f p = mPos;
        p.z = 0.0f;
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, field_0x4, &p, 0, -1, -1);
    } else {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x564, 2, 0, NULL, NULL, true, 0.5f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x503, 1, 0, NULL, NULL, true, 1.0f);
    }
    startSound_801F0E34(0x30003, 0);
    switch (field_0x94) {
    case 0:
    case 1:
    case 3: {
        s32 item = field_0x264;
        if (item < 2) {
            item = field_0x294;
        }
        item = adjustItem_80320374(this, item);
        switch (item) {
        case 2: {
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_NNJN, field_0x4, &mPos, 1, -1, -1);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    actor->field_0x3c.set(0.0f, 0.0f, 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                }
            }
            break;
        }
        case 4: {
            JGeometry::TVec3f p(mPos.x, mPos.y - mPos.z, 0.0f);
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_NIWA, field_0x4, &p, ((field_0x23c & 3) << 8) + 3, -1, -1);
            break;
        }
        case 6:
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &mPos, 0x260002, -1, -1);
            break;
        case 5: {
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BMTY, field_0x4, &mPos, 1, -1, -1);
            if (viewLayerFlag(field_0x4)) {
                if (serial >= 0) {
                    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (actor != NULL) {
                        if (field_0x3c.x == 0.0f) {
                            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() > 0.5f) {
                                actor->field_0x3c.set(2.5f, 0.0f, 0.0f);
                            } else {
                                actor->field_0x3c.set(-2.5f, 0.0f, 0.0f);
                            }
                        } else {
                            actor->field_0x3c = field_0x3c;
                        }
                    }
                }
            } else {
                if (serial >= 0) {
                    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (actor != NULL) {
                        actor->field_0x3c = field_0x3c;
                    }
                }
                if (field_0x3c.x == 0.0f) {
                    JGeometry::TVec3f p(mPos.x, mPos.y, 0.0f);
                    s32 a = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BMTY, field_0x4, &p, 2, -1, -1);
                    if (a >= 0) {
                        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(a);
                        if (actor != NULL) {
                            f32 v = 0.7f * field_0x3c.y;
                            actor->field_0x3c.set(v, v, field_0x3c.z);
                        }
                    }
                    s32 b = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BMTY, field_0x4, &mPos, 2, -1, -1);
                    if (b >= 0) {
                        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(b);
                        if (actor != NULL) {
                            actor->field_0x3c.set(0.7f * -field_0x3c.y, 0.7f * field_0x3c.y, field_0x3c.z);
                        }
                    }
                } else if (field_0x3c.y == 0.0f) {
                    s32 a = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BMTY, field_0x4, &mPos, 2, -1, -1);
                    if (a >= 0) {
                        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(a);
                        if (actor != NULL) {
                            f32 v = 0.7f * field_0x3c.x;
                            actor->field_0x3c.set(v, v, field_0x3c.z);
                        }
                    }
                    s32 b = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BMTY, field_0x4, &mPos, 2, -1, -1);
                    if (b >= 0) {
                        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(b);
                        if (actor != NULL) {
                            actor->field_0x3c.set(0.7f * field_0x3c.x, 0.7f * -field_0x3c.x, field_0x3c.z);
                        }
                    }
                }
            }
            break;
        }
        case 7:
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BHRT, field_0x4, &mPos, 0, -1, -1);
            break;
        case 3:
        case 8: {
            s32 param;
            if (item == 3) {
                param = ((field_0x23c & 3) << 8) + 0x32;
            } else {
                param = ((field_0x23c & 3) << 8) + 0x38;
            }
            JGeometry::TVec3f p(mPos.x, mPos.y - mPos.z, 0.0f);
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_NIWA, field_0x4, &p, param, -1, -1);
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL) {
                    field_0x3c.z = 0.0f;
                    actor->field_0x3c = field_0x3c;
                }
            }
            break;
        }
        }
        fn_801F3668();
        break;
    }
    case 2:
        mPos.z = 0.0f;
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_LSTI, field_0x4, &mPos, field_0x294 | 0x180 | ((field_0x298 & 3) << 22), -1, -1);
        fn_801F3668();
        break;
    default:
        fn_801F3668();
        break;
    }
}
