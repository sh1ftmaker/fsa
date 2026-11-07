#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_trick_floor_2.h"

// FIXME(shotgun): the 7-entry tables of .rodata at 0x8046F788 (outer count) and 0x8046F7A4
// (inner count), indexed by mType.
extern const s32 lbl_8046F788[];
extern const s32 lbl_8046F7A4[];

// FIXME(shotgun): draw (0xD14) and update (0x10CC) not written in this pass.
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

TrickFloor2_804AA628::~TrickFloor2_804AA628() {}
