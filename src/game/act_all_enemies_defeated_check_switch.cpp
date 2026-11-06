#include "game/GameAudio.h"
#include "game/act/act_all_enemies_defeated_check_switch.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/Unk_8045A080.h"
#include "game/ViewLayer.h"

// Whether p (x, y) is in box (min inclusive, max exclusive).
static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

void AllEnemiesDefeatedCheckSwitch_804A8AC8::init() {
    field_0x230 = 0;
    field_0x234 = field_0x90 & 0x1F;
    field_0x238 = (field_0x90 >> 7) & 0x1F;
    field_0x258 = ((field_0x90 >> 22) & 0x1F) + 1;
    field_0x25c = ((field_0x90 >> 17) & 0x1F) + 1;
    field_0x254 = (field_0x90 >> 12) & 0x1F;
    field_0x250 = 0;

    switch (field_0x234) {
    case 2:
    case 3:
    case 6:
    case 7:
        field_0x23c.set(8.0f * -field_0x258, 8.0f * -field_0x25c, 8.0f * field_0x258, 8.0f * field_0x25c);
        field_0x23c.i.x += mPos.x;
        field_0x23c.i.y += mPos.y;
        field_0x23c.f.x += mPos.x;
        field_0x23c.f.y += mPos.y;
        break;
    case 4:
    case 5:
    case 8:
    case 9:
        fn_802BDCC0();
        break;
    }
}

void AllEnemiesDefeatedCheckSwitch_804A8AC8::fn_802BDCC0() {
    if (field_0x4 < 8) {
        field_0x23c = *layerBox(field_0x4);
    } else {
        JGeometry::TVec3f pos = mPos;
        u32 x = (u32)pos.x & ~7;
        u32 y = (u32)pos.y & ~7;
        pos.x = x;
        pos.y = y;
        pos.z = 0.0f;

        for (s32 i = 0; i < 64; i++) {
            if (pos.x >= 512.0f) {
                pos.x = 512.0f;
                break;
            }
            u16 tile = fn_80226850(&pos, field_0x4);
            if (tile >= 8 && tile < 16 && (tile & 1)) {
                break;
            }
            pos.x += 8.0f;
        }
        field_0x23c.f.x = pos.x;

        pos.x = x;
        for (s32 i = 0; i < 64; i++) {
            if (pos.x < 8.0f) {
                pos.x = 0.0f;
                break;
            }
            u16 tile = fn_80226850(&pos, field_0x4);
            if (tile >= 8 && tile < 16 && (tile & 1)) {
                pos.x += 8.0f;
                break;
            }
            pos.x -= 8.0f;
        }
        field_0x23c.i.x = pos.x;

        pos.x = x;
        pos.y = y;
        pos.z = 0.0f;
        for (s32 i = 0; i < 64; i++) {
            if (pos.y >= 512.0f) {
                pos.y = 512.0f;
                break;
            }
            u16 tile = fn_80226850(&pos, field_0x4);
            if (tile >= 8 && tile < 16 && (tile & 1)) {
                break;
            }
            pos.y += 8.0f;
        }
        field_0x23c.f.y = pos.y;

        pos.y = y;
        for (s32 i = 0; i < 64; i++) {
            if (pos.y < 8.0f) {
                pos.y = 0.0f;
                break;
            }
            u16 tile = fn_80226850(&pos, field_0x4);
            if (tile >= 8 && tile < 16 && (tile & 1)) {
                pos.y += 8.0f;
                break;
            }
            pos.y -= 8.0f;
        }
        field_0x23c.i.y = pos.y;
    }
}

void AllEnemiesDefeatedCheckSwitch_804A8AC8::update() {
    if (field_0x250 == 0) {
        field_0x24c = gAudioMgr_8053AF80->field_0x58;
        field_0x250 = 1;
        return;
    }

    switch (field_0x230) {
    case 0:
        switch (field_0x234) {
        case 0:
        case 1:
            if (getActorManager_801F856C()->field_0x305c == 1) {
                if (field_0x238 == 0 || (field_0x238 == 1 && !vfunc_0x34())) {
                    vfunc_0x30(0xFF, true);
                    if (field_0x234 == 0) {
                        fn_801F0D20(3);
                    }
                    if (field_0x238 == 0) {
                        field_0x230 = 1;
                    }
                    if (field_0x254 == 0) {
                        if (field_0x24c != gAudioMgr_8053AF80->field_0x58) {
                            gGameRoot_8053AAF8->fn_80138B7C();
                            field_0x254 = 1;
                        }
                    } else if (field_0x254 == 2) {
                        gGameRoot_8053AAF8->fn_80138B7C();
                        field_0x254 = 1;
                    }
                }
            } else {
                if (field_0x238 == 1 && vfunc_0x34() == true) {
                    vfunc_0x30(0xFF, false);
                }
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5: {
            bool found = false;
            for (s32 i = 0; i < 4; i++) {
                if (contains(field_0x23c, *getPlayerPos_8023E724(i)) == true) {
                    if (field_0x4 == getPlayerLayer_8023DE58(i)) {
                        found = true;
                    }
                }
            }
            if (!found) {
                break;
            }
        }
        // fallthrough
        case 6:
        case 7:
        case 8:
        case 9:
            if (getActorManager_801F856C()->fn_801F75B4(field_0x4, &field_0x23c) == true) {
                if (field_0x238 == 0 || (field_0x238 == 1 && !vfunc_0x34())) {
                    vfunc_0x30(0xFF, true);
                    if (field_0x234 == 2 || field_0x234 == 4 || field_0x234 == 6 || field_0x234 == 8) {
                        fn_801F0D20(3);
                    }
                    if (field_0x238 == 0) {
                        field_0x230 = 1;
                    }
                    if (field_0x254 == 0) {
                        if (field_0x24c != gAudioMgr_8053AF80->field_0x58) {
                            gGameRoot_8053AAF8->fn_80138B7C();
                            field_0x254 = 1;
                        }
                    } else if (field_0x254 == 2) {
                        gGameRoot_8053AAF8->fn_80138B7C();
                        field_0x254 = 1;
                    }
                }
            } else {
                if (field_0x238 == 1 && vfunc_0x34() == true) {
                    vfunc_0x30(0xFF, false);
                }
            }
            break;
        }
        break;
    case 1:
        break;
    }
}

void AllEnemiesDefeatedCheckSwitch_804A8AC8::draw() {}

AllEnemiesDefeatedCheckSwitch_804A8AC8::~AllEnemiesDefeatedCheckSwitch_804A8AC8() {}
