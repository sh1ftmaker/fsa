#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_stone_disguised_npc.h"

static inline void normalize(JGeometry::TVec2f& v) {
    f32 sq = v.x * v.x + v.y * v.y;
    if (sq <= 32.0f * FLT_EPSILON) {
        return;
    }
    f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
    v.x *= norm;
    v.y *= norm;
}

NPCDisguisedAsAStone_804A8750::~NPCDisguisedAsAStone_804A8750() {}

void NPCDisguisedAsAStone_804A8750::vfunc_0x24() {
    if (field_0x4 >= 8) {
        JGeometry::TVec3f pos(mPos);
        pos.z = 0.0f;
        fn_801F856C()->fn_801F9484(ACTOR_KMRI, field_0x4, &pos, 0, -1, -1);
    } else if (mPos.z < 0.0f && fn_8022461C(&mPos, field_0x4) == 0xD) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 1.0f);
    } else {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x8C, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x8D, 1, 0, NULL, NULL, true, 1.0f);
    }
    startSound_801F0E34(0x30003, 0);
    mAlive = false;
}

void NPCDisguisedAsAStone_804A8750::draw() {
    drawSprite_801F06F0("enemy_both2_sob.bin", 0x29, NULL, 0, 1.0f, 1.0f);
}

void NPCDisguisedAsAStone_804A8750::update() {
    switch (mState) {
    case 0: {
        JGeometry::TBox2f box = mBox;
        box.addPos(mPos.x, mPos.y);
        field_0x24c = fn_8023AF14(&box, field_0x4, false, 0.0f);
        if (field_0x24c >= 0) {
            field_0x234 = field_0x238 = field_0x23c = 0.0f;
            JGeometry::TVec3f playerPos = *fn_8023E724(field_0x24c);
            f32 dx = playerPos.x - mPos.x;
            f32 dy = playerPos.y - mPos.y;
            if (__fabsf(dx) > __fabsf(dy)) {
                if (playerPos.x < mPos.x) {
                    field_0x234 = 2.0f;
                } else {
                    field_0x234 = -2.0f;
                }
            } else {
                if (playerPos.y < mPos.y) {
                    field_0x238 = 2.0f;
                } else {
                    field_0x238 = -2.0f;
                }
            }
            field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
            field_0x3c.z = 4.0f;
            mState = 1;
        }
        break;
    }
    case 1:
        addCalc2_801F71A4(&field_0x3c.x, field_0x234, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x238, 0.3f, 1.0f);
        mPos.add(field_0x3c);
        if (mPos.z > 0.0f) {
            field_0x3c.z -= 0.3f;
        } else if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x248 = 20;
            mState = 2;
        }
        break;
    case 2:
        field_0x248--;
        if (field_0x248 == 0) {
            mState = 3;
            field_0x244 = 4.0f;
            field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
            field_0x3c.z = 2.0f;
            mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        }
        break;
    case 3: {
        JGeometry::TVec3f* target = fn_8023E724(field_0x24c);
        // FIXME: 97.7%. The original also copies target's x and y (y first)
        // to the 8-byte slot at sp+8 before subtracting mPos (two stfs, then
        // stfs of the differences): four dead stores that MWCC drops here.
        JGeometry::TVec2f dir(target->x - mPos.x, target->y - mPos.y);
        normalize(dir);
        field_0x234 = dir.x * field_0x244;
        field_0x238 = dir.y * field_0x244;
        addCalc2_801F71A4(&field_0x3c.x, field_0x234, 0.3f, field_0x240);
        addCalc2_801F71A4(&field_0x3c.y, field_0x238, 0.3f, field_0x240);
        addCalc2_801F71A4(&field_0x240, 1.0f, 0.1f, 0.1f);
        field_0x3c.z -= 0.1f;
        mPos.add(field_0x3c);
        JGeometry::TVec3f savedPos(mPos);
        if (fn_801F3D94(true) || fn_801F305C()) {
            vfunc_0x24();
        } else if (fn_801F2618(NULL, false)) {
            mPos = savedPos;
            vfunc_0x24();
        } else if (mPos.z < 0.0f) {
            vfunc_0x24();
        } else if (fn_801F2B7C(field_0x114, -1, 4) || fn_801F2B20(0)) {
            vfunc_0x24();
        }
        break;
    }
    }
}

void NPCDisguisedAsAStone_804A8750::init() {
    mBox.set(-40.0f, -40.0f, 40.0f, 40.0f);
    field_0x80.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x234 = field_0x238 = field_0x23c = 0.0f;
    field_0x240 = 0.75f;
    mState = 0;
}
