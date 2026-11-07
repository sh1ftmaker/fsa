#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/GameMath.h"
#include "game/Player.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "game/RotMatrix.h"
#include "game/ViewLayer.h"
#include "game/act/act_force_gem.h"

// Per-kind value table (fn_8032D564). The two SOB file names that follow it
// (lbl_80471A28, lbl_80471A3C) are string literals of unwritten functions
// that address them off the same base.
static const s32 lbl_804719D8[20] = {
    1, 5, 20, 50, 100, 150, 200, 300, 1000, 1500, 2000, 3000, 404, 403, 402, 401, 439, 440, 441, 442,
};

// Kick direction of a thrown gem: the new actor gets this velocity (x, y, 5).
static inline void setKick(ActorBase_804A5770* actor, s32 dir) {
    switch (dir) {
    case 0: {
        JGeometry::TVec3f v(-2.0f, 0.0f, 5.0f);
        actor->field_0x3c = v;
        break;
    }
    case 1: {
        JGeometry::TVec3f v(2.0f, 0.0f, 5.0f);
        actor->field_0x3c = v;
        break;
    }
    case 2: {
        JGeometry::TVec3f v(0.0f, -2.0f, 5.0f);
        actor->field_0x3c = v;
        break;
    }
    case 3: {
        JGeometry::TVec3f v(0.0f, 2.0f, 5.0f);
        actor->field_0x3c = v;
        break;
    }
    }
}

// 0x8032D6C0.
// FIXME(shotgun): 0 %, new
void ForceGem_804AB6D8::init() {
    field_0x94 = (field_0x90 >> 16) & 0xFF;
    field_0x268 = field_0x90 & 0xFF;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x240 = 0;
    field_0x258 = 0;
    field_0x242 = 0;
    field_0x25c = 0;
    field_0x244 = 0;
    field_0x260 = 1;
    field_0x280 = (field_0x90 >> 25) & 1;
    field_0x281 = (field_0x90 >> 26) & 1;
    field_0x278 = (field_0x90 >> 8) & 0xFF;
    field_0x234 = 0x12;
    field_0x264 = -1;
    field_0x238 = 1.0f;
    field_0x23c = 0.0f;
    field_0x250 = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 60;
    field_0x80.set(-4.0f, 0.0f, 4.0f, 8.0f);
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        field_0x24c = 0x3F2;
    } else {
        field_0x24c = 0x33E;
    }
    field_0x248 = 0;
    if (field_0x268 >= 8) {
        field_0x80.set(-8.0f, -4.0f, 8.0f, 12.0f);
        mBox.set(-16.0f, -16.0f, 16.0f, 16.0f);
    }
    field_0x26c = -1;
    field_0x282 = 0;
    field_0x27c = 1;
    field_0x27d = 0;
    field_0x27e = 0;
    field_0x284.x = field_0x284.y = field_0x284.z = 0.0f;
    field_0x27f = (field_0x90 >> 24) & 1;
    field_0x270 = 0;
    field_0x290.x = field_0x290.y = field_0x290.z = 0.0f;
    field_0x274 = 0;
    field_0x29c = mPos;
    field_0x283 = 0;
    switch (field_0x94) {
    case 0:
    case 0x22:
        field_0xb0 = 0x20000;
        if (field_0x268 >= 4) {
            field_0xb0 = 0x2000A;
        }
        if (viewLayerFlag(field_0x4) == 1) {
            mPos.y -= mPos.z;
            mPos.z = 0.0f;
            field_0x3c.y -= field_0x3c.z;
            field_0x3c.z = 0.0f;
        }
        field_0x230 = 0;
        break;
    case 1:
        field_0x230 = 0xB;
        break;
    case 2:
        field_0x24c = 300;
        field_0x80.set(-4.0f, -4.0f, 4.0f, 4.0f);
        mPos.z += 4.0f;
        mPos.y += 4.0f;
        field_0x230 = 2;
        break;
    case 3:
        field_0xb0 = 0x20000;
        if (field_0x268 >= 4) {
            field_0xb0 = 0x2000A;
        }
        if (viewLayerFlag(field_0x4) == 1) {
            field_0x3c.y = 3.0f;
        }
        field_0x24c = 0x30C;
        field_0x27d = 1;
        field_0x230 = 9;
        break;
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x21:
        field_0x248 = 2;
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x1F:
        if (vfunc_0x34()) {
            fn_801F3668();
            return;
        }
        field_0x24c = 0;
        field_0x230 = 8;
        field_0x280 = 1;
        break;
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x20:
        field_0x248 = 1;
    case 4:
    case 5:
    case 6:
    case 7:
    case 0x1D:
        if (vfunc_0x34()) {
            fn_801F3668();
            return;
        }
    case 8:
    case 9:
    case 0xA:
    case 0xB:
    case 0x1E:
        field_0x24c = 0;
        field_0x230 = 7;
        field_0x280 = 1;
        break;
    case 0x19:
    case 0x1A:
        field_0x238 = 0.0f;
        field_0x280 = 1;
        field_0x274 = 10;
        if (vfunc_0x34() && field_0x94 == 0x1A) {
            field_0x238 = 1.0f;
        }
        field_0x230 = 0xA;
        break;
    case 0x10:
        if (vfunc_0x34()) {
            fn_801F3668();
            return;
        }
        mPos.z = 400.0f;
        field_0x280 = 1;
        field_0x230 = 0xC;
        break;
    case 0x1B:
        if (!(mSerial & 1)) {
            field_0x242 = 8192.0f + 8192.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        } else {
            field_0x242 = 57344.0f - 8192.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        }
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            field_0x24c = 0x3F2;
        } else {
            field_0x24c = 0x33E;
        }
        field_0x230 = 0xD;
        break;
    case 0x1C:
        mPos.z = 400.0f;
        field_0x230 = 0xE;
        if (field_0x268 >= 4) {
            s32 layer = field_0x4;
            if (layer >= 8) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RSNP, layer, &mPos, 0, -1, -1);
            }
        }
        break;
    case 0x23:
    case 0x24:
        field_0x24c = 0x30C;
        field_0x27d = 1;
        field_0x230 = 9;
        break;
    case 0x25:
        field_0x274 = 10;
        field_0x24c = 0x30C;
        field_0x27d = 1;
        field_0x230 = 0;
        break;
    case 0x26:
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            field_0x24c = 0x3D9;
        } else {
            field_0x24c = 0x325;
        }
        field_0x274 = 10;
        if (viewLayerFlag(field_0x4) == 1) {
            mPos.y -= mPos.z;
            mPos.z = 0.0f;
            field_0x3c.y -= field_0x3c.z;
            field_0x3c.z = 0.0f;
        }
        field_0x230 = 0xF;
        break;
    }
    if (field_0x278 > 0) {
        field_0x234 = field_0x230;
        field_0x230 = 0x11;
    }
    if (gGameRoot_8053AAF8->field_0x7f == 0) {
        fn_801F3668();
    } else {
        field_0x194 = 1;
    }
}

void ForceGem_804AB6D8::vfunc_0x10() {
    if (field_0x94 == 0x19 || field_0x94 == 0x1a) {
        if (vfunc_0x34()) {
            field_0x280 = 0;
        }
    }
}

// 0x8032D578.
// FIXME(shotgun): 88.79 %, missing/extra + reloc-name
bool ForceGem_804AB6D8::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    u32 skipIds[2] = {'TOMI', 'NULL'};
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, skipIds)) {
        return true;
    }
    u8 result;
    u16 code = fn_8022461C(pos, layer);
    if (code == 0x3B || code == 0x23) {
        result = false;
    } else {
        result = fn_802265DC(pos, layer, false);
    }
    if (!result) {
        field_0x283 = 0;
    }
    return result;
}

s32 ActorBase_804A5770::fn_8032D564(s32 index) { return lbl_804719D8[index]; }

// 0x8032D534.
// FIXME(shotgun): 99.58 %, reloc-name
void ForceGem_804AB6D8::vfunc_0x40() {
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    if (field_0x24c < 0x320) {
        field_0x24c = 0x320;
    }
    field_0x230 = 1;
}

// 0x8032C664.
// FIXME(shotgun): not written yet

// FIXME(shotgun): table not written out (gate.py literal tripwire); extern-declared, size from
// symbols.txt (0x90). draw's base frame per kind (field_0x268).
extern const s32 lbl_80471948[36];

// 0x8032BFB0.
// FIXME(shotgun): 0 %, new
void ForceGem_804AB6D8::draw() {
    s32 state = field_0x230;
    if (state == 7 || state == 8 || state == 0xC || state == 0xE || state == 0x11) {
        return;
    }
    if (field_0x94 == 0x24) {
        return;
    }
    if (state == 0xA) {
        if (field_0x238 == 0.0f) {
            return;
        }
    } else if (state == 0xF || state == 0x10) {
        if (field_0x4 < 8) {
            return;
        }
    }
    const char* name = "enemy_both2_sob.bin";
    f32 scale = field_0x238;
    s32 gc = 0;
    if (field_0x268 >= 8) {
        name = "enemy_gc_sob.bin";
        gc = 1;
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    PSMTXIdentity(mtx);
    JGeometry::TVec3f rot(0.0f, field_0x23c, 0.0f);
    setRotScaleMtx(mtx, rot, scale);
    params.mMtx = mtx;
    params.field_0x2d = 1;
    params.field_0x20 = 0;
    if (field_0x230 == 5 || field_0x230 == 6) {
        params.field_0x10.a = 255.0f * (f32)field_0x24c / 30.0f;
        s32 frame = field_0x254 + lbl_80471948[field_0x268];
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', name);
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        return;
    }
    if (field_0x94 != 2 && field_0x24c < 0x100 && (field_0x24c & 2) && field_0x4 >= 8 &&
        gGameRoot_8053AAF8->field_0x96 != 1) {
        return;
    }
    params.field_0x1d = 1;
    if (field_0x4 < 8 && field_0x24c < 0xE0) {
        s32 m = field_0x24c % 64;
        if (m > 32) {
            params.field_0x10.a = 255.0f * (32.0f - (f32)m) * 0.03125f;
        } else {
            params.field_0x10.a = 255.0f * (f32)m * 0.03125f;
        }
    }
    if (mPos.z > 0.0f) {
        params.field_0x1c = 1;
    } else if (field_0x94 == 0x23) {
        params.field_0x1c = -1;
    } else {
        params.field_0x1c = 0;
    }
    s32 flags = 7;
    if (field_0x94 == 0x23) {
        flags = -1;
        params.field_0x10.a = 0x7F;
        params.field_0x10.r = 0x7F;
        params.field_0x10.g = 0x7F;
        params.field_0x10.b = 0x7F;
    } else if (field_0x230 == 3 || field_0x230 == 4) {
        if (field_0x264 >= 0 && field_0x264 <= 3) {
            params.field_0x1d = fn_8023CB24(field_0x264) + 1;
            params.field_0x1c = fn_8023CA88(field_0x264);
        }
        flags = -1;
    }
    if (field_0x230 == 0xD && mPos.z > 0.0f) {
        params.field_0x30 = 0x11;
    }
    JGeometry::TVec3f saved = mPos;
    mPos += field_0x290;
    drawSprite_801F06F0(name, field_0x254 + lbl_80471948[field_0x268], &params, flags, 1.0f, 1.0f);
    s32 frame = field_0x254 + lbl_80471948[field_0x268];
    if (field_0x268 >= 8) {
        frame = field_0x254 + lbl_80471948[field_0x268 - 4];
    }
    if (field_0x230 == 0xB) {
        fn_803075AC(mSerial, field_0x4, &mPos, frame, gc != 0, 2, -1, true, 0, 0);
    } else {
        fn_803075AC(mSerial, field_0x4, &mPos, frame, gc != 0, 2, -1, false, 0, 0);
    }
    mPos = saved;
}

// FIXME(shotgun): 95.41 %, missing/extra + reloc-name
void ForceGem_804AB6D8::fn_8032BB90() {
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023A51C(i) == true && field_0x4 == getPlayerLayer_8023DE58(i)) {
            JGeometry::TVec3f* p = fn_8023A478(i);
            f32 dx = p->x - mPos.x;
            f32 dy = p->y - mPos.y;
            if (!(square(dx) + square(dy) > 6400.0f) && gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&mPos) == true) {
                mPos.z = 5.0f;
                if (field_0x230 == 7 || field_0x248 != 0) {
                    u32 id = ACTOR_RUPY;
                    s32 dir;
                    if (field_0x248 != 0) {
                        id = ACTOR_ESRY;
                        field_0x268 = (field_0x90 & 0xF8000000) | field_0x268;
                        field_0x268 |= 0x10000;
                        if (field_0x94 == 0x20 || field_0x94 == 0x21) {
                            dir = fn_8023A240();
                        } else if (field_0x248 == 1) {
                            dir = field_0x94 - 0x11;
                        } else {
                            dir = field_0x94 - 0x15;
                        }
                    } else {
                        field_0x268 |= 0x2000000;
                        if (field_0x94 == 0x1d || field_0x94 == 0x1e) {
                            dir = fn_8023A240();
                        } else if (field_0x94 >= 4 && field_0x94 <= 7) {
                            dir = field_0x94 - 4;
                        } else {
                            dir = field_0x94 - 8;
                        }
                    }
                    s32 serial = getActorManager_801F856C()->fn_801F8544(id, field_0x4, &mPos, field_0x268);
                    if (serial >= 0) {
                        setKick(getActorManager_801F856C()->findActorBySerial_801F666C(serial), dir);
                    }
                    fn_801F3668();
                } else {
                    s32 dir;
                    if (field_0x94 == 0x1f) {
                        dir = fn_8023A240();
                    } else {
                        dir = field_0x94 - 0xc;
                    }
                    setKick(this, dir);
                    if (gGameRoot_8053AAF8->field_0x7e == 1) {
                        field_0x24c = 0x3d9;
                    } else {
                        field_0x24c = 0x325;
                    }
                    field_0xb0 = 0x20000;
                    if (field_0x268 >= 4) {
                        field_0xb0 = 0x2000A;
                    }
                    field_0x230 = 0;
                }
                if ((field_0x94 >= 4 && field_0x94 <= 7) || (field_0x94 >= 0xc && field_0x94 <= 0xf) ||
                    field_0x94 == 0x1d || field_0x94 == 0x1f) {
                    vfunc_0x30(0xFF, true);
                }
                break;
            }
        }
    }
}

// FIXME(shotgun): 96.87 %, missing/extra + reloc-name
void ForceGem_804AB6D8::fn_8032B7C0() {
    if (field_0x94 == 0x23 || field_0x94 == 0x24) {
        return;
    }
    s32 layer = field_0x4;
    bool near = false;
    if (getGameView_802D6A9C()->field_0x131df[layer] == 0) {
        if (field_0x230 == 1 || field_0x230 == 9) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023A51C(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                    JGeometry::TVec3f* p = fn_8023A478(i);
                    f32 dx = p->x - mPos.x;
                    f32 dy = p->y - mPos.y;
                    if (!(square(dx) + square(dy) > 6400.0f)) {
                        near = true;
                        break;
                    }
                }
            }
            if (near && mPos.z <= 4.0f) {
                field_0x3c.z = 5.0f;
                switch (fn_8023A240()) {
                case 0: {
                    JGeometry::TVec3f v(-2.0f, 0.0f, field_0x3c.z);
                    field_0x3c = v;
                    break;
                }
                case 1: {
                    JGeometry::TVec3f v(2.0f, 0.0f, field_0x3c.z);
                    field_0x3c = v;
                    break;
                }
                case 2: {
                    JGeometry::TVec3f v(0.0f, -2.0f, field_0x3c.z);
                    field_0x3c = v;
                    break;
                }
                case 3: {
                    JGeometry::TVec3f v(0.0f, 2.0f, field_0x3c.z);
                    field_0x3c = v;
                    break;
                }
                }
                field_0x260 = 1;
                field_0x230 = 0;
            }
        }
    } else if (field_0x230 == 0 || field_0x230 == 1 || field_0x230 == 9) {
        field_0x284.z = 0.0f;
        field_0x284.y = 0.0f;
        field_0x284.x = 0.0f;
        field_0x283 = 1;
        field_0x27e = fn_801F2618(&field_0x284, false);
        if (field_0x283 == 1) {
            mPos = field_0x29c;
        }
        if (field_0x230 != 0) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023A51C(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                    JGeometry::TVec3f* p = fn_8023A478(i);
                    f32 dx = p->x - mPos.x;
                    f32 dy = p->y - mPos.y;
                    if (!(square(dx) + square(dy) > 6400.0f)) {
                        near = true;
                        break;
                    }
                }
            }
            if (near && field_0x27e != 0) {
                field_0x3c.y = -5.0f;
                switch (fn_8023A240()) {
                case 0: {
                    JGeometry::TVec3f v(2.0f, field_0x3c.y, 0.0f);
                    field_0x3c = v;
                    break;
                }
                case 1: {
                    JGeometry::TVec3f v(-2.0f, field_0x3c.y, 0.0f);
                    field_0x3c = v;
                    break;
                }
                case 2: {
                    JGeometry::TVec3f v(0.0f, field_0x3c.y, 0.0f);
                    field_0x3c = v;
                    break;
                }
                case 3: {
                    JGeometry::TVec3f v(0.0f, field_0x3c.y, 0.0f);
                    field_0x3c = v;
                    break;
                }
                }
                field_0x260 = 1;
                field_0x230 = 0;
            }
        }
    }
}

// FIXME(shotgun): 99.19 %, regalloc-only + reloc-name
bool ForceGem_804AB6D8::fn_8032B3F8() {
    s32 layer = field_0x4;
    if (getGameView_802D6A9C()->field_0x131df[layer] == 0) {
        if (mPos.z > 0.0f || field_0x3c.z > 0.0f) {
            field_0x3c.z -= 0.3f;
        } else if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            if (field_0x280 == 1) {
                fn_801F0D20(0x42);
                field_0x280 = 0;
            }
            if (fn_8032B0A4()) {
                return false;
            }
            if (field_0x260 == 0) {
                if (!fn_802DEBA0()) {
                    startSound_801F0E34(0x3000A, 0);
                }
                field_0x260 = 1;
                if (field_0x27f == 0) {
                    field_0x3c.z = 2.0f + (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    field_0x3c.x = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                    field_0x3c.y = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                } else {
                    field_0x3c.z = 2.0f;
                    field_0x27f = 0;
                }
            } else {
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                if (field_0x27d != 0) {
                    field_0x230 = 9;
                } else {
                    field_0x230 = 1;
                }
            }
        }
    } else {
        if (field_0x3c.z != 0.0f) {
            field_0x3c.y -= field_0x3c.z;
            field_0x3c.z = 0.0f;
        }
        if (mPos.z > 0.0f) {
            mPos.y -= mPos.z;
            mPos.z = 0.0f;
        }
        field_0x3c.y += 0.3f;
        if (field_0x27e == 1 && field_0x3c.y > 0.0f) {
            if (field_0x284.x != 0.0f) {
                field_0x3c.x = field_0x3c.x - field_0x3c.x;
            }
            if (field_0x284.y != 0.0f) {
                if (field_0x280 == 1) {
                    fn_801F0D20(0x42);
                    field_0x280 = 0;
                }
                if (field_0x260 == 0) {
                    if (!fn_802DEBA0()) {
                        startSound_801F0E34(0x3000A, 0);
                    }
                    field_0x260 = 1;
                    field_0x3c.z = 0.0f;
                    field_0x3c.x = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                    field_0x3c.y = -2.0f + (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                } else {
                    field_0x3c.x = 0.0f;
                    field_0x3c.z = 0.0f;
                    field_0x3c.y = 3.0f;
                    if (field_0x27d != 0) {
                        field_0x230 = 9;
                    } else {
                        field_0x230 = 1;
                    }
                }
            }
        }
    }
    return true;
}

// FIXME(shotgun): 92.36 %, missing/extra + reloc-name
bool ForceGem_804AB6D8::fn_8032B0A4() {
    if (field_0x94 == 0x23 || field_0x94 == 0x24) {
        return false;
    }
    if (mPos.z > 4.0f) {
        return false;
    }
    u16 tile = fn_8022461C(&mPos, field_0x4);
    if (tile == 3) {
        field_0x24c = 0x3ff;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x230 = 5;
        return true;
    } else if (tile == 0xe) {
        field_0x24c = 0x3ff;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x238 = 0.0f;
        if (field_0x230 != 5) {
            field_0x230 = 5;
            u16 effectA;
            u16 effectB;
            if (fn_802DE9A0() == true) {
                startSound_801F0E34(0x50040, 0);
                effectA = 0x2a9;
                effectB = 0x2aa;
            } else if (fn_802DEB58() == true) {
                startSound_801F0E34(0x5003f, 0);
                effectA = 0x45e;
                effectB = 0x45f;
            } else {
                return true;
            }
            if (field_0x268 < 4) {
                spawnEmitter_8013CC50(field_0x4, &mPos, effectA, -1, 1, NULL, NULL, true, 0.75f);
                spawnEmitter_8013CC50(field_0x4, &mPos, effectB, 0, 0, NULL, NULL, true, 0.75f);
            } else if (field_0x268 < 8) {
                spawnEmitter_8013CC50(field_0x4, &mPos, effectA, -1, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, effectB, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                spawnEmitter_8013CC50(field_0x4, &mPos, effectA, -1, 1, NULL, NULL, true, 2.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, effectB, 0, 0, NULL, NULL, true, 2.0f);
            }
        }
        return true;
    } else if (tile == 0xd) {
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.z = -0.1f;
        field_0x238 = 1.0f;
        if (field_0x230 != 6) {
            field_0x24c = 30;
            if (field_0x268 < 4) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3d0, 0, 1, NULL, NULL, true, 0.75f);
            } else if (field_0x268 < 8) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3d0, 0, 1, NULL, NULL, true, 1.0f);
            } else {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3d0, 0, 1, NULL, NULL, true, 2.0f);
            }
            startSound_801F0E34(0x30022, 0);
        }
        field_0x230 = 6;
        return true;
    }
    return false;
}

// FIXME(shotgun): 81.84 %, missing/extra + reloc-name
void ForceGem_804AB6D8::fn_8032ABE0() {
    if (!(field_0x230 == 0 || field_0x230 == 1 || field_0x230 == 2 || field_0x230 == 6 || field_0x230 == 9 ||
          field_0x230 == 0xd)) {
        return;
    }
    if (field_0x94 != 2) {
        u8 mode = gGameRoot_8053AAF8->field_0x7e;
        if (mode == 1) {
            if (field_0x24c <= 0x3d4) {
                return;
            }
        } else if (mode != 0 || field_0x24c > 0x320) {
            return;
        }
    }
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    JGeometry::TBox2f box(mBox);
    box.addPos(mPos.x, mPos.y);
    s32 found = -1;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023B0F4(i, &box, field_0x4, false, mPos.z) == true) {
            if ((mPos.z <= 6.0f || field_0x94 == 2) && fn_80230188(i) != true && fn_80234ECC(i) != true &&
                fn_8023EA28(i) != true) {
                found = i;
                break;
            }
        }
    }
    if (found < 0 && (mPos.z < 6.0f || field_0x94 == 2)) {
        for (s32 i = 0; i < 4; i++) {
            if (!fn_80230188(i) && !fn_8023EA28(i) && !fn_80236B50(i)) {
                if (fn_8023B9CC(i, &box, field_0x4, &pos, true) == 1 || fn_8023B848(i, &box, field_0x4, &pos) == 1) {
                    found = i;
                    break;
                }
            }
        }
    }
    if (found < 0 || fn_80234C5C(found)) {
        return;
    }
    if (field_0x94 == 0x23) {
        if (fn_8022B7C4(found) != true) {
            return;
        }
    } else if (fn_8022B7C4(found)) {
        return;
    }
    field_0x238 = 1.0f;
    s32 other = found;
    if (fn_8022F514(found) == true) {
        other = fn_8022F3DC(found);
    }
    if (getGameView_802D6A9C()->field_0x4 == 0 && getGameView_802D6A9C()->field_0x8 == 0 &&
        !getGameView_802D6A9C()->fn_802D7F40(0)) {
        lbl_8053AED0->fn_8043D67C(0x13f, other, 0);
        getGameView_802D6A9C()->fn_802D7F84(0, true);
    }
    if (field_0x268 >= 4) {
        if (fn_8023ED8C(found) || fn_8023EBB8(found) || fn_80233DA4(found)) {
            lbl_8053AED0->fn_8043D874(found, lbl_804719D8[field_0x268], 0);
            field_0x94 = 1;
            field_0x264 = found;
            field_0x230 = 3;
            field_0x274 = 0;
            field_0xb0 = 0;
        } else if (fn_802308E0(found) == true) {
            JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(found);
            JGeometry::TVec3f spawnPos(playerPos.x, playerPos.y - 12.0f, playerPos.z);
            if (fn_80233E40(found)) {
                fn_80230498(found, &playerPos, 0x336, 0xff, -128, -128, true);
            } else {
                fn_80230498(found, &playerPos, 0xed, 0xff, -128, -128, true);
            }
            getActorManager_801F856C()->spawnActor_801F9484('IMUP', field_0x4, &spawnPos,
                                                           (found << 24) + (found << 26) + 0xA00000 + field_0x268, -1, -1);
            fn_801F3668();
        }
    } else {
        fn_80230FD0(other, 0x20011);
        fn_802362C4(found, lbl_804719D8[field_0x268]);
        field_0x94 = 1;
        field_0x264 = found;
        field_0x230 = 3;
        field_0x274 = 0;
        field_0xb0 = 0;
    }
}

// Destructor at 0x8032AB80.
ForceGem_804AB6D8::~ForceGem_804AB6D8() {}
