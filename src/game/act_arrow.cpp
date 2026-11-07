#include "game/GameAudio.h"
#include "game/DrawShadow.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_arrow.h"
#include "game/GameView.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "JSystem/JMath/JMath.h"

// The arrows each player has shot, the unit's .bss object at 0x804C6330 (declared extern: defining it here changes the static initialiser).
extern s32 lbl_804C6330[4];
extern const u32 lbl_804662E8[5];
extern const u32 lbl_804662FC[4];

// The unit's .data table at 0x804A0530.
struct ArrowTables_804A0530 {
    f32 emitterOffset[4][2]; // by direction: where the flame emitter sits relative to mPos
    s32 turn[4];             // next direction when the arrow spins after a hit
    s32 frame[4][8];         // sprite frame by direction and spin step
};
extern const ArrowTables_804A0530 lbl_804A0530;

// Frees an emitter the arrow owns (the status/frame stores of the original are in
// the shared JPA emitter code): FIXME(shotgun) stores 1 into the emitter word 0x24 as well.
static inline void freeEmitter(JPABaseEmitter** emitter) {
    if (*emitter != NULL) {
        (*emitter)->setStatus(1);
        *emitter = NULL;
    }
} // ids of the actors the arrow flies through (rodata)

// 0x80166814.
// FIXME(shotgun): 99.46 %, reloc-name
void ArrowPlayer_804A05E0::init() {
    field_0x98 = (field_0x90 >> 4) & 3;
    field_0x94 = (field_0x90 >> 8) & 0xF;
    field_0x90 = field_0x90 & 3;
    if (fn_8022B160()) {
        s32 player = fn_8022F340(field_0x90);
        lbl_804C6330[player]++;
    }
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mBox.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x80.set(-2.0f, -2.0f, 2.0f, 2.0f);
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
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
    }
    field_0x234 = 255.0f;
    field_0x24c = 500;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    mLife = 10000;
    field_0x268 = field_0x98;
    field_0x230 = 0;
    field_0x274 = 0;
}

// 0x8016636C. Whether the arrow is blocked at pos: by a collision box, or by the tile there.
// FIXME(shotgun): 88.86 %, missing/extra
bool ArrowPlayer_804A05E0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, lbl_804662E8) == true) {
        return true;
    }
    u16 code = fn_8022461C(pos, layer);
    s32 tile = fn_80226850(pos, layer);
    if ((u32)(code - 0x10) <= 7 || (tile >= 0x140 && tile <= 0x15F)) {
        field_0x23c = 0;
        field_0x238 = 1;
        return false;
    }
    if ((field_0x94 == 0 || field_0x94 == 4) && tile >= 0x40 && tile <= 0x9A) {
        return false;
    }
    if ((u32)(tile - 0x230) <= 1 || tile == 0x234) {
        field_0x238 = 0;
        return false;
    }
    if (code != 0 && code != 4 && code != 5 && code != 6 && code != 7 && code != 2 && code != 3 &&
        code != 0x50 && code != 0xE && code != 0xC && code != 0x27 && code != 0x28 && code != 0x29 &&
        code != 0x2A && code != 0x2B && code != 0x2C && code != 0x2D && code != 0x2E && code != 0x2F &&
        code != 0x20 && code != 0xD && code != 0x39 && code != 0x3A && code != 0x3B && code != 0x4C &&
        code != 0x4D && code != 0x4E && code != 0x4F && code != 0x24) {
        if ((u32)(code - 7) <= 1 || code == 0xA) {
            if (field_0x98 == 1) {
                return false;
            }
            if (field_0x238 == 0) {
                field_0x240++;
                if (field_0x240 > 20) {
                    return true;
                }
            }
            return false;
        }
        if (code == 6 || code == 9 || code == 0xB) {
            if (field_0x98 == 0) {
                return false;
            }
            if (field_0x238 == 0) {
                field_0x240++;
                if (field_0x240 > 20) {
                    return true;
                }
            }
            return false;
        }
        if (code == 4) {
            if (field_0x98 == 2) {
                return false;
            }
            if (field_0x238 == 0) {
                field_0x240++;
                if (field_0x240 > 20) {
                    return true;
                }
            }
            return false;
        }
        if (code == 5) {
            if (field_0x98 == 3) {
                return false;
            }
            if (field_0x238 == 0) {
                field_0x240++;
                if (field_0x240 > 20) {
                    return true;
                }
            }
            return false;
        }
        if (fn_80226850(pos, layer) == 0x3A || fn_80226850(pos, layer) == 0x3D) {
            field_0x238 = 0;
            return false;
        }
        if (tile == 0x14) {
            field_0x238 = 0;
            return false;
        }
        if (code == 0x25) {
            if (field_0x238 == 0 && getGameView_802D6A9C()->field_0x171 == 0) {
                return true;
            }
            field_0x238 = 0;
            return false;
        }
        if (code == 0x26) {
            if (field_0x238 == 0 && getGameView_802D6A9C()->field_0x171 == 1) {
                return true;
            }
            field_0x238 = 0;
            return false;
        }
        if (field_0x238 == 0) {
            if (code != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
                return true;
            }
        }
        if (field_0x238 == 1) {
            return false;
        }
    }
    if (field_0x238 == 1) {
        field_0x23c++;
        if (field_0x23c > 11) {
            field_0x23c = 0;
            field_0x238 = 0;
        }
    }
    return false;
}

// 0x801662D8.
ArrowPlayer_804A05E0::~ArrowPlayer_804A05E0() {
    for (int i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// 0x80165730. States: 0 flying, 1 spinning away after a hit, 2/3 burning up, 4 caught by a flame.
// FIXME(shotgun): 87.83 %, missing/extra + reloc-name
void ArrowPlayer_804A05E0::update() {
    if (field_0x94 == 3) {
        JGeometry::TBox2f box;
        box.i.x = mBox.i.x + mPos.x;
        box.i.y = mBox.i.y + mPos.y;
        box.f.x = mBox.f.x + mPos.x;
        box.f.y = mBox.f.y + mPos.y;
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && !fn_8022E438(i, field_0x90) && fn_8023B190(i, &box, field_0x4)) {
                fn_8022E1A0(i, mSerial);
            }
        }
        return;
    }
    if (field_0x230 != 4) {
        s32 flame = fn_801F2718('FLME');
        if (flame < 0) {
            flame = fn_801F2718('BLZ2');
            if (flame >= 0) {
                ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(flame);
                if (a == NULL) {
                    flame = -1;
                } else if (a->field_0x94 == 0) {
                    flame = -1;
                }
            }
        }
        if (flame >= 0) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0xBD, 0, 1, NULL, NULL, true, 1.0f);
            field_0x26c = 2;
            mLife = 0;
            field_0x258 = 0;
            field_0x244 = 20;
            startSound_801F0E34(0x30005, 0);
            field_0x230 = 4;
        }
    }
    mPos += field_0x3c;
    if ((field_0x94 == 2 || field_0x94 == 4) && field_0x230 != 4) {
        if (field_0x260 == 0) {
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0xBD, 0, 1, NULL, NULL, true, 1.0f);
            field_0x260 = 1;
        }
        if (mEmitters[0] != NULL) {
            JGeometry::TVec3f p = mPos;
            p.x += lbl_804A0530.emitterOffset[field_0x98][0];
            p.y += lbl_804A0530.emitterOffset[field_0x98][1];
            setEmitterPos_8013CB44(mEmitters[0], &p, 0, 2, true);
            s32 rz = 0;
            if (field_0x98 == 2) {
                rz = lbl_804662FC[field_0x98];
            }
            JGeometry::TVec3<s16> rot;
            rot.x = -1;
            rot.y = (s16)rz;
            rot.z = (s16)lbl_804662FC[field_0x98];
            mEmitters[0]->setGlobalRotation(rot);
        }
        setEmitterPos_8013CB44(mEmitters[1], &mPos, 0, 1, true);
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox2.set(-8.0f, -8.0f, 8.0f, 8.0f);
        fn_801F2B7C(field_0x114, field_0x90, 7);
    }
    switch (field_0x230) {
    case 0:
        if (field_0x24c == 0) {
            mAlive = false;
        }
        if ((field_0x274 == 0 && fn_801F2A10(field_0x90, field_0x98, true, true)) || mLife == 0) {
            if (field_0x98 == 3) {
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                    field_0x3c.x = 5.0f;
                } else {
                    field_0x3c.x = -5.0f;
                }
            }
            field_0x3c.x *= -0.7f;
            field_0x3c.y *= -0.7f;
            field_0x3c.z = 5.0f;
            startSound_801F0E34(0x400D1, 0);
            field_0x248 = 100;
            switch (field_0x98) {
            case 0:
                field_0x258 = 3;
                break;
            case 1:
                field_0x258 = 1;
                break;
            case 2:
                field_0x258 = 0;
                break;
            case 3:
                field_0x258 = 2;
                break;
            }
            field_0x26c = 2;
            mLife = 0;
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
            field_0x230 = 1;
            break;
        }
        if (field_0x94 == 0 || field_0x94 == 4) {
            if (mEmitters[1] == NULL) {
                s32 id = 0;
                switch (field_0x98) {
                case 0:
                    id = 0x283;
                    break;
                case 1:
                    id = 0x282;
                    break;
                case 2:
                    id = 0x281;
                    break;
                case 3:
                    id = 0x284;
                    break;
                }
                mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, id, 0, 1, NULL, NULL, true, 1.0f);
            } else {
                setEmitterPos_8013CB44(mEmitters[1], &mPos, 0, 1, true);
            }
            s32 angle = (s16)fn_801F6874(field_0x98) + 0x2000;
            for (s32 n = 0; n < 2; n++) {
                JGeometry::TVec3f p;
                p.x = mPos.x;
                p.y = mPos.y;
                p.z = mPos.z;
                p.x = mPos.x + 8.0f * JMASinShort(angle);
                p.y = mPos.y + 8.0f * JMACosShort(angle);
                gBgRewrite_8053AC98->fn_802A20F0(&p, field_0x90, field_0x4, 0, 0);
                angle -= 0x4000;
            }
        }
        {
            JGeometry::TBox2f box;
            box.i.x = mBox.i.x + mPos.x;
            box.i.y = mBox.i.y + mPos.y;
            box.f.x = mBox.f.x + mPos.x;
            box.f.y = mBox.f.y + mPos.y;
            if (field_0x274 == 0) {
                for (s32 i = 0; i < 4; i++) {
                    if (fn_8023F264(i) && !fn_8022E438(i, field_0x90) && fn_8023B190(i, &box, field_0x4)) {
                        JGeometry::TVec2f c;
                        c.x = 0.5f * (box.i.x - box.f.x) + box.f.x;
                        c.y = 0.5f * (box.i.y - box.f.y) + box.f.y;
                        if (field_0x94 == 0 || field_0x94 == 4) {
                            if (gGameRoot_8053AAF8->field_0x7f) {
                                fn_80238548(i, 0, &c, 0);
                            } else {
                                fn_80238548(i, 2, &c, 0);
                            }
                            fn_8022E1A0(i, mSerial);
                        } else {
                            if (gGameRoot_8053AAF8->field_0x7f) {
                                fn_80238548(i, 0, &c, 1);
                            } else {
                                fn_80238548(i, 1, &c, 1);
                            }
                            mAlive = false;
                        }
                        field_0x274 = 1;
                    }
                }
            }
        }
        if (fn_801F2618(NULL, false)) {
            if (field_0x94 == 2 || field_0x94 == 4) {
                getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, &mPos, field_0x90 & 3, -1, -1);
            }
            field_0x26c = 1;
            mLife = 0;
            freeEmitter(&mEmitters[1]);
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x258 = 0;
            field_0x244 = 0;
            field_0x230 = 2;
        } else {
            if (field_0x258 != field_0x270) {
                field_0x258 = field_0x270;
                startSound_801F0E34(0x3001D, 0);
            }
            if (field_0x4 < 8) {
                gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
            }
        }
        break;
    case 1:
        if (!(field_0x248 & 1)) {
            field_0x258 = (field_0x258 + 1) & 3;
            field_0x98 = lbl_804A0530.turn[field_0x258];
        }
        freeEmitter(&mEmitters[0]);
        freeEmitter(&mEmitters[1]);
        addCalc2_801F71A4(&field_0x234, 0.0f, 0.5f, 10.0f);
        field_0x3c.z -= 0.3f;
        if (mPos.z < 0.0f) {
            mAlive = false;
        }
        break;
    case 2:
        startSound_801F0E34(0x30000, 0);
        field_0x230 = 3;
        // fall through
    case 3:
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        }
        if (field_0x244 == 0) {
            field_0x244 = 1;
            field_0x258++;
            if (field_0x258 == 6) {
                field_0x244 = 30;
            } else if (field_0x258 >= 7) {
                mAlive = false;
            }
        }
        if (field_0x258 == 6 && field_0x244 < 3) {
            field_0x26c = 2;
        }
        break;
    case 4:
        if (field_0x244 == 0) {
            freeEmitter(&mEmitters[0]);
            freeEmitter(&mEmitters[1]);
            mAlive = false;
        }
        break;
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
    if (field_0x230 != 4) {
        if (field_0x230 == 0 || field_0x230 == 1) {
            field_0x268 = field_0x98;
            if (field_0x4 >= 8 && (field_0x94 == 0 || field_0x94 == 4)) {
                field_0x268 = field_0x98 * 3;
                field_0x268 += field_0x264;
                if (field_0x244 == 0) {
                    field_0x244 = 2;
                    field_0x264++;
                    if (field_0x264 > 2) {
                        field_0x264 = 0;
                    }
                }
            }
        } else {
            field_0x268 = lbl_804A0530.frame[field_0x98][field_0x258];
        }
    }
}

// 0x8016545C.
// FIXME(shotgun): 99.39 %, missing/extra + reloc-name
void ArrowPlayer_804A05E0::draw() {
    if (field_0x94 != 3) {
        if (field_0x24c >= 50 || (field_0x24c & 2) == 0) {
            SpriteParams_801F5AE0 params;
            params.field_0x10.a = (u8)(s32)field_0x234;
            if (fn_8022B160() == true) {
                switch (field_0x90) {
                case 0:
                    params.field_0x30 = 0;
                    break;
                case 1:
                    params.field_0x30 = 1;
                    break;
                case 2:
                    params.field_0x30 = 2;
                    break;
                case 3:
                    params.field_0x30 = 3;
                    break;
                }
            }
            s32 frame = field_0x268 + 0x4C;
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
            }
            s32 base = 0x4C;
            if ((field_0x94 == 0 || field_0x94 == 4) && field_0x230 == 0) {
                base = 0x2DF;
            }
            fn_803075AC(mSerial, field_0x4, &mPos, base + field_0x268, false, 2, -1, 0, 0, 0);
        }
        if ((field_0x94 == 2 || field_0x94 == 4) && field_0x230 != 4) {
            drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 32.0f);
        }
    }
}
