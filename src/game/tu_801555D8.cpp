#include "game/GameAudio.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JMath/JMath.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_bomb.h"
#include "game/act/act_chief_soldier.h"
#include "game/act/tu_801F9484.h"
#include "std/math.h"

f32 fn_80154744();
f32 fn_80154378(f32);
void fn_80154590(JGeometry::TBox2f*, f32, f32, f32, f32);

struct Table_8049FFD8 {
    s32 tiles[6][2];
    f32 vel[6][4][2];
};

extern const f32 lbl_8049FDA8[4][2];
extern const s32 lbl_8049FDC8[];     // life by kind and anim tables (.data of this unit)
extern const Table_8049FFD8 lbl_8049FFD8; // tile positions and velocities of the kind-2 variant
extern s32 lbl_8053AB50;            // running count of the soldiers spawned

void fn_801583E4(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(src->x, src->y, src->z);
}

void fn_801583D4(JGeometry::TVec3f* dst, f32 x, f32 y, f32 z) {
    dst->set(x, y, z);
}

// FIXME(shotgun): 93.98 %, missing/extra + reloc-name; behaviour near.
void cls_804A00C8::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -12.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 12.0f;
    field_0x80.i.x = -6.0f;
    field_0x80.i.y = -6.0f;
    field_0x80.f.x = 6.0f;
    field_0x80.f.y = 6.0f;
    mBox2.i.x = -10.0f;
    mBox2.i.y = -10.0f;
    mBox2.f.x = 10.0f;
    mBox2.f.y = 10.0f;
    field_0x94 = (field_0x90 >> 16) & 0xFF;
    field_0x280 = field_0x90 >> 24;
    field_0x28c = 0;
    if (field_0x280 == 0) {
        field_0x280 = -1;
    }
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x90 &= 0xFF;
    mLife = lbl_8049FDC8[field_0x90 * 2];
    field_0x274 = 0;
    field_0x278 = 0;
    field_0x298 = 1.0f;
    field_0x2b4 = 0.0f;
    field_0x2b0 = 0.0f;
    field_0x2ac = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x98 = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) & 3;
    field_0x2a8 = 0;
    field_0x238 = 8;
    field_0x284 = -1;
    field_0x27c = lbl_8053AB50;
    field_0x2aa = -1;
    lbl_8053AB50++;
    field_0xb0 = 0x12205;
    if (field_0x94 == 2 || field_0x90 == 3) {
        field_0xb0 = 0x4000000;
    }
    field_0x290 = 0;
    if (field_0x90 == 2 && lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 3) {
        mLife = 1;
        for (s32 i = 0; i < 6; i++) {
            if ((s32)(0.1f * mPos.x) == lbl_8049FFD8.tiles[i][0] && (s32)(0.1f * mPos.y) == lbl_8049FFD8.tiles[i][1]) {
                field_0x290 = i;
                break;
            }
        }
    }
    field_0x2a4 = 0.7f;
    if (field_0x94 == 1) {
        field_0x2a4 = 1.4f;
    }
    field_0x294 = 1.5f * field_0x2a4 + 0.2f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
    field_0x234 = 0;
    switch (field_0x90) {
    case 2:
        field_0xbc = 4;
        field_0xb8 = 1;
        mState = 0xF;
        break;
    case 3:
        field_0x298 = 0.0f;
        field_0x278 = 1;
        mState = 8;
        break;
    case 0:
    case 1:
    default:
        field_0xbc = 2;
        field_0xb8 = 1;
        mState = 0;
        break;
    }
    field_0x29c = 0.0f;
    field_0x2a0 = 1.0f;
    field_0x288 = 0;
    if (field_0x280 >= 0) {
        mState = 0xE;
    }
}

// FIXME(shotgun): 99.91 %, reloc-name; behaviour clean.
cls_804A00C8::~cls_804A00C8() {
    s32 partnerSerial = field_0x280;
    if (partnerSerial >= 0) {
        cls_804A00C8* partner = static_cast<cls_804A00C8*>(getActorManager_801F856C()->findActorBySerial_801F666C(partnerSerial));
        if (partner != NULL) {
            partner->field_0x290--;
        }
    }
    s32 serial = field_0x284;
    if (serial >= 0) {
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (other != NULL) {
            JGeometry::TVec3f kick(0.0f, 0.0f, 5.0f);
            other->field_0x3c = kick;
        }
        field_0x284 = -1;
    }
}

// FIXME(shotgun): 99.89 %, reloc-name; behaviour clean.
void cls_804A00C8::vfunc_0x44() {
    s32 serial = field_0x284;
    if (serial >= 0) {
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (other != NULL) {
            JGeometry::TVec3f kick(0.0f, 0.0f, 5.0f);
            other->field_0x3c = kick;
        }
        field_0x284 = -1;
    }
    if (mLife <= 0) {
        s32 partnerSerial = field_0x280;
        if (partnerSerial >= 0) {
            cls_804A00C8* partner = static_cast<cls_804A00C8*>(getActorManager_801F856C()->findActorBySerial_801F666C(partnerSerial));
            if (partner != NULL) {
                partner->field_0x290--;
            }
        }
    }
}

// FIXME(shotgun): 99.82 %, reloc-name; behaviour clean.
// only (literal pool order)
bool cls_804A00C8::vfunc_0x54() {
    if (mState == 0xD) {
        return true;
    }
    if (mPos.z > 2.0f) {
        return false;
    }
    field_0x2b8.x = mPos.x;
    field_0x2b8.y = mPos.y;
    field_0x2b8.z = mPos.z;
    field_0x2b8.x = (f32)((s32)field_0x2b8.x & 0xFFF8);
    field_0x2b8.y = (f32)((s32)field_0x2b8.y & 0xFFF8);
    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    field_0x2b8.x += -12.0f * sincos.sinShort(fn_801F6874(field_0x98));
    field_0x2b8.y += -12.0f * sincos.cosShort(fn_801F6874(field_0x98));
    s32 kind = fn_8022461C(&field_0x2b8, field_0x4);
    if (kind == 2 || kind == 3 || kind == 0xE) {
        if (kind == 0xE && fn_802DE9A0() == true) {
            field_0x2a8 = 1;
        }
        field_0xd8 = 0;
        field_0xdc = 0;
        field_0x3c.x = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x2ac = 0.0f;
        field_0x2b0 = 0.0f;
        field_0x2b4 = 0.0f;
        field_0x254 = 10000;
        mState = 0xC;
        mLife = 200;
        return true;
    }
    return false;
}

// FIXME(shotgun): 85.33 %, missing/extra + reloc-name; behaviour drift.
void cls_804A00C8::update() {
    JGeometry::TVec3f pos = mPos;
    u8 moved = 0;
    bool flag = true;
    u16 angle = 0;
    f32 dist = 0.0f;
    s32 dir = 0;

    if (field_0x94 == 2) {
        if (field_0x2aa == -1) {
            if (getGameView_802D6A9C()->fn_802DCD0C()) {
                return;
            }
            JGeometry::TVec3f tmpPos;
            tmpPos.x = mPos.x;
            tmpPos.y = mPos.y;
            field_0x2aa = fn_80226998(&tmpPos, field_0x4);
            return;
        }
        JGeometry::TVec3f tmpPos;
        tmpPos.x = mPos.x;
        tmpPos.y = mPos.y;
        if ((u16)field_0x2aa == fn_80226998(&tmpPos, field_0x4)) {
            return;
        }
        field_0x254 = 40;
        field_0x94 = 0;
    }

    if (field_0x254 == 1) {
        field_0xb0 = 0x12205;
        if (field_0x90 == 3) {
            field_0xb0 = 0x12202;
        }
    }

    if (field_0x90 == 3) {
        if (fn_8022461C(&mPos, field_0x4) != 0x21 && !fn_8022517C(&mPos, field_0x4)) {
            field_0x298 = 1.0f;
            field_0x278 = 0;
            field_0x94 = 0;
            field_0xb0 = 0x12205;
            field_0x90 = 0;
            field_0xbc = 2;
            field_0xb8 = 1;
            mState = 0;
        }
    }

    if (mState == 8 || mState == 11) {
        flag = false;
    } else if (field_0x90 == 3) {
        flag = false;
    }

    if (fn_801F3D94(flag) != 0) {
        if (field_0x90 == 3) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x2b4 = 0.0f;
            field_0x2b0 = 0.0f;
            field_0x2ac = 0.0f;
        }
        if (!vfunc_0x54()) {
            fn_801F2618(NULL, false);
        }
        return;
    }

    if (mState == 8 || mState == 11) {
        if (fn_801F2DB4('BLZE', 'NULL', NULL)) {
            fn_801F2618(NULL, false);
            return;
        }
    } else if (field_0x254 == 0) {
        u32 excludeId = 'NULL';
        if (field_0x90 == 2 && field_0x258 == 0 && field_0x284 < 0) {
            fn_80154590(&mBox, -12.0f, -12.0f, 12.0f, 12.0f);
            field_0x284 = fn_801F2718('HBMB');
            fn_80154590(&mBox, -8.0f, -8.0f, 8.0f, 8.0f);
            if (field_0x284 >= 0) {
                Bomb_8049F784* bomb = static_cast<Bomb_8049F784*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x284));
                if (bomb != NULL) {
                    if (bomb->field_0x238 != 0 && (u16)bomb->field_0x94 == 0) {
                        field_0x98 = fn_801F681C((u16)(10430.378f * JMath::atanTable_.atan2_(bomb->mPos.x - mPos.x, bomb->mPos.y - mPos.y)));
                        fn_801583E4(&pos, &mPos);
                        pos.x += lbl_8049FDA8[field_0x98][0];
                        pos.z += lbl_8049FDA8[field_0x98][1];
                        JGeometry::TVec3f zeroVec;
                        fn_801583D4(&zeroVec, 0.0f, 0.0f, 0.0f);
                        bomb->field_0x3c = zeroVec;
                        excludeId = 'HBMB';
                        bomb->mPos = pos;
                        bomb->field_0x90 = 6;
                        bomb->field_0x24c = 0;
                        bomb->field_0x238 = 200;
                        bomb->field_0x240 = 50;
                        bomb->field_0x230 = 0;
                        field_0x238 = 0;
                        field_0x23c = 0;
                        field_0x25c = 1;
                        field_0x260 = 1;
                        field_0x258 = 20;
                    } else {
                        field_0x284 = -1;
                    }
                } else {
                    field_0x284 = -1;
                }
            }
        }
        if (field_0x290 != 2 && field_0x290 != 3 && field_0x290 != 4 && field_0x290 != 5) {
            if (fn_801F2FAC() || fn_801F2DB4('NULL', excludeId, NULL)) {
                if (mLife <= 0) {
                    field_0xbc = 2;
                }
                if (field_0x90 == 3) {
                    field_0x3c.z = 0.0f;
                    field_0x3c.y = 0.0f;
                    field_0x3c.x = 0.0f;
                    field_0x2b4 = 0.0f;
                    field_0x2b0 = 0.0f;
                    field_0x2ac = 0.0f;
                    fn_801F2618(NULL, false);
                    return;
                }
                if (mState != 3 && mState != 12 && mState != 13 && mState != 4 && field_0x90 != 2) {
                    mState = 3;
                }
                if (field_0x284 >= 0) {
                    ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x284);
                    if (bomb != NULL) {
                        JGeometry::TVec3f dropVel;
                        fn_801583D4(&dropVel, 0.0f, 0.0f, 5.0f);
                        bomb->field_0x3c = dropVel;
                    }
                    field_0x284 = -1;
                }
                fn_801F2618(NULL, false);
                return;
            }
        }
    }

    if (mState != 12 && mState != 13) {
        fn_801EEC98(mSerial, &angle, &dir, &dist, &field_0x268, 0, 0);
        if (dir == -1) {
            if (mState == 3 || mState == 4) {
                mState = 0;
            }
            if (mState == 9) {
                mState = 11;
            }
        }
        addCalc2_801F71A4(&field_0x3c.x, field_0x2ac, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x2b0, 0.3f, 1.0f);
        PSVECAdd(&mPos, &field_0x3c, &mPos);
        if (field_0x298 > 0.2f) {
            fn_801F2B7C(-100, -1, 1);
        }
        if (field_0x9c >= 0) {
            startSound_801F0E34(0x40010, 0);
        }
        if (mState == 0 || mState == 4 || mState == 5) {
            moved = fn_801F2618(NULL, false);
        }
    }

    switch (mState) {
    case 0:
        if (moved && field_0x24c == 0) {
            field_0x2b4 = 0.0f;
            field_0x2b0 = 0.0f;
            field_0x2ac = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x238 = 8;
            field_0x98++;
            field_0x98 &= 3;
            field_0x274 = lbl_8049FDC8[80 + field_0x98];
            mState = 1;
            break;
        }
        if (field_0x23c == 0) {
            field_0x2b4 = 0.0f;
            field_0x2b0 = 0.0f;
            field_0x2ac = 0.0f;
            field_0x25c = 0;
            field_0x260 = (s32)(1.99f * fn_80154744()) + 1;
            field_0x238 = 8;
            mState = 2;
            break;
        }
        if (field_0x238 == 0) {
            field_0x25c++;
            field_0x25c &= 3;
            field_0x238 = lbl_8049FDC8[76 + field_0x25c];
        }
        field_0x274 = lbl_8049FDC8[60 + field_0x98 * 4 + field_0x25c];
        if (field_0x280 >= 0 && field_0x288 == 1) {
            mState = 3;
        }
        if (field_0x240 == 0 && dir == (s32)field_0x98 && dist <= 100.0f) {
            mState = 3;
        }
        break;

    case 1:
        if (field_0x238 == 0) {
            switch (field_0x98) {
            case 0:
                field_0x2ac = field_0x2a4;
                field_0x2b0 = 0.0f;
                field_0x2b4 = 0.0f;
                break;
            case 1:
                field_0x2ac = -field_0x2a4;
                field_0x2b0 = 0.0f;
                field_0x2b4 = 0.0f;
                break;
            case 2:
                field_0x2ac = 0.0f;
                field_0x2b0 = field_0x2a4;
                field_0x2b4 = 0.0f;
                break;
            case 3:
                field_0x2ac = 0.0f;
                field_0x2b0 = -field_0x2a4;
                field_0x2b4 = 0.0f;
                break;
            }
            field_0x25c = 0;
            field_0x238 = 20;
            if (field_0x23c < 10) {
                field_0x23c = 10;
            }
            field_0x24c = 10;
            mState = 0;
        }
        break;

    case 2:
        if (field_0x238 == 0) {
            field_0x25c++;
            field_0x25c &= 3;
            field_0x238 = 15;
            if (field_0x260 <= 0) {
                if ((field_0x98 & 2) == 0) {
                    if (fn_80154744() < 0.5f) {
                        field_0x98 = 2;
                    } else {
                        field_0x98 = 3;
                    }
                } else {
                    if (fn_80154744() < 0.5f) {
                        field_0x98 = 0;
                    } else {
                        field_0x98 = 1;
                    }
                }
                switch (field_0x98) {
                case 0:
                    field_0x2ac = field_0x2a4;
                    field_0x2b0 = 0.0f;
                    field_0x2b4 = 0.0f;
                    break;
                case 1:
                    field_0x2ac = -field_0x2a4;
                    field_0x2b0 = 0.0f;
                    field_0x2b4 = 0.0f;
                    break;
                case 2:
                    field_0x2ac = 0.0f;
                    field_0x2b0 = field_0x2a4;
                    field_0x2b4 = 0.0f;
                    break;
                case 3:
                    field_0x2ac = 0.0f;
                    field_0x2b0 = -field_0x2a4;
                    field_0x2b4 = 0.0f;
                    break;
                }
                field_0x23c = (s32)(50.0f * fn_80154744()) + 50;
                field_0x25c = 0;
                field_0x238 = 8;
                mState = 0;
            } else if (field_0x25c == 0) {
                field_0x260--;
            }
        }
        if (dir == lbl_8049FDC8[100 + field_0x98 * 4 + field_0x25c] && dist <= 100.0f) {
            mState = 3;
        }
        field_0x274 = lbl_8049FDC8[84 + field_0x98 * 4 + field_0x25c];
        break;

    case 3:
        field_0x2b4 = 0.0f;
        field_0x2b0 = 0.0f;
        field_0x2ac = 0.0f;
        field_0x25c = 0;
        field_0x260 = 0;
        field_0x264 = 0;
        field_0x268 = 0;
        field_0x26c = 0;
        field_0x270 = 0;
        field_0x238 = 0;
        field_0x23c = 20;
        fn_801583E4(&pos, &mPos);
        pos.y -= 16.0f;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 1, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x40063, 0);
        startSound_801F0E34(0x40009, 0);
        field_0x23c = (s32)(250.0f * fn_80154744()) + 250;
        field_0x240 = (s32)(10.0f * ((f32)(field_0x27c & 7) * fn_80154744())) + 70;
        if (dir != -1) {
            field_0x98 = dir;
        }
        field_0x264 = 0;
        if (field_0x98 == 0) {
            field_0x264 = 0xC000;
        } else if (field_0x98 == 1) {
            field_0x264 = 0x4000;
        } else if (field_0x98 == 2) {
            field_0x264 = 0x8000;
        }
        field_0x294 = 1.5f * field_0x2a4 + 0.2f * fn_80154744();
        mState = 4;
        if (field_0x280 == -2) {
            field_0x240 = (s32)(100.0f * fn_80154744()) + 150;
            mState = 5;
            break;
        }
        // fallthrough

    case 4:
        if (field_0x280 >= 0 && field_0x288 == 1) {
            field_0x240 = 20;
            field_0x244 = 20;
        }
        if (moved) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x264 = (field_0x264 + 0x4000) & 0xF000;
        }
        if (field_0x250 == 0 && field_0x268 >= 0 && fn_8023F264(field_0x268)) {
            if (dir != -1) {
                field_0x98 = dir;
            }
            const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
            f32 s = sincos.sinShort(field_0x264);
            f32 px = getPlayerPos_8023E724(field_0x268)->x + 70.0f * s;
            f32 dx = mPos.x - px;
            f32 c = sincos.cosShort(field_0x264);
            f32 py = getPlayerPos_8023E724(field_0x268)->y + 70.0f * c;
            f32 dy = mPos.y - py;
            dist = fn_80154378(dx * dx + dy * dy);
            angle = (s32)(10430.378f * JMath::atanTable_.atan2_(-dx, -dy));
            if (dir != -1) {
                field_0x98 = dir;
            }
            if (field_0x280 >= 0 && field_0x288 == 1) {
                ActorBase_804A5770* leader = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x280);
                if (leader != NULL) {
                    f32 ldx = leader->mPos.x - mPos.x;
                    f32 ldy = leader->mPos.y - mPos.y;
                    field_0x250 = (s32)((f32)(field_0x27c & 7) * fn_80154744()) + 20;
                    if (fn_80154378(ldx * ldx + ldy * ldy) > 40.0f) {
                        angle = (s32)(10430.378f * JMath::atanTable_.atan2_(ldx, ldy));
                        angle += (s32)(4096.0f - 8192.0f * fn_80154744());
                    }
                }
            }
            field_0x2ac = field_0x294 * sincos.sinShort(angle);
            field_0x2b0 = field_0x294 * sincos.cosShort(angle);
        }
        if ((dist < 2.0f && field_0x244 == 0) || field_0x240 == 0) {
            field_0x25c = 0;
            field_0x2b4 = 0.0f;
            field_0x2b0 = 0.0f;
            field_0x2ac = 0.0f;
            field_0x238 = 8;
            mState = 6;
        }
        break;

    case 5: {
        if (moved) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x234 = fn_801F6874(field_0x98);
            field_0x248 = 40;
            if (field_0x26c == 0) {
                if (fn_80154744() < 0.5f) {
                    field_0x234 -= 0x4000;
                } else {
                    field_0x234 += 0x4000;
                }
                field_0x26c++;
            } else {
                field_0x26c = 0;
                field_0x234 += 0x8000;
            }
        }
        if (field_0x248 == 0) {
            field_0x234 = angle + 0x8000;
            field_0x26c = 0;
        }
        field_0x98 = fn_801F681C(field_0x234);
        const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
        field_0x2ac = 1.5f * sincos.sinShort(field_0x234);
        field_0x2b0 = 1.5f * sincos.cosShort(field_0x234);
        if (field_0x240 == 0) {
            field_0x238 = 20;
            field_0x23c = (s32)(50.0f * fn_80154744()) + 50;
            field_0x240 = (s32)((f32)(field_0x27c & 7) * fn_80154744()) + 30;
            field_0x2b4 = 0.0f;
            field_0x2b0 = 0.0f;
            field_0x2ac = 0.0f;
            mState = 0;
        }
        break;
    }

    case 9:
        if (dir >= 0) {
            field_0x98 = dir;
        }
        if ((field_0x280 < 0 && (dist > 100.0f || dir < 0)) || (field_0x280 >= 0 && field_0x288 == 1)) {
            mState = 11;
            break;
        }
        if (field_0x280 == -2) {
            vfunc_0x24();
            break;
        }
        field_0x23c = 10;
        // fallthrough

    case 6:
        if (field_0x238 != 0) {
            field_0x274 = lbl_8049FDC8[116 + field_0x98 * 4 + field_0x25c];
            break;
        }
        field_0x25c++;
        field_0x25c &= 3;
        field_0x238 = 8;
        switch (field_0x25c) {
        case 0:
            field_0x238 = 15;
            if (field_0x23c == 0) {
                field_0x2b4 = 0.0f;
                field_0x2b0 = 0.0f;
                field_0x2ac = 0.0f;
                field_0x25c = 0;
                field_0x260 = 0;
                field_0x264 = 0;
                field_0x268 = 0;
                field_0x26c = 0;
                field_0x270 = 0;
                field_0x238 = 20;
                field_0x23c = (s32)(50.0f * fn_80154744()) + 50;
                field_0x240 = (s32)((f32)(field_0x27c & 7) * fn_80154744()) + 30;
                mState = 0;
            } else if (field_0x90 != 3) {
                field_0x238 = 0;
                field_0x244 = 10;
                field_0x240 = (s32)(70.0f * ((f32)(field_0x27c & 7) * fn_80154744())) + 70;
                mState = 4;
                if (field_0x280 == -2) {
                    field_0x240 = (s32)(100.0f * fn_80154744()) + 150;
                    mState = 5;
                }
            }
            break;

        case 2: {
            fn_801583E4(&pos, &mPos);
            switch (field_0x98) {
            case 0:
                pos.x += 2.0f;
                if (mState == 9) {
                    pos.y -= 10.0f;
                } else {
                    pos.y -= 4.0f;
                }
                break;
            case 1:
                pos.x -= 2.0f;
                if (mState == 9) {
                    pos.y -= 10.0f;
                } else {
                    pos.y -= 4.0f;
                }
                break;
            case 2:
                pos.x -= 2.0f;
                break;
            case 3:
                pos.x += 2.0f;
                pos.y -= 10.0f;
                break;
            }
            s32 bulletSerial = getActorManager_801F856C()->spawnActor_801F9484('OKTM', field_0x4, &pos, 1, -1, -1);
            if (bulletSerial >= 0) {
                Bomb_8049F784* bullet = static_cast<Bomb_8049F784*>(getActorManager_801F856C()->findActorBySerial_801F666C(bulletSerial));
                startSound_801F0E34(0x40085, 0);
                startSound_801F0E34(0x4000D, 0);
                if (bullet != NULL) {
                    JGeometry::TVec3f bulletVel;
                    switch (field_0x98) {
                    case 0:
                        bulletVel.y = 0.0f;
                        bulletVel.x = 3.0f;
                        bulletVel.z = 0.0f;
                        break;
                    case 1:
                        bulletVel.y = 0.0f;
                        bulletVel.x = -3.0f;
                        bulletVel.z = 0.0f;
                        break;
                    case 2:
                        bulletVel.x = 0.0f;
                        bulletVel.y = 3.0f;
                        bulletVel.z = 0.0f;
                        break;
                    case 3:
                        bulletVel.x = 0.0f;
                        bulletVel.y = -3.0f;
                        bulletVel.z = 0.0f;
                        break;
                    }
                    bullet->field_0x98 = field_0x98;
                    bullet->field_0x3c = bulletVel;
                    bullet->field_0x23c = 5;
                }
            }
            break;
        }

        case 3:
            if (mState == 9) {
                field_0x270++;
                if (field_0x270 >= 3) {
                    field_0x270 = 0;
                    field_0x238 = 50;
                }
            }
            break;
        }
        field_0x274 = lbl_8049FDC8[116 + field_0x98 * 4 + field_0x25c];
        break;

    case 7:
        if (dist <= 40.0f) {
            mState = 3;
        }
        break;

    case 8:
        if (field_0x248 == 1) {
            field_0x278 = 1;
        }
        if (field_0x280 == -2) {
            vfunc_0x24();
            break;
        }
        if (dir >= 0) {
            if ((field_0x248 == 0 && dist < 80.0f && field_0x280 < 0) || (field_0x280 >= 0 && field_0x288 == 0)) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x358, 0, 1, NULL, NULL, true, 0.15f);
                field_0x278 = 0;
                field_0xb0 = 0x12202;
                startSound_801F0E34(0x40067, 0);
                mState = 10;
            }
        }
        break;

    case 10:
        addCalc2_801F71A4(&field_0x298, 1.0f, 0.3f, 1.0f);
        if (field_0x298 > 0.9f) {
            field_0x298 = 1.0f;
            field_0x270 = 0;
            mState = 9;
        }
        break;

    case 11:
        field_0xb0 = 0x4000000;
        addCalc2_801F71A4(&field_0x298, 0.0f, 0.3f, 1.0f);
        if (field_0x298 < 0.1f) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x358, 0, 1, NULL, NULL, true, 0.15f);
            startSound_801F0E34(0x40067, 0);
            field_0x298 = 0.1f;
            field_0x248 = 10;
            mState = 8;
        }
        break;

    case 12:
        field_0x2b4 = 0.0f;
        field_0x2b0 = 0.0f;
        field_0x2ac = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        field_0x29c = 0.0f;
        field_0x238 = 0;
        field_0x23c = 30;
        field_0xb0 = 0x4000000;
        mLife = 10000;
        mState = 13;
        // fallthrough

    case 13:
        field_0xd8 = 0;
        field_0xdc = 0;
        addCalc2_801F71A4(&mPos.x, field_0x2b8.x, 0.1f, 0.5f);
        addCalc2_801F71A4(&mPos.y, field_0x2b8.y, 0.1f, 0.5f);
        if (field_0x23c == 0 && fn_80154378((mPos.x - field_0x2b8.x) * (mPos.x - field_0x2b8.x) + (mPos.y - field_0x2b8.y) * (mPos.y - field_0x2b8.y)) < 2.0f) {
            if (field_0x90 == 2) {
                if (field_0x274 != 20) {
                    startSound_801F0E34(0x4001A, 0);
                    field_0x274 = 20;
                }
            } else {
                if (field_0x274 != 24) {
                    startSound_801F0E34(0x4001A, 0);
                    field_0x274 = 24;
                }
            }
            field_0x29c += 0.1f;
            if (fn_802DEB58() == true) {
                if (field_0x29c > 1.0f) {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, -1, 0, NULL, NULL, true, 1.0f);
                    startSound_801F0E34(0x30047, 0);
                    fn_801F3668();
                }
                return;
            }
            field_0x2a0 -= 0.02f;
            if (field_0x2a0 < 0.1f) {
                if (field_0x2a8 == 1) {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, -1, 0, NULL, NULL, true, 1.0f);
                    startSound_801F0E34(0x5005F, 0);
                }
                fn_801F3668();
            }
        } else if (field_0x238 == 0 && field_0x90 != 2) {
            field_0x25c++;
            field_0x25c &= 3;
            field_0x238 = (s32)(0.5f * (f32)lbl_8049FDC8[76 + field_0x25c]);
            field_0x274 = lbl_8049FDC8[60 + field_0x98 * 4 + field_0x25c];
        }
        break;

    case 14: {
        mState = 0;
        ActorBase_804A5770* list[1024];
        s32 count = getActorManager_801F856C()->fn_801F7818('SLR4', field_0x4, list, -1);
        for (s32 i = 0; i < count; i++) {
            ChiefSoldier_804A0B90* chief = static_cast<ChiefSoldier_804A0B90*>(list[i]);
            if (chief != NULL && field_0x280 == chief->field_0x28c) {
                field_0x280 = chief->mSerial;
                chief->field_0x290++;
                if (chief->field_0x90 == 1) {
                    field_0x28c = 1;
                }
                if (field_0x90 == 2) {
                    mState = 15;
                    return;
                }
                if (field_0x90 == 3) {
                    mState = 8;
                }
                return;
            }
        }
        field_0x280 = -1;
        if (field_0x90 == 2) {
            mState = 15;
        } else if (field_0x90 == 3) {
            mState = 8;
        }
        break;
    }

    case 15:
        if (field_0x260 == 0) {
            if (field_0x23c == 0 && dir == lbl_8049FDC8[44 + field_0x98 * 4 + field_0x25c] && dist <= 100.0f) {
                field_0x98 = dir;
                field_0x238 = 15;
                field_0x23c = 0;
                field_0x25c = 0;
                field_0x260 = 1;
                break;
            }
            if (field_0x238 == 0) {
                field_0x238 = 15;
                field_0x274 = lbl_8049FDC8[28 + field_0x98 * 4 + field_0x25c];
                field_0x25c++;
                field_0x25c &= 3;
                if (field_0x25c == 0) {
                    field_0x98 ^= 2;
                    if (fn_80154744() < 0.5f) {
                        field_0x98 ^= 1;
                    }
                }
            }
        } else if (field_0x238 == 0) {
            field_0x238 = lbl_8049FDC8[24 + field_0x25c];
            field_0x274 = lbl_8049FDC8[8 + field_0x98 * 4 + field_0x25c];
            if (field_0x25c == 1) {
                fn_801583E4(&pos, &mPos);
                pos.x += lbl_8049FDA8[field_0x98][0];
                pos.z += lbl_8049FDA8[field_0x98][1];
                if (field_0x284 < 0) {
                    field_0x284 = getActorManager_801F856C()->spawnActor_801F9484('HBMB', field_0x4, &pos, 6, -1, -1);
                    if (field_0x284 >= 0) {
                        ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x284);
                        if (bomb != NULL) {
                            bomb->field_0x98 = field_0x98;
                        }
                    }
                } else {
                    field_0x238 += 10;
                }
            }
            if (field_0x25c == 2 && field_0x284 >= 0) {
                ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x284);
                f32 speed = 1.4f + fn_80154744();
                if (bomb != NULL) {
                    if (lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 3) {
                        JGeometry::TVec3f vel;
                        vel.x = lbl_8049FFD8.vel[field_0x290][field_0x98][0];
                        if (777.0f == vel.x) {
                            vel.x = speed;
                            if (field_0x98 == 1) {
                                vel.x = speed * -1.0f;
                            }
                        }
                        vel.z = 3.0f;
                        vel.y = lbl_8049FFD8.vel[field_0x290][field_0x98][1];
                        bomb->field_0x3c = vel;
                    } else {
                        switch (field_0x98) {
                        case 0: {
                            JGeometry::TVec3f vel;
                            fn_801583D4(&vel, speed, 0.0f, 3.0f);
                            bomb->field_0x3c = vel;
                            break;
                        }
                        case 1: {
                            JGeometry::TVec3f vel;
                            fn_801583D4(&vel, -speed, 0.0f, 3.0f);
                            bomb->field_0x3c = vel;
                            break;
                        }
                        case 2: {
                            JGeometry::TVec3f vel;
                            fn_801583D4(&vel, 0.0f, speed, 3.0f);
                            bomb->field_0x3c = vel;
                            break;
                        }
                        case 3: {
                            JGeometry::TVec3f vel;
                            fn_801583D4(&vel, 0.0f, -speed, 3.0f);
                            bomb->field_0x3c = vel;
                            break;
                        }
                        }
                    }
                    startSound_801F0E34(0x40086, 0);
                    startSound_801F0E34(0x40008, 0);
                }
                field_0x258 = 20;
                field_0x284 = -1;
            }
            field_0x25c++;
            if (field_0x25c > 2) {
                field_0x25c = 0;
                field_0x260 = 0;
                field_0x23c = 90;
            }
        }
        break;
    }

    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    if (field_0x240 > 0) {
        field_0x240--;
    }
    if (field_0x244 > 0) {
        field_0x244--;
    }
    if (field_0x248 > 0) {
        field_0x248--;
    }
    if (field_0x24c > 0) {
        field_0x24c--;
    }
    if (field_0x250 > 0) {
        field_0x250--;
    }
    if (field_0x254 > 0) {
        field_0x254--;
    }
    if (field_0x258 > 0) {
        field_0x258--;
    }

    if (mState != 13) {
        if (((u32)(mState - 3) <= 1 || mState == 5) && (!moved || mState != 5)) {
            if (field_0x238 == 0) {
                field_0x25c++;
                field_0x25c &= 3;
                field_0x238 = (s32)(0.5f * (f32)lbl_8049FDC8[76 + field_0x25c]);
            }
            field_0x274 = lbl_8049FDC8[60 + field_0x98 * 4 + field_0x25c];
        }
        if (field_0x280 >= 0) {
            ChiefSoldier_804A0B90* leader = static_cast<ChiefSoldier_804A0B90*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x280));
            if (leader != NULL) {
                if (leader->field_0x288 == 0) {
                    field_0x288 = 0;
                }
                if (leader->field_0x288 == 1) {
                    field_0x288 = 1;
                    if (field_0x90 == 3 && mState == 9) {
                        mState = 11;
                    }
                }
            } else {
                field_0x280 = -2;
                if (field_0x28c == 1) {
                    vfunc_0x24();
                }
            }
        }
    }
}

// FIXME(shotgun): 95.63 %, missing/extra + reloc-name; behaviour drift.
void cls_804A00C8::draw() {
    if (field_0x94 == 2 || field_0x278 != 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 3 && field_0x90 == 2) {
        params.field_0x0[2] = 4;
    }
    if (mState == 0xD) {
        f32 yaw = field_0x29c;
        f32 scale = field_0x2a0;
        f32 sx = sin(0.0f);
        f32 sy = sin(yaw);
        f32 sz = sin(0.0f);
        f32 cx = cos(0.0f);
        f32 cy = cos(yaw);
        f32 cz = cos(0.0f);
        mtx[0][0] = scale * (cy * cz);
        mtx[0][1] = scale * (sx * sy * cz - cx * sz);
        mtx[0][2] = scale * (cx * cz * sy + sx * sz);
        mtx[1][0] = scale * (cy * sz);
        mtx[1][1] = scale * (sx * sy * sz + cx * cz);
        mtx[1][2] = scale * (cx * sz * sy - sx * cz);
        mtx[2][0] = scale * -sy;
        mtx[2][1] = scale * (sx * cy);
        mtx[2][2] = scale * (cx * cy);
        params.mMtx = mtx;
        if (field_0x90 == 2) {
            drawSprite_801F06F0("enemy_gc_sob.bin", field_0x274 + 0x114, &params, 0, 1.0f, 1.0f);
        } else if (field_0x29c == 0.0f) {
            s32 frame = field_0x274 + 0x4F;
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
            }
        } else {
            s32 frame = field_0x274 + 0x8F;
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
            }
        }
        return;
    }
    if (field_0x90 == 3) {
        f32 scale = field_0x298;
        mtx[0][0] = scale;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = scale;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = scale;
        params.mMtx = mtx;
    }
    if (field_0x90 == 2) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x274 + 0x114, &params, 0, 1.0f, 1.0f);
    } else if (field_0x90 != 3) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x274 + 0x4F, &params, 0, 1.0f, 1.0f);
    }
    if (field_0x90 == 3) {
        JGeometry::TVec3f pos = mPos;
        mPos.y -= 6.0f;
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x274 + 0x4F, &params, -1, 1.0f, 1.0f);
        mPos = pos;
        mtx[0][0] = 1.3f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.3f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.3f;
        params.mMtx = mtx;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, 0x6F, &params, 1.0f, 1.0f);
        }
    }
}
