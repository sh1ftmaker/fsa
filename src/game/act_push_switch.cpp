#include "game/GameAudio.h"
#include "game/Player.h"
#include "game/act/act_push_switch.h"

PushSwitch_804A7B38::~PushSwitch_804A7B38() {}

void PushSwitch_804A7B38::draw() {}

// FIXME: 96.65%. Same instructions, but the original keeps the loop counter in r29 and the
// player count in r30, with the param bits in a temporary r31 (shared with the earlier
// field_0x4 load); ours puts the count in r31, the counter in r30 and the bits in r29.
void PushSwitch_804A7B38::update() {
    JGeometry::TBox2f area(mBox);
    area.i.x += mPos.x;
    area.f.x += mPos.x;
    area.i.y += mPos.y;
    area.f.y += mPos.y;
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (getPlayerLayer_8023DE58(i) == field_0x4 && fn_8022DB04(i)) {
            s32 direction = (field_0x90 >> 17) & 3;
            if (direction != fn_80230CFC(i)) {
                continue;
            }
            JGeometry::TVec3f* pos = getPlayerPos_8023E724(i);
            bool inside = false;
            if (pos->x >= area.i.x && pos->y >= area.i.y && pos->x < area.f.x &&
                pos->y < area.f.y) {
                inside = true;
            }
            if (inside) {
                count++;
            }
        }
    }
    s32 needed = 1;
    if ((field_0x90 >> 22) & 0x1F) {
        needed = (field_0x90 >> 22) & 0x1F;
    }
    if (count >= needed) {
        vfunc_0x30(0xFF, true);
    } else {
        vfunc_0x30(0xFF, false);
    }
}

void PushSwitch_804A7B38::init() {
    switch ((field_0x90 >> 17) & 3) {
    case 0:
        mBox.set(-15.0f, 0.0f, 0.0f, 16.0f * getLength());
        break;
    case 1:
        mBox.set(16.0f, 0.0f, 31.0f, 16.0f * getLength());
        break;
    case 2:
        mBox.set(0.0f, -15.0f, 16.0f * getLength(), 0.0f);
        break;
    case 3:
        mBox.set(0.0f, 16.0f, 16.0f * getLength(), 31.0f);
        break;
    }
    field_0x80.set(mBox);
}
