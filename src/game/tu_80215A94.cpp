#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/tu_80215A94.h"
#include "game/tu_801ED3DC.h"

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

extern "C" void fn_80215E18(s32* p);

void cls_80215E48::fn_80215DB8() {
    cls_80215E48* entry = this;
    for (s32 i = 0; i < mCount; i++) {
        fn_80215E18(&entry->mId);
        entry++;
    }
}

void cls_80215E48::fn_8021600C() {
    cls_80215E48* entry = this;
    for (s32 i = 0; i < mCount; i++) {
        reinterpret_cast<cls_801ED3DC*>(entry->pad_0x1C)->fn_801ED798();
        entry++;
    }
}

void cls_80215E48::fn_80215FB8(s32 index, s32 key, f32 fraction) {
    reinterpret_cast<cls_801ED3DC*>((this + index)->pad_0x1C)->fn_801ED5C8(fraction);
    (this + index)->mKey = key;
}
