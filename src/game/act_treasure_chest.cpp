#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/cls_804AE9B0.h"
#include "game/tu_80379F84.h"
#include "game/act/act_treasure_chest.h"

TreasureChest_804AA178::~TreasureChest_804AA178() {}

// FIXME: 99.63 %. Only the literal relocations differ (lbl_805413B8 vs @474,
// lbl_805413C0 vs @476 and the other sdata2 floats): the data split, not code.
void TreasureChest_804AA178::init() {
    field_0x234 = field_0x90 & 0xFF;
    field_0x238 = (field_0x90 >> 8) & 0xF;
    field_0x244 = (field_0x90 >> 12) & 0xF;
    if (field_0x244 >= 4) {
        field_0x244 = -1;
    }
    if ((field_0x234 >= 0x1A && field_0x234 < 0x20) || (field_0x234 >= 0x26 && field_0x234 < 0x2A) ||
        (field_0x234 >= 0x3B && field_0x234 < 0x40) || field_0x234 >= 0x4B ||
        (field_0x234 == 0x2E && field_0x4 >= 8)) {
        field_0x234 = 0;
    }

    field_0x25c = mPos;
    mPos.x += 8.0f;
    mPos.y += 12.0f;
    if (viewLayerFlag(field_0x4)) {
        mBox.i.x = -10.0f;
        mBox.i.y = 4.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 5.0f;
    } else {
        mBox.i.x = -6.0f;
        mBox.i.y = 0.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 5.0f;
    }

    JGeometry::TVec2f cell(field_0x25c.x, field_0x25c.y);
    mCell = fn_80226BC4(&cell, field_0x4);
    if (fn_802D6A9C()->fn_802D49A8(field_0x4, (u16)mCell) == true) {
        mAlive = false;
    }

    if (field_0x238 == 0) {
        field_0x230 = 0;
    } else if (field_0x238 == 2) {
        field_0x230 = 3;
    } else {
        field_0x230 = 2;
    }
    field_0x248 = -1;
    field_0x24c[0] = -1;
    field_0x24c[1] = -1;
    field_0x24c[2] = -1;
    field_0x24c[3] = -1;
}

void TreasureChest_804AA178::update() {
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    s32 layer = field_0x4;
    if (viewLayerFlag(layer)) {
        mBox.i.x = -10.0f;
        mBox.i.y = 2.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 5.0f;
    } else {
        mBox.i.x = -6.0f;
        mBox.i.y = 0.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 5.0f;
    }
    {
        JGeometry::TBox2f box(mBox);
        box.addPos(pos);
        field_0x240 = fn_8023AF14(&box, field_0x4, false, 0.0f);
    }

    switch (field_0x230) {
    case 0:
        if (lbl_8053AB10->field_0x20->fn_802F9908(field_0x4, mCell) == 0x30) {
            field_0x230 = 1;
        }
        break;
    case 1: {
        bool result = false;
        if (field_0x240 >= 0) {
            if (viewLayerFlag(field_0x4)) {
                if (fn_80230CFC(field_0x240) == 3 || fn_80230CFC(field_0x240) == 0 || fn_80230CFC(field_0x240) == 1) {
                    result = true;
                }
            } else if (fn_80230CFC(field_0x240) == 3) {
                result = true;
            }
            if (result == true) {
                mBox.i.x = -20.0f;
                mBox.i.y = -18.0f;
                mBox.f.x = 20.0f;
                mBox.f.y = 18.0f;
                JGeometry::TBox2f box(mBox);
                box.addPos(pos);
                fn_8022FEB8(&box, field_0x4, mSerial, true, true);
            }
            field_0x248 = field_0x240;
            if (fn_8022F514(field_0x240)) {
                field_0x248 = fn_8022F3DC(field_0x240);
            }
            if (result == true) {
                if (!fn_8023EFEC(field_0x240)) {
                    if (fn_8022EA0C(field_0x240) == field_0x240) {
                        if (lbl_8053AD30->fn_80309D6C(fn_8030AAF4(field_0x248), 0x100) != 0) {
                            if (vfunc_0x54()) {
                                mAlive = false;
                            }
                        }
                    }
                }
            }
        }
        break;
    }
    case 2:
        field_0x240 = field_0x244;
        field_0x248 = field_0x240;
        if (fn_8022F514(field_0x240)) {
            field_0x248 = fn_8022F3DC(field_0x240);
        }
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f from = *fn_8023E724(i);
            JGeometry::TVec3f at = from;
            at.x = from.x + 1.0f;
            at.y = from.y - 12.0f;
            at.z = from.z;
            if (fn_8022F514(i) == true) {
                if (field_0x244 == i) {
                    u32 code = ((field_0x244 << 26) + 0x80000000) + ((i << 24) + field_0x234);
                    s32 spawnLayer = fn_8023DE58(i);
                    field_0x24c[i] = fn_801F856C()->fn_801F9484(ACTOR_IMUP, spawnLayer, &at, code, -1, -1);
                }
            } else {
                if (field_0x244 == i) {
                    u32 code = ((field_0x244 << 26) + 0x80000000) + ((i << 24) + field_0x234);
                    s32 spawnLayer = fn_8023DE58(i);
                    field_0x24c[i] = fn_801F856C()->fn_801F9484(ACTOR_IMUP, spawnLayer, &at, code, -1, -1);
                }
            }
        }
        if (field_0x238 == 1) {
            field_0x230 = 4;
        } else {
            mAlive = false;
        }
        break;
    case 4:
        // Set by case 2 when the chest has no more spawns; nothing else reads it.
        break;
    case 3:
        field_0x240 = field_0x244;
        field_0x248 = field_0x240;
        if (fn_8022F514(field_0x240)) {
            field_0x248 = fn_8022F3DC(field_0x240);
        }
        if (vfunc_0x54()) {
            mAlive = false;
        }
        break;
    }
}

void TreasureChest_804AA178::draw() {}

bool TreasureChest_804AA178::fn_802EB27C() {
    // FIXME: 97.11 %. The original copies fn_801F666C's result through r0
    // (mr r0, r3; mr. r30, r0) where ours has mr. r30, r3; the assignment in
    // the condition gives the same bytes.
    s32 serial;
    bool result = false;
    for (s32 i = 0; i < 4; i++) {
        serial = field_0x24c[i];
        if (serial < 0) {
            continue;
        }
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
        if (actor != NULL && fn_801597B8(actor) == 3) {
            fn_801597C0(actor);
            result = true;
        }
    }
    return result;
}

// FIXME: 94.63 %. Two instructions too many. The stage-table lookup puts the
// row address and the bit in other registers than the original (slwi/addi
// order, lwz 0x10 base), the table flag is tested as a bool (clrlwi.) where the
// original tests a 0/1 word (cmpwi 0), and the flags live in r30/r31 where
// the original uses r26/r27 (stmw r27 vs stmw r26).
bool TreasureChest_804AA178::vfunc_0x54() {
    bool ok = false;
    bool result = false;
    if (lbl_8053AAF8->field_0x7e == 1) {
        if (fn_802308E0(field_0x240) == true) {
            ok = true;
        }
    } else if (fn_8023077C() == true) {
        lbl_8053AD70->fn_8037AE30();
        ok = true;
    }
    if (ok == true) {
        if (lbl_8053AAF8->field_0x7e == 1) {
            // Stage bit: lbl_8050EC80[0].field_0x10 selects the row, stage 0x118
            // (0-7) and 0x11C (0-3) the bit.
            bool open = false;
            s32 stage = lbl_80529DEC.field_0x118;
            s32 sub = lbl_80529DEC.field_0x11c;
            if (stage < 0 || stage > 7) {
                open = false;
            } else if (sub < 0 || sub > 3) {
                open = false;
            } else {
                open = (lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x28 & (1 << (sub + stage * 4))) != 0;
            }
            if (open && (field_0x234 <= 2 || field_0x234 == 0x2F)) {
                s32 r30 = lbl_8053AAF8->fn_80138A30();
                if (fn_801F856C()->fn_801F6058(r30 - 1) == field_0x240) {
                    s32 limit = 20;
                    if (lbl_8053AAF8->fn_80138A30() == 3) {
                        limit = 30;
                    } else if (lbl_8053AAF8->fn_80138A30() == 4) {
                        limit = 50;
                    }
                    if (fn_801CD664(100) <= limit) {
                        field_0x234 = 0x20;
                    }
                }
            }
        }

        f32 one = 1.0f;
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *fn_8023E724(i);
            JGeometry::TVec3f at = pos;
            at.x = pos.x + 1.0f;
            at.y = pos.y - 12.0f;
            at.z = pos.z;
            if (field_0x234 >= 0x30) {
                if (lbl_8053AAF8->field_0x7e == 1) {
                    if (fn_8022F514(i) == true) {
                        if (field_0x248 == fn_8022F3DC(i)) {
                            if (field_0x240 == i) {
                                u32 code = field_0x234 + (i << 24) + (field_0x240 << 26);
                                s32 layer = fn_8023DE58(i);
                                fn_801F856C()->fn_801F9484(ACTOR_IMUP, layer, &at, code, -1, -1);
                            } else {
                                u32 code = field_0x234 + (i << 24) + (field_0x240 << 26) + 0x10000000;
                                s32 layer = fn_8023DE58(i);
                                fn_801F856C()->fn_801F9484(ACTOR_IMUP, layer, &at, code, -1, -1);
                            }
                        }
                    } else if (field_0x248 == i) {
                        if (field_0x240 == i) {
                            u32 code = field_0x234 + (i << 24) + (field_0x240 << 26);
                            s32 layer = fn_8023DE58(i);
                            fn_801F856C()->fn_801F9484(ACTOR_IMUP, layer, &at, code, -1, -1);
                        } else {
                            u32 code = field_0x234 + (i << 24) + (field_0x240 << 26) + 0x10000000;
                            s32 layer = fn_8023DE58(i);
                            fn_801F856C()->fn_801F9484(ACTOR_IMUP, layer, &at, code, -1, -1);
                        }
                    }
                } else {
                    if (field_0x240 == i) {
                        u32 code = field_0x234 + (i << 24) + (field_0x240 << 26);
                        s32 layer = fn_8023DE58(i);
                        fn_801F856C()->fn_801F9484(ACTOR_IMUP, layer, &at, code, -1, -1);
                    } else {
                        u32 code = field_0x234 + (i << 24) + (field_0x240 << 26) + 0x10000000;
                        s32 layer = fn_8023DE58(i);
                        fn_801F856C()->fn_801F9484(ACTOR_IMUP, layer, &at, code, -1, -1);
                    }
                }
            } else {
                if (fn_8022F514(i) == true && fn_8022F3DC(i) == field_0x248 && field_0x240 == i) {
                    u32 code = field_0x234 + (i << 24) + (field_0x240 << 26) + 0x00800000;
                    s32 layer = fn_8023DE58(i);
                    fn_801F856C()->fn_801F9484(ACTOR_IMUP, layer, &at, code, -1, -1);
                } else if (field_0x248 == i && field_0x240 == i) {
                    u32 code = field_0x234 + (i << 24) + (field_0x240 << 26) + 0x00800000;
                    s32 layer = fn_8023DE58(i);
                    fn_801F856C()->fn_801F9484(ACTOR_IMUP, layer, &at, code, -1, -1);
                }
            }
        }

        if (field_0x238 == 0) {
            fn_80230FD0(field_0x240, 0x20013);
            lbl_8053AC98->fn_802A3948(&field_0x25c, 0, field_0x4, 0x10);
            s32 bit = fn_802D6A9C()->fn_802D4C10(field_0x4, (u16)mCell);
            if (bit != -1) {
                fn_802D6A9C()->fn_802D7D00(field_0x4, bit, true);
            }
            JGeometry::TVec3f fxA(field_0x25c.x + 8.0f, field_0x25c.y + 8.0f, 0.0f);
            fn_8013CC50(field_0x4, &fxA, 0xD0, 0, 0, NULL, NULL, true, one);
            JGeometry::TVec3f fxB(field_0x25c.x + 8.0f, field_0x25c.y + 8.0f, 0.0f);
            fn_8013CC50(field_0x4, &fxB, 0xD1, 0, 1, NULL, NULL, true, one);
        }
        result = true;
    }
    return result;
}
