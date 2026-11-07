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
    // 0x80408114 (called by fn_804108B4): in states 0xB-0xD fades the eight
    // Pwarea panes of field_0xc: the selected stage's blinks between 150 and
    // 255, the previous one fades out, the others are hidden (alpha 0).
    void fn_80408114();
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
    // 0x80405D00 / 0x80405B10 (called by fn_80409FDC with the stick direction): with
    // more than one sub-stage of field_0x340 open (bits of field_0x38c), move field_0x348
    // (D00, through lbl_80543880 / lbl_80543888 / lbl_8048C90C / lbl_8048C924) or
    // field_0x34c (B10, lbl_8048C93C / lbl_8048C94C, then field_0x348 from
    // lbl_8048C88C) one step when dir is the table's direction.
    void fn_80405D00(s32 dir);
    void fn_80405B10(s32 dir);
    // Dead-stripped (fn_80405D00, fn_80405B10 and fn_8040B388 inline it): resets the
    // field_0x120 panes if field_0x65e is set.
    void hideSubStagePanes();
    // 0x80405FC0 (called by fn_80411230): draws the four players (player_sob.bin)
    // around the cursor field_0x394 / field_0x398, the frame picked by the state, and
    // starts the emitters 0xCB / 0xCC of field_0x2cc / field_0x2dc at frame 26 of state 0x14.
    void fn_80405FC0();
    // 0x80406634 (called by fn_80411230 with field_0x340 - 1 / true, and field_0x340 /
    // false in state 4): draws the cursor sprite (enemy_both2_sob.bin, frames
    // lbl_8048CB68[stage] + 0-5) at the selected sub-stage's point of lbl_8048C95C
    // (50 higher unless `raised`) and keeps the emitters 0x32C / 0x184 of field_0x308 on it.
    void fn_80406634(s32 stage, bool raised);
    // 0x804069E8: draws a marker (enemy_gc_sob.bin, frame 0x25A, 0x25B for the one
    // fn_80404074 picks) on each sub-stage point of the stage (lbl_80529DEC's 0x120
    // for stage 10 / sub-stage 2) that fn_80404074 lets through; at field_0x35c 10
    // the picked one starts the emitters 0x7B-0x7D of field_0x2ec.
    void fn_804069E8();
    // 0x80406F68 (called by fn_80411230 with field_0x3ac / field_0x3b0): draws three
    // enemy_both2_sob.bin sprites at (x, y) on the 256 x 192 layer: frame 0xC6 bobbing
    // with a sine of field_0x380, and lbl_8048CB58[(field_0x380 % 40) / 10] + 0xC0 /
    // + 0xC3 16 to the left and right of it.
    void fn_80406F68(f32 x, f32 y);
    // 0x804083D8: highlights row field_0x340 of field_0x28 (its picture scaled back by
    // the 72 x 36 size, the second resized to 72 x 36, the text box moved with it, the
    // colours field_0x328[3-5] / field_0x310[3-5]); the other rows get their own size,
    // place and colours [0-2]; then hangs the rows under 'Ndata01', the selected one last.
    void fn_804083D8();
    // 0x80409014: every 15 frames swaps which pane of each field_0x228 pair is shown;
    // cross-fades field_0x23c's picture (setBlendRatio) over a 180-frame cycle and turns
    // it 90 / 270 degrees about its centre.
    void fn_80409014();
    // Inline (fn_804096E8 twice, fn_8040C950, fn_8040CE90): the name of stage
    // field_0x340 into field_0x438: message lbl_8048C1F8[0][stage] for stages 0-7, the
    // debug names "Test-1", "ECTS", "E3", "Boss1-4", "Boss5-8" for 8-12, else "".
    void setStageName();


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
    /* 0x344 */ s32 field_0x344; // the stage selected before field_0x340 (fn_80408114)
    /* 0x348 */ s32 field_0x348; // sub-stage of field_0x340 (0-3; fn_80409450)
    /* 0x34C */ s32 field_0x34c; // row of field_0x348 in lbl_8048C88C (fn_80405B10)
    /* 0x350 */ s32 field_0x350; // state
    /* 0x354 */ u8 field_0x354[0x358 - 0x354];
    /* 0x358 */ s32 field_0x358;
    /* 0x35C */ s32 field_0x35c; // frame counter of fn_8040E720
    /* 0x360 */ s32 field_0x360; // facing of the cursor (0, 8, 0x10, 0x18; fn_8040E3A0)
    /* 0x364 */ s32 field_0x364; // first stage whose bit fn_80404D70 finds clear
    /* 0x368 */ u8 field_0x368[0x370 - 0x368];
    /* 0x370 */ s32 field_0x370; // stages fn_80404D70 counts as open
    /* 0x374 */ u8 field_0x374[0x378 - 0x374];
    /* 0x378 */ s32 field_0x378;
    /* 0x37C */ s32 field_0x37c;
    /* 0x380 */ s32 field_0x380; // frame counter of fn_8040B5A0
    /* 0x384 */ s32 field_0x384; // frame counter of state 0x15
    /* 0x388 */ s32 field_0x388; // -1, 0 or 1 (fn_80404074)
    /* 0x38C */ u32 field_0x38c; // flags; bit 0x10 lets fn_80404648 / fn_804049DC open the stage view
    /* 0x390 */ s32 field_0x390; // message id
    /* 0x394 */ f32 field_0x394; // cursor x (fn_8040E3A0 moves it to lbl_8048C95C)
    /* 0x398 */ f32 field_0x398; // cursor y
    /* 0x39C */ f32 field_0x39c;
    /* 0x3A0 */ f32 field_0x3a0;
    /* 0x3A4 */ f32 field_0x3a4;
    /* 0x3A8 */ u8 field_0x3a8[0x3AC - 0x3A8];
    /* 0x3AC */ f32 field_0x3ac; // x of the stage-name pane's centre (fn_8040B5A0)
    /* 0x3B0 */ f32 field_0x3b0; // y of it
    /* 0x3B4 */ u8 field_0x3b4[0x3B8 - 0x3B4];
    /* 0x3B8 */ f32 field_0x3b8; // y of field_0x208[field_0x340] (fn_8040B5A0)
    /* 0x3BC */ u8 field_0x3bc[0x400 - 0x3BC];
    /* 0x400 */ f32 field_0x400; // alpha (0-1) of the cursor sprite (fn_80406634)
    /* 0x404 */ u8 field_0x404[0x40E - 0x404];
    /* 0x40E */ s16 field_0x40e[8]; // per-stage blink counters of the Pwarea panes (fn_80408114)
    /* 0x41E */ s16 field_0x41e;
    /* 0x420 */ s16 field_0x420; // 0-179, the blend / turn cycle of field_0x23c (fn_80409014)
    /* 0x422 */ s16 field_0x422; // blink counter of field_0x228 (fn_80409014)
    /* 0x424 */ u8 field_0x424[0x426 - 0x424];
    /* 0x426 */ s16 field_0x426; // frame counter of fn_8040C950 / fn_8040CE90
    /* 0x428 */ s16 field_0x428; // step of fn_8040B5A0 (0: drop in, 1: rise out)
    /* 0x42A */ s16 field_0x42a;
    /* 0x42C */ s16 field_0x42c; // frame counter of fn_8040E298
    /* 0x42E */ s16 field_0x42e; // 30 after fn_804083D8
    /* 0x430 */ s16 field_0x430; // frame of field_0x2b4's slide
    /* 0x432 */ s16 field_0x432; // frame counter of the cursor sprite (0-5999, fn_80406634)
    /* 0x434 */ s16 field_0x434;
    /* 0x436 */ s16 field_0x436;
    // The stage name (getMessageText_803832BC or a debug name) fn_8040C950 copies into
    // Ttlv11 / Ttlv12; its length is not proven (it runs to field_0x5dc at most).
    /* 0x438 */ char field_0x438[0x5DC - 0x438];
    /* 0x5DC */ char field_0x5dc[1]; // text buffer, length not known
    /* 0x5DD */ u8 field_0x5dd[0x640 - 0x5DD];
    /* 0x640 */ u8 field_0x640; // set by fn_804096E8 (stage 7)
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
    /* 0x65E */ u8 field_0x65e; // 1 while the field_0x120 panes are shown (hideSubStagePanes)
    /* 0x65F */ u8 field_0x65f[0x662 - 0x65F];
    /* 0x662 */ u8 field_0x662;
    /* 0x663 */ u8 field_0x663;
    /* 0x664 */ u8 field_0x664;
    /* 0x665 */ u8 field_0x665; // alpha of the player sprites in state 4 (fn_80405FC0)
    /* 0x666 */ u8 field_0x666[0x668 - 0x666];
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
