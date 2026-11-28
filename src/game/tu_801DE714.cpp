#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/ViewLayer.h"
#include "game/tu_80212AC0.h"
#include "game/act/act_force_gem.h"
#include "game/act/act_tingle_buster.h"
#include "game/act/tu_801DE714.h"
#include "game/GameMath.h"
#include "game/save_data_803B8C74.h"

// The actor with this serial, or NULL. This file looks actors up through one inline
// helper, used at all 35 sites: written out by hand, cls_804A51E4::calcVelocity_0x88 differs
// (97.22 %) (docs/analysis/INLINE_LEADS.md).
static inline ActorBase_804A5770* actorBySerial(s32 serial) {
    return getActorManager_801F856C()->findActorBySerial_801F666C(serial);
}

// Whether player `player` is present, not busy (fn_802375B0 is 0) and one of the
// players taking part.
static inline bool canTalkTo(s32 player) {
    bool ok = false;
    bool free = false;
    if (fn_802375B0(player) == 0 && fn_801CCFE4(player)) {
        free = true;
    }
    if (free) {
        if (player < gGameRoot_8053AAF8->getPlayerCount_80138A30()) {
            ok = true;
        }
    }
    return ok;
}

// The animation tables: pairs of frame and duration, ended by -1.
static const s16 lbl_80469AF8[] = { 0, 10, 1, 10, 2, 10, 1, 10, -1, -1 };
static const s16 lbl_80469B0C[] = { 3, 10, 4, 10, 5, 10, 4, 10, -1, -1 };
static const s16 lbl_8053F450[] = { 6, 4, -1, -1 };
static const s16 lbl_80469B20[] = { 7, 2, 8, 2, 9, 2, -1, -1 };
static const s16 lbl_80469B30[] = { 10, 2, 11, 2, 12, 2, -1, -1 };
static const s16 lbl_80469B40[] = { 21, 4, 19, 4, 22, 4, 20, 4, -1, -1 };
static const s16 lbl_8053F458[] = { 27, 3, -1, -1 };
static const s16 lbl_80469B54[] = { 28, 4, 29, 12, 28, 6, 30, 6, 21, 20, -1, -1 };
static const s16 lbl_8053F460[] = { 19, 20, -1, -1 };
static const s16 lbl_8053F468[] = { 20, 20, -1, -1 };
static const s16 lbl_8053F470[] = { 21, 20, -1, -1 };
static const s16 lbl_8053F478[] = { 22, 20, -1, -1 };
static const s16 lbl_80469B6C[] = { 21, 4, 26, 4, 25, 4, 24, 4, 23, 4, 6, 10, -1, -1 };
static const s16 lbl_80469B88[] = { 0, 4, 1, 4, 2, 6, 1, 6, -1, -1 };
static const s16 lbl_80469B9C[] = { 3, 4, 4, 4, 5, 6, 4, 6, -1, -1 };
static const s16 lbl_80469BB0[] = { 0, 4, 13, 8, 14, 8, 15, 6, 14, 6, 15, 6, 14, 6, 15, 6, 14, 6, -1, -1 };
static const s16 lbl_80469BD8[] = { 3, 4, 16, 8, 17, 8, 18, 6, 17, 6, 18, 6, 17, 6, 18, 6, 17, 6, -1, -1 };

static const s16* lbl_804A51A0[17] = {
    lbl_80469AF8, lbl_80469B0C, lbl_8053F450, lbl_80469B20,
    lbl_80469B30, lbl_80469B40, lbl_8053F458, lbl_80469B54,
    lbl_8053F460, lbl_8053F468, lbl_8053F470, lbl_8053F478,
    lbl_80469B6C, lbl_80469B88, lbl_80469B9C, lbl_80469BB0,
    lbl_80469BD8,
};
s32 randInt_801CD664(s32 n);
bool fn_801CD290(s32 layer);

// Whether x lies in the left half of the view's box for `layer`.
static inline bool inLeftHalf(s32 layer, f32 x) {
    bool result = false;
    JGeometry::TBox2f* box = layerBox(layer);
    if (x < 0.5f * (box->i.x + box->f.x)) {
        result = true;
    }
    return result;
}

// Whether x lies behind this actor in the direction it faces.
static inline bool behind(cls_804A51E4* a, f32 x) {
    bool result = false;
    switch (a->mDir_0x98) {
    case 0:
        if (x < a->mPos.x - 0.001f) {
            result = true;
        }
        break;
    case 1:
        if (a->mPos.x + 0.001f < x) {
            result = true;
        }
        break;
    }
    return result;
}

// The actor enters from the side of the screen edge (shared by both targets).
static inline void enterFromEdge(cls_804A51E4* a) {
    a->mFlags |= 0x4000;
    a->field_0x2c4.y = a->mPos.y;
    JGeometry::TBox2f* box = layerBox(a->mLayer_0x4);
    f32 right = 30.0f + box->f.x;
    f32 left = box->i.x - 30.0f;
    if (inLeftHalf(a->mLayer_0x4, a->mPos.x) && left < a->mPos.x) {
        a->field_0x2c4.x = right;
    } else {
        a->field_0x2c4.x = left;
    }
    switch (a->mState) {
    case 0xD:
        if (a->mPos.x < a->field_0x2c4.x) {
            a->mDir_0x98 = 0;
        } else {
            a->mDir_0x98 = 1;
        }
        if (a->mPos.z >= 30.0f) {
            a->mPos.z = 30.0f;
            a->setState_801E1594(1);
        } else {
            a->setState_801E1594(4);
        }
        break;
    case 7: {
        bool cross = behind(a, right);
        if (cross) {
            a->field_0x2c4.x = right;
        } else {
            a->field_0x2c4.x = left;
        }
        a->setState_801E1594(5);
        break;
    }
    default:
        if (behind(a, a->field_0x2c4.x)) {
            a->setState_801E1594(5);
        } else if (a->mPos.z >= 30.0f) {
            a->mPos.z = 30.0f;
            a->setState_801E1594(1);
        } else {
            a->setState_801E1594(4);
        }
        break;
    }
}

// Looks the actors up again: the nearest of the players' kind, then the others.
static inline void findTargets(cls_804A51E4* a) {
    a->field_0x2ac = getActorManager_801F856C()->fn_801F750C(a->mLayer_0x4, a->field_0x2b8);
    if (a->field_0x2ac >= 0) {
        a->field_0x2b0 = -1;
        a->field_0x2b4 = -1;
    } else {
        a->field_0x2b0 = getActorManager_801F856C()->fn_801F7464(a->mLayer_0x4, -1);
        if (a->field_0x2b0 >= 0) {
            a->field_0x2b4 = -1;
        } else {
            a->field_0x2b4 =
                getActorManager_801F856C()->fn_801F732C(a->mLayer_0x4, &a->mPos, &a->mPos, a->field_0x2b8, 500.0f);
        }
    }
}


// Whether the state may change to 7. fn_801E0CF4 has the same test spelled out.
static inline bool canChase(const cls_804A51E4* a) {
    bool result = false;
    switch (a->mState) {
    case 0:
    case 7:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
        break;
    default:
        if (!(a->mFlags & 0x4000) && a->field_0x2cc == 0) {
            if (a->fn_801E1DB4()) {
                if (a->mFlags & 0x20000) {
                    if (a->mFlags & 0x100000) {
                        result = true;
                    }
                } else if (a->mTargetSerial < 0) {
                    result = true;
                }
            }
        }
        break;
    }
    return result;
}

// Takes `player` as the one to follow and moves to that player's layer; with no player, to
// one of the players' layers that has something to collect, picked at random. An inline in
// the original: fn_801E0CF4 stores -1 and tests the stored field, and starts the loop
// counter with a copy of `count`.
static inline void followPlayer(cls_804A51E4* a, s32 player) {
    a->field_0x2a8 = player;
    if (a->field_0x2a8 >= 0) {
        a->mLayer_0x4 = getPlayerLayer_8023DE58(a->field_0x2a8);
    } else {
        s32 count = 0;
        s32 layers[4];
        for (s32 i = 0; i < 4; i++) {
            s32 layer = getPlayerLayer_8023DE58(i);
            if (isUpperLayer_801CD2F8(layer) && layer != 8 && !fn_801E3250(layer, false)) {
                if (getActorManager_801F856C()->fn_801F750C(layer, -1) >= 0 ||
                    getActorManager_801F856C()->fn_801F7464(layer, -1) >= 0) {
                    layers[count] = layer;
                    count++;
                }
            }
        }
        if (count == 0) {
            a->mLayer_0x4 = 0;
        } else {
            a->mLayer_0x4 = layers[randInt_801CD664(count)];
        }
    }
}

// Turns round (state 6) when the place it is heading for lies behind it. An inline in the
// original: fn_801E0CF4 keeps its `cross` in the register of behind()'s result with no copy,
// which MWCC only does between two locals of inlined bodies.
static inline void turnIfBehind(cls_804A51E4* a) {
    bool cross = false;
    switch (a->mState) {
    case 0:
    case 5:
    case 6:
    case 7:
    case 15:
    case 16:
    case 17:
    case 18:
        break;
    default:
        if (a->fn_801E1DB4() && !a->fn_801E1D9C()) {
            if (a->mFlags & 0x4000) {
                cross = behind(a, a->field_0x2c4.x);
            } else if (a->mFlags & 0x20000) {
                s32 player = a->field_0x2a8;
                if (player >= 0 && player == -1) {
                    cross = behind(a, getPlayerPos_8023E724(player)->x);
                }
            } else {
                ActorBase_804A5770* t = NULL;
                s32 serial = a->field_0x2ac;
                if (serial >= 0) {
                    t = actorBySerial(serial);
                }
                if (t == NULL) {
                    serial = a->field_0x2b0;
                    if (serial >= 0) {
                        t = actorBySerial(serial);
                    }
                }
                if (t == NULL) {
                    serial = a->field_0x2b4;
                    if (serial >= 0) {
                        t = actorBySerial(serial);
                    }
                }
                if (t != NULL) {
                    cross = behind(a, t->mPos.x);
                }
            }
            if (cross) {
                a->field_0x2d0 = a->mState;
                a->setState_801E1594(6);
            }
        }
        break;
    }
}

// tu_801DE714 (0x801E22A4): the constructor, after EnemyBase's (a plain bl).
cls_804A51E4::cls_804A51E4() {
    field_0x2a8 = -1;
    field_0x2ac = -1;
    field_0x2b0 = -1;
    field_0x2b4 = -1;
    field_0x2b8 = -1;
    mTargetSerial = -1;
    field_0x2c0 = 0;
    field_0x2c4.set(0.0f, 0.0f);
    field_0x2cc = 0;
    field_0x2ce = 0;
    field_0x2d0 = -1;
}

// tu_801DE714 (0x801E2180, vtable slot 0x08): the deleting destructor. Its
// own part runs first; EnemyBase's destructor is inlined after it.
cls_804A51E4::~cls_804A51E4() {
    if (mTargetSerial >= 0) {
        if (field_0x2c0 > 0) {
            // Bit 0x80000 of mFlags is not in EnemyFlag_804A4808 yet.
            if ((mFlags & 0x80000) == 0) {
                lbl_8050EC80.getCurrentFile().field_0xc += field_0x2c0;
                // The clamp looks the current file up again (the original
                // indexes lbl_8050EC80 a second time after the add).
                // FAKEMATCH(crack-pass): row view (save_data_803B8C74.h SaveFileRow_CrackPass).
                SaveFileRow_CrackPass& row = lbl_8050EC80.getCurrentRow_CrackPass();
                if (row.field_0x2c < 0) {
                    row.field_0x2c = 0;
                }
            }
            field_0x2c0 = 0;
        }
        ActorBase_804A5770* target = actorBySerial(mTargetSerial);
        if (target != NULL) {
            target->mAlive = false;
        }
        mTargetSerial = -1;
    }
}

void cls_804A51E4::init() {
    EnemyBase_804A4808::init();
    mMapBox_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0xb0 = 0;
    mDir_0x98 = 0;
    mFlags |= 0x300;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    mLayer_0x4 = 0;
    field_0x2cc = randInt_801CD664(0x3D) + 0x3C;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.setKeyTables_801D2608(lbl_804A51A0, 0x11);
    mAnim.startAnim_801D22DC(0, 2, false);
}

// tu_801DE714 (0x801E2004, vtable slot 0x54, EnemyBase's start hook): see the
// declaration in act/tu_801DE714.h.
void cls_804A51E4::onFirstUpdate_0x54() {
    getActorManager_801F856C()->fn_801F7AB8(mActorId, mSerial);
    s32 layer = mLayer_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    mPos.x = box->i.x - 30.0f;
    field_0x2c4.set(mPos.x, mPos.y);
}

void cls_804A51E4::fn_801E1E08() {
    if (mTargetSerial >= 0) {
        if (!checkFlag(0x80000)) {
            mFlags |= 0x40000;
        }
        if (mTargetSerial >= 0) {
            if (field_0x2c0 > 0) {
                if (!(mFlags & 0x80000)) {
                    lbl_8050EC80.getCurrentFile().field_0xc += field_0x2c0;
                    // FAKEMATCH(crack-pass): row view (save_data_803B8C74.h SaveFileRow_CrackPass).
                    SaveFileRow_CrackPass& row = lbl_8050EC80.getCurrentRow_CrackPass();
                    if (row.field_0x2c < 0) {
                        row.field_0x2c = 0;
                    }
                }
                field_0x2c0 = 0;
            }
            ActorBase_804A5770* target = actorBySerial(mTargetSerial);
            if (target != NULL) {
                target->mAlive = false;
            }
            mTargetSerial = -1;
        }
    }
    mLayer_0x4 = 0;
    mDir_0x98 = 0;
    s32 state = mLayer_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = state < 8 ? &view->mVisibleBox : &view->field_0xdc;
    mPos.x = box->i.x - 30.0f;
    state = mLayer_0x4;
    view = getGameView_802D6A9C();
    box = state < 8 ? &view->mVisibleBox : &view->field_0xdc;
    mPos.y = 100.0f + box->i.y;
    mPos.z = 30.0f;
    field_0x54.set(1.0f, 1.0f, 1.0f);
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.set(mBox);
    field_0xb0 = 0;
    mFlags &= 0xFF841FFF;
    field_0x2a8 = -1;
    field_0x2ac = -1;
    field_0x2b0 = -1;
    field_0x2b4 = -1;
    mTargetSerial = -1;
    field_0x2c0 = 0;
    field_0x2cc = randInt_801CD664(0x79) + 0xB4;
    field_0x2ce = 0;
    field_0x2d0 = -1;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
}

// tu_801DE714 (0x801E1DE8): forwards the result of fn_801CCF7C.
bool cls_804A51E4::fn_801E1DE8() {
    return fn_801CCF7C();
}

bool cls_804A51E4::fn_801E1DB4() const {
    bool result = false;
    switch (mAnim.mAnim) {
    case 0:
    case 1:
    case 2:
    case 13:
    case 14:
    case 15:
    case 16:
        result = true;
        break;
    }
    return result;
}

// tu_801DE714 (0x801E1D9C)
bool cls_804A51E4::fn_801E1D9C() const {
    bool result = false;
    switch (mAnim.mAnim) {
    case 2:
        result = true;
        break;
    }
    return result;
}

// tu_801DE714 (0x801E1D7C): whether mState is 0xB or 0xC.
bool cls_804A51E4::fn_801E1D7C() {
    bool result = false;
    switch (mState) {
    case 0xB:
    case 0xC:
        result = true;
        break;
    }
    return result;
}

bool cls_804A51E4::fn_801E1CF8() const {
    bool result = false;
    switch (mState) {
    case 0:
    case 15:
    case 16:
    case 17:
    case 18:
        break;
    default:
        if (!(mFlags & 0x80000) && field_0x2cc == 0) {
            if (fn_801E1DB4()) {
                result = true;
            }
        }
        break;
    }
    return result;
}

// FIXME(shotgun): 98.24 %, regalloc-only; behaviour clean.
void cls_804A51E4::fn_801E1AB4(bool arg) {
    bool found = false;
    u32 kind;
    JGeometry::TVec3f pos;
    if (arg) {
        kind = 2;
        found = true;
        pos.x = mPos.x;
        pos.y = mPos.y;
        pos.z = mPos.z;
    } else {
        ActorBase_804A5770* target = NULL;
        s32 serial = field_0x2ac;
        if (serial >= 0) {
            target = actorBySerial(serial);
        }
        if (target == NULL) {
            serial = field_0x2b0;
            if (serial >= 0) {
                target = actorBySerial(serial);
            }
        }
        if (target == NULL) {
            serial = field_0x2b4;
            if (serial >= 0) {
                target = actorBySerial(serial);
            }
        }
        ForceGem_804AB6D8* gem = (ForceGem_804AB6D8*)target;
        pos.x = gem->mPos.x;
        pos.y = gem->mPos.y;
        pos.z = gem->mPos.z;
        s32 state = gem->field_0x230;
        kind = gem->mSpawnParam_0x90;
        if (state == 0 || (u32)(state - 0xF) <= 1 || (u32)(state - 1) <= 1 || state == 9 || state == 0xD) {
            gem->markDead_801F3668();
        }
        if (!gem->mAlive) {
            found = true;
        }
    }
    if (found) {
        mFlags &= ~0x8000;
        mTargetSerial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, mLayer_0x4, &pos,
                                                                       (kind & 0xFFFF) | 0x10000, -1, -1);
        if (mTargetSerial >= 0) {
            if (!arg && field_0x2b0 >= 0) {
                mFlags |= 0x8000;
            }
            ActorBase_804A5770* gem = actorBySerial(mTargetSerial);
            if (gem != NULL) {
                gem->field_0x194 = 1;
                field_0x2c0 = gem->fn_8032D564(gem->mSpawnParam_0x90 & 0xFF);
                if (arg) {
                    fn_8021FDB8();
                    JGeometry::TBox2f box;
                    box.set(1.0f, 1.0f, -1.0f, -1.0f);
                    gem->mBox = box;
                    JGeometry::TBox2f box2;
                    box2.set(1.0f, 1.0f, -1.0f, -1.0f);
                    gem->mBox2 = box2;
                }
            }
        }
        field_0x2ac = -1;
        field_0x2b0 = -1;
        field_0x2b4 = -1;
    }
}

void cls_804A51E4::setState_801E1594(s32 newState) {
    switch (newState) {
    case 0:
        fn_801E1E08();
        break;
    case 1:
    case 2:
    case 4:
        if (mState == 0 && newState == 1) {
            f32 ty;
            f32 tx;
            if (mFlags & 0x20000) {
                tx = getPlayerPos_8023E724(field_0x2a8)->x;
                ty = getPlayerPos_8023E724(field_0x2a8)->y;
            } else {
                ActorBase_804A5770* target = NULL;
                s32 serial = field_0x2ac;
                if (serial >= 0) {
                    target = actorBySerial(serial);
                }
                if (target == NULL) {
                    serial = field_0x2b0;
                    if (serial >= 0) {
                        target = actorBySerial(serial);
                    }
                }
                if (target == NULL) {
                    serial = field_0x2b4;
                    if (serial >= 0) {
                        target = actorBySerial(serial);
                    }
                }
                tx = target->mPos.x;
                ty = target->mPos.y;
            }
            if (inLeftHalf(mLayer_0x4, tx)) {
                mPos.x = 30.0f + layerBox(mLayer_0x4)->f.x;
                mDir_0x98 = 1;
            } else {
                mPos.x = layerBox(mLayer_0x4)->i.x - 30.0f;
                mDir_0x98 = 0;
            }
            mPos.y = ty;
            mPos.z = 30.0f;
            field_0x2c4.set(mPos.x, mPos.y);
            field_0xb0 = 0x51303;
        }
        field_0x250 = -1;
        if (mTargetSerial >= 0) {
            mTargetSpeed = 0.3f;
        } else if (mFlags & 0x4000) {
            mTargetSpeed = 0.3f;
        } else {
            mTargetSpeed = 0.6f;
        }
        if (mState == 0xD) {
            mAnim.startAnim_801D22DC(mDir_0x98 + 0xD, 0, false);
        } else {
            mAnim.startAnimIfChanged_801D22A0(mDir_0x98, 2);
        }
        break;
    case 3:
        field_0x250 = -1;
        mTargetSpeed = 0.6f;
        if (field_0x2ac >= 0) {
            mAnim.startAnim_801D22DC(mDir_0x98 + 0xF, 2, false);
        } else {
            mAnim.startAnimIfChanged_801D22A0(mDir_0x98, 2);
        }
        break;
    case 5:
    case 6:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(2, 0, false);
        break;
    case 7:
        field_0x250 = randInt_801CD664(1) + 0x5A;
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        field_0x2ce = 0x2D;
        fn_8020D2AC();
        if (!(mFlags & 0x20000)) {
            if (fn_80218538()) {
                startSound_801F0E34(0x40070, 0);
            }
        }
        break;
    case 8:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(mDir_0x98 + 3, 0, false);
        break;
    case 9:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(5, 0, false);
        break;
    case 10:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(6, 0, false);
        break;
    case 11:
        fn_8020D16C();
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(7, 0, false);
        break;
    case 12:
        mDir_0x98 = 2;
        field_0x250 = randInt_801CD664(0x259) + 0x12C;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(mDir_0x98 + 8, 0, false);
        break;
    case 13:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(0xC, 0, false);
        break;
    case 14:
        fn_8020CFC4();
        mVel_0x3c.y = 0.0f;
        mVel_0x3c.x = 0.0f;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2.set(mBox);
        field_0xb0 = 0;
        field_0x250 = -1;
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(6, 0, false);
        break;
    case 15:
        field_0x250 = -1;
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        mAnim.startAnimIfChanged_801D22A0(mDir_0x98, 2);
        break;
    case 16:
        startSound_801F0E34(0x5001D, 0);
        break;
    case 18: // the original's jump table runs to state 18 (nothing to do for it)
    default:
        break;
    }
    mStateTimer = 0;
    mState = newState;
}

// Not auto-inlined: fn_801E0CF4 calls fn_801E1300 out of line in the
// original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 95.15 %, scheduling; behaviour clean.
void cls_804A51E4::fn_801E1300() {
    bool lost = false;
    ActorBase_804A5770* actor;
    s32 serial = field_0x2ac;
    if (serial >= 0) {
        actor = actorBySerial(serial);
        if (actor == NULL || !fn_801CC910(actor)) {
            lost = true;
        }
    } else {
        serial = field_0x2b0;
        if (serial >= 0) {
            actor = actorBySerial(serial);
            if (actor == NULL || !fn_801CC960(actor)) {
                lost = true;
            }
        } else {
            serial = field_0x2b4;
            if (serial >= 0) {
                actor = actorBySerial(serial);
                if (actor == NULL || !fn_801CC9B0(actor)) {
                    lost = true;
                }
            } else {
                findTargets(this);
            }
        }
    }
    if (lost) {
        findTargets(this);
        if (canChase(this)) {
            setState_801E1594(7);
        }
    }
}
#pragma auto_inline reset

// FIXME(shotgun): 99.53 %, regalloc-only; behaviour clean.
void cls_804A51E4::fn_801E0CF4() {
    if (mState == 0) {
        followPlayer(this, -1);
    } else {
        if ((mFlags & 0x4000) && __fabsf(field_0x2c4.x - mPos.x) < 0.1f && __fabsf(field_0x2c4.y - mPos.y) < 0.1f) {
            setState_801E1594(0);
            return;
        }
        if (isUpperLayer_801CD2F8(mLayer_0x4) && !fn_801CD290(mLayer_0x4)) {
            setState_801E1594(0);
            return;
        }
        if (checkFlag(0x10000)) {
            mFlags &= ~0x10000;
            switch (mState) {
            case 5:
            case 6:
                mDir_0x98 = oppositeDir_801CD354(mDir_0x98);
                field_0x2d0 = -1;
                break;
            }
            setState_801E1594(8);
        } else {
            switch (mAnim.mAnim) {
            case 13:
            case 14:
                if (mStateTimer > 1 && mAnim.isStopped_801D21F0()) {
                    mAnim.startAnim_801D22DC(mDir_0x98, 2, false);
                }
                break;
            }
        }
        if (mFlags & 0x20000) {
            switch (mState) {
            case 1:
            case 6:
                if (field_0x2a8 >= 0 && field_0x2a8 != -1) {
                    bool start = false;
                    switch (mState) {
                    case 0:
                    case 7:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                        break;
                    default:
                        if (!(mFlags & 0x4000) && field_0x2cc == 0 && fn_801E1DB4()) {
                            if (mFlags & 0x20000) {
                                if (mFlags & 0x100000) {
                                    start = true;
                                }
                            } else if (mTargetSerial < 0) {
                                start = true;
                            }
                        }
                        break;
                    }
                    if (start) {
                        field_0x2a8 = -1;
                        setState_801E1594(7);
                    }
                }
                break;
            case 15:
            case 16:
            case 17:
                fn_8022FE80(field_0x2a8, mLayer_0x4, mSerial, false, false);
                break;
            }
        }
        fn_8021FDB8();
        turnIfBehind(this);
        if (fn_801E1D7C()) {
            if (fn_801E2F40()) {
                setState_801E1594(0xE);
            } else {
                fn_801E2EE4();
            }
        }
    }
    if (field_0x2a8 == -1 && !(mFlags & 0x20000)) {
        fn_801E1300();
    }
}

// The height it hovers at: higher with flag 0x20000 or while a remembered actor exists.
static inline f32 hoverHeight(cls_804A51E4* a) {
    f32 height = 5.0f;
    if (a->mFlags & 0x20000) {
        height = 20.0f;
    } else {
        s32 serial = a->field_0x2ac;
        if (serial >= 0 && actorBySerial(serial) != NULL) {
            height = 20.0f;
        } else {
            serial = a->field_0x2b0;
            if (serial >= 0 && actorBySerial(serial) != NULL) {
                height = 20.0f;
            }
        }
    }
    return height;
}

// Whether it is in state 3 and has got to the found actor, at its hover height.
static inline bool reachedTarget(cls_804A51E4* a) {
    s32 serial = a->field_0x2ac;
    bool arrived = false;
    ActorBase_804A5770* found = NULL;
    if (serial >= 0) {
        found = actorBySerial(serial);
    }
    if (found == NULL) {
        serial = a->field_0x2b0;
        if (serial >= 0) {
            found = actorBySerial(serial);
        }
    }
    if (found == NULL) {
        serial = a->field_0x2b4;
        if (serial >= 0) {
            found = actorBySerial(serial);
        }
    }
    if (a->mState == 3 && found != NULL && a->field_0x2ac == -1) {
        if (__fabsf(found->mPos.x - a->mPos.x) < 10.0f && __fabsf(found->mPos.y - a->mPos.y) < 10.0f) {
            f32 height = hoverHeight(a);
            if (__fabsf(height - a->mPos.z) < 0.1f) {
                arrived = true;
            }
        }
    }
    return arrived;
}

// Registers and the order of findTargets' argument loads still differ.
// FIXME: 99.32 %, scheduling+regalloc; behaviour clean.
void cls_804A51E4::fn_801E002C(ActorBase_804A5770* target) {
    if (target != NULL && field_0x2ac >= 0) {
        if (mStateTimer >= mAnim.mLength * 2) {
            if (!(mFlags & 0x20000)) {
                if (fn_80218538()) {
                    startSound_801F0E34(0x40070, 0);
                }
            }
            if (!checkFlag(0x2000)) {
                field_0x2b8 = field_0x2ac;
                findTargets(this);
                ActorBase_804A5770* found = NULL;
                s32 serial = field_0x2ac;
                if (serial >= 0) {
                    found = actorBySerial(serial);
                }
                if (found == NULL) {
                    serial = field_0x2b0;
                    if (serial >= 0) {
                        found = actorBySerial(serial);
                    }
                }
                if (found == NULL) {
                    serial = field_0x2b4;
                    if (serial >= 0) {
                        found = actorBySerial(serial);
                    }
                }
                if (found != NULL) {
                    mFlags |= 0x2000;
                    if (behind(this, found->mPos.x)) {
                        setState_801E1594(6);
                    } else {
                        setState_801E1594(1);
                    }
                } else {
                    enterFromEdge(this);
                }
            } else {
                enterFromEdge(this);
            }
        } else {
            s32 t = mStateTimer - 0xD;
            if (t >= 0 && t % mAnim.mLength == 0) {
                fn_8020CF7C();
            }
        }
        return;
    }
    if (reachedTarget(this)) {
        if (!fn_801CCF7C()) {
            startSound_801F0E34(0x50019, 0);
        }
        fn_801E1AB4(false);
        enterFromEdge(this);
    } else if (mStateTimer >= 0xB4) {
        enterFromEdge(this);
    }
}

// FIXME(shotgun): 99.94 %, regalloc-only; behaviour clean.
void cls_804A51E4::fn_801DF9D8(ActorBase_804A5770* target) {
    if (mStateTimer < field_0x250) {
        return;
    }
    if (checkFlag(0x20000)) {
        field_0x2a8 = -1;
        if (!(mFlags & 0x2000)) {
            field_0x2a8 = -1;
        }
        if (field_0x2a8 >= 0) {
            mFlags |= 0x2000;
            if (behind(this, getPlayerPos_8023E724(field_0x2a8)->x)) {
                setState_801E1594(6);
            } else {
                setState_801E1594(1);
            }
        } else {
            enterFromEdge(this);
        }
    } else {
        if (!checkFlag(0x2000) && target != NULL) {
            mFlags |= 0x2000;
            if (behind(this, target->mPos.x)) {
                setState_801E1594(6);
            } else {
                setState_801E1594(1);
            }
        } else {
            enterFromEdge(this);
        }
    }
}

// FIXME(shotgun): 99.86 %, regalloc-only; behaviour near.
void cls_804A51E4::updateState_0x90(u8 mapHit) {
    if (field_0x2cc == 0) {
        fn_801E0CF4();
    }
    bool blocked = fn_801CCF7C();
    ActorBase_804A5770* target = NULL;
    s32 serial = field_0x2ac;
    if (serial >= 0) {
        target = actorBySerial(serial);
    }
    if (target == NULL) {
        serial = field_0x2b0;
        if (serial >= 0) {
            target = actorBySerial(serial);
        }
    }
    if (target == NULL) {
        serial = field_0x2b4;
        if (serial >= 0) {
            target = actorBySerial(serial);
        }
    }
    switch (mState) {
    case 0:
        if (!blocked) {
            if (field_0x2cc != 0) {
                field_0x2cc--;
                break;
            }
            if (field_0x2a8 >= 0) {
                mFlags |= 0x120000;
                setState_801E1594(1);
                break;
            }
            if (!(mFlags & 0x40000)) {
                s32 s = field_0x2ac;
                if (!(s >= 0 && actorBySerial(s) != NULL)) {
                    s = field_0x2b0;
                    if (!(s >= 0 && actorBySerial(s) != NULL)) {
                        break;
                    }
                }
                if (!fn_801E3250(mLayer_0x4, false)) {
                    setState_801E1594(1);
                }
            }
        }
        break;
    case 1:
        if (mFlags & 0x20000) {
            if (mFlags & 0x4000) {
                break;
            }
            s32 player = field_0x2a8;
            if (player >= 0) {
                if (canTalkTo(player)) {
                    if (getPlayerLayer_8023DE58(field_0x2a8) == getField_0x4()) {
                        if (__fabsf(getPlayerPos_8023E724(field_0x2a8)->x - mPos.x) < 5.0f &&
                            __fabsf(getPlayerPos_8023E724(field_0x2a8)->y - mPos.y) < 5.0f) {
                            fn_8022FE80(field_0x2a8, mLayer_0x4, mSerial, false, false);
                            if (field_0x2a8 == fn_8022FD8C(mSerial) && fn_802308E0(field_0x2a8)) {
                                if (openMessage_8038AD24(field_0x2a8, 0xF0, mSerial, mLayer_0x4,
                                                         JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 4)) {
                                    setState_801E1594(0xF);
                                } else {
                                    fn_8023061C(field_0x2a8, false);
                                }
                            }
                        }
                    }
                }
            }
        } else {
            if (mTargetSerial >= 0) {
                break;
            }
            if (mFlags & 0x4000) {
                break;
            }
            if (target != NULL) {
                f32 dx = target->mPos.x - mPos.x;
                f32 dy = target->mPos.y - mPos.y;
                if (square(dx) + square(dy) <= 0.36f) {
                    setState_801E1594(2);
                }
            }
        }
        break;
    case 2: {
        f32 height = hoverHeight(this);
        if (mPos.z <= height) {
            mPos.z = height;
            setState_801E1594(3);
        }
        break;
    }
    case 3:
        fn_801E002C(target);
        break;
    case 4:
        if (mPos.z >= 30.0f) {
            mPos.z = 30.0f;
            setState_801E1594(1);
        }
        break;
    case 5:
        if (mStateTimer > 1 && mAnim.isStopped_801D21F0()) {
            mDir_0x98 = oppositeDir_801CD354(mDir_0x98);
            if (mPos.z >= 30.0f) {
                mPos.z = 30.0f;
                setState_801E1594(1);
            } else {
                setState_801E1594(4);
            }
        }
        break;
    case 6:
        if (mStateTimer > 1 && mAnim.isStopped_801D21F0()) {
            mDir_0x98 = oppositeDir_801CD354(mDir_0x98);
            if (field_0x2d0 >= 0) {
                setState_801E1594(field_0x2d0);
                field_0x2d0 = -1;
            } else {
                setState_801E1594(1);
            }
        }
        break;
    case 7:
        fn_801DF9D8(target);
        break;
    case 8:
        if (mStateTimer > 1 && mAnim.isStopped_801D21F0()) {
            setState_801E1594(9);
        }
        break;
    case 9:
        if (mStateTimer > 1 && mAnim.isStopped_801D21F0()) {
            setState_801E1594(10);
        }
        break;
    case 10:
        if (mStateTimer > 1 && mAnim.isStopped_801D21F0()) {
            if (isOnGround_801D0DD4()) {
                if (fn_801E2F40()) {
                    setState_801E1594(14);
                } else {
                    setState_801E1594(11);
                }
            }
        }
        break;
    case 11:
        if (mStateTimer > 1 && mAnim.isStopped_801D21F0()) {
            setState_801E1594(12);
        }
        break;
    case 12:
        if (!blocked) {
            if (mAnim.isStopped_801D21F0()) {
                if (mStateTimer >= field_0x250) {
                    setState_801E1594(13);
                } else {
                    s32 player = findNearestPlayer_801D05F8(&mPos, 0.0f);
                    if (player >= 0) {
                        s32 dir = fn_801D0020(&mPos, player, false);
                        if (dir != mDir_0x98) {
                            mDir_0x98 = dir;
                            mAnim.startAnim_801D22DC(mDir_0x98 + 8, 0, false);
                        }
                    }
                }
            }
        }
        break;
    case 13:
        if (mStateTimer > 1 && mAnim.isStopped_801D21F0()) {
            enterFromEdge(this);
        }
        break;
    case 14:
        if (field_0x54.x != 0.0f) {
            moveToward_801CD950(&field_0x54.x, 0.0f, 0.2f);
            field_0x54.y = field_0x54.x;
            field_0x54.z = field_0x54.x;
        } else {
            setState_801E1594(0);
        }
        break;
    case 15:
        if (stepMessageWindow_803865B8(field_0x2a8)) {
            if (fn_8037EFB4(field_0x2a8) == 0) {
                if (fn_80236228(field_0x2a8) < 0x14) {
                    if (fn_8038470C(field_0x2a8, 0xF1)) {
                        setState_801E1594(0x11);
                    }
                } else if (fn_80384C58(field_0x2a8, mSerial)) {
                    setState_801E1594(0x12);
                }
            } else if (fn_8038470C(field_0x2a8, 0xF2)) {
                setState_801E1594(0x10);
            }
        }
        break;
    case 16:
        if (stepMessageWindow_803865B8(field_0x2a8) && fn_80384C58(field_0x2a8, mSerial)) {
            fn_8023061C(field_0x2a8, false);
            enterFromEdge(this);
        }
        break;
    case 17:
        if (stepMessageWindow_803865B8(field_0x2a8) && fn_80384C58(field_0x2a8, mSerial)) {
            setState_801E1594(0x12);
        }
        break;
    case 18: {
        f32 height = hoverHeight(this);
        if (mPos.z <= height) {
            mPos.z = height;
            fn_80237524(field_0x2a8, 0x180);
            fn_8023619C(field_0x2a8, -0x14);
            fn_8023061C(field_0x2a8, false);
            mFlags &= ~0x100000;
            mFlags |= 0x80000;
            fn_801E1AB4(true);
            startSound_801F0E34(0x5001B, 0);
            enterFromEdge(this);
        }
        break;
    }
    }
    fn_801E2D88();
}

void cls_804A51E4::updateAnim_0x94() {
    if (field_0x2ce != 0) {
        field_0x2ce--;
    }
    if (field_0x2ce == 0) {
        if (mState == 7) {
            switch (mAnim.mAnim) {
            case 0xF:
            case 0x10:
                mAnim.startAnim_801D22DC(mDir_0x98, 2, false);
                return;
            }
        }
        EnemyBase_804A4808::updateAnim_0x94();
    }
}


// tu_801DE714 (0x801DE9C0): vtable slot 0x9C. The step is EnemyBase's
// fn_801CE2BC (a plain bl), then the view check.
void cls_804A51E4::updateBehavior_0x9C(u8 mapHit) {
    EnemyBase_804A4808::fn_801CE2BC(mapHit);
    fn_8021835C();
}

void cls_804A51E4::calcVelocity_0x88() {
    bool blocked = fn_801CCF7C();
    mVel_0x3c.x = 0.0f;
    mVel_0x3c.y = 0.0f;
    if (mState != 0xA) {
        mVel_0x3c.z = 0.0f;
    }
    if (mState != 0) {
        if (mSpeed != 0.0f && !blocked) {
            JGeometry::TVec2f d(0.0f, 0.0f);
            if (mFlags & 0x4000) {
                calcVelocityToward_801D01A0((const JGeometry::TVec3f*)&field_0x2c4, &d, mSpeed);
            } else if (mFlags & 0x20000) {
                if (field_0x2a8 >= 0) {
                    fn_801D00B4(field_0x2a8, &d, mSpeed);
                }
            } else {
                ActorBase_804A5770* target = NULL;
                s32 serial = field_0x2ac;
                if (serial >= 0) {
                    target = actorBySerial(serial);
                }
                if (target == NULL) {
                    serial = field_0x2b0;
                    if (serial >= 0) {
                        target = actorBySerial(serial);
                    }
                }
                if (target == NULL) {
                    serial = field_0x2b4;
                    if (serial >= 0) {
                        target = actorBySerial(serial);
                    }
                }
                if (target != NULL) {
                    calcVelocityToward_801D01A0(&target->mPos, &d, mSpeed);
                }
            }
            mVel_0x3c.x = d.x;
            mVel_0x3c.y = d.y;
        }
        switch (mState) {
        case 2:
            if (!blocked) {
                mVel_0x3c.z = -0.15f;
            }
            break;
        case 4:
            if (!blocked) {
                if (mFlags & 0x80000) {
                    mVel_0x3c.z = 0.1f;
                } else {
                    mVel_0x3c.z = mTargetSerial >= 0 ? 0.15f : 0.3f;
                }
            }
            break;
        case 10:
            mVel_0x3c.z += -0.1f;
            if (mVel_0x3c.z < -lbl_8053EFBC) {
                mVel_0x3c.z = -lbl_8053EFBC;
            }
            break;
        case 18:
            if (!blocked) {
                mVel_0x3c.z = -0.1f;
            }
            break;
        }
    }
}

void cls_804A51E4::onMoved_0x8C() {
    if (fn_801E1D7C()) {
        if (!isUpperLayer_801CD2F8(mLayer_0x4)) {
            fn_80212AC0()->fn_80212AF0(&mPos);
        }
    }
}


// tu_801DE714 (0x801DE714): vtable slot 0x80. In state 0xA the floor check
// is EnemyBase's, called directly (a plain bl, not through the vtable).
void cls_804A51E4::checkFloor_0x80() {
    if (mState == 0xA) {
        EnemyBase_804A4808::checkFloor_0x80();
    }
}
