#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/ViewLayer.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/tu_80212AC0.h"
#include "game/tu_80182914.h"
#include "game/act/tu_80183544.h"

// Auto-inlining is off for the whole file (docs/INLINING.md, pragma rule 2): the
// original inlines no same-unit plain function (init calls fn_8018330C of tu_80182914 out of line).
#pragma auto_inline off

// The emitter's colour comes from field_0x238 while view layer 0x13 is flagged.
static inline void setEmitterColour(cls_804A1790* self, JPABaseEmitter* emitter) {
    if (viewLayerFlag(0x13) == 1) {
        emitter->setGlobalPrmColor(self->field_0x238[0], self->field_0x238[1], self->field_0x238[2]);
    }
}

// Moves mEmitter270 (if any) to the actor and gives it the colour.
static inline void followEmitter(cls_804A1790* self) {
    if (self->mEmitter270 != NULL) {
        self->mEmitter270->setGlobalTranslation(self->mPos);
        if (viewLayerFlag(0x13) == 1) {
            self->mEmitter270->setGlobalPrmColor(self->field_0x238[0], self->field_0x238[1], self->field_0x238[2]);
        }
    }
}

// The actor was hit (or is held): the emitter follows it and the rest of update() is skipped.
static inline void followHeld(cls_804A1790* self) {
    followEmitter(self);
    self->fn_801F2618(NULL, false);
}

// Structure after tu_80182914.cpp (cls_804A1718::init), a sibling actor.
void cls_804A1790::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x238[0] = 0;
    field_0x238[1] = 0;
    field_0x238[2] = 0;
    if (viewLayerFlag(0x13) == 1) {
        field_0x234 = fn_8018330C();
        field_0x238[0] = field_0x234.toUInt32() >> 24;
        field_0x238[1] = (field_0x234.toUInt32() >> 16) & 0xFF;
        field_0x238[2] = (field_0x234.toUInt32() >> 8) & 0xFF;
    }
    field_0x25c = (field_0x90 >> 8) & 1;
    field_0x195 = 1;
    field_0x254 = 2;
    field_0x264.zero();
    field_0x3c.zero();
    field_0x258 = 0;
    field_0x260 = 0;
    mEmitter270 = NULL;
    field_0x230 = 0;
}

void cls_804A1790::vfunc_0x44() {
    if (field_0x258 != 0) {
        return;
    }
    if (field_0x230 == 5) {
        return;
    }
    field_0xb0 = 0x11101;
    field_0x254 = 0;
    field_0x230 = 4;
}

cls_804A1790::~cls_804A1790() {
    releaseEmitter_801EE434(mEmitter270);
}

void cls_804A1790::vfunc_0x18() {
    fn_80212AC0()->fn_80212AF0(&mPos);
    ActorBase_804A5770::vfunc_0x18();
}

void cls_804A1790::update() {
    u16 angle = 0;
    f32 dist = 0.0f;
    s32 player = -1;

    if (fn_801F3D94(1) != 0) {
        followHeld(this);
        return;
    }
    if (field_0x258 != 0 && fn_801F305C()) {
        followHeld(this);
        return;
    }

    addCalc2_801F71A4(&field_0x3c.x, field_0x264.x, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x264.y, 0.3f, 1.0f);
    mPos += field_0x3c;
    fn_80212AC0()->fn_80212AF0(&mPos);
    fn_801EEC98(mSerial, &angle, NULL, &dist, &player, 0, 0);
    if (field_0x258 != 0) {
        fn_801F2B7C(-100, -1, 1);
    }
    fn_801F2618(NULL, false);

    switch (field_0x230) {
    case 0:
        if (field_0x25c == 1) {
            if (vfunc_0x34()) {
                field_0x230 = 2;
            }
            break;
        }
        field_0x240 = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 30;
        field_0x230 = 1;
        // fall through
    case 1:
        if (field_0x240 != 0) {
            break;
        }
        if (dist > 64.0f) {
            break;
        }
        field_0x230 = 2;
        // fall through
    case 2:
        startSound_801F0E34(0x40097, 0);
        field_0xb8 = 1;
        if (mEmitter270 == NULL) {
            JPABaseEmitter* puff = spawnEmitter_8013CC50(field_0x4, &mPos, 0x1DA, 0, 1, NULL, NULL, true, 1.0f);
            setEmitterColour(this, puff);
            mEmitter270 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x1DB, 0, 1, NULL, NULL, true, 1.0f);
            if (viewLayerFlag(0x13) == 1) {
                mEmitter270->setGlobalPrmColor(field_0x238[0], field_0x238[1], field_0x238[2]);
            }
        }
        field_0x264.zero();
        field_0x3c.zero();
        field_0x23c = 8;
        field_0x248 = 0;
        field_0x254 = 2;
        field_0x230 = 3;
        // fall through
    case 3:
        if (field_0x23c != 0) {
            break;
        }
        followEmitter(this);
        field_0x230 = 4;
        // fall through
    case 4:
        field_0xb0 = 0x11101;
        field_0x23c = 8;
        field_0x240 = 200;
        field_0x248 = 0;
        field_0x254 = 0;
        field_0x258 = 1;
        field_0x230 = 5;
        // fall through
    case 5:
        followEmitter(this);
        addCalcAngle2_801F714C(&field_0x260, angle, 1, 0x200);
        field_0x264.x = 0.7f * JMath::sincosTable_.sinShort(field_0x260);
        field_0x264.y = 0.7f * JMath::sincosTable_.cosShort(field_0x260);
        if (field_0x23c == 0) {
            field_0x254 += 1;
            field_0x254 &= 1;
            field_0x23c = 8;
        }
        if (field_0x240 != 0) {
            break;
        }
        field_0x230 = 6;
        // fall through
    case 6:
        if (mEmitter270 != NULL) {
            JPABaseEmitter* puff = spawnEmitter_8013CC50(field_0x4, &mPos, 0x1DA, 0, 1, NULL, NULL, true, 1.0f);
            setEmitterColour(this, puff);
            JPABaseEmitter* emitter = mEmitter270;
            emitter->stopCreateParticle();
            emitter->mMaxFrame = 1;
            mEmitter270 = NULL;
        }
        field_0xb0 = 0x1000;
        field_0x264.zero();
        field_0x3c.zero();
        field_0x258 = 0;
        field_0x23c = 8;
        field_0x248 = 0;
        field_0x254 = 2;
        field_0x230 = 7;
        // fall through
    case 7:
        if (field_0x23c == 0) {
            field_0x230 = 0;
        }
        break;
    }

    if (field_0x23c > 0) {
        field_0x23c -= 1;
    }
    if (field_0x240 > 0) {
        field_0x240 -= 1;
    }
    if (field_0x244 > 0) {
        field_0x244 -= 1;
    }
}

void cls_804A1790::draw() {
    s32 state = field_0x230;
    if (state == 0 || state == 1) {
        return;
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x254 + 0x155, NULL, -1, 1.0f, 1.0f);
}
