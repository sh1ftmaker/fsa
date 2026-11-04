#include "game/GameAudio.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/GameView.h"
#include "game/act/tu_80376FA0.h"

static const f32 lbl_80475148[4][3] = {
    {0.2f, 0.0f, 0.0f},
    {-0.2f, 0.0f, 0.0f},
    {0.0f, 0.2f, 0.0f},
    {0.0f, -0.2f, 0.0f},
};
static const f32 lbl_80475178[4] = {-24.0f, -8.0f, 8.0f, 24.0f};

// Grows (d > 0) or shrinks (d < 0) a box by d on every side.
static inline void inflate(JGeometry::TBox2f& box, f32 d) {
    box.i.x -= d;
    box.i.y -= d;
    box.f.x += d;
    box.f.y += d;
}

cls_804AE7D8::~cls_804AE7D8() {}

// FIXME: 99.81 %. The two names are .rodata string literals in the original
// (lbl_80475188, lbl_80475198); ours are literals in the .o, so only their
// relocation names differ. Code is identical.
void cls_804AE7D8::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1c = 0;
    params.field_0x1d = -1;

    switch (field_0x244) {
    case 0: {
        void* res = fn_804032E8('TIMG', "yy_1_1_iwa1.bti");
        if (field_0x4 < 8) {
            lbl_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
        }
        break;
    }
    case 1: {
        void* res = fn_804032E8('TIMG', "yy_1_1_iwa2.bti");
        if (field_0x4 < 8) {
            lbl_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
        }
        break;
    }
    }
}

// FIXME: 15.9 %. The box copy from mBox keeps its four floats in f3, f2, f1, f4
// in the original and adds mPos to them there; ours reloads them, so the
// adds and stores are scheduled and register-allocated differently. The
// state dispatch also lays out its blocks differently (the state 1 block
// starts at another offset), so every later hunk shifts.
void cls_804AE7D8::update() {
    // Stack slots: the box (highest), then the position and the probe
    // result, as the original's frame lays them out.
    JGeometry::TBox2f box;
    JGeometry::TVec3f pos;
    JGeometry::TVec3f tmp;

    box.i.x = mBox.i.x;
    box.i.y = mBox.i.y;
    box.f.x = mBox.f.x;
    box.f.y = mBox.f.y;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;

    switch (field_0x244) {
    case 0: {
        inflate(box, 1.0f);
        fn_80239914(&box, field_0x4, mSerial);
        inflate(box, -1.0f);
        fn_8020D58C()->addBox(mSerial);
        if (field_0x264 != 0) {
            return;
        }

        f32 margin;
        f32 spawnDist = PSVECDistance(&mPos, &mSpawnPos);
        if (spawnDist < 12.0f) {
            margin = 11.9f;
        } else if (spawnDist >= 44.1f) {
            margin = 15.9f;
        } else {
            margin = 19.9f;
        }

        if (PSVECDistance(&mPos, &field_0x248) >= margin) {
            // Arrived: move by field_0x3c, count down field_0x254, and when
            // the offset is zero stop the emitter.
            mPos += field_0x3c;
            if (field_0x254 != 0) {
                if (field_0x254 > 0) {
                    field_0x254--;
                }
                return;
            }
            if (field_0x3c.isZero()) {
                if (mEmitter230 != NULL) {
                    fn_8013CB44(mEmitter230, &mPos, 0, 0, true);
                }
                return;
            }
            if (fn_80239108(mSerial) < 4) {
                return;
            }
            if (PSVECDistance(&mPos, &mSpawnPos) >= 63.9f) {
                return;
            }
            if (field_0x260 < 10) {
                field_0x260++;
                return;
            }
            s32 index = fn_80238BC4(mSerial);
            if (field_0x258 == 0) {
                field_0x258 = index + 1;
            } else if (index + 1 != field_0x258) {
                return;
            }
            if (index < 0 || index >= 4) {
                return;
            }
            fn_80238E08(mSerial);
            field_0x3c.x = lbl_80475148[index][0];
            field_0x3c.y = lbl_80475148[index][1];
            field_0x3c.z = lbl_80475148[index][2];
            field_0x248 = mPos;
            fn_801F0E34(0x30004, 0);
            mPos += field_0x3c;
            if (index == 0) {
                mEmitter230 = fn_8013CC50(field_0x4, &mPos, 0x31a, 1, 1, NULL, NULL, true, 1.0f);
            } else if (index == 1) {
                mEmitter230 = fn_8013CC50(field_0x4, &mPos, 0x31b, 1, 1, NULL, NULL, true, 1.0f);
            } else if (index == 2) {
                mEmitter230 = fn_8013CC50(field_0x4, &mPos, 0x31c, 1, 1, NULL, NULL, true, 1.0f);
            } else {
                mEmitter230 = fn_8013CC50(field_0x4, &mPos, 0x319, 1, 1, NULL, NULL, true, 1.0f);
            }
            return;
        }

        // Still on the way: approach the spawn point's box on x and y.
        f32 sx = mSpawnPos.x;
        if (mPos.x > sx + 11.5f && mPos.x < sx - 11.5f) {
            mPos.x = sx;
        } else {
            mPos.x = fn_8033D4AC(mPos.x, field_0x248.x, margin);
        }
        f32 sy = mSpawnPos.y;
        if (mPos.y > sy + 11.5f && mPos.y < sy - 11.5f) {
            mPos.y = sy;
        } else {
            mPos.y = fn_8033D4AC(mPos.y, field_0x248.y, margin);
        }

        field_0x248 = mPos;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        if (mEmitter230 != NULL) {
            fn_801EE434(mEmitter230);
            mEmitter230 = NULL;
        }
        field_0x254 = 8;
        mPrevPos = mPos;
        if (PSVECDistance(&mPos, &mSpawnPos) >= 63.9) {
            return;
        }
        vfunc_0x30(0xFF, true);

        // The same offset loop as init().
        f32 x = mPos.x;
        x -= 24.0f;
        f32 y = mPos.y;
        f32 z = mPos.z;
        s32 j;
        for (s32 i = 0; i < 4; i++) {
            pos.x = x;
            pos.y = y;
            pos.z = z;
            const f32* offset = lbl_80475178;
            for (j = 0; j < 4; j++) {
                pos.y = y + *offset;
                lbl_8053AC98->fn_802A3948(&pos, 0, field_0x4, 0);
                offset++;
            }
            x += 16.0f;
        }
        field_0x244 = 1;
        field_0x25c = 50;
        field_0x260 = 0;

        if (mEmitter230 == NULL) {
            mEmitter230 = fn_8013CC50(field_0x4, &mPos, 0x542, 0, 0, NULL, NULL, true, 1.0f);
            fn_8013CB44(mEmitter230, &mPos, -1, -1, true);
        }
        if (mEmitter234 == NULL) {
            mEmitter234 = fn_8013CC50(field_0x4, &mPos, 0xb7, 0, 0, NULL, NULL, true, 1.0f);
            fn_8013CB44(mEmitter234, &mPos, -1, -1, true);
        }
        if (mEmitter238 == NULL) {
            mEmitter238 = fn_8013CC50(field_0x4, &mPos, 0xb8, 0, 0, NULL, NULL, true, 1.0f);
            fn_8013CB44(mEmitter238, &mPos, 0, 0, true);
        }
        if (mEmitter23c == NULL) {
            mEmitter23c = fn_8013CC50(field_0x4, &mPos, 0xb9, 0, 0, NULL, NULL, true, 1.0f);
            fn_8013CB44(mEmitter23c, &mPos, 0, 0, true);
        }
        if (mEmitter240 == NULL) {
            mEmitter240 = fn_8013CC50(field_0x4, &mPos, 0xba, 0, 0, NULL, NULL, true, 1.0f);
            fn_8013CB44(mEmitter240, &mPos, 0, 0, true);
        }
        fn_801F0E34(0x50065, 0);
        if (field_0x265 != 0) {
            return;
        }
        fn_802D6A9C()->fn_802D7584(field_0x16c, (u8)field_0x258);
        return;
    }
    case 1: {
        if (field_0x25c != 0) {
            field_0x25c--;
            if (field_0x25c == 0) {
                fn_801F0D20(3);
            }
        }
        for (s32 player = 0; player < 4; player++) {
            bool hit = false;
            if (fn_8023DE58(player) == field_0x4) {
                f32 left = mBox.i.x + mPos.x;
                f32 bottom = mBox.i.y + mPos.y;
                f32 right = mBox.f.x + mPos.x;
                f32 top = mBox.f.y + mPos.y;
                for (s32 k = 0; k < 8; k++) {
                    fn_8022E088(player, k, &tmp);
                    tmp.z = 0.0f;
                    if (tmp.x < left && tmp.y < bottom && tmp.x < right && tmp.y < top) {
                        hit = true;
                        break;
                    }
                }
            }
            if (hit) {
                fn_8022DF48(player);
            }
        }
        break;
    }
    }
}

// FIXME: 99.78 %. The sdata2 constants (24.0f, 16.0f, 0.0f, -31.5f, 31.5f)
// are literals in ours and lbl_80542F88 etc. in the original: only the
// relocation names differ. Code is identical.
void cls_804AE7D8::init() {
    field_0x244 = 0;
    field_0x264 = 0;
    field_0x258 = (u8)((u8)field_0x90 + 1);
    field_0x265 = (field_0x90 >> 16) & 1;
    if (field_0x265 == 0 && fn_8028D4DC(&mPos, 3, field_0x16c)) {
        field_0x244 = 1;
        f32 x = mPos.x;
        x -= 24.0f;
        f32 y = mPos.y;
        f32 z = mPos.z;
        // The inner counter is declared outside its loop: that gives the
        // original's register order (r29), not the loop-local one.
        s32 j;
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos;
            pos.x = x;
            pos.y = y;
            pos.z = z;
            const f32* offset = lbl_80475178;
            for (j = 0; j < 4; j++) {
                pos.y = y + *offset;
                lbl_8053AC98->fn_802A3948(&pos, 0, field_0x4, 0);
                offset++;
            }
            x += 16.0f;
        }
        field_0x264 = 1;
    } else {
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x248 = mPos;
        mBox.i.x = -31.5f;
        mBox.i.y = -31.5f;
        mBox.f.x = 31.5f;
        mBox.f.y = 31.5f;
        field_0x80.i.x = -31.5f;
        field_0x80.i.y = -31.5f;
        field_0x80.f.x = 31.5f;
        field_0x80.f.y = 31.5f;
        field_0x1a4 = 4;
        field_0x254 = 0;
        field_0xb8 = 2;
        field_0x25c = 0;
        field_0x260 = 0;
        mEmitter230 = NULL;
        mEmitter234 = NULL;
        mEmitter238 = NULL;
        mEmitter23c = NULL;
        mEmitter240 = NULL;
    }
}
