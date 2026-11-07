#include "game/GameAudio.h"
#include "game/DrawShadow.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_arrow.h"
#include "game/GameView.h"

// The arrows each player has shot, the unit's .bss object at 0x804C6330 (declared extern: defining it here changes the static initialiser).
extern s32 lbl_804C6330[4];
extern const u32 lbl_804662E8[5]; // ids of the actors the arrow flies through (rodata)

// 0x80166814.
// FIXME(shotgun): 99.46 %, reloc-name
void ArrowPlayer_804A05E0::init() {
    field_0x98 = (field_0x90 >> 4) & 3;
    field_0x94 = (field_0x90 >> 8) & 0xF;
    field_0x90 = field_0x90 & 3;
    if (fn_8022B160()) {
        s32 player = fn_8022F340(field_0x90);
        lbl_804C6330[player]++;
    }
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mBox.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x80.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    switch (field_0x98) {
    case 0:
        field_0x3c.x = 5.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 1:
        field_0x3c.x = -5.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 2:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 5.0f;
        field_0x3c.z = 0.0f;
        break;
    case 3:
        field_0x3c.x = 0.0f;
        field_0x3c.y = -5.0f;
        field_0x3c.z = 0.0f;
        break;
    }
    field_0x234 = 255.0f;
    field_0x24c = 500;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    mLife = 10000;
    field_0x268 = field_0x98;
    field_0x230 = 0;
    field_0x274 = 0;
}

// 0x8016636C. Whether the arrow is blocked at pos: by a collision box, or by the tile there.
// FIXME(shotgun): 88.86 %, missing/extra
bool ArrowPlayer_804A05E0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, lbl_804662E8) == true) {
        return true;
    }
    u16 code = fn_8022461C(pos, layer);
    s32 tile = fn_80226850(pos, layer);
    if ((u32)(code - 0x10) <= 7 || (tile >= 0x140 && tile <= 0x15F)) {
        field_0x23c = 0;
        field_0x238 = 1;
        return false;
    }
    if ((field_0x94 == 0 || field_0x94 == 4) && tile >= 0x40 && tile <= 0x9A) {
        return false;
    }
    if ((u32)(tile - 0x230) <= 1 || tile == 0x234) {
        field_0x238 = 0;
        return false;
    }
    if (code != 0 && code != 4 && code != 5 && code != 6 && code != 7 && code != 2 && code != 3 &&
        code != 0x50 && code != 0xE && code != 0xC && code != 0x27 && code != 0x28 && code != 0x29 &&
        code != 0x2A && code != 0x2B && code != 0x2C && code != 0x2D && code != 0x2E && code != 0x2F &&
        code != 0x20 && code != 0xD && code != 0x39 && code != 0x3A && code != 0x3B && code != 0x4C &&
        code != 0x4D && code != 0x4E && code != 0x4F && code != 0x24) {
        if ((u32)(code - 7) <= 1 || code == 0xA) {
            if (field_0x98 == 1) {
                return false;
            }
            if (field_0x238 == 0) {
                field_0x240++;
                if (field_0x240 > 20) {
                    return true;
                }
            }
            return false;
        }
        if (code == 6 || code == 9 || code == 0xB) {
            if (field_0x98 == 0) {
                return false;
            }
            if (field_0x238 == 0) {
                field_0x240++;
                if (field_0x240 > 20) {
                    return true;
                }
            }
            return false;
        }
        if (code == 4) {
            if (field_0x98 == 2) {
                return false;
            }
            if (field_0x238 == 0) {
                field_0x240++;
                if (field_0x240 > 20) {
                    return true;
                }
            }
            return false;
        }
        if (code == 5) {
            if (field_0x98 == 3) {
                return false;
            }
            if (field_0x238 == 0) {
                field_0x240++;
                if (field_0x240 > 20) {
                    return true;
                }
            }
            return false;
        }
        if (fn_80226850(pos, layer) == 0x3A || fn_80226850(pos, layer) == 0x3D) {
            field_0x238 = 0;
            return false;
        }
        if (tile == 0x14) {
            field_0x238 = 0;
            return false;
        }
        if (code == 0x25) {
            if (field_0x238 == 0 && getGameView_802D6A9C()->field_0x171 == 0) {
                return true;
            }
            field_0x238 = 0;
            return false;
        }
        if (code == 0x26) {
            if (field_0x238 == 0 && getGameView_802D6A9C()->field_0x171 == 1) {
                return true;
            }
            field_0x238 = 0;
            return false;
        }
        if (field_0x238 == 0) {
            if (code != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
                return true;
            }
        }
        if (field_0x238 == 1) {
            return false;
        }
    }
    if (field_0x238 == 1) {
        field_0x23c++;
        if (field_0x23c > 11) {
            field_0x23c = 0;
            field_0x238 = 0;
        }
    }
    return false;
}

// 0x801662D8.
ArrowPlayer_804A05E0::~ArrowPlayer_804A05E0() {
    for (int i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// 0x8016545C.
// FIXME(shotgun): 99.39 %, missing/extra + reloc-name
void ArrowPlayer_804A05E0::draw() {
    if (field_0x94 != 3) {
        if (field_0x24c >= 50 || (field_0x24c & 2) == 0) {
            SpriteParams_801F5AE0 params;
            params.field_0x10.a = (u8)(s32)field_0x234;
            if (fn_8022B160() == true) {
                switch (field_0x90) {
                case 0:
                    params.field_0x30 = 0;
                    break;
                case 1:
                    params.field_0x30 = 1;
                    break;
                case 2:
                    params.field_0x30 = 2;
                    break;
                case 3:
                    params.field_0x30 = 3;
                    break;
                }
            }
            s32 frame = field_0x268 + 0x4C;
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
            }
            s32 base = 0x4C;
            if ((field_0x94 == 0 || field_0x94 == 4) && field_0x230 == 0) {
                base = 0x2DF;
            }
            fn_803075AC(mSerial, field_0x4, &mPos, base + field_0x268, false, 2, -1, 0, 0, 0);
        }
        if ((field_0x94 == 2 || field_0x94 == 4) && field_0x230 != 4) {
            drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 32.0f);
        }
    }
}
