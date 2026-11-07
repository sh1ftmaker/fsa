#include "game/GameAudio.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/SpriteParams.h"
#include "game/EnemyBase.h"
#include "game/Effect.h"
#include "game/GameRootSub20.h"
#include "game/Player.h"
#include "game/act/act_shadow_link_demo.h"


// Values that vfunc_0x54 compares the object's field_0x8 with, by the index it
// passes to fn_8011B878. Entries 0, 5, 13, 21-26, 30 and 34 are never compared.
static const s32 lbl_80464AF8[35] = {
    0x00000000, 0x0000083E, 0x0000087A, 0x000008DE, 0x00000901, 0x00000956, 0x00000A8C,
    0x00000AF0, 0x00000E1A, 0x00000E56, 0x00000E92, 0x00000ECE, 0x00000F0A, 0x00000000,
    0x0000005A, 0x000000D2, 0x0000014A, 0x00000384, 0x00000479, 0x000004D3, 0x0000050F,
    0x00000573, 0x00000573, 0x00000573, 0x00000573, 0x00000000, 0x00000000, 0x00000000,
    0x00000226, 0x00000280, 0x00000280, 0x000002F8, 0x00000320, 0x00000384, 0x00000000,
};

cls_8049DB90::cls_8049DB90() {
    field_0x230 = 1;
    field_0x231 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0.0f;
    field_0x248 = 0.0f;
    field_0x254 = 0.0f;
    field_0x250 = 0.0f;
    field_0x25c = 0.0f;
    field_0x258 = 0.0f;
}

// FIXME(shotgun): 99.9 %, reloc-name when compiled, but then __ct__ drops from 100 % to 99.8 % (the .sdata2 literal pool gains 80.0f), so it stays out of the build.
#ifdef NONMATCHING
void cls_8049DB90::init() {
    cls_8049D140::init();
    field_0x234 = field_0x90 & 0xF;
    field_0x98 = (s8)((field_0x90 >> 4) & 3);
    switch (field_0x234) {
    case 1:
        fn_8011B878(0xD);
        field_0x98 = 3;
        mSequencer.fn_800FEB78(0);
        break;
    case 2:
        fn_8011B878(0x16);
        field_0x98 = 3;
        mSequencer.fn_800FEB78(1);
        mPos.y += 80.0f;
        break;
    default:
        mSequencer.fn_800FEB78(0);
        break;
    }
    mSequencer.mDir = field_0x98;
}
#endif

#ifdef NONMATCHING
// The hop that update's states share: field_0x3c gets (vx, vy, base - 0.2 * t);
// once t > 1 and the actor is back at z <= 0, it stops and the hop is over.
static inline bool hop(cls_8049DB90* self, s32 t, f32 base, f32 vy, f32 vx) {
    self->field_0x3c.z = base - 0.2f * t;
    self->field_0x3c.y = vy;
    self->field_0x3c.x = vx;
    if (t > 1 && self->mPos.z <= 0.0f) {
        self->mPos.z = self->field_0x3c.x = self->field_0x3c.y = self->field_0x3c.z = 0.0f;
        return true;
    }
    return false;
}
#endif

// FIXME(shotgun): 95.29 %, missing/extra + reloc-name (gated: its literals drop __ct__ from 100 % to 99.83 %)
#ifdef NONMATCHING
void cls_8049DB90::update() {
    cls_8049D140::update();
    if (field_0x238 >= 2 && field_0x238 <= 11) {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        fn_8011B3F0();
    }
    switch (field_0x238) {
    case 0:
        mDrawEnabled = false;
        break;
    case 4:
        if (hop(this, fn_8011921C()->field_0x8 - lbl_80464AF8[4], 4.5f, 0.5f, 0.0f)) {
            fn_8011B878(5);
        }
        break;
    case 9: {
        s32 t = fn_8011921C()->field_0x8 - lbl_80464AF8[9];
        field_0x3c.z = 4.5f - 0.2f * t;
        field_0x3c.y = -0.5f;
        if (t > 1 && mPos.z <= 0.0f) {
            mPos.z = field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        }
        break;
    }
    case 11:
        // FIXME(shotgun): the original passes 0xFF as fn_802303FC's third argument (li r5, 0xff).
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos(*getPlayerPos_8023E724(i));
            pos.add(field_0x26c);
            fn_802303FC(i, &pos, true);
        }
        break;
    case 16:
        mPos.y -= 0.3f;
        if (mPos.y <= mSpawnPos.y - 64.0f) {
            mPos.y = mSpawnPos.y - 64.0f;
        }
        break;
    case 19:
        hop(this, fn_8011921C()->field_0x8 - lbl_80464AF8[19], 2.5f, 0.0f, 0.0f);
        break;
    case 20:
        mPos.y -= 1.2f;
        if (mPos.y <= mSpawnPos.y - 282.0f) {
            mPos.y = mSpawnPos.y - 282.0f;
            fn_8011B878(23);
        } else if (fn_8011921C()->field_0x8 >= 0x52D) {
            JGeometry::TVec3f pos(*getPlayerPos_8023E724(0));
            pos.y -= 1.6f;
            for (s32 i = 0; i < 4; i++) {
                fn_802303FC(i, &pos, true);
            }
        }
        break;
    case 22:
        if (fn_8011921C()->field_0x0 == 0) {
            fn_8011921C()->fn_801186B0(0x12);
            fn_8011921C()->field_0x2 = 1;
        }
        mPos.y -= 1.2f;
        if (mPos.y <= mSpawnPos.y) {
            mPos.y = mSpawnPos.y;
            fn_8011B878(23);
        } else {
            JGeometry::TVec3f pos(mPos);
            pos.y += 32.0f;
            for (s32 i = 0; i < 4; i++) {
                fn_802303FC(i, &pos, true);
            }
        }
        break;
    case 23:
        field_0x23c++;
        if (hop(this, field_0x23c, 4.5f, -0.8f, 1.45f)) {
            fn_8011B878(24);
        }
        break;
    case 24:
        startSound_801F0E34(0x400C7, 0);
        fn_8011B878(25);
        break;
    case 25:
        field_0x23c++;
        if (hop(this, field_0x23c, 2.5f, 0.0f, 0.0f)) {
            field_0x23c = 0;
        }
        if (fn_801F2B7C(0, -1, 1)) {
            fn_8011B878(26);
        }
        break;
    case 26: {
        f32 vx = field_0x231 ? -2.9f : 2.9f;
        field_0x23c++;
        if (hop(this, field_0x23c, 4.5f, 0.0f, vx)) {
            fn_8011B878(25);
            if (field_0x231) {
                mPos.x = mSpawnPos.x - 64.0f;
            } else {
                mPos.x = 64.0f + mSpawnPos.x;
            }
        }
        break;
    }
    case 28:
        if (field_0x240 < 3) {
            field_0x23c++;
            if (hop(this, field_0x23c, 2.5f, 0.0f, 0.0f)) {
                field_0x23c = 0;
                field_0x240++;
            }
        }
        break;
    case 29: {
        f32 vx = field_0x231 ? 1.45f : -1.45f;
        if (hop(this, fn_8011921C()->field_0x8 - lbl_80464AF8[29], 4.5f, 1.1f, vx)) {
            fn_8011B878(30);
        }
        break;
    }
    case 32:
        mPos.y += 1.2f;
        if (mPos.y >= 300.0f + mSpawnPos.y) {
            mPos.y = 300.0f + mSpawnPos.y;
        }
        break;
    }
    mPos.add(field_0x3c);
    mSequencer.mDir = field_0x98;
    if (field_0x238 == 16) {
        if (field_0x230) {
            mSequencer.fn_800FE8C0();
            field_0x230 = 0;
        } else {
            field_0x230 = 1;
        }
    } else {
        mSequencer.fn_800FE8C0();
    }
}
#endif

// FIXME(shotgun): 94.83 %, missing/extra (gated: its literals drop __ct__ from 100 % to 99.83 %)
#ifdef NONMATCHING
void cls_8049DB90::draw() {
    cls_8049D140::draw();
    SpriteParams_801F5AE0 params;
    params.field_0x2e = 0x40;
    s32 frame = mSequencer.mFrame;
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, &params, 7, "player_sob.bin", frame, NULL);
    }
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, NULL, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, mSequencer.mFrame, 0, 1, -1, false, 0, 0);
}
#endif

void cls_8049DB90::vfunc_0x54() {
    s32 value = fn_8011921C()->field_0x8;
    switch (field_0x234) {
    case 0:
        if (value == lbl_80464AF8[1]) {
            fn_8011B878(1);
        } else if (value == lbl_80464AF8[2]) {
            fn_8011B878(2);
        } else if (value == lbl_80464AF8[3]) {
            fn_8011B878(3);
        } else if (value == lbl_80464AF8[4]) {
            fn_8011B878(4);
        } else if (value == lbl_80464AF8[6]) {
            fn_8011B878(6);
        } else if (value == lbl_80464AF8[7]) {
            fn_8011B878(7);
        } else if (value == lbl_80464AF8[8]) {
            fn_8011B878(8);
        } else if (value == lbl_80464AF8[9]) {
            fn_8011B878(9);
        } else if (value == lbl_80464AF8[10]) {
            fn_8011B878(10);
        } else if (value == lbl_80464AF8[11]) {
            fn_8011B878(11);
        } else if (value == lbl_80464AF8[12]) {
            fn_8011B878(12);
        }
        break;
    case 1:
        if (fn_8011921C()->field_0x4 == 0x11) {
            if (value == lbl_80464AF8[14]) {
                fn_8011B878(14);
            } else if (value == lbl_80464AF8[15]) {
                fn_8011B878(15);
            } else if (value == lbl_80464AF8[16]) {
                fn_8011B878(16);
            } else if (value == lbl_80464AF8[17]) {
                fn_8011B878(17);
            } else if (value == lbl_80464AF8[18]) {
                fn_8011B878(18);
            } else if (value == lbl_80464AF8[19]) {
                fn_8011B878(19);
            } else if (value == lbl_80464AF8[20]) {
                fn_8011B878(20);
            }
        } else if (fn_8011921C()->field_0x4 == 2) {
            if (value == lbl_80464AF8[27]) {
                fn_8011B878(27);
            } else if (value == lbl_80464AF8[28]) {
                fn_8011B878(28);
            } else if (value == lbl_80464AF8[29]) {
                fn_8011B878(29);
            } else if (value == lbl_80464AF8[31]) {
                fn_8011B878(31);
            } else if (value == lbl_80464AF8[32]) {
                fn_8011B878(32);
            } else if (value == lbl_80464AF8[33]) {
                fn_8011B878(33);
            }
        }
        break;
    case 2: // nothing sets state 2; the original's switch has an entry for it
        break;
    }
}

s32 cls_8049DB90::vfunc_0x58() {
    return field_0x238;
}

s32 cls_8049DB90::vfunc_0x5C() {
    switch (field_0x238) {
    case 12:
    case 21:
    case 24:
    case 33:
    case 34:
        return 1;
    }
    return 0;
}

// FIXME(shotgun): 96.3 %, missing/extra + reloc-name (gated: its literals drop __ct__ from 100 % to 99.83 %)
#ifdef NONMATCHING
void cls_8049DB90::fn_8011B878(s32 index) {
    switch (index) {
    case 0:
        mDrawEnabled = false;
        break;
    case 1:
        vfunc_0x30(0xFF, true);
        break;
    case 4: {
        GameRootSub20_801902C0* sub = fn_801902C0();
        sub->field_0x7c0 = 1;
        sub->field_0x7bc = 1;
        startSound_801F0E34(0x40035, 0);
        mSequencer.fn_800FEB78(4);
        mDrawEnabled = true;
        break;
    }
    case 5:
        mSequencer.fn_800FEB78(0);
        break;
    case 6:
        startSound_801F0E34(0x400C7, 0);
        break;
    case 9:
        mSequencer.fn_800FEB78(4);
        break;
    case 10: {
        vfunc_0x30(0xFF, false);
        mDrawEnabled = false;
        JGeometry::TVec3f pos(mSpawnPos);
        pos.y += 128.0f;
        for (s32 i = 0; i < 4; i++) {
            fn_80230498(i, &pos, 0x18, 0xFF, -0x80, -0x80, true);
        }
        break;
    }
    case 11: {
        for (s32 i = 0; i < 4; i++) {
            fn_80230BF4(i, 3);
        }
        field_0x260 = mSpawnPos;
        JGeometry::TVec3f d = mSpawnPos - *getPlayerPos_8023E724(0);
        field_0x26c.set(d.x / 60.0f, d.y / 60.0f, d.z / 60.0f);
        break;
    }
    case 13:
        mDrawEnabled = false;
        break;
    case 14: {
        JGeometry::TVec3f pos(mPos);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x41D, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x41E, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x41F, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x420, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30091, 0);
        break;
    }
    case 15:
        mDrawEnabled = true;
        break;
    case 16:
        field_0x98 = 3;
        mSequencer.mState = 1;
        mSequencer.mDir = 3;
        break;
    case 17:
        field_0x98 = 2;
        mSequencer.mState = 0;
        mSequencer.mDir = 2;
        break;
    case 18: {
        field_0x98 = 2;
        mSequencer.mState = 0;
        mSequencer.mDir = 2;
        JGeometry::TVec3f pos(mPos);
        pos.y += -8.0f;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 0, NULL, NULL, true, 1.0f);
        break;
    }
    case 20:
        field_0x98 = 3;
        mSequencer.fn_800FEB78(1);
        break;
    case 21:
        mSequencer.fn_800FEB78(0);
        break;
    case 22:
        field_0x98 = 3;
        mSequencer.fn_800FEB78(1);
        field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        break;
    case 23:
        mSequencer.fn_800FEB78(4);
        field_0x23c = 0;
        for (s32 i = 0; i < 4; i++) {
            fn_80230498(i, getPlayerPos_8023E724(i), 0x3F, 0xFF, -0x80, -0x80, true);
        }
        break;
    case 24:
        field_0x98 = 2;
        mSequencer.fn_800FEB78(0);
        field_0x23c = 0;
        break;
    case 25:
        mSequencer.fn_800FEB78(0);
        field_0x23c = 0;
        break;
    case 26:
        mSequencer.fn_800FEB78(4);
        field_0x231 = !field_0x231;
        field_0x23c = 0;
        mPos.z = 0.0f;
        startSound_801F0E34(0x400C7, 0);
        break;
    case 27:
        mSequencer.fn_800FEB78(0);
        mPos.z = field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        break;
    case 28:
        startSound_801F0E34(0x400C7, 0);
        break;
    case 29:
        mSequencer.fn_800FEB78(4);
        mPos.z = 0.0f;
        break;
    case 30:
        field_0x98 = 3;
        mSequencer.fn_800FEB78(0);
        break;
    case 31:
        startSound_801F0E34(0x400C7, 0);
        break;
    case 32:
        field_0x98 = 2;
        mSequencer.fn_800FEB78(1);
        break;
    case 33:
    case 34:
        break;
    }
    field_0x238 = index;
}
#endif

// Pushes the 4-float box at 0x250 to the scripted camera (mode 5).
static inline void setCameraBox(cls_8049DB90* self) {
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    if (cam->field_0x7f5 == 0) {
        cam->field_0x7f4 = 1;
        cam->field_0x7f8 = 5;
        cam->field_0x808.set(self->field_0x250, self->field_0x254, self->field_0x258, self->field_0x25c);
        cam->field_0x7f6 = 0;
    }
}

// FIXME(shotgun): 5.55 %, missing/extra (gated: its literals drop __ct__ from 100 % to 99.83 %)
#ifdef NONMATCHING
void cls_8049DB90::fn_8011B3F0() {
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    if (cam->field_0x179 == 0) {
        cam->field_0x17a = 1;
        cam->field_0x179 = 0;
    }
    switch (field_0x238) {
    case 1:
    case 2:
    case 3:
        field_0x250 = mPos.x - 64.0f;
        field_0x254 = mPos.y - 32.0f;
        field_0x258 = 64.0f + mPos.x;
        field_0x25c = 32.0f + mPos.y;
        setCameraBox(this);
        break;
    case 4:
    case 5: {
        JGeometry::TVec3f p(mPos);
        p.y += field_0x248;
        fn_801CD950(&field_0x248, 32.0f, 0.8f);
        field_0x250 = p.x - 64.0f;
        field_0x254 = p.y - 32.0f;
        field_0x258 = 64.0f + p.x;
        field_0x25c = 32.0f + p.y;
        setCameraBox(this);
        break;
    }
    case 6: {
        JGeometry::TVec3f p(mPos);
        p.y += field_0x248;
        fn_801CD950(&field_0x248, 0.0f, 0.8f);
        field_0x250 = p.x - 64.0f;
        field_0x254 = p.y - 32.0f;
        field_0x258 = 64.0f + p.x;
        field_0x25c = 32.0f + p.y;
        setCameraBox(this);
        break;
    }
    case 7: {
        s32 diff = fn_8011921C()->field_0x8 - lbl_80464AF8[7];
        if (diff >= 0xB4) {
            if (diff > 0x19A && diff != 0) {
                fn_801CD950(&field_0x244, 0.0f, -0.007f);
            } else {
                fn_801CD950(&field_0x244, 0.1f, 0.0035f);
            }
            fn_801CD950(&field_0x254, (mPos.y - 32.0f) - 24.0f, -field_0x244);
            fn_801CD950(&field_0x25c, (32.0f + mPos.y) + 24.0f, field_0x244);
        }
        setCameraBox(this);
        break;
    }
    case 8: {
        Camera_804AA9E4* c = gGameRoot2_8053AB10->field_0x24;
        if (c->field_0x7f5 == 0) {
            c->field_0x7f4 = 1;
            c->field_0x7f8 = 4;
            c->field_0x7fc.set(mPos.x, mPos.y, mPos.z);
            c->field_0x7f6 = 0;
        }
        break;
    }
    case 9:
    case 10:
    case 11: {
        Camera_804AA9E4* c = gGameRoot2_8053AB10->field_0x24;
        if (c->field_0x840 == 0) {
            c->field_0x840 = 1;
            c->field_0x844.set(mPos.x, mPos.y, mPos.x, mPos.y);
            c->field_0x854 = 1.0f;
            c->field_0x1d4 = -1.0f;
            c->field_0x1d8 = -1.0f;
        }
        break;
    }
    default:
        setCameraBox(this);
        break;
    }
}
#endif

cls_8049DB90::~cls_8049DB90() {}
