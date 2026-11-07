#include "game/GameAudio.h"
#include "game/cls_804A7BE8.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/Camera.h"
#include "game/SpriteParams.h"
#include "JSystem/JUtility/JUTGamePad.h"

// FIXME: 96.58 %. The loop: the original keeps the index in r30 and a byte
// offset (mr r28, r30 after the end load, stepped by 4) for mActors[i]; ours
// keeps the index in r28 and the offset in r30 whatever the declaration order
// (tried: index before/after the end load, actor hoisted). The 30.0f literal is
// lbl_8054075C in the original and an anonymous literal here (data split).
void UnknownFORM_804A7BE8::init() {
    if (fn_8022F514(field_0x90 & 3)) {
        mAlive = false;
        return;
    }
    fn_8022CF08(field_0x90 & 3);
    mPos = *getPlayerPos_8023E724(field_0x90 & 3);
    mPos.z += 30.0f;
    mLife = 0;
    field_0x234 = 1;
    field_0x238 = 0;
    field_0x240 = field_0x23c = fn_8022CAF8(field_0x90 & 3);
    field_0x244 = true;
    field_0x11f = 1;
    fn_801F0D20(0x45);
    field_0x248 = field_0x90 & 3;

    s32 count = 3;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022F514(i)) {
            count--;
        }
    }
    if (count == 0) {
        field_0x24c = true;
        if (viewLayerFlag(field_0x4)) {
            field_0x230 = 0x3F;
        } else {
            field_0x230 = 0x3D;
        }
        if (field_0x23c == 5) {
            field_0x23c = 3;
        }
    } else {
        field_0x24c = false;
        if (viewLayerFlag(field_0x4)) {
            field_0x230 = 0x44;
        } else {
            field_0x230 = 0x43;
        }
        if (field_0x23c == 5) {
            field_0x23c = 0;
        }
    }

    field_0x250 = 0;
    field_0x254 = NULL;
    ActorBase_804A5770* actor;
    s32 actorEnd = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < actorEnd; i++) {
        actor = getActorManager_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == 'FGAL' && actor->mAlive == true) {
            field_0x254 = static_cast<cls_804AE4B0*>(actor);
        }
    }
}

UnknownFORM_804A7BE8::~UnknownFORM_804A7BE8() {
    if (field_0x244 == 0 && field_0x240 != field_0x23c) {
        fn_8022CA6C(field_0x90 & 3, field_0x23c, true);
    }
    if (gGameRoot_8053AAF8->field_0x7e == 0) {
        gGameRoot_8053AAF8->fn_80134FE0(0);
    }
    fn_8022CDD4(field_0x90 & 3);
}

// Sprite-state remap tables (rodata, 6 states each): lbl_8046C400 + 0x00..0x48 remap field_0x23c
// on the two shoulder buttons, 0x60 / 0x78 map field_0x23c to the sprite id (field_0x230).
static const s32 kShoulderLeft[2][6] = {{3, 0, 1, 2, 4, 5}, {1, 0, 2, 3, 4, 5}};
static const s32 kShoulderRight[2][6] = {{1, 2, 3, 0, 4, 5}, {1, 0, 2, 3, 4, 5}};
static const s32 kSpriteId[2][6] = {{0x3A, 0x3B, 0x3C, 0x39, 0x3E, 0x3E}, {0x41, 0x42, 0x39, 0x39, 0x39, 0x40}};

// FIXME(shotgun): 65.77 %, missing/extra
void UnknownFORM_804A7BE8::update() {
    if (field_0x254 != NULL && field_0x254->mState >= 1 && field_0x254->mState <= 10) {
        field_0x254->fn_803703C8();
    }
    if (field_0x250 != 0) {
        field_0x250--;
        if (field_0x250 == 0) {
            mAlive = false;
            return;
        }
        field_0x238 = 0;
    } else {
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
        mPos.z += 30.0f;
        if (field_0x234 > 0) {
            field_0x234--;
            if (viewLayerFlag(field_0x4)) {
                if (field_0x24c) {
                    field_0x230 = 0x3F;
                } else {
                    field_0x230 = 0x44;
                }
            } else {
                if (field_0x24c) {
                    field_0x230 = 0x3D;
                } else {
                    field_0x230 = 0x43;
                }
            }
        } else {
            bool changed = false;
            s32 dir = -1;
            JUTGamePad* pad = JUTGamePad::getGamePad(getPlayerInputSlot_8030AAF4(field_0x90 & 3));
            if (fn_8022C5EC(field_0x90 & 3, 0x100)) {
                field_0x244 = false;
                mAlive = false;
                if (field_0x240 == field_0x23c) {
                    fn_80230EB8(field_0x90 & 3, 0x58);
                }
            } else if (fn_8022C5EC(field_0x90 & 3, 0x200) || fn_8022C5EC(field_0x90 & 3, 0x40)) {
                field_0x244 = true;
                mAlive = false;
                fn_80230EB8(field_0x90 & 3, 0x58);
            } else if (viewLayerFlag(field_0x4)) {
                field_0x23c = 5;
            } else {
                bool viaDir = false;
                if (fn_8022C52C(field_0x90 & 3, 0x40)) {
                    viaDir = true;
                    if (fn_8022C9AC(field_0x90 & 3)) {
                        if (fn_8022C5EC(field_0x90 & 3, 8)) {
                            dir = 0;
                        } else if (fn_8022C5EC(field_0x90 & 3, 4)) {
                            dir = 1;
                        } else if (fn_8022C5EC(field_0x90 & 3, 1)) {
                            dir = 2;
                        } else if (fn_8022C5EC(field_0x90 & 3, 2)) {
                            dir = 3;
                        }
                    }
                } else if (pad != NULL && field_0x244 == true && fn_8022C9AC(field_0x90 & 3) &&
                           pad->mSubStick.mValue > 0.0f) {
                    viaDir = true;
                    s32 angle = pad->mSubStick.mAngle;
                    if (angle < -0x7000 || angle >= 0x7000) {
                        dir = 0;
                    } else if (angle >= -0x5000 && angle < -0x3000) {
                        dir = 2;
                    } else if (angle >= -0x1000 && angle < 0x1000) {
                        dir = 1;
                    } else if (angle >= 0x3000 && angle < 0x5000) {
                        dir = 3;
                    }
                }
                if (viaDir) {
                    if (dir >= 0) {
                        if (dir == 0) {
                            field_0x23c = field_0x24c ? 2 : 1;
                        } else if (dir == 1) {
                            field_0x23c = 1;
                        } else if (dir == 2) {
                            field_0x23c = field_0x24c ? 3 : 0;
                        } else {
                            field_0x23c = 0;
                        }
                        changed = true;
                    }
                    if (changed == true) {
                        field_0x244 = false;
                        if (field_0x240 == field_0x23c) {
                            fn_80230EB8(field_0x90 & 3, 0x58);
                        }
                        field_0x250 = 6;
                        field_0x238 = 0;
                    }
                } else if (fn_8022C5EC(field_0x90 & 3, 1)) {
                    fn_801F0D20(0x46);
                    field_0x23c = kShoulderLeft[field_0x24c ? 0 : 1][field_0x23c];
                } else if (fn_8022C5EC(field_0x90 & 3, 2)) {
                    fn_801F0D20(0x46);
                    field_0x23c = kShoulderRight[field_0x24c ? 0 : 1][field_0x23c];
                }
            }
            field_0x230 = kSpriteId[field_0x24c ? 0 : 1][field_0x23c];
        }
    }
    field_0x238++;
    if (field_0x238 >= 12) {
        field_0x238 = 0;
    }
    if (viewLayerFlag(field_0x4)) {
        if (field_0x24c) {
            field_0x230 = 0x3F;
        } else {
            field_0x230 = 0x44;
        }
    } else if (field_0x238 >= 6) {
        if (field_0x24c) {
            field_0x230 = 0x3D;
        } else {
            field_0x230 = 0x43;
        }
    }
}

// Keeps x inside [lo, hi): below lo it becomes lo, from hi up it becomes hi.
static inline void clampX(f32& x, f32 lo, f32 hi) {
    if (x < lo) {
        x = lo;
    } else if (x >= hi) {
        x = hi;
    }
}

// FIXME(shotgun): 88 %, different + reloc-name
void UnknownFORM_804A7BE8::draw() {
    JGeometry::TBox2f box;
    JGeometry::TVec3f pos(mPos);
    if (field_0x4 == 0) {
        box = gGameRoot2_8053AB10->field_0x24->field_0x164;
        if (pos.y - pos.z < 28.0f + box.i.y) {
            pos.y += 52.0f;
        }
        if (field_0x24c) {
            clampX(pos.x, 68.0f + box.i.x, box.f.x - 60.0f);
        } else {
            clampX(pos.x, 52.0f + box.i.x, box.f.x - 44.0f);
        }
    } else {
        fn_8022B8EC(field_0x248, &box);
        if (pos.y - pos.z < 28.0f + box.i.y) {
            pos.y += 52.0f;
        }
        if (field_0x24c) {
            if (fn_8022C8EC(field_0x90 & 3)) {
                clampX(pos.x, 52.0f + box.i.x, box.f.x - 52.0f);
            } else {
                clampX(pos.x, 80.0f + box.i.x, box.f.x - 80.0f);
            }
        } else {
            if (fn_8022C8EC(field_0x90 & 3)) {
                clampX(pos.x, 36.0f + box.i.x, box.f.x - 36.0f);
            } else {
                clampX(pos.x, 66.0f + box.i.x, box.f.x - 66.0f);
            }
        }
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1c = 2;
    params.field_0x2d = 1;
    s32 id = field_0x230;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
    }
    if (fn_8022C8EC(field_0x90 & 3)) {
        fn_80307AC8(mSerial, field_0x4, &pos, field_0x230, 0, 0, -1);
    } else {
        fn_80307AC8(mSerial, field_0x4, &pos, field_0x230, 1, 0, -1);
    }
}
