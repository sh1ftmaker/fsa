#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "std/math.h"

// 0x801A6160.
// FIXME(shotgun): 96.9 %, different
s32 ActorBase_804A5770::fn_801A6160() {
    s32 result = 0;
    JGeometry::TBox2f box = mBox2;
    JGeometry::TVec2f pos(mPos.x, mPos.y - mPos.z);
    s32 timer = field_0xa8;
    if (timer != 0) {
        if (timer == 30) {
            f32 scale = fn_801EE4F0();
            if (scale < 1.0f) {
                scale = 1.0f;
            }
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x17C, 0, 1, NULL, NULL, true, scale);
        }
        if (timer - 1 <= 0) {
            vfunc_0x24();
        }
        field_0xa8 = timer - 1;
        return 1;
    }
    if (mLife > 0 && fn_802242F4(this)) {
        return 1;
    }
    if (field_0xb0 == 0 || mLife <= 0) {
        return 0;
    }
    box.addPos(pos);
    lbl_8053A100 = -1;
    s32 hit;
    if ((hit = fn_801A5400(&box, &pos)) != 0) {
        result = 3;
    } else if ((hit = fn_801A4D74(&box, &pos)) != 0) {
        result = 3;
    } else if ((hit = fn_801A4754(&box, &pos)) != 0) {
        result = 1;
    } else if ((hit = fn_801A41D8(&box, &pos)) == 50) {
        result = 1;
    }
    if (hit == 0xFF) {
        return 0;
    }
    if (result <= 1 && fn_801A4FB4() != 0) {
        result = 2;
    }
    if (field_0xa4 != 0) {
        s32 count = field_0xa4;
        if (mPos.z > 0.0f) {
            field_0x3c = JGeometry::TVec3f(field_0x3c.x, field_0x3c.y, 0.0f);
            mPos = JGeometry::TVec3f(mPos.x, mPos.y, mPos.z - 1.0f);
        } else {
            mPos = JGeometry::TVec3f(mPos.x, mPos.y, 0.0f);
        }
        field_0xa4 = count - 1;
        result = 1;
        if (fn_801F305C()) {
            if (field_0xbc == 3) {
                field_0x3c = JGeometry::TVec3f(0.0f, 0.0f, 0.0f);
            }
            result = 2;
            fn_801F2618(NULL, false);
        }
    }
    if (result > 1) {
        field_0xa4 = 0;
    }
    return result;
}

extern "C" {

// 0x801A6108.
s32 fn_801A6108(s32 serial, const JGeometry::TBox2f* box, JGeometry::TVec2f* pos) {
    return getActorManager_801F856C()->findActorBySerial_801F666C(serial)->fn_801A5400(box, pos);
}

} // extern "C"

// FIXME(shotgun): fn_801A5400 (0x801A5400, 0xD08 bytes) not written. It reads and writes fields
// past ActorBase_804A5770 of the actor fn_8023B2B0(player) names (0x270) and of the OKTM actor it
// spawns (0x230), whose classes are not declared yet.

// 0x801A4FB4.
// FIXME(shotgun): 97.1 %, missing/extra
s32 ActorBase_804A5770::fn_801A4FB4() {
    s32 kind = (field_0xb0 >> 12) & 0xF;
    if (kind == 0) {
        return 0;
    }
    if (field_0xd4 == 0) {
        f32 range = 0.0f;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i) && fn_8023A51C(i) && mPos.z == 0.0f) {
                if (fn_8023725C(i) == 1) {
                    range = 50.0f;
                } else if (fn_8023725C(i) == 2) {
                    range = 777.0f;
                }
                if (range > 0.0f) {
                    f32 dx = fn_8023A478(i)->x - mPos.x;
                    f32 dy = fn_8023A478(i)->y - mPos.y;
                    if (std::sqrtf(dx * dx + dy * dy) < range || 777.0f == range) {
                        field_0xd4 = 1;
                        if (i >= 0) {
                            lbl_8053A100 = i;
                        }
                        vfunc_0x44();
                    }
                }
            }
        }
    }
    if (field_0xd4 == 1) {
        f32 z = 0.0f;
        switch (kind) {
        case 1:
            z = 5.0f + (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            break;
        case 2:
            z = 2.0f + (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            break;
        case 3:
            if (mLife - 10 <= 0) {
                mLife = 0;
            }
            field_0xd4 = 0;
            break;
        }
        field_0x3c = JGeometry::TVec3f(field_0x3c.x, field_0x3c.y, z);
        field_0xd4 = 2;
    }
    if (field_0xd4 == 2) {
        mPos = JGeometry::TVec3f(mPos.x, mPos.y, mPos.z + field_0x3c.z);
        field_0x3c = JGeometry::TVec3f(field_0x3c.x, field_0x3c.y, field_0x3c.z - 0.5f);
        if (mPos.z <= 0.0f) {
            field_0x3c = JGeometry::TVec3f(field_0x3c.x, field_0x3c.y, 0.0f);
            mPos = JGeometry::TVec3f(mPos.x, mPos.y, 0.0f);
            field_0xd4 = 0;
            if (((field_0xb0 >> 16) & 0xFF) == 1) {
                fn_801F3780(300);
            }
        }
    }
    return field_0xd4;
}

extern "C" {

// 0x801A4F5C.
s32 fn_801A4F5C(s32 serial, const JGeometry::TBox2f* box, JGeometry::TVec2f* pos) {
    return getActorManager_801F856C()->findActorBySerial_801F666C(serial)->fn_801A4D74(box, pos);
}

} // extern "C"

// Not auto-inlined: fn_801A4F5C and fn_801A6160 call fn_801A4D74 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// 0x801A4D74.
// FIXME(shotgun): 83.7 %, missing/extra
s32 ActorBase_804A5770::fn_801A4D74(const JGeometry::TBox2f* box, JGeometry::TVec2f* pos) {
    s32 kind = (field_0xb0 >> 8) & 0xF;
    if (kind == 0) {
        return 0;
    }
    if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0 && mPos.z > 32.0f) {
        return 0;
    }
    s32 player = fn_8023AA18(box, field_0x4, pos);
    if (player < 0) {
        return 0;
    }
    if (kind == 3) {
        return 0xFF;
    }
    if (field_0xd8 == 0) {
        s32 life = mLife - fn_8023B520(player);
        field_0x10c = player;
        if (life <= 0) {
            life = 0;
            if (field_0xbc != 3) {
                fn_801A3C50(player);
                return 40;
            }
        }
        mLife = life;
        fn_801F37AC(0, getPlayerPos_8023E724(player), 0, 4, -1);
        if (field_0xbc == 3) {
            field_0x3c = JGeometry::TVec3f(0.0f, 0.0f, 0.0f);
        }
        if (player >= 0) {
            lbl_8053A100 = player;
        }
        vfunc_0x44();
    }
    startSound_801F0E34(0x4001E, 0);
    return 30;
}
#pragma auto_inline reset

extern "C" {

// 0x801A4D1C.
s32 fn_801A4D1C(s32 serial, const JGeometry::TBox2f* box, JGeometry::TVec2f* pos) {
    return getActorManager_801F856C()->findActorBySerial_801F666C(serial)->fn_801A4754(box, pos);
}

} // extern "C"

// 0x801A4754.
// FIXME(shotgun): 84.6 %, missing/extra. The actor fn_8023B22C(player) names (the player's field_0xb18, a
// BOME actor) has an s32 at 0x254 that this function reads and writes (kind 6, cases 1/4 and 8);
// that class is not declared yet, so those accesses are not written.
s32 ActorBase_804A5770::fn_801A4754(const JGeometry::TBox2f* box, JGeometry::TVec2f* pos) {
    s32 kind = (field_0xb0 >> 16) & 0xFF;
    if (kind == 0) {
        return 0;
    }
    if ((kind == 2 || kind == 7 || kind == 8) && field_0xa0 >= 0) {
        ActorBase_804A5770* holder = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xa0);
        if (holder != NULL) {
            JGeometry::TVec3f p = holder->mPos;
            s32 hit = fn_8023AC74(box, field_0x4, pos);
            if (hit >= 0) {
                lbl_8053A100 = hit;
            }
            vfunc_0x44();
            if (kind == 8 && holder->field_0x4 != 0) {
                field_0xb0 = field_0xb4;
                field_0xa0 = -1;
                return 0;
            }
            if (mSerial == 0) {
                p.x += 3.5f - (s32)(mSerial & 7);
                p.y += 3.5f - (s32)(mSerial & 7);
            } else {
                p.x -= 3.5f - (s32)(mSerial & 7);
                p.y -= 3.5f - (s32)(mSerial & 7);
            }
            mPos = p;
            field_0x4 = holder->field_0x4;
            if (kind == 2) {
                return 50;
            }
            return 0xFF;
        }
        if (kind == 7 || kind == 2 || kind == 8) {
            field_0xb0 = field_0xb4;
        }
        field_0xa0 = -1;
        return 0;
    }
    if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0 && mPos.z > 32.0f) {
        return 0;
    }
    s32 player = fn_8023AC74(box, field_0x4, pos);
    if (player < 0) {
        return 0;
    }
    s32 result = 0;
    ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(fn_8023B22C(player));
    if (other == NULL) {
        return 0;
    }
    if (kind == 6) {
        kind = 4;
        bool turned = false;
        s32 face = field_0x98;
        if ((face ^ 1) != fn_801F681C((s32)(JMath::atanTable_.atan2_(-(mPos.x - other->mPos.x),
                                                                     -(mPos.y - other->mPos.y)) *
                                            10430.378f))) {
            turned = true;
        }
        // FIXME(shotgun): the original also sets turned when other's s32 at 0x254 is nonzero.
        if (!turned) {
            kind = 1;
        }
        if (field_0xa4 != 0) {
            kind = 1;
        }
    }
    if (player >= 0) {
        lbl_8053A100 = player;
    }
    vfunc_0x44();
    switch (kind) {
    case 4:
        if (field_0x1dc.fn_801EAC20(player)) {
            startSound_801F0E34(0x400D1, 0);
        }
        // fall through
    case 1:
        // FIXME(shotgun): the original stores 1 in other's s32 at 0x254 here (other != NULL).
        field_0xd8 = 0;
        field_0xdc = 0;
        // fall through
    case 5:
        result = 0xFF;
        if (kind == 1) {
            if (mPos.z == 0.0f) {
                field_0x3c = JGeometry::TVec3f(field_0x3c.x, field_0x3c.y,
                                               4.0f + (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
                mPos = JGeometry::TVec3f(mPos.x, mPos.y, mPos.z + field_0x3c.z);
            }
            fn_801F3780(300);
            result = 50;
            startSound_801F0E34(0x30021, 0);
        }
        break;
    case 2:
        result = 50;
        field_0xa0 = fn_8023B22C(player);
        field_0xb4 = field_0xb0;
        field_0xb0 = 0x20000;
        vfunc_0x40();
        break;
    case 7:
        result = 0xFF;
        if (!field_0xc4) {
            field_0xa0 = fn_8023B22C(player);
            field_0xb4 = field_0xb0;
            field_0xb0 = 0x70000;
            vfunc_0x40();
        }
        break;
    case 8:
        if (other != NULL) {
            bool own = false;
            s32 idx = field_0x94 - 1;
            if (fn_8022F514(player)) {
                if (idx == fn_8022F3DC(player)) {
                    own = true;
                }
            } else if (idx == player) {
                own = true;
            }
            // FIXME(shotgun): the original then, when (field_0x94 == 0 || own == 1) and other's s32
            // at 0x254 is not 2, stores 2 there, sets field_0xa0 = fn_8023B22C(player), calls
            // vfunc_0x40 and sets field_0xb4 = field_0xb0, field_0xb0 = 0x80000. Not written.
        }
        result = 0xFF;
        break;
    case 3:
        field_0x10c = player;
        fn_801F4694();
        vfunc_0x24();
        break;
    }
    return result;
}

extern "C" {

// 0x801A46FC.
s32 fn_801A46FC(s32 serial, const JGeometry::TBox2f* box, JGeometry::TVec2f* pos) {
    return getActorManager_801F856C()->findActorBySerial_801F666C(serial)->fn_801A41D8(box, pos);
}

} // extern "C"

// 0x801A41D8.
// FIXME(shotgun): 95.8 %, missing/extra
s32 ActorBase_804A5770::fn_801A41D8(const JGeometry::TBox2f* box, JGeometry::TVec2f* pos) {
    s32 player = fn_8023AD54(box, field_0x4, pos);
    if (player < 0) {
        return 0;
    }
    if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0 && mPos.z > 32.0f) {
        return 0;
    }
    ActorBase_804A5770* weapon = getActorManager_801F856C()->findActorBySerial_801F666C(fn_8023B358(player));
    if (weapon == NULL || weapon->mLife == 0) {
        return 0;
    }
    bool byPlayer = false;
    s32 kind;
    if (weapon->field_0x94 != 0) {
        kind = (field_0xb0 >> 16) & 0xFF;
    } else {
        byPlayer = true;
        kind = field_0xb0 & 0xFF;
    }
    if (kind == 0) {
        return 0;
    }
    s32 result = 0;
    if (player >= 0) {
        lbl_8053A100 = player;
    }
    vfunc_0x44();
    if (!byPlayer) {
        if (kind == 6) {
            kind = 4;
            bool turned = false;
            s32 face = field_0x98;
            if ((face ^ 1) != fn_801F681C((s32)(JMath::atanTable_.atan2_(-(mPos.x - weapon->mPos.x),
                                                                         -(mPos.y - weapon->mPos.y)) *
                                                10430.378f))) {
                turned = true;
            }
            if (!turned) {
                kind = 1;
            }
            if (field_0xa4 != 0) {
                kind = 1;
            }
        }
        switch (kind) {
        case 0:
        case 4:
        case 6:
        default:
            for (s32 i = 0; i < 4; i++) {
                s32 serial = fn_8023B358(i);
                if (serial >= 0) {
                    ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (other != NULL) {
                        other->mLife = 0;
                    }
                }
            }
            // fall through
        case 2:
        case 5:
            result = 0xFF;
            break;
        case 1:
            if (mPos.z == 0.0f) {
                field_0x3c = JGeometry::TVec3f(field_0x3c.x, field_0x3c.y,
                                               4.0f + (1.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
                mPos = JGeometry::TVec3f(mPos.x, mPos.y, mPos.z + field_0x3c.z);
            }
            field_0xd8 = 0;
            field_0xdc = 0;
            weapon->mLife = 0;
            fn_801F3780(300);
            startSound_801F0E34(0x30021, 0);
            result = 50;
            break;
        case 7:
            weapon->mAlive = false;
            result = 0xFF;
            mLife = 0;
            break;
        case 3:
            weapon->mAlive = false;
            field_0x10c = player;
            fn_801F4694();
            vfunc_0x24();
            break;
        }
    } else {
        if (kind == 9) {
            kind = 4;
            f32 dx = mPos.x - getPlayerPos_8023E724(player)->x;
            s32 face = field_0x98;
            if (face != fn_801F681C((s32)(JMath::atanTable_.atan2_(-dx, -(mPos.y - getPlayerPos_8023E724(player)->y)) *
                                          10430.378f))) {
                kind = 5;
            }
        }
        switch (kind) {
        case 8:
            mLife = 0;
            result = 0xFF;
            break;
        case 1:
        case 2:
        case 5: {
            s32 life = mLife - fn_8023B5B4(player);
            weapon->mAlive = false;
            if (life <= 0) {
                mLife = 0;
                fn_80235B68(player, mSerial);
                field_0x10c = player;
                result = 20;
            } else {
                mLife = life;
                result = 10;
            }
            break;
        }
        case 4:
        case 6:
        case 11:
        case 16:
        case 17:
        case 18:
            for (s32 i = 0; i < 4; i++) {
                s32 serial = fn_8023B358(i);
                if (serial >= 0) {
                    weapon = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (weapon != NULL) {
                        weapon->mLife = 0;
                    }
                }
            }
            // fall through
        case 0:
        case 3:
        case 9:
        case 10:
        case 12:
        case 13:
        case 14:
        case 15:
        default:
            result = 0xFF;
            break;
        case 7:
            spawnEmitter_8013CC50(weapon->field_0x4, &weapon->mPos, 0x24A, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(weapon->field_0x4, &weapon->mPos, 0x24B, 0, 1, NULL, NULL, true, 1.0f);
            result = 0xFF;
            weapon->mAlive = false;
            mAlive = false;
            break;
        }
        if (result != 0 && (kind == 1 || kind == 2 || kind == 5)) {
            JGeometry::TVec3f from = mPos;
            from -= weapon->field_0x3c;
            fn_801F37AC(0, &from, 3, 0, -1);
        }
    }
    return result;
}

// 0x801A3CFC.
void ActorBase_804A5770::fn_801A3CFC() {
    SpriteParams_801F5AE0 params;
    static JGeometry::TSMtxf sMtx;
    mSpriteParams = params;
    if (field_0xa0 >= 0 && 1.0f != field_0x48.x) {
        params.field_0x30 = 8;
        PSMTXScale(sMtx, field_0x48.x, field_0x48.y, field_0x48.y);
        sMtx.mMtx[0][3] = 0.0f;
        sMtx.mMtx[1][3] = 0.0f;
        sMtx.mMtx[2][3] = 0.0f;
        params.mMtx = sMtx;
        mSpriteParams = params;
    } else if (field_0xa4 != 0) {
        PSMTXTrans(sMtx, 0.0f, 0.0f, 0.0f);
        if (field_0xa4 < 100) {
            if ((field_0xa4 & 2) == 0) {
                sMtx.mMtx[0][3] = 1.0f;
                sMtx.mMtx[1][3] = 0.0f;
                sMtx.mMtx[2][3] = 0.0f;
            } else {
                sMtx.mMtx[0][3] = -1.0f;
                sMtx.mMtx[1][3] = 0.0f;
                sMtx.mMtx[2][3] = 0.0f;
            }
        }
        params.mMtx = sMtx;
        mSpriteParams = params;
    }
}

// 0x801A3C50.
void ActorBase_804A5770::fn_801A3C50(s32 player) {
    mLife = 0;
    field_0x10c = player;
    field_0xa0 = player;
    field_0x48 = JGeometry::TVec3f(1.4f, 0.4f, 0.4f);
    field_0xd8 = 0;
    field_0xdc = 0;
    field_0xa4 = 0;
    fn_801F4694();
    if (field_0x4 >= 8) {
        field_0xa8 = 30;
    } else {
        field_0xa8 = 35;
    }
    lbl_8053A100 = player;
    vfunc_0x44();
}
