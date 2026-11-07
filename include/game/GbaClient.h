#ifndef GAME_GBACLIENT_H
#define GAME_GBACLIENT_H

#include "dolphin/os.h"

// One 0x20-byte packet per port (lbl_804D6AB8[port], indexed by the client's
// mPort << 5). fn_80300A8C compares the client's copy, mPacket, with it
// (memcmp 0x20) and clears and refills it.
struct GbaPacket_80300A8C {
    /* 0x00 */ u8 field_0x0;
    /* 0x01 */ u8 mPort;       // the client's mPort
    /* 0x02 */ u8 field_0x2;   // the client's mPacket.field_0x2
    /* 0x03 */ u8 field_0x3;
    /* 0x04 */ OSTick field_0x4; // lbl_8053ACC8 (OSGetTick at fn_803007D0)
    /* 0x08 */ OSTick field_0x8; // the client's field_0x48 (OSGetTick)
    /* 0x0C */ u32 field_0xc;
    /* 0x10 */ u8 field_0x10[0x20 - 0x10];
}; // Size: 0x20

// One GBA client per port (lbl_804DB798[4], set up by fn_803007D0: memset 0xE0,
// the queue with its one message slot, the callback fn_80300A8C, the program
// and its length). Named after fn_803007D0, the first function that fills one.
// fn_80304FB0 (tu_80301CD0) and fn_8030A258 (tu_803099FC) read the array by name.
struct GbaClient_803007D0 {
    /* 0x00 */ OSMessageQueue mQueue;
    /* 0x20 */ OSMessage mMessage[1];
    /* 0x24 */ void (*mCallback)(GbaClient_803007D0* client, OSMessage msg); // fn_80300A8C
    /* 0x28 */ GbaPacket_80300A8C mPacket;
    /* 0x48 */ OSTick field_0x48;
    /* 0x4C */ u8* mProgram;  // lbl_8053ACBC, passed to GBAJoyBoot
    /* 0x50 */ s32 mLength;   // lbl_8053ACC0, passed to GBAJoyBoot
    /* 0x54 */ u32 mState;   // tested against 0x40000 (GBA linked) / 0 by cls_804AA8C8::fn_8030A43C
    /* 0x58 */ u8 field_0x58[0x60 - 0x58];
    /* 0x60 */ u32 mKeys;     // the packed key word fn_8030A43C decodes
    /* 0x64 */ u8 field_0x64[0x68 - 0x64];
    /* 0x68 */ u8 mTransfer[0xD0 - 0x68]; // fn_8030A43C copies lbl_80506DE8[port] in
    /* 0xD0 */ s8 mPort;      // the GBA channel of every GBA call
    /* 0xD1 */ u8 field_0xd1;
    /* 0xD2 */ u8 field_0xd2;
    /* 0xD3 */ u8 field_0xd3;
    /* 0xD4 */ u8 mStatus;    // GBAGetStatus / GBAWrite status byte
    /* 0xD5 */ u8 field_0xd5;
    /* 0xD6 */ u8 field_0xd6;
    /* 0xD7 */ u8 field_0xd7;
    /* 0xD8 */ u8 field_0xd8;
    /* 0xD9 */ u8 field_0xd9[0xDC - 0xD9];
    /* 0xDC */ u8 field_0xdc; // tested by cls_804AA8C8::fn_8030A258 (game/tu_803099FC.cpp)
    /* 0xDD */ u8 field_0xdd;
    /* 0xDE */ u8 field_0xde;
    /* 0xDF */ u8 field_0xdf;
}; // Size: 0xE0

extern GbaClient_803007D0 lbl_804DB798[4];
// The unit's .sdata2 table (0x80541618): transfer lengths in words, indexed by lbl_8053AD08;
// cls_804AA8C8::fn_8030A43C copies that many words into each client's mTransfer.
extern const u8 lbl_80541618[4];
// 0x803006E8 (not written yet): an 8-bit CRC (polynomial 0xCD) over the two low bytes of its
// argument, low byte first; cls_804AA8C8::fn_8030A43C checks a GBA key word's low byte against it.
u8 fn_803006E8(u32 keys);

#endif
