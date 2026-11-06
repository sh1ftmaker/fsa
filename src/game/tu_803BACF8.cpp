#include "game/GameAudio.h"
#include "game/tu_803B8C74.h"

// The memory card save file's images: fn_803BAE68 / fn_803BAEF4 read them with
// CARDRead, fn_803BBF08 builds and writes them with CARDWrite (0x2000-byte
// card sectors; card DMA buffers are 32-byte aligned).
//
// Not written yet: every function of the unit (fn_803BACF8, fn_803BADAC, fn_803BAE68,
// fn_803BAEF4, fn_803BBF08). The file holds the unit's .bss objects, in the original's
// order, and the JAudio pool __sinit (game/GameAudio.h).

// The two save images (card offsets 0x2000 and 0x4000; fn_803BAE68 reads the second
// when the first fails). fn_803BBF08 copies the save data to +0xA and the bodies
// (lbl_805242C0 / lbl_80526020) to +0x262 of each.
u8 lbl_8051E2C0[0x2000] ATTRIBUTE_ALIGN(32);
u8 lbl_805202C0[0x2000] ATTRIBUTE_ALIGN(32);
// The card file's first sector (card offset 0): fn_803BBF08 copies two resources that
// fn_804030C4 returns to +0x0 and +0xE00 and writes it.
u8 lbl_805222C0[0x2000] ATTRIBUTE_ALIGN(32);
// The three save-file bodies of each image (memset / memcpy of 0x1D4C = 3 * 0x9C4).
SaveFileBody_803B950C lbl_805242C0[3] ATTRIBUTE_ALIGN(32);
SaveFileBody_803B950C lbl_80526020[3] ATTRIBUTE_ALIGN(32);
