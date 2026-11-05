#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_rock.h"

Rock_804A9480::~Rock_804A9480() {}

bool Rock_804A9480::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    return fn_801F132C(layer, pos, &field_0x234);
}

void Rock_804A9480::vfunc_0x24() {
    s32 tile = fn_8022461C(&mPos, field_0x4);
    if (tile == 0xD || tile == 0x3B || tile == 0x39) {
        mPos.z = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        startSound_801F0E34(0x30047, 0);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F3, -1, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F4, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F5, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F6, 0, 0, NULL, NULL, true, 1.0f);
        mAlive = false;
    } else {
        if (field_0x230 < 0x50) {
            if (field_0x4 >= 8) {
                JGeometry::TVec3f pos(mPos);
                pos.z = 0.0f;
                fn_801F856C()->fn_801F9484(ACTOR_KMRI, field_0x4, &pos, 0, -1, -1);
            } else {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x8A, 1, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x8B, 1, 0, NULL, NULL, true, 1.0f);
            }
        }
        startSound_801F0E34(0x30003, 0);
        mAlive = false;
    }
}

void Rock_804A9480::draw() {
    drawSprite_801F06F0("enemy_both2_sob.bin", 0x2B, NULL, 7, 1.0f, 1.0f);
    fn_803075AC(mSerial, field_0x4, &mPos, 0x2B, false, 2, -1, 1, 0, 0);
}

void Rock_804A9480::update() {
    if (field_0xc4) {
        mPos.set(*fn_8023E7B0(field_0xc8));
        field_0x4 = fn_8023DE58(field_0xc8);
        if (field_0x230 < 1000) {
            field_0x230++;
        }
    } else {
        field_0x3c.z += -0.15f;
        mPos += field_0x3c;
        if (mPos.z < 0.0f || fn_801F2618(NULL, false)) {
            if (mPos.z < 0.0f) {
                if (fn_80226850(&mPos, field_0x4) == 0x90) {
                    lbl_8053AC98->fn_802A381C(&mPos, field_0x4, 0x91);
                } else if (fn_80226850(&mPos, field_0x4) == 0x91) {
                    lbl_8053AC98->fn_802A381C(&mPos, field_0x4, 0x92);
                }
            }
            vfunc_0x24();
        } else if (fn_801F2B7C(0, field_0xc8, 5) || fn_801F2B20(0)) {
            vfunc_0x24();
        }
    }
}

void Rock_804A9480::init() {
    mBox.i.x = -16.0f;
    mBox.i.y = -16.0f;
    mBox.f.x = 16.0f;
    mBox.f.y = 16.0f;
    field_0x1a4 = 1;
    field_0x230 = 0;
    field_0x80.i.x = -12.0f;
    field_0x80.i.y = -12.0f;
    field_0x80.f.x = 12.0f;
    field_0x80.f.y = 12.0f;
    field_0x234 = 0;
}
