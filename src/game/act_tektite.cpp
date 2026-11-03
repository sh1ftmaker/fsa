#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Effect.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_tektite.h"

Tektite_8049FD50::~Tektite_8049FD50() {}

void Tektite_8049FD50::vfunc_0x24() {
    setField_0x18C(0);
    if (field_0x90 != 0) {
        fn_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, 3);
    } else {
        fn_801F856C()->fn_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
    }
    ActorBase_804A5770::vfunc_0x24();
}

// FIXME: 93.86%. The target loads the three scale components (z, y, x) before
// its first matrix store; we load each one just before its store (scheduling).
void Tektite_8049FD50::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x90 & 2) {
        return;
    }
    if (field_0x90 != 0) {
        mtx[0][0] = field_0x54.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x54.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x54.z;
        params.field_0x1f = 0;
        params.mMtx = mtx;
    }
    fn_801F06F0("enemy_gc_sob.bin", field_0x90 * 3 + field_0x250 + 0x49, &params, 7, 1.0f, 1.0f);
}

// FIXME: 99.23%. The target compares field_0x254 (in r0) with the tile (in r3)
// as `cmplw r0, r3`; we emit the operands the other way round.
void Tektite_8049FD50::update() {
    if (field_0x90 == 2 || field_0x90 == 3) {
        // fn_80226998 (declared with TVec3f in Map.h) reads only the x and y of
        // its position, so the 8-byte stack TVec2f is passed as a TVec3f.
        if (field_0x254 == 0xFFFF) {
            if (fn_802D6A9C()->fn_802DCD0C()) {
                return;
            }
            s32 layer = field_0x4;
            JGeometry::TVec2f p(mPos.x, mPos.y);
            field_0x254 = fn_80226998((const JGeometry::TVec3f*)&p, layer);
            return;
        }
        s32 layer = field_0x4;
        JGeometry::TVec2f p(mPos.x, mPos.y);
        u16 tile = fn_80226998((const JGeometry::TVec3f*)&p, layer);
        if (tile == field_0x254) {
            return;
        }
        field_0x90 &= 1;
    }
    if (fn_801F3D94(1)) {
        fn_801F2618(NULL, false);
        return;
    }
    if (field_0x230 == 0) {
        if (fn_801F305C()) {
            field_0x238 = 0;
            fn_801F2618(NULL, false);
            return;
        }
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
    fn_801F71A4(&field_0x3c.x, field_0x258.x, 0.3f, 1.0f);
    fn_801F71A4(&field_0x3c.y, field_0x258.y, 0.3f, 1.0f);
    mPos += field_0x3c;
    fn_801F2B7C(-100, -1, 1);
    bool hit = fn_801F2618(NULL, false);
    switch (field_0x230) {
    case 0:
        if (field_0x238 == 0) {
            u16 angle = 0;
            s32 dir = 0;
            if (!(field_0x90 & 1)) {
                field_0x3c.z = 6.0f;
            } else {
                field_0x3c.z = 12.0f;
            }
            if (lbl_8053AAF8->mRandom.get_ufloat_1() < 0.2f) {
                fn_801EEC98(mSerial, &angle, &dir, NULL, NULL, 0, 0);
            } else {
                angle = (s32)(65536.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
            }
            field_0x258.x = 2.0f * JMASinShort(angle);
            field_0x258.y = 2.0f * JMACosShort(angle);
            if (dir == -1) {
                field_0x258.set(0.0f, 0.0f, 0.0f);
            }
            field_0xb0 = 0;
            field_0x250 = 2;
            field_0x230 = 1;
        } else if (field_0x234 == 0) {
            field_0x250++;
            field_0x250 &= 1;
            field_0x234 = 8;
        }
        break;
    case 1:
        if (hit) {
            field_0x258.set(0.0f, 0.0f, 0.0f);
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
        }
        field_0x3c.z -= 0.5f;
        // Landed (mPos.z <= 0) and still falling (field_0x3c.z < 0).
        if (mPos.z <= 0.0f && field_0x3c.z < 0.0f) {
            JGeometry::TVec3f pos;
            pos.set(mPos);
            pos.y += 8.0f;
            fn_8013CC50(field_0x4, &pos, 0x3CF, 0, 0, NULL, NULL, true, 1.0f);
            field_0x234 = 16;
            field_0x250 = 1;
            field_0x258.set(0.0f, 0.0f, 0.0f);
            field_0x3c.z = 0.0f;
            mPos.z = 0.0f;
            field_0xb0 = 0x11101;
            field_0x238 = 50 + (s32)(50.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
            if (field_0x90 & 1) {
                if (mLife < 8) {
                    field_0x238 = 4 + (s32)(8.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
                }
            }
            field_0x230 = 0;
        }
        break;
    }
}

// FIXME: 95.70%. The 0xFFFF store is built with `lis r6, 1; subi r0, r6, 1` in
// the original and with r3 here (register choice only).
void Tektite_8049FD50::init() {
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x90 &= 7;
    if (!(field_0x90 & 1)) {
        mBox.i.x = -6.0f;
        mBox.i.y = -6.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 6.0f;
        field_0x80.i.x = -8.0f;
        field_0x80.i.y = -8.0f;
        field_0x80.f.x = 8.0f;
        field_0x80.f.y = 8.0f;
        mLife = 4;
    } else {
        field_0x54.set(1.2f, 1.2f, 1.2f);
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        field_0x80.i.x = -10.0f;
        field_0x80.i.y = -10.0f;
        field_0x80.f.x = 10.0f;
        field_0x80.f.y = 10.0f;
        mLife = 8;
    }
    field_0x258.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x254 = 0xFFFF;
    if (field_0x90 == 4 || field_0x90 == 5) {
        u16 angle = (s32)(65536.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
        s32 dir;
        fn_801EEC98(mSerial, &angle, &dir, NULL, NULL, 0, 0);
        if (dir >= 0) {
            angle += 0x8000;
        }
        field_0x258.x = 2.0f * JMASinShort(angle);
        field_0x258.y = 2.0f * JMACosShort(angle);
        field_0x3c.z = 6.0f;
        field_0x238 = 0;
        field_0x90 &= 1;
        field_0xb0 = 0;
        field_0x250 = 2;
        field_0x230 = 1;
    } else {
        field_0xb0 = 0x11101;
        field_0x238 = 0x14 + (s32)(20.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
        field_0x250 = 0;
        field_0x230 = 0;
    }
}
