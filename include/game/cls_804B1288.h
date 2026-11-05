#ifndef GAME_CLS_804B1288_H
#define GAME_CLS_804B1288_H

#include "types.h"
#include "JSystem/JUtility/TColor.h"

class J2DScreen;
class cls_804AF86C;

// Vtable 0x804B1288 (one slot, the destructor), 0x66C bytes, defined in
// game/tu_80403D84.cpp. The constructor fn_804122F4 stores
// `this` in lbl_8053ADF8; tu_8039003C allocates it with __nw__(0x66C) when
// lbl_8053AAF8->field_0x7f is set. Its methods are the handlers of a state
// machine (field_0x350).
class cls_804B1288 {
public:
    cls_804B1288(); // fn_804122F4
    /* 0x08 */ virtual ~cls_804B1288();

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
    // 0x804045F8 (this unused): when lbl_8053AAF8's byte 0x7E is set, whether
    // bit `bit` of byte 0x7A of the current save row is set.
    bool fn_804045F8(s32 bit);
    // 0x8040E278: counts field_0x384 up; past 100 the state becomes 0x15.
    void fn_8040E278();
    // 0x8040E700: unless 0x65A is set, clears 0x664 and the state becomes 8.
    void fn_8040E700();
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
    // 0x8040F480: stores 6 in field_0x668, then sets the object up.
    void fn_8040F480();
    void fn_804108B4(); // 0x804108B4
    // 0x804111C4: draws the screen.
    void fn_804111C4();
    void fn_80411230(); // 0x80411230


    /* 0x004 */ u8 field_0x4[0x18 - 0x4];
    /* 0x018 */ J2DScreen* field_0x18; // the screen fn_804111C4 draws
    /* 0x01C */ u8 field_0x1c[0x11C - 0x1C];
    /* 0x11C */ cls_804AF86C* field_0x11c;
    /* 0x120 */ u8 field_0x120[0x228 - 0x120];
    /* 0x228 */ cls_804AF86C* field_0x228[2][2];
    /* 0x238 */ cls_804AF86C* field_0x238;
    /* 0x23C */ cls_804AF86C* field_0x23c;
    /* 0x240 */ cls_804AF86C* field_0x240[12]; // panes the state handlers animate together
    /* 0x270 */ cls_804AF86C* field_0x270[2]; // text panes
    /* 0x278 */ cls_804AF86C* field_0x278[15]; // three groups of five panes
    /* 0x2B4 */ u8 field_0x2b4[0x310 - 0x2B4];
    /* 0x310 */ JUtility::TColor field_0x310;
    /* 0x314 */ u8 field_0x314[0x350 - 0x314];
    /* 0x350 */ s32 field_0x350; // state
    /* 0x354 */ u8 field_0x354[0x358 - 0x354];
    /* 0x358 */ s32 field_0x358;
    /* 0x35C */ u8 field_0x35c[0x384 - 0x35C];
    /* 0x384 */ s32 field_0x384; // frame counter of state 0x15
    /* 0x388 */ u8 field_0x388[0x390 - 0x388];
    /* 0x390 */ s32 field_0x390; // message id
    /* 0x394 */ u8 field_0x394[0x39C - 0x394];
    /* 0x39C */ f32 field_0x39c;
    /* 0x3A0 */ u8 field_0x3a0[0x42A - 0x3A0];
    /* 0x42A */ s16 field_0x42a;
    /* 0x42C */ u8 field_0x42c[0x5DC - 0x42C];
    /* 0x5DC */ char field_0x5dc[1]; // text buffer, length not known
    /* 0x5DD */ u8 field_0x5dd[0x659 - 0x5DD];
    /* 0x659 */ u8 field_0x659;
    /* 0x65A */ u8 field_0x65a;
    /* 0x65B */ u8 field_0x65b[0x65D - 0x65B];
    /* 0x65D */ u8 field_0x65d;
    /* 0x65E */ u8 field_0x65e[0x664 - 0x65E];
    /* 0x664 */ u8 field_0x664;
    /* 0x665 */ u8 field_0x665[0x668 - 0x665];
    /* 0x668 */ s32 field_0x668; // fn_8040F474 returns it
}; // Size: 0x66C

// Data of game/tu_80401848.cpp.
extern cls_804B1288* lbl_8053ADF8;

// 0x8040F474 (game/tu_80403D84.cpp): lbl_8053ADF8->field_0x668.
s32 fn_8040F474();

// Out-of-line copy of JUtility::TColor::operator= (0x8041499C).
void fn_8041499C(JUtility::TColor* dst, const JUtility::TColor* src);
// 0x804149C0 (game/tu_80403D84.cpp): bit 0 of the current save row's byte 0x87.
bool fn_804149C0();

#endif
