#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JUtility/JUTVideo.h"

// The texture name fn_80119D48 loads (.sdata 0x80539F10).
const char* lbl_80539F10 = "kk_black.bti";

inline cls_80119E5C::cls_80119E5C() {
    field_0x0 = 0;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    field_0x10 = 0;
    field_0x14 = 0.0f;
    field_0x18 = 0.0f;
    field_0x1c = 0;
    field_0x20 = 0;
    field_0x24 = 0;
    field_0x28 = 0;
    field_0x2c = NULL;
}

// The accessor of the fade object; fn_80119DD0 is its out-of-line copy (callers elsewhere).
static inline cls_80119E5C* fadeObject() {
    static cls_80119E5C instance;
    return &instance;
}

// The fade length for a mode: 0x23 for modes 0 and 1, 0x46 otherwise.
static inline s32 fadeFrames(s32 mode) {
    switch (mode) {
    case 0:
    case 1:
        return 0x23;
    case 2:
    default:
        return 0x46;
    }
}

cls_80119E5C::~cls_80119E5C() {
    if (field_0x2c != NULL) {
        delete field_0x2c;
    }
    field_0x2c = NULL;
}

cls_80119E5C* fn_80119DD0() {
    return fadeObject();
}

// FIXME: 96.9. Regalloc: the original keeps `this` in r30 and the picture in r29, the texture
// in r31; here the constructor result takes an extra move through r0.
void cls_80119E5C::fn_80119D48() {
    if (field_0x2c == NULL) {
        void* texture = gGameRoot_8053AAF8->field_0xa4->getResource('????', lbl_80539F10);
        field_0x2c = new J2DPicture((ResTIMG*)texture);
    }
}

void cls_80119E5C::fn_80119998(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    field_0x0 = 1;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x4 = 0;
    field_0x8 = a;
    field_0xc = b;
    field_0x1c = c;
    field_0x20 = d;
    field_0x24 = e;
    field_0x28 = g;
    field_0x10 = f;
    if (field_0x10 > 0) {
        field_0x2 = 1;
    } else {
        field_0x2 = 0;
    }
    if (field_0x24 == 2) {
        field_0x8 = fadeFrames(c);
    }
    switch (field_0x1c) {
    case 0:
        if (field_0x24 == 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EC, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3ED, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EE, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EF, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x14 = 255.0f;
        field_0x18 = 255.0f / (f32)(s32)(field_0x8 - 1);
        break;
    case 1:
        if (field_0x24 == 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F0, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F1, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F2, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F3, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x14 = 0.0f;
        field_0x18 = 255.0f / (f32)(s32)(field_0x8 - 1);
        break;
    case 2:
        if (field_0x24 == 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F0, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F1, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F2, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F3, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x14 = 0.0f;
        field_0x18 = 510.0f / (f32)(s32)(field_0x8 - 1);
        break;
    }
}

void cls_80119E5C::fn_80119958() {
    field_0x0 = 0;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    field_0x14 = 0.0f;
    field_0x18 = 0.0f;
    field_0x1c = 0;
    field_0x20 = 0;
    field_0x24 = 0;
    field_0x28 = 0;
    field_0x10 = 0;
}

// FIXME: 99.88. reloc-name only: the unit's .sdata2 pool still lacks 0.05f (fn_80118E20,
// unwritten) and has 1.0f after the double; the original has a 1.0f made before
// fn_801194C4's double (no function here explains it).
// The fade step: field_0x4 counts the frames up to field_0x8 while field_0x14 moves
// by field_0x18 (down for mode 0, up for 1, up then down for 2) within 0..255; at
// the end the fade holds while field_0x10 counts down (when field_0x2 is set), then
// resets. Not during game modes 9 and 0x16.
void cls_80119E5C::fn_80119628() {
    switch (gGameRoot_8053AAF8->field_0x4c) {
    case 9:
    case 0x16:
        return;
    }
    if (field_0x0 == 0) {
        return;
    }
    if (field_0x1 != 0) {
        if (field_0x2 != 0) {
            field_0x10--;
            if (field_0x10 <= 0) {
                field_0x1 = 0;
            }
        }
        return;
    }
    field_0x4++;
    switch (field_0x1c) {
    case 0:
        if (field_0x2 != 0 && field_0x10 > 0) {
            field_0x1 = 1;
        }
        field_0x14 -= field_0x18;
        break;
    case 1:
        field_0x14 += field_0x18;
        break;
    case 2:
        if (field_0x24 == 2 && field_0x4 == field_0x8 / 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EC, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3ED, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EE, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EF, 0, 0, NULL, NULL, true, 1.0f);
        }
        if (field_0x4 >= field_0x8 / 2 + 1) {
            if (field_0x2 != 0 && field_0x10 > 0) {
                field_0x1 = 1;
            } else {
                field_0x14 -= field_0x18;
            }
        } else {
            field_0x14 += field_0x18;
        }
        break;
    }
    if (field_0x14 >= 255.0f) {
        field_0x14 = 255.0f;
    } else if (field_0x14 <= 0.0f) {
        field_0x14 = 0.0f;
    }
    if (field_0x4 > field_0x8) {
        if (field_0x2 != 0) {
            if (field_0x10 <= 0) {
                fn_80119958();
            } else {
                field_0x1 = 1;
            }
        } else {
            fn_80119958();
        }
    }
}

// FIXME: 99.94. reloc-name only (the double of the u16 to float conversions; see
// fn_80119628).
// Draws the fade picture over the screen with alpha field_0x14: black, or white
// for field_0x24 == 1 (not for field_0x24 == 2).
void cls_80119E5C::fn_801194C4() {
    if (field_0x0 != 0 && field_0x24 != 2 && field_0x14 != 0.0f) {
        gGameRoot2_8053AB10->fn_8013B490();
        switch (field_0x24) {
        case 1:
            field_0x2c->setBlackWhite(JUtility::TColor(0xFFFFFFFF), JUtility::TColor(0));
            break;
        case 0: // the original's switch also has case 0 (bge / b both to the black one)
        default:
            field_0x2c->setBlackWhite(JUtility::TColor(0xFF), JUtility::TColor(0));
            break;
        }
        field_0x2c->setAlpha(field_0x14);
        field_0x2c->draw(0.0f, 0.0f, (s32)JUTVideo::sManager->getFbWidth(),
                         (s32)JUTVideo::sManager->getEfbHeight(), false, false, false);
    }
}

void cls_80119E5C::fn_80119468(s32 g, s32 mode) {
    fn_80119998(fadeFrames(mode), 0, mode, 0, 2, 0, g);
}

// FIXME(shotgun): not written: fn_80118E20 (0x2C4: the per-frame pass over the actor list
// with the lbl_8053AED0 flag bits and the inlined fn_801184F0 reset), fn_801190E4 (the vector
// reserve), the vector internals fn_801188B8 / fn_801188E0 / fn_80118938 / fn_801189B0
// (JGadget TVector insert machinery at this+0xC, needs a typed vector member), fn_8011888C
// (calls them), fn_8011921C (the static getter) and the destructor fn_801192AC.
// FIXME(shotgun): 79 %, missing/extra (erase is a plain copy loop; the original is a vector erase)
void cls_801192AC::fn_80118798(ActorBase_804A5770* actor) {
    ActorBase_804A5770** it = mBegin;
    ActorBase_804A5770** end = mEnd;
    while (it != end && (*it)->mSerial != actor->mSerial) {
        it++;
    }
    if (it != end) {
        ActorBase_804A5770** next = it + 1;
        while (next < end) {
            *it++ = *next++;
        }
        mEnd = it;
    }
}

// FIXME(shotgun): 37 %, missing/extra
ActorBase_804A5770** cls_801192AC::fn_8011871C(ActorBase_804A5770** from, u32 actorId) {
    if (mBegin == NULL || mEnd == mBegin) {
        return mEnd;
    }
    while (from != mEnd) {
        if ((*from)->mActorId == actorId) {
            return from;
        }
        from++;
    }
    return mEnd;
}

bool cls_801192AC::fn_801186B0(u32 value) {
    if (getActorManager_801F856C()->fn_801F68E0()) {
        field_0x0 = 1;
        field_0x1 = 0;
        field_0x2 = 0;
        field_0x4 = value;
        field_0x8 = 0;
        return true;
    }
    return false;
}

// FIXME(shotgun): 57 %, missing/extra (the vector clear is a plain assignment)
bool cls_801192AC::fn_801184F0(bool flag) {
    getActorManager_801F856C()->fn_801F6884();
    field_0x0 = 0;
    if (flag) {
        fadeObject()->fn_80119958();
        mEnd = mBegin;
        field_0x0 = 0;
        field_0x1 = 0;
        field_0x2 = 0;
        field_0x4 = 0x13;
        field_0x8 = 0;
    }
    return true;
}

void cls_801192AC::fn_80118454() {
    field_0x1 = 1;
    fadeObject()->field_0x1 = 1;
}

void cls_801192AC::fn_801183BC() {
    field_0x1 = 0;
    fadeObject()->field_0x1 = 0;
}

void cls_801192AC::fn_80118384() {
    getActorManager_801F856C()->fn_801F6884();
    field_0x0 = 0;
}
