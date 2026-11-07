#include "game/GameAudio.h"
#include "game/Player.h"
#include "game/act/tu_801D8400.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"

// FIXME(shotgun): 97.94 %, missing/extra + reloc-name
void cls_804A4C58::fn_801DE12C() {
    if ((mState >= 0 && mState < 4) || (mState >= 0x13 && mState < 0x15)) {
        mBox.i.x = 1.0f;
        mBox.i.y = 1.0f;
        mBox.f.x = -1.0f;
        mBox.f.y = -1.0f;
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        return;
    }
    switch (mSeq2a8.fn_801DA96C()) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 11:
    case 12:
        mBox.i.x = -24.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 16.0f;
        break;
    case 6:
    case 7:
        mBox.i.x = -24.0f;
        mBox.i.y = -22.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 10.0f;
        break;
    case 8:
        mBox.i.x = -24.0f;
        mBox.i.y = -25.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 7.0f;
        break;
    case 9:
        mBox.i.x = -24.0f;
        mBox.i.y = -28.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 4.0f;
        break;
    case 10:
        mBox.i.x = -24.0f;
        mBox.i.y = -31.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 1.0f;
        break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 24:
        mBox.i.x = -24.0f;
        mBox.i.y = -48.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 16.0f;
        break;
    case 17:
    case 23:
        mBox.i.x = -24.0f;
        mBox.i.y = -40.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 16.0f;
        break;
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
        mBox.i.x = -24.0f;
        mBox.i.y = -32.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 16.0f;
        break;
    case 25:
    case 26:
    case 27:
        mBox.i.x = -24.0f;
        mBox.i.y = -52.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 16.0f;
        break;
    case 28:
    case 29:
    case 42:
    case 43:
    case 45:
    case 46:
    case 47:
        mBox.i.x = -41.0f;
        mBox.i.y = -41.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 9.0f;
        break;
    case 30:
    case 31:
    case 44:
        mBox.i.x = -41.0f;
        mBox.i.y = -47.0f;
        mBox.f.x = 6.0f;
        mBox.f.y = 3.0f;
        break;
    case 32:
        mBox.i.x = -38.0f;
        mBox.i.y = -61.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = -8.0f;
        break;
    case 33:
        mBox.i.x = -38.0f;
        mBox.i.y = -66.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = -12.0f;
        break;
    case 34:
        mBox.i.x = -35.0f;
        mBox.i.y = -68.0f;
        mBox.f.x = 14.0f;
        mBox.f.y = -16.0f;
        break;
    case 35:
        mBox.i.x = -24.0f;
        mBox.i.y = -72.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = -36.0f;
        break;
    case 36:
        mBox.i.x = -24.0f;
        mBox.i.y = -76.0f;
        mBox.f.x = 28.0f;
        mBox.f.y = -36.0f;
        break;
    case 37:
        mBox.i.x = -24.0f;
        mBox.i.y = -68.0f;
        mBox.f.x = 28.0f;
        mBox.f.y = -28.0f;
        break;
    case 38:
        mBox.i.x = -24.0f;
        mBox.i.y = -60.0f;
        mBox.f.x = 28.0f;
        mBox.f.y = -20.0f;
        break;
    case 39:
    case 40:
    case 41:
        mBox.i.x = -24.0f;
        mBox.i.y = -36.0f;
        mBox.f.x = 28.0f;
        mBox.f.y = 16.0f;
        break;
    }
    if (mBox.f.x - mBox.i.x > 0.0f && (mFlags & 0x2000) && mSeq2a8.fn_801DA904()) {
        f32 right = mBox.f.x;
        f32 left = mBox.i.x;
        mBox.i.x = -right;
        mBox.f.x = -left;
    }
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
}

// FIXME(shotgun): 90 %, missing/extra + reloc-name
bool cls_804A4C58::fn_801DE018() {
    bool inRange = false;
    if (mState == 5 || (mState >= 4 && mState < 8)) {
        inRange = true;
    }
    bool result = false;
    if (inRange) {
        JGeometry::TBox2f box;
        JGeometry::TVec2f out;
        box.set(-40.0f + mPos.x, -16.0f + mPos.y, 40.0f + mPos.x, 50.0f + mPos.y);
        s32 player = fn_8023AE34(&box, field_0x4, &out);
        if (player < 0) {
            player = fn_8023AA18(&box, field_0x4, &out);
        }
        if (player < 0) {
            player = fn_8023AC74(&box, field_0x4, &out);
        }
        if (player < 0) {
            player = fn_8023AD54(&box, field_0x4, &out);
        }
        if (player >= 0) {
            fn_801D9C58(5);
            result = true;
        }
    }
    return result;
}

extern const u8 lbl_8053F1C8; // hits before the knock-back state (tu_801D8400's .sdata)

// FIXME(shotgun): 77.34 %, missing/extra + reloc-name
bool cls_804A4C58::fn_801DDBE4() {
    bool hit = false;
    bool active = false;
    if (mState == 5 || (mState >= 4 && mState < 8)) {
        active = true;
    }
    if (active) {
        JGeometry::TBox2f saved = mBox;
        mBox.i.x = -40.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 50.0f;
        s32 serial = getActorManager_801F856C()->fn_801F82CC(this, field_0x195, 32.0f);
        if (serial >= 0 && serial != mSerial) {
            fn_801D9C58(5);
        }
        mBox = saved;
    }
    if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        s32 serial = getActorManager_801F856C()->fn_801F82CC(this, field_0x195, 32.0f);
        if (serial >= 0 && serial != mSerial) {
            ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            bool touched = false;
            if (other->mActorId == 'SWBM' || other->mActorId == 'FIRE') {
                if (mState == 0x11) {
                    touched = true;
                } else if (!(other->field_0x3c.y > 0.0f)) {
                    switch (mState) {
                    case 0xC:
                        touched = true;
                        break;
                    case 0xB:
                    case 0xD:
                    case 0xE:
                        if (other->field_0x3c.y < 0.0f) {
                            touched = true;
                        } else if (mFlags & 0x2000) {
                            if (other->field_0x3c.x < 0.0) {
                                touched = true;
                            }
                        } else if (other->field_0x3c.x > 0.0) {
                            touched = true;
                        }
                        break;
                    }
                }
            }
            if (touched) {
                s32 life = mLife;
                JGeometry::TVec3f from = mPos - other->field_0x3c;
                if (fn_801F37AC(other->field_0x118, &from, 0, other->field_0x168, other->field_0x110)) {
                    if (mLife != life) {
                        hit = true;
                        if (mState != 0x11) {
                            mLife = life;
                        }
                    }
                }
            }
            if (hit) {
                field_0xf8 = 0;
                field_0x3c.z = 0.0f;
                mPos.z = 0.0f;
                startSound_801F0E34(0x40005, 0);
                fn_801E62D4();
                if (mState != 0x11) {
                    field_0x33f++;
                    if (field_0x33f >= lbl_8053F1C8) {
                        field_0x3c.x *= 2.0f;
                        field_0x3c.y *= 2.0f;
                        fn_801D9C58(0xF);
                    }
                }
                if (mLife <= 0) {
                    fn_801D9C58(0x13);
                }
            } else if (field_0x1dc.fn_801EAC54(other->mSerial)) {
                startSound_801F0E34(0x400D1, 0);
                switch (other->mActorId) {
                case 'SWBM':
                    fn_801CB538(field_0x4, &other->mPos, 0, 0);
                    break;
                case 'FIRE':
                    fn_801CB538(field_0x4, &other->mPos, 0, 0);
                    spawnEmitter_8013CC50(field_0x4, &other->mPos, 0x19D, 0, 0, NULL, NULL, true, 1.0f);
                    break;
                }
            }
            other->field_0x164 = 1;
        }
    }
    return hit;
}

// Whether player i overlaps the sequencer's hit area at all; state 0x11 always counts.
static inline bool playerTouches(cls_804A4C58* boss, s32 i) {
    bool touched = false;
    if (i >= 0 && i < 4 && !fn_80235448(i)) {
        if (boss->mState == 0x11) {
            touched = true;
        } else if (8.0f + getPlayerPos_8023E724(i)->y >= boss->mPos.y + boss->mBox.i.y) {
            switch (boss->mState) {
            case 0xC:
                touched = true;
                break;
            case 0xB:
            case 0xD:
            case 0xE:
                if (getPlayerPos_8023E724(i)->y - 8.0f > boss->mPos.y + boss->mBox.f.y) {
                    touched = true;
                } else if (boss->mFlags & 0x2000) {
                    if (boss->mPos.x < getPlayerPos_8023E724(i)->x) {
                        touched = true;
                    }
                } else if (getPlayerPos_8023E724(i)->x < boss->mPos.x) {
                    touched = true;
                }
                break;
            }
        }
    }
    return touched;
}

// FIXME(shotgun): 87.94 %, different + reloc-name
bool cls_804A4C58::fn_801DD410() {
    bool hit = false;
    bool active = false;
    if (mState == 5 || (mState >= 4 && mState < 8)) {
        active = true;
    }
    s32 outFlag;
    if (active) {
        JGeometry::TBox2f saved = mBox;
        mBox.i.x = -40.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 40.0f;
        mBox.f.y = 50.0f;
        if (fn_801F285C(NULL, &outFlag, true, -1) >= 0) {
            fn_801D9C58(5);
        }
        mBox = saved;
    }
    if (mState == 0x11) {
        JGeometry::TBox2f saved = mBox;
        mBox.i.x = -20.0f;
        mBox.i.y = -26.0f;
        mBox.f.x = 22.0f;
        mBox.f.y = -25.0f;
        if (mBox.f.x - mBox.i.x > 0.0f && (mFlags & 0x2000) && mSeq2a8.fn_801DA904()) {
            f32 right = mBox.f.x;
            f32 left = mBox.i.x;
            mBox.i.x = -right;
            mBox.f.x = -left;
        }
        for (s32 i = 0; i < 4; i++) {
            if (i == fn_801F285C(NULL, &outFlag, true, i) && fn_80235448(i)) {
                if (fn_801F37AC(0, getPlayerPos_8023E724(i), 0, outFlag, i)) {
                    hit = true;
                    break;
                }
            }
        }
        if (hit) {
            mLife = 0;
            field_0xf8 = 0;
            field_0x3c.z = 0.0f;
            mPos.z = 0.0f;
            startSound_801F0E34(0x40005, 0);
            fn_801E62D4();
            if (mState != 0x11) {
                field_0x33f++;
                if (field_0x33f >= lbl_8053F1C8) {
                    field_0x3c.x *= 2.0f;
                    field_0x3c.y *= 2.0f;
                    fn_801D9C58(0xF);
                }
            }
            fn_801D9C58(0x13);
        }
        mBox = saved;
        if (!hit && mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
            for (s32 i = 0; i < 4; i++) {
                if (i == fn_801F285C(NULL, &outFlag, true, i)) {
                    if (playerTouches(this, i) && fn_801F37AC(0, getPlayerPos_8023E724(i), fn_8023B6CC(i), outFlag, i)) {
                        hit = true;
                    }
                }
            }
            if (hit) {
                field_0xf8 = 0;
                field_0x3c.z = 0.0f;
                mPos.z = 0.0f;
                startSound_801F0E34(0x40005, 0);
                fn_801E62D4();
                if (mState != 0x11) {
                    field_0x33f++;
                    if (field_0x33f >= lbl_8053F1C8) {
                        field_0x3c.x *= 2.0f;
                        field_0x3c.y *= 2.0f;
                        fn_801D9C58(0xF);
                    }
                }
                if (mLife <= 0) {
                    fn_801D9C58(0x13);
                }
            }
        }
    } else if (mState >= 0xB && mState < 0xF) {
        if (mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
            bool sound = false;
            JGeometry::TVec2f where;
            for (s32 i = 0; i < 4; i++) {
                if (i == fn_801F285C(&where, &outFlag, true, i)) {
                    fn_8023B6CC(i);
                    if (playerTouches(this, i)) {
                        if (fn_801F37AC(0, getPlayerPos_8023E724(i), 0, outFlag, i)) {
                            hit = true;
                        }
                    } else {
                        JGeometry::TVec2f p(mPos.x, mPos.y);
                        fn_80237948(i, &p);
                        if (outFlag == 1 && field_0x1dc.fn_801EAC20(i)) {
                            JGeometry::TVec3f at(where.x, where.y, 0.0f);
                            fn_801CB538(field_0x4, &at, 0, 0);
                            sound = true;
                        }
                    }
                }
            }
            if (sound) {
                startSound_801F0E34(0x400D1, 0);
            }
            if (hit) {
                field_0xf8 = 0;
                field_0x3c.z = 0.0f;
                mPos.z = 0.0f;
                startSound_801F0E34(0x40005, 0);
                fn_801E62D4();
                if (mState != 0x11) {
                    field_0x33f++;
                    if (field_0x33f >= lbl_8053F1C8) {
                        field_0x3c.x *= 2.0f;
                        field_0x3c.y *= 2.0f;
                        fn_801D9C58(0xF);
                    }
                }
            }
        }
    }
    return hit;
}

// FIXME(shotgun): 98 %, missing/extra + reloc-name
u8 cls_804A4C58::fn_801DD28C() {
    u8 hit = 0;
    f32 x0 = 1.0f;
    f32 y0 = x0;
    f32 x1 = -1.0f;
    f32 y1 = x1;
    if (mState == 9) {
        x0 = -55.0f;
        y0 = -32.0f;
        x1 = 55.0f;
        y1 = 12.0f;
    }
    if (x1 - x0 > 0.0f && (mFlags & 0x2000) && mSeq2a8.fn_801DA904()) {
        f32 t = x0;
        x0 = -x1;
        x1 = -t;
    }
    if (x1 - x0 > 0.0f && y1 - y0 > 0.0f) {
        f32 sx0 = mBox.i.x;
        f32 sy0 = mBox.i.y;
        f32 sx1 = mBox.f.x;
        f32 sy1 = mBox.f.y;
        mBox.i.x = x0;
        mBox.i.y = y0;
        mBox.f.x = x1;
        mBox.f.y = y1;
        hit = fn_801F2B7C(field_0x114, -1, 1);
        mBox.i.x = sx0;
        mBox.i.y = sy0;
        mBox.f.x = sx1;
        mBox.f.y = sy1;
    }
    return hit;
}

// FIXME(shotgun): 98 %, missing/extra
bool cls_804A4C58::fn_801DD1AC() {
    bool clamped = false;
    if (!(mFlags & ENEMY_FLAG_NO_MAP_HIT)) {
        if (mPos.x < field_0x2d4[0].x) {
            mPos.x = field_0x2d4[0].x;
            clamped = true;
            mMapHitDir.set(1.0f, 0.0f, 0.0f);
        } else if (field_0x2d4[3].x < mPos.x) {
            mPos.x = field_0x2d4[3].x;
            clamped = true;
            mMapHitDir.set(-1.0f, 0.0f, 0.0f);
        }
        if (mPos.y > field_0x2d0) {
            mPos.y = field_0x2d0;
            clamped = true;
            mMapHitDir.set(0.0f, -1.0f, 0.0f);
        } else if (mState >= 0xB && mState < 0x11) {
            if (mPos.y < field_0x2d4[0].y) {
                mPos.y = field_0x2d4[0].y;
                clamped = true;
                mMapHitDir.set(0.0f, 1.0f, 0.0f);
            }
        }
    }
    return clamped;
}
