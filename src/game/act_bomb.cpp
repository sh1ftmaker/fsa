#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/ViewLayer.h"
#include "game/act/act_bomb.h"
#include "game/act/act_warp.h"
#include "std/math.h"
#include "JSystem/JMath/JMath.h"

static s32 lbl_8049F760[9] = {4, 4, 2, 2, 4, 4, 4, 4, 4};

// Records the landing position and stops the bomb (shared by the three tile cases).
static inline void land(Bomb_8049F784* bomb) {
    bomb->field_0x284.x = bomb->mPos.x;
    bomb->field_0x284.y = bomb->mPos.y;
    bomb->field_0x284.z = bomb->mPos.z;
    bomb->field_0x3c.z = 0.0f;
    bomb->field_0x3c.y = 0.0f;
    bomb->field_0x3c.x = 0.0f;
}

// Inlined into fn_8014D868's loop in the original (its second NULL test is in the asm).
static inline void checkWarp(Warp_804A6DC8* warp, const JGeometry::TVec3f* pos) {
    if (warp == NULL) {
        return;
    }
    f32 dy = warp->mPos.y - pos->y;
    f32 dx = warp->mPos.x - pos->x;
    f32 dist = std::sqrtf(dx * dx + dy * dy);
    if (dist < 24.0f) {
        const JGeometry::TVec3f* spawnPos = &warp->field_0x238;
        s32 raw = (warp->field_0x90 >> 24) & 7;
        // raw + 7 is computed before the zero test (addi, then mr under bne).
        s32 layer = raw + 7;
        if (raw == 0) {
            layer = raw;
        }
        getActorManager_801F856C()->spawnActor_801F9484('HBMB', layer, spawnPos, 0xC, -1, -1);
    }
}

void Bomb_8049F784::vfunc_0x44() {
    s32 serial = field_0xa0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL && actor->mActorId == 'ARRW') {
            setEmitterPos_8013CB44(mEmitters[1], &mPos, 0, 0, false);
            setEmitterPos_8013CB44(mEmitters[2], &mPos, 0, 0, false);
        }
    }
}

// FIXME: 99.14 % (regalloc-only + reloc-name). Only register allocation differs in checkWarp loop.
void Bomb_8049F784::fn_8014D868(const JGeometry::TVec3f* pos) {
    ActorBase_804A5770* warps[ACTOR_MANAGER_MAX_ACTORS];
    s32 count = getActorManager_801F856C()->fn_801F7818('WARP', field_0x4, warps, -1);
    for (s32 i = 0; i < count; i++) {
        Warp_804A6DC8* warp = static_cast<Warp_804A6DC8*>(warps[i]);
        if (warp != NULL) {
            checkWarp(warp, pos);
        }
    }
}

// FIXME: 99.90 % (reloc-name). Functionally 100 %; only .sdata2 float relocations differ.
bool Bomb_8049F784::fn_8014D6DC() {
    if (mPos.z > 0.1f) {
        return false;
    }
    JGeometry::TBox2f box(-3.0f, -3.0f, 3.0f, 3.0f);
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    box.addPos(pos);
    JGeometry::TVec2f center;
    center.x = box.f.x + 0.5f * (box.i.x - box.f.x);
    center.y = box.f.y + 0.5f * (box.i.y - box.f.y);
    u16 tile = fn_802265FC((const JGeometry::TVec3f*)&center, field_0x4);
    if (tile == 3) {
        land(this);
        field_0x230 = 6;
        return true;
    }
    if (tile == 0xE) {
        land(this);
        if (fn_802DE9A0() == 1 || fn_802DEB58() == 1) {
            field_0x230 = 8;
        } else {
            field_0x230 = 6;
        }
        return true;
    }
    if (tile == 0xD || tile == 0x3B) {
        land(this);
        field_0x230 = 7;
        return true;
    }
    return false;
}

// FIXME: 92.86 %. Scheduling of the 2x2 probe loop (the original loads mPos.y/mPos.x and converts j/i in a different order) and the .sdata2 reloc names.
void Bomb_8049F784::vfunc_0x54() {
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0xb0 = 0;
    getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
    gGameRoot_8053AAF8->fn_80134DE4(1, 0);
    field_0x94 = 1;
    if (field_0x90 == 8 || field_0x90 == 9) {
        field_0x27c = 2.0f;
    }
    deleteEmitter(&mEmitters[1]);
    deleteEmitter(&mEmitters[2]);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x59, 1, 1, NULL, NULL, true, field_0x27c);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x5A, 1, 1, NULL, NULL, true, field_0x27c);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x5B, 1, 1, NULL, NULL, true, field_0x27c);
    if (field_0x90 == 8 || field_0x90 == 9) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x5C, 1, 1, NULL, NULL, true, 1.0f);
    }
    startSound_801F0E34(0x30001, 0);
    field_0x24c = 0;
    field_0x234 = lbl_8049F760[field_0x24c];
    JGeometry::TVec3f result;
    JGeometry::TVec3f probe;
    result.z = 0.0f;
    result.y = 0.0f;
    result.x = 0.0f;
    // The original compares the bomb kind (field_0x90) as signed (cmpwi) in the
    // tests marked (s32) here and in init/draw, and as unsigned (cmplwi) elsewhere.
    if ((s32)field_0x90 != 7 && (s32)field_0x90 != 0xD && (s32)field_0x90 != 0xB) {
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        if ((s32)field_0x90 == 8 || (s32)field_0x90 == 9) {
            mBox.i.x = -32.0f;
            mBox.i.y = -32.0f;
            mBox.f.x = 32.0f;
            mBox.f.y = 32.0f;
        }
    }
    if (field_0x90 != 0xC) {
        if (field_0x90 != 8 && field_0x90 != 9) {
            if (gBgRewrite_8053AC98->fn_8029EDB8(&mPos, 0, field_0x4, &result) == 1) {
                fn_8014D868(&result);
            }
        } else {
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    probe.z = mPos.z;
                    probe.y = (mPos.y - 8.0f) + 16.0f * (f32)j;
                    probe.x = (mPos.x - 8.0f) + 16.0f * (f32)i;
                    if (gBgRewrite_8053AC98->fn_8029EDB8(&probe, 0, field_0x4, &result) == 1) {
                        fn_8014D868(&result);
                    }
                }
            }
        }
    } else {
        gBgRewrite_8053AC98->fn_8029EDB8(&mPos, 0, field_0x4, &result);
    }
    field_0x230 = 5;
}

void Bomb_8049F784::vfunc_0x48() {
    vfunc_0x54();
}

// FIXME: 99.67 % (reloc-name). Functionally 100 %; only .sdata2 float relocations and jumptable label differ.
void Bomb_8049F784::init() {
    mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
    field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    mLife = 2;
    field_0x26c = 0;
    field_0x280 = 0;
    field_0x27c = 1.0f;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    field_0x274 = 1.0f;
    field_0x268 = -1;
    field_0xb0 = 0x53308;
    field_0x270 = (field_0x90 >> 8) & 1;
    field_0x90 = field_0x90 & 0xFF;
    field_0x94 = 0;
    field_0x260 = -1;
    field_0x278 = 0.0f;

    switch (field_0x90) {
    case 10:
        field_0x248 = 20;
        // fallthrough
    case 0:
    case 8:
        field_0x238 = 300;
        field_0x230 = 2;
        field_0xb0 = 0x73308;
        if (viewLayerFlag(field_0x4)) {
            mPos.y -= mPos.z;
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        }
        if (field_0x90 == 8) {
            mBox.set(-13.0f, -13.0f, 13.0f, 13.0f);
            field_0x274 = 2.0f;
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x66, 0, 0, NULL, NULL, false, field_0x274);
        }
        if (mEmitters[2] == NULL) {
            mEmitters[2] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x67, 0, 0, NULL, NULL, false, field_0x274);
        }
        break;

    default:
    case 5:
    case 6:
        field_0x240 = 50;
        // fallthrough
    case 3:
    case 9:
        field_0x240 = 50;
        if (field_0x90 == 9) {
            mBox.set(-13.0f, -13.0f, 13.0f, 13.0f);
            field_0x274 = 2.0f;
        }
        field_0xb0 = 0x73308;
        field_0x230 = 0;
        break;

    case 1:
        mBox.set(0.0f, 0.0f, 0.0f, 0.0f);
        field_0x274 = 0.0f;
        field_0xb0 = 0;
        field_0x230 = 9;
        break;

    case 2:
        field_0x274 = 0.0f;
        // fallthrough
    case 4:
        if (gGameRoot_8053AAF8->field_0x7f == 0) {
            field_0x11f = 1;
        }
        mBox.set(-10.0f, -10.0f, 10.0f, 10.0f);
        field_0xb0 = 0x53303;
        field_0x230 = 10;
        break;

    case 12:
        vfunc_0x54();
        break;

    case 7:
    case 11:
    case 13:
        field_0x24c = 2;
        field_0x274 = 1.0f;
        if ((s32)field_0x90 == 13) {
            field_0x274 = 2.0f;
            field_0x27c = 2.0f;
        }
        startSound_801F0E34(0x30001, 0);
        field_0x234 = lbl_8049F760[field_0x24c];
        field_0x230 = 4;
        break;
    }
}

Bomb_8049F784::~Bomb_8049F784() {
    for (int i = 0; i < 3; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// FIXME(shotgun): 97.63 %, missing/extra
void Bomb_8049F784::update() {
    JGeometry::TBox2f box(mBox);
    JGeometry::TVec2f pos(mPos.x, mPos.y);
    box.i.add(pos);
    box.f.add(pos);
    JGeometry::TVec2f mid(0.5f * (box.i.x - box.f.x) + box.f.x, 0.5f * (box.i.y - box.f.y) + box.f.y);
    s32 damage = field_0x114;
    bool landed = false;
    s32 fuseY = 0;
    s32 fuseX = 0;
    if (field_0x230 != 9) {
        if (field_0xc4) {
            field_0x264 = 0;
            if (field_0x230 != 5) {
                if (mEmitters[1] == NULL) {
                    mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x66, 0, 0, NULL, NULL, false, field_0x274);
                }
                if (mEmitters[2] == NULL) {
                    mEmitters[2] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x67, 0, 0, NULL, NULL, false, field_0x274);
                }
            }
            field_0x244 = 5;
            field_0x240 = 5;
            field_0x25c = 0;
            mPos.set(*fn_8023E7B0(field_0xc8));
            if (field_0x90 == 8) {
                if (viewLayerFlag(field_0x4)) {
                    mPos.y -= 8.0f;
                } else {
                    mPos.z += 8.0f;
                }
            }
            field_0x4 = getPlayerLayer_8023DE58(field_0xc8);
            field_0x260 = field_0xc8;
        } else {
            mPos += field_0x3c;
        }
    }
    switch (field_0x230) {
    case 0:
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x66, 0, 0, NULL, NULL, false, field_0x274);
        }
        if (mEmitters[2] == NULL) {
            mEmitters[2] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x67, 0, 0, NULL, NULL, false, field_0x274);
        }
        if (field_0x3c.x == 0.0f && field_0x3c.y == 0.0f && field_0x3c.z == 0.0f) {
            break;
        }
        if (viewLayerFlag(field_0x4)) {
            mPos.y -= mPos.z;
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        }
        if (field_0x240 == 0) {
            field_0x240 = 5;
        }
        field_0x25c = 1;
        field_0x230 = 1;
    case 1:
        if (field_0xc4) {
            fuseX = fn_8023CB24(field_0xc8) + 1;
            fuseY = fn_8023CA88(field_0xc8);
        }
        setEmitterPos_8013CB44(mEmitters[1], &mPos, fuseY, fuseX, false);
        setEmitterPos_8013CB44(mEmitters[2], &mPos, fuseY, fuseX, false);
        if (mEmitters[1] != NULL) {
            startSound_801F0E34(0x3003E, 0);
        }
        if (!viewLayerFlag(field_0x4)) {
            if (field_0x90 != 6) {
                field_0x3c.z -= 1.0f;
                if (field_0x90 == 9) {
                    if (field_0x3c.z < -10.0f) {
                        field_0x3c.z = -10.0f;
                    }
                } else if (field_0x3c.z < -4.0f) {
                    field_0x3c.z = -4.0f;
                }
            } else {
                field_0x3c.z -= 0.15f;
                if (field_0x3c.z < -4.0f) {
                    field_0x3c.z = -4.0f;
                }
            }
            if (mPos.z < 0.0f && field_0x3c.z < 0.0f) {
                landed = true;
                if (!field_0xc4 && fn_8014D6DC()) {
                    break;
                }
            }
        } else {
            field_0x3c.y += 0.15f;
            if (field_0x3c.y > 10.0f) {
                field_0x3c.y = 10.0f;
            }
            JGeometry::TVec3f saved;
            saved.set(mPos);
            mPos.y += 8.0f;
            if (fn_801F2618(NULL, false)) {
                landed = true;
                field_0x3c.zero();
            }
            mPos.set(saved);
        }
        if (!landed) {
            break;
        }
        mPos.z = 0.0f;
        if (field_0x260 >= 0) {
            fn_80230FD0(field_0x260, 0x20006);
        } else {
            startSound_801F0E34(0x30077, 0);
        }
        if (field_0x238 == 0) {
            field_0x238 = 300;
            if (field_0x90 == 6) {
                field_0x238 = 200;
            } else if (field_0x90 == 9) {
                field_0x238 = 420;
            }
        }
        field_0x254 = 0;
        if (field_0x90 == 3 && field_0x250 == 0) {
            field_0x3c.z = 5.0f;
            field_0x3c.x = JMath::sincosTable_.sinShort((s32)(65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
            field_0x3c.y = JMath::sincosTable_.cosShort((s32)(65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()));
            // The original clears the velocity again right after setting it.
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x250 = 1;
            break;
        }
        field_0x240 = 5;
        field_0x25c = 1;
        field_0x230 = 3;
        break;
    case 10:
        if (field_0x258 == 0 && field_0x274 < 0.98f) {
            addCalc2_801F71A4(&field_0x274, 1.0f, 0.4f, 0.4f);
            break;
        }
        if (field_0x258 == 0) {
            field_0x258 = 1;
            field_0x274 = 0.0f;
        }
        field_0x25c = 2;
        field_0x238 = 300;
    case 2:
        if (field_0xc4) {
            fuseX = fn_8023CB24(field_0xc8) + 1;
            fuseY = fn_8023CA88(field_0xc8);
        }
        setEmitterPos_8013CB44(mEmitters[1], &mPos, fuseY, fuseX, false);
        setEmitterPos_8013CB44(mEmitters[2], &mPos, fuseY, fuseX, false);
        if (mEmitters[1] != NULL) {
            startSound_801F0E34(0x3003E, 0);
        }
        if (!viewLayerFlag(field_0x4)) {
            if (field_0x26c == 0) {
                field_0x3c.zero();
            } else {
                addCalc2_801F71A4(&field_0x3c.x, 0.0f, 0.3f, 0.2f);
                addCalc2_801F71A4(&field_0x3c.y, 0.0f, 0.3f, 0.2f);
                if (std::fabsf(field_0x3c.x) < 0.1f && std::fabsf(field_0x3c.y) < 0.1f) {
                    field_0x3c.set(0.0f, 0.0f, 0.0f);
                    field_0x26c = 0;
                }
            }
            if (!field_0xc4 && fn_8014D6DC()) {
                break;
            }
            if ((field_0x3c.x != 0.0f || field_0x3c.y != 0.0f || field_0x3c.z != 0.0f) && field_0x90 != 6 && field_0x230 != 10) {
                fn_801F2618(NULL, false);
            }
        } else {
            field_0x3c.y += 0.15f;
            if (field_0x3c.y > 10.0f) {
                field_0x3c.y = 10.0f;
            }
            JGeometry::TVec3f hit;
            if (fn_801F2618(&hit, false)) {
                if (std::fabsf(hit.y - -1.0f) < 0.001f) {
                    field_0x3c.zero();
                } else if (hit.x != 0.0f) {
                    field_0x3c.x = 0.0f;
                }
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && fn_80239C00(i, &box, field_0x4) && fn_8022F9E4(i, mSerial, true, false)) {
                field_0x244 = 5;
                field_0x240 = 5;
                field_0x254 = 0;
                if (field_0x230 == 10) {
                    field_0xb0 = 0x73308;
                    field_0x90 = 0;
                    if (field_0x270 == 0) {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x59);
                    } else {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x85);
                    }
                    mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
                    field_0x274 = 1.0f;
                }
                if (field_0x90 == 6) {
                    field_0x90 = 0;
                }
                field_0x230 = 3;
                break;
            }
        }
        break;
    case 3:
        if (field_0xc4) {
            fuseX = fn_8023CB24(field_0xc8) + 1;
            fuseY = fn_8023CA88(field_0xc8);
        }
        setEmitterPos_8013CB44(mEmitters[1], &mPos, fuseY, fuseX, false);
        setEmitterPos_8013CB44(mEmitters[2], &mPos, fuseY, fuseX, false);
        if (mEmitters[1] != NULL) {
            startSound_801F0E34(0x3003E, 0);
        }
        if (!viewLayerFlag(field_0x4)) {
            if (!field_0xc4) {
                if (fn_8014D6DC()) {
                    break;
                }
                if (field_0x260 >= 0 && field_0x25c == 0) {
                    field_0x3c.z -= 0.15f;
                    if (field_0x3c.z < -4.0f) {
                        field_0x3c.z = -4.0f;
                    }
                } else {
                    field_0x3c.z -= 1.0f;
                    if (field_0x3c.z < -4.0f) {
                        field_0x3c.z = -4.0f;
                    }
                }
                if (field_0x90 != 6) {
                    fn_801F2618(NULL, false);
                }
            }
            if (mPos.z >= 0.0f || field_0x3c.z >= 0.0f) {
                break;
            }
            if (field_0x26c == 0 && mPos.z < 2.0f && (field_0x3c.x != 0.0f || field_0x3c.y != 0.0f)) {
                if (fn_802265FC(&mPos, field_0x4) == 0x50) {
                    field_0x26c = 1;
                }
            }
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            if (field_0x260 >= 0) {
                fn_80230FD0(field_0x260, 0x20006);
            } else {
                startSound_801F0E34(0x30077, 0);
            }
            if (field_0x26c == 0) {
                field_0x3c.x *= 0.3f;
                field_0x3c.y *= 0.3f;
            }
            if (field_0x25c == 0) {
                if (field_0x26c == 0) {
                    field_0x3c.z = 3.0f;
                }
                field_0x25c = 1;
                break;
            }
            if (field_0x26c == 0) {
                field_0x3c.zero();
            }
            field_0x258 = 1;
            field_0x230 = 2;
        } else {
            if (field_0xc4) {
                break;
            }
            mPos.y -= mPos.z;
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x184 = 16.0f;
            if (field_0x3c.y < 10.0f) {
                field_0x3c.y += 0.15f;
            }
            JGeometry::TVec3f hit;
            if (!fn_801F2618(&hit, false)) {
                break;
            }
            if (std::fabsf(hit.y - -1.0f) < 0.001f) {
                if (field_0x25c < 2) {
                    field_0x3c.x *= 0.1f;
                    field_0x3c.y = -1.0f;
                    field_0x25c = 2;
                    break;
                }
                field_0x258 = 1;
                field_0x3c.zero();
                field_0x230 = 2;
            } else if (hit.x != 0.0f) {
                field_0x3c.x = 0.0f;
            }
        }
        break;
    case 4:
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x59, 1, 1, NULL, NULL, true, field_0x27c);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x5A, 1, 1, NULL, NULL, true, field_0x27c);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x5B, 1, 1, NULL, NULL, true, field_0x27c);
        if (field_0x90 == 0xB) {
            field_0x234 = 0;
            field_0x24c = 0;
        }
        field_0x230 = 5;
    case 5:
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        if (field_0x24c < 4 && field_0x90 != 7 && field_0x90 != 0xD && field_0x90 != 0xB) {
            mBox.set(-16.0f, -16.0f, 16.0f, 16.0f);
            if (field_0x90 == 8 || field_0x90 == 9) {
                mBox.set(-32.0f, -32.0f, 32.0f, 32.0f);
            }
            fn_801F2B7C(damage, -1, 5);
            fn_801F2B20(0);
        }
        if (field_0x90 != 0xB) {
            if (field_0x234 != 0) {
                break;
            }
            field_0x24c++;
            if (field_0x24c > 8) {
                mAlive = false;
            }
            field_0x234 = lbl_8049F760[field_0x24c];
        } else {
            if (field_0x234 != 0) {
                break;
            }
            field_0x24c++;
            if (field_0x24c > 7) {
                mAlive = false;
            }
            field_0x234 = 2;
        }
        break;
    case 7:
        if (viewLayerFlag(field_0x4)) {
            deleteEmitter(&mEmitters[1]);
            deleteEmitter(&mEmitters[2]);
            mAlive = false;
            break;
        }
        if (mEmitters[0] == NULL) {
            field_0x27c = 0.66f;
            if (field_0x90 == 8) {
                field_0x27c = 1.32f;
            }
            if (field_0x3c.x != 0.0f || field_0x3c.y != 0.0f) {
                mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D4, 1, 1, NULL, NULL, true, field_0x27c);
                startSound_801F0E34(0x30022, 0);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F3, -1, 1, NULL, NULL, true, field_0x27c);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F4, 0, 0, NULL, NULL, true, field_0x27c);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F5, 0, 0, NULL, NULL, true, field_0x27c);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F6, 0, 0, NULL, NULL, true, field_0x27c);
            } else {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 1, 1, NULL, NULL, true, field_0x27c);
            }
        }
        field_0x250 = 0;
        land(this);
        field_0x230 = 6;
    case 6:
        if (mEmitters[1] != NULL) {
            mEmitters[1]->becomeInvalidEmitter();
            mEmitters[1] = NULL;
        }
        if (mEmitters[2] != NULL) {
            mEmitters[2]->becomeInvalidEmitter();
            mEmitters[2] = NULL;
        }
        addCalc2_801F71A4(&mPos.x, field_0x284.x, 0.1f, 1.0f);
        addCalc2_801F71A4(&mPos.y, field_0x284.y, 0.1f, 1.0f);
        addCalc2_801F71A4(&field_0x274, 0.0f, 0.1f, 0.1f);
        field_0x278 += 0.5f;
        if (field_0x274 < 0.1f) {
            if (mEmitters[0] != NULL) {
                field_0x250++;
                if (field_0x250 < 40) {
                    break;
                }
                mEmitters[0]->becomeInvalidEmitter();
            }
            mAlive = false;
        }
        break;
    case 8:
        mPos.z = 0.0f;
        field_0x27c = 0.66f;
        if (field_0x90 == 8) {
            field_0x27c = 1.32f;
        }
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F7, -1, 1, NULL, NULL, true, field_0x27c);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F8, 0, 0, NULL, NULL, true, field_0x27c);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4F9, 0, 0, NULL, NULL, true, field_0x27c);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4FA, 0, 0, NULL, NULL, true, field_0x27c);
        if (fn_802DEB58() == true) {
            startSound_801F0E34(0x30022, 0);
        } else {
            startSound_801F0E34(0x5005F, 0);
        }
        field_0x250 = 0;
        land(this);
        field_0x230 = 6;
        break;
    case 9: {
        s32 serial = field_0x268;
        if (serial < 0) {
            if (field_0x234 != 0) {
                break;
            }
            if (fn_8023AF14(&box, field_0x4, false, 200.0f) >= 0) {
                break;
            }
            s32 param = 2;
            if (field_0x250 == 0) {
                param = 4;
            }
            param |= field_0x270 << 8;
            field_0x268 = getActorManager_801F856C()->spawnActor_801F9484('HBMB', field_0x4, &mPos, param, -1, -1);
            if (field_0x268 < 0) {
                break;
            }
            if (field_0x270 == 0) {
                gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x58);
            } else {
                gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x84);
            }
            if (field_0x250 != 0) {
                startSound_801F0E34(0x3001A, 0);
            }
            field_0x250 = 1;
        } else {
            ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (bomb == NULL || bomb->field_0xc4) {
                field_0x234 = 360;
                field_0x268 = -1;
            }
        }
        break;
    }
    }
    if (field_0x280 == 1) {
        field_0x24c = 0;
        field_0x3c.zero();
        return;
    }
    u8 hit = 0;
    if (!field_0xc4 && field_0x230 != 5 &&
        ((field_0x230 >= 1 && field_0x230 <= 3) || (field_0x230 == 10 && field_0x258 == 1))) {
        if (((mPos.z != 0.0f && field_0x90 != 6) || (mPos.z != 0.0f && mPos.z < 8.0f && field_0x90 == 6)) && field_0x240 == 0 &&
            field_0x244 == 0) {
            u8 mask = fn_801F2740(-1);
            bool other = false;
            if (mask) {
                s32 player = 0;
                while (mask) {
                    mask >>= 1;
                    if (mask) {
                        player++;
                    }
                }
                if (field_0x260 >= 0) {
                    if (player != field_0x260) {
                        if (!fn_8022F514(field_0x260)) {
                            if (field_0x260 != fn_8022F3DC(player)) {
                                other = true;
                            }
                        } else if (player != fn_8022F3DC(field_0x260) && fn_8022F3DC(player) != fn_8022F3DC(field_0x260)) {
                            other = true;
                        }
                        if (other == true) {
                            fn_801F3128(&mid);
                            fn_80238548(player, damage, &mid, 5);
                            hit = 1;
                        }
                    }
                } else {
                    hit = 1;
                }
            }
        }
        if (field_0x240 == 0) {
            if ((field_0x90 == 5 || field_0x90 == 6) && field_0x258 != 1) {
            } else if (field_0x230 == 10) {
                if (fn_801F2DB4('NULL', 'NULL', NULL)) {
                    hit = 2;
                }
            } else if (field_0x248 == 0) {
                s32 serial = fn_801F2718('HBMB');
                if (!viewLayerFlag(field_0x4)) {
                    if (serial >= 0) {
                        ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                        if (bomb != NULL) {
                            if (bomb->field_0x94 != 0 || (bomb->field_0x90 != 1 && bomb->mPos.z == 0.0f && mPos.z != 0.0f)) {
                                hit = 2;
                            } else if (bomb->mPos.z == 0.0f && mPos.z == 0.0f && fn_801F2DB4('NULL', 'HBMB', NULL)) {
                                hit = 2;
                            }
                            mBox.set(-3.0f, -3.0f, 3.0f, 3.0f);
                        }
                    } else if (fn_801F2DB4('NULL', 'NULL', NULL)) {
                        hit = 2;
                    } else if (field_0x3c.x != 0.0f || field_0x3c.y != 0.0f || field_0x3c.z != 0.0f) {
                        if (fn_801F2B20(0)) {
                            hit = 2;
                        }
                    }
                } else if (serial >= 0) {
                    ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (bomb != NULL) {
                        if (bomb->field_0x94 != 0 || fn_801F2DB4('NULL', 'HBMB', NULL)) {
                            hit = 2;
                        }
                        mBox.set(-3.0f, -3.0f, 3.0f, 3.0f);
                    }
                } else if (fn_801F2DB4('NULL', 'NULL', NULL)) {
                    hit = 2;
                } else if (mPos.y != mPrevPos.y) {
                    if (fn_801F2B20(0)) {
                        hit = 2;
                    }
                }
            }
            if (!hit && fn_801A4F5C(mSerial, &box, &pos)) {
                hit = 1;
            }
            if (!hit && field_0x230 == 10) {
                if (fn_801A4D1C(mSerial, &box, &pos) || fn_801A6108(mSerial, &box, &pos) || fn_801A46FC(mSerial, &box, &pos)) {
                    hit = 1;
                }
            }
            for (s32 i = 0; i < 4; i++) {
                if (!hit && fn_801F285C(NULL, NULL, true, i) >= 0) {
                    hit = 2;
                }
            }
            if (hit || mLife == 0) {
                if (field_0x230 == 10) {
                    if (field_0x270 == 0) {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x59);
                    } else {
                        gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x85);
                    }
                }
                vfunc_0x54();
            }
        }
    }
    if (field_0x230 == 2 || field_0x230 == 3) {
        if (field_0x238 < 100) {
            field_0x254++;
            if (field_0x238 < 50 && (field_0x254 & 1)) {
                field_0x254++;
            }
            field_0x254 &= 7;
            if (field_0x254 == 0) {
                field_0x24c ^= 1;
            }
        }
        if (field_0x238 == 0) {
            vfunc_0x54();
        }
    }
    if (field_0x234 > 0) {
        field_0x234--;
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

// FIXME: 98.17 % (scheduling+regalloc). Only register allocation differs (frame in r30/flag in r31, float registers in setRotScaleMtx).
void Bomb_8049F784::draw() {
    if (field_0x230 == 9) {
        return;
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;

    s32 i;
    s32 count = 0;
    for (i = 0; i < 4; i++) {
        if (fn_80236B50(i)) {
            count++;
        }
    }
    if (count == 4 && field_0x230 == 5) {
        if (field_0x234 == 0) {
            field_0x24c++;
            if (field_0x24c > 8) {
                field_0x24c = 8;
                field_0x234 = 0;
                return;
            }
            field_0x234 = lbl_8049F760[field_0x24c];
        } else {
            field_0x234--;
        }
    }

    s32 frame;
    s32 flag;
    f32 scale;
    if (field_0x90 == 8 || field_0x90 == 9) {
        if (field_0x230 != 5) {
            flag = 0;
            scale = field_0x274 * 0.5f;
            frame = field_0x24c + 0x312;
        } else {
            flag = 1;
            frame = field_0x24c + 0xE8;
        }
    } else {
        flag = 0;
        scale = field_0x274;
        frame = field_0x24c + 0xE8;
    }

    JGeometry::TVec3f rot(0.0f, field_0x278, 0.0f);
    setRotScaleMtx(mtx, rot, scale);
    params.mMtx = mtx;
    params.field_0x1d = 1;
    params.field_0x1c = 1;

    if ((s32)field_0x90 == 0xB) {
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x24c + 0x2D5, 0, 2, -1, 1, 0, 0);
        return;
    }

    if (field_0x230 == 10 || field_0x230 == 2) {
        params.field_0x1d = 0;
        params.field_0x1c = 0;
    }
    if (field_0xc4) {
        params.field_0x20 = 0;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
    }

    if (field_0x24c < 2) {
        s32 flags = -1;
        if (field_0x230 != 10 && field_0x230 != 0 && field_0x230 != 6 &&
            field_0x230 != 7 && field_0x230 != 8 && !field_0xc4) {
            if (field_0x90 == 8 || field_0x90 == 9) {
                flags = 7;
            } else {
                flags = 0;
            }
        }
        if (field_0x230 != 5) {
            drawSprite_801F06F0("enemy_both2_sob.bin", frame, &params, flags, 1.0f, 1.0f);
        }
    }

    bool h = false;
    if (field_0xc4 || (s32)field_0x90 == 0xB) {
        h = true;
    }
    fn_803075AC(mSerial, field_0x4, &mPos, frame, flag != 0, 2, -1, h, 0, 0);
}

// FIXME: 97.38 % (scheduling+regalloc). Only register allocation differs (layer in r31/pos in r29/code in r30 in target).
bool Bomb_8049F784::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 code = fn_8022461C(pos, layer);
    s32 tile = fn_80226850(pos, layer);
    if ((u32)(code - 0x10) <= 7 || (tile >= 0x140 && tile <= 0x15F)) {
        field_0x264 = 1;
        return false;
    }
    if (code == 3) {
        field_0x264 = 0;
        return false;
    }
    if (code == 0xE) {
        field_0x264 = 0;
        return false;
    }
    if (code == 0xD || code == 0x3B) {
        field_0x264 = 0;
        return false;
    }
    if (code != 0 && code != 4 && code != 5 && code != 6 && code != 7 && code != 2 &&
        code != 0x50 && code != 0xC && code != 0x27 && code != 0x28 && code != 0x29 &&
        code != 0x2A && code != 0x2B && code != 0x2C && code != 0x2D && code != 0x2E &&
        code != 0x2F && code != 0x20 && code != 0x39 && code != 0x3A && code != 0x4C &&
        code != 0x4D && code != 0x4E && code != 0x4F && code != 0x24 && code != 0x23) {
        if (fn_80226850(pos, layer) == 0x3A || fn_80226850(pos, layer) == 0x3D) {
            field_0x264 = 0;
            return false;
        }
        if (code == 0x25) {
            if (field_0x264 == 0 && getGameView_802D6A9C()->field_0x171 == 0) {
                return true;
            }
            return false;
        }
        if (code == 0x26) {
            if (field_0x264 == 0 && getGameView_802D6A9C()->field_0x171 == 1) {
                return true;
            }
            return false;
        }
        if (field_0x264 == 0) {
            if (code != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
                return true;
            }
        }
    }
    return false;
}
