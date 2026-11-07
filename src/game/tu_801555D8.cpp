#include "game/GameAudio.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JMath/JMath.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "game/act/tu_801F9484.h"

extern const s32 lbl_8049FDC8[][2]; // life by kind (.data of this unit)
extern const s32 lbl_8049FFD8[][2]; // tile positions of the kind-2 variant
extern s32 lbl_8053AB50;            // running count of the soldiers spawned

// FIXME(shotgun): 100 %, matches
void fn_801583E4(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(src->x, src->y, src->z);
}

// FIXME(shotgun): 100 %, matches
void fn_801583D4(JGeometry::TVec3f* dst, f32 x, f32 y, f32 z) {
    dst->set(x, y, z);
}

// FIXME(shotgun): 93.60 %, missing/extra + reloc-name
void cls_804A00C8::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -12.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 12.0f;
    field_0x80.i.x = -6.0f;
    field_0x80.i.y = -6.0f;
    field_0x80.f.x = 6.0f;
    field_0x80.f.y = 6.0f;
    mBox2.i.x = -10.0f;
    mBox2.i.y = -10.0f;
    mBox2.f.x = 10.0f;
    mBox2.f.y = 10.0f;
    field_0x94 = (field_0x90 >> 16) & 0xFF;
    field_0x280 = field_0x90 >> 24;
    field_0x28c = 0;
    if (field_0x280 == 0) {
        field_0x280 = -1;
    }
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x90 &= 0xFF;
    mLife = lbl_8049FDC8[field_0x90][0];
    field_0x274 = 0;
    field_0x278 = 0;
    field_0x298 = 1.0f;
    field_0x2b4 = 0.0f;
    field_0x2b0 = 0.0f;
    field_0x2ac = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x98 = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 3;
    field_0x2a8 = 0;
    field_0x238 = 8;
    field_0x284 = -1;
    field_0x27c = lbl_8053AB50;
    field_0x2aa = -1;
    lbl_8053AB50++;
    field_0xb0 = 0x12205;
    if (field_0x94 == 2 || field_0x90 == 3) {
        field_0xb0 = 0x4000000;
    }
    field_0x290 = 0;
    if (field_0x90 == 2 && lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 3) {
        mLife = 1;
        for (s32 i = 0; i < 6; i++) {
            if ((s32)(0.1f * mPos.x) == lbl_8049FFD8[i][0] && (s32)(0.1f * mPos.y) == lbl_8049FFD8[i][1]) {
                field_0x290 = i;
                break;
            }
        }
    }
    field_0x2a4 = 0.7f;
    if (field_0x94 == 1) {
        field_0x2a4 = 1.4f;
    }
    field_0x294 = 1.5f * field_0x2a4 + 0.2f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
    field_0x234 = 0;
    switch (field_0x90) {
    case 2:
        field_0xbc = 4;
        field_0xb8 = 1;
        mState = 0xF;
        break;
    case 3:
        field_0x298 = 0.0f;
        field_0x278 = 1;
        mState = 8;
        break;
    default:
        field_0xbc = 2;
        field_0xb8 = 1;
        mState = 0;
        break;
    }
    field_0x29c = 0.0f;
    field_0x2a0 = 1.0f;
    field_0x288 = 0;
    if (field_0x280 >= 0) {
        mState = 0xE;
    }
}

// FIXME(shotgun): 87.05 %, missing/extra + reloc-name
cls_804A00C8::~cls_804A00C8() {
    if (field_0x280 >= 0) {
        cls_804A00C8* partner = static_cast<cls_804A00C8*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x280));
        if (partner != NULL) {
            partner->field_0x290--;
        }
    }
    if (field_0x284 >= 0) {
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x284);
        if (other != NULL) {
            other->field_0x3c.set(0.0f, 0.0f, 5.0f);
        }
        field_0x284 = -1;
    }
}

// FIXME(shotgun): 84.00 %, missing/extra + reloc-name
void cls_804A00C8::vfunc_0x44() {
    if (field_0x284 >= 0) {
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x284);
        if (other != NULL) {
            other->field_0x3c.set(0.0f, 0.0f, 5.0f);
        }
        field_0x284 = -1;
    }
    if (mLife <= 0) {
        if (field_0x280 >= 0) {
            cls_804A00C8* partner = static_cast<cls_804A00C8*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x280));
            if (partner != NULL) {
                partner->field_0x290--;
            }
        }
    }
}

// FIXME(shotgun): 89.71 %, missing/extra + reloc-name
bool cls_804A00C8::vfunc_0x54() {
    if (mState == 0xD) {
        return true;
    }
    if (mPos.z > 2.0f) {
        return false;
    }
    field_0x2b8 = mPos;
    field_0x2b8.x = (f32)((s32)field_0x2b8.x & 0xFFF8);
    field_0x2b8.y = (f32)((s32)field_0x2b8.y & 0xFFF8);
    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    field_0x2b8.x += -12.0f * sincos.sinShort(fn_801F6874(field_0x98));
    field_0x2b8.y += -12.0f * sincos.cosShort(fn_801F6874(field_0x98));
    u16 kind = fn_8022461C(&field_0x2b8, field_0x4);
    if (kind == 2 || kind == 3 || kind == 0xE) {
        if (kind == 0xE && fn_802DE9A0() == true) {
            field_0x2a8 = 1;
        }
        field_0xd8 = 0;
        field_0xdc = 0;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x2ac = 0.0f;
        field_0x2b0 = 0.0f;
        field_0x2b4 = 0.0f;
        field_0x254 = 10000;
        mState = 0xC;
        mLife = 200;
        return true;
    }
    return false;
}
