#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Player.h"
#include "game/act/act_invisible_npc_wall.h"

InvisibleNPCWallForLink_804AE780::~InvisibleNPCWallForLink_804AE780() {}

void InvisibleNPCWallForLink_804AE780::update() {
    if (field_0x230 == 0 || vfunc_0x34()) {
        JGeometry::TBox2f box = mBox;
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        fn_80239914(&box, field_0x4, mSerial);
    }
}

void InvisibleNPCWallForLink_804AE780::init() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    f32 w = ((field_0x90 & 0xFF) + 1) * 8;
    f32 h = (((field_0x90 >> 8) & 0xFF) + 1) * 8;
    if ((field_0x90 >> 16) & 1) {
        mPos.x += w;
        mPos.y += h;
    } else {
        mPos.x = w + (s32)mPos.x / 16 * 16;
        mPos.y = h + (s32)mPos.y / 16 * 16;
    }
    mBox.set(-w, -h, w, h);
    field_0x80.set(-w, -h, w, h);
    field_0x230 = field_0x90 >> 27;
}
