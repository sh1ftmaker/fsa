#ifndef GAME_ACT_ACT_TINGLE_FIRST_H
#define GAME_ACT_ACT_TINGLE_FIRST_H

// Code: game/act_tingle_first.cpp (ACTOR_FTGL, Tingle's first appearance).

#include "game/ActorBase.h"
#include "game/Message.h"

class cls_8031709C;

// Named after its vtable, 0x804AADD8. The slots it overrides: 0x08 the
// destructor, 0x0C init, 0x14 update, 0x20 draw.
class cls_804AADD8 : public ActorBase_804A5770 {
public:
    cls_804AADD8();
    /* 0x08 */ virtual ~cls_804AADD8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // fn_8038AD24 takes the position by value. The original loads the message,
    // serial and layer before that copy, so they are parameters here.
    bool talk(u32 player, u32 msg, s32 serial, s32 layer, const JGeometry::TVec3f& pos, u32 flags) {
        return fn_8038AD24(player, msg, serial, layer, pos, flags);
    }

    // No address (dead-stripped; update inlines it in case 1): finds the
    // nearest player on this actor's layer and starts Tingle's intro sound.
    // Its literal 65536.0f (lbl_805418C0) leads the unit's .sdata2 pool ahead
    // of draw's 1.0f (lbl_805418C4), so it was defined below draw in the
    // original (docs/RULES_CARD.md, literals before their first visible use (3)).
    void selectNearestPlayer();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ cls_8031709C* field_0x24c; // sound/effect sequencer, owned here
    /* 0x250 */ u8 field_0x250[0x260 - 0x250]; // not yet known
    /* 0x260 */ JGeometry::TBox2f field_0x260;
    /* 0x270 */ u8 field_0x270;
    // Size: unknown; the fields used so far end at 0x271.
};

#endif
