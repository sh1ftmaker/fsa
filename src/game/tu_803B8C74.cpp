#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/StageFlag.h"
#include "game/tu_803B8C74.h"
#include "string.h"

// The unit's .bss, in the original's order (the __sinit at 0x803B961C addresses
// both from 0x8050EC60: the save data at +0x20, cls_804AFB3C at +0x200C, each
// after its destructor-chain object). The save data is 32-byte aligned: its chain
// object at +0x0 is 0xC bytes and it starts at +0x20.
cls_804AFB30 lbl_8050EC80 ATTRIBUTE_ALIGN(32);
cls_804AFB3C lbl_80510C6C;

// lbl_80476548 (.rodata, 8 x 10000: the initial value of cls_804AFB3C's two word
// tables) belongs to its constructor, fn_803B9160, which is not written yet.

// Bit stage * 4 + sub of save file `file`'s flag word (stage 0-7, sub 0-3,
// else false): isStageFlagSet (game/StageFlag.h) for any file. fn_803B8CAC
// inlines it three times, fn_803B90E4 once.
static inline BOOL isFileFlagSet(s32 file, s32 stage, s32 sub) {
    if (stage < 0 || stage > 7) {
        return FALSE;
    }
    if (sub < 0 || sub > 3) {
        return FALSE;
    }
    return ((1 << (stage * 4 + sub)) & lbl_8050EC80.getFile(file).mFlags) ? TRUE : FALSE;
}

// 0x803B95D4
cls_804AFB3C::~cls_804AFB3C() {}

// 0x803B950C
void fn_803B950C(SaveFile_803B950C* file, SaveFileBody_803B950C* body) {
    memset(file, 0, sizeof(SaveFile_803B950C));
    memset(body, 0, sizeof(SaveFileBody_803B950C));
    file->setDefaults();
}

// 0x803B94C4
cls_804AFB30::~cls_804AFB30() {}

// FIXME: 92.4 %. scheduling+regalloc: the two destination pointers land in r30/r31
// swapped against the original (files[index] in r30), and their adds are scheduled
// one slot apart.
// 0x803B93E8
s32 cls_804AFB30::fn_803B93E8(SaveFile_803B950C* files, SaveFileBody_803B950C* bodies, s32 index) {
    SaveFile_803B950C* dstFile = &files[index];
    SaveFileBody_803B950C* dstBody = &bodies[index];
    SaveFile_803B950C file;
    SaveFileBody_803B950C body;
    fn_803B950C(&file, &body);
    memcpy(dstFile, &file, sizeof(SaveFile_803B950C));
    memcpy(dstBody, &body, sizeof(SaveFileBody_803B950C));
    return 0;
}

// 0x803B933C
s32 cls_804AFB30::fn_803B933C(SaveFile_803B950C* files, s32 index) {
    SaveFile_803B950C* dst = &files[index];
    SaveFile_803B950C file;
    memset(&file, 0, sizeof(SaveFile_803B950C));
    file.setDefaults();
    memcpy(dst, &file, sizeof(SaveFile_803B950C));
    return 0;
}

// 0x803B9160 (cls_804AFB3C's constructor) is not written yet: two natural loops
// (16 bytes of both halves, then 8 entries of the other byte arrays and both word
// tables) reach 88.8 %, but ours unrolls the 16-byte loop completely where the
// original keeps a 2-iteration counted loop unrolled by 8; the source shape of
// that loop is not found yet.

// 0x803B9120
bool fn_803B9120(s32 stage) {
    if (stage < 3) {
        return true;
    }
    if (isStageFlagSet(7, 3)) {
        return true;
    }
    return false;
}

// 0x803B90E4
bool fn_803B90E4(s32 file, s32 index) {
    if (index < 3) {
        return true;
    }
    if (isFileFlagSet(file, 7, 3)) {
        return true;
    }
    return false;
}

// 0x803B8EBC
bool fn_803B8EBC(s32 index) {
    if (index == 7) {
        if (isStageFlagSet(index, 0) &&
            isStageFlagSet(index, 2) &&
            isStageFlagSet(index, 3)) {
            return true;
        }
    } else if (isStageFlagSet(index, 0) &&
               isStageFlagSet(index, 1) &&
               isStageFlagSet(index, 3)) {
        return true;
    }
    return false;
}

// 0x803B8CAC
bool fn_803B8CAC(s32 file, s32 index) {
    if (index == 7) {
        if (isFileFlagSet(file, index, 0) && isFileFlagSet(file, index, 2) &&
            isFileFlagSet(file, index, 3)) {
            return true;
        }
    } else if (isFileFlagSet(file, index, 0) && isFileFlagSet(file, index, 1) &&
               isFileFlagSet(file, index, 3)) {
        return true;
    }
    return false;
}

// 0x803B8C90
u8 fn_803B8C90(void) {
    return lbl_8050EC80.getCurrentFile().field_0x66;
}

// 0x803B8C74
void fn_803B8C74(u8 value) {
    lbl_8050EC80.getCurrentFile().field_0x66 = value;
}
