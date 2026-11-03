#include "game/GameAudio.h"
#include "game/act/act_thwomp_switch.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/Player.h"

ThwompSwitch_804A8940::~ThwompSwitch_804A8940() {}

void ThwompSwitch_804A8940::init() {}

void ThwompSwitch_804A8940::draw() {}

// FIXME: 90%. The distance test compiles to fmadds (dx * dx + dy * dy fused)
// where the original has fmuls, fmuls, fadds, and we load the player's y
// before its x (the original: x first, with &mPos computed before mPos.x).
void ThwompSwitch_804A8940::update() {
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023A51C(i) == 1 && getField_0x4() == fn_8023DE58(i)) {
            JGeometry::TVec3f* playerPos = fn_8023A478(i);
            f32 dx = playerPos->x - mPos.x;
            f32 dy = playerPos->y - mPos.y;
            if (dx * dx + dy * dy > 6400.0f) {
                continue;
            }
            if (field_0x4 != 0) {
                vfunc_0x30(0xFF, true);
                return;
            }
            if (lbl_8053AB10->field_0x24->fn_8030C2A8(&mPos) == 1) {
                vfunc_0x30(0xFF, true);
                return;
            }
        }
    }
}
