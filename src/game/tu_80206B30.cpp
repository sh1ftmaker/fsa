#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_thief.h"

bool Thief_804A5A30::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if (field_0xc4 != 0 || mState == 3) {
        return false;
    }
    switch (fn_8022461C(pos, layer)) {
    case 0x39:
        return false;
    case 0x20:
        if (fn_80204638()->field_0x0 != 0 || fn_802064E0()) {
            return true;
        }
        if (mState == 4 && mPos.z > 1.0f) {
            return false;
        }
        break;
    }
    return ActorBase_804A5770::vfunc_0x4C(layer, pos);
}

bool Thief_804A5A30::vfunc_0x64() {
    bool hit = false;
    if (!fn_8020609C() && !(mFlags & ENEMY_FLAG_NO_MAP_HIT) && field_0x80.getWidth() > 0.0f &&
        field_0x80.getHeight() > 0.0f) {
        hit = fn_801F2618(&mMapHitDir, lbl_8053EFC0);
    }
    return hit;
}

u8 Thief_804A5A30::vfunc_0x68() {
    u8 hit = 0;
    JGeometry::TVec3f out;
    JGeometry::TBox2f box;
    JGeometry::TVec3f in;
    JGeometry::TVec2f center;
    JGeometry::TVec2f pos;
    if (field_0xc4 == 0 && mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f && !fn_802064E0()) {
        switch (mState) {
        case 2: {
            f32 x0 = mBox.i.x;
            f32 y0 = mBox.i.y;
            f32 x1 = mBox.f.x;
            f32 y1 = mBox.f.y;
            mBox.set(-4.0f, -6.0f, 4.0f, 4.0f);
            hit = fn_801F2740(-1);
            mBox.set(x0, y0, x1, y1);
            if (hit) {
                fn_801F3128(&center);
                for (s32 i = 0; i < 4; i++) {
                    if (hit & (1 << i)) {
                        if (fn_801F329C(i)) {
                            in.set(center.x, center.y, 0.0f);
                            fn_801CD0D0(&in, &out);
                            pos.x = out.x;
                            pos.y = out.y;
                        } else {
                            pos.x = center.x;
                            pos.y = center.y;
                        }
                        fn_80238548(i, 0, &pos, lbl_8046A238[mType].field_0x8);
                        if (mState != 5) {
                            // Only x and y of the player's position are read.
                            fn_801D2978(&field_0x35c, &pos, (const JGeometry::TVec2f*)fn_8023E724(i), 24, 1.0f);
                            fn_80205E0C(5);
                        }
                    }
                }
            }
            break;
        }
        case 3:
        case 4:
        case 9:
            break;
        default: {
            fn_801F31C4(&box);
            fn_802397B4(&box, field_0x4, mSerial);
            break;
        }
        }
    }
    return hit;
}

bool Thief_804A5A30::vfunc_0x5C() {
    bool hit = false;
    if (field_0xc4 == 0 && mState != 3 && mState != 4 && mBox.getWidth() > 0.0f &&
        mBox.getHeight() > 0.0f) {
        for (s32 i = 0; i < 4; i++) {
            s32 type;
            if (i == fn_801F285C(NULL, &type, true, i)) {
                fn_8023B6CC(i);
                if (fn_801F37AC(0, fn_8023E724(i), 0, type, i)) {
                    hit = true;
                }
            }
        }
        if (hit) {
            fn_80205E0C(7);
        }
    }
    return hit;
}

bool Thief_804A5A30::vfunc_0x60() {
    bool hit = false;
    if (field_0xc4 == 0 && mState != 3 && mState != 4 && mBox.getWidth() > 0.0f &&
        mBox.getHeight() > 0.0f) {
        hit = fn_801F2DB4('NULL', 'NULL', NULL);
        if (hit) {
            fn_80205E0C(7);
        }
    }
    return hit;
}

// FIXME: 98.14 %. Same instructions; the target builds mPos - vel48 in a
// second stack temporary (the xy pair is stored to 0x20 and the z reloaded
// from 0x1c) before the scalar copy into `from`, this code does it in place.
// `w = diff; from.set(w.x, w.y, w.z);` reproduces it (100 %) but is a
// redundant copy kept only for the bytes, so it is not used.
bool Thief_804A5A30::vfunc_0x58() {
    if (field_0xc4 != 0 || mState == 3 || mState == 4 || mBox2.getWidth() <= 0.0f ||
        mBox2.getHeight() <= 0.0f) {
        return false;
    }
    JGeometry::TVec3f vel54;
    JGeometry::TVec3f vel48;
    JGeometry::TBox2f box;
    JGeometry::TVec3f from;
    JGeometry::TVec3f diff;
    JGeometry::TVec2f probe;
    s32 player;
    s32 other;
    bool arrow = false;
    bool bomb = false;
    bool seed = false;
    other = -1;
    vel48.set(0.0f, 0.0f, 0.0f);
    fn_801F317C(&box);
    player = fn_8023AE34(&box, field_0x4, &probe);
    if (player >= 0) {
        arrow = fn_801CC820(player, &vel48);
    } else {
        player = fn_8023AA18(&box, field_0x4, &probe);
        if (player >= 0) {
            bomb = true;
        } else {
            player = fn_8023AC74(&box, field_0x4, &probe);
            if (player >= 0) {
                seed = fn_801CC538(player, &probe, &vel54, &other);
            } else {
                player = fn_8023AD54(&box, field_0x4, &probe);
                if (player >= 0) {
                    fn_801CC640(player, &other, &vel48);
                }
            }
        }
    }
    bool hit = false;
    if (player >= 0) {
        from = *fn_8023E724(player);
        s32 type = 0;
        if (bomb) {
            type = 4;
        } else if (seed) {
            from.set(vel54.x, vel54.y, vel54.z);
        } else {
            diff = mPos;
            diff.sub(vel48);
            from.set(diff.x, diff.y, diff.z);
            if (arrow) {
                type = 3;
            }
        }
        hit = fn_801F37AC(0, &from, 0, type, player);
        if (hit) {
            fn_80205E0C(7);
        }
    }
    return hit;
}

void Thief_804A5A30::fn_802072E4() {
    mBox.set(-8.0f, -16.0f, 8.0f, 8.0f);
    mBox2.set(mBox);
}
