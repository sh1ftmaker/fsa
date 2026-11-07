#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/GameGlobals.h"
#include "game/GameAudio.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_cucco.h"

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
