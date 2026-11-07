#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/act/tu_800F46C0.h"

Boulder_8049C4C0::~Boulder_8049C4C0() {}

// FIXME(shotgun): 97.48 %, missing/extra + reloc-name
void Boulder_8049C4C0::init() {
    field_0x23c = 0;
    field_0x231 = 0;
    field_0x238 = 0;
    field_0x240 = 0;
    field_0x232 = -1;
    field_0x254.set(0.0f, 0.0f, 0.0f);
    field_0x230 = field_0x90;
    field_0x250 = 0;
    field_0x24c = 0;
    field_0x248 = 0;
    field_0x244 = 0;
    switch (field_0x230) {
    case 0:
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x80.i.x = -16.0f;
        field_0x80.i.y = -16.0f;
        field_0x80.f.x = 16.0f;
        field_0x80.f.y = 16.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.5f + gGameRoot_8053AAF8->mRandom.get_ufloat_1();
        field_0x3c.z = 1.0f;
        break;
    case 1:
        mBox.i.x = -10.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 10.0f;
        field_0x80.i.x = -10.0f;
        field_0x80.i.y = -10.0f;
        field_0x80.f.x = 10.0f;
        field_0x80.f.y = 10.0f;
        break;
    case 2:
        mBox.i.x = -6.0f;
        mBox.i.y = -6.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 6.0f;
        field_0x80.i.x = -6.0f;
        field_0x80.i.y = -6.0f;
        field_0x80.f.x = 6.0f;
        field_0x80.f.y = 6.0f;
        mPos.z = 1.0f;
        break;
    case 3:
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x80.i.x = -16.0f;
        field_0x80.i.y = -16.0f;
        field_0x80.f.x = 16.0f;
        field_0x80.f.y = 16.0f;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        break;
    case 5:
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x80.i.x = -16.0f;
        field_0x80.i.y = -16.0f;
        field_0x80.f.x = 16.0f;
        field_0x80.f.y = 16.0f;
        field_0x240 = (s32)(5.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        field_0x244 = 0;
        field_0x248 = 0x300 - (s32)(1536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        break;
    }
}

// FIXME(shotgun): 97 %, missing/extra + reloc-name
void Boulder_8049C4C0::update() {
    switch (field_0x230) {
    case 0:
        fn_800F5E08();
        break;
    case 1:
        if (field_0x3c.x != 0.0f || field_0x3c.y != 0.0f || field_0x3c.z != 0.0f) {
            fn_801F2B7C(2, -1, 6);
            field_0x3c.z -= 0.2f;
        } else {
            field_0x23c = 0;
        }
        mPos += field_0x3c;
        if (mPos.z <= 0.0f) {
            startSound_801F0E34(0x30075, 0);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x76, 0, 0, NULL, NULL, true, 0.8f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x77, 0, 0, NULL, NULL, true, 0.8f);
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
            mAlive = false;
        }
        break;
    case 2:
        fn_801F2B7C(1, -1, 1);
        field_0x3c.z -= 0.2f;
        mPos += field_0x3c;
        if (mPos.z <= 0.0f) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x76, 0, 0, NULL, NULL, true, 0.2f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x77, 0, 0, NULL, NULL, true, 0.2f);
            mAlive = false;
        }
        break;
    case 3:
        fn_800F555C();
        break;
    case 5:
        field_0x3c.z -= 0.03f;
        field_0x244 += field_0x248;
        mPos += field_0x3c;
        if (mPos.z <= 0.0f) {
            f32 scale = 1.0f;
            if (field_0x240 > 1) {
                scale = 0.5f;
            }
            startSound_801F0E34(0x3005A, 0);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x497, 0, 0, NULL, NULL, true, scale);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x498, 0, 0, NULL, NULL, true, scale);
            mAlive = false;
        }
        break;
    }
}

// FIXME(shotgun): 92 %, scheduling+regalloc
bool Boulder_8049C4C0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 code = fn_8022461C(pos, layer);
    if (code == 3 || code == 0xE || code == 2) {
        return false;
    }
    return fn_802265DC(pos, layer, false);
}

// FIXME(shotgun): 99 %, reloc-name
void BoulderSpawner_8049C46C::init() {
    field_0x230 = field_0x90;
    if (field_0x230 == 2) {
        field_0x234 = 0;
    } else if (field_0x230 == 4) {
        field_0x234 = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    } else {
        field_0x234 = (s32)(30.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    }
}

// FIXME(shotgun): 97.47 %, missing/extra
void BoulderSpawner_8049C46C::update() {
    if (field_0x234 == 0) {
        switch (field_0x230) {
        case 0:
            if ((field_0x90 & 0xF8000000) != 0 && !vfunc_0x34()) {
                return;
            }
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &mPos, 0, -1, -1);
            field_0x234 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x96;
            break;
        case 1: {
            if ((field_0x90 & 0xF8000000) != 0 && !vfunc_0x34()) {
                return;
            }
            JGeometry::TVec3f pos = mPos;
            pos.z += 200.0f;
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &pos, 1, -1, -1);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            actor->field_0x3c.set(0.0f, -0.01f, 0.0f);
            field_0x234 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x96;
            break;
        }
        case 2:
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &mPos, 3, -1, -1);
            mAlive = false;
            return;
        case 3:
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &mPos, (field_0x90 & ~0xFF) + 4, -1, -1);
            field_0x234 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x96;
            break;
        case 4: {
            if ((field_0x90 & 0xF8000000) != 0 && !vfunc_0x34()) {
                return;
            }
            JGeometry::TVec3f pos;
            pos.x = mPos.x;
            pos.y = mPos.y;
            pos.z = mPos.z + 200.0f;
            pos.x += (f32)(40 - (s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
            pos.y += (f32)(20 - (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_OIWA, field_0x4, &pos, 5, -1, -1);
            field_0x234 = (s32)(150.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x50;
            break;
        }
        }
    }
    field_0x234--;
}

void BoulderSpawner_8049C46C::draw() {}

BoulderSpawner_8049C46C::~BoulderSpawner_8049C46C() {}
