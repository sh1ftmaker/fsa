#ifndef GAME_TU_803A8DFC_H
#define GAME_TU_803A8DFC_H

#include "types.h"
#include "JSystem/JGeometry.h"
#include "JSystem/J2D/J2DPicture.h"

class J2DScreen;
class J2DPane;
class J2DTextBox;
class J2DAnmTextureSRTKey;
class JPABaseEmitter;
class ActorSoundObj_804578F4;
class cls_804B20F8;

// The results screen (game/tu_803A8DFC.cpp): vtable 0x804AF9A0 (only the
// destructor, fn_803B6D04), constructor fn_803B6FD8, 0x4E0 bytes
// (__nw__(0x4E0) in cls_804AE9A0's constructor fn_8037A8D4, tu_80379F84,
// which keeps it as its field_0x4). The constructor stores the object in
// lbl_8053ADA0 and loads, from the archive cls_804AE9A0 holds at 0x18, the
// screens "result_{4,3,2}P.blo" (field_0x4), "result_score_{4,3,2}P.blo"
// (field_0x10), "result_back.blo" and "result_title.blo", with the
// "result_{4,3,2}P.btk" texture animations; lbl_804AF980 lists the player
// ("P1.bti".."P4.bti") and rank ("kk_1st_01.bti".."kk_4th_01.bti") textures.
// Named after those resources (docs/names/names.tsv, med).
class ResultScreen_804AF9A0 {
public:
    ResultScreen_804AF9A0();
    /* 0x08 */ virtual ~ResultScreen_804AF9A0();

    // For player 0-3, the player fn_8037EFB4(player) (0-2) names among the
    // other three (fn_803B0978 / fn_803B14F8).
    s32 fn_803A8DFC(s32 player);
    // Called by cls_804AE9A0 (tu_80379F84); no other arguments.
    void fn_803A8F5C();
    // Update (cls_804AE9A0::fn_8037A580): steps the mode field_0x4c0 (0 wait,
    // 1 fade in, 2-4 the result pages fn_803AA8F8 / fn_803AA1F8 / fn_803A9AB8,
    // 5 fade out, 6 fade to mode 5 or 9), then the field_0x450 layout's
    // texture animation.
    void fn_803A9830();
    // Mode 4 / 3: the score bars (field_0x158 panes, from the field_0x320..
    // field_0x3b0 totals), the sprites, rank effects; buttons 0x1100 add the
    // round to the save file and go to mode 6 / 4.
    void fn_803A9AB8();
    void fn_803AA1F8();
    // Mode 2: the state machine field_0x4c1 (states 0-33), then the score bars.
    void fn_803AA8F8();
    // Draw: field_0x16c's alpha from field_0x4c2 (mode 2 and up), then
    // result_back.blo.
    void fn_803A97A0();
    // Update states (field_0x4c1) counting field_0x440 down. The five
    // title-pane steps slide one field_0x20 pane out and the next in (or
    // skip on button 0x100), then go to the state they name.
    void fn_803AD220(); // 'Pdhbk02' -> 'Plhbk02', then state 26
    void fn_803ADC0C(); // 'Pllosb06' -> 'Pfobk02', then state 21
    void fn_803AE308(); // 'Prmhb06' -> 'Pllosb06', then state 19
    void fn_803AEC78(); // 'Pembsb06' -> 'Prmhb06', then state 16
    void fn_803B0158(); // 'Pttlbk05' -> 'Pembsb06', then state 9
    void fn_803AF604();
    // Called by fn_803A9AB8 / fn_803AA1F8 / fn_803AA8F8 each frame: player
    // input (button 0x40) marks a player in field_0x4b0, lists it in
    // field_0x4b4, then shows the listed players' effects at field_0x2f0.
    void fn_803B2FB0();
    // Draws the fade picture field_0x170 (alpha field_0x4c2) over the screen,
    // except on the stages fn_80379FF4 skips and while
    // lbl_80529DEC.field_0x237 is 8.
    void fn_803B31AC();
    // Switches on index (0-7 / 0-8; fn_803AF604 passes field_0x4c7) and
    // loops over the players in each case, doing nothing.
    void fn_803B3358(s32 index);
    void fn_803B35F8(s32 index);
    // Reset: picks the 4P / 3P / 2P layout from the player count (and sets
    // it up with fn_803B5378 / fn_803B42C0 / fn_803B38E8), puts the title
    // panes back, clears the per-player state and starts the music.
    void fn_803B6768();
    void fn_803B38E8();
    void fn_803B42C0();
    void fn_803B5378();
    void fn_803B236C();
    // Inlined into fn_803B236C (six copies): effect `id` in the first free
    // field_0x1c8 slot, 25 units off field_0x244, kept alive.
    void spawnEffect(u16 id, f32 scale);
    s32 findFreeEffect(); // first NULL field_0x1c8 slot, or -1
    // The 4P result states (field_0x4c1, fn_803AA8F8's switch).
    void fn_803AB494();
    void fn_803ABCEC();
    void fn_803AC154();
    void fn_803AC6FC();
    void fn_803AC998();
    void fn_803ACDA4();
    void fn_803AD4B0();
    void fn_803AD894();
    void fn_803ADE9C();
    void fn_803AE598();
    void fn_803AE810();
    void fn_803AEF08();
    void fn_803AF1B0();
    void fn_803AF79C();
    void fn_803AFA78();
    void fn_803AFCF0();
    void fn_803B03E8();
    void fn_803B0978();
    void fn_803B0FD4();
    void fn_803B14F8();
    void fn_803B1B60();
    // Dead-stripped helper (see the .cpp): true when the winners' animation ends.
    BOOL animateWinners();

    /* 0x000 */ // vtable
    /* 0x004 */ J2DScreen* field_0x4[3];  // "result_4P.blo", "result_3P.blo", "result_2P.blo"
    /* 0x010 */ J2DScreen* field_0x10[3]; // "result_score_4P.blo", 3P, 2P
    /* 0x01C */ J2DScreen* field_0x1c;    // "result_back.blo"
    /* 0x020 */ J2DScreen* field_0x20;    // "result_title.blo"
    /* 0x024 */ J2DAnmTextureSRTKey* field_0x24[3]; // "result_{4,3,2}P.btk" on field_0x4[i]
    // Per-player rank panes (fn_803AB494): the rank texture pictures 0x30 /
    // 0x40 (0x40 also takes the field_0x46c colours) and the panes shown for
    // first place (0x50) or the other places (0x60).
    /* 0x030 */ J2DPicture* field_0x30[4];
    /* 0x040 */ J2DPicture* field_0x40[4];
    /* 0x050 */ J2DPane* field_0x50[4];
    /* 0x060 */ J2DPane* field_0x60[4];
    // field_0x20's panes (constructor).
    /* 0x070 */ J2DPane* field_0x70; // 'Pttlbk05'
    /* 0x074 */ J2DPane* field_0x74; // 'Pdrwbk05'
    /* 0x078 */ J2DPane* field_0x78; // 'Pwinbk05'
    /* 0x07C */ J2DPane* field_0x7c; // 'Pwnr01'
    /* 0x080 */ J2DPane* field_0x80; // 'Pwnr02'
    /* 0x084 */ u8 field_0x84[0x8C - 0x84];
    /* 0x08C */ J2DPane* field_0x8c; // 'Pembsb06'
    /* 0x090 */ J2DPane* field_0x90; // 'Prmhb06'
    /* 0x094 */ J2DPane* field_0x94; // 'Pllosb06'
    /* 0x098 */ J2DPane* field_0x98; // 'Plhbk02'
    /* 0x09C */ J2DPane* field_0x9c; // 'Pdhbk02'
    /* 0x0A0 */ J2DPane* field_0xa0; // 'Phabk02'
    /* 0x0A4 */ J2DPane* field_0xa4; // 'Pfobk02'
    // Score digit text boxes per player (fn_803A8F5C): sign / thousands /
    // hundreds / tens / ones, twice (0xA8-0xE8 and 0xF8-0x138, the same
    // text in both).
    /* 0x0A8 */ J2DTextBox* field_0xa8[4];
    /* 0x0B8 */ J2DTextBox* field_0xb8[4];
    /* 0x0C8 */ J2DTextBox* field_0xc8[4];
    /* 0x0D8 */ J2DTextBox* field_0xd8[4];
    /* 0x0E8 */ J2DTextBox* field_0xe8[4];
    /* 0x0F8 */ J2DTextBox* field_0xf8[4];
    /* 0x108 */ J2DTextBox* field_0x108[4];
    /* 0x118 */ J2DTextBox* field_0x118[4];
    /* 0x128 */ J2DTextBox* field_0x128[4];
    /* 0x138 */ J2DTextBox* field_0x138[4];
    // field_0x4[field_0x450]'s per-player panes (fn_803B38E8: 'Pltest1', 'Pltwst2').
    /* 0x148 */ J2DPane* field_0x148[4];
    // Per-player panes whose centre, with field_0x168's offset, places the
    // player sprite field_0x2f0[i] (fn_803B6768).
    /* 0x158 */ J2DPane* field_0x158[4];
    /* 0x168 */ J2DPane* field_0x168;
    /* 0x16C */ J2DPane* field_0x16c;
    /* 0x170 */ J2DPicture* field_0x170; // "kk_black.bti", the fade
    /* 0x174 */ cls_804B20F8* field_0x174[4]; // one sound object per player
    /* 0x184 */ ActorSoundObj_804578F4* field_0x184; // at field_0x244
    // Emitters the destructor lets run out (becomeInvalidEmitter).
    /* 0x188 */ JPABaseEmitter* field_0x188[4];
    /* 0x198 */ JPABaseEmitter* field_0x198[4];
    /* 0x1A8 */ JPABaseEmitter* field_0x1a8[4];
    /* 0x1B8 */ JPABaseEmitter* field_0x1b8[4];
    // Emitters the destructor deletes (deleteEmitter).
    /* 0x1C8 */ JPABaseEmitter* field_0x1c8[30];
    /* 0x240 */ u8 field_0x240[0x244 - 0x240];
    /* 0x244 */ JGeometry::TVec3f field_0x244;
    /* 0x250 */ JGeometry::TVec3f field_0x250[10];
    /* 0x2C8 */ s32 field_0x2c8[10];
    /* 0x2F0 */ JGeometry::TVec3f field_0x2f0[4]; // per player: sprite position (fn_803B6768, fn_803A8F5C)
    // Per-player words, cleared by fn_803B6768 (which clears field_0x350
    // twice and leaves field_0x330 alone).
    /* 0x320 */ s32 field_0x320[4];
    /* 0x330 */ s32 field_0x330[4];
    /* 0x340 */ s32 field_0x340[4];
    /* 0x350 */ s32 field_0x350[4];
    /* 0x360 */ s32 field_0x360[4];
    /* 0x370 */ s32 field_0x370[4];
    /* 0x380 */ s32 field_0x380[4];
    /* 0x390 */ s32 field_0x390[4];
    /* 0x3A0 */ s32 field_0x3a0[4];
    /* 0x3B0 */ s32 field_0x3b0[4];
    /* 0x3C0 */ s32 field_0x3c0[4]; // per player: sprite frame (62 at reset)
    /* 0x3D0 */ s32 field_0x3d0[4];
    /* 0x3E0 */ s32 field_0x3e0[4]; // per player: the score fn_803A8F5C prints
    /* 0x3F0 */ f32 field_0x3f0; // field_0x4[0] 'Pltest4' / 'Prpst04' / 'Ndata04' / 'Ndata05'
    // field_0x20 panes' y (constructor).
    /* 0x3F4 */ f32 field_0x3f4; // 'Pttlbk05'
    /* 0x3F8 */ f32 field_0x3f8; // 'Pembsb06'
    /* 0x3FC */ f32 field_0x3fc; // 'Prmhb06'
    /* 0x400 */ f32 field_0x400; // 'Pllosb06'
    /* 0x404 */ f32 field_0x404; // 'Pdhbk02'
    /* 0x408 */ f32 field_0x408; // 'Plhbk02'
    /* 0x40C */ f32 field_0x40c; // 'Pfobk02'
    /* 0x410 */ f32 field_0x410; // 'Phabk02'
    /* 0x414 */ f32 field_0x414; // 'Pwinbk05'
    /* 0x418 */ f32 field_0x418; // 'Pdrwbk05'
    /* 0x41C */ f32 field_0x41c;
    /* 0x420 */ f32 field_0x420;
    /* 0x424 */ f32 field_0x424;
    /* 0x428 */ f32 field_0x428;
    /* 0x42C */ f32 field_0x42c;
    /* 0x430 */ f32 field_0x430;
    /* 0x434 */ s32 field_0x434; // 1200 at reset
    /* 0x438 */ s32 field_0x438;
    /* 0x43C */ s32 field_0x43c;
    /* 0x440 */ s32 field_0x440; // state timer
    /* 0x444 */ s32 field_0x444;
    /* 0x448 */ s32 field_0x448;
    /* 0x44C */ s32 field_0x44c; // animation frame
    /* 0x450 */ s32 field_0x450; // layout: 0 4P, 1 3P, 2 2P (field_0x4 / field_0x24 index)
    /* 0x454 */ s32 field_0x454; // player count
    /* 0x458 */ s16 field_0x458;
    /* 0x45A */ s16 field_0x45a;
    /* 0x45C */ s16 field_0x45c; // -1 at reset
    /* 0x45E */ s16 field_0x45e; // -1 at reset
    /* 0x460 */ s16 field_0x460[4];
    /* 0x468 */ u8 field_0x468[0x46C - 0x468];
    // Corner colours of field_0x40's pictures per rank (fn_803AB494); white
    // (TColor's default) unless set. __construct_array builds them with the
    // out-of-line TCornerColor constructor (0x803B7B90).
    /* 0x46C */ J2DPicture::TCornerColor field_0x46c[4];
    // Per-player bytes, reset by fn_803B6768 (0x4AC / 0x4B0 to 0, the rest to 0xFF).
    /* 0x4AC */ u8 field_0x4ac[4];
    /* 0x4B0 */ u8 field_0x4b0[4]; // bit 1: listed in field_0x4b4
    /* 0x4B4 */ u8 field_0x4b4[4]; // listed players, 0xFF after the last
    /* 0x4B8 */ u8 field_0x4b8[4];
    /* 0x4BC */ u8 field_0x4bc[4];
    /* 0x4C0 */ u8 field_0x4c0; // mode, read by fn_80379F84 (tu_80379F84)
    /* 0x4C1 */ u8 field_0x4c1; // state
    /* 0x4C2 */ u8 field_0x4c2; // fade alpha
    /* 0x4C3 */ u8 field_0x4c3;
    /* 0x4C4 */ u8 field_0x4c4;
    /* 0x4C5 */ u8 field_0x4c5;
    /* 0x4C6 */ u8 field_0x4c6;
    /* 0x4C7 */ u8 field_0x4c7;
    /* 0x4C8 */ u8 field_0x4c8;
    // Alphas for field_0x10's panes (fn_803AA8F8 state 1): the
    // lbl_80476208 tags, then each player's lbl_80476270 / lbl_804762D0 tags.
    /* 0x4C9 */ u8 field_0x4c9[13];
    /* 0x4D6 */ u8 field_0x4d6[3];
    /* 0x4D9 */ u8 field_0x4d9[2];
    /* 0x4DB */ u8 field_0x4db; // bits 0-3: draw a second sprite for player i; bits 4-7 pick its alpha
    /* 0x4DC */ u8 field_0x4dc; // that sprite's alpha (bit i + 4 clear)
    /* 0x4DD */ u8 field_0x4dd; // that sprite's alpha (bit i + 4 set)
    /* 0x4DE */ u8 field_0x4de[0x4E0 - 0x4DE];
}; // Size: 0x4E0

// .sbss 0x8053ADA0: the screen (set by the constructor, cleared by the
// destructor).
extern ResultScreen_804AF9A0* lbl_8053ADA0;

#endif
