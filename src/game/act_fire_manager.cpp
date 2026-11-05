#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/act/act_fire_manager.h"

FireManager_804AB2B0::~FireManager_804AB2B0() {}

void FireManager_804AB2B0::update() {
    JGeometry::TVec3f pos = mBasePos;
    if (!vfunc_0x34()) {
        if (field_0x240 == 0) {
            switch (field_0x248) {
            case 0:
                pos.x += 24.0f;
                break;
            case 1:
                pos.x -= 24.0f;
                break;
            case 2:
                pos.y += 16.0f;
                break;
            case 3:
                pos.y -= 16.0f;
                break;
            }
            if (field_0x248 >= 2) {
                if (field_0x254 == 0) {
                    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() <= field_0x23c) {
                        pos.x += 8.0;
                        field_0x23c = 0.2f;
                    } else {
                        pos.x -= 8.0;
                        field_0x23c = 0.8f;
                    }
                } else if (field_0x248 == 2) {
                    pos.y -= 8.0f;
                } else {
                    pos.y += 8.0f;
                }
            }
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FIRE, field_0x4, &pos, field_0x24c & 0xF80101FF, -1,
                                       -1);
        }
        if (field_0x240 <= 0) {
            field_0x240 = field_0x244 + (s32)((s32)field_0x245 * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        } else {
            field_0x240--;
        }
    }
}

// FIXME: 99.8%. The original emits `lis r0, 0x4330` (the unsigned int-to-float
// exponent) before `lis r4, 0x19` (the RNG multiplier); here they are swapped.
void FireManager_804AB2B0::init() {
    field_0x24c = 0;
    field_0x250 = field_0x90 & 0xF;
    if (field_0x250 < 3) {
        field_0x250 = 3;
    }
    mBasePos = mPos;
    field_0x23c = 0.5f;
    field_0x244 = (field_0x90 >> 8) & 0xFF;
    field_0x245 = (field_0x90 >> 16) & 0xFF;
    field_0x240 = field_0x245 * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
    field_0x254 = (field_0x90 >> 24) & 1;
    field_0x255 = (field_0x90 >> 25) & 1;
    field_0x24c = ((field_0x90 & 0x30) ^ 0x20) | 0xF | (field_0x250 << 6) |
                  (field_0x90 & 0xF8000000) | (field_0x255 << 16);
    field_0x248 = ((field_0x90 >> 4) & 3) ^ 2;
}
