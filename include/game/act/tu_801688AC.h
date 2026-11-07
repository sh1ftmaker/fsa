#ifndef GAME_ACT_TU_801688AC_H
#define GAME_ACT_TU_801688AC_H

// Code: game/tu_801688AC.cpp.

#include "game/ActorBase.h"
#include "game/act/tu_801F9484_elems.h"

// Two trails of five recorded points (stride 0x18): two (x, y) records and their layers.
struct TrailPoint_804A0738 {
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    s32 layer0;
    s32 layer1;
};

// Named after its vtable, 0x804A0738.
class cls_804A0738 : public ActorBase_804A5770 {
public:
    cls_804A0738() {}
    /* 0x08 */ virtual ~cls_804A0738();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    // Code: game/tu_801688AC.cpp.
    void fn_8016890C(const JGeometry::TVec3f* pos, s32 layer, JGeometry::TBox2f* out);
    bool fn_80168C38();
    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 field_0x240;
    /* 0x244 */ s32 field_0x244;
    /* 0x248 */ s32 field_0x248;
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250; // sprite frame - 0x69
    /* 0x254 */ s32 field_0x254; // set to 1 by fn_801CC538 (tu_801CB270)
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c; // bit 1 of the spawn argument: the fast variant
    /* 0x260 */ u8 field_0x260;
    /* 0x261 */ u8 field_0x261;
    /* 0x262 */ u8 field_0x262[2];
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ s32 field_0x268;
    /* 0x26C */ s32 field_0x26c;
    /* 0x270 */ s32 field_0x270;
    /* 0x274 */ TrailPoint_804A0738 field_0x274[5];
    /* 0x2EC */ TrailPoint_804A0738 field_0x2ec[5];
    /* 0x364 */ f32 field_0x364[3];
    /* 0x370 */ JGeometry::TBox2f field_0x370; // fn_8016890C's result
}; // Size: 0x380

#endif
