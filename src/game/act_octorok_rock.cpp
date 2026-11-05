#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/ActorManager.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_octorok_rock.h"

// .data: the four facing directions' step order (lbl_8049F548), and the
// frame table indexed by facing * 8 + step (lbl_8049F558).
static s32 lbl_8049F548[4] = {0, 2, 1, 3};
static s32 lbl_8049F558[32] = {
    8,  9,  8,  9,  4,  8,  4,  0,  //
    10, 11, 10, 11, 5,  11, 5,  0,  //
    12, 13, 12, 13, 6,  12, 6,  0,  //
    14, 15, 14, 15, 7,  14, 7,  0,  //
};

OctorokRock_8049F5D8::~OctorokRock_8049F5D8() {}

void OctorokRock_8049F5D8::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x10.a = field_0x248;
    s32 state = field_0x90;
    switch (state) {
    case 0:
        if (field_0x4 == 8) {
            GameView_804A9950* view = getGameView_802D6A9C();
            // FIXME: 94.3 %. The original loads the four words of the view's box
            // (0xCC-0xDB) into registers before storing them to the stack copy;
            // here each word is loaded and stored in turn, and the position's
            // scheduling follows from that.
            JGeometry::TBox2f box;
            box = view->mVisibleBox;
            JGeometry::TVec3f pos(mPos.x + box.i.x, mPos.y + box.i.y, mPos.z);
            fn_801EF574("enemy_gc_sob.bin", 0x25, &pos, &params);
            fn_801F02BC("enemy_both2_sob.bin", 0x317, &params, 1, 0, false, 1.0f, 1.0f);
        } else {
            fn_801F02BC("enemy_gc_sob.bin", 0x25, &params, 1, 0, false, 1.0f, 1.0f);
        }
        break;
    case 4:
        params.field_0x30 = 0x11;
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x24c + 0xE5, &params, -1, 1.0f, 1.0f);
        break;
    case 1:
    case 5:
        {
            s32 frame = field_0x24c + 0x4c;
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
            }
        }
        break;
    case 2:
        fn_801F02BC("enemy_both2_sob.bin", field_0x24c + 0x170, &params, 7, 0, false, 1.0f, 1.0f);
        break;
    case 3:
        fn_801F02BC("enemy_both2_sob.bin", 0x1BA, NULL, 0, 0, false, 1.0f, 1.0f);
        break;
    }
}

void OctorokRock_8049F5D8::update() {
    s32 facing;
    bool blocked = false;
    mPos += field_0x3c;
    facing = field_0x98;
    // FIXME: 94.2 %. The original compares field_0x90 signed (cmpwi) in the
    // state 0 tests and the final switch, unsigned elsewhere; the tail's `li 0`
    // and the sprite target's register (r29 here, r30) differ too.
    switch (field_0x230) {
    case 0:
        if (field_0x23c == 0) {
            if (field_0x90 == 5) {
                mAlive = false;
                break;
            }
            blocked = fn_801F2618(NULL, false);
        }
        if (field_0x90 == 2 || field_0x90 == 3) {
            facing = -1;
        }
        if (fn_801F2A10(-1, facing, true, true)) {
            field_0x230 = 1;
            break;
        }
        if (!blocked && !fn_801F2B7C(field_0x114, -1, true)) {
            break;
        }
        if (!blocked) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x24A, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x24B, 0, 1, NULL, NULL, true, 1.0f);
            mAlive = false;
        } else if (field_0x90 != 1 && field_0x90 != 5) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x339, 0, 1, NULL, NULL, true, 1.0f);
            mAlive = false;
        } else {
            startSound_801F0E34(0x30000, 0);
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x240 = 0;
            field_0x234 = 0;
            field_0x230 = 3;
        }
        break;
    case 1:
        startSound_801F0E34(0x400D1, 0);
        if (field_0x90 == 1) {
            field_0x238 = 100;
            switch (field_0x98) {
            case 0:
                field_0x240 = 3;
                break;
            case 1:
                field_0x240 = 1;
                break;
            case 2:
                field_0x240 = 0;
                break;
            case 3:
                field_0x240 = 2;
                break;
            }
            field_0x3c.x *= -0.7f;
            field_0x3c.y *= -0.7f;
            field_0x3c.z = 5.0f;
        } else {
            field_0x3c.x *= -0.3f;
            field_0x3c.y *= -0.3f;
            field_0x3c.z = 3.0f;
        }
        mLife = 0;
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
        field_0x230 = 2;
        break;
    case 2:
        if (field_0x90 == 1 && (field_0x238 & 1) == 0) {
            field_0x240 = (field_0x240 + 1) & 3;
            field_0x98 = lbl_8049F548[field_0x240];
        }
        field_0x248 -= 10;
        if (field_0x248 < 0) {
            field_0x248 = 0;
        }
        field_0x3c.z -= 0.3f;
        if (mPos.z < 0.0f) {
            mAlive = false;
        }
        break;
    case 3:
        if (field_0x234 == 0) {
            field_0x234 = 1;
            field_0x240 = field_0x240 + 1;
            if (field_0x240 == 6) {
                field_0x234 = 30;
            } else if (field_0x240 >= 7) {
                mAlive = false;
            }
        }
        break;
    case 4:
    case 5:
    case 6: {
        field_0xb0 = 0;
        s32 target = field_0x258;
        if (target >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(target);
            if (actor != NULL) {
                mPos.x = field_0x254 * JMASinShort(fn_801F6874(actor->field_0x98) - field_0x25c);
                mPos.y = field_0x254 * JMACosShort(fn_801F6874(actor->field_0x98) - field_0x25c);
                mPos.x += actor->mPos.x;
                mPos.y += actor->mPos.y;
                mPos.z = actor->mPos.z;
                f32 dx = mPos.x - actor->mPos.x;
                f32 dy = mPos.y - actor->mPos.y;
                f32 angle = JMath::atanTable_.atan2_(-dx, -dy) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
                // Through s32 (fctiwz), then the u16 argument (clrlwi 16).
                field_0x98 = fn_801F681C(static_cast<u16>(static_cast<s32>(angle)));
                if (field_0x240 < 6) {
                    field_0x240 = field_0x240 + 1;
                } else if (field_0x230 == 6) {
                    field_0x244 = field_0x244 + 1;
                    if (field_0x244 > 0x14) {
                        mAlive = false;
                    }
                }
                break;
            }
        }
        if (field_0x230 == 4) {
            getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &mPos, 1);
        }
        mAlive = false;
        break;
    }
    }
    if (field_0x234 > 0) {
        field_0x234--;
        if (field_0x230 == 0 && field_0x234 == 0) {
            mAlive = false;
        }
    }
    if (field_0x238 > 0) {
        field_0x238--;
        if (field_0x230 == 0 && field_0x234 == 0) {
            mAlive = false;
        }
    }
    if (field_0x23c > 0) {
        field_0x23c--;
        if (field_0x230 == 0 && field_0x234 == 0) {
            mAlive = false;
        }
    }
    switch (field_0x90) {
    case 2:
    case 4:
        if (field_0x238 == 0) {
            field_0x238 = 3;
            field_0x24c = (field_0x24c + 1) & 3;
        }
        break;
    case 1:
    case 5:
        if (field_0x230 == 0 || field_0x230 == 2) {
            field_0x24c = field_0x98;
        } else {
            field_0x24c = lbl_8049F558[field_0x98 * 8 + field_0x240];
        }
        break;
    }
}

bool OctorokRock_8049F5D8::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
        return true;
    }
    s32 tile = fn_8022461C(pos, layer);
    s32 code = fn_80226850(pos, layer);
    if (code == 0x230 || code == 0x231) {
        return false;
    }
    // FIXME: 68.7 %. The 30 tile codes are one run of compares in the original,
    // in source order (0, 4, 5, 6, 7, 2, 3, ...), each to one shared `li 0`;
    // this switch is emitted sorted, and as a `||` chain MWCC folds 2..7 into a range.
    // The 0x25 / 0x26 returns also differ: the original converts the cntlzw
    // result with extrwi 8,19 where this emits srwi 5.
    switch (tile) {
    case 0x0:
    case 0x4:
    case 0x5:
    case 0x6:
    case 0x7:
    case 0x2:
    case 0x3:
    case 0x50:
    case 0xE:
    case 0xC:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:
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
    default:
        break;
    }
    if (code == 0x3A || code == 0x3D) {
        return false;
    }
    if (tile == 0x25) {
        return getGameView_802D6A9C()->field_0x171 == 0;
    }
    if (tile == 0x26) {
        return getGameView_802D6A9C()->field_0x171 == 1;
    }
    if (tile == 0x22) {
        return getGameView_802D6A9C()->field_0x170 == 1;
    }
    return true;
}

void OctorokRock_8049F5D8::init() {
    mBox.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x80.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x250 = 0;
    field_0x234 = 300;
    field_0x98 = 0;
    field_0x24c = 0;
    field_0x248 = 0xFF;
    s32 mode = field_0x90;
    if (mode == 3) {
        mBox.set(-6.0f, -6.0f, 6.0f, 6.0f);
        field_0x80.set(-6.0f, -6.0f, 6.0f, 6.0f);
    }
    switch (field_0x90) {
    case 5:
        field_0x234 = 10000;
        field_0x23c = 300;
        // fall through
    case 1:
        field_0xb0 = 6;
        break;
    case 2:
        field_0x238 = 3;
        mPos.z = 8.0f;
        field_0xb0 = 7;
        break;
    }
    field_0x230 = 0;
}
