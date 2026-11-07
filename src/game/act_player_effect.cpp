#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_player_effect.h"

// 0x8022A83C.
// FIXME(shotgun): 96.55 %, different + reloc-name
void PlayerEffect_804A6E20::init() {
    u32 kind = (field_0x90 >> 2) & 0x3F;
    if (kind != 5) {
        if (kind == 1) {
            if (fn_8022F514(field_0x90 & 3)) {
                mAlive = false;
                return;
            }
        } else if (kind != 6 && kind != 7 && kind != 8 && kind != 9 && field_0x4 < 8) {
            mAlive = false;
            return;
        }
    }
    kind = (field_0x90 >> 2) & 0x3F;
    if (kind == 3) {
        mPos.x += (f32)(s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 2.0f;
        mPos.y += (f32)(s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 5.0f;
    } else if (kind == 4) {
        mPos.z += 16.0f;
    } else if (kind == 0xE) {
        mPos.x -= 4.0f;
        mPos.z += 16.0f;
    }
    mLife = 0;
    field_0x230 = 0;
    field_0x234 = 0;
    kind = (field_0x90 >> 2) & 0x3F;
    switch (kind) {
    case 0:
        field_0x234 = 0x85;
        break;
    case 1:
        field_0x234 = 0x7D;
        break;
    case 2:
        field_0x234 = 0x2AB;
        break;
    case 3:
        field_0x234 = 0x1AA;
        break;
    case 4:
    case 14:
        field_0x234 = 0x1C3;
        break;
    case 6:
    case 8:
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
        mPos.x += 10.0f;
        mPos.z += 20.0f;
        field_0x234 = 0x2A3;
        break;
    case 7:
    case 9:
        mPos = *getPlayerPos_8023E724(field_0x90 & 3);
        mPos.x -= 10.0f;
        mPos.z += 20.0f;
        field_0x234 = 0x2A3;
        break;
    case 10:
        field_0x234 = 0x2F5;
        break;
    case 11:
        field_0x234 = 0x306;
        break;
    case 12:
        field_0x234 = 0x30C;
        break;
    case 13:
        field_0x234 = 0x300;
        break;
    }
    field_0x11f = 1;
}

// Destructor at 0x8022979C.
PlayerEffect_804A6E20::~PlayerEffect_804A6E20() {}
