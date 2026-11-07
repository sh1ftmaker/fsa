#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/act/act_trick_floor_2.h"

// FIXME(shotgun): the 7-entry tables of .rodata at 0x8046F788 (outer count) and 0x8046F7A4
// (inner count), indexed by mType.
extern const s32 lbl_8046F788[];
extern const s32 lbl_8046F7A4[];
// FIXME(shotgun): the 4x4 u16 tiles at 0x8046F7C0 (inside dtk's lbl_8046F7A4) state 5 restores
// for type 1.
extern const u16 lbl_8046F7C0[4][4];

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

// FIXME(shotgun): 81 %, missing/extra + reloc-name (literal pool not split into this unit yet)
void TrickFloor2_804AA628::init() {
    mType = field_0x90 & 0xF;
    field_0x238 = (field_0x90 >> 4) & 0xF;
    field_0x240 = (field_0x90 >> 12) & 0xFFF;
    field_0x248 = 0.0f;
    switch (mType) {
    case 3:
        mBox.set(-24.0f, -32.0f, 24.0f, 32.0f);
        break;
    case 2:
        mBox.set(-40.0f, -32.0f, 40.0f, 32.0f);
        break;
    case 4:
    case 5:
        mBox.set(-16.0f, -50.0f, 24.0f, 50.0f);
        field_0x248 = -0.06f;
        break;
    default:
        mBox.set(-32.0f, -32.0f, 32.0f, 32.0f);
        break;
    }
    field_0x244 = 20;
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        field_0x252 = 12;
    } else {
        field_0x252 = 13;
    }
    if (mType >= 2) {
        f32 x = mPos.x;
        s32 layer = field_0x4;
        const JGeometry::TBox2f& view = layer < 8 ? getGameView_802D6A9C()->mVisibleBox
                                                  : getGameView_802D6A9C()->field_0xdc;
        JGeometry::TBox2f box = view;
        for (s32 i = 0; i < lbl_8046F788[mType]; i++) {
            s32 tx = (s32)(x - box.i.x);
            f32 y = mPos.y;
            for (s32 j = 0; j < lbl_8046F7A4[mType]; j++) {
                s32 ty = (s32)(y - box.i.y);
                s32 cx = tx < 0 ? 0 : (tx >= 0x200 ? 0x1FF : tx);
                s32 cy = ty < 0 ? 0 : (ty >= 0x200 ? 0x1FF : ty);
                s32 index = (cx & 0x100) | ((cy & 0x100) << 1) | (cy & 0xF0) | ((cx >> 4) & 0xF);
                gGameRoot2_8053AB10->field_0x20->fn_802F9974(field_0x4, index, 6);
                y += 16.0f;
            }
            x += 16.0f;
        }
    }
    field_0x230 = 0;
}

// FIXME(shotgun): 78.8 %, missing/extra + reloc-name
// Finds who stands on the floor (fn_8023AF14, the 'SBAL' ball, fn_8023A97C per player), then:
// 0 reads the tile under it, 2 waits for a rider (vfunc_0x34) and drops (tiles 6, state 3) or,
// for types 4 / 5, waits for all four players to open (state 6), 3 sinks to 1.6384, 4 holds
// while ridden, 5 rises back and restores the tiles, 6 / 7 fall away (camera shake, debris).
void TrickFloor2_804AA628::update() {
    JGeometry::TVec3f saved = mPos;
    switch (mType) {
    case 3:
        mPos.x += 32.0f;
        mPos.y += 40.0f;
        break;
    case 2:
        mPos.x += 48.0f;
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
    default:
        mPos.x += 32.0f;
        mPos.y += 32.0f;
        break;
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
    }
}

TrickFloor2_804AA628::~TrickFloor2_804AA628() {}
