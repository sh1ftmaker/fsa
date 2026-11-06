#ifndef GAME_ACT_ACT_SHOVEL_OLD_TIMER_H
#define GAME_ACT_ACT_SHOVEL_OLD_TIMER_H

// Code: game/act_shovel_old_timer.cpp.

#include "game/ActorBase.h"
#include "game/Message.h"

// Named after its vtable, 0x804AAB40.
class ShovelOldTimer_804AAB40 : public ActorBase_804A5770 {
public:
    ShovelOldTimer_804AAB40() {}
    /* 0x08 */ virtual ~ShovelOldTimer_804AAB40();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_8031179C();
    void showHint(s32 player, s32 msg);
    bool findDigSpot();

    bool talk(s32 player, u32 msg, s32 layer, s32 serial, const JGeometry::TVec3f& pos, u32 flags) {
        return fn_8038C678(player, msg, serial, layer, pos, flags);
    }

    /* 0x230 */ JGeometry::TBox2f field_0x230;
    /* 0x240 */ JGeometry::TVec3f field_0x240;
    /* 0x24C */ s32 mState;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ bool field_0x258;
    /* 0x259 */ bool field_0x259;
    /* 0x25A */ bool field_0x25a;
}; // Size: 0x25C

#endif
