#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_four_switch_monitor.h"

FourSwitchMonitor_804A80A0::~FourSwitchMonitor_804A80A0() {}

void FourSwitchMonitor_804A80A0::draw() {}

// Snaps a coordinate to the middle of its 16-unit cell.
static inline f32 snapToCell(f32 v) {
    return (f32)((s32)v / 16 * 16 + 8);
}

// Two set slots with different keys may not share a value, and an unset
// slot (0xFF) fails.
static inline s32 slotsCollide(const s32* flags, const s32* keys) {
    s32 collide = 0;
    for (s32 j = 0; j < 3; j++) {
        if (flags[j] == 0xFF) {
            collide = 1;
            continue;
        }
        for (s32 k = j + 1; k < 4; k++) {
            if (flags[k] == 0xFF) {
                continue;
            }
            if (keys[j] == keys[k]) {
                continue;
            }
            if (flags[j] == flags[k]) {
                collide = 1;
            }
        }
    }
    return collide;
}

// FIXME: 91.98 %. The original runs the same slot check twice, once in each arm
// of `lbl_8053AAF8->field_0x7e == 1` (two identical loops; `li r0, 0` before the
// test), and ours runs it once, so the registers and block layout after it differ.
// The switch table is `jumptable_804A80F4` in the original but a compiler-made
// `@NNN` here (reloc-name, a data-naming issue). `(field_0x83 & 3) == 1` is an
// unsigned compare (cmplwi) in the original, signed here: field_0x83's type is
// not known yet.
void FourSwitchMonitor_804A80A0::update() {
    s32 count = 0;
    s32 flags[4];

    switch (field_0x230) {
    case 0: {
        s32 fixedSlot = 1;
        for (s32 i = 0; i < 4; i++) {
            flags[i] = 0xFF;
            if (getGameView_802D6A9C()->fn_802D800C(field_0x244[i])) {
                flags[i] = getGameView_802D6A9C()->fn_802D7E1C(field_0x244[i]);
                count++;
                if (flags[i] == 0xFF) {
                    flags[i] = i + 4;
                    fixedSlot = -1;
                }
            }
        }
        if (count == 4) {
            if (slotsCollide(flags, field_0x244) == 0) {
                if (field_0x234 == 3) {
                    fn_8022E2D8(0xF);
                    vfunc_0x30(0xFF, true);
                    field_0x230 = 1;
                } else if (lbl_8053AAF8->field_0x7f == 0 || fixedSlot == -1) {
                    fn_8022E2D8(0xF);
                    vfunc_0x30(0xFF, true);
                    for (s32 i = 0; i < 4; i++) {
                        getGameView_802D6A9C()->fn_802D8050(field_0x244[i], true, 0xFF);
                    }
                    if (field_0x234 != 0) {
                        if (field_0x234 == 2) {
                            fn_801F0D20(3);
                        }
                        mAlive = false;
                    } else {
                        field_0x230 = 1;
                    }
                } else {
                    if (fn_8023077C()) {
                        for (s32 i = 0; i < 4; i++) {
                            JGeometry::TVec3f pos = *fn_8023E724(i);
                            pos.x = snapToCell(pos.x);
                            pos.y = snapToCell(pos.y);
                            fn_80230498(i, &pos, 0xE8, 0xFF, -0x80, -0x80, true);
                        }
                        field_0x230 = 3;
                        field_0x238 = 0;
                        field_0x23c = 0x1E;
                    } else {
                        field_0x230 = 2;
                    }
                }
            }
        }
        break;
    }
    case 1:
    case 6:
    default:
        if (!getGameView_802D6A9C()->fn_802D800C((field_0x90 >> 22) & 0x1F)) {
            vfunc_0x30(0xFF, false);
            field_0x230 = 0;
        } else if (!getGameView_802D6A9C()->fn_802D800C((field_0x90 >> 17) & 0x1F)) {
            vfunc_0x30(0xFF, false);
            field_0x230 = 0;
        } else if (!getGameView_802D6A9C()->fn_802D800C((field_0x90 >> 12) & 0x1F)) {
            vfunc_0x30(0xFF, false);
            field_0x230 = 0;
        } else if (!getGameView_802D6A9C()->fn_802D800C((field_0x90 >> 7) & 0x1F)) {
            vfunc_0x30(0xFF, false);
            field_0x230 = 0;
        }
        break;
    case 2:
        if (fn_8023077C()) {
            s32 end = fn_801F856C()->mActorEnd;
            for (int i = 0; i < 4; i++) {
                for (s32 j = 0; j < end; j++) {
                    ActorBase_804A5770* actor = fn_801F856C()->mActors[j];
                    if (actor != NULL && actor->mActorId == ACTOR_SWTH &&
                        (actor->field_0x90 >> 27) == field_0x244[i]) {
                        JGeometry::TVec3f pos = *fn_8023E724(i);
                        pos.x = snapToCell(pos.x);
                        pos.y = snapToCell(pos.y);
                        fn_80230498(i, &pos, 0xE8, 0xFF, -0x80, -0x80, true);
                        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
                    }
                }
            }
            fn_8022E2D8(0xF);
            field_0x230 = 3;
            field_0x238 = 0;
            field_0x23c = 0x1E;
        }
        break;
    case 3:
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *fn_8023E724(i);
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
        }
        field_0x23c--;
        if (field_0x23c == 0) {
            field_0x238++;
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = *fn_8023E724(i);
                fn_80230498(i, &pos, field_0x238 + 0xDB, 0xFF, -0x80, -0x80, true);
                if (field_0x238 == 1) {
                    fn_80230FD0(i, 0x10001);
                }
            }
            field_0x23c = 2;
            if (field_0x238 == 0xD) {
                field_0x230 = 4;
                field_0x23c = 10;
                if (field_0x234 == 4) {
                    if ((lbl_8053AAF8->field_0x83 & 3) == 1) {
                        field_0x23c = 0x78;
                        field_0x240 = 0x3C;
                        for (s32 k = 0; k < 4; k++) {
                            u8 bit = 1 << k;
                            bool entered = lbl_8053AAF8->field_0x82 & bit;
                            if (entered == true) {
                                fn_80230E1C(k, 0);
                            }
                        }
                    } else {
                        field_0x23c = 0x50;
                        field_0x240 = 0x28;
                    }
                }
            }
        }
        break;
    case 4:
        if (field_0x234 == 4) {
            gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
            if (field_0x23c == field_0x240) {
                vfunc_0x30(0xFF, true);
            }
        } else {
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = *fn_8023E724(i);
                gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
            }
        }
        if (field_0x23c == 0) {
            vfunc_0x30(0xFF, true);
            for (s32 i = 0; i < 4; i++) {
                getGameView_802D6A9C()->fn_802D8050(field_0x244[i], true, 0xFF);
            }
            field_0x230 = 5;
            field_0x23c = 0xF;
            if (field_0x234 == 2) {
                fn_801F0D20(3);
            }
        } else {
            field_0x23c--;
        }
        break;
    case 5:
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *fn_8023E724(i);
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
        }
        if (field_0x23c == 0) {
            if (fn_80230534(false)) {
                if (field_0x234 != 0) {
                    mAlive = false;
                } else {
                    field_0x230 = 1;
                }
            }
        } else {
            field_0x23c--;
        }
        break;
    case 7: {
        for (s32 i = 0; i < 4; i++) {
            flags[i] = 0xFF;
            if (getGameView_802D6A9C()->fn_802D800C(field_0x244[i])) {
                flags[i] = getGameView_802D6A9C()->fn_802D7E1C(field_0x244[i]);
                count++;
                if (flags[i] == 0xFF) {
                    flags[i] = i + 4;
                }
            }
        }
        if (count == 4) {
            vfunc_0x30(0xFF, true);
            field_0x230 = 1;
        } else if (count != 0) {
            mAlive = false;
        }
        break;
    }
    }
}

void FourSwitchMonitor_804A80A0::init() {
    static const s32 lbl_8046C850[4] = {22, 17, 12, 7};

    field_0x234 = field_0x90 & 0x1F;
    if (field_0x234 != 5) {
        field_0x230 = 0;
    } else {
        field_0x230 = 7;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x244[i] = (field_0x90 >> lbl_8046C850[i]) & 0x1F;
    }
}
