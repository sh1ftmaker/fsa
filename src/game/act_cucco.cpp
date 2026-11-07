#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/GameGlobals.h"
#include "game/GameAudio.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/SpriteParams.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"
#include "game/act/act_cucco.h"
#include "game/act/act_magic_circle.h"
#include "game/act/act_warp.h"

Cucco_804AB8C0::~Cucco_804AB8C0() {
    releaseEmitter_801EE434(mEmitter2f4);
    releaseEmitter_801EE434(mEmitter2f8);
    releaseEmitter_801EE434(mEmitter2fc);
    releaseEmitter_801EE434(mEmitter300);
}

// FIXME(shotgun): 90.1 %, missing/extra
void Cucco_804AB8C0::vfunc_0x10() {
    s32 serial = -1;
    if (field_0x16c == -1) {
        return;
    }
    while (true) {
        s32 next = getActorManager_801F856C()->fn_801F78F4(ACTOR_NIWA, serial);
        serial = next;
        if (next < 0) {
            return;
        }
        ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(next);
        if (other == NULL) {
            continue;
        }
        if (other->mSerial == mSerial) {
            continue;
        }
        if (other->field_0x16c != field_0x16c) {
            continue;
        }
        if (fn_801F6758(&other->mPos, other->field_0x4, 16.0f) && other->field_0xc4 != 1) {
            continue;
        }
        mAlive = false;
        return;
    }
}

// FIXME(shotgun): 91.63 %, missing/extra + reloc-name
void Cucco_804AB8C0::fn_803324A8() {
    s32 outDir = -1;
    f32 outDist = 0.0f;
    u16 outAngle = 0;
    if (field_0x290 == 6 && field_0x230 == 0 && mPos.z < 8.0f && field_0x284 <= 0) {
        fn_801EEC98(mSerial, NULL, &outDir, &outDist, NULL, 0, 0);
        if (outDist < 48.0f && outDir >= 0) {
            if (field_0x2a2 == 1) {
                field_0x240 = (s32)(200.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0xC8;
            } else {
                field_0x240 = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x64;
            }
            if (field_0x2a1 == 0) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x348, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0xD2, 0, 0, NULL, NULL, true, 1.0f);
            }
            startSound_801F0E34(0x40001, 0);
            field_0x238 = 0;
            field_0x258 = 0;
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x2c4[2].set(mPos);
            field_0x294 = field_0x4;
            field_0x230 = 1;
        } else {
            field_0x280 = 0;
        }
    }
    if (field_0x290 == 6 && field_0x230 == 1) {
        field_0x280++;
        if (field_0x240 % 4 == 0 && field_0x290 == 6) {
            f32 dx = mPos.x - field_0x2c4[2].x;
            f32 dy = mPos.y - field_0x2c4[2].y;
            field_0x294 = field_0x4;
            field_0x2c4[2].set(mPos);
            f32 distSq = dx * dx + dy * dy;
            if (field_0x294 != field_0x4 || !(distSq < 5.0f)) {
                field_0x280 = 0;
                return;
            }
        }
        if ((field_0x2a3 == 1 && field_0x280 >= 10) || (field_0x2a3 == 0 && field_0x280 >= 0x2D)) {
            fn_801EEC98(mSerial, &outAngle, &outDir, &outDist, NULL, 0, 0);
            if (outDir >= 0 &&
                ((field_0x2a3 == 0 && outDist < 64.0f) || (field_0x2a3 == 1 && outDist < 96.0f)))
            {
                field_0x238 = 0x32;
                if (field_0x2a2 == 1) {
                    field_0x240 = (s32)(200.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0xC8;
                } else {
                    field_0x240 = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x64;
                }
                if (field_0x2a1 == 0) {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x348, 0, 0, NULL, NULL, true, 1.0f);
                } else {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0xD2, 0, 0, NULL, NULL, true, 1.0f);
                }
                startSound_801F0E34(0x40001, 0);
                f32 speed = 2.0f;
                if (field_0x2a3 == 1) {
                    speed = 4.0f;
                }
                s32 layer = field_0x4;
                if (getGameView_802D6A9C()->field_0x131df[layer] && field_0x2b4 != 0.0f) {
                    field_0x2b8[0] = speed * JMath::sincosTable_.sinShort(outAngle);
                } else {
                    outAngle += (s32)(5461.0f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f));
                    field_0x2b8[0] = speed * JMath::sincosTable_.sinShort(outAngle);
                    field_0x2b8[1] = speed * JMath::sincosTable_.cosShort(outAngle);
                }
            }
            field_0x280 = 0;
        }
    }
}

// FIXME(shotgun): 95.7 %, scheduling+regalloc
u8 Cucco_804AB8C0::fn_80332378() {
    JGeometry::TVec3f out;
    out.z = 0.0f;
    out.y = 0.0f;
    out.x = 0.0f;
    u8 r = fn_801F2618(&out, false);
    if (r == 1) {
        fn_801F11B4();
        s32 layer = field_0x4;
        if (getGameView_802D6A9C()->field_0x131df[layer] == 1 && out.y < 0.0f) {
            field_0x2b8[1] = 0.0f;
        }
    } else {
        if (field_0x270 > 0) {
            if (field_0x270 != field_0x274) {
                field_0x274 = field_0x270;
                field_0x278 = 5;
            } else {
                field_0x278 = field_0x278 - 1;
                if (field_0x278 <= 0) {
                    field_0x270 = 0;
                    field_0x274 = 0;
                }
            }
        }
        s32 layer = field_0x4;
        if (getGameView_802D6A9C()->field_0x131df[layer] == 1 && field_0x2a9 == 1 && field_0x2b4 == 0.0f) {
            field_0x3c.y = 0.0f;
            field_0x2b8[1] = 0.0f;
        }
    }
    return r;
}

// FIXME(shotgun): 63.06 %, missing/extra + reloc-name
void Cucco_804AB8C0::fn_80331E6C() {
    if (field_0x2a4[0] != 1) {
        field_0x284--;
        if (field_0x284 <= 0) {
            f32 dx = field_0x2c4[3].x - mPos.x;
            f32 dy = field_0x2c4[3].y - mPos.y;
            if (JGeometry::TVec2f(dx, dy).length() > 12.0f) {
                field_0x2a4[0] = 1;
            }
        }
        return;
    }
    s32 warpSerial = -1;
    s32 circleSerial = -1;
    if (field_0x230 == 3) {
        return;
    }
    s32 count = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < count; i++) {
        ActorBase_804A5770* other = getActorManager_801F856C()->mActors[i];
        if (other != NULL && (other->mActorId == ACTOR_WARP || other->mActorId == ACTOR_CIRC) &&
            field_0x4 == other->field_0x4)
        {
            f32 dx = other->mPos.x - mPos.x;
            f32 dy = other->mPos.y - mPos.y;
            if (JGeometry::TVec2f(dx, dy).length() < 12.0f) {
                if (other->mActorId == ACTOR_WARP) {
                    if (other->vfunc_0x3C() == 0 || other->vfunc_0x34() == true) {
                        warpSerial = other->mSerial;
                        break;
                    }
                } else if (static_cast<MagicCircle_804A7960*>(other)->field_0x262 == 1) {
                    circleSerial = other->mSerial;
                    break;
                }
            }
        }
    }
    if (warpSerial < 0 && circleSerial < 0) {
        return;
    }
    JGeometry::TVec3f target(0.0f, 0.0f, 0.0f);
    s32 code = 0;
    s32 newLayer = 0;
    if (warpSerial >= 0) {
        Warp_804A6DC8* warp = static_cast<Warp_804A6DC8*>(getActorManager_801F856C()->findActorBySerial_801F666C(warpSerial));
        if (warp == NULL) {
            return;
        }
        target.set(warp->field_0x238);
        if (!getGameView_802D6A9C()->field_0x131df[field_0x4]) {
            target.z = mPos.z;
        }
        u32 arg = warp->field_0x90;
        s32 hi = (arg >> 24) & 7;
        newLayer = hi != 0 ? hi + 7 : hi;
        code = arg & 0x7F;
    } else if (circleSerial > 0) {
        MagicCircle_804A7960* circle = static_cast<MagicCircle_804A7960*>(getActorManager_801F856C()->findActorBySerial_801F666C(circleSerial));
        if (circle == NULL) {
            return;
        }
        target.set(circle->field_0x268);
        if (!getGameView_802D6A9C()->field_0x131df[field_0x4]) {
            target.z = mPos.z;
        }
        s32 hi = (circle->field_0x90 >> 24) & 7;
        newLayer = hi != 0 ? hi + 7 : hi;
        code = 6;
    }
    if (code == 6 || code == 0x14 || code == 4 || code == 0x20 || code == 0x24) {
        if (mPos.z >= 8.0f) {
            return;
        }
    } else if (code == 0 || code == 3 || code == 0x15 || code == 0x23) {
        if (field_0x3c.y >= 0.0f) {
            return;
        }
    }
    if (code == 1 || code == 2) {
        if (field_0x3c.y <= 0.0f) {
            return;
        }
    }
    if (code == 0x17) {
        if (field_0x3c.x <= 0.0f) {
            return;
        }
    }
    if (code == 0x18) {
        if (field_0x3c.x >= 0.0f) {
            return;
        }
    }
    mPos.set(target);
    field_0x4 = newLayer;
    field_0x284 = 0x14;
    field_0x2c4[3].set(target);
    field_0x2c4[1].set(target);
    field_0x2a4[0] = 0;
    field_0x2a4[1] = 0;
    f32 mag = PSVECMag((Vec*)&field_0x2b8);
    if (mag < 1.0f) {
        mag = 1.0f;
    } else if (mag > 3.0f) {
        mag = 3.0f;
    }
    f32 half = mag * 0.5f;
    if (code != 6 && code != 0x14) {
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        field_0x2b8[0] = 0.0f;
        field_0x2b8[1] = 0.0f;
        field_0x2b8[2] = 0.0f;
        field_0x238 = 0x14;
        field_0x258 = 0;
    }
    switch (code) {
    case 0:
    case 3:
        field_0x3c.y = -half;
        field_0x2b8[1] = -mag;
        break;
    case 1:
    case 2:
        field_0x3c.y = half;
        field_0x2b8[1] = mag;
        break;
    case 0x17:
        field_0x3c.x = half;
        field_0x2b8[0] = mag;
        break;
    case 0x18:
        field_0x3c.x = -half;
        field_0x2b8[0] = -mag;
        break;
    }
    if (field_0x23c > 0) {
        field_0x23c = 0x14;
    }
}

// FIXME(shotgun): 96.7 %, scheduling+regalloc
void Cucco_804AB8C0::fn_80331D38() {
    if (mPos.z < 8.0f && field_0x268 > 0) {
        JGeometry::TBox2f box;
        box.i.x = mBox.i.x;
        box.i.y = mBox.i.y;
        box.f.x = mBox.f.x;
        box.f.y = mBox.f.y;
        box.i.x += mPos.x;
        box.f.x += mPos.x;
        box.i.y += mPos.y;
        box.f.y += mPos.y;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && fn_80239C00(i, &box, field_0x4) && fn_8022F9E4(i, mSerial, true, false)) {
                mPos.z = 0.0f;
                field_0x258 = 0;
                field_0x238 = 0;
                field_0x240 = 0;
                field_0x2b8[0] = 0.0f;
                field_0x2b8[1] = 0.0f;
                field_0x2b8[2] = 0.0f;
                field_0x3c.x = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x25c = field_0xc8;
                field_0xb8 = 2;
                field_0x230 = 3;
                break;
            }
        }
    }
}

// FIXME(shotgun): 99.0 %, different (literal pool)
void Cucco_804AB8C0::fn_80331C24() {
    if (field_0x290 == 0 || field_0x290 == 6) {
        if (fn_8022461C(&mPos, field_0x4) == 0x39) {
            if (((field_0x230 == 0 && field_0x258 == 0) || field_0x230 == 1) && mPos.z < 4.0f) {
                startSound_801F0E34(0x4007F, 0);
                if (mEmitter2f8 == NULL) {
                    mEmitter2f8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x4D2, 0, 3, NULL, NULL, true, 1.0f);
                } else {
                    setEmitterPos_8013CB44(mEmitter2f8, &mPos, 0, 3, true);
                }
                return;
            }
        }
        if (mEmitter2f8 != NULL) {
            mEmitter2f8->becomeInvalidEmitter();
            mEmitter2f8 = NULL;
        }
    }
}

// FIXME(shotgun): 99.9 %, reloc-name
void Cucco_804AB8C0::fn_80331B40() {
    if (field_0x2a3 == 1) {
        if (field_0x230 == 1) {
            if (mEmitter2fc == NULL) {
                mEmitter2fc = spawnEmitter_8013CC50(field_0x4, &mPos, 0x116, 0, 3, NULL, NULL, true, 1.0f);
            } else {
                setEmitterPos_8013CB44(mEmitter2fc, &mPos, 0, 3, true);
            }
        } else {
            if (mEmitter2fc != NULL) {
                mEmitter2fc->becomeInvalidEmitter();
                mEmitter2fc = NULL;
            }
            if (mEmitter300 != NULL) {
                mEmitter300->becomeInvalidEmitter();
                mEmitter300 = NULL;
            }
        }
    }
}

// FIXME(shotgun): 99.9 %, reloc-name
void Cucco_804AB8C0::fn_803319B8() {
    const s32 tbl[4] = {0, 1, 0, 1};
    field_0x264 = ((field_0x98 & 1) << 1) + tbl[field_0x254];
    if (field_0x230 == 9) {
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            field_0x264 += 4;
        } else {
            field_0x264 += 8;
        }
    } else if (field_0x2a3 == 1 && field_0x2a8 == 0 && (mPos.z > 8.0f || field_0x230 == 1)) {
        if (field_0x254 == 0) {
            if (field_0x98 & 1) {
                field_0x264 = 13;
            } else {
                field_0x264 = 12;
            }
            startSound_801F0E34(0x40001, 0);
        }
        if (!(field_0x254 & 1)) {
            if (field_0x2a1 == 0) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x348, 0, 0, NULL, NULL, true, 1.0f);
            } else {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0xD2, 0, 0, NULL, NULL, true, 1.0f);
            }
        }
    }
}

// FIXME(shotgun): 74.05 %, missing/extra + reloc-name
bool Cucco_804AB8C0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
        return true;
    }
    if (fn_801F6758(pos, layer, -8.0f)) {
        return true;
    }
    u16 tile = fn_80226850(pos, layer);
    if (tile == 0x19C || tile == 0x19D) {
        return true;
    }
    u16 code = fn_8022461C(pos, layer);
    if (code == 0x20 && mPos.z > 8.0f) {
        return false;
    }
    if ((code >= 0x10 && code <= 0x16) || code == 0x17) {
        if (mPos.z > 8.0f) {
            field_0x270 = 300;
            return false;
        }
    }
    if (code == 0) {
        field_0x2b4 = 0.0f;
        return false;
    }
    bool blocked;
    if (code == 3 && tile != 0xC) {
        blocked = false;
    } else if (code == 0x27 || code == 2 || code == 0x39) {
        blocked = false;
    } else if (code == 0x28 && (s32)pos->y % 16 > 4) {
        blocked = false;
    } else if (code == 0x29 && (s32)pos->y % 16 < 12) {
        blocked = false;
    } else if (code == 0x2C && (s32)pos->y % 16 > 4) {
        blocked = false;
    } else if (code == 0x2D && (s32)pos->y % 16 < 12) {
        blocked = false;
    } else if (code == 0x2A && (s32)pos->x % 16 > 4) {
        blocked = false;
    } else if (code == 0x2B && (s32)pos->x % 16 < 12) {
        blocked = false;
    } else if (code == 0x2E && (s32)pos->x % 16 > 4) {
        blocked = false;
    } else if (code == 0x2F && (s32)pos->x % 16 < 12) {
        blocked = false;
    } else {
        blocked = true;
    }
    if (field_0x270 > 0) {
        if (blocked) {
            field_0x270--;
        }
        return false;
    }
    return blocked;
}

// FIXME(shotgun): 95.86 %, missing/extra + reloc-name
void Cucco_804AB8C0::draw() {
    if (field_0x2a7 == 1 || field_0x288 == 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    s32 flags = 7;
    s32 held = 0;
    s32 tint = 0;
    if (mPos.z == 0.0f) {
        if (fn_8022461C(&mPos, field_0x4) == 0x39) {
            flags = 0;
        }
    }
    params.field_0x20 = 0;
    if (field_0x2a8 == 1) {
        if (field_0x25c >= 0 && field_0x25c <= 3) {
            tint = field_0x25c + 1;
        }
    }
    switch (tint) {
    case 1:
        params.field_0x10.r = 200;
        params.field_0x10.b = 150;
        params.field_0x14.g = 20;
        break;
    case 2:
        params.field_0x10.g = 200;
        params.field_0x10.b = 170;
        params.field_0x14.r = 20;
        break;
    case 3:
        params.field_0x10.r = 180;
        params.field_0x10.g = 200;
        params.field_0x14.b = 20;
        break;
    case 4:
        params.field_0x10.g = 195;
        params.field_0x14.r = 20;
        params.field_0x14.b = 20;
        break;
    }
    if (field_0x230 == 3) {
        params.field_0x1d = 1;
        params.field_0x1c = 1;
        if (field_0x290 != 0 && !(field_0x26c & 8)) {
            params.field_0x0[0xB] = 9;
        }
    } else {
        if (field_0x290 != 0 && field_0x290 != 6) {
            params.field_0x0[0xB] = 9;
        }
        if (field_0x23c != 0) {
            params.field_0x30 = 0x11;
        }
    }
    if (field_0xc4) {
        params.field_0x10.a = field_0x288;
        flags = -1;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
        held = 1;
    }
    if (field_0x230 == 10) {
        if (gGameRoot_8053AAF8->field_0x7f != 1) {
            return;
        }
        params.field_0x1d = fn_8023CB24(field_0x25c) + 1;
        params.field_0x1c = fn_8023CA88(field_0x25c);
        if (field_0x290 == 2) {
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x264 + 0x1D, &params, -1, 1.0f, 1.0f);
            fn_803075AC(mSerial, field_0x4, &mPos, 0x1D, 0, 2, -1, 1, 0, 0);
        } else {
            drawSprite_801F06F0("enemy_both2_sob.bin", field_0x264 + 0x11, &params, -1, 1.0f, 1.0f);
            fn_803075AC(mSerial, field_0x4, &mPos, 0x11, 0, 2, -1, 1, 0, 0);
        }
        return;
    }
    if (field_0x290 == 2 || ((field_0x290 == 0 || field_0x290 == 6 || field_0x290 == 1) && field_0x2a1 == 1)) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x264 + 0x2C7, &params, flags, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x264 + 0x2C7, 0, 2, -1, held, 0, 0);
    } else {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x264 + 0x2AF, &params, flags, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x264 + 0x2AF, 0, 2, -1, held, 0, 0);
    }
}

// A one-player version of ActorBase::fn_801EEC98: the angle, direction, distance and
// player of actor `serial` seen from `player`.
// FIXME(shotgun): 94.38 %, missing/extra + reloc-name
void Cucco_804AB8C0::fn_8032F148(s32 serial, u16* outAngle, s32* outDir, f32* outDist, s32* outPlayer, s32 player) {
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    f32 dist = -1.0f;
    s32 angle = 0;
    s32 found = -1;
    if (actor == NULL) {
        if (outDir != NULL) {
            *outDir = -1;
        }
        if (outPlayer != NULL) {
            *outPlayer = -1;
        }
        return;
    }
    s32 onLayer = 0;
    if (getPlayerLayer_8023DE58(player) < 8 && actor->getField_0x4() == 8) {
        onLayer = 2;
    } else if (actor->getField_0x4() == getPlayerLayer_8023DE58(player)) {
        onLayer = 1;
    }
    if (fn_8023F264(player) && !fn_8022DCD8(player) && onLayer != 0 && !fn_8022F478(player) &&
        !fn_80236B50(player) && !fn_8022D534(player))
    {
        f32 dx;
        f32 dy;
        if (onLayer == 1) {
            dx = actor->mPos.x - getPlayerPos_8023E724(player)->x;
            dy = actor->mPos.y - getPlayerPos_8023E724(player)->y;
        } else {
            s32 layer = getPlayerLayer_8023DE58(player);
            GameView_804A9950* view = getGameView_802D6A9C();
            JGeometry::TBox2f box = layer < 8 ? view->mVisibleBox : view->field_0xdc;
            dx = actor->mPos.x - (getPlayerPos_8023E724(player)->x - box.i.x);
            dy = actor->mPos.y - (getPlayerPos_8023E724(player)->y - box.i.y);
        }
        dist = std::sqrtf(dx * dx + dy * dy);
        angle = (u16)(JMath::atanTable_.atan2_(-dx, -dy) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        found = player;
    }
    if (found < 0) {
        if (outDir != NULL) {
            *outDir = -1;
        }
        if (outPlayer != NULL) {
            *outPlayer = -1;
        }
        return;
    }
    if (outAngle != NULL) {
        *outAngle = angle;
    }
    if (outDir != NULL) {
        *outDir = fn_801F681C((u16)angle);
    }
    if (outDist != NULL) {
        *outDist = dist;
    }
    if (outPlayer != NULL) {
        *outPlayer = found;
    }
}
