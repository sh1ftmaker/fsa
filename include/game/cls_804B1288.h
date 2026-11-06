#ifndef GAME_CLS_804B1288_H
#define GAME_CLS_804B1288_H

#include "types.h"
#include "JSystem/JUtility/TColor.h"

class J2DScreen;
class PaneCtrl_804AF86C;
class cls_804B1B60;
class cls_804B0288;
class cls_804B20F8;
class ActorSoundObj_804578F4;
class JPABaseEmitter;
class J2DAnmTransform;
class J2DAnmColor;

// Vtable 0x804B1288 (one slot, the destructor), 0x66C bytes, defined in
// game/tu_80403D84.cpp. The constructor fn_804122F4 stores
// `this` in lbl_8053ADF8; tu_8039003C allocates it with __nw__(0x66C) when
// gGameRoot_8053AAF8->field_0x7f is set. Its methods are the handlers of a state
// machine (field_0x350).
class MapScreen_804B1288 {
public:
    typedef void (MapScreen_804B1288::*StateFunc)();

    MapScreen_804B1288(); // fn_804122F4
    /* 0x08 */ virtual ~MapScreen_804B1288();

    // 0x80403D84: starts animation 0xB on the 12 panes of field_0x240 (to
    // 0 / 100); if the first one finishes it clears 0x659 / 0x65A.
    void fn_80403D84();
    // 0x80403E24: counts field_0x42a down; at 0 clears the panes' field_0x22
    // and sets 0x659 to 3.
    void fn_80403E24();
    // 0x80403EAC: starts animation 0x13 on the 12 panes (to 100 / 0); if the
    // first one finishes it sets field_0x42a to 100 and 0x659 to 2.
    void fn_80403EAC();
    // 0x80403F50: when 0x65A is set, resets the panes, builds the text of
    // message field_0x390 (fn_803837A4) in field_0x5dc, copies it into the two
    // text boxes of field_0x270 and moves the panes; 0x659 becomes 1.
    void fn_80403F50();
    // 0x80408F38: sets the alpha of the 9 panes of field_0x14 named by
    // lbl_8048C5D0 to field_0x642[i] * alpha.
    void fn_80408F38(f32 alpha);
    // 0x80409330: fn_8037AA34 then, for its result 0x1A, resets the 15 panes of
    // field_0x278 (state 0xF), for 0x19 sets field_0x668; while 0x663 is set
    // it wobbles field_0x3a0 / field_0x3a4 with two sine waves.
    void fn_80409330();
    // 0x804045F8 (this unused): when gGameRoot_8053AAF8's byte 0x7E is set, whether
    // bit `bit` of byte 0x7A of the current save row is set.
    BOOL fn_804045F8(s32 bit);
    // 0x80404074: counts the stage flags 0-3 of `stage` set (isStageFlagSet),
    // less one when that leaves at most 1 (stage 7) / 2 (other stages); may set
    // *out from field_0x388; true when `index` is not 1 (stage 7) / 2 and is
    // above the count or equal to *out.
    bool fn_80404074(s32 stage, s32 index, s32* out);
    // 0x80404D70: with every stage open (fn_803B8EBC(7)) hides the route panes
    // of stages 1-6 and counts 7; else walks the open stages, hiding the route
    // panes of each whose bit in the save file's byte 0x62 is set.
    void fn_80404D70();
    // Dead-stripped (fn_80404D70 inlines it twice): hides field_0xc's panes
    // P21-P23, P31-P34, P41-P43, P51-P53, P61-P62 or P71-P73 of `stage` 1-6.
    void hideStagePanes(s32 stage);
    // 0x80404270: false while lbl_80529DEC's byte 0x23F or field_0x656 is set,
    // outside GameRoot's 0x7E mode, or when fn_804045F8(stage) is set; else
    // stage flag 1 of stages 0-6 (isStageFlagSet), flag 2 of stage 7.
    bool fn_80404270(s32 stage);
    // 0x8040E278: counts field_0x384 up; past 100 the state becomes 0x15.
    void fn_8040E278();
    // 0x8040E298: after 60 frames: state 6 when field_0x664 is set, else
    // state 0x10 (resetting field_0x278's panes and playing sound 0x56) when
    // fn_80404270(field_0x340), else state 0x15.
    void fn_8040E298();
    // 0x8040E700: unless 0x65A is set, clears 0x664 and the state becomes 8.
    void fn_8040E700();
    // 0x8040E720: counts field_0x35c up; past 10 it plays sound 0x30039 on
    // field_0x2c8 (at frame 22) and deletes the emitters of field_0x2ec once
    // they are done; when all three are gone the state becomes 8.
    void fn_8040E720();
    // 0x8040F288: eases field_0x39c from field_0x358; once it reaches 0 the
    // 15 panes of field_0x278 are reset (0x65D set) and the state becomes 0xF.
    void fn_8040F288();
    // 0x8040F35C: starts animation 0x14 on field_0x11c's pane (to 200 / 0);
    // when it finishes the state becomes 0xB.
    void fn_8040F35C();
    // 0x8040F3AC: starts animation 0x14 on the panes of field_0x228 (to -150 /
    // 0), field_0x238 and field_0x23c, and on field_0x11c's pane (to 200 /
    // 0); when that one finishes the state becomes 0xB.
    void fn_8040F3AC();
    // 0x80411A94: sets the alpha of the panes Ttlv11-14 of field_0x10 from
    // field_0x64b / field_0x64c times alpha (the last two times 1 - alpha).
    void fn_80411A94(f32 alpha);
    // 0x8040F480: stores 6 in field_0x668, then sets the object up.
    void fn_8040F480();
    void fn_804108B4(); // 0x804108B4
    // 0x804111C4: draws the screen.
    void fn_804111C4();
    void fn_80411230(); // 0x80411230
    void slidePane(s16 frame, s16 frames);
    // State functions not written yet, lbl_804B10F8[state] (index in the comment;
    // fn_804108B4 calls lbl_804B10F8[field_0x350] through lbl_8053ADF8).
    void fn_8040F16C(); // 3
    void fn_8040ED20(); // 4
    void fn_8040E838(); // 5
    void fn_8040E3A0(); // 8
    void fn_8040D3D0(); // 11
    void fn_8040CE90(); // 12
    void fn_8040C950(); // 13
    void fn_8040BC1C(); // 14
    void fn_80409FDC(); // 15
    void fn_8040B93C(); // 16
    void fn_8040B5A0(); // 17
    void fn_8040B388(); // 18
    void fn_804096E8(); // 19
    void fn_80409450(); // 20
    // 0x80404648 (called by fn_8040B2C4) / 0x804049DC (fn_8040AB4C): when bit
    // 0x10 of field_0x38c is set, clears the step counters of the screen's
    // panes, hides the Ppoint / route panes of stage field_0x340, moves the
    // eight field_0x28 rows to (-150, 0), hides Ndata10 and the state becomes
    // 0x13; else field_0x668 = 4 and sound 0xB. They differ only in where
    // Ndata10 is hidden.
    void fn_80404648();
    void fn_804049DC();


    /* 0x004 */ cls_804B1B60* field_0x4; // __nw__(0x50), fn_80444B68
    /* 0x008 */ cls_804B0288* field_0x8; // __nw__(0x2180), fn_803D58E0
    /* 0x00C */ J2DScreen* field_0xc;    // "map.blo"
    /* 0x010 */ J2DScreen* field_0x10; // the screen fn_80411A94 fades
    /* 0x014 */ J2DScreen* field_0x14; // the screen whose panes fn_80408F38 fades
    /* 0x018 */ J2DScreen* field_0x18; // the screen fn_804111C4 draws
    /* 0x01C */ J2DAnmTransform* field_0x1c; // "map.bck" (field_0xc's setAnimation)
    /* 0x020 */ J2DAnmColor* field_0x20;     // field_0xc's colour animation
    /* 0x024 */ u8 field_0x24[0x28 - 0x24];
    // Eight rows of five panes; the destructor deletes [i][0-2].
    /* 0x028 */ PaneCtrl_804AF86C* field_0x28[8][5];
    /* 0x0C8 */ PaneCtrl_804AF86C* field_0xc8;
    /* 0x0CC */ PaneCtrl_804AF86C* field_0xcc;
    /* 0x0D0 */ PaneCtrl_804AF86C* field_0xd0;
    /* 0x0D4 */ PaneCtrl_804AF86C* field_0xd4[18];
    /* 0x11C */ PaneCtrl_804AF86C* field_0x11c;
    /* 0x120 */ PaneCtrl_804AF86C* field_0x120[2];
    /* 0x128 */ PaneCtrl_804AF86C* field_0x128[32]; // four per stage
    /* 0x1A8 */ PaneCtrl_804AF86C* field_0x1a8[24]; // three per stage
    /* 0x208 */ PaneCtrl_804AF86C* field_0x208[8];  // one per stage
    /* 0x228 */ PaneCtrl_804AF86C* field_0x228[2][2];
    /* 0x238 */ PaneCtrl_804AF86C* field_0x238;
    /* 0x23C */ PaneCtrl_804AF86C* field_0x23c;
    /* 0x240 */ PaneCtrl_804AF86C* field_0x240[12]; // panes the state handlers animate together
    /* 0x270 */ PaneCtrl_804AF86C* field_0x270[2]; // text panes
    /* 0x278 */ PaneCtrl_804AF86C* field_0x278[15]; // three groups of five panes
    /* 0x2B4 */ PaneCtrl_804AF86C* field_0x2b4; // the pane fn_804108B4 slides in and out
    /* 0x2B8 */ cls_804B20F8* field_0x2b8[4]; // sound objects (__nw__(0x60))
    /* 0x2C8 */ ActorSoundObj_804578F4* field_0x2c8;
    /* 0x2CC */ JPABaseEmitter* field_0x2cc[4];
    /* 0x2DC */ JPABaseEmitter* field_0x2dc[4];
    /* 0x2EC */ JPABaseEmitter* field_0x2ec[3];
    /* 0x2F8 */ JPABaseEmitter* field_0x2f8[4];
    /* 0x308 */ JPABaseEmitter* field_0x308[2];
    /* 0x310 */ JUtility::TColor field_0x310[6];
    /* 0x328 */ JUtility::TColor field_0x328[6];
    /* 0x340 */ s32 field_0x340; // the selected stage (0-7; fn_80404270)
    /* 0x344 */ u8 field_0x344[0x350 - 0x344];
    /* 0x350 */ s32 field_0x350; // state
    /* 0x354 */ u8 field_0x354[0x358 - 0x354];
    /* 0x358 */ s32 field_0x358;
    /* 0x35C */ s32 field_0x35c; // frame counter of fn_8040E720
    /* 0x360 */ u8 field_0x360[0x364 - 0x360];
    /* 0x364 */ s32 field_0x364; // first stage whose bit fn_80404D70 finds clear
    /* 0x368 */ u8 field_0x368[0x370 - 0x368];
    /* 0x370 */ s32 field_0x370; // stages fn_80404D70 counts as open
    /* 0x374 */ u8 field_0x374[0x378 - 0x374];
    /* 0x378 */ s32 field_0x378;
    /* 0x37C */ s32 field_0x37c;
    /* 0x380 */ u8 field_0x380[0x384 - 0x380];
    /* 0x384 */ s32 field_0x384; // frame counter of state 0x15
    /* 0x388 */ s32 field_0x388; // -1, 0 or 1 (fn_80404074)
    /* 0x38C */ u32 field_0x38c; // flags; bit 0x10 lets fn_80404648 / fn_804049DC open the stage view
    /* 0x390 */ s32 field_0x390; // message id
    /* 0x394 */ u8 field_0x394[0x39C - 0x394];
    /* 0x39C */ f32 field_0x39c;
    /* 0x3A0 */ f32 field_0x3a0;
    /* 0x3A4 */ f32 field_0x3a4;
    /* 0x3A8 */ u8 field_0x3a8[0x41E - 0x3A8];
    /* 0x41E */ s16 field_0x41e;
    /* 0x420 */ u8 field_0x420[0x42A - 0x420];
    /* 0x42A */ s16 field_0x42a;
    /* 0x42C */ s16 field_0x42c; // frame counter of fn_8040E298
    /* 0x42E */ u8 field_0x42e[0x430 - 0x42E];
    /* 0x430 */ s16 field_0x430; // frame of field_0x2b4's slide
    /* 0x432 */ u8 field_0x432[0x434 - 0x432];
    /* 0x434 */ s16 field_0x434;
    /* 0x436 */ s16 field_0x436;
    /* 0x438 */ u8 field_0x438[0x5DC - 0x438];
    /* 0x5DC */ char field_0x5dc[1]; // text buffer, length not known
    /* 0x5DD */ u8 field_0x5dd[0x641 - 0x5DD];
    /* 0x641 */ u8 field_0x641; // alpha of the Ppoint panes (fn_80404648)
    /* 0x642 */ u8 field_0x642[9]; // alpha of each pane of lbl_8048C5D0
    /* 0x64B */ u8 field_0x64b;
    /* 0x64C */ u8 field_0x64c;
    /* 0x64D */ u8 field_0x64d[0x656 - 0x64D];
    /* 0x656 */ u8 field_0x656;
    /* 0x657 */ u8 field_0x657;
    /* 0x658 */ u8 field_0x658;
    /* 0x659 */ u8 field_0x659;
    /* 0x65A */ u8 field_0x65a;
    /* 0x65B */ u8 field_0x65b[0x65D - 0x65B];
    /* 0x65D */ u8 field_0x65d;
    /* 0x65E */ u8 field_0x65e[0x663 - 0x65E];
    /* 0x663 */ u8 field_0x663;
    /* 0x664 */ u8 field_0x664;
    /* 0x665 */ u8 field_0x665[0x668 - 0x665];
    /* 0x668 */ s32 field_0x668; // fn_8040F474 returns it
}; // Size: 0x66C

// Data of game/tu_80401848.cpp.
extern MapScreen_804B1288* lbl_8053ADF8;

// 0x8040F474 (game/tu_80403D84.cpp): lbl_8053ADF8->field_0x668.
s32 fn_8040F474();

// Out-of-line copy of JUtility::TColor::operator= (0x8041499C).
void fn_8041499C(JUtility::TColor* dst, const JUtility::TColor* src);
// 0x804149C0 (game/tu_80403D84.cpp): bit 0 of the current save row's byte 0x87.
bool fn_804149C0();

#endif
