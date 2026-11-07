#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/act_life_meter.h"
#include "game/act/tu_801F9484.h"

// The 'ZLD4' actor, cls_804A3698.

// FIXME(shotgun): table not written out (gate.py literal tripwire); extern-declared, size
// from symbols.txt (0x38). update indexes it.
extern s32 lbl_804A3660[14];

// 0x801B3CD0.
// FIXME(shotgun): 0 %, new
cls_804A3698::~cls_804A3698() {
    lbl_8053ABE8 = 0;
    lbl_8053ABF0 = 0;
}

// 0x801B3B7C.
// FIXME(shotgun): 0 %, new
void cls_804A3698::init() {
    lbl_8053ABF0 = 0;
    field_0x80.set(-4.0f, -8.0f, 4.0f, 8.0f);
    field_0x30c = 0;
    field_0x310 = 0;
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x31c = 0;
    field_0x300 = 0;
    field_0x304 = 0;
    field_0x308 = 0;
    field_0x330 = field_0x90 & 7;
    field_0x328 = 5;
    field_0x324 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_LIMT, field_0x4, &mPos, 0, -1, -1);
    field_0x11e = 1;
    fn_8022ADE4();
    s32 serial = field_0x324;
    if (serial >= 0) {
        LifeMeter_804B174C* meter =
            static_cast<LifeMeter_804B174C*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (meter != NULL) {
            meter->field_0x250 = mSerial;
            meter->field_0x254 = -28.0f;
            meter->field_0x24c = field_0x328;
        }
    }
    lbl_8053AED0->fn_8043A450(6, 3);
    field_0x320 = 1;
    field_0x334 = field_0x338 = field_0x33c = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x340 = mPos.z;
    lbl_8053ABEC = 0;
    field_0x2fc = 0;
    field_0x32d = 0;
    field_0x32c = 0;
    field_0x1a8 = false;
    lbl_8053ABE8 = 0;
    field_0x2f8 = 0;
}

// 0x801B2998.
// FIXME(shotgun): 0 %, new
void cls_804A3698::draw() {
    if (field_0x308 == 0 || (field_0x308 & 2)) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x320 + 0x9B, NULL, 7, 1.0f, 1.0f);
    }
}

// 0x801B27DC.
// FIXME(shotgun): 0 %, new
bool cls_804A3698::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    u16 code = fn_8022461C(pos, layer);
    u16 tile = fn_80226850(pos, layer);
    if (tile == 0x230 || tile == 0x231 || tile == 0x234) {
        return false;
    }
    if (code != 0 && code != 4 && code != 5 && code != 6 && code != 7 && code != 2 && code != 3 &&
        code != 0x50 && code != 0xE && code != 0xC && code != 0x27 && code != 0x28 && code != 0x29 &&
        code != 0x2A && code != 0x2B && code != 0x2C && code != 0x2D && code != 0x2E && code != 0x2F &&
        code != 0x20 && code != 0xD && code != 0x39 && code != 0x3A && code != 0x3B && code != 0x4C &&
        code != 0x4D && code != 0x4E && code != 0x4F && code != 0x24) {
        if (code == 0x25) {
            return (u8)!getGameView_802D6A9C()->field_0x171;
        }
        if (code == 0x26) {
            return (u8)(getGameView_802D6A9C()->field_0x171 == 1);
        }
        if (code != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
            return true;
        }
    }
    return false;
}
