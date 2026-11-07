#ifndef GAME_TU_803944C8_H
#define GAME_TU_803944C8_H

// The JMessage processors of the message windows (game/tu_803944C8.cpp), named
// after the vtables their destructors store (0x804AF6D4 .. 0x804AF774). The
// window object cls_804AF5A0 (game/Message.h) owns them; only the members the
// unit's functions touch are declared.

#include <string.h>
#include "game/Message.h"
#include "JSystem/JMessage/TProcessor.h"
#include "JSystem/JUtility/JUTFont.h"

class cls_804AF5A0;

// Vtable 0x804AF6D4: a JMessage::TReference with do_word left to the base.
class cls_804AF6D4 : public JMessage::TReference {
public:
    // Inlined by cls_804AF5A0's constructor (no out-of-line copy).
    cls_804AF6D4() {}
    virtual ~cls_804AF6D4();
};

// Vtable 0x804AF774: the sequence processor of a window, slot 0x3C of the
// object (its 0x60 points back at the window).
class cls_804AF774 : public JMessage::TSequenceProcessor {
public:
    // Inlined by cls_804AF5A0's constructor (no out-of-line copy).
    cls_804AF774(const JMessage::TReference* reference, JMessage::TControl* control)
        : JMessage::TSequenceProcessor(reference, control) {
        field_0x5c = 1;
    }
    virtual ~cls_804AF774();
    virtual void do_reset();
    virtual void do_begin(const void*, const char*);
    virtual void do_end();
    virtual void do_character(int);
    virtual bool do_tag(u32, const void*, u32);
    virtual bool do_isReady();
    virtual bool do_jump_isReady();
    virtual void do_jump(const void*, const char*);

    // 0x803A2F74: resets the status fields (called by tu_80390818's update).
    void fn_803A2F74();
    // 0x803A2E48 / 0x803A2CB0: called by fn_8039FE78 to count a number's
    // characters and the noun after it into field_0x50.
    void fn_803A2E48(s32 value);
    void fn_803A2CB0(u8 noun, s32 count);

    /* 0x4C */ s32 field_0x4c;
    /* 0x50 */ s32 field_0x50;
    /* 0x54 */ s32 field_0x54;
    /* 0x58 */ s32 field_0x58;
    /* 0x5C */ u8 field_0x5c;
    /* 0x5D */ u8 field_0x5d;
    /* 0x5E */ u8 field_0x5e[0x60 - 0x5E];
    /* 0x60 */ cls_804AF5A0* field_0x60;
};

// Vtable 0x804AF6E4: a rendering processor (text drawing). cls_804AF72C holds
// one at 0x38 (its destructor destroys it there). The 42-entry arrays are
// per-line entries indexed by field_0x201 / field_0x281 (do_begin, do_end).
class cls_804AF6E4 : public JMessage::TRenderingProcessor {
public:
    // Inlined by cls_804AF5A0's constructor through cls_804AF72C's.
    cls_804AF6E4(const JMessage::TReference* reference) : JMessage::TRenderingProcessor(reference) {
        field_0x1d8 = 0;
        field_0x1dc = 0;
        field_0x1e8 = 0;
    }
    virtual ~cls_804AF6E4();
    virtual void do_begin(const void*, const char*);
    virtual void do_end();
    virtual void do_character(int);
    virtual bool do_tag(u32, const void*, u32);

    // 0x80398700: adds the width of the counted noun after a number (the
    // same nouns as cls_804AF774::fn_803A2CB0; singular when count is 1) to
    // the current line, like do_character. Called by the do_tag (0x803997B8).
    void fn_80398700(u8 noun, s32 count);
    // 0x80398A34: adds the width of a number's characters to the current line (like do_character).
    void fn_80398A34(s32 number);
    // 0x80398EF4: the width of a time (minutes' seconds') added to the current line.
    void fn_80398EF4(s32 minutes, s32 seconds);
    // 0x8039934C: the same, leaving out the minutes when they are 0 (clamped to 99'59'').
    void fn_8039934C(s32 minutes, s32 seconds);
    // Inlined: the seconds' mark is a doubled apostrophe, one character twice as wide.
    void addSecondsMarkWidth(f32 ratio) {
        JUTFont::TWidth width;
        field_0x38->getWidthEntry('\'', &width);
        field_0x4c[field_0x201] += 2.0f * (ratio * (field_0x44 * (s32)width.w1));
        field_0xf4[field_0x201] += 6.0f;
        field_0x22d[field_0x201]++;
    }
    // Inlined: the width of one character, scaled by `ratio`, added to the current line.
    void addCharWidth(f32 ratio, int c) {
        JUTFont::TWidth width;
        field_0x38->getWidthEntry(c, &width);
        field_0x4c[field_0x201] += ratio * (field_0x44 * (s32)width.w1);
        field_0xf4[field_0x201] += 3.0f;
        field_0x22d[field_0x201]++;
    }

    // Inlined by cls_804AF72C::do_begin (its body is under NONMATCHING; the four window getters are read
    // first, last argument first, then the four stores); no out-of-line copy.
    void setFields0x1e0(s16 a, s16 b, s32 c, s32 d) {
        field_0x1ea = a;
        field_0x1ec = b;
        field_0x1e0 = c;
        field_0x1e4 = d;
    }

    // Whether bit `index` (0..63) of the two mask words at 0x3C / 0x40 is set.
    int isMasked(int index) const {
        if (index < 32) {
            return ((u64)(u32)field_0x3c & (u64)(s64)(1 << index)) ? 1 : 0;
        }
        if (index < 64) {
            return ((u64)(u32)field_0x40 & (u64)(s64)(1 << (index - 32))) ? 1 : 0;
        }
        return 0;
    }

    // The font do_character measures with (getCellWidth, getWidthEntry).
    /* 0x038 */ JUTFont* field_0x38;
    /* 0x03C */ s32 field_0x3c;
    /* 0x040 */ s32 field_0x40;
    // fn_803944EC stores the text scale and the window's font width
    // (cls_804AF5A0 field_0x60) here.
    /* 0x044 */ f32 field_0x44;
    /* 0x048 */ f32 field_0x48;
    /* 0x04C */ f32 field_0x4c[42];
    /* 0x0F4 */ f32 field_0xf4[42];
    /* 0x19C */ f32 field_0x19c[15];
    // cls_804AF72C::do_begin: the hours and minutes of the message
    // manager's counter (field_0x44 / 3600, / 60 % 60 or 1), then the
    // window's 0x178 / 0x17C and 0x190 / 0x192.
    /* 0x1D8 */ s32 field_0x1d8;
    /* 0x1DC */ s32 field_0x1dc;
    /* 0x1E0 */ s32 field_0x1e0;
    /* 0x1E4 */ s32 field_0x1e4;
    /* 0x1E8 */ u8 field_0x1e8;
    /* 0x1E9 */ u8 field_0x1e9;
    /* 0x1EA */ s16 field_0x1ea;
    /* 0x1EC */ s16 field_0x1ec;
    /* 0x1EE */ u8 field_0x1ee[15];
    /* 0x1FD */ u8 field_0x1fd;
    /* 0x1FE */ u8 field_0x1fe[3];
    /* 0x201 */ u8 field_0x201;
    /* 0x202 */ s8 field_0x202[42];
    /* 0x22C */ s8 field_0x22c; // a line count (compared signed with field_0x202)
    /* 0x22D */ u8 field_0x22d[42];
    /* 0x257 */ s8 field_0x257[42];
    /* 0x281 */ u8 field_0x281;
    /* 0x282 */ u8 field_0x282;
}; // Size: 0x284

// Vtable 0x804AF72C: the other rendering processor (the window's text layout).
class cls_804AF72C : public JMessage::TRenderingProcessor {
public:
    // Inlined by cls_804AF5A0's constructor (no out-of-line copy).
    cls_804AF72C(const JMessage::TReference* reference)
        : JMessage::TRenderingProcessor(reference), field_0x38(reference) {}
    virtual ~cls_804AF72C();
    // Inlined by fn_803944EC and the window constructor (both load field_0x60
    // before the one load of the processor, then the three stores); no
    // out-of-line copy survives.
    void setScale(f32 scale, f32 width) {
        field_0x2c8 = scale;
        field_0x38.field_0x44 = scale;
        field_0x38.field_0x48 = width;
    }
    virtual void do_reset();
    virtual void do_begin(const void*, const char*);
    virtual void do_end();
    // 0x8039D0DC: records the current slot's offset and line count, then the value.
    void fn_8039D0DC(u8 value);
    // 0x8039D30C: the same for the text buffer, as -2 marked control bytes.
    void fn_8039D30C(u8 value);
    // 0x8039F540: lays one character of the message out in the line buffers (handles the newline,
    // the leading offset, two-byte characters).
    void fn_8039F540(int c);
    // 0x8039CDF8: places a picture (field_0x68 entry) in the first free slot.
    void fn_8039CDF8(u8 value);
    // 0x8039C9A0: writes a number's digits (with a leading '-', and a colour-code offset first).
    void fn_8039C9A0(s32 number);
    // 0x8039C630: appends a counted noun (the nouns of fn_803A2CB0), singular when count is 1.
    void fn_8039C630(u8 noun, s32 count);
    // 0x8039C258: the same number as fn_8039C9A0, as glyph codes in the first line buffer.
    void fn_8039C258(s32 number);
    // Inlined: appends a glyph code (two bytes) to the first line buffer.
    void appendGlyph(u16 glyph) {
        field_0x2c0[field_0x2d0++] = glyph >> 8;
        field_0x2c0[field_0x2d0++] = glyph;
    }
    // Inlined: appends a short text to both line buffers.
    void appendText(const char* text, int length = 1) {
        strcat((char*)field_0x2c0, text);
        strcat((char*)field_0x2c4, text);
        field_0x2d0 += length;
    }

    // Destroyed by the destructor at 0x38 (vtable 0x804AF6E4 stored there).
    /* 0x038 */ cls_804AF6E4 field_0x38;
    // The font of the window's 'Tm01' text box (the constructor), handed to
    // field_0x38 by do_begin.
    /* 0x2BC */ JUTFont* field_0x2bc;
    /* 0x2C0 */ s8* field_0x2c0;
    /* 0x2C4 */ s8* field_0x2c4;
    /* 0x2C8 */ f32 field_0x2c8; // the text scale (fn_803944EC)
    /* 0x2CC */ s32 field_0x2cc;
    /* 0x2D0 */ s32 field_0x2d0;
    /* 0x2D4 */ u8 field_0x2d4;
    /* 0x2D5 */ u8 field_0x2d5;
    /* 0x2D6 */ u8 field_0x2d6;
    /* 0x2D7 */ u8 field_0x2d7;
    /* 0x2D8 */ cls_804AF5A0* field_0x2d8;
};

#endif
