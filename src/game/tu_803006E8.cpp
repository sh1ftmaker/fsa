#include "game/GameAudio.h"
#include "dolphin/os.h"
#include "game/GbaClient.h"
#include "JSystem/JKernel/JKRArchive.h"

// The GBA client threads: fn_803007D0 loads "client_thread.bin" (the GBA program
// sent with GBAJoyBoot) and starts one thread per controller port, each running
// fn_80300E1C on its client's message queue.
//
// Not written yet: fn_803007D0, fn_80300A8C, fn_80300E1C (thread setup, checksum and the client
// thread body) and fn_80301A88 (OSResumeThread of the thread at +0x28 of an unidentified object). The file holds the unit's .bss objects, in
// the original's order, and the JAudio pool __sinit (game/GameAudio.h).

GbaPacket_80300A8C lbl_804D6AB8[4];
// The client threads and their 0x1000-byte stacks (OSCreateThread in fn_803007D0:
// stack top lbl_804D7798[i] + 0x1000, priority 8).
OSThread lbl_804D6B38[4];
u8 lbl_804D7798[4][0x1000];
GbaClient_803007D0 lbl_804DB798[4];

// The archive the client program is loaded from (lbl_80529DEC + 0x58); fn_803007D0 sets it.
JKRArchive* lbl_8053ACB8;

// FIXME(shotgun): 100 %
void fn_803007A0() {
    lbl_8053ACB8->unmount();
}

// The 8-bit check value of a 16-bit word (a CRC over two bytes, then eight clock-out steps).
// FIXME(shotgun): 40.7 %, missing/extra: the eight clock-out steps are unrolled by our build, the original keeps a loop
u8 fn_803006E8(u32 data) {
    u32 crc = 0;
    for (u32 mask = 0x80; mask != 0; mask >>= 1) {
        crc <<= 1;
        if (data & mask) {
            if (crc & 0x100) {
                crc ^= 0xCC;
            } else {
                crc += 1;
            }
        } else if (crc & 0x100) {
            crc ^= 0xCD;
        }
    }
    data >>= 8;
    for (u32 mask = 0x80; mask != 0; mask >>= 1) {
        crc <<= 1;
        if (data & mask) {
            if (crc & 0x100) {
                crc ^= 0xCC;
            } else {
                crc += 1;
            }
        } else if (crc & 0x100) {
            crc ^= 0xCD;
        }
    }
    for (int i = 0; i < 8; i++) {
        crc <<= 1;
        if (crc & 0x100) {
            crc ^= 0xCD;
        }
    }
    return crc;
}
