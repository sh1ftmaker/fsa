#include "game/GameAudio.h"
#include "game/Map.h"
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"
#include "game/act/act_fish.h"

// FIXME(shotgun): not written (size 0x4FC): init (0x80346704)
bool Fish_804AC658::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 tile = (u16)fn_8022461C(pos, layer);
    if (tile == 0xD || tile == 0x3A) {
        return false;
    }
    return true;
}

// FIXME(shotgun): not written (size 0x390): fn_80346330
// FIXME(shotgun): not written (size 0xBEC): update (0x80345744)

// FIXME(shotgun): 76.2 %, missing/extra (the SpriteParams stores and the matrix are scheduled differently)
void Fish_804AC658::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x10.a = 200;
    params.field_0x1c = -1;
    params.field_0x1d = -1;
    params.field_0x21 = 1;
    params.mMtx = mtx;
    f32 angle = (f32)field_0x252 / -10430.228f;
    setRotScaleMtx(mtx, JGeometry::TVec3f(0.0f, angle, 0.0f), 1.0f);
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x24c + 0x283, &params, -1, 1.0f, 1.0f);
}

Fish_804AC658::~Fish_804AC658() {}
