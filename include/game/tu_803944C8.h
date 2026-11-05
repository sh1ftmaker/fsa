#ifndef GAME_TU_803944C8_H
#define GAME_TU_803944C8_H

// The JMessage processors of the message windows (game/tu_803944C8.cpp), named
// after the vtables their destructors store (0x804AF6D4 .. 0x804AF774). The
// window object cls_804AF5A0 (game/Message.h) owns them; only the members the
// unit's functions touch are declared.

#include "game/Message.h"
#include "JSystem/JMessage/TProcessor.h"

class cls_804AF5A0;

// Vtable 0x804AF6D4: a JMessage::TReference with do_word left to the base.
class cls_804AF6D4 : public JMessage::TReference {
public:
    virtual ~cls_804AF6D4();
};

// Vtable 0x804AF774: the sequence processor of a window, slot 0x3C of the
// object (its 0x60 points back at the window).
class cls_804AF774 : public JMessage::TSequenceProcessor {
public:
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

    /* 0x4C */ s32 field_0x4c;
    /* 0x50 */ s32 field_0x50;
    /* 0x54 */ s32 field_0x54;
    /* 0x58 */ s32 field_0x58;
    /* 0x5C */ u8 field_0x5c;
    /* 0x5D */ u8 field_0x5d;
    /* 0x5E */ u8 field_0x5e[0x60 - 0x5E];
    /* 0x60 */ cls_804AF5A0* field_0x60;
};

// Vtable 0x804AF6E4: a rendering processor (text drawing).
class cls_804AF6E4 : public JMessage::TRenderingProcessor {
public:
    virtual ~cls_804AF6E4();
    virtual void do_end();

    /* 0x38 */ u8 field_0x38[0x202 - 0x38];
    /* 0x202 */ u8 field_0x202[0x22C - 0x202];
    /* 0x22C */ u8 field_0x22c;
    /* 0x22D */ u8 field_0x22d[0x257 - 0x22D];
    /* 0x257 */ u8 field_0x257[0x281 - 0x257];
    /* 0x281 */ u8 field_0x281;
};

// Vtable 0x804AF72C: the other rendering processor (the window's text layout).
class cls_804AF72C : public JMessage::TRenderingProcessor {
public:
    virtual ~cls_804AF72C();
    virtual void do_reset();
    virtual void do_end();

    /* 0x38 */ u8 field_0x38[0x2C0 - 0x38];
    /* 0x2C0 */ s8* field_0x2c0;
    /* 0x2C4 */ s8* field_0x2c4;
    /* 0x2C8 */ u8 field_0x2c8[0x2D0 - 0x2C8];
    /* 0x2D0 */ s32 field_0x2d0;
    /* 0x2D4 */ u8 field_0x2d4[0x2D8 - 0x2D4];
    /* 0x2D8 */ cls_804AF5A0* field_0x2d8;
};

#endif
