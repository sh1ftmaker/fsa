#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_hinox.h"
#include "JSystem/JMath/JMath.h"

// FIXME(shotgun): 98.16 %, missing/extra + reloc-name.
void Hinox_8049F9B4::init() {
    mBox.i.x = -14.0f;
    mBox.i.y = -14.0f;
    mBox.f.x = 14.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x90 &= 1;
    field_0x264 = 0;
    field_0x268 = -1;
    if (field_0x90 != 1) {
        field_0xb0 = 0x9042012;
    }
    field_0xbc = 3;
    field_0xb8 = 1;
    field_0x98 = 2;
    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f < 0.5f) {
        field_0x98 = 3;
    }
    field_0x26c = 0;
    field_0x278 = 0.0f;
    field_0x274 = 0.0f;
    field_0x270 = 0.0f;
    field_0x284 = 0.0f;
    field_0x280 = 0.0f;
    field_0x27c = 0.0f;
    field_0x26e = 0;
    field_0x250 = -1;
    field_0x254 = -1;
    mState = 0;
}

// FIXME(shotgun): 90.05 %, different + reloc-name.
Hinox_8049F9B4::~Hinox_8049F9B4() {
    JGeometry::TVec3f dir(0.0f, 0.0f, 0.0f);
    if (field_0x98 == 2) {
        dir.set(0.0f, 15.0f, 0.0f);
    } else {
        dir.set(0.0f, -15.0f, 0.0f);
    }
    if (field_0x254 >= 0) {
        f32 x = mPos.x;
        f32 y = mPos.y;
        f32 z = mPos.z;
        mPos.x = getPlayerPos_8023E724(field_0x254)->x;
        mPos.y = getPlayerPos_8023E724(field_0x254)->y;
        mPos.z = getPlayerPos_8023E724(field_0x254)->z;
        if (fn_801F2618(NULL, false)) {
            f32 angle = JMath::atanTable_.atan2_(x - mPos.x, y - mPos.y) * 10430.378f;
            dir.x = 3.0f * JMath::sincosTable_.sinShort((s16)(s32)angle);
            dir.y = 3.0f * JMath::sincosTable_.cosShort((s16)(s32)angle);
        }
        mPos.x = x;
        mPos.y = y;
        mPos.z = z;
        fn_8022F784(field_0x254, mSerial, &dir);
    }
    s32 serial = field_0x250;
    if (serial >= 0) {
        ActorBase_804A5770* held = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (held != NULL) {
            held->field_0x3c.set(0.0f, 0.0f, 3.0f);
            field_0x250 = -1;
            startSound_801F0E34(0x40008, 0);
        }
    }
}

// FIXME(shotgun): 88.19 %, missing/extra + reloc-name.
void Hinox_8049F9B4::fn_8014F76C() {
    s32 serial = field_0x250;
    if (serial >= 0) {
        ActorBase_804A5770* held = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (held != NULL) {
            held->field_0x3c.set(0.0f, 0.0f, 3.0f);
            field_0x250 = -1;
            startSound_801F0E34(0x40008, 0);
        }
    }
}

void Hinox_8049F9B4::draw() {
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x264 + 0x2B, NULL, 7, 1.0f, 1.0f);
}

// FIXME(shotgun): update (fn_8014DD14, 0x1A58 bytes) is not written.
