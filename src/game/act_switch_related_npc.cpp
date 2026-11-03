#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Effect.h"
#include "game/act/act_switch_related_npc.h"

SwitchRelatedNpc_804A8C28::~SwitchRelatedNpc_804A8C28() {}

void SwitchRelatedNpc_804A8C28::draw() {}

// FIXME: 89.66 %. Same instructions and structure; the register allocation
// differs. The original keeps this in r29 (stmw r27) with the layer and later
// the cell in r30, the step in r28 and the tile in r27 (the u8 step converted
// into the tile's register); ours needs r26 too (this r28, layer r27, step
// r26, cell r29, converted step r30). Because of that the case 1 table
// pointer lands in r3 instead of r6 and the table loads are scheduled
// earlier. Tried: no layer local, ternary step, s32 slot with casts.
void SwitchRelatedNpc_804A8C28::update() {
    static const u16 lbl_8046D470[6] = {0x20, 0x360, 0x370, 0x361, 0x371, 0};
    static const u16 lbl_8046D47C[14] = {0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
                                         0x1F, 0xA0, 0xA1, 0xA2, 0xA3, 0xFFF, 0};
    static const u16 lbl_8046D498[14] = {0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E,
                                         0x2F, 0xB0, 0xB1, 0xB2, 0xB3, 0x100F, 0};
    static const u16 lbl_8046D4B4[15][2] = {
        {0, 0}, {0, 0}, {0x18, 0x28}, {0x19, 0x29}, {0x1A, 0x2A}, {0x1B, 0x2B}, {0x1C, 0x2C},
        {0x1D, 0x2D}, {0x1E, 0x2E}, {0x1F, 0x2F}, {0xA0, 0xB0}, {0xA1, 0xB1},
        {0xA2, 0xB2}, {0xA3, 0xB3}, {0xFFF, 0x100F}};

    if ((field_0x90 >> 27) == 0 || vfunc_0x34() == 1) {
        if (mDelay == 0) {
            switch (mType) {
            case 0:
                lbl_8053AB10->field_0x20->fn_802F9974(field_0x4, mCell, 0x30);
                break;
            case 1:
                lbl_8053AB10->field_0x20->fn_802F9974(field_0x4, mCell, lbl_8046D470[mTileIndex]);
                break;
            case 2: {
                s32 layer = field_0x4;
                u8 step = 8;
                if (layer < 8) {
                    step = 1;
                }
                u16 slot = fn_802D6A9C()->fn_802D7B34(layer + 1, (s32)mSlot);
                s32 cell = mCell;
                if (slot == 0) {
                    u32 tile = lbl_8046D498[mTable];
                    if (tile < 0xA0) {
                        fn_802D6A9C()->fn_802D7BE8(field_0x4, (s32)mSlot, tile - 0x26);
                    } else {
                        fn_802D6A9C()->fn_802D7BE8(field_0x4, (s32)mSlot, tile - 0xA6);
                    }
                    lbl_8053AB10->field_0x20->fn_802F9974(field_0x4 + step, cell, lbl_8046D498[mTable]);
                    if ((cell & 0x100) != ((cell - 0x10) & 0x100)) {
                        cell -= 0x110;
                    } else {
                        cell -= 0x10;
                    }
                    lbl_8053AB10->field_0x20->fn_802F9974(field_0x4 + step, cell, lbl_8046D47C[mTable]);
                } else {
                    lbl_8053AB10->field_0x20->fn_802F9974(field_0x4 + step, cell, lbl_8046D4B4[slot][1]);
                    if ((cell & 0x100) != ((cell - 0x10) & 0x100)) {
                        cell -= 0x110;
                    } else {
                        cell -= 0x10;
                    }
                    lbl_8053AB10->field_0x20->fn_802F9974(field_0x4 + step, cell, lbl_8046D4B4[slot][0]);
                }
                lbl_8053AB10->field_0x20->fn_802F9974(field_0x4, mCell, 0x17);
                break;
            }
            case 3:
                switch (mSound) {
                case 0:
                case 3:
                    fn_801F0D20(3);
                    break;
                case 1:
                    fn_801F0D20(0x42);
                    break;
                case 2:
                    fn_801F0D20(6);
                    break;
                case 4:
                    fn_801F0D20(0x61);
                    break;
                case 5:
                    fn_801F0D20(0x61);
                    fn_801F3668();
                    break;
                }
                break;
            }
            if (mEffects) {
                fn_8013CC50(field_0x4, &mPos, 0x564, 0, 0, NULL, NULL, true, 0.5f);
                fn_8013CC50(field_0x4, &mPos, 0x503, 1, 0, NULL, NULL, true, 1.0f);
                fn_801F0E34(0x30003, 0);
            }
            mAlive = false;
        } else {
            mDelay--;
        }
    }
}

void SwitchRelatedNpc_804A8C28::init() {
    mType = field_0x90 & 0x3F;
    field_0x238 = (field_0x90 >> 6) & 0xF;
    mTileIndex = (field_0x90 >> 10) & 0xF;
    mTable = (field_0x90 >> 14) & 0xF;
    mSound = (field_0x90 >> 18) & 0xF;
    if (field_0x90 & 0x04000000) {
        mEffects = true;
    } else {
        mEffects = false;
    }
    JGeometry::TVec2f cell(mPos.x, mPos.y);
    mCell = fn_80226BC4(&cell, field_0x4);
    mDelay = 0;
    switch (mType) {
    case 0:
        fn_802D6A9C()->fn_802D4874(field_0x4, mCell, &mBit, &mMask);
        if (fn_802D6A9C()->fn_802D4ADC(field_0x4, mCell) == true) {
            lbl_8053AB10->field_0x20->fn_802F9974(field_0x4, mCell, 0x10);
            mAlive = false;
        }
        break;
    case 2:
        fn_802D6A9C()->fn_802D4768(field_0x4 + 1, mCell, &mSlot, &mMask);
        break;
    case 3:
        if (mSound == 0) {
            mDelay = 0x32;
        }
        break;
    }
    if (mAlive) {
        update();
    }
}
