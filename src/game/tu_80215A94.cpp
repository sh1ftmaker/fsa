#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/tu_80215A94.h"
#include "game/tu_801ED3DC.h"
#include "game/tu_80379F84.h"

static const s32 lbl_8046A590[4] = {500, 300, 100, 0};

void cls_80215E48::fn_80215B6C(s32 index) {
    cls_80215E48* entry = this + index;
    s32 kind = entry->mKey;
    if (kind >= 0 && kind < 4) {
        s32 value = lbl_8046A590[kind];
        if (value > 0) {
            lbl_8053AED0->fn_8043D874(entry->mId, value, 0);
        }
    }
}

s32 cls_80215E48::fn_80215E48(s32 index) {
    return (this + index)->mKey;
}

s32 cls_80215E48::fn_80215E58(s32 key) {
    s32 result = -1;
    cls_80215E48* entry = this;
    for (s32 i = 0; i < mCount; i++) {
        if (key == entry->mKey) {
            result = i;
            break;
        }
        entry++;
    }
    return result;
}

s32 cls_80215E48::fn_80215E98() {
    s32 count = 0;
    cls_80215E48* entry = this;
    for (s32 i = 0; i < mCount; i++) {
        if (entry->mKey >= 0) {
            count++;
        }
        entry++;
    }
    return count;
}

bool cls_80215E48::fn_80215D38() {
    cls_80215E48* entry = this;
    bool ready;
    bool result = true;
    for (s32 i = 0; i < mCount; i++) {
        ready = false;
        if (entry->mFlagC != 0) {
            if (entry->mFlagE == 0) {
                ready = true;
            }
        }
        if (!ready) {
            result = false;
            break;
        }
        entry++;
    }
    return result;
}

void cls_80215E48::fn_80215BCC(s32 index) {
    // FIXME: 73.75%. The original folds the stopwatch's mStopped load into
    // lbz 0x1c(entry) off the entry pointer; ours computes &entry->pad_0x1C
    // first (addi/add) and keeps it in a register, so one more register is
    // saved. The stopwatch is 0xA bytes past the entry's 0x24 stride, so it
    // can't be a struct member here.
    if (reinterpret_cast<cls_801ED3DC*>(this[index].pad_0x1C)->mStopped != 0 &&
        !reinterpret_cast<cls_801ED3DC*>(this[index].pad_0x1C)->fn_801ED7B0()) {
        lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x79 =
            reinterpret_cast<cls_801ED3DC*>(this[index].pad_0x1C)->mSeconds;
        lbl_8050EC80[lbl_8050EC80[0].field_0x10].field_0x78 =
            reinterpret_cast<cls_801ED3DC*>(this[index].pad_0x1C)->mCentis;
        fn_802D6A9C()->fn_802D7844(1, this[index].mId);
    }
}

void cls_80215E48::fn_80215D8C(s32 index) {
    cls_80215E48* entry = this + index;
    if (entry->mFlagC != 0) {
        return;
    }
    if (entry->mFlagD != 0) {
        return;
    }
    entry->mFlagD = 1;
}

void cls_80215E48::fn_80215DB8() {
    cls_80215E48* entry = this;
    for (s32 i = 0; i < mCount; i++) {
        fn_80215E18(&entry->mId);
        entry++;
    }
}

bool cls_80215E48::fn_8021606C() {
    bool result = false;
    cls_80215E48* entry = this;
    // FIXME: 86.76%. The original loads mCount ahead of `entry = this` and
    // keeps it in r31; ours reads it in the loop test and the two copies of
    // this swap registers.
    for (s32 i = 0; i < mCount; i++) {
        if (entry->mKey < 0) {
            if (reinterpret_cast<cls_801ED3DC*>(entry->pad_0x1C)->fn_801ED7B0()) {
                result = true;
                break;
            }
        }
        entry++;
    }
    return result;
}

void cls_80215E48::fn_8021600C() {
    cls_80215E48* entry = this;
    for (s32 i = 0; i < mCount; i++) {
        reinterpret_cast<cls_801ED3DC*>(entry->pad_0x1C)->fn_801ED798();
        entry++;
    }
}

void cls_80215E48::fn_80215ECC() {
    s32 count = mCount;
    cls_80215E48* entry = this;
    // FIXME: 92.00%. The count load (lwz r31, 0(r3)) is scheduled before
    // `mr r30, r3` in the original; ours loads it after `li r29, 0`.
    for (s32 i = 0; i < count; i++) {
        reinterpret_cast<cls_801ED3DC*>(entry->pad_0x1C)->fn_801ED3DC(entry->mId, entry->mKey);
        entry++;
    }
}

void cls_80215E48::fn_80215FB8(s32 index, s32 key, f32 fraction) {
    reinterpret_cast<cls_801ED3DC*>((this + index)->pad_0x1C)->fn_801ED5C8(fraction);
    (this + index)->mKey = key;
}
