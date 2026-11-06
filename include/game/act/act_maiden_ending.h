#ifndef GAME_ACT_ACT_MAIDEN_ENDING_H
#define GAME_ACT_ACT_MAIDEN_ENDING_H

// Code: game/act_maiden_ending.cpp.

#include "game/act/act_princess_zelda_ending.h"
#include "JSystem/JParticle/JPAEmitter.h"

// The EMIK actor ("Maiden (ending)", docs/names/actors.tsv): constructed by
// fn_80200E94 for actor ID 'EMIK', which stores vtable 0x804AD5B8.
class MaidenEnding_804AD5B8 : public cls_804A5958 {
public:
    MaidenEnding_804AD5B8() {}
    /* 0x08 */ virtual ~MaidenEnding_804AD5B8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    // State functions (in lbl_804AD528)
    void fn_8035D6C4();
    void fn_8035D4B8();
    void fn_8035D1CC();
    void fn_8035CF50();
    void fn_8035CAF8();
    void fn_8035C940();

    // Vec functions (in lbl_804AD594)
    inline JGeometry::TVec3f calcTarget_inline();
    JGeometry::TVec3f fn_8035CE78();
    JGeometry::TVec3f fn_8035CDC0();
    JGeometry::TVec3f fn_8035CD00();

    /* 0x254 */ f32 field_0x254;
    /* 0x258 */ f32 field_0x258;
    /* 0x25C */ f32 field_0x25c;
    /* 0x260 */ s16 field_0x260;
    /* 0x264 */ f32 field_0x264;
    /* 0x268 */ f32 field_0x268;
    /* 0x26C */ f32 field_0x26c;
    /* 0x270 */ s16 field_0x270;
    /* 0x272 */ char field_0x272;
    /* 0x274 */ JPABaseEmitter* field_0x274;
    /* 0x278 */ JPABaseEmitter* field_0x278;
}; // Size: 0x27C

#endif
