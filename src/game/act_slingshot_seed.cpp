#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_slingshot_seed.h"

// The actors whose touch destroys the seed (update's fn_801F26CC list, 'NULL'-terminated).
static const u32 lbl_80471840[] = {'FLME', 'BLZ2', 'NULL'};

// 0x80324AC4.
// FIXME(shotgun): 99.25 %, regalloc-only + reloc-name
void SlingshotSeed_804AB3A8::init() {
    field_0x98 = (field_0x90 >> 4) & 7;
    field_0x94 = (field_0x90 >> 8) & 3;
    field_0x90 = field_0x90 & 3;
    field_0x268 = 0;
    field_0x269 = 0;
    mEmitter26c = NULL;
    field_0x80.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    switch (field_0x98) {
    case 0:
        field_0x3c.x = 5.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 1:
        field_0x3c.x = -5.0f;
        field_0x3c.y = 0.0f;
        field_0x3c.z = 0.0f;
        break;
    case 2:
        field_0x3c.x = 0.0f;
        field_0x3c.y = 5.0f;
        field_0x3c.z = 0.0f;
        break;
    case 3:
        field_0x3c.x = 0.0f;
        field_0x3c.y = -5.0f;
        field_0x3c.z = 0.0f;
        break;
    case 4:
        field_0x3c.x = 3.5f;
        field_0x3c.y = 3.5f;
        field_0x3c.z = 0.0f;
        break;
    case 5:
        field_0x3c.x = -3.5f;
        field_0x3c.y = 3.5f;
        field_0x3c.z = 0.0f;
        break;
    case 6:
        field_0x3c.x = 3.5f;
        field_0x3c.y = -3.5f;
        field_0x3c.z = 0.0f;
        break;
    case 7:
        field_0x3c.x = -3.5f;
        field_0x3c.y = -3.5f;
        field_0x3c.z = 0.0f;
        break;
    }
    field_0x234 = 255.0f;
    field_0x240 = 500;
    field_0x274 = 0.0f;
    s32 layer = field_0x4;
    field_0x278 = getGameView_802D6A9C()->field_0x131df[layer];
    if (field_0x94 == 1) {
        s32 layer2 = field_0x4;
        if (getGameView_802D6A9C()->field_0x131df[layer2] == 0) {
            field_0x3c.z = 0.4f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 0.9f;
        } else {
            field_0x270 = 0.4f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 0.9f;
            field_0x3c.y = field_0x3c.y - (0.2f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 0.6f);
        }
    } else if (field_0x94 == 0) {
        if (mEmitter26c == NULL) {
            mEmitter26c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x35D, 0, 1, NULL, NULL, true, 1.0f);
        }
    }
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    mLife = 10000;
    field_0x230 = 0;
    field_0x26a = 0;
}

// The "hit a wall tile" counter step shared by the tile cases of vfunc_0x4C: while the seed is
// still flying, the 21st contact reports a hit.
static inline bool seedBump(SlingshotSeed_804AB3A8* seed) {
    if (seed->field_0x250 == 0) {
        seed->field_0x264++;
        if (seed->field_0x264 > 0x14) {
            return true;
        }
    }
    return false;
}

// FIXME(shotgun): 84 %, missing/extra
bool SlingshotSeed_804AB3A8::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    u32 ids[2];
    ids[0] = 'TOMI';
    ids[1] = ACTOR_NULL;
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, ids) == 1) {
        return true;
    }
    u16 tile = fn_8022461C(pos, layer);
    u16 code = fn_80226850(pos, layer);
    if ((u16)(tile - 0x10) <= 7 || (code >= 0x140 && code <= 0x15F)) {
        if (field_0x250 == 0) {
            field_0x250 = 1;
            field_0x260 = 6;
            field_0x268 = 1;
            field_0x269 = 1;
            return false;
        }
        if (field_0x94 == 0) {
            field_0x268 = 1;
            if (field_0x268 != field_0x269) {
                return true;
            }
        }
    }
    if (field_0x94 == 0 && field_0x250 == 1) {
        return false;
    }
    if ((u16)(code - 0x230) <= 1 || code == 0x234) {
        field_0x250 = 0;
        return false;
    }
    if (tile != 0 && tile != 4 && tile != 5 && tile != 6 && tile != 7 && tile != 2 &&
        tile != 3 && tile != 0x50 && tile != 0xE && tile != 0xC && tile != 0x27 &&
        tile != 0x28 && tile != 0x29 && tile != 0x2A && tile != 0x2B && tile != 0x2C &&
        tile != 0x2D && tile != 0x2E && tile != 0x2F && tile != 0x20 && tile != 0xD &&
        tile != 0x23 && tile != 0x39 && tile != 0x3A && tile != 0x3B && tile != 0x4C &&
        tile != 0x4D && tile != 0x4E && tile != 0x4F && tile != 0x24) {
        if (tile == 7 || tile == 8 || tile == 0xA) {
            if (field_0x98 == 1) {
                return false;
            }
            return seedBump(this);
        }
        if (tile == 6 || tile == 9 || tile == 0xB) {
            if (field_0x98 == 0) {
                return false;
            }
            return seedBump(this);
        }
        if (tile == 4) {
            if (field_0x98 == 2) {
                return false;
            }
            return seedBump(this);
        }
        if (tile == 5) {
            if (field_0x98 == 3) {
                return false;
            }
            return seedBump(this);
        }
        if ((u16)fn_80226850(pos, layer) == 0x3A || (u16)fn_80226850(pos, layer) == 0x3D) {
            field_0x250 = 0;
            return false;
        }
        if (tile == 0x25) {
            if (field_0x250 == 0 && getGameView_802D6A9C()->field_0x171 == 0) {
                return true;
            }
            field_0x250 = 0;
            return false;
        }
        if (tile == 0x26) {
            if (field_0x250 == 0 && getGameView_802D6A9C()->field_0x171 == 1) {
                return true;
            }
            field_0x250 = 0;
            return false;
        }
        if (field_0x250 == 0 && (tile != 0x22 || getGameView_802D6A9C()->field_0x170 == 1)) {
            return true;
        }
        if (field_0x250 != 0) {
            return false;
        }
    }
    if (field_0x250 == 1) {
        field_0x268 = 1;
        if (field_0x260 < 0) {
            field_0x260 = 0;
            field_0x250 = 0;
        }
    }
    return false;
}

// Destructor at 0x80324570.
SlingshotSeed_804AB3A8::~SlingshotSeed_804AB3A8() {
    releaseEmitter_801EE434(mEmitter26c);
}

// FIXME(shotgun): 95.1 %, missing/extra + reloc-name; NONMATCHING because its literals shift
// init's .sdata2 pairing (99.25 -> 98.86 %).
#ifdef NONMATCHING
// Re-aims after a layer flag change, moves; state 0 flies (its trail emitter, burnt by FLME /
// lit BLZ2, bounces off walls and actors, lands in pits and water, hits players in its box),
// state 1 fades out.
void SlingshotSeed_804AB3A8::update() {
    if (field_0x278 != getGameView_802D6A9C()->field_0x131df[field_0x4]) {
        if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0) {
            switch (field_0x98) {
            case 0:
                field_0x3c.x = 5.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                break;
            case 1:
                field_0x3c.x = -5.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
                break;
            case 2:
                field_0x3c.x = 0.0f;
                field_0x3c.y = 5.0f;
                field_0x3c.z = 0.0f;
                break;
            case 3:
                field_0x3c.x = 0.0f;
                field_0x3c.y = -5.0f;
                field_0x3c.z = 0.0f;
                break;
            case 4:
                field_0x3c.x = 3.5f;
                field_0x3c.y = 3.5f;
                field_0x3c.z = 0.0f;
                break;
            case 5:
                field_0x3c.x = -3.5f;
                field_0x3c.y = 3.5f;
                field_0x3c.z = 0.0f;
                break;
            case 6:
                field_0x3c.x = 3.5f;
                field_0x3c.y = -3.5f;
                field_0x3c.z = 0.0f;
                break;
            case 7:
                field_0x3c.x = -3.5f;
                field_0x3c.y = -3.5f;
                field_0x3c.z = 0.0f;
                break;
            }
            if (field_0x94 == 1) {
                field_0x3c.z = field_0x270;
            }
            mPos.z = field_0x274;
        } else {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        }
    }
    field_0x278 = getGameView_802D6A9C()->field_0x131df[field_0x4];
    mPos.add(field_0x3c);
    switch (field_0x230) {
    case 0: {
        if (field_0x240 == 0) {
            mAlive = false;
        }
        if (field_0x94 == 0) {
            if (mEmitter26c != NULL) {
                mEmitter26c->setGlobalTranslation(mPos);
            } else {
                mEmitter26c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x35D, 0, 1, NULL, NULL, true, 1.0f);
            }
        }
        s32 serial = fn_801F26CC(lbl_80471840);
        if (serial >= 0) {
            ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (other != NULL &&
                (other->mActorId == 'FLME' || (other->mActorId == 'BLZ2' && other->field_0x94 == 1)))
            {
                fn_801F3668();
            }
        }
        if (field_0x268 == 1) {
            field_0x260--;
        }
        field_0x269 = field_0x268;
        field_0x268 = 0;
        JGeometry::TVec3f wall(0.0f, 0.0f, 0.0f);
        u8 blocked = fn_801F2618(&wall, false);
        if ((field_0x26a == 0 && fn_801F2A10(field_0x90, field_0x98, false, false)) || mLife == 0 ||
            blocked != 0)
        {
            if (field_0x4 < 8) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x339, 0, 1, NULL, NULL, true, 1.0f);
            } else {
                getActorManager_801F856C()->spawnActor_801F9484('HTMK', field_0x4, &mPos, 0, -1, -1);
            }
            if (blocked != 0) {
                if (0.0f != wall.x) {
                    field_0x3c.x *= -0.3f;
                    field_0x3c.y *= 0.3f;
                    field_0x3c.z = 2.0f;
                } else {
                    field_0x3c.x *= 0.3f;
                    field_0x3c.y *= -0.3f;
                    field_0x3c.z = 2.0f;
                }
            }
            startSound_801F0E34(0x30027, 0);
            field_0x23c = 100;
            mLife = 0;
            field_0x230 = 1;
            break;
        }
        if (mPos.z < 0.0f) {
            u16 tile = fn_8022461C(&mPos, field_0x4);
            fn_80226850(&mPos, field_0x4);
            if (tile == 0xC || tile == 3) {
                mAlive = false;
                break;
            }
            if (tile == 0xE) {
                s32 splash1;
                s32 splash2;
                if (fn_802DE9A0() == true) {
                    startSound_801F0E34(0x30061, 0);
                    splash1 = 0x2A9;
                    splash2 = 0x2AA;
                } else if (fn_802DEB58()) {
                    startSound_801F0E34(0x50042, 0);
                    splash1 = 0x45E;
                    splash2 = 0x45F;
                } else {
                    mAlive = false;
                    break;
                }
                spawnEmitter_8013CC50(field_0x4, &mPos, splash1, -1, 1, NULL, NULL, true, 0.75f);
                spawnEmitter_8013CC50(field_0x4, &mPos, splash2, 0, 0, NULL, NULL, true, 0.75f);
                mAlive = false;
                break;
            }
            if (tile == 0xD || tile == 0x39) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 0.75f);
                startSound_801F0E34(0x50041, 0);
                mAlive = false;
                break;
            }
            mPos.z = 0.0f;
            field_0x3c.x *= 0.5f;
            field_0x3c.y *= 0.5f;
            field_0x3c.z = 1.2f;
            field_0x23c = 100;
            mLife = 0;
            if (fn_80225000(&mPos, field_0x4) == true) {
                startSound_801F0E34(0x50074, 0);
            } else if (fn_80224D0C(&mPos, field_0x4) == true) {
                startSound_801F0E34(0x50072, 0);
            } else if (fn_80224A04(&mPos, field_0x4) == true) {
                startSound_801F0E34(0x50073, 0);
            } else {
                startSound_801F0E34(0x50075, 0);
            }
            if (field_0x4 < 8) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x339, 0, 1, NULL, NULL, true, 1.0f);
            } else {
                getActorManager_801F856C()->spawnActor_801F9484('HTMK', field_0x4, &mPos, 0, -1, -1);
            }
            field_0x230 = 1;
            break;
        }
        if (field_0x94 == 1) {
            if (getGameView_802D6A9C()->field_0x131df[field_0x4] == 0) {
                field_0x3c.z = field_0x3c.z - 0.1;
            } else {
                field_0x270 -= 0.1f;
                field_0x3c.y = field_0x3c.y + 0.1;
            }
        }
        JGeometry::TBox2f box = mBox;
        box.addPos(mPos.x, mPos.y);
        if (field_0x26a == 0) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && !fn_8022E438(i, field_0x90) && fn_8023B190(i, &box, field_0x4)) {
                    JGeometry::TVec2f centre;
                    centre.x = box.f.x + 0.5f * (box.i.x - box.f.x);
                    centre.y = box.f.y + 0.5f * (box.i.y - box.f.y);
                    if (field_0x94 == 0 && gGameRoot_8053AAF8->field_0x7f == 0) {
                        fn_80238548(i, 1, &centre, 1);
                        mAlive = false;
                    } else {
                        fn_80238548(i, 0, &centre, 0xB);
                        mAlive = false;
                    }
                    field_0x26a = 1;
                }
            }
        }
        if (field_0x24c != field_0x25c) {
            field_0x24c = field_0x25c;
        }
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        }
        break;
    }
    case 1:
        addCalc2_801F71A4(&field_0x234, 0.0f, 0.5f, 10.0f);
        if (field_0x94 == 0) {
            if (mEmitter26c != NULL) {
                mEmitter26c->setGlobalTranslation(mPos);
                mEmitter26c->setGlobalAlpha(field_0x234);
            } else {
                mEmitter26c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x35D, 0, 1, NULL, NULL, true, 1.0f);
                if (mEmitter26c != NULL) {
                    mEmitter26c->setGlobalAlpha(field_0x234);
                }
            }
        }
        field_0x3c.z -= 0.3f;
        if ((field_0x278 == 0 && mPos.z < 0.0f) || field_0x23c == 0 || field_0x234 < 5.0f) {
            mAlive = false;
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
}
#endif

// 0x803239A0.
// FIXME(shotgun): 97.24 %, missing/extra + reloc-name
void SlingshotSeed_804AB3A8::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x10.a = (u8)(s32)field_0x234;
    if (field_0x94 == 1 && field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, 0xE7, &params, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, 0xE7, false, 2, -1, 0, 0, 0);
}
