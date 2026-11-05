#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_8033F448.h"

cls_804AC388::~cls_804AC388() {}

void cls_804AC388::draw() {}

// FIXME: 88.4 %. Three differences remain. (1) The original builds the
// fn_80238548 position as an 8-byte stack object (x, y copied from the player
// at 0x8/0xC, then +0.1 and -0.1; fn_80238548 reads only x and y), while a
// TVec3f local puts it at 0x14 and skips the copy; a const TVec2f* prototype
// would fit but act_attack_hitbox_when_player_is_thrown passes &mPos. (2) The
// visible box is copied to the stack with lwz/stw (0x1C..0x28) and then read
// with lfs; here the copy is elided and the lfs read the view directly. (3) The
// spawned flag is r28 and the second loop's i r29 in the original, the reverse
// here (the frame is 0x10 bytes larger in the original for (1) and (2)).
void cls_804AC388::update() {
    bool spawned = false;
    if (field_0x23c == true) {
        getGameView_802D6A9C()->fn_802D9FF4(field_0x234, field_0x238);
        gGameRoot_8053AAF8->fn_80136798(16, 1);
        for (s32 i = 0; i < 4; i++) {
            fn_8023764C(i, fn_80237810(i) - 4);
            fn_8022CA6C(i, 0, true);
        }
        mAlive = false;
    } else {
        if (field_0x230 <= 0) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && getPlayerLayer_8023DE58(i) < 8 && !fn_80230188(i) &&
                    !fn_8022F478(i) && !fn_80236D14(i)) {
                    JGeometry::TVec3f* player = getPlayerPos_8023E724(i);
                    JGeometry::TVec3f pos;
                    pos.x = player->x + 0.1f;
                    pos.y = player->y - 0.1f;
                    // fn_80238548 reads only x and y of the position.
                    fn_80238548(i, 1, (const JGeometry::TVec2f*)&pos, 1);
                    lbl_80529DEC.field_0x239 |= (u8)(1 << i);
                    spawned = true;
                }
            }
            field_0x230 = 300;
        }
        if (spawned) {
            JGeometry::TBox2f box = getGameView_802D6A9C()->mVisibleBox;
            f32 x = box.f.x + 0.5f * (box.i.x - box.f.x);
            f32 y = box.f.y + 0.5f * (box.i.y - box.f.y);
            startSound_801F0E34(0x30036, 0);
            JGeometry::TVec3f pos(x, y, 0.0f);
            spawnEmitter_8013CC50(field_0x4, &pos, 0x1bc, 2, 2, NULL, NULL, true, 1.0f);
        }
        for (s32 i = 0; i < 4; i++) {
            if (!fn_8022F514(i) && fn_80236A18(i) && fn_8023F264(i)) {
                field_0x23c = true;
            }
        }
        field_0x230--;
    }
}

void cls_804AC388::init() {
    field_0x230 = 300;
    field_0x234 = (field_0x90 >> 4) & 0xF;
    field_0x238 = (field_0x90 >> 8) & 0xF;
    field_0x114 = 1;
    field_0x23c = false;
}
