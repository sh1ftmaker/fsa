#include "game/GameAudio.h"
#include "game/tu_803B8C74.h"
#include "game/GameState.h"
#include "game/GameGlobals.h"
#include "JSystem/ResTIMG.h"
#include "dolphin/card.h"
#include "dolphin/os.h"
#include "stdio.h"
#include "string.h"

// The memory card save file's images: fn_803BAE68 / fn_803BAEF4 read them with
// CARDRead, fn_803BBF08 builds and writes them with CARDWrite (0x2000-byte
// card sectors; card DMA buffers are 32-byte aligned).

struct SaveSector_803BACF8 {
    /* 0x0000 */ u16 mHeader;
    /* 0x0002 */ u8 field_0x2;
    /* 0x0003 */ u8 field_0x3;
    /* 0x0004 */ u8 field_0x4;
    /* 0x0005 */ u8 pad_0x5[5];
    /* 0x000A */ u8 mData[0x258];
    /* 0x0262 */ u8 mBodies[0x1D4C];
    /* 0x1FAE */ u8 pad_0x1FAE[0x4E];
    /* 0x1FFC */ u32 mChecksum;

    u32 calcChecksum() const {
        const u16* halfwords = (const u16*)this;
        u32 sum = 0;
        u16 inv = 0;
        for (int i = 0; i < 0xFFE; i++) {
            u16 val = *halfwords++;
            sum += val;
            inv += ~val;
        }
        return (sum << 16) | inv;
    }
};

// The two save images (card offsets 0x2000 and 0x4000; fn_803BAE68 reads the second
// when the first fails). fn_803BBF08 copies the save data to +0xA and the bodies
// (lbl_805242C0 / lbl_80526020) to +0x262 of each.
SaveSector_803BACF8 lbl_8051E2C0 ATTRIBUTE_ALIGN(32);
SaveSector_803BACF8 lbl_805202C0 ATTRIBUTE_ALIGN(32);
// The card file's first sector (card offset 0): fn_803BBF08 copies two resources that
// fn_804030C4 returns to +0x0 and +0xE00 and writes it.
u8 lbl_805222C0[0x2000] ATTRIBUTE_ALIGN(32);
// The three save-file bodies of each image (memset / memcpy of 0x1D4C = 3 * 0x9C4).
SaveFileBody_803B950C lbl_805242C0[3] ATTRIBUTE_ALIGN(32);
SaveFileBody_803B950C lbl_80526020[3] ATTRIBUTE_ALIGN(32);

static inline bool isCardStatusValid(CARDFileInfo* file) {
    CARDStat stat;
    CARDGetStatus(lbl_8051C1A0.field_0x1ff4, file->fileNo, &stat);
    return stat.iconAddr == 0 && stat.commentAddr == 0x1C00 &&
           CARDGetBannerFormat(&stat) == CARD_STAT_BANNER_C8 &&
           CARDGetIconAnim(&stat) == CARD_STAT_ANIM_LOOP &&
           CARDGetIconFormat(&stat, 0) == CARD_STAT_ICON_C8 &&
           CARDGetIconFormat(&stat, 1) == CARD_STAT_ICON_NONE &&
           CARDGetIconFormat(&stat, 2) == CARD_STAT_ICON_NONE &&
           CARDGetIconFormat(&stat, 3) == CARD_STAT_ICON_NONE &&
           CARDGetIconFormat(&stat, 4) == CARD_STAT_ICON_NONE &&
           CARDGetIconFormat(&stat, 5) == CARD_STAT_ICON_NONE &&
           CARDGetIconFormat(&stat, 6) == CARD_STAT_ICON_NONE &&
           CARDGetIconFormat(&stat, 7) == CARD_STAT_ICON_NONE &&
           CARDGetIconSpeed(&stat, 0) == CARD_STAT_SPEED_SLOW &&
           CARDGetIconSpeed(&stat, 1) == CARD_STAT_SPEED_END &&
           CARDGetIconSpeed(&stat, 2) == CARD_STAT_SPEED_END &&
           CARDGetIconSpeed(&stat, 3) == CARD_STAT_SPEED_END &&
           CARDGetIconSpeed(&stat, 4) == CARD_STAT_SPEED_END &&
           CARDGetIconSpeed(&stat, 5) == CARD_STAT_SPEED_END &&
           CARDGetIconSpeed(&stat, 6) == CARD_STAT_SPEED_END &&
           CARDGetIconSpeed(&stat, 7) == CARD_STAT_SPEED_END;
}

// FIXME: 40.10 %, missing/extra; behaviour drift.
s32 fn_803BBF08(CARDFileInfo* file, void* src, void* bodySrc, u32 size, u32 bodySize) {
    snprintf((char*)(lbl_805222C0 + 0x1C00), 0x20, "Four Swords Adventures");
    OSCalendarTime cal;
    OSTicksToCalendarTime(OSGetTime(), &cal);
    snprintf((char*)(lbl_805222C0 + 0x1C20), 0x20, "%d/%d Save Data", cal.mon + 1, cal.mday);

    const ResTIMG* banner = (const ResTIMG*)fn_804030C4('TIMG', "kk_diskbanner_01.bti");
    const ResTIMG* icon = (const ResTIMG*)fn_804030C4('TIMG', "kk_ipl_icon_01.bti");
    memcpy(lbl_805222C0, (const u8*)banner + banner->mImageDataOffset, (banner->mPaletteEntryCount << 1) + 0xC00);
    memcpy(lbl_805222C0 + 0xE00, (const u8*)icon + icon->mImageDataOffset, (icon->mPaletteEntryCount << 1) + 0x400);

    s32 res = CARDWrite(file, lbl_805222C0, 0x2000, 0);
    if (res != 0) {
        return res;
    }

    if (!isCardStatusValid(file)) {
        memset(&lbl_8051E2C0, 0, 0x2000);
        res = CARDWrite(file, &lbl_8051E2C0, 0x2000, 0x2000);
        if (res != 0) {
            return res;
        }
        res = CARDWrite(file, &lbl_8051E2C0, 0x2000, 0x4000);
        if (res != 0) {
            return res;
        }
        memset(lbl_805242C0, 0, 0x1D4C);
        memset(lbl_80526020, 0, 0x1D4C);
    }

    SaveSector_803BACF8* img1 = &lbl_8051E2C0;
    SaveSector_803BACF8* img2 = &lbl_805202C0;
    img1->mHeader = 0;
    img2->mHeader = 0;
    memcpy(img1->mData, src, size);
    memcpy(img2->mData, src, size);
    memcpy(img1->mBodies, lbl_805242C0, 0x1D4C);
    memcpy(img2->mBodies, lbl_80526020, 0x1D4C);
    img2->field_0x2 = lbl_80529DEC.field_0x245;
    img1->field_0x2 = lbl_80529DEC.field_0x245;
    img2->field_0x3 = lbl_80529DEC.field_0x244;
    img1->field_0x3 = lbl_80529DEC.field_0x244;
    img2->field_0x4 = lbl_80529DEC.field_0x246;
    img1->field_0x4 = lbl_80529DEC.field_0x246;

    u32 chk1 = img1->calcChecksum();
    img1->mChecksum = chk1;
    u32 chk2 = img2->calcChecksum();
    img2->mChecksum = chk2;

    res = CARDWrite(file, &lbl_8051E2C0, 0x2000, 0x2000);
    if (res != 0) {
        return res;
    }
    res = CARDRead(file, &lbl_8051E2C0, 0x2000, 0x2000);
    if (res != 0) {
        return res;
    }
    if (chk1 != lbl_8051E2C0.calcChecksum()) {
        return res;
    }

    res = CARDWrite(file, &lbl_805202C0, 0x2000, 0x4000);
    if (res != 0) {
        return res;
    }
    res = CARDRead(file, &lbl_805202C0, 0x2000, 0x4000);
    if (res != 0) {
        return res;
    }
    if (chk2 != lbl_805202C0.calcChecksum()) {
        return res;
    }

    CARDStat stat;
    CARDGetStatus(lbl_8051C1A0.field_0x1ff4, file->fileNo, &stat);
    CARDSetBannerFormat(&stat, CARD_STAT_BANNER_C8);
    CARDSetIconAnim(&stat, CARD_STAT_ANIM_LOOP);
    CARDSetIconFormat(&stat, 0, CARD_STAT_ICON_C8);
    CARDSetIconFormat(&stat, 1, CARD_STAT_ICON_NONE);
    CARDSetIconFormat(&stat, 2, CARD_STAT_ICON_NONE);
    CARDSetIconFormat(&stat, 3, CARD_STAT_ICON_NONE);
    CARDSetIconFormat(&stat, 4, CARD_STAT_ICON_NONE);
    CARDSetIconFormat(&stat, 5, CARD_STAT_ICON_NONE);
    CARDSetIconFormat(&stat, 6, CARD_STAT_ICON_NONE);
    CARDSetIconFormat(&stat, 7, CARD_STAT_ICON_NONE);
    CARDSetIconSpeed(&stat, 0, CARD_STAT_SPEED_SLOW);
    CARDSetIconSpeed(&stat, 1, CARD_STAT_SPEED_END);
    CARDSetIconSpeed(&stat, 2, CARD_STAT_SPEED_END);
    CARDSetIconSpeed(&stat, 3, CARD_STAT_SPEED_END);
    CARDSetIconSpeed(&stat, 4, CARD_STAT_SPEED_END);
    CARDSetIconSpeed(&stat, 5, CARD_STAT_SPEED_END);
    CARDSetIconSpeed(&stat, 6, CARD_STAT_SPEED_END);
    CARDSetIconSpeed(&stat, 7, CARD_STAT_SPEED_END);
    CARDSetIconAddress(&stat, 0);
    CARDSetCommentAddress(&stat, 0x1C00);
    CARDSetStatus(lbl_8051C1A0.field_0x1ff4, file->fileNo, &stat);
    return res;
}


// FIXME(shotgun): 62.5 %, halfword clamp loop and sector-header store not written; reviewer moved under NONMATCHING (empty filler loop, casts)
#ifdef NONMATCHING
s32 fn_803BAEF4(CARDFileInfo* file, void* dst, void* bodyDst, u32 size, u32 bodySize) {
    u8* img1 = lbl_8051E2C0;
    u8* img2 = lbl_805202C0;
    s32 needWrite = 0;

    s32 res = CARDRead(file, img1, 0x2000, 0x2000);
    if (res != 0) {
        return res;
    }

    SaveFile_803B950C* files1 = (SaveFile_803B950C*)(img1 + 0xA);
    SaveFile_803B950C* files2 = (SaveFile_803B950C*)(img2 + 0xA);

    s32 v1_0 = fn_803BADAC(&files1[0]);
    s32 v1_1 = fn_803BADAC(&files1[1]);
    s32 v1_2 = fn_803BADAC(&files1[2]);

    res = CARDRead(file, img2, 0x2000, 0x4000);
    if (res != 0) {
        return res;
    }

    s32 v2_0 = fn_803BADAC(&files2[0]);
    s32 v2_1 = fn_803BADAC(&files2[1]);
    s32 v2_2 = fn_803BADAC(&files2[2]);

    if (!v1_0) {
        u32 sum = 0;
        const u8* p = (const u8*)&files1[0];
        for (int i = 0; i < 0xC8; i++) {
            sum += p[i];
        }
        if (sum == 0) {
            lbl_8050EC80.fn_803B933C(files1, 0);
            fn_803BACF8(files1, 0);
        } else if (v2_0) {
            memcpy(&files1[0], &files2[0], sizeof(SaveFile_803B950C));
            needWrite = 1;
        }
    } else {
        memcpy(&files2[0], &files1[0], sizeof(SaveFile_803B950C));
    }

    if (!v1_1) {
        u32 sum = 0;
        const u8* p = (const u8*)&files1[1];
        for (int i = 0; i < 0xC8; i++) {
            sum += p[i];
        }
        if (sum == 0) {
            lbl_8050EC80.fn_803B933C(files1, 1);
            fn_803BACF8(files1, 1);
        } else if (v2_1) {
            memcpy(&files1[1], &files2[1], sizeof(SaveFile_803B950C));
            needWrite = 1;
        }
    } else {
        memcpy(&files2[1], &files1[1], sizeof(SaveFile_803B950C));
    }

    if (!v1_2) {
        u32 sum = 0;
        const u8* p = (const u8*)&files1[2];
        for (int i = 0; i < 0xC8; i++) {
            sum += p[i];
        }
        if (sum == 0) {
            lbl_8050EC80.fn_803B933C(files1, 2);
            fn_803BACF8(files1, 2);
        } else if (v2_2) {
            memcpy(&files1[2], &files2[2], sizeof(SaveFile_803B950C));
            needWrite = 1;
        }
    } else {
        memcpy(&files2[2], &files1[2], sizeof(SaveFile_803B950C));
    }

    if (!isCardStatusValid(file)) {
        return CARD_RESULT_FATAL_ERROR;
    }

    if (needWrite) {
        res = CARDWrite(file, img1, 0x2000, 0x2000);
        if (res != 0) {
            return res;
        }
        res = CARDWrite(file, img1, 0x2000, 0x4000);
        if (res != 0) {
            return res;
        }
    }

    for (int i = 0; i < 3; i++) {
        SaveFile_803B950C* f = &files1[i];
        u8 locked = 0;
        u8 dirty = 0;
        if (f->field_0x65 & 1) {
            if (!(f->field_0x65 & 2)) {
                f->field_0x14 = 0;
                locked = 1;
                dirty = 1;
                f->field_0x16 = 0;
                f->field_0x58 = 0;
                f->field_0x59 = 50;
                f->field_0x66 = 0;
                f->field_0x67 = 0;
            }
        } else {
            f->field_0x65 = 0;
            locked = 1;
            f->field_0x14 = 0;
            f->field_0x16 = 0;
            f->field_0x58 = 0;
            f->field_0x59 = 50;
            f->field_0x66 = 0;
            f->field_0x67 = 0;
        }

        for (int stage = 0; stage < 8; stage++) {
            u8 newlyLocked = 0;
            for (int sub = 0; sub < 4; sub++) {
                if ((stage < 7 && sub == 2) || (stage == 7 && sub == 1)) {
                    f->mFlags |= 1 << (stage * 4 + sub);
                } else if (f->mFlags & (1 << (stage * 4 + sub))) {
                    if (locked == 1) {
                        f->mFlags &= ~(1 << (stage * 4 + sub));
                        dirty = 1;
                    }
                } else if (!locked) {
                    locked = 1;
                    newlyLocked = 1;
                }
            }
            if (f->field_0x5a[0] & (u8)(1 << stage)) {
                if (stage < 7) {
                    if (!(f->mFlags & (1 << (stage * 4 + 1)))) {
                        f->field_0x5a[0] &= ~(u8)(1 << stage);
                        dirty = 1;
                    }
                } else {
                    if (!(f->mFlags & (1 << (stage * 4 + 2)))) {
                        f->field_0x5a[0] &= ~(u8)(1 << stage);
                        dirty = 1;
                    }
                }
            }
            if ((f->field_0x62[0] & (u8)(1 << stage)) && locked == 1 && !newlyLocked) {
                f->field_0x62[0] &= ~(u8)(1 << stage);
                dirty = 1;
            }
        }

        if (f->field_0xc > 99999) {
            f->field_0xc = 0;
            dirty = 1;
        }
        if (f->field_0x10 > 99999) {
            f->field_0x10 = 99999;
            dirty = 1;
        }
        if (f->field_0x5a[1] > 1) {
            f->field_0x5a[1] = 0;
            dirty = 1;
        }
        if (f->field_0x66 > 99) {
            f->field_0x66 = 0;
            dirty = 1;
        }
        if (f->field_0x67 > 1) {
            f->field_0x67 = 1;
            dirty = 1;
        }
        if (f->field_0x59 > 119) {
            f->field_0x59 = 50;
            dirty = 1;
        }
        if (f->field_0x58 > 99) {
            f->field_0x58 = 0;
            dirty = 1;
        }
        if (f->field_0x14 > 999) {
            f->field_0x14 = 999;
            dirty = 1;
        }
        if (f->field_0x5c > 1) {
            f->field_0x5c = 1;
            dirty = 1;
        }
        if (f->field_0x5e > 1) {
            f->field_0x5e = 0;
            dirty = 1;
        }
        if (f->field_0x68 > 1) {
            f->field_0x68 = 1;
            dirty = 1;
        }
        if (f->field_0x16 > 999) {
            f->field_0x16 = 999;
            dirty = 1;
        }
        if (f->field_0x5d > 1) {
            f->field_0x5d = 0;
            dirty = 1;
        }
        if (f->field_0x5f > 1) {
            f->field_0x5f = 0;
            dirty = 1;
        }
        if (f->field_0x69 > 1) {
            f->field_0x69 = 1;
            dirty = 1;
        }
        // FIXME(shotgun): halfword clamping loop over field_0x18[0..0x3F] not written
        if (dirty) {
            // FIXME(shotgun): dirty-flag write-back not written
        }
    }

    memcpy(dst, img1 + 0xA, size);
    // FIXME(shotgun): store of sector header u16 to lbl_8051C1A0.field_0x2000 not written

    u64 serial;
    CARDGetSerialNo(lbl_8051C1A0.field_0x1ff4, &serial);
    if (serial != lbl_8051C1A0.field_0x1ff8) {
        memcpy(lbl_805242C0, img1 + 0x262, 0x1D4C);
        memcpy(lbl_80526020, img2 + 0x262, 0x1D4C);
    }
    lbl_8051C1A0.field_0x1ff8 = serial;
    return 0;
}
#endif

// Reads the save image at card offset 0x2000, or at 0x4000 when that fails, into
// lbl_8051E2C0; on success keeps its bytes 3 and 4 in the game state. Returns the card result.
s32 fn_803BAE68(CARDFileInfo* file) {
    SaveSector_803BACF8* image = &lbl_8051E2C0;
    if (CARDRead(file, image, 0x2000, 0x2000) != 0) {
        s32 result = CARDRead(file, image, 0x2000, 0x4000);
        if (result != 0) {
            return result;
        }
    }
    lbl_80529DEC.field_0x244 = image->field_0x3;
    lbl_80529DEC.field_0x246 = image->field_0x4;
    return 0;
}

// True when the checksum words of the save file match its first 0xC0 bytes.
// FIXME(shotgun): 43.57 %, missing/extra; behaviour drift.
// (our unrolled checksum loop needs more registers)
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
// FIXME(shotgun): 53.84 %, missing/extra; behaviour near.
// (as fn_803BADAC)
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
