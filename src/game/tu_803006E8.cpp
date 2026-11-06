#include "game/GameAudio.h"
#include "dolphin/os.h"

// The GBA client threads: fn_803007D0 loads "client_thread.bin" (the GBA program
// sent with GBAJoyBoot) and starts one thread per controller port, each running
// fn_80300E1C on its client's message queue.
//
// Not written yet: every function of the unit (fn_803006E8, fn_803007A0, fn_803007D0,
// fn_80300A8C, fn_80300E1C, fn_80301A88). The file holds the unit's .bss objects, in
// the original's order, and the JAudio pool __sinit (game/GameAudio.h).

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
    /* 0x54 */ u8 field_0x54[0xD0 - 0x54];
    /* 0xD0 */ s8 mPort;      // the GBA channel of every GBA call
    /* 0xD1 */ u8 field_0xd1[0xD4 - 0xD1];
    /* 0xD4 */ u8 mStatus;    // GBAGetStatus / GBAWrite status byte
    /* 0xD5 */ u8 field_0xd5[0xE0 - 0xD5];
}; // Size: 0xE0

GbaPacket_80300A8C lbl_804D6AB8[4];
// The client threads and their 0x1000-byte stacks (OSCreateThread in fn_803007D0:
// stack top lbl_804D7798[i] + 0x1000, priority 8).
OSThread lbl_804D6B38[4];
u8 lbl_804D7798[4][0x1000];
GbaClient_803007D0 lbl_804DB798[4];
