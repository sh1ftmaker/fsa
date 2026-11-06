#ifndef GAME_ACT_ACT_DODONGO_H
#define GAME_ACT_ACT_DODONGO_H

#include "types.h"
#include "game/ActorBase.h"

class JPABaseEmitter;

// Spawned for 'DDGO' (Dodongo, docs/names/actors.tsv, high confidence);
// moved here from game/act/tu_801F9484.h. Named after its vtable, 0x804A3334.
class Dodongo_804A3334 : public ActorBase_804A5770 {
public:
    Dodongo_804A3334() {}
    /* 0x08 */ virtual ~Dodongo_804A3334();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    void fn_801AC930();
    void fn_801ACCF0();
    void fn_801ACFF4();
    void fn_801AE5AC();

    void setAnim(s32 anim) {
        if (field_0x290 != anim) {
            field_0x290 = anim;
            field_0x298 = 0;
            mTimers[0] = 0;
            field_0x24c = 0;
            switch (field_0x290) {
            case 0:
                field_0x28c = 0;
                break;
            case 1:
                field_0x28c = 12;
                field_0x258 = field_0x28c;
                break;
            case 2:
                field_0x28c = 24;
                break;
            case 3:
                field_0x28c = 32;
                break;
            case 4:
                field_0x28c = 36;
                break;
            }
        }
    }

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 mTimers[5];
    /* 0x24C */ s32 field_0x24c;
    /* 0x250 */ s32 field_0x250;
    /* 0x254 */ s32 field_0x254;
    /* 0x258 */ s32 field_0x258;
    /* 0x25C */ s32 field_0x25c;
    /* 0x260 */ s32 mTargetSerial;
    /* 0x264 */ s32 field_0x264;
    /* 0x268 */ JGeometry::TVec3f field_0x268;
    /* 0x274 */ f32 field_0x274[6];
    /* 0x28C */ s32 field_0x28c;
    /* 0x290 */ s32 field_0x290;
    /* 0x294 */ s32 field_0x294;
    /* 0x298 */ u8 field_0x298;
    /* 0x299 */ u8 field_0x299[3];
    /* 0x29C */ JPABaseEmitter* mEmitters[2];
}; // Size: 0x2A4

#endif
