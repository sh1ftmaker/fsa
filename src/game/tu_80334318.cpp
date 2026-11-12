#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/tu_801F9484.h"
#include "game/GameGlobals.h"
#include "game/RotMatrix.h"
#include "game/BgRewrite.h"
#include "game/Player.h"
#include "game/VecEpsilon.h"
#include "JSystem/JMath/JMath.h"
#include <float.h>

// Breaks the 'SBAL' tiles `radius` ahead of the ball along `angle`.
// FIXME(shotgun): the original adds a whole TVec3f offset whose z is never set: pos.z gets
// the stack word at sp+0x18 added (psq_l / ps_add / psq_st, 4 instructions per call), and
// nothing in update ever writes that word. Reading it would be undefined behaviour, so only
// x and y are added here; that is the one intended deviation from the asm.
static inline void breakTiles(IronBallHoming_804AB9C0* self, u16 angle, f32 radius) {
    JGeometry::TVec3f pos(self->mPos);
    pos.x += radius * JMath::sincosTable_.sinShort(angle);
    pos.y += radius * JMath::sincosTable_.cosShort(angle);
    gBgRewrite_8053AC98->fn_802A1688(&pos, self->field_0x4, 'SBAL');
}

// States 1 and 2 turn toward the last player position seen (field_0x260), or away from
// a wall (fn_801F2618) for 10 frames.
static inline void steer(IronBallHoming_804AB9C0* self, u8 hitWall, s32 player) {
    if (self->field_0x254 == 0 && player >= 0) {
        if (hitWall) {
            self->field_0x26c += 0x2000;
            self->field_0x254 = 10;
        }
    } else if (epsilonEquals(self->field_0x260, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 32.0f * FLT_EPSILON)) {
        self->field_0x26c = 0;
    } else {
        self->field_0x26c = (s32)(JMath::atanTable_.atan2_(self->field_0x260.x - self->mPos.x,
                                                           self->field_0x260.y - self->mPos.y) *
                                  10430.378f);
    }
}

// 0x8033571C.
void IronBallHoming_804AB9C0::init() {
    mLife = 0x80;
    field_0xb0 = 0x42304;
    if (field_0x90 & 1) {
        field_0x250 = 1;
        mBox.set(-8.0f, -4.0f, 8.0f, 12.0f);
        field_0x80.set(-14.0f, -8.0f, 14.0f, 20.0f);
    } else {
        field_0x250 = 0;
        mBox.set(-4.0f, -3.0f, 4.0f, 5.0f);
        field_0x80.set(-7.0f, -6.0f, 7.0f, 8.0f);
    }
    field_0x94 = (field_0x90 >> 4) & 1;
    field_0x234 = 0.0f;
    field_0x238 = 0.0f;
    field_0x23c = 0.0f;
    field_0x240 = 1.0f;
    field_0x260.z = 0.0f;
    field_0x260.y = 0.0f;
    field_0x260.x = 0.0f;
    if (field_0x94 == 0) {
        field_0x230 = 0;
    } else {
        field_0x230 = 5;
        mPos.z = 100.0f;
    }
    field_0x24c = 0;
    field_0x248 = 10;
    field_0x244 = 100;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x26c = 0;
    field_0xb8 = 1;
}

// 0x80335688.
// FIXME(shotgun): 85.76 %, different
bool IronBallHoming_804AB9C0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 tile = fn_8022461C(pos, layer);
    if (field_0x230 != 3 && (tile == 0xC || tile == 3 || tile == 0xE)) {
        if (field_0x230 != 7) {
            field_0x25c = 30;
        }
        field_0x230 = 7;
        return false;
    }
    if (tile == 0 || tile == 0x22) {
        return false;
    }
    return true;
}

// 0x80334A04.
// FIXME(shotgun): 82.95 %, missing/extra + reloc-name
void IronBallHoming_804AB9C0::update() {
    u16 angle = 0;
    s32 player;
    mPos.add(field_0x3c);
    if (field_0x90 & 1) {
        breakTiles(this, field_0x26c, 25.0f);
        breakTiles(this, field_0x26c + 0x1500, 25.0f);
        breakTiles(this, field_0x26c - 0x1500, 25.0f);
    } else {
        breakTiles(this, field_0x26c, 13.0f);
        breakTiles(this, field_0x26c + 0x1500, 13.0f);
        breakTiles(this, field_0x26c - 0x1500, 13.0f);
    }
    u8 hitWall = fn_801F2618(NULL, false);
    fn_801EEC98(mSerial, &angle, NULL, NULL, &player, 0, 1);
    if (player >= 0) {
        field_0x260 = *getPlayerPos_8023E724(player);
    }
    if (0.0f == mPos.z && field_0x230 != 7) {
        bool hit;
        if (field_0x250 == 1) {
            hit = fn_801F2B7C(field_0x114, -1, 5) != 0;
        } else {
            hit = fn_801F2B7C(field_0x114, -1, 4) != 0;
        }
        if (hit) {
            field_0x3c.x = -field_0x3c.x;
            field_0x3c.y = -field_0x3c.y;
            field_0x3c.z = 0.0f;
            field_0x244 = 20;
            field_0x230 = 3;
        }
    }
    switch (field_0x230) {
    case 0:
        field_0x244--;
        if (field_0x244 <= 0) {
            field_0x230 = 1;
            field_0x244 = 0;
        }
        break;
    case 1:
        angle = 0;
        steer(this, hitWall, player);
        field_0x254--;
        field_0x244++;
        field_0x23c = (f32)(field_0x244 * field_0x244) / 25.0f;
        if (field_0x244 >= 5) {
            field_0x230 = 2;
        }
        if (player >= 0 ||
            !epsilonEquals(field_0x260, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 32.0f * FLT_EPSILON)) {
            field_0x3c.x = field_0x23c * JMath::sincosTable_.sinShort(field_0x26c);
            field_0x3c.y = field_0x23c * JMath::sincosTable_.cosShort(field_0x26c);
        } else {
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
        }
        field_0x248--;
        break;
    case 2:
        angle = 0;
        steer(this, hitWall, player);
        field_0x254--;
        if (player >= 0 ||
            !epsilonEquals(field_0x260, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 32.0f * FLT_EPSILON)) {
            field_0x3c.x = JMath::sincosTable_.sinShort(field_0x26c);
            field_0x3c.y = JMath::sincosTable_.cosShort(field_0x26c);
        } else {
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
        }
        field_0x248--;
        break;
    case 3:
    case 4:
        field_0x244--;
        if (field_0x3c.squared() > 0.01f) {
            addCalc2_801F71A4(&field_0x3c.x, 0.0f, 0.05f, 0.05f);
            addCalc2_801F71A4(&field_0x3c.y, 0.0f, 0.05f, 0.05f);
        }
        if (field_0x244 <= 0) {
            field_0x230 = 0;
            field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
            field_0x244 = 90;
        }
        break;
    case 5:
        if (vfunc_0x34()) {
            mPos.z = 100.0f;
            field_0x230 = 6;
        }
        break;
    case 6:
        field_0x3c.z -= 0.5f;
        if (field_0x3c.z <= -6.0f) {
            field_0x3c.z = -6.0f;
        }
        if (mPos.z <= 0.0f && field_0x3c.z < 0.0f) {
            mPos.z = field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
            field_0x230 = 0;
            field_0x244 = 40;
            if (field_0x90 & 1) {
                getGameView_802D6A9C()->fn_802D79E8(field_0x4, 8.0f, 0.0f, false);
            } else {
                getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
            }
            startSound_801F0E34(0x3000F, 0);
        }
        break;
    case 7:
        if (field_0x25c > 0) {
            field_0x25c--;
        } else {
            field_0x240 -= 0.02f;
            field_0x3c.scale(0.98f);
            if (field_0x240 < 0.1f) {
                field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
                if (vfunc_0x54() < 0) {
                    fn_801F3668();
                } else {
                    field_0x240 = 1.0f;
                    mPos.z = 1000.0f;
                    field_0x230 = 6;
                }
            }
        }
        break;
    }
    if (field_0x230 == 1 || field_0x230 == 2) {
        if (field_0x248 <= 5) {
            field_0x24c = 1;
            if (field_0x248 <= 0) {
                field_0x248 = 10;
                field_0x24c = 0;
            }
        }
        if (field_0x258 <= 0) {
            startSound_801F0E34(0x30019, 0);
            field_0x258 = 70;
        }
        field_0x258--;
    }
}

// 0x80334488.
// FIXME(shotgun): 99.46 %, regalloc-only
void IronBallHoming_804AB9C0::draw() {
    if (field_0x230 == 5) {
        return;
    }
    SpriteParams_801F5AE0 shadow;
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    PSMTXIdentity(mtx);
    shadow.field_0x1d = -1;
    if (field_0x230 == 7) {
        JGeometry::TVec3f rot(0.0f, 0.0f, 0.0f);
        setRotScaleMtx(mtx, rot, field_0x240);
        params.mMtx = mtx;
    }
    if (field_0x250 == 0) {
        if (field_0x230 != 7) {
            JGeometry::TVec3f pos(mPos.x, 3.0f + mPos.y, 0.0f);
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "kage_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 0, &shadow, 1.0f, 1.0f);
            }
        }
        s32 frame = field_0x24c + 0x25;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        fn_80307AC8(mSerial, field_0x4, &mPos, field_0x24c + 0x25, 0, 0, -1);
    } else {
        if (field_0x230 != 7) {
            JGeometry::TVec3f pos(mPos.x, 12.0f + mPos.y, 0.0f);
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "kage_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, 3, &shadow, 2.0f, 1.0f);
            }
        }
        s32 frame = field_0x24c + 0x27;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
        fn_80307AC8(mSerial, field_0x4, &mPos, field_0x24c + 0x27, 0, 0, -1);
    }
}

// 0x80334378.
// FIXME(shotgun): 85.24 %, missing/extra
s32 IronBallHoming_804AB9C0::vfunc_0x54() {
    s32 serial = -1;
    while (true) {
        serial = getActorManager_801F856C()->fn_801F78F4('WARP', serial);
        if (serial < 0) {
            break;
        }
        ActorBase_804A5770* warp = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (warp == NULL) {
            continue;
        }
        u32 param = warp->field_0x90;
        if ((param & 0xFF) != 7) {
            continue;
        }
        s32 layer = (param >> 3) & 7;
        f32 baseX;
        f32 baseY;
        if (layer == 0) {
            baseX = getGameView_802D6A9C()->mVisibleBox.i.x;
            baseY = getGameView_802D6A9C()->mVisibleBox.i.y;
        } else {
            baseX = 0.0f;
            baseY = 0.0f;
            layer += 7;
        }
        JGeometry::TVec3f dest(8.0f + ((f32)((param >> 13) & 0x1F8) + baseX), 8.0f + ((f32)((param >> 5) & 0x1F8) + baseY), 0.0f);
        fn_801F349C(layer, &dest);
        break;
    }
    return serial;
}

// Destructor at 0x80334318.
IronBallHoming_804AB9C0::~IronBallHoming_804AB9C0() {}
