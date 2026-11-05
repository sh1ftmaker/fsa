#include "game/GameAudio.h"
#include "game/act/tu_8012EA00.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"

cls_8049E8F8::~cls_8049E8F8() {}

inline bool cls_8049E8F8::playersInside() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    for (s32 i = 0; i < 4; i++) {
        // (u8) gives the clrlwi on the mask that the original has.
        if (lbl_8053AAF8->field_0x82 & (u8)(1 << i)) {
            if (!fn_8023F264(i)) {
                return false;
            }
            if (fn_8023EA28(i)) {
                return false;
            }
            if (!fn_8023561C(i)) {
                return false;
            }
            if (fn_80230188(i)) {
                return false;
            }
            JGeometry::TVec3f* pos = getPlayerPos_8023E724(i);
            bool inside = false;
            if (pos->x >= box.i.x && pos->y >= box.i.y && pos->x < box.f.x && pos->y < box.f.y) {
                inside = true;
            }
            if (!inside) {
                return false;
            }
        }
    }
    return true;
}

inline void cls_8049E8F8::setState(s32 state) {
    field_0x230 = state;
    switch (field_0x230) {
    case 1:
        fn_8023077C();
        fn_80119DD0()->fn_80119998(0x3C, 0, 1, 0, 1, 0x5A, 0);
        break;
    case 2:
        getGameView_802D6A9C()->fn_802D9FF4(field_0x234, field_0x238);
        lbl_8053AAF8->fn_80136798(15, 0);
        break;
    }
}

// FIXME: 98.5 %. Two differences remain. (1) The player's x and y are loaded
// into f1 and f0 in the original and f0 and f1 here. (2) The inlined switch in
// setState is compiled as cmpwi 1 / beq / bge, then cmpwi 3 / bge (case 2) in
// the original, and as cmpwi 2 first here.
void cls_8049E8F8::update() {
    ActorBase_804A5770::update();
    switch (field_0x230) {
    case 0:
        if (vfunc_0x3C() != 0 && !vfunc_0x34()) {
            break;
        }
        if (playersInside()) {
            setState(2);
        }
        break;
    case 1:
        field_0x23c++;
        if (field_0x23c == 0x78) {
            setState(2);
        }
        break;
    case 2: // done: nothing left to do once state 2 is entered
        break;
    }
}

void cls_8049E8F8::init() {
    ActorBase_804A5770::init();
    field_0x234 = (field_0x90 >> 8) & 0xF;
    field_0x238 = (field_0x90 >> 12) & 0xF;
    mBox.set(-32.0f, -16.0f, 32.0f, 16.0f);
    mBox2.i = mBox.i;
    mBox2.f = mBox.f;
    field_0x80.i = mBox.i;
    field_0x80.f = mBox.f;
}
