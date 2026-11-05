#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_heart_container.h"

HeartContainer_804A7E58::~HeartContainer_804A7E58() {}

void HeartContainer_804A7E58::draw() {
    drawSprite_801F06F0("enemy_both2_sob.bin", 0x31, NULL, 1, 1.0f, 1.0f);
    fn_803075AC(mSerial, field_0x4, &mPos, 0x31, false, 2, -1, 0, 0, 0);
}

void HeartContainer_804A7E58::update() {
    JGeometry::TVec2<f32> p(mPos.x, mPos.y);
    JGeometry::TVec3f pos;
    switch (field_0x230) {
    case 0:
        if (mPos.z != 0.0f || field_0x3c.z != 0.0f) {
            mPos += field_0x3c;
            field_0x3c.z -= 0.3f;
            if (mPos.z <= 0.0f) {
                mPos.z = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
            }
        }
        {
            JGeometry::TBox2f box(mBox);
            box.addPos(p);
            field_0x238 = fn_8023AF14(&box, field_0x4, true, 0.0f);
        }
        if (field_0x238 >= 0 && !fn_8023ED8C(field_0x238) && !fn_8023EBB8(field_0x238) &&
            mPos.z <= 0.0f) {
            pos = *getPlayerPos_8023E724(field_0x238);
            mPos.x = pos.x + 6.0f;
            mPos.y = pos.y - 24.0f;
            if (fn_802308E0(field_0x238) == true) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_IMUP, field_0x4, &mPos,
                                           (field_0x238 << 24) + 0x800000 + (field_0x238 << 26) + 0x14, -1, -1);
                mAlive = false;
            }
        }
        break;
    case 1:
        if (field_0x234 == 0 && fn_80388EEC(field_0x238)) {
            fn_80230224(field_0x238, false);
            fn_80230224(field_0x90 & 0xF, false);
            mAlive = false;
        } else if (field_0x234 != 0) {
            field_0x234--;
        }
        break;
    }
    if (field_0x4 < 8) {
        JGeometry::TVec3f camPos(mPos.x, mPos.y - mPos.z, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&camPos);
    }
}

void HeartContainer_804A7E58::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x3c.z = 0.0f;
    field_0x230 = 0;
}
