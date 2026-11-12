#ifndef GAME_CLS_804B1D58_H
#define GAME_CLS_804B1D58_H

#include "types.h"
#include "JSystem/JUtility/TColor.h"

class J2DScreen;
class PaneCtrl_804AF86C;
class JPABaseEmitter;
struct J2DAnmTevRegKey;
struct J2DAnmTextureSRTKey;

// 0x10-byte element of field_0x164, named after its constructor
// __ct__12cls_80453830Fv (fn_80453830, game/tu_8044C7F0.cpp), which
// __construct_array runs on the two elements.
struct cls_80453830 {
    cls_80453830(); // out-of-line (game/tu_8044C7F0.cpp): every word -1

    // Four window corner colours (default TColor is 0xFFFFFFFF).
    /* 0x0 */ JUtility::TColor field_0x0;
    /* 0x4 */ JUtility::TColor field_0x4;
    /* 0x8 */ JUtility::TColor field_0x8;
    /* 0xC */ JUtility::TColor field_0xc;
}; // Size: 0x10

// Vtable 0x804B1D58 (one slot, the destructor), 0x188 bytes, defined in
// game/tu_8044C7F0.cpp (not written yet). The constructor fn_80452A80 stores
// `this` in lbl_8053AF40; tu_8037ACC4 allocates it with __nw__(0x188) when
// gGameRoot_8053AAF8->field_0x7f is set.
class cls_804B1D58 {
public:
    cls_804B1D58(); // fn_80452A80
    /* 0x08 */ virtual ~cls_804B1D58();

    u8 fn_8044CB00(); // 0x8044CB00: field_0x140
    void fn_8044CB08(int arg); // 0x8044CB08
    u8 fn_8044D290(); // 0x8044D290: field_0x140
    void fn_8044D298(int arg); // 0x8044D298
    void fn_8044D894(); // 0x8044D894
    s32 fn_8044F680(); // 0x8044F680
    void fn_804522C8(); // 0x804522C8
    void fn_80452308(); // 0x80452308
    void fn_80452350(); // 0x80452350
    // 0x8044C968: puts the pane controls back (slid out by 300, alpha 0) and moves 'Pcul01' to the
    // row field_0xf8; state 2.
    void fn_8044C968();
    // 0x8044C7F0: lets the emitters of 0x6C / 0x88 / 0xB0 run out (stopCreateParticle, setMaxFrame(1), pointer
    // cleared) and deletes the ones of 0xA4 / 0xC0; the destructor inlines it.
    void fn_8044C7F0();
    // 0x80453848: bit 8 of field_0xf4 is set and field_0xd0 + field_0xdc is 0.
    bool fn_80453848();

    /* 0x004 */ J2DScreen* mScreen; // drawn by fn_80452308
    /* 0x008 */ PaneCtrl_804AF86C* field_0x8;
    /* 0x00C */ PaneCtrl_804AF86C* field_0xc[15];
    /* 0x048 */ PaneCtrl_804AF86C* field_0x48[3];
    /* 0x054 */ PaneCtrl_804AF86C* field_0x54[3];
    /* 0x060 */ PaneCtrl_804AF86C* field_0x60[3];
    /* 0x06C */ JPABaseEmitter* field_0x6c[7];
    /* 0x088 */ JPABaseEmitter* field_0x88[7];
    /* 0x0A4 */ JPABaseEmitter* field_0xa4[3];
    /* 0x0B0 */ JPABaseEmitter* field_0xb0[4];
    /* 0x0C0 */ JPABaseEmitter* field_0xc0[3];
    /* 0x0CC */ void* field_0xcc; // "collect.brk" resource
    // 0xD0, 0xF4: fn_80453848 reads them as counts / flags (bit 8 of 0xF4); the constructor loads
    // animations into them and the destructor deletes them.
    union {
        /* 0x0D0 */ s32 field_0xd0; // fn_80453848
        /* 0x0D0 */ J2DAnmTevRegKey* mAnm_0xd0; // "collect.brk"
    };
    /* 0x0D4 */ void* field_0xd4; // "collect.btk" resource
    /* 0x0D8 */ J2DAnmTextureSRTKey* field_0xd8[4];
    /* 0x0E8 */ void* field_0xe8; // "collect_02.btk" resource
    /* 0x0EC */ J2DAnmTextureSRTKey* field_0xec;
    /* 0x0F0 */ void* field_0xf0; // "collect_03.btk" resource
    union {
        /* 0x0F4 */ u32 field_0xf4; // flags; bit 8 is tested by fn_80453848
        /* 0x0F4 */ J2DAnmTextureSRTKey* mAnm_0xf4;
    };
    /* 0x0F8 */ s16 field_0xf8;
    // fn_804522C8 tests it against 7, 8 and 9 (lha).
    /* 0x0FA */ s16 field_0xfa;
    /* 0x0FC */ s16 field_0xfc;
    /* 0x0FE */ s16 field_0xfe;
    /* 0x100 */ u8 field_0x100[0x108 - 0x100];
    /* 0x108 */ f32 field_0x108[3];
    /* 0x114 */ f32 field_0x114;
    /* 0x118 */ f32 field_0x118;
    /* 0x11C */ s32 field_0x11c; // fn_8044D894: the players' fn_80236100 sum
    /* 0x120 */ s32 field_0x120;
    /* 0x124 */ s32 field_0x124;
    /* 0x128 */ s32 field_0x128;
    /* 0x12C */ s32 field_0x12c;
    /* 0x130 */ s16 field_0x130;
    /* 0x132 */ s16 field_0x132;
    /* 0x134 */ s16 field_0x134;
    /* 0x136 */ u8 field_0x136;
    /* 0x137 */ u8 field_0x137; // state (2 after fn_8044CB08)
    /* 0x138 */ u8 field_0x138; // the selected row (0-6) fn_8044CB08 was given
    // Initial alphas of panes (the constructor reads their 0xB2 byte).
    /* 0x139 */ u8 field_0x139;
    /* 0x13A */ u8 field_0x13a;
    /* 0x13B */ u8 field_0x13b;
    /* 0x13C */ u8 field_0x13c;
    /* 0x13D */ u8 field_0x13d;
    /* 0x13E */ u8 field_0x13e;
    /* 0x13F */ u8 field_0x13f; // fn_8037ADD8 / fn_8037AE00 / fn_8037AE30
    /* 0x140 */ u8 field_0x140; // fn_8044CB00 returns it
    /* 0x141 */ u8 field_0x141[0x144 - 0x141];
    // Pane colours (the constructor __construct_array's four pairs of TColor).
    /* 0x144 */ JUtility::TColor field_0x144[2];
    /* 0x14C */ JUtility::TColor field_0x14c[2];
    /* 0x154 */ JUtility::TColor field_0x154[2];
    /* 0x15C */ JUtility::TColor field_0x15c[2];
    /* 0x164 */ cls_80453830 field_0x164[2]; // constructed by __construct_array (0x10 x 2)
    /* 0x184 */ s32 field_0x184; // fn_8044F664 returns it
}; // Size: 0x188

// Data of game/tu_8044C7F0.cpp.
extern cls_804B1D58* lbl_8053AF40;

// 0x8044F664 (game/tu_8044C7F0.cpp): 9 without lbl_8053AF40, else its
// field_0x184.
s32 fn_8044F664();

#endif
