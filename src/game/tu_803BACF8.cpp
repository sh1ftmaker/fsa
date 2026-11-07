#include "game/GameAudio.h"
#include "game/tu_803B8C74.h"
#include "game/GameState.h"
#include "dolphin/card.h"

// The memory card save file's images: fn_803BAE68 / fn_803BAEF4 read them with
// CARDRead, fn_803BBF08 builds and writes them with CARDWrite (0x2000-byte
// card sectors; card DMA buffers are 32-byte aligned).
//
// Not written yet: fn_803BAEF4 and fn_803BBF08. The file holds the unit's .bss objects, in the original's
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

// Reads the save image at card offset 0x2000, or at 0x4000 when that fails, into
// lbl_8051E2C0; on success keeps its bytes 3 and 4 in the game state. Returns the card result.
// FIXME(shotgun): see report
s32 fn_803BAE68(CARDFileInfo* file) {
    u8* image = lbl_8051E2C0;
    if (CARDRead(file, image, 0x2000, 0x2000) != 0) {
        s32 result = CARDRead(file, image, 0x2000, 0x4000);
        if (result != 0) {
            return result;
        }
    }
    lbl_80529DEC.field_0x244 = image[3];
    lbl_80529DEC.field_0x246[0] = image[4];
    return 0;
}

// True when the checksum words of the save file match its first 0xC0 bytes.
// FIXME(shotgun): 43.6 %, missing/extra (our unrolled checksum loop needs more registers)
bool fn_803BADAC(SaveFile_803B950C* file) {
    u32 sum = 0;
    u32 inverse = 0;
    const u8* bytes = (const u8*)file; // byte view for the checksum
    for (int i = 0; i < 0xC0; i++) {
        sum += bytes[i];
        inverse += ~bytes[i];
    }
    return sum == file->field_0xc0 && inverse == file->field_0xc4;
}

// Writes the checksum words of files[file].
// FIXME(shotgun): 53.8 %, missing/extra (as fn_803BADAC)
void fn_803BACF8(SaveFile_803B950C* files, u8 file) {
    SaveFile_803B950C* f = &files[file];
    u32 sum = 0;
    u32 inverse = 0;
    const u8* bytes = (const u8*)f; // byte view for the checksum
    for (int i = 0; i < 0xC0; i++) {
        sum += bytes[i];
        inverse += ~bytes[i];
    }
    f->field_0xc4 = inverse;
    f->field_0xc0 = sum;
}
