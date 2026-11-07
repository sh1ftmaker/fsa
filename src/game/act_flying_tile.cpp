#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/GameGlobals.h"
#include "game/act/act_flying_tile.h"

// FIXME(shotgun): 99 %, scheduling + reloc-name
void FlyingTile_804AC100::init() {
    field_0x244 = field_0x90 & 0x7FF;
    field_0x250 = (field_0x90 >> 11) & 0x1F;
    field_0x24c = (field_0x90 >> 16) & 0xF;
    field_0x254 = (field_0x90 >> 20) & 0xF;
    field_0x264 = ((field_0x90 >> 24) & 7) * 5 + 15;
    mLife = 0;
    field_0x27c = 0.0f;
    field_0x280 = 0.0f;
    field_0x284 = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x234 = 0;
    field_0x260 = 0;
    field_0x288 = NULL;
    field_0x238 = 0;
    field_0x23c = 5;
    field_0x25a = 0;
    field_0x240 = 0x8000;
    field_0x258 = 0;
    field_0x114 = 1;
    field_0x268 = 1;
    field_0x270 = field_0x244 * 60 + 60;
    field_0x248 = field_0x244;
    field_0x278 = 0;
    field_0x26c = 0xFFF;
    field_0x274 = 0;
    if (field_0x250 == 0) {
        field_0x230 = 0;
    } else {
        field_0x230 = 4;
    }
    switch (field_0x24c) {
    case 0:
    case 5:
    case 8:
        mPos += JGeometry::TVec3f(8.0f, 8.0f, 0.0f);
        field_0x80.set(-3.5f, -3.5f, 3.5f, 3.5f);
        mBox.set(-3.5f, -3.5f, 3.5f, 3.5f);
        field_0x25c = 0x1000;
        field_0x114 = 2;
        break;
    case 1:
        field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
        field_0x25c = 0x1000;
        field_0x114 = 2;
        break;
    case 2:
        mPos += JGeometry::TVec3f(0.0f, 16.0f, 0.0f);
        field_0x80.set(-30.0f, -30.0f, 30.0f, 30.0f);
        mBox.set(-30.0f, -30.0f, 30.0f, 30.0f);
        field_0x25c = 0x800;
        field_0x288 = getGameView_802D6A9C()->fn_802D9C4C();
        field_0x114 = 2;
        break;
    case 3:
        field_0x80.set(-40.0f, -40.0f, 40.0f, 40.0f);
        mBox.set(-40.0f, -40.0f, 40.0f, 40.0f);
        field_0x25c = 0x400;
        field_0x288 = getGameView_802D6A9C()->fn_802D9C4C();
        field_0x114 = 4;
        field_0x268 = 2;
        break;
    case 4:
        field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
        field_0x25c = 0x800;
        field_0x114 = 4;
        break;
    case 6:
    case 9:
        field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
        mBox.set(-14.0f, -14.0f, 14.0f, 14.0f);
        field_0x25c = 0x800;
        field_0x114 = 4;
        break;
    case 7:
        mPos += JGeometry::TVec3f(8.0f, 8.0f, 0.0f);
        field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
        field_0x25c = 0x800;
        field_0x114 = 2;
        break;
    }
}

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

// FIXME(shotgun): 86 %, missing/extra
void FlyingTile_804AC100::fn_80339820() {
    JGeometry::TVec3f pos = mPos;
    pos.y -= pos.z;
    pos.z = 0.0f;
    if (field_0x24c == 5) {
        if (field_0x260 >= 0x45) {
            field_0x260 -= 0x45;
        } else {
            field_0x260 -= 0x40;
        }
    } else if (field_0x24c == 1) {
        switch (field_0x260) {
        case 0x75:
            field_0x260 = 2;
            break;
        case 0x6B:
            field_0x260 = 4;
            break;
        default:
            field_0x260 = 1;
            break;
        }
    }
    if (field_0x260 <= 1) {
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.2f) {
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.8f) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &pos, 0x261400, -1, -1);
            } else {
                bool skip = false;
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                    skip = true;
                }
                if (!skip) {
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.2f) {
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FARY, field_0x4, &pos, 0, -1, -1);
                } else {
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &pos, 0, -1, -1);
                }
                }
            }
        }
    } else if (field_0x260 == 2) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &pos, 0, -1, -1);
    } else if (field_0x260 != 3 && field_0x260 == 4) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FARY, field_0x4, &pos, 0, -1, -1);
    }
}

FlyingTile_804AC100::~FlyingTile_804AC100() {}
