#include "game/ActorManager.h"
#include "game/DrawShadow.h"
#include "game/Effect.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_dropped_item.h"
#include "game/cls_804AE9B0.h"
#include <math.h>

static const u32 lbl_8046D4F0[23] = {
    0x30, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
};

// Spawns the 'IMUP' actor for player i with the given param (item sprite code, collecting player and
// i in the top bits). Inlined at six sites in fn_802BFF30.
static inline void spawnImup(u32 param, s32 i, const JGeometry::TVec3f* pos) {
    s32 layer = getPlayerLayer_8023DE58(i);
    getActorManager_801F856C()->spawnActor_801F9484('IMUP', layer, pos, param, -1, -1);
}

inline void DroppedItem_804A8C80::talk(s32 serial, s32 layer, const JGeometry::TVec3f& pos) {
    fn_8038AD24(field_0x25c, 0x151, serial, layer, pos, 4);
}

// FIXME: 95.98 %, missing/extra (ours has 2 fewer instructions). The original saves one more callee-saved register (r28)
// across the second getGameView call, builds the final spawn key (fn_802D6F38 with `true`) in a different
// rlwimi order, and loads hit.x / hit.y before the 0.0f literal in the `!= 0.0f` tests.
void DroppedItem_804A8C80::init() {
    field_0x238 = field_0x90 & 0x7F;
    // Both arms unpack the same four fields: hoisting them out of the if/else drops init from 96 % to 91 %
    // (the original's code is duplicated per arm).
    if (field_0x238 == 11 || field_0x238 == 22) {
        field_0x23c = (field_0x90 >> 22) & 0x1F;
        field_0x234 = (field_0x90 >> 7) & 0x1F;
        field_0x240 = (field_0x90 >> 12) & 0x1F;
        field_0x244 = (field_0x90 >> 17) & 0x1F;
    } else {
        field_0x23c = (field_0x90 >> 22) & 0x1F;
        field_0x234 = (field_0x90 >> 7) & 0x1F;
        field_0x240 = (field_0x90 >> 12) & 0x1F;
        field_0x244 = (field_0x90 >> 17) & 0x1F;
        if (field_0x244 >= 4) {
            field_0x244 = -1;
        }
    }

    field_0xb0 = 0x20000;
    field_0x264 = field_0x90;
    field_0x268.set(mPos.x, mPos.y);

    s32 viewRow = getGameView_802D6A9C()->field_0x18;
    s32 viewCol = getGameView_802D6A9C()->field_0x14;
    field_0x270 = ((viewCol & 0xFF) << 16) | (field_0x244 << 24) | ((viewRow & 0xFF) << 8) | (field_0x4 & 0xFF);

    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-6.0f, -6.0f, 6.0f, 6.0f);

    if (field_0x234 == 0 || field_0x234 == 2 || field_0x234 == 3) {
        field_0x230 = 0;
        if (field_0x234 == 2) {
            field_0x234 = 0;
        }
        if (field_0x234 == 3) {
            if (viewLayerFlag(field_0x4)) {
                s32 r = fn_801CD664(2) + 1;
                field_0x3c = JGeometry::TVec3f(0.0f, -(f32)r, 0.0f);
            } else {
                s32 r = fn_801CD664(3) + 1;
                field_0x3c = JGeometry::TVec3f(0.0f, 0.0f, (f32)r);
            }
        } else {
            if (viewLayerFlag(field_0x4)) {
                s32 z = fn_801CD664(3) + 1;
                s32 x = fn_801CD664(5) - 2;
                field_0x3c = JGeometry::TVec3f(x * 0.5f, 0.0f, (f32)z);
            } else {
                s32 z = fn_801CD664(3) + 2;
                s32 y = fn_801CD664(5) - 2;
                s32 x = fn_801CD664(5) - 2;
                field_0x3c = JGeometry::TVec3f(x * 0.5f, y * 0.5f, (f32)z);
            }
        }

        JGeometry::TVec3f hit;
        if (fn_801F2618(&hit, false)) {
            if (hit.x != 0.0f) {
                field_0x3c.x *= -1.0f;
            }
            if (hit.y != 0.0f) {
                field_0x3c.y *= -1.0f;
            }
        }

        if (field_0x234 == 0) {
            // The original re-reads the view row/col and rebuilds the key (field_0x270 above) here.
            s32 row = getGameView_802D6A9C()->field_0x18;
            s32 col = getGameView_802D6A9C()->field_0x14;
            JGeometry::TVec2f pos(mPos.x, mPos.y);
            s32 packed = ((col & 0xFF) << 16) | (field_0x244 << 24) | ((row & 0xFF) << 8) | (field_0x4 & 0xFF);
            getGameView_802D6A9C()->fn_802D6F38(field_0x90, pos, packed, true);
        }
        field_0x248 = 150;
    } else {
        field_0x230 = 0;
        field_0x248 = 0;
    }

    field_0x24c = 0xFF;
    field_0x250 = 1;
    field_0x258 = 0;
    field_0x254 = 180;
    if (gGameRoot_8053AAF8->field_0x7f == 0) {
        if (field_0x234 == 0) {
            field_0x254 = 180;
        } else {
            field_0x254 = 600;
        }
    }

    if (viewLayerFlag(field_0x4) == 1) {
        mPos.y -= mPos.z;
        mPos.z = 0.0f;
        field_0x3c.y -= field_0x3c.z;
        field_0x3c.z = 0.0f;
    }
}

void DroppedItem_804A8C80::vfunc_0x40() {
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x248 = 0;
    if (field_0x254 < 180) {
        field_0x254 = 180;
    }
    field_0x24c = 255;
    field_0x250 = 1;
}

// FIXME: 98.46 %, missing/extra (ours has 4 more instructions). Our tile 13/14 branches store mAlive = false twice
// more than the original, and the `hit.x != 0.0f` test loads the 0.0f literal before hit.x (original: hit.x first).
void DroppedItem_804A8C80::update() {
    JGeometry::TVec2<f32> p(mPos.x, mPos.y);
    JGeometry::TBox2f box(mBox);
    box.addPos(p);
    s32 player = fn_8023AF14(&box, field_0x4, false, 0.0f);

    if (field_0x248 != 0) {
        if (field_0x250 != 0) {
            field_0x24c -= 8;
            if (field_0x24c < 60) {
                field_0x250 = 0;
            }
        } else {
            field_0x24c += 8;
            if (field_0x24c >= 255) {
                field_0x24c = 255;
                field_0x250 = 1;
            }
        }
        field_0x248--;
        if (field_0x248 == 0) {
            field_0x24c = 255;
            field_0x250 = 1;
        }
    }

    switch (field_0x230) {
    case 0:
        mPos += field_0x3c;
        if (viewLayerFlag(field_0x4) == 0) {
            if (mPos.z > 0.0f) {
                field_0x3c.z -= 0.3f;
                JGeometry::TVec3f hit(0.0f, 0.0f, 0.0f);
                if (fn_801F2618(&hit, false)) {
                    if (hit.x != 0.0f) {
                        field_0x3c.x *= -1.0f;
                    }
                    if (hit.y != 0.0f) {
                        field_0x3c.y *= -1.0f;
                    }
                }
            } else if (mPos.z <= 0.0f) {
                mPos.z = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
            }
        } else {
            JGeometry::TVec3f hit;
            if (fn_801F2618(&hit, false) == 1) {
                if (hit.y != 0.0f) {
                    field_0x3c.z = 0.0f;
                    field_0x3c.y = 0.0f;
                    field_0x3c.x = 0.0f;
                }
            }
            field_0x3c.y += 0.15f;
        }

        fn_802C0570();

        if (player >= 0) {
            if (fn_8022C0A0(player)) {
                if (fn_8023E8F0(player)) {
                    if (fn_802BFF30(player) == 1) {
                        field_0xb0 = 0;
                        field_0x230 = 1;
                        break;
                    }
                }
            } else if (gGameRoot_8053AAF8->field_0x7f == 1) {
                if (fn_8022CAF8(player) != 4 || !fn_8022F478(player)) {
                    field_0x260 = player;
                    if (fn_8022F514(player)) {
                        field_0x260 = fn_8022F3DC(player);
                    }
                    if (!getGameView_802D6A9C()->fn_802D46D8(field_0x260, 0x151, false) && fn_802DE7CC()) {
                        field_0x25c = player;
                        if (fn_8022CAF8(player) != 4) {
                            field_0x25c = field_0x260;
                        }
                        if (gGameRoot_8053AAF8->field_0x7e == 1) {
                            if (fn_802308E0(field_0x260) == 1) {
                                fn_8022FE80(field_0x25c, field_0x4, mSerial, false, false);
                                field_0x230 = 2;
                                break;
                            }
                        } else if (fn_8023077C() == 1) {
                            fn_8022FE80(field_0x25c, field_0x4, mSerial, false, false);
                            field_0x230 = 2;
                            break;
                        }
                    }
                }
            }
        }

        {
            u16 tile = fn_8022461C(&mPos, field_0x4);
            if (tile == 3 || tile == 14 || tile == 13) {
                if (tile == 14) {
                    if (fn_802DE9A0()) {
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, 0, 0, NULL, NULL, true, 1.0f);
                        startSound_801F0E34(0x5005F, 0);
                        mAlive = false;
                        break;
                    }
                    if (fn_802DEB58()) {
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x45E, -1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x45F, 0, 0, NULL, NULL, true, 1.0f);
                        startSound_801F0E34(0x5003F, 0);
                        mAlive = false;
                        break;
                    }
                } else if (tile == 13) {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 1.0f);
                    startSound_801F0E34(0x30022, 0);
                    mAlive = false;
                    break;
                }
            }
        }

        if (gGameRoot_8053AAF8->field_0x7f == 0) {
            field_0x254--;
            if (field_0x254 <= 120) {
                if (field_0x250 != 0) {
                    field_0x24c -= 8;
                    if (field_0x24c < 60) {
                        field_0x250 = 0;
                    }
                } else {
                    field_0x24c += 8;
                    if (field_0x24c >= 255) {
                        field_0x24c = 255;
                        field_0x250 = 1;
                    }
                }
                if (field_0x254 == 0) {
                    mAlive = false;
                }
            }
        }
        break;

    default:
        mAlive = false;
        if (field_0x234 == 1) {
            fn_801F35AC();
        } else {
            // Same load order as in fn_802BFF30: the original loads these two before the TVec2f argument copy.
            u32 v270 = field_0x270;
            u32 v264 = field_0x264;
            getGameView_802D6A9C()->fn_802D6F38(v264, field_0x268, v270, false);
        }
        break;

    case 2:
        fn_8022FE80(field_0x25c, field_0x4, mSerial, false, false);
        talk(mSerial, field_0x4, mPos);
        field_0x230 = 3;
        break;

    case 3:
        fn_8022FE80(field_0x25c, field_0x4, mSerial, false, false);
        if (fn_80388CAC(field_0x25c, mSerial)) {
            if (gGameRoot_8053AAF8->field_0x7e == 1) {
                if (fn_8023061C(field_0x260, false) == 1) {
                    field_0x230 = 0;
                }
            } else if (fn_80230534(false) == 1) {
                field_0x230 = 0;
            }
        }
        break;
    }
}

// FIXME: 99.43 %, scheduling+regalloc. The diffX / diffY loads and the fabs / frsp / fctiwz pairs are interleaved
// in the opposite order (the original starts with the mPos.y / playerPos.y pair).
void DroppedItem_804A8C80::fn_802C0570() {
    mBox.set(-6.0f, -6.0f, 6.0f, 6.0f);
    JGeometry::TVec3f savedPos(mPos);
    u8 hitFlags = fn_801F2740(-1);
    if (hitFlags != 0) {
        u8 mask = 1;
        for (s32 player = 0; player < 4; player++, mask <<= 1) {
            if (hitFlags & mask) {
                JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(player);
                f32 diffX = mPos.x - playerPos.x;
                f32 diffY = mPos.y - playerPos.y;
                s32 absX = FABS(diffX);
                s32 absY = FABS(diffY);
                s32 dir = fn_80230CFC(player);
                if ((absX >= 0 && absX <= 15 && dir == 0) || dir == 1) {
                    if (mPos.x > playerPos.x) {
                        mPos.x += (15 - absX) >> 1;
                    } else {
                        mPos.x -= (15 - absX) >> 1;
                    }
                }
                if (viewLayerFlag(field_0x4) == 0) {
                    if ((absY >= 0 && absY <= 15 && dir == 3) || dir == 2) {
                        if (mPos.y > playerPos.y) {
                            mPos.y += (15 - absY) >> 1;
                        } else {
                            mPos.y -= (15 - absY) >> 1;
                        }
                    }
                }
            }
        }
    }
    if (hitFlags != 0) {
        if (fn_801F2618(NULL, false) == 1) {
            mPos = savedPos;
        }
    }
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
}

// FIXME: 98.79 %, missing/extra (ours has 1 fewer instruction). The original tests field_0x4 == 0 once more (a dead
// beq, probably from an inlined helper), and the `mPos.z != 0.0f` test loads mPos.z before the 0.0f literal.
void DroppedItem_804A8C80::draw() {
    if (field_0x230 == 1) {
        return;
    }
    s32 layer = field_0x4;
    if (layer == 0) {
        SpriteParams_801F5AE0 params;
        params.field_0x10.a = field_0x24c;
        if (mPos.z != 0.0f) {
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x238 + 0x40, &params, 0, 1.0f, 1.0f);
        } else if (field_0x23c == 2) {
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x238 + 0x20c, &params, 0, 1.0f, 1.0f);
        } else {
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x238 + 0x182, &params, 0, 1.0f, 1.0f);
        }
    } else {
        if (field_0x248 == 0 || (field_0x248 & 3) == 0) {
            if (field_0x254 >= 120 || (field_0x254 != 0 && (field_0x254 & 3) == 0)) {
                if (mPos.z != 0.0f) {
                    fn_803075AC(mSerial, layer, &mPos, field_0x238 + 0x40, 0, 2, -1, false, 0, 0);
                } else if (field_0x23c == 2) {
                    fn_803075AC(mSerial, layer, &mPos, field_0x238 + 0x20c, 0, 2, -1, false, 0, 0);
                } else {
                    fn_803075AC(mSerial, layer, &mPos, field_0x238 + 0x182, 0, 2, -1, false, 0, 0);
                }
            }
        }
    }
    drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 32.0f);
}

// FIXME: 95.55 %, missing/extra (ours has 1 fewer instruction). The original copies player into a second saved
// register (r22, an inlined leader lookup) and orders the 0x30000000 addis / param add differently.
bool DroppedItem_804A8C80::fn_802BFF30(s32 player) {
    s32 leader = player;
    if (fn_8022F514(player)) {
        leader = fn_8022F3DC(player);
    }
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        if (!fn_802308E0(leader)) {
            return false;
        }
    } else {
        if (!fn_8023077C()) {
            return false;
        }
        lbl_8053AD70->fn_8037AE30();
    }

    s32 spriteIdx = field_0x238;
    if (field_0x23c == 2) {
        spriteIdx += 11;
    }

    for (s32 i = 0; i < 4; i++) {
        JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
        JGeometry::TVec3f pos(1.0f + p.x, p.y - 12.0f, p.z);

        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            if (fn_8022F514(i) == 1) {
                if (leader != fn_8022F3DC(i)) {
                    continue;
                }
                if (i == player) {
                    spawnImup(lbl_8046D4F0[spriteIdx] + (player << 26) + (i << 24), i, &pos);
                } else {
                    spawnImup(lbl_8046D4F0[spriteIdx] + (player << 26) + 0x30000000 + (i << 24), i, &pos);
                }
            } else {
                if (i != leader) {
                    continue;
                }
                if (i == player) {
                    spawnImup(lbl_8046D4F0[spriteIdx] + (player << 26) + (i << 24), i, &pos);
                } else {
                    spawnImup(lbl_8046D4F0[spriteIdx] + (player << 26) + 0x30000000 + (i << 24), i, &pos);
                }
            }
        } else {
            if (i == player) {
                spawnImup(lbl_8046D4F0[spriteIdx] + (player << 26) + (i << 24), i, &pos);
            } else {
                spawnImup(lbl_8046D4F0[spriteIdx] + (player << 26) + 0x30000000 + (i << 24), i, &pos);
            }
        }
    }

    if (field_0x244 >= 0) {
        // The original loads these two before the TVec2f argument copy.
        u32 v270 = field_0x270;
        u32 v264 = field_0x264;
        getGameView_802D6A9C()->fn_802D6F38(v264, field_0x268, v270, false);
    }
    return true;
}

// FIXME: 85.12 %, missing/extra (ours has 8 fewer instructions). The original compares the 24 tile codes with a
// linear cmpwi / beq chain; a switch gives a bge decision tree (here), an || chain folds into ranges (78 %).
bool DroppedItem_804A8C80::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if (field_0x258 == 0) {
        s32 serial = mSerial;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
            return true;
        }
        s32 tileCode = fn_8022461C(pos, layer);
        s32 rawCode = fn_80226850(pos, layer);
        if (rawCode == 0x230 || rawCode == 0x231) {
            return false;
        }
        if (rawCode == 0x19E || rawCode == 0x19F || rawCode == 0x350 || rawCode == 0x351 || rawCode == 0x342 ||
            rawCode == 0x343 || rawCode == 0x345 || rawCode == 0x346 || rawCode == 0x355 || rawCode == 0x356 ||
            rawCode == 0x2EE || rawCode == 0x2EF || rawCode == 0x2FE || rawCode == 0x2FF || rawCode == 0x338 ||
            rawCode == 0x339 || rawCode == 0x32A || rawCode == 0x32B || rawCode == 0x32D || rawCode == 0x32E ||
            rawCode == 0x33D || rawCode == 0x33E || rawCode == 0x2CE || rawCode == 0x2CF || rawCode == 0x2DE ||
            rawCode == 0x2DF || rawCode == 0x3B0 || rawCode == 0x3B1 || rawCode == 0x3A2 || rawCode == 0x3A3 ||
            rawCode == 0x3A5 || rawCode == 0x3A6 || rawCode == 0x3B6 || rawCode == 0x398 || rawCode == 0x399 ||
            rawCode == 0x38A || rawCode == 0x38B || rawCode == 0x39D || rawCode == 0x38D || rawCode == 0x38E ||
            rawCode == 0x39E) {
            return true;
        }
        switch (tileCode) {
        case 0:
        case 4:
        case 5:
        case 6:
        case 7:
        case 2:
        case 3:
        case 0x50:
        case 0xE:
        case 0xC:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        case 0x20:
        case 0xD:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x4C:
        case 0x4D:
        case 0x4E:
        case 0x4F:
        case 0x24:
            return false;
        }
        if ((tileCode >= 0x10 && tileCode <= 0x12) || tileCode == 0x13) {
            field_0x258 = 120;
            return false;
        }
        if (fn_80226850(pos, layer) == 0x3A || fn_80226850(pos, layer) == 0x3D) {
            return false;
        }
        // The original returns these two tests through a u8 (extrwi r3, r0, 8, 19 after cntlzw).
        if (tileCode == 0x25) {
            return (u8)!getGameView_802D6A9C()->field_0x171;
        }
        if (tileCode == 0x26) {
            return (u8)(getGameView_802D6A9C()->field_0x171 == 1);
        }
        if (tileCode == 0x22) {
            return getGameView_802D6A9C()->field_0x170 == 1;
        }
        return true;
    } else {
        field_0x258--;
    }
    return false;
}

DroppedItem_804A8C80::~DroppedItem_804A8C80() {}
