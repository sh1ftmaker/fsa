#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/RotMatrix.h"
#include "game/ActorId.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/SpriteParams.h"
#include "game/act/tu_800F46C0.h"
#include "game/act/tu_800F7FCC.h"

// Sprite names of the shard textures (.data 0x8049C458, not written as source here), indexed by field_0x240.
extern const char* const lbl_8049C458[];
// Offsets (x, y) of the four tiles checked around a landing boulder (.rodata 0x80463678).
extern const f32 lbl_80463678[];

Boulder_8049C4C0::~Boulder_8049C4C0() {}

// FIXME(shotgun): 97.48 %, missing/extra + reloc-name
void Boulder_8049C4C0::init() {
    field_0x23c = 0;
    field_0x231 = 0;
    field_0x238 = 0;
    field_0x240 = 0;
    field_0x232 = -1;
    field_0x254.set(0.0f, 0.0f, 0.0f);
    field_0x230 = field_0x90;
    field_0x250 = 0;
    field_0x24c = 0;
    field_0x248 = 0;
    field_0x244 = 0;
    switch (field_0x230) {
    case 0:
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x80.i.x = -16.0f;
        field_0x80.i.y = -16.0f;
        field_0x80.f.x = 16.0f;
        field_0x80.f.y = 16.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.5f + gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        field_0x3c.z = 1.0f;
        break;
    case 1:
        mBox.i.x = -10.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 10.0f;
        field_0x80.i.x = -10.0f;
        field_0x80.i.y = -10.0f;
        field_0x80.f.x = 10.0f;
        field_0x80.f.y = 10.0f;
        break;
    case 2:
        mBox.i.x = -6.0f;
        mBox.i.y = -6.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 6.0f;
        field_0x80.i.x = -6.0f;
        field_0x80.i.y = -6.0f;
        field_0x80.f.x = 6.0f;
        field_0x80.f.y = 6.0f;
        mPos.z = 1.0f;
        break;
    case 3:
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x80.i.x = -16.0f;
        field_0x80.i.y = -16.0f;
        field_0x80.f.x = 16.0f;
        field_0x80.f.y = 16.0f;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        break;
    case 5:
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x80.i.x = -16.0f;
        field_0x80.i.y = -16.0f;
        field_0x80.f.x = 16.0f;
        field_0x80.f.y = 16.0f;
        field_0x240 = (s32)(5.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        field_0x244 = 0;
        field_0x248 = 0x300 - (s32)(1536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        break;
    }
}

// FIXME(shotgun): 93.17 %, missing/extra + reloc-name
void Boulder_8049C4C0::fn_800F5E08() {
    if (field_0x240 != 0) {
        field_0x244++;
        if (field_0x240 != 2 && field_0x244 >= 0x78) {
            mPos.x += 0.8f * JMath::sincosTable_.sinShort((field_0x244 & 3) << 14);
        }
        if (field_0x244 >= 0xB4) {
            vfunc_0x30(0xFF, false);
            field_0x240 = 2;
            field_0x248++;
            if (field_0x248 == 0x3C) {
                mAlive = false;
            }
            return;
        }
        for (s32 i = 0; i < 4; i++) {
            if (getPlayerLayer_8023DE58(i) == field_0x4) {
                f32 x0 = mBox.i.x + mPos.x;
                f32 y0 = mBox.i.y + mPos.y;
                f32 x1 = mBox.f.x + mPos.x;
                f32 y1 = mBox.f.y + mPos.y;
                for (s32 j = 0; j < 8; j++) {
                    JGeometry::TVec3f p;
                    fn_8022E088(i, j, &p);
                    bool inside = false;
                    if (p.x >= x0 && p.y >= y0 && p.x < x1 && p.y < y1) {
                        inside = true;
                    }
                    if (inside) {
                        fn_8022DF48(i);
                        break;
                    }
                }
            }
        }
        return;
    }
    field_0x231++;
    if (field_0x231 == 10) {
        field_0x23c++;
        field_0x231 = 0;
    }
    field_0x23c = field_0x23c & 3;
    fn_801F2B7C(2, -1, 6);
    s32 serial = fn_801F2718(ACTOR_OIWA);
    if (serial != -1) {
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        JGeometry::TVec3f d(mPos.x - other->mPos.x, mPos.y - other->mPos.y, mPos.z - other->mPos.z);
        if (d.x > 0.0f) {
            field_0x3c.x = 0.5f;
        } else {
            field_0x3c.x = -0.5f;
        }
    }
    field_0x3c.z -= 0.04f;
    mPos += field_0x3c;
    if (mPos.z <= 0.0f) {
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5) {
            field_0x3c.x = 0.5f;
        } else {
            field_0x3c.x = -0.5f;
        }
        mPos.z = 0.0f;
        field_0x3c.z = 1.0f;
        startSound_801F0E34(0x30028, 0);
        JPABaseEmitter* emitter = spawnEmitter_8013CC50(field_0x4, &mPos, 0x3CF, 1, 0, NULL, NULL, true, 1.2f);
        if (emitter != NULL) {
            emitter->mDirSpeed = 0.0f;
        }
        if (field_0x230 == 4) {
            const f32* offset = lbl_80463678;
            for (s32 k = 0; k < 4; k++, offset += 2) {
                JGeometry::TVec3f p = mPos;
                p.x += offset[0];
                p.y += offset[1];
                u16 code = fn_8022461C(&p, field_0x4);
                if ((code == 3 || code == 0xE || code == 2) && !vfunc_0x34()) {
                    getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x76, 0, 0, NULL, NULL, true, 0.8f);
                    field_0x240 = 1;
                    vfunc_0x30(0xFF, true);
                    startSound_801F0E34(0x4002C, 0);
                    s32 btin = getActorManager_801F856C()->fn_801F778C(ACTOR_BTIN, &mPos, NULL);
                    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(btin);
                    mPos.x = actor->mPos.x;
                    mPos.y = actor->mPos.y;
                    mPos.z = actor->mPos.z;
                    mPos.z = 0.0f;
                    mPos.y += 8.0f;
                }
            }
        }
    }
    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    if (50.0f + box->f.y < mPos.y) {
        mAlive = false;
    }
}

// FIXME(shotgun): 94.65 %, missing/extra + reloc-name
void Boulder_8049C4C0::fn_800F555C() {
    JGeometry::TBox2f box(mPos.x - 16.0f, mPos.y - 16.0f, mPos.x + 16.0f, mPos.y + 16.0f);
    bool anyPlayer = false;
    for (s32 i = 0; i < 4; i++) {
        if (field_0x4 == getPlayerLayer_8023DE58(i)) {
            anyPlayer = true;
        }
    }
    if (field_0x240 == 0) {
        fn_80239914(&box, field_0x4, mSerial);
        s32 pushers = fn_80239108(mSerial);
        s32 dir = 0;
        if (pushers >= 2) {
            dir = fn_80238BC4(mSerial);
            if (dir >= 0 && dir < 4) {
                field_0x244++;
                switch (dir) {
                case 0:
                    field_0x254.x = 3.0f;
                    break;
                case 1:
                    field_0x254.x = -3.0f;
                    break;
                case 2:
                    field_0x254.y = 3.0f;
                    break;
                case 3:
                    field_0x254.y = -3.0f;
                    break;
                }
            }
        } else {
            if (field_0x244 < 0x3C) {
                field_0x244 = 0;
            }
            field_0x254.set(0.0f, 0.0f, 0.0f);
        }
        if (field_0x244 >= 0x3C) {
            field_0x240 = 1;
            field_0x254.set(0.0f, 0.0f, 0.0f);
            switch (dir) {
            case 0:
                field_0x232 = 0;
                field_0x234 = 0;
                break;
            case 1:
                field_0x232 = 1;
                field_0x234 = 1;
                break;
            case 2:
                field_0x232 = 2;
                field_0x234 = 2;
                break;
            case 3:
                field_0x232 = 3;
                field_0x234 = 3;
                break;
            }
            fn_80238E08(mSerial);
            for (s32 i = 0; i < 4; i++) {
                if (fn_80238CB4(i) == mSerial) {
                    fn_80230FD0(i, 0x10012);
                }
            }
            fn_80238D50(mSerial);
        }
    } else if (field_0x240 == 1) {
        field_0x231++;
        if (field_0x231 == 0xF) {
            field_0x23c++;
            field_0x231 = 0;
        }
        field_0x23c = field_0x23c & 3;
        if (__fabsf(field_0x3c.x) < 1.5f && __fabsf(field_0x3c.y) < 1.5f && __fabsf(field_0x3c.z) == 0.0f) {
            fn_80239914(&box, field_0x4, mSerial);
        } else {
            if (field_0x3c.x != 0.0f || field_0x3c.y != 0.0f) {
                startSound_801F0E34(0x30060, 0);
            }
            fn_801F2B7C(2, -1, 6);
        }
        u16 code = fn_8022461C(&mPos, field_0x4);
        if (code == 3 || code == 0xE || code == 2) {
            JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
            fn_800F82CC(&field_0x3c, &zero, 0.22f);
            if (field_0x3c.x == 0.0f && field_0x3c.y == 0.0f) {
                getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3CF, 0, 0, NULL, NULL, true, 1.2f);
                startSound_801F0E34(0x4002C, 0);
                fn_801F0D20(3);
                field_0x240 = 2;
                field_0x232 = -1;
                for (s32 i = 0; i < 8; i++) {
                    JGeometry::TVec3f p = mPos;
                    p.x += (f32)(0x12 - (s32)(32.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
                    p.y += (f32)(0x12 - (s32)(32.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, field_0x4, &p, 3, -1, -1);
                }
            }
        }
        if (fn_801F2618(NULL, false)) {
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 3.0f;
            field_0x233 = field_0x232;
            field_0x232 = -1;
            if (anyPlayer == true) {
                getGameView_802D6A9C()->fn_802D79E8(field_0x4, 2.0f, 0.0f, false);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3CF, 0, 0, NULL, NULL, true, 1.2f);
                startSound_801F0E34(0x4002C, 0);
            }
        }
        if (field_0x232 == -1 && field_0x3c.z == 0.0f) {
            if (field_0x234 == 1) {
                switch (field_0x233) {
                case 0:
                    field_0x232 = 3;
                    break;
                case 1:
                    field_0x232 = 2;
                    break;
                case 2:
                    field_0x232 = 0;
                    break;
                case 3:
                    field_0x232 = 1;
                    break;
                }
            } else {
                switch (field_0x233) {
                case 0:
                    field_0x232 = 2;
                    break;
                case 1:
                    field_0x232 = 3;
                    break;
                case 2:
                    field_0x232 = 1;
                    break;
                case 3:
                    field_0x232 = 0;
                    break;
                }
            }
        }
    } else {
        for (s32 i = 0; i < 4; i++) {
            if (getPlayerLayer_8023DE58(i) == field_0x4) {
                f32 x0 = mBox.i.x + mPos.x;
                f32 y0 = mBox.i.y + mPos.y;
                f32 x1 = mBox.f.x + mPos.x;
                f32 y1 = mBox.f.y + mPos.y;
                for (s32 j = 0; j < 8; j++) {
                    JGeometry::TVec3f p;
                    fn_8022E088(i, j, &p);
                    bool inside = false;
                    if (p.x >= x0 && p.y >= y0 && p.x < x1 && p.y < y1) {
                        inside = true;
                    }
                    if (inside) {
                        fn_8022DF48(i);
                        break;
                    }
                }
            }
        }
        return;
    }
    switch (field_0x232) {
    case 0:
        field_0x3c.x += 0.1f;
        if (field_0x3c.x >= 2.0f) {
            field_0x3c.x = 2.0f;
        }
        break;
    case 1:
        field_0x3c.x -= 0.1f;
        if (field_0x3c.x <= -2.0f) {
            field_0x3c.x = -2.0f;
        }
        break;
    case 2:
        field_0x3c.y += 0.1f;
        if (field_0x3c.y >= 2.0f) {
            field_0x3c.y = 2.0f;
        }
        break;
    case 3:
        field_0x3c.y -= 0.1f;
        if (field_0x3c.y <= -2.0f) {
            field_0x3c.y = -2.0f;
        }
        break;
    }
    field_0x3c.z -= 0.3f;
    mPos += field_0x3c;
    if (mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        field_0x3c.z = 0.0f;
    }
}

// FIXME(shotgun): 97 %, missing/extra + reloc-name
void Boulder_8049C4C0::update() {
    switch (field_0x230) {
    case 0:
        fn_800F5E08();
        break;
    case 1:
        if (field_0x3c.x != 0.0f || field_0x3c.y != 0.0f || field_0x3c.z != 0.0f) {
            fn_801F2B7C(2, -1, 6);
            field_0x3c.z -= 0.2f;
        } else {
            field_0x23c = 0;
        }
        mPos += field_0x3c;
        if (mPos.z <= 0.0f) {
            startSound_801F0E34(0x30075, 0);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x76, 0, 0, NULL, NULL, true, 0.8f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x77, 0, 0, NULL, NULL, true, 0.8f);
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
            mAlive = false;
        }
        break;
    case 2:
        fn_801F2B7C(1, -1, 1);
        field_0x3c.z -= 0.2f;
        mPos += field_0x3c;
        if (mPos.z <= 0.0f) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x76, 0, 0, NULL, NULL, true, 0.2f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x77, 0, 0, NULL, NULL, true, 0.2f);
            mAlive = false;
        }
        break;
    case 3:
        fn_800F555C();
        break;
    case 5:
        field_0x3c.z -= 0.03f;
        field_0x244 += field_0x248;
        mPos += field_0x3c;
        if (mPos.z <= 0.0f) {
            f32 scale = 1.0f;
            if (field_0x240 > 1) {
                scale = 0.5f;
            }
            startSound_801F0E34(0x3005A, 0);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x497, 0, 0, NULL, NULL, true, scale);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x498, 0, 0, NULL, NULL, true, scale);
            mAlive = false;
        }
        break;
    }
}

// FIXME(shotgun): 92 %, scheduling+regalloc
bool Boulder_8049C4C0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 code = fn_8022461C(pos, layer);
    if (code == 3 || code == 0xE || code == 2) {
        return false;
    }
    return fn_802265DC(pos, layer, false);
}

// FIXME(shotgun): 94.68 %, different + reloc-name
void Boulder_8049C4C0::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    s32 flags;
    params.field_0x1c = 1;
    params.field_0x1f = 0;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    switch (field_0x230) {
    case 0:
        mtx[0][0] = 1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.0f;
        flags = 3;
        if (field_0x240 == 1) {
            params.field_0x1c = 0;
            params.field_0x1d = -1;
            params.field_0x10.r = 200;
            params.field_0x10.g = 200;
            params.field_0x10.b = 200;
            flags = -1;
        } else if (field_0x240 == 2) {
            params.field_0x1c = 0;
            params.field_0x1d = -1;
            flags = -1;
            f32 scale = (f32)(0x3C - field_0x248) / 60.0f;
            mtx[0][0] = scale;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = scale;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = scale;
            params.field_0x10.r = 200 - field_0x248 * 2;
            params.field_0x10.g = 200 - field_0x248 * 2;
            params.field_0x10.b = 200 - field_0x248 * 2;
        }
        break;
    case 1:
        mtx[0][0] = 0.8f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 0.8f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 0.8f;
        if (field_0x3c.x != 0.0f || field_0x3c.y != 0.0f || field_0x3c.z != 0.0f) {
            flags = 3;
        } else {
            flags = -1;
        }
        break;
    case 2:
        mtx[0][0] = 0.2f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 0.2f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 0.2f;
        flags = 7;
        params.field_0x30 = 0x11;
        break;
    case 3:
        mtx[0][0] = 1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.0f;
        flags = 3;
        if (field_0x240 == 2) {
            params.field_0x1d = -1;
            params.field_0x1c = -1;
        }
        break;
    case 5: {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        setRotScaleMtx(mtx, JGeometry::TVec3f(0.0f, (f32)field_0x244 / -10430.228f, 0.0f), 1.0f);
        params.mMtx = mtx;
        void* res = fn_804032E8('TIMG', lbl_8049C458[field_0x240]);
        fn_801EFD34(res, &params, -1, 0.0f, 0.0f, 0.0f);
        return;
    }
    }
    params.mMtx = mtx;
    JGeometry::TVec3f savedPos = mPos;
    mPos.x += field_0x254.x;
    mPos.y += field_0x254.y;
    mPos.z += field_0x254.z;
    if (field_0x4 <= 8) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23c + 0xB1, &params, flags, 1.0f, 1.0f);
    } else {
        fn_801F02BC("enemy_both_sob.bin", field_0x23c, &params, flags, 0, false, 1.0f, 1.0f);
    }
    mPos.set(savedPos);
}

// FIXME(shotgun): 99 %, reloc-name
void BoulderSpawner_8049C46C::init() {
    field_0x230 = field_0x90;
    if (field_0x230 == 2) {
        field_0x234 = 0;
    } else if (field_0x230 == 4) {
        field_0x234 = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    } else {
        field_0x234 = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    }
}

// FIXME(shotgun): 97.47 %, missing/extra
void BoulderSpawner_8049C46C::update() {
    if (field_0x234 == 0) {
        switch (field_0x230) {
        case 0:
            if ((field_0x90 & 0xF8000000) != 0 && !vfunc_0x34()) {
                return;
            }
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &mPos, 0, -1, -1);
            field_0x234 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x96;
            break;
        case 1: {
            if ((field_0x90 & 0xF8000000) != 0 && !vfunc_0x34()) {
                return;
            }
            JGeometry::TVec3f pos = mPos;
            pos.z += 200.0f;
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &pos, 1, -1, -1);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            actor->field_0x3c.set(0.0f, -0.01f, 0.0f);
            field_0x234 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x96;
            break;
        }
        case 2:
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &mPos, 3, -1, -1);
            mAlive = false;
            return;
        case 3:
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &mPos, (field_0x90 & ~0xFF) + 4, -1, -1);
            field_0x234 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x96;
            break;
        case 4: {
            if ((field_0x90 & 0xF8000000) != 0 && !vfunc_0x34()) {
                return;
            }
            JGeometry::TVec3f pos;
            pos.x = mPos.x;
            pos.y = mPos.y;
            pos.z = mPos.z + 200.0f;
            pos.x += (f32)(40 - (s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
            pos.y += (f32)(20 - (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &pos, 5, -1, -1);
            field_0x234 = (s32)(150.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x50;
            break;
        }
        }
    }
    field_0x234--;
}

void BoulderSpawner_8049C46C::draw() {}

BoulderSpawner_8049C46C::~BoulderSpawner_8049C46C() {}
