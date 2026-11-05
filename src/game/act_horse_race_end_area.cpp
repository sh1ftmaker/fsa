#include "game/GameAudio.h"
#include "game/Player.h"
#include "game/act/act_horse_race_end_area.h"

HorseRaceEndArea_804AE830::~HorseRaceEndArea_804AE830() {}

static inline bool contains(const JGeometry::TBox2f& box, f32 x, f32 y) {
    return x >= box.i.x && y >= box.i.y && x < box.f.x && y < box.f.y;
}

void HorseRaceEndArea_804AE830::update() {
    JGeometry::TBox2f box = mBox;
    box.addPos(mPos.x, mPos.y);
    for (s32 i = 0; i < 4; i++) {
        JGeometry::TVec3f* pos = fn_8023E724(i);
        if (contains(box, pos->x, pos->y) == true) {
            fn_80230050(i);
        }
    }
}

void HorseRaceEndArea_804AE830::init() {
    f32 halfW = ((field_0x90 & 0xFF) + 1) * 8;
    f32 halfH = (((field_0x90 >> 8) & 0xFF) + 1) * 8;
    if ((field_0x90 >> 16) & 1) {
        mPos.x += halfW;
        mPos.y += halfH;
    } else {
        mPos.x = halfW + (s32)mPos.x / 16 * 16;
        mPos.y = halfH + (s32)mPos.y / 16 * 16;
    }
    mBox.set(-halfW, -halfH, halfW, halfH);
}
