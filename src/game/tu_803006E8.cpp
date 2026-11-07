#include "game/GameAudio.h"
#include "dolphin/os.h"
#include "game/GbaClient.h"

// The GBA client threads: fn_803007D0 loads "client_thread.bin" (the GBA program
// sent with GBAJoyBoot) and starts one thread per controller port, each running
// fn_80300E1C on its client's message queue.
//
// Not written yet: every function of the unit (fn_803006E8, fn_803007A0, fn_803007D0,
// fn_80300A8C, fn_80300E1C, fn_80301A88). The file holds the unit's .bss objects, in
// the original's order, and the JAudio pool __sinit (game/GameAudio.h).

GbaPacket_80300A8C lbl_804D6AB8[4];
// The client threads and their 0x1000-byte stacks (OSCreateThread in fn_803007D0:
// stack top lbl_804D7798[i] + 0x1000, priority 8).
OSThread lbl_804D6B38[4];
u8 lbl_804D7798[4][0x1000];
GbaClient_803007D0 lbl_804DB798[4];
