#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "JSystem/JMath/JMath.h"
#include <math.h>
#include "game/act/act_trick_floor_2.h"

static const s32 lbl_8046F788[7] = {4, 4, 6, 4, 6, 6, 4};
static const s32 lbl_8046F7A4[7] = {4, 4, 5, 5, 6, 6, 4};
static const u16 lbl_8046F7C0[4][4] = {
    {0x01B0, 0x01B0, 0x01B0, 0x01B0},
    {0x01B0, 0x01D8, 0x01DA, 0x01B0},
    {0x01B0, 0x01D9, 0x01DB, 0x01B0},
    {0x01B0, 0x01B0, 0x01B0, 0x01B0},
};
static const f32 lbl_8046F7E0[7] = {32.0f, 32.0f, 48.0f, 32.0f, 64.0f, 64.0f, 32.0f};
static const f32 lbl_8046F7FC[7] = {32.0f, 32.0f, 40.0f, 40.0f, 64.0f, 64.0f, 32.0f};
static const f32 lbl_8046F818[7] = {1.0f, 1.0f, 1.5f, 1.0f, 1.0f, 1.0f, 1.0f};
static const f32 lbl_8046F834[7] = {1.0f, 1.0f, 1.3f, 1.3f, 1.0f, 1.0f, 1.0f};

// The rotation part of m from Euler angles (R = Rz Ry Rx), as setRotScaleMtx (game/RotMatrix.h)
// without the scale and with the cosines first (z, y, x, then the sines).
static inline void setRotMtx(Mtx m, const JGeometry::TVec3f& rot) {
    f32 cz = cos(rot.z);
    f32 cy = cos(rot.y);
    f32 cx = cos(rot.x);
    f32 sz = sin(rot.z);
    f32 sy = sin(rot.y);
    f32 sx = sin(rot.x);
    m[0][0] = cy * cz;
    m[1][0] = cy * sz;
    m[2][0] = -sy;
    m[0][1] = sx * sy * cz - cx * sz;
    m[1][1] = sx * sy * sz + cx * cz;
    m[2][1] = sx * cy;
    m[0][2] = cx * cz * sy + sx * sz;
    m[1][2] = cx * sz * sy - sx * cz;
    m[2][2] = cx * cy;
}

// Draws the 'SMP ' sprite `name` at pos with the floor's params, below layer 8.
static inline void drawSmp(TrickFloor2_804AA628* self, const JGeometry::TVec3f* pos, const char* name,
                           SpriteParams_801F5AE0* params) {
    if (self->field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', name);
        gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(pos, res, params);
    }
}

// The tile index of a view-relative position (as init computes it).
static inline s32 tileIndex(s32 tx, s32 ty) {
    s32 cx = tx < 0 ? 0 : (tx >= 0x200 ? 0x1FF : tx);
    s32 cy = ty < 0 ? 0 : (ty >= 0x200 ? 0x1FF : ty);
    return (cx & 0x100) | ((cy & 0x100) << 1) | (cy & 0xF0) | ((cx >> 4) & 0xF);
}

// Sets every tile of the floor's block (counts by mType) from x to `tile` (update's four
// copies; init's loop with tile 6).
static inline void fillTiles(TrickFloor2_804AA628* self, f32 x, u16 tile) {
    s32 layer = self->field_0x4;
    const JGeometry::TBox2f& view = layer < 8 ? getGameView_802D6A9C()->mVisibleBox
                                              : getGameView_802D6A9C()->field_0xdc;
    JGeometry::TBox2f box = view;
    for (s32 i = 0; i < lbl_8046F788[self->mType]; i++) {
        s32 tx = (s32)(x - box.i.x);
        f32 y = self->mPos.y;
        for (s32 j = 0; j < lbl_8046F7A4[self->mType]; j++) {
            s32 ty = (s32)(y - box.i.y);
            gGameRoot2_8053AB10->field_0x20->fn_802F9974(self->field_0x4, tileIndex(tx, ty), tile);
            y += 16.0f;
        }
        x += 16.0f;
    }
}

// The four dust emitters 0x381-0x384 at the floor's corner.
static inline void spawnDust(TrickFloor2_804AA628* self, s32 a) {
    {
        JGeometry::TVec3f pos(32.0f + self->mPos.x, 32.0f + self->mPos.y, 0.0f);
        spawnEmitter_8013CC50(self->field_0x4, &pos, 0x381, a, 0, NULL, NULL, true, 1.0f);
    }
    {
        JGeometry::TVec3f pos(32.0f + self->mPos.x, 32.0f + self->mPos.y, 0.0f);
        spawnEmitter_8013CC50(self->field_0x4, &pos, 0x382, a, 0, NULL, NULL, true, 1.0f);
    }
    {
        JGeometry::TVec3f pos(32.0f + self->mPos.x, 32.0f + self->mPos.y, 0.0f);
        spawnEmitter_8013CC50(self->field_0x4, &pos, 0x383, a, 0, NULL, NULL, true, 1.0f);
    }
    {
        JGeometry::TVec3f pos(32.0f + self->mPos.x, 32.0f + self->mPos.y, 0.0f);
        spawnEmitter_8013CC50(self->field_0x4, &pos, 0x384, a, 0, NULL, NULL, true, 1.0f);
    }
}

// FIXME(shotgun): 85.4 %, missing/extra + reloc-name
void TrickFloor2_804AA628::init() {
    mType = field_0x90 & 0xF;
    field_0x238 = (field_0x90 >> 4) & 0xF;
    field_0x240 = (field_0x90 >> 12) & 0xFFF;
    field_0x248 = 0.0f;
    switch (mType) {
    case 0:
    case 1:
    case 6:
        mBox.set(-32.0f, -32.0f, 32.0f, 32.0f);
        break;
    case 2:
        mBox.set(-40.0f, -32.0f, 40.0f, 32.0f);
        break;
    case 3:
        mBox.set(-24.0f, -32.0f, 24.0f, 32.0f);
        break;
    case 4:
    case 5:
        mBox.set(-16.0f, -50.0f, 24.0f, 50.0f);
        field_0x248 = -0.06f;
        break;
    }
    field_0x244 = 20;
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        field_0x252 = 12;
    } else {
        field_0x252 = 13;
    }
    if (mType >= 2) {
        fillTiles(this, mPos.x, 6);
    }
    field_0x230 = 0;
}

// FIXME(shotgun): 81.1 %, missing/extra + reloc-name
// Finds who stands on the floor (fn_8023AF14, the 'SBAL' ball, fn_8023A97C per player), then:
// 0 reads the tile under it, 2 waits for a rider (vfunc_0x34) and drops (tiles 6, state 3) or,
// for types 4 / 5, waits for all four players to open (state 6), 3 sinks to 1.6384, 4 holds
// while ridden, 5 rises back and restores the tiles, 6 / 7 fall away (camera shake, debris),
// 8 stays down.
void TrickFloor2_804AA628::update() {
    JGeometry::TVec3f saved = mPos;
    switch (mType) {
    case 0:
    case 1:
    case 6:
        mPos.x += 32.0f;
        mPos.y += 32.0f;
        break;
    case 2:
        mPos.x += 48.0f;
        mPos.y += 40.0f;
        break;
    case 3:
        mPos.x += 32.0f;
        mPos.y += 40.0f;
        break;
    case 4:
    case 5: {
        field_0x80.set(-48.0f, -48.0f, 48.0f, 48.0f);
        JGeometry::TBox2f box = field_0x80;
        mPos.x += 48.0f;
        mPos.y += 48.0f;
        JGeometry::TVec2f p(mPos.x, mPos.y);
        box.addPos(p);
        fn_80239914(&box, field_0x4, mSerial);
        mPos = saved;
        mPos.y += 48.0f;
        mBox.set(-16.0f, -50.0f, 24.0f, 50.0f);
        break;
    }
    }
    JGeometry::TVec2f p(mPos.x, mPos.y);
    JGeometry::TBox2f box = mBox;
    box.addPos(p);
    s32 player = fn_8023AF14(&box, field_0x4, false, 4.5f);
    if (player >= 0 && fn_8022F514(player) == true && fn_8022F478(player) == player) {
        player = -1;
    }
    if (field_0x23c == 1 && player >= 0 && !fn_80234F68(player)) {
        player = -1;
    }
    s32 ball = fn_801F2718('SBAL');
    fn_8023A89C(&box, field_0x4, &p);
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023A97C(i, &box, field_0x4, &p)) {
            count++;
        }
    }
    mPos = saved;
    switch (field_0x230) {
    case 0: {
        s32 layer = field_0x4;
        const JGeometry::TBox2f& view = layer < 8 ? getGameView_802D6A9C()->mVisibleBox
                                                  : getGameView_802D6A9C()->field_0xdc;
        JGeometry::TBox2f vbox = view;
        s32 tx = (s32)(mPos.x - vbox.i.x);
        s32 ty = (s32)(mPos.y - vbox.i.y);
        field_0x250 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(field_0x4, tileIndex(tx, ty));
        field_0x230 = 2;
        break;
    }
    case 1:
        mAlive = false;
        break;
    case 2:
        switch (mType) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 6:
            if (!vfunc_0x34()) {
                break;
            }
            if (player < 0 && ball < 0) {
                break;
            }
            spawnDust(this, 0);
            fillTiles(this, mPos.x, 6);
            field_0x230 = 3;
            startSound_801F0E34(0x3001B, 0);
            break;
        case 4:
        case 5:
            if (count != 4) {
                break;
            }
            vfunc_0x30(0xFF, true);
            startSound_801F0E34(0x30009, 0);
            fillTiles(this, mPos.x, 0x7C);
            field_0x24c = 0.07f;
            field_0x230 = 6;
            startSound_801F0E34(0x3001B, 0);
            break;
        }
        break;
    case 3:
        field_0x248 += 0.2f;
        if (field_0x248 >= 1.6f) {
            field_0x248 = 1.6384f;
            fillTiles(this, mPos.x, field_0x252);
            field_0x244 = 0x28;
            if (field_0x23c == 2) {
                field_0x230 = 8;
            } else {
                field_0x230 = 4;
            }
        }
        break;
    case 4:
        if (field_0x244 == 0 && player < 0 && ball < 0) {
            startSound_801F0E34(0x3001C, 0);
            field_0x230 = 5;
            break;
        }
        if (field_0x244 != 0) {
            field_0x244--;
            if (field_0x244 == 0 && field_0x23c != 0 && vfunc_0x34() == true) {
                field_0x244 = 1;
            }
        }
        break;
    case 5:
        field_0x248 -= 0.04f;
        if (field_0x248 < 0.0f) {
            field_0x248 = 0.0f;
            f32 x = mPos.x;
            s32 layer = field_0x4;
            const JGeometry::TBox2f& view = layer < 8 ? getGameView_802D6A9C()->mVisibleBox
                                                      : getGameView_802D6A9C()->field_0xdc;
            JGeometry::TBox2f vbox = view;
            for (s32 i = 0; i < lbl_8046F788[mType]; i++) {
                s32 tx = (s32)(x - vbox.i.x);
                f32 y = mPos.y;
                for (s32 j = 0; j < lbl_8046F7A4[mType]; j++) {
                    s32 ty = (s32)(y - vbox.i.y);
                    s32 index = tileIndex(tx, ty);
                    if (mType == 1) {
                        gGameRoot2_8053AB10->field_0x20->fn_802F9974(field_0x4, index, lbl_8046F7C0[i][j]);
                    } else {
                        gGameRoot2_8053AB10->field_0x20->fn_802F9974(field_0x4, index, field_0x250);
                    }
                    y += 16.0f;
                }
                x += 16.0f;
            }
            field_0x230 = 2;
            spawnDust(this, 1);
        }
        break;
    case 6: {
        field_0x248 -= field_0x24c;
        field_0x24c = field_0x24c - 0.001;
        if (field_0x248 <= -1.6f) {
            field_0x248 = -1.6f;
            field_0x244 = 4;
            field_0x230 = 7;
        }
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x7f4 == 0) {
            camera->field_0x7f5 = 1;
        }
        break;
    }
    case 7: {
        if (field_0x244 != 0) {
            field_0x244--;
        } else {
            field_0x248 -= field_0x24c;
            field_0x24c = field_0x24c + 0.0012;
            if (field_0x248 <= -3.2f) {
                field_0x248 = -3.2f;
                field_0x230 = 8;
                getGameView_802D6A9C()->fn_802D79E8(field_0x4, 8.0f, 0.0f, true);
                JGeometry::TVec3f pos(48.0f + (96.0f + mPos.x), 48.0f + mPos.y, 0.0f);
                spawnEmitter_8013CC50(field_0x4, &pos, 0x263, 0, 0, NULL, NULL, true, 1.0f);
                if (mType == 4) {
                    const char* name = getGameView_802D6A9C()->fn_802D9C4C();
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x82, 1, 0, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x83, 1, 0, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &pos, 0x84, 1, 0, NULL, NULL, true, 1.0f);
                    if (name[0] == 'r' || name[0] == 's' || name[0] == 'y') {
                        spawnEmitter_8013CC50(field_0x4, &pos, 0x80, 1, 0, NULL, NULL, true, 1.0f);
                    } else {
                        spawnEmitter_8013CC50(field_0x4, &pos, 0x81, 1, 0, NULL, NULL, true, 1.0f);
                    }
                    startSound_801F0E34(0x30001, 0);
                } else {
                    fillTiles(this, 96.0f + mPos.x, field_0x240);
                }
                mAlive = false;
            }
        }
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x7f4 == 0) {
            camera->field_0x7f5 = 1;
        }
        break;
    }
    case 8:
        break;
    }
}

// FIXME(shotgun): 90.6 %, missing/extra + reloc-name
// The hole picture "otosiana_bg_s.bti" under the floor (scaled per type), then the floor's two
// halves tilted by field_0x248 (the left one about its edge, the right one mirrored); types 4 / 5
// draw the ground or the house instead while falling.
void TrickFloor2_804AA628::draw() {
    SpriteParams_801F5AE0 sp;
    sp.field_0x1c = -1;
    sp.field_0x1d = -2;
    Mtx m;
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[0][1] = 0.0f;
    m[0][0] = lbl_8046F818[mType];
    m[0][2] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = lbl_8046F834[mType];
    sp.mMtx = m;
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "otosiana_bg_s.bti");
    JGeometry::TVec3f hole(mPos.x + lbl_8046F7E0[mType], mPos.y + lbl_8046F7FC[mType], mPos.z);
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&hole, res, &sp);
    }

    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    f32 angle = field_0x248;
    setRotMtx(m, JGeometry::TVec3f(0.0f, angle, 0.0f));
    sp.field_0x1c = -1;
    sp.field_0x1d = -1;
    sp.field_0x21 = 1;
    sp.mMtx = m;
    switch (mType) {
    case 0:
        drawSmp(this, &mPos, "floor_smp.bin", &sp);
        break;
    case 1:
        drawSmp(this, &mPos, "mastersword_left_smp.bin", &sp);
        break;
    case 2:
        drawSmp(this, &mPos, "house_left_smp.bin", &sp);
        break;
    case 3:
        drawSmp(this, &mPos, "tree0_smp.bin", &sp);
        drawSmp(this, &mPos, "tree1_smp.bin", &sp);
        break;
    case 4:
    case 5:
        break;
    case 6:
        drawSmp(this, &mPos, "field_smp.bin", &sp);
        break;
    }

    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    if (mType == 3 || mType == 4 || mType == 5) {
        angle = 3.1415927f - field_0x248;
    } else {
        angle = -field_0x248;
    }
    setRotMtx(m, JGeometry::TVec3f(0.0f, angle, 0.0f));
    sp.mMtx = m;
    switch (mType) {
    case 0: {
        JGeometry::TVec3f pos(64.0f + mPos.x - 32.0f * JMath::sincosTable_.cosRadian(-field_0x248),
                              32.0f * JMath::sincosTable_.sinRadian(field_0x248) + mPos.y, mPos.z);
        drawSmp(this, &pos, "floor_smp.bin", &sp);
        break;
    }
    case 1: {
        JGeometry::TVec3f pos(64.0f + mPos.x - 32.0f * JMath::sincosTable_.cosRadian(-field_0x248),
                              32.0f * JMath::sincosTable_.sinRadian(field_0x248) + mPos.y, mPos.z);
        drawSmp(this, &pos, "mastersword_right_smp.bin", &sp);
        break;
    }
    case 2: {
        JGeometry::TVec3f pos(96.0f + mPos.x - 48.0f * JMath::sincosTable_.cosRadian(-field_0x248),
                              48.0f * JMath::sincosTable_.sinRadian(field_0x248) + mPos.y, mPos.z);
        drawSmp(this, &pos, "house_right_smp.bin", &sp);
        break;
    }
    case 3: {
        JGeometry::TVec3f pos0(64.0f + mPos.x, mPos.y, mPos.z);
        drawSmp(this, &pos0, "tree0_smp.bin", &sp);
        JGeometry::TVec3f pos1(64.0f + mPos.x, mPos.y, mPos.z);
        drawSmp(this, &pos1, "tree1_smp.bin", &sp);
        break;
    }
    case 4:
    case 5:
        sp.field_0x1c = 1;
        sp.field_0x1d = -1;
        if (field_0x230 == 7) {
            JGeometry::TVec3f pos(96.0f + mPos.x, mPos.y, mPos.z);
            drawSmp(this, &pos, "jimen_smp.bin", &sp);
        } else if (field_0x230 != 8) {
            JGeometry::TVec3f pos(96.0f + mPos.x, mPos.y, mPos.z);
            drawSmp(this, &pos, "house_smp.bin", &sp);
        }
        break;
    case 6: {
        JGeometry::TVec3f pos(64.0f + mPos.x - 32.0f * JMath::sincosTable_.cosRadian(-field_0x248),
                              32.0f * JMath::sincosTable_.sinRadian(field_0x248) + mPos.y, mPos.z);
        drawSmp(this, &pos, "field_smp.bin", &sp);
        break;
    }
    }
}

TrickFloor2_804AA628::~TrickFloor2_804AA628() {}
