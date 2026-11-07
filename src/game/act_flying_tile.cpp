#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_flying_tile.h"

// FIXME(shotgun): 100 %, matches
void FlyingTile_804AC100::vfunc_0x10() {
    s32 serial = -1;
    s32 best = 0;
    for (;;) {
        serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_FLYO, serial);
        if (serial < 0) {
            break;
        }
        FlyingTile_804AC100* tile =
            static_cast<FlyingTile_804AC100*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (tile == NULL) {
            continue;
        }
        s32 value;
        if (tile->field_0x230 == 0 || tile->field_0x230 == 4) {
            value = tile->field_0x248;
        } else {
            value = 0xFFF;
        }
        if (value < field_0x26c) {
            field_0x26c = value;
        }
        if (best < value && value < field_0x244) {
            best = value;
        }
    }
    if (field_0x26c == field_0x244) {
        field_0x278 = 1;
    }
    if (best != 0) {
        field_0x274 = field_0x244 - best;
    }
}

// FIXME(shotgun): 95 %, missing/extra
bool FlyingTile_804AC100::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL) == true) {
        return true;
    }
    s32 code = fn_8022461C(pos, layer);
    if (code == 0 || code == 2 || code == 0x50) {
        return false;
    }
    if (code == 0x22 && getGameView_802D6A9C()->field_0x170 == 0) {
        return false;
    }
    if ((u16)code == 0x39) {
        return false;
    }
    if ((u16)code == 0x25 && getGameView_802D6A9C()->field_0x171 == 0) {
        return false;
    }
    if ((u16)code == 4) {
        if (((s32)pos->y & 0xF) >= 8) {
            return false;
        }
    } else if ((u16)code == 5) {
        if (((s32)pos->y & 0xF) < 8) {
            return false;
        }
    } else if ((u16)code == 6) {
        if (((s32)pos->x & 0xF) >= 8) {
            return false;
        }
    } else if ((u16)code == 7) {
        if (((s32)pos->x & 0xF) < 8) {
            return false;
        }
    } else if ((u16)code == 3 || (u16)code == 0xE || (u16)code == 0xD) {
        return false;
    }
    return true;
}

FlyingTile_804AC100::~FlyingTile_804AC100() {}
