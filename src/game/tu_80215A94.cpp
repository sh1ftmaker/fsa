#include "game/EnemyBase.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/tu_80215A94.h"
#include "game/tu_801ED3DC.h"
#include "game/tu_803B8C74.h"
#include "game/GameView.h"
#include "game/Player.h"
#include <float.h>

static const s32 lbl_8046A590[4] = {500, 300, 100, 0};

// Moves v toward target by 1/d of the way (all the way once d is below epsilon).
static inline void stepToward(f32& v, f32 target, f32 d) {
    if (d >= FLT_EPSILON) {
        v += (1.0f / d) * (target - v);
    } else {
        v = target;
    }
}

static inline void fn_802162B8_loop(cls_80216238Sub* sub, s32 id, s32 key) {
    s32 player = id;
    if (fn_8022F514(player)) {
        player = fn_8022F3DC(player);
    }
    u16 effect = fn_8023CBC0(player);
    f32 yTarget = 16.0f * (17.5f + key);
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022E438(i, id)) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(id);
            if (key < 0) {
                fn_80230498(i, &pos, effect, 0xFF, -0x80, -0x80, true);
            } else if (sub->mFlagA == 30) {
                fn_80230498(i, &pos, effect, 0xFF, -0x80, -0x80, true);
            } else {
                GameView_804A9950* view = getGameView_802D6A9C();
                f32 d = sub->mFlagA;
                stepToward(pos.x, 32.0f + view->mVisibleBox.i.x, d);
                stepToward(pos.y, view->mVisibleBox.i.y + yTarget, d);
                stepToward(pos.z, 0.0f, d);
                fn_80230498(i, &pos, sub->field_0xc.field_0x0 + 0x33A, 0xFF, -0x80, -0x80, true);
            }
        }
    }
}

// FIXME(shotgun): 85.1 %, cls_80216238Sub
void cls_80216238Sub::fn_802162B8(s32 id, s32 key) {
    if (mFlag8) {
        if (mFlagA) {
            field_0xc.fn_8021BFA8();
            mFlagA--;
            if (mFlagA) {
                fn_802162B8_loop(this, id, key);
            }
        }
    } else if (mFlag9) {
        if (fn_802308E0(id)) {
            mFlag8 = 1;
            mFlag9 = 0;
            mFlagA = 30;
            fn_802162B8_loop(this, id, key);
        }
    }
}

// FIXME(shotgun): 96.9 %, missing/extra
void cls_80216238::fn_80216238() {
    mSub.field_0xc.fn_8021C00C();
    mSub.mFlag8 = 0;
    mSub.mFlag9 = 0;
    mSub.mFlagA = 0;
    mSub.field_0xc.fn_8021C00C();
    mWatch.fn_801ED80C();
    mId = 0;
    mKey = -1;
    mSub.mFlag8 = 0;
    mSub.mFlag9 = 0;
    mSub.mFlagA = 0;
    mSub.field_0xc.fn_8021C00C();
    mWatch.fn_801ED7F0();
}

// FIXME(shotgun): 55.0 %, missing/extra + reloc-name; the original builds the entries with __construct_array(fn_80216238), which
// needs a constructor on cls_80216238 (not added: it would make the horse-race man's inline
// constructor build its table member).
cls_80215E48* fn_80216178(cls_80215E48* table) {
    table->mCount = gGameRoot_8053AAF8->fn_80138A30();
    for (s32 i = 0; i < 4; i++) {
        table->mEntries[i].fn_80216238();
    }
    table->fn_802160F4();
    return table;
}

void cls_80215E48::fn_802160F4() {
    for (s32 i = 0; i < mCount; i++) {
        mEntries[i].mId = i;
        mEntries[i].mKey = -1;
        mEntries[i].mSub.mFlag8 = 0;
        mEntries[i].mSub.mFlag9 = 0;
        mEntries[i].mSub.mFlagA = 0;
        mEntries[i].mSub.field_0xc.fn_8021C00C();
        mEntries[i].mWatch.fn_801ED7F0();
    }
    field_0x94.fn_8021C078();
    field_0xa0 = 0;
}

// const: the original loads mCount above `mr r30, r3` (the loop tests the member; a count
// local keeps the load after the register saves).
bool cls_80215E48::fn_8021606C() const {
    bool result = false;
    for (s32 i = 0; i < mCount; i++) {
        if (mEntries[i].mKey < 0) {
            if (mEntries[i].mWatch.fn_801ED7B0()) {
                result = true;
                break;
            }
        }
    }
    return result;
}

void cls_80215E48::fn_8021600C() {
    for (s32 i = 0; i < mCount; i++) {
        mEntries[i].mWatch.fn_801ED798();
    }
}

void cls_80215E48::fn_80215FB8(s32 index, s32 key, f32 fraction) {
    mEntries[index].mWatch.fn_801ED5C8(fraction);
    mEntries[index].mKey = key;
}

void cls_80215E48::fn_80215F30() {
    if (gGameRoot_8053AAF8->field_0x4c != 9 && gGameRoot_8053AAF8->field_0x4c != 0xB) {
        field_0xa0 = fn_801CD664(10);
    }
    for (s32 i = 0; i < mCount; i++) {
        mEntries[i].mWatch.fn_801ED458(field_0xa0);
    }
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

s32 cls_80215E48::fn_80215E98() {
    s32 count = 0;
    for (s32 i = 0; i < mCount; i++) {
        if (mEntries[i].mKey >= 0) {
            count++;
        }
    }
    return count;
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

s32 cls_80215E48::fn_80215E48(s32 index) {
    return mEntries[index].mKey;
}

// Not auto-inlined: fn_80215DB8 calls it out of line (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
void cls_80216238::fn_80215E18() {
    mSub.fn_802162B8(mId, mKey);
}
#pragma auto_inline reset

void cls_80215E48::fn_80215DB8() {
    for (s32 i = 0; i < mCount; i++) {
        mEntries[i].fn_80215E18();
    }
}

void cls_80215E48::fn_80215D8C(s32 index) {
    if (mEntries[index].mSub.mFlag8 != 0) {
        return;
    }
    if (mEntries[index].mSub.mFlag9 != 0) {
        return;
    }
    mEntries[index].mSub.mFlag9 = 1;
}

bool cls_80215E48::fn_80215D38() {
    bool ready;
    bool result = true;
    for (s32 i = 0; i < mCount; i++) {
        ready = false;
        if (mEntries[i].mSub.mFlag8 != 0) {
            if (mEntries[i].mSub.mFlagA == 0) {
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

// FIXME(shotgun): 82.3 %, missing/extra
bool cls_80215E48::fn_80215C6C(s32 index) {
    bool result = false;
    if (mEntries[index].mWatch.mStopped != 0 && !mEntries[index].mWatch.fn_801ED7B0()) {
        u8 seconds = lbl_8050EC80.getCurrentFile().field_0x59;
        u8 centis = lbl_8050EC80.getCurrentFile().field_0x58;
        if (seconds == 0 && centis == 0) {
            result = true;
        } else if (mEntries[index].mWatch.mSeconds < seconds) {
            result = true;
        } else if (mEntries[index].mWatch.mSeconds == seconds && mEntries[index].mWatch.mCentis < centis) {
            result = true;
        }
    }
    return result;
}

// FIXME: 93.62 %. The original loads mCentis (lbz 0x25) right after
// mSeconds, before the second save-row index load; ours loads it after.
void cls_80215E48::fn_80215BCC(s32 index) {
    if (mEntries[index].mWatch.mStopped != 0 && !mEntries[index].mWatch.fn_801ED7B0()) {
        lbl_8050EC80.getCurrentFile().field_0x59 = mEntries[index].mWatch.mSeconds;
        lbl_8050EC80.getCurrentFile().field_0x58 = mEntries[index].mWatch.mCentis;
        getGameView_802D6A9C()->fn_802D7844(1, mEntries[index].mId);
    }
}

void cls_80215E48::fn_80215B6C(s32 index) {
    s32 kind = mEntries[index].mKey;
    if (kind >= 0 && kind < 4) {
        s32 value = lbl_8046A590[kind];
        if (value > 0) {
            lbl_8053AED0->fn_8043D874(mEntries[index].mId, value, 0);
        }
    }
}

// FIXME(shotgun): 85.1 %, missing/extra
s32 cls_80215E48::fn_80215A94() {
    s32 count = mCount;
    s32 result = -1;
    if (count >= 2 && count - 1 == fn_80215E98()) {
        for (s32 i = 0; i < count; i++) {
            if (mEntries[i].mKey < 0 && fn_801CD2F8(getPlayerLayer_8023DE58(i))) {
                result = i;
                break;
            }
        }
    }
    return result;
}
