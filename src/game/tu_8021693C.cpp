// cls_80217128, an attack hit box of the shadow Link (see its header). The
// unit's __sinit (the 9 JAudio pools) comes from the GameAudio.h include.

#include "game/GameAudio.h"
#include "game/cls_80217128.h"
#include "game/act/act_shadow_link_2.h"
#include "game/Player.h"
#include "game/ViewLayer.h"

// The box (-8, -8)-(8, 8) moved by (x, y), with the swing direction.
static inline void setBox(cls_80217128* hit, f32 x, f32 y, s32 type) {
    hit->mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    hit->mBox.addPos(x, y);
    hit->mType = type;
}

// FIXME: 90.9 %. Same instructions except the box copy and registers: the
// target reloads the box's i.y, f.x and f.y (0x8, 0xc, 0x10) after the two
// width/height tests, reusing only i.x, and loads the owner before the
// copy's stores into r30 with the mask in r29; ours keeps the test's loads
// live for the copy (three fewer instructions) and allocates the owner and
// mask to different registers. Tried: owner before / after the box copy,
// box.set(mBox) into a declared box.
u8 cls_80217128::fn_8021693C(u8 mask) {
    if (mask == 0) {
        return 0;
    }
    u8 result = 0;
    if (mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        ShadowLink2_804A43E8* owner = mOwner;
        JGeometry::TBox2f box(mBox);
        box.addPos(owner->mPos.x, owner->mPos.y - owner->mPos.z);
        for (s32 player = 0; player < 4; player++) {
            s32 layer = owner->field_0x4;
            if (viewLayerFlag(layer) == 0 &&
                __fabsf(mOwner->mPos.z - fn_8023E724(player)->z) > 32.0f) {
                continue;
            }
            s32 bit = 1 << player;
            if ((mask & bit) != 0 && fn_80239B64(player, &box, mOwner->field_0x4, mType, true, true)) {
                result |= bit;
            }
        }
    }
    return result;
}

void cls_80217128::fn_80216A8C() {
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    cls_800FE8C0& seq = mOwner->field_0x340;
    if (seq.mState == 2) {
        if (seq.mIndex < 3) {
            return;
        }
        switch (seq.mDir) {
        case 0:
            setBox(this, 16.0f, 0.0f, 0);
            break;
        case 1:
            setBox(this, -16.0f, 0.0f, 1);
            break;
        case 2:
            setBox(this, 0.0f, 16.0f, 2);
            break;
        case 3:
            setBox(this, 0.0f, -16.0f, 3);
            break;
        }
    } else if (seq.mState == 3) {
        if (seq.mIndex < 1) {
            return;
        }
        switch (seq.mDir) {
        case 0:
            setBox(this, 16.0f, 0.0f, 0);
            break;
        case 1:
            setBox(this, -16.0f, 0.0f, 1);
            break;
        case 2:
            setBox(this, 0.0f, 16.0f, 2);
            break;
        case 3:
            setBox(this, 0.0f, -16.0f, 3);
            break;
        }
    } else if (seq.mState == 6) {
        if (seq.mIndex < 2) {
            return;
        }
        switch (seq.mFrame) {
        case 0x90: case 0x9B: case 0xA1:
            setBox(this, 16.0f, 0.0f, 0);
            break;
        case 0x91: case 0x9C: case 0xA2:
            setBox(this, 13.0f, 13.0f, 2);
            break;
        case 0x92: case 0x9D: case 0xA3:
            setBox(this, 0.0f, 16.0f, 2);
            break;
        case 0x93: case 0x9E: case 0xA4:
            setBox(this, -13.0f, 13.0f, 1);
            break;
        case 0x94: case 0x97: case 0xA5:
            setBox(this, -16.0f, 0.0f, 1);
            break;
        case 0x95: case 0x98: case 0xA6:
            setBox(this, -13.0f, -13.0f, 3);
            break;
        case 0x96: case 0x99: case 0x9F:
            setBox(this, 0.0f, -16.0f, 3);
            break;
        case 0x8F: case 0x9A: case 0xA0:
            setBox(this, 13.0f, -13.0f, 0);
            break;
        }
    }
}

cls_80217128::cls_80217128() {
    mOwner = NULL;
    mType = 0;
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
}
