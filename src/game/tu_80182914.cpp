#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/ViewLayer.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/tu_80212AC0.h"
#include "game/tu_80182914.h"
#include "game/act/tu_80182914.h"

// The emitter's colour comes from field_0x238 while view layer 0x13 is flagged.
static inline void setEmitterColour(cls_804A1718* self, JPABaseEmitter* emitter) {
    if (viewLayerFlag(0x13) == 1) {
        emitter->setGlobalPrmColor(self->field_0x238[0], self->field_0x238[1], self->field_0x238[2]);
    }
}

// Moves mEmitter274 (if any) to the actor and gives it the colour.
static inline void followEmitter(cls_804A1718* self) {
    if (self->mEmitter274 != NULL) {
        self->mEmitter274->setGlobalTranslation(self->mPos);
        if (viewLayerFlag(0x13) == 1) {
            self->mEmitter274->setGlobalPrmColor(self->field_0x238[0], self->field_0x238[1], self->field_0x238[2]);
        }
    }
}

// The actor was hit (or is held): the emitter follows it and the rest of update() is skipped.
static inline void followHeld(cls_804A1718* self) {
    followEmitter(self);
    self->fn_801F2618(NULL, false);
}

void cls_804A1718::draw() {
    s32 flags = -1;
    s32 state = field_0x230;
    s32 frame = field_0x254;
    if ((state == 0 || state == 1) && field_0x23c == 0) {
        return;
    }
    if (field_0x258 != 0) {
        flags = 0;
    }
    if (field_0x25c == 1 && frame >= 3) {
        frame += 5;
    }
    fn_801F06F0("enemy_gc_sob.bin", frame + 0x158, NULL, flags, 1.0f, 1.0f);
}

// FIXME: 99.95 %. Same instructions; the jump-table base is relocated to the
// compiler's label (@959) where the original uses jumptable_804A176C, and try.py
// also reports one scheduling difference.
void cls_804A1718::update() {
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

    fn_801F71A4(&field_0x3c.x, field_0x268.x, 0.3f, 1.0f);
    fn_801F71A4(&field_0x3c.y, field_0x268.y, 0.3f, 1.0f);
    mPos += field_0x3c;
    fn_80212AC0()->fn_80212AF0(&mPos);
    fn_801EEC98(mSerial, &angle, NULL, &dist, &player, 0, 0);
    if (field_0x258 != 0) {
        fn_801F2B7C(-100, -1, 1);
    }
    fn_801F2618(NULL, false);

    switch (field_0x230) {
    case 0:
        if (field_0x260 == 1) {
            if (vfunc_0x34()) {
                field_0x230 = 2;
            }
            break;
        }
        field_0x240 = 100;
        field_0x230 = 1;
        // fall through
    case 1:
        field_0x268.x = 0.5f * JMath::sincosTable_.sinShort(angle);
        field_0x268.y = 0.5f * JMath::sincosTable_.cosShort(angle);
        if (field_0x240 != 0) {
            break;
        }
        field_0x230 = 2;
        // fall through
    case 2:
        field_0xb8 = 1;
        if (mEmitter274 == NULL) {
            JPABaseEmitter* puff = fn_8013CC50(field_0x4, &mPos, 0x1DA, 0, 1, NULL, NULL, true, 1.0f);
            setEmitterColour(this, puff);
            mEmitter274 = fn_8013CC50(field_0x4, &mPos, 0x1DB, 0, 1, NULL, NULL, true, 1.0f);
            if (viewLayerFlag(0x13) == 1) {
                mEmitter274->setGlobalPrmColor(field_0x238[0], field_0x238[1], field_0x238[2]);
            }
        }
        field_0x268.set(0.0f, 0.0f, 0.0f);
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        field_0x23c = 6;
        field_0x248 = 0;
        field_0x254 = 0;
        field_0x230 = 3;
        // fall through
    case 3:
        if (field_0x23c == 0) {
            field_0x254 += 1;
            field_0x23c = 6;
            if (field_0x254 > 5) {
                field_0x230 = 4;
            }
        }
        followEmitter(this);
        break;
    case 4:
        field_0xb0 = 0x11101;
        field_0x240 = 200;
        field_0x254 = 6;
        field_0x23c = 8;
        field_0x248 = 0;
        field_0x258 = 1;
        field_0x230 = 5;
        // fall through
    case 5: {
        followEmitter(this);
        f32 scale;
        u16 maxStep;
        if (field_0x25c == 1) {
            scale = 1.0f;
            maxStep = 0x300;
        } else {
            scale = 0.5f;
            maxStep = 0x100;
        }
        fn_801F714C(&field_0x264, angle, 1, maxStep);
        field_0x268.x = scale * JMath::sincosTable_.sinShort(field_0x264);
        field_0x268.y = scale * JMath::sincosTable_.cosShort(field_0x264);
        if (field_0x23c == 0) {
            field_0x254 += 1;
            if (field_0x254 > 7) {
                field_0x254 = 6;
            }
            field_0x23c = 8;
        }
        if (field_0x240 != 0) {
            break;
        }
        field_0x230 = 6;
    }
        // fall through
    case 6:
        followEmitter(this);
        field_0xb0 = 0x1000;
        field_0x268.set(0.0f, 0.0f, 0.0f);
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        field_0x258 = 0;
        field_0x23c = 4;
        field_0x248 = 0;
        field_0x254 = 6;
        field_0x230 = 7;
        // fall through
    case 7:
        followEmitter(this);
        if (field_0x23c == 0) {
            field_0x254 -= 1;
            field_0x23c = 6;
            if (field_0x254 <= 0) {
                if (mEmitter274 != NULL) {
                    JPABaseEmitter* puff = fn_8013CC50(field_0x4, &mPos, 0x1DA, 0, 1, NULL, NULL, true, 1.0f);
                    setEmitterColour(this, puff);
                    JPABaseEmitter* emitter = mEmitter274;
                    emitter->stopCreateParticle();
                    emitter->mMaxFrame = 1;
                    mEmitter274 = NULL;
                }
                field_0x23c = 3;
                field_0x230 = 0;
            }
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

cls_804A1718::~cls_804A1718() {
    fn_801EE434(mEmitter274);
}

void cls_804A1718::vfunc_0x18() {
    fn_80212AC0()->fn_80212AF0(&mPos);
    ActorBase_804A5770::vfunc_0x18();
}

void cls_804A1718::vfunc_0x44() {
    if (field_0x258 != 0) {
        return;
    }
    if (field_0x230 == 5) {
        return;
    }
    field_0xb0 = 0x11101;
    field_0x254 = 6;
    field_0x230 = 4;
}

void cls_804A1718::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x25c = field_0x90 & 1;
    field_0x260 = (field_0x90 >> 8) & 1;
    field_0x254 = 0;
    field_0x238[0] = 0;
    field_0x238[1] = 0;
    field_0x238[2] = 0;
    if (viewLayerFlag(0x13) == 1) {
        field_0x234 = fn_8018330C();
        field_0x238[0] = field_0x234.toUInt32() >> 24;
        field_0x238[1] = (field_0x234.toUInt32() >> 16) & 0xFF;
        field_0x238[2] = (field_0x234.toUInt32() >> 8) & 0xFF;
    }
    field_0x268.zero();
    field_0x3c.zero();
    field_0x258 = 0;
    field_0x264 = 0;
    field_0x195 = 1;
    mEmitter274 = NULL;
    field_0x230 = 0;
}

// Returns the colour of the cls_804A0160 at lbl_8053AB10->0x2C.
JUtility::TColor fn_8018330C() {
    return lbl_8053AB10->field_0x2c->field_0x4;
}
