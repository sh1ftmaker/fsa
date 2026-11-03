#ifndef GAME_ACT_TU_801D7AFC_H
#define GAME_ACT_TU_801D7AFC_H

// Code: game/tu_801D7AFC.cpp. An EnemyBase_804A4808 that slides along
// field_0x3c with two effects (ids 0x226 and 0x227) following it, named
// after its vtable (0x804A4BB0). It is drawn as a "SOB " sprite (cel
// mAnim.mCel + 0x1C7) and scales its box and effects with fn_801D7DB4,
// which tu_801D6CEC's code calls (0x801D7090, with f1 = field_0x60).

#include "types.h"
#include "game/EnemyBase.h"

class JPABaseEmitter;

class cls_804A4BB0 : public EnemyBase_804A4808 {
public:
    cls_804A4BB0();
    virtual ~cls_804A4BB0();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // 0x801D7DB4. Sets field_0x54 to (scale, scale, scale), mBox to
    // (-3, -3)-(3, 3) times it and mBox2 to (-6, -6)-(6, 6) times it, and
    // gives both effects the new scale (init() inlines it for 0 and 1).
    void fn_801D7DB4(f32 scale);

    /* 0x2A8 */ JPABaseEmitter* mEmitters[2];
}; // Size: 0x2B0

#endif
