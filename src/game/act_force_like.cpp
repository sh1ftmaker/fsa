#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"
#include "game/act/tu_801F9484.h"
#include "game/act/act_force_gem.h"

// Sprite frame bases per kind (data table at 0x8049C518), the player-drain amounts and the
// rupee values that fill the log (0x8049C548 / 0x8049C558); effect ids at 0x80539E70, the
// animation frame script at 0x80539E78.
static const s32 lbl_8049C518[12] = {0x11, 0x15, 0x19, 0x1D, 0x285, 0x293, 0x28C, 0x29A, 0x2A1, 0x2A9, 0x2A5, 0x2AD};
static const s32 lbl_8049C548[4] = {0xA, 0xF, 0x14, 0x1E};
static const s32 lbl_8049C558[8] = {1, 5, 20, 50, 100, 150, 200, 300};
static const u16 lbl_80539E70[4] = {0x3D9, 0x3D8, 0x3DB, 0x3DA};
static const u8 lbl_80539E78[8] = {0, 1, 2, 3, 1, 0xFF, 0, 0};

// The sprite frame stepper the update inlines in its three carrying states.
static inline void stepFrame(ForceLike_8049C578* self) {
    self->field_0x240++;
    if (self->field_0x230 == 2) {
        self->field_0x235 = lbl_80539E78[self->field_0x240 / 2];
        if (self->field_0x235 == 0xFF) {
            self->field_0x235 = 0;
            self->field_0x240 = 0;
        }
    } else if (self->field_0x230 == 3) {
        if (self->field_0x240 == 6) {
            self->field_0x235++;
            if (self->field_0x235 == 7) {
                self->field_0x235--;
            }
            self->field_0x240 = 0;
        }
    } else if (self->field_0x240 == 6) {
        self->field_0x235++;
        if (self->field_0x235 == 7) {
            self->field_0x235 = 0;
        }
        self->field_0x240 = 0;
    }
}

// FIXME(shotgun): 99 %, reloc-name
void ForceLike_8049C578::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x235 = 0;
    field_0x240 = 0;
    field_0x230 = 0;
    field_0x244 = 0;
    field_0x234 = field_0x90 & 0xF;
    field_0x23c = 1;
    field_0x238 = 0;
    field_0x264 = 0.0f;
    field_0x260 = 0.0f;
    if (field_0x90 & 0x100) {
        field_0x230 = 1;
    }
    if (vfunc_0x3C()) {
        field_0x230 = 4;
    }
    if (field_0x90 & 0x400) {
        field_0x23c = 0;
        mPos.z = 400.0f;
    }
    field_0x24c = 0;
}

// FIXME(shotgun): 100 %, matches
ForceLike_8049C578::~ForceLike_8049C578() {
    if (fn_8022DCD8(field_0x238)) {
        fn_8022DD74(field_0x238);
    }
}

// FIXME(shotgun): 97.59 %, missing/extra
void ForceLike_8049C578::update() {
    u16 angle = 0;
    s32 dir = 0;
    f32 dist = 0.0f;
    s32 player = -1;
    fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
    field_0x24c++;
    switch (field_0x230) {
    case 4:
        if (vfunc_0x34()) {
            field_0x230 = 0;
        }
        break;
    case 0: {
        bool landed = false;
        field_0x3c.z -= 0.3f;
        if (mPos.z < 0.0f) {
            if (field_0x23c == 0) {
                startSound_801F0E34(0x3000A, 0);
                mPos.z = 0.0f;
                field_0x3c.z = 2.0f + (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                field_0x3c.x = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                field_0x3c.y = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                field_0x23c = 1;
            } else {
                field_0x3c.zero();
                landed = true;
            }
        }
        mPos.add(field_0x3c);
        if (--field_0x240 <= 0) {
            field_0x235 = (field_0x235 + 1) & 3;
            if (field_0x235 == 0) {
                field_0x240 = (s32)(60.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 60;
            } else {
                field_0x240 = 2;
            }
        }
        if (player != -1 && dist < 40.0f && landed == true) {
            field_0x230 = 1;
            field_0x240 = 0;
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x3DC, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80539E70[field_0x234], 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x50023, 0);
        }
        break;
    }
    case 1: {
        field_0xb0 = 0x11101;
        if (fn_801F3D94(1) != 0) {
            break;
        }
        if (fn_801F305C()) {
            break;
        }
        stepFrame(this);
        if (player != -1 && dist < 80.0f && !fn_8022DCD8(player)) {
            field_0x23e = angle;
            f32 dx = std::fabsf(mPos.x - getPlayerPos_8023E724(player)->x);
            f32 dy = std::fabsf(mPos.y - getPlayerPos_8023E724(player)->y);
            if (dx < 8.0f && dy < 8.0f && fn_8022DE10(player, 1) == 1) {
                field_0x230 = 2;
                field_0x238 = player;
                field_0x250 = 0;
                field_0x248 = 0;
                field_0x240 = 0;
                field_0x235 = 0;
            }
        }
        f32 distSq;
        s32 serial = getActorManager_801F856C()->fn_801F778C('RUPY', &mPos, &distSq);
        if (serial != -1) {
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            f32 tx = target->mPos.x;
            f32 ty = target->mPos.y;
            field_0x23e = (s32)(JMath::atanTable_.atan2_(tx - mPos.x, ty - mPos.y) * 10430.378f);
            if (distSq < 25.0f) {
                target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                s32 idx = -1;
                if (target->field_0x94 == 0 || target->field_0x94 == 3) {
                    idx = target->field_0x90 & 0xFF;
                }
                if (idx != -1) {
                    field_0x244 += lbl_8049C558[idx];
                    target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    s32 st = static_cast<ForceGem_804AB6D8*>(target)->field_0x230;
                    if (st == 0 || (u32)(st - 0xF) <= 1 || (u32)(st - 1) <= 1 || st == 9 || st == 0xD) {
                        target->fn_801F3668();
                    }
                }
            }
        } else if ((field_0x24c & 0xF) == 0) {
            field_0x23e = (s32)(32768.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + field_0x23e - 0x4000;
        }
        mPos.x += 0.3f * JMath::sincosTable_.sinShort(field_0x23e);
        mPos.y += 0.3f * JMath::sincosTable_.cosShort(field_0x23e);
        break;
    }
    case 2: {
        s32 target = field_0x238;
        if (fn_8022F514(field_0x238)) {
            target = fn_8022F3DC(field_0x238);
        }
        s32 life = mLife;
        if (fn_801F305C()) {
            field_0x248 += life - mLife;
            mLife = life;
            if (fn_80236228(target) > 20) {
                fn_802362C4(target, -20);
                getActorManager_801F856C()->spawnActor_801F9484('GOLD', field_0x4, &mPos, 20, -1, -1);
            }
        }
        if (field_0x248 > 20) {
            fn_8022DD74(field_0x238);
        }
        fn_801F3D94(1);
        stepFrame(this);
        field_0x254.x = mPos.x - getPlayerPos_8023E724(field_0x238)->x;
        field_0x254.y = mPos.y - getPlayerPos_8023E724(field_0x238)->y;
        field_0x254.z = mPos.z - getPlayerPos_8023E724(field_0x238)->z;
        fn_8023DFF4(field_0x238, &field_0x254);
        s32 drain = lbl_8049C548[field_0x234];
        if ((field_0x24c & 0x1F) == 0 && fn_80236228(target) - drain > 0) {
            fn_802362C4(target, -drain);
            field_0x244 += drain;
        }
        if (!fn_8022DCD8(field_0x238)) {
            field_0x230 = 3;
            field_0x240 = 0;
            JGeometry::TVec2f p(mPos.x, mPos.y);
            fn_80238548(field_0x238, 0, &p, 1);
        }
        if (player != -1) {
            field_0x254.x = mPos.x - getPlayerPos_8023E724(player)->x;
            field_0x254.y = mPos.y - getPlayerPos_8023E724(player)->y;
            field_0x254.z = mPos.z - getPlayerPos_8023E724(player)->z;
            field_0x254.normalize();
        }
        mPos.x += 0.4f * field_0x254.x;
        mPos.y += 0.4f * field_0x254.y;
        field_0x3c.z -= 0.21f;
        mPos.z += field_0x3c.z;
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 2.0f;
            startSound_801F0E34(0x50024, 0);
            if (fn_80236228(target) > 5) {
                fn_802362C4(target, -5);
                getActorManager_801F856C()->spawnActor_801F9484('GOLD', field_0x4, &mPos, 5, -1, -1);
            }
        }
        field_0x260 = 0.0f;
        field_0x264 = 0.0f;
        if (fn_8022C5EC(field_0x238, 8)) {
            field_0x264 = -5.0f;
        } else if (fn_8022C5EC(field_0x238, 4)) {
            field_0x264 = 5.0f;
        } else if (fn_8022C5EC(field_0x238, 1)) {
            field_0x260 = -5.0f;
        } else if (fn_8022C5EC(field_0x238, 2)) {
            field_0x260 = 5.0f;
        }
        break;
    }
    case 3: {
        f32 z = field_0x3c.z;
        if (fn_801F3D94(1) != 0 || fn_801F305C()) {
            field_0x3c.z = z;
        } else {
            fn_801F2B7C(-100, -1, 1);
            stepFrame(this);
            if (field_0x235 != 0) {
                if (mPos.z == 0.0f && field_0x235 != 0) {
                    field_0x3c.z = 2.78f;
                    startSound_801F0E34(0x50025, 0);
                    if ((s32)(3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) < 1) {
                        field_0x254.x = 2.0f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                        field_0x254.y = 2.0f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                    } else {
                        field_0x254.x = mPos.x - getPlayerPos_8023E724(field_0x238)->x;
                        field_0x254.y = mPos.y - getPlayerPos_8023E724(field_0x238)->y;
                    }
                    field_0x254.z = 0.01f;
                    field_0x254.normalize();
                }
                mPos.x += 0.8f * field_0x254.x;
                mPos.y += 0.8f * field_0x254.y;
            }
        }
        field_0x3c.z -= 0.21f;
        mPos.z += field_0x3c.z;
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x235 = 0;
        }
        break;
    }
    }
    fn_801F2618(NULL, false);
}

// FIXME(shotgun): 90.25 %, missing/extra
void ForceLike_8049C578::draw() {
    SpriteParams_801F5AE0 params;
    switch (field_0x230) {
    case 0:
        fn_801F02BC("enemy_both2_sob.bin", lbl_8049C518[field_0x234] + field_0x235, &params, 0, 0, false, 1.0f, 1.0f);
        break;
    case 2:
        mPos.x += field_0x260;
        mPos.y += field_0x264;
        fn_801F02BC("enemy_gc_sob.bin", lbl_8049C518[field_0x234] + field_0x235, &params, 7, 0, false, 1.0f, 1.0f);
        mPos.x -= field_0x260;
        mPos.y -= field_0x264;
        break;
    case 4:
        break;
    default: {
        f32 scale = 1.0f + 0.3f * (field_0x244 / 600.0f);
        if (scale > 1.3f) {
            scale = 1.3f;
        }
        Mtx mtx;
        mtx[0][0] = scale;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[0][3] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = scale;
        mtx[2][3] = 0.0f;
        params.mMtx = mtx;
        fn_801F02BC("enemy_gc_sob.bin", lbl_8049C518[field_0x234] + field_0x235, &params, 7, 0, false, 1.0f, 1.0f);
        break;
    }
    }
}

// FIXME(shotgun): 97.94 %, different
void ForceLike_8049C578::vfunc_0x24() {
    s32 layer = field_0x4;
    if (fn_8022DCD8(field_0x238)) {
        fn_8022DD74(field_0x238);
    }
    JGeometry::TVec3f pos;
    bool flag;
    if (field_0x90 & 0x1000) {
        flag = true;
        fn_801CD150(&mPos, &pos);
        layer = 8;
    } else {
        flag = false;
        pos.x = mPos.x;
        pos.y = mPos.y;
        pos.z = mPos.z;
    }
    while (field_0x244 > 0) {
        s32 pick;
        if (flag == false) {
            if (field_0x244 >= 200) {
                pick = 3;
                field_0x244 -= 50;
            } else if (field_0x244 >= 80) {
                pick = 2;
                field_0x244 -= 20;
            } else if (field_0x244 >= 20) {
                pick = 1;
                field_0x244 -= 5;
            } else {
                pick = 0;
                field_0x244 -= 1;
            }
        } else if (field_0x244 >= 200) {
            pick = 4;
            field_0x244 -= 100;
        } else if (field_0x244 >= 50) {
            pick = 3;
            field_0x244 -= 50;
        } else {
            pick = 2;
            field_0x244 -= 20;
        }
        s32 serial = getActorManager_801F856C()->fn_801F8544('RUPY', layer, &pos, pick);
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                f32 vz = 3.0f + 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                f32 vy = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                f32 vx = 0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                actor->field_0x3c = JGeometry::TVec3f(vx, vy, vz);
            }
        }
    }
    spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80539E70[field_0x234], 0, 0, NULL, NULL, true, 1.0f);
    field_0x18c = 0;
    ActorBase_804A5770::vfunc_0x24();
}
