#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/tu_80215A94.h"
#include "game/tu_801ED3DC.h"
#include "game/tu_80379F84.h"

static const s32 lbl_8046A590[4] = {500, 300, 100, 0};

void cls_80215E48::fn_80215B6C(s32 index) {
    s32 kind = mEntries[index].mKey;
    if (kind >= 0 && kind < 4) {
        s32 value = lbl_8046A590[kind];
        if (value > 0) {
            lbl_8053AED0->fn_8043D874(mEntries[index].mId, value, 0);
        }
    }
}

void cls_80215E48::fn_80215BCC(s32 index) {
    // FIXME: 93.62 %. The original loads mCentis (lbz 0x25) right after
    // mSeconds, before the second save-row index load; ours loads it after.
    if (mEntries[index].mWatch.mStopped != 0 && !mEntries[index].mWatch.fn_801ED7B0()) {
        lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x79 = mEntries[index].mWatch.mSeconds;
        lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x78 = mEntries[index].mWatch.mCentis;
        fn_802D6A9C()->fn_802D7844(1, mEntries[index].mId);
    }
}

bool cls_80215E48::fn_80215D38() {
    bool ready;
    bool result = true;
    for (s32 i = 0; i < mCount; i++) {
        ready = false;
        if (mEntries[i].mFlag8 != 0) {
            if (mEntries[i].mFlagA == 0) {
                ready = true;
            }
        }
        if (!ready) {
            result = false;
            break;
        }
    }
    return result;
}

void cls_80215E48::fn_80215D8C(s32 index) {
    if (mEntries[index].mFlag8 != 0) {
        return;
    }
    if (mEntries[index].mFlag9 != 0) {
        return;
    }
    mEntries[index].mFlag9 = 1;
}

void cls_80215E48::fn_80215DB8() {
    for (s32 i = 0; i < mCount; i++) {
        mEntries[i].fn_80215E18();
    }
}

s32 cls_80215E48::fn_80215E48(s32 index) {
    return mEntries[index].mKey;
}

s32 cls_80215E48::fn_80215E58(s32 key) {
    s32 result = -1;
    for (s32 i = 0; i < mCount; i++) {
        if (key == mEntries[i].mKey) {
            result = i;
            break;
        }
    }
    return result;
}

s32 cls_80215E48::fn_80215E98() {
    s32 count = 0;
    for (s32 i = 0; i < mCount; i++) {
        if (mEntries[i].mKey >= 0) {
            count++;
        }
    }
    return count;
}

void cls_80215E48::fn_80215ECC() {
    s32 count = mCount;
    cls_80216238* entry = mEntries;
    // FIXME: 89.48 %. The original keeps the entry walk off `this` (lwz
    // 0x4(r30), addi 0x1c) with the count loaded first; ours adds 4 first.
    for (s32 i = 0; i < count; i++) {
        entry->mWatch.fn_801ED3DC(entry->mId, entry->mKey);
        entry++;
    }
}

void cls_80215E48::fn_80215FB8(s32 index, s32 key, f32 fraction) {
    mEntries[index].mWatch.fn_801ED5C8(fraction);
    mEntries[index].mKey = key;
}

void cls_80215E48::fn_8021600C() {
    for (s32 i = 0; i < mCount; i++) {
        mEntries[i].mWatch.fn_801ED798();
    }
}

bool cls_80215E48::fn_8021606C() {
    // FIXME: 91.62 %. The original loads mCount above `mr r30, r3` (a const
    // `this`?); ours loads it after the register saves.
    s32 count = mCount;
    bool result = false;
    for (s32 i = 0; i < count; i++) {
        if (mEntries[i].mKey < 0) {
            if (mEntries[i].mWatch.fn_801ED7B0()) {
                result = true;
                break;
            }
        }
    }
    return result;
}
