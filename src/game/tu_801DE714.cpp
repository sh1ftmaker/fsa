#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/tu_80212AC0.h"
#include "game/act/act_force_gem.h"
#include "game/act/act_tingle_buster.h"
#include "game/act/tu_801DE714.h"
#include "game/tu_803B8C74.h"

extern const s16* lbl_804A51A0[]; // the animation tables (.data, this unit)
s32 fn_801CD664(s32 n);
bool fn_801CD290(s32 layer);

// Whether x lies behind this actor in the direction it faces.
static inline bool behind(cls_804A51E4* a, f32 x) {
    bool result = false;
    switch (a->field_0x98) {
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
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = a->field_0x4 < 8 ? &view->mVisibleBox : &view->field_0xdc;
    f32 right = 30.0f + box->f.x;
    f32 left = box->i.x - 30.0f;
    f32 px = a->mPos.x;
    bool leftHalf = false;
    view = getGameView_802D6A9C();
    box = a->field_0x4 < 8 ? &view->mVisibleBox : &view->field_0xdc;
    if (px < 0.5f * (box->i.x + box->f.x)) {
        leftHalf = true;
    }
    if (leftHalf && left < a->mPos.x) {
        a->field_0x2c4.x = right;
    } else {
        a->field_0x2c4.x = left;
    }
    switch (a->mState) {
    case 0xD:
        if (a->mPos.x < a->field_0x2c4.x) {
            a->field_0x98 = 0;
        } else {
            a->field_0x98 = 1;
        }
        if (a->mPos.z >= 30.0f) {
            a->mPos.z = 30.0f;
            a->fn_801E1594(1);
        } else {
            a->fn_801E1594(4);
        }
        break;
    case 7: {
        bool cross = behind(a, right);
        if (cross) {
            a->field_0x2c4.x = right;
        } else {
            a->field_0x2c4.x = left;
        }
        a->fn_801E1594(5);
        break;
    }
    default:
        if (behind(a, a->field_0x2c4.x)) {
            a->fn_801E1594(5);
        } else if (a->mPos.z >= 30.0f) {
            a->mPos.z = 30.0f;
            a->fn_801E1594(1);
        } else {
            a->fn_801E1594(4);
        }
        break;
    }
}

// Looks the actors up again: the nearest of the players' kind, then the others.
static inline void findTargets(cls_804A51E4* a) {
    a->field_0x2ac = getActorManager_801F856C()->fn_801F750C(a->field_0x4, a->field_0x2b8);
    if (a->field_0x2ac >= 0) {
        a->field_0x2b0 = -1;
        a->field_0x2b4 = -1;
    } else {
        a->field_0x2b0 = getActorManager_801F856C()->fn_801F7464(a->field_0x4, -1);
        if (a->field_0x2b0 >= 0) {
            a->field_0x2b4 = -1;
        } else {
            a->field_0x2b4 =
                getActorManager_801F856C()->fn_801F732C(a->field_0x4, &a->mPos, &a->mPos, a->field_0x2b8, 500.0f);
        }
    }
}


// tu_801DE714 (0x801E22A4): the constructor, after EnemyBase's (a plain bl).
// FIXME: 99.8 %. Only the literal label of 0.0f (@NNN vs lbl_; data split).
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
                // FAKEMATCH(crack-pass): row view (tu_803B8C74.h SaveFileRow_CrackPass).
                SaveFileRow_CrackPass& row = lbl_8050EC80.getCurrentRow_CrackPass();
                if (row.field_0x2c < 0) {
                    row.field_0x2c = 0;
                }
            }
            field_0x2c0 = 0;
        }
        ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
        if (target != NULL) {
            target->mAlive = false;
        }
        mTargetSerial = -1;
    }
}

// FIXME(shotgun): 80.67 %, missing/extra + reloc-name
void cls_804A51E4::init() {
    EnemyBase_804A4808::init();
    mBox2.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2 = mBox;
    field_0xb0 = 0;
    field_0x98 = 0;
    mFlags |= 0x300;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x194 = 1;
    field_0x4 = 0;
    field_0x2cc = fn_801CD664(0x3D) + 0x3C;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A51A0, 0x11);
    mAnim.startAnim_801D22DC(0, 2, false);
}

// tu_801DE714 (0x801E2004, vtable slot 0x54, EnemyBase's start hook): see the
// declaration in act/tu_801DE714.h.
// FIXME: 99.8 %. Only the literal label (30.0f is @NNN, not lbl_8053F4A4:
// the unit's earlier .sdata2 users are not written yet).
void cls_804A51E4::vfunc_0x54() {
    getActorManager_801F856C()->fn_801F7AB8(mActorId, mSerial);
    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    mPos.x = box->i.x - 30.0f;
    field_0x2c4.set(mPos.x, mPos.y);
}

// FIXME(shotgun): 90.20 %, missing/extra + reloc-name
void cls_804A51E4::fn_801E1E08() {
    if (mTargetSerial >= 0) {
        if (!(mFlags & 0x80000)) {
            mFlags |= 0x40000;
        }
        if (mTargetSerial >= 0) {
            if (field_0x2c0 > 0) {
                if (!(mFlags & 0x80000)) {
                    lbl_8050EC80.getCurrentFile().field_0xc += field_0x2c0;
                    // FAKEMATCH(crack-pass): row view (tu_803B8C74.h SaveFileRow_CrackPass).
                    SaveFileRow_CrackPass& row = lbl_8050EC80.getCurrentRow_CrackPass();
                    if (row.field_0x2c < 0) {
                        row.field_0x2c = 0;
                    }
                }
                field_0x2c0 = 0;
            }
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
            if (target != NULL) {
                target->mAlive = false;
            }
            mTargetSerial = -1;
        }
    }
    field_0x4 = 0;
    field_0x98 = 0;
    GameView_804A9950* view = getGameView_802D6A9C();
    JGeometry::TBox2f* box = field_0x4 < 8 ? &view->mVisibleBox : &view->field_0xdc;
    mPos.x = box->i.x - 30.0f;
    view = getGameView_802D6A9C();
    box = field_0x4 < 8 ? &view->mVisibleBox : &view->field_0xdc;
    mPos.y = 100.0f + box->i.y;
    mPos.z = 30.0f;
    field_0x54.set(1.0f, 1.0f, 1.0f);
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2 = mBox;
    field_0xb0 = 0;
    mFlags &= 0xFF841FFF;
    field_0x2a8 = -1;
    field_0x2ac = -1;
    field_0x2b0 = -1;
    field_0x2b4 = -1;
    mTargetSerial = -1;
    field_0x2c0 = 0;
    field_0x2cc = fn_801CD664(0x79) + 0xB4;
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

// FIXME(shotgun): 59.00 %, missing/extra
bool cls_804A51E4::fn_801E1DB4() const {
    bool result = false;
    s32 anim = mAnim.mAnim;
    if (anim >= 0xD) {
        if (anim >= 0x11) {
            return false;
        }
        result = true;
    } else {
        if (anim >= 3) {
            return false;
        }
        if (anim >= 0) {
            result = true;
        }
    }
    return result;
}

// tu_801DE714 (0x801E1D9C)
bool cls_804A51E4::fn_801E1D9C() const {
    bool result = false;
    if (mAnim.mAnim == 2) {
        result = true;
    }
    return result;
}

// tu_801DE714 (0x801E1D7C): whether mState is 0xB or 0xC.
bool cls_804A51E4::fn_801E1D7C() {
    bool result = false;
    if (mState < 0xD && mState >= 0xB) {
        result = true;
    }
    return result;
}

// FIXME(shotgun): 77.61 %, missing/extra
bool cls_804A51E4::fn_801E1CF8() const {
    bool result = false;
    if ((mState < 0xF && mState != 0) || (mState >= 0xF && mState >= 0x13)) {
        if (!(mFlags & 0x80000) && field_0x2cc == 0) {
            if (fn_801E1DB4()) {
                result = true;
            }
        }
    }
    return result;
}

// FIXME(shotgun): 88.71 %, missing/extra + reloc-name
void cls_804A51E4::fn_801E1AB4(bool arg) {
    bool found = false;
    u32 kind;
    JGeometry::TVec3f pos;
    if (arg) {
        kind = 2;
        found = true;
        pos = mPos;
    } else {
        ActorBase_804A5770* target = NULL;
        s32 serial = field_0x2ac;
        if (serial >= 0) {
            target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        }
        if (target == NULL) {
            serial = field_0x2b0;
            if (serial >= 0) {
                target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            }
        }
        if (target == NULL) {
            serial = field_0x2b4;
            if (serial >= 0) {
                target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            }
        }
        pos = target->mPos;
        s32 state = ((ForceGem_804AB6D8*)target)->field_0x230;
        kind = target->field_0x90;
        if (state == 0 || (u32)(state - 0xF) <= 1 || (u32)(state - 1) <= 1 || state == 9 || state == 0xD) {
            target->fn_801F3668();
        }
        if (!target->mAlive) {
            found = true;
        }
    }
    if (found) {
        mFlags &= ~0x8000;
        mTargetSerial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &pos,
                                                                       (kind & 0xFFFF) | 0x10000, -1, -1);
        if (mTargetSerial >= 0) {
            if (!arg && field_0x2b0 >= 0) {
                mFlags |= 0x8000;
            }
            ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(mTargetSerial);
            if (gem != NULL) {
                gem->field_0x194 = 1;
                field_0x2c0 = gem->fn_8032D564(gem->field_0x90 & 0xFF);
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

// FIXME(shotgun): 97.79 %, different + reloc-name
void cls_804A51E4::fn_801E1594(s32 newState) {
    switch (newState) {
    case 0:
        fn_801E1E08();
        break;
    case 1:
    case 2:
    case 4:
        if (mState == 0 && newState == 1) {
            f32 tx;
            f32 ty;
            if (mFlags & 0x20000) {
                tx = getPlayerPos_8023E724(field_0x2a8)->x;
                ty = getPlayerPos_8023E724(field_0x2a8)->y;
            } else {
                ActorBase_804A5770* target = NULL;
                s32 serial = field_0x2ac;
                if (serial >= 0) {
                    target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                }
                if (target == NULL) {
                    serial = field_0x2b0;
                    if (serial >= 0) {
                        target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    }
                }
                if (target == NULL) {
                    serial = field_0x2b4;
                    if (serial >= 0) {
                        target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    }
                }
                tx = target->mPos.x;
                ty = target->mPos.y;
            }
            s32 layer = field_0x4;
            bool left = false;
            GameView_804A9950* view = getGameView_802D6A9C();
            JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
            if (tx < 0.5f * (box->i.x + box->f.x)) {
                left = true;
            }
            if (left) {
                layer = field_0x4;
                view = getGameView_802D6A9C();
                box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
                mPos.x = 30.0f + box->f.x;
                field_0x98 = 1;
            } else {
                layer = field_0x4;
                view = getGameView_802D6A9C();
                box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
                mPos.x = box->i.x - 30.0f;
                field_0x98 = 0;
            }
            mPos.y = ty;
            mPos.z = 30.0f;
            field_0x2c4.set(mPos.x, mPos.y);
            field_0xb0 = 0x51303;
        }
        field_0x250 = -1;
        if (mTargetSerial >= 0) {
            mTargetSpeed = 0.3f;
        } else if (mFlags & 0x20000) {
            mTargetSpeed = 0.3f;
        } else {
            mTargetSpeed = 0.6f;
        }
        if (mState == 0xD) {
            mAnim.startAnim_801D22DC(field_0x98 + 0xD, 0, false);
        } else {
            mAnim.fn_801D22A0(field_0x98, 2);
        }
        break;
    case 3:
        field_0x250 = -1;
        mTargetSpeed = 0.6f;
        if (field_0x2ac >= 0) {
            mAnim.startAnim_801D22DC(field_0x98 + 0xF, 2, false);
        } else {
            mAnim.fn_801D22A0(field_0x98, 2);
        }
        break;
    case 5:
    case 6:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(2, 0, false);
        break;
    case 7:
        field_0x250 = fn_801CD664(1) + 0x5A;
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
        mAnim.startAnim_801D22DC(field_0x98 + 3, 0, false);
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
        field_0x98 = 2;
        field_0x250 = fn_801CD664(0x259) + 0x12C;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(field_0x98 + 8, 0, false);
        break;
    case 13:
        field_0x250 = -1;
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(0xC, 0, false);
        break;
    case 14:
        fn_8020CFC4();
        field_0x3c.y = 0.0f;
        field_0x3c.x = 0.0f;
        mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
        mBox2 = mBox;
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
        mAnim.fn_801D22A0(field_0x98, 2);
        break;
    case 16:
        startSound_801F0E34(0x5001D, 0);
        break;
    default:
        break;
    }
    mStateTimer = 0;
    mState = newState;
}

// FIXME(shotgun): 92.39 %, missing/extra
void cls_804A51E4::fn_801E1300() {
    bool lost = false;
    ActorBase_804A5770* actor;
    s32 serial = field_0x2ac;
    if (serial >= 0) {
        actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL || !fn_801CC910(actor)) {
            lost = true;
        }
    } else {
        serial = field_0x2b0;
        if (serial >= 0) {
            actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor == NULL || !fn_801CC960(actor)) {
                lost = true;
            }
        } else {
            serial = field_0x2b4;
            if (serial >= 0) {
                actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
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
        bool start = false;
        if (mState != 7 && mState != 0 && !(mState >= 0xF && mState < 0x13)) {
            if (!(mFlags & 0x4000) && field_0x2cc == 0) {
                if (fn_801E1DB4()) {
                    if (mFlags & 0x20000) {
                        if (mFlags & 0x100000) {
                            start = true;
                        }
                    } else if (mTargetSerial < 0) {
                        start = true;
                    }
                }
            }
        }
        if (start) {
            fn_801E1594(7);
        }
    }
}

// FIXME(shotgun): 89.89 %, missing/extra + reloc-name
void cls_804A51E4::fn_801E0CF4() {
    if (mState == 0) {
        field_0x2a8 = -1;
        if (field_0x2a8 >= 0) {
            field_0x4 = getPlayerLayer_8023DE58(field_0x2a8);
        } else {
            s32 count = 0;
            s32 layers[4];
            for (s32 i = 0; i < 4; i++) {
                s32 layer = getPlayerLayer_8023DE58(i);
                if (fn_801CD2F8(layer) && layer != 8 && !fn_801E3250(layer, false)) {
                    if (getActorManager_801F856C()->fn_801F750C(layer, -1) >= 0 ||
                        getActorManager_801F856C()->fn_801F7464(layer, -1) >= 0) {
                        layers[count] = layer;
                        count++;
                    }
                }
            }
            if (count == 0) {
                field_0x4 = 0;
            } else {
                field_0x4 = layers[fn_801CD664(count)];
            }
        }
    } else {
        if ((mFlags & 0x4000) && fabs(field_0x2c4.x - mPos.x) < 0.1f && fabs(field_0x2c4.y - mPos.y) < 0.1f) {
            fn_801E1594(0);
            return;
        }
        if (fn_801CD2F8(field_0x4) && !fn_801CD290(field_0x4)) {
            fn_801E1594(0);
            return;
        }
        if (mFlags & 0x10000) {
            mFlags &= ~0x10000;
            if (mState < 7 && mState >= 5) {
                field_0x98 = fn_801CD354(field_0x98);
                field_0x2d0 = -1;
            }
            fn_801E1594(8);
        } else if (mAnim.mAnim < 0xF && mAnim.mAnim >= 0xD) {
            if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
                mAnim.startAnim_801D22DC(field_0x98, 2, false);
            }
        }
        if (mFlags & 0x20000) {
            switch (mState) {
            case 1:
            case 6:
                if (field_0x2a8 >= 0 && field_0x2a8 != -1) {
                    bool start = false;
                    if (mState != 7 && mState != 0 && !(mState >= 0xF && mState < 0x13)) {
                        if (!(mFlags & 0x4000) && field_0x2cc == 0 && fn_801E1DB4()) {
                            if (mFlags & 0x20000) {
                                if (mFlags & 0x100000) {
                                    start = true;
                                }
                            } else if (mTargetSerial < 0) {
                                start = true;
                            }
                        }
                    }
                    if (start) {
                        field_0x2a8 = -1;
                        fn_801E1594(7);
                    }
                }
                break;
            case 15:
            case 16:
            case 17:
                fn_8022FE80(field_0x2a8, field_0x4, mSerial, false, false);
                break;
            }
        }
        fn_8021FDB8();
        bool cross = false;
        if (mState == 0 || (mState >= 5 && mState < 8) || (mState >= 0xF && mState < 0x13)) {
            // no crossing check
        } else if (fn_801E1DB4() && mAnim.mAnim != 2) {
            if (mFlags & 0x4000) {
                cross = behind(this, field_0x2c4.x);
            } else if (mFlags & 0x20000) {
                s32 player = field_0x2a8;
                if (player >= 0 && player == -1) {
                    cross = behind(this, getPlayerPos_8023E724(player)->x);
                }
            } else {
                ActorBase_804A5770* t = NULL;
                s32 serial = field_0x2ac;
                if (serial >= 0) {
                    t = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                }
                if (t == NULL) {
                    serial = field_0x2b0;
                    if (serial >= 0) {
                        t = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    }
                }
                if (t == NULL) {
                    serial = field_0x2b4;
                    if (serial >= 0) {
                        t = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    }
                }
                if (t != NULL) {
                    cross = behind(this, t->mPos.x);
                }
            }
            if (cross) {
                field_0x2d0 = mState;
                fn_801E1594(6);
            }
        }
        if (fn_801E1D7C()) {
            if (fn_801E2F40()) {
                fn_801E1594(0xE);
            } else {
                fn_801E2EE4();
            }
        }
    }
    if (field_0x2a8 == -1 && !(mFlags & 0x20000)) {
        fn_801E1300();
    }
}

// FIXME(shotgun): 95.70 %, missing/extra + reloc-name
void cls_804A51E4::fn_801E002C(ActorBase_804A5770* target) {
    if (target != NULL && field_0x2ac >= 0) {
        if (mStateTimer >= mAnim.mLength * 2) {
            if (!(mFlags & 0x4000)) {
                if (fn_80218538()) {
                    startSound_801F0E34(0x40070, 0);
                }
            }
            if (!(mFlags & 0x2000)) {
                field_0x2b8 = field_0x2ac;
                findTargets(this);
                ActorBase_804A5770* found = NULL;
                s32 serial = field_0x2ac;
                if (serial >= 0) {
                    found = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                }
                if (found == NULL) {
                    serial = field_0x2b0;
                    if (serial >= 0) {
                        found = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    }
                }
                if (found == NULL) {
                    serial = field_0x2b4;
                    if (serial >= 0) {
                        found = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    }
                }
                if (found != NULL) {
                    mFlags |= 0x2000;
                    if (behind(this, found->mPos.x)) {
                        fn_801E1594(6);
                    } else {
                        fn_801E1594(1);
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
    bool arrived = false;
    ActorBase_804A5770* found = NULL;
    s32 serial = field_0x2ac;
    if (serial >= 0) {
        found = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    }
    if (found == NULL) {
        serial = field_0x2b0;
        if (serial >= 0) {
            found = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        }
    }
    if (found == NULL) {
        serial = field_0x2b4;
        if (serial >= 0) {
            found = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        }
    }
    if (mState == 3 && found != NULL && field_0x2ac == -1) {
        if (fabs(found->mPos.x - mPos.x) < 10.0f && fabs(found->mPos.y - mPos.y) < 10.0f) {
            f32 height = 5.0f;
            if (mFlags & 0x20000) {
                height = 20.0f;
            } else if (field_0x2ac >= 0 && getActorManager_801F856C()->findActorBySerial_801F666C(field_0x2ac) != NULL) {
                height = 20.0f;
            } else if (field_0x2b0 >= 0 && getActorManager_801F856C()->findActorBySerial_801F666C(field_0x2b0) != NULL) {
                height = 20.0f;
            }
            if (fabs(height - mPos.z) < 0.1f) {
                arrived = true;
            }
        }
    }
    if (arrived) {
        if (!fn_801CCF7C()) {
            startSound_801F0E34(0x50019, 0);
        }
        fn_801E1AB4(false);
        enterFromEdge(this);
    }
    if (mStateTimer >= 0xB4) {
        enterFromEdge(this);
    }
}

// FIXME(shotgun): 96.58 %, missing/extra + reloc-name
void cls_804A51E4::fn_801DF9D8(ActorBase_804A5770* target) {
    if (mStateTimer < field_0x250) {
        return;
    }
    if (mFlags & 0x20000) {
        field_0x2a8 = -1;
        if (!(mFlags & 0x2000)) {
            field_0x2a8 = -1;
        }
        if (field_0x2a8 >= 0) {
            mFlags |= 0x2000;
            if (behind(this, getPlayerPos_8023E724(field_0x2a8)->x)) {
                fn_801E1594(6);
            } else {
                fn_801E1594(1);
            }
        } else {
            enterFromEdge(this);
        }
    } else {
        if (!(mFlags & 0x2000) && target != NULL) {
            mFlags |= 0x2000;
            if (behind(this, target->mPos.x)) {
                fn_801E1594(6);
            } else {
                fn_801E1594(1);
            }
        } else {
            enterFromEdge(this);
        }
    }
}

// FIXME(shotgun): 95.65 %, missing/extra + reloc-name
void cls_804A51E4::vfunc_0x90(u8 mapHit) {
    if (field_0x2cc == 0) {
        fn_801E0CF4();
    }
    bool blocked = fn_801CCF7C();
    ActorBase_804A5770* target = NULL;
    s32 serial = field_0x2ac;
    if (serial >= 0) {
        target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    }
    if (target == NULL) {
        serial = field_0x2b0;
        if (serial >= 0) {
            target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        }
    }
    if (target == NULL) {
        serial = field_0x2b4;
        if (serial >= 0) {
            target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
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
                fn_801E1594(1);
                break;
            }
            if (!(mFlags & 0x40000)) {
                ActorBase_804A5770* found = NULL;
                s32 s = field_0x2ac;
                if (s >= 0) {
                    found = getActorManager_801F856C()->findActorBySerial_801F666C(s);
                }
                if (found == NULL) {
                    s = field_0x2b0;
                    if (s < 0) {
                        break;
                    }
                    found = getActorManager_801F856C()->findActorBySerial_801F666C(s);
                    if (found == NULL) {
                        break;
                    }
                }
                if (!fn_801E3250(field_0x4, false)) {
                    fn_801E1594(1);
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
                bool ok = false;
                bool cond = false;
                if (fn_802375B0(player) == 0 && fn_801CCFE4(player)) {
                    cond = true;
                }
                if (cond && player < gGameRoot_8053AAF8->fn_80138A30()) {
                    ok = true;
                }
                if (ok) {
                    if (getPlayerLayer_8023DE58(field_0x2a8) == field_0x4) {
                        if (fabs(getPlayerPos_8023E724(field_0x2a8)->x - mPos.x) < 5.0f &&
                            fabs(getPlayerPos_8023E724(field_0x2a8)->y - mPos.y) < 5.0f) {
                            fn_8022FE80(field_0x2a8, field_0x4, mSerial, false, false);
                            if (field_0x2a8 == fn_8022FD8C(mSerial) && fn_802308E0(field_0x2a8)) {
                                JGeometry::TVec3f zero(0.0f, 0.0f, 0.0f);
                                if (fn_8038AD24(field_0x2a8, 0xF0, mSerial, field_0x4, zero, 4)) {
                                    fn_801E1594(0xF);
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
                if (dx * dx + dy * dy <= 0.36f) {
                    fn_801E1594(2);
                }
            }
        }
        break;
    case 2: {
        f32 height = 5.0f;
        if (mFlags & 0x20000) {
            height = 20.0f;
        } else if (field_0x2ac >= 0 && getActorManager_801F856C()->findActorBySerial_801F666C(field_0x2ac) != NULL) {
            height = 20.0f;
        } else if (field_0x2b0 >= 0 && getActorManager_801F856C()->findActorBySerial_801F666C(field_0x2b0) != NULL) {
            height = 20.0f;
        }
        if (mPos.z <= height) {
            mPos.z = height;
            fn_801E1594(3);
        }
        break;
    }
    case 3:
        fn_801E002C(target);
        break;
    case 4:
        if (mPos.z >= 30.0f) {
            mPos.z = 30.0f;
            fn_801E1594(1);
        }
        break;
    case 5:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            field_0x98 = fn_801CD354(field_0x98);
            if (mPos.z >= 30.0f) {
                mPos.z = 30.0f;
                fn_801E1594(1);
            } else {
                fn_801E1594(4);
            }
        }
        break;
    case 6:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            field_0x98 = fn_801CD354(field_0x98);
            if (field_0x2d0 >= 0) {
                fn_801E1594(field_0x2d0);
                field_0x2d0 = -1;
            } else {
                fn_801E1594(1);
            }
        }
        break;
    case 7:
        fn_801DF9D8(target);
        break;
    case 8:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_801E1594(9);
        }
        break;
    case 9:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_801E1594(10);
        }
        break;
    case 10:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            if (fn_801D0DD4()) {
                if (fn_801E2F40()) {
                    fn_801E1594(14);
                } else {
                    fn_801E1594(11);
                }
            }
        }
        break;
    case 11:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_801E1594(12);
        }
        break;
    case 12:
        if (!blocked) {
            if (mAnim.fn_801D21F0()) {
                if (mStateTimer >= field_0x250) {
                    fn_801E1594(13);
                } else {
                    s32 player = fn_801D05F8(&mPos, 0.0f);
                    if (player >= 0) {
                        s32 dir = fn_801D0020(&mPos, player, false);
                        if (dir != field_0x98) {
                            field_0x98 = dir;
                            mAnim.startAnim_801D22DC(field_0x98 + 8, 0, false);
                        }
                    }
                }
            }
        }
        break;
    case 13:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            enterFromEdge(this);
        }
        break;
    case 14:
        if (field_0x54.x != 0.0f) {
            fn_801CD950(&field_0x54.x, 0.0f, 0.2f);
            field_0x54.y = field_0x54.x;
            field_0x54.z = field_0x54.x;
        } else {
            fn_801E1594(0);
        }
        break;
    case 15:
        if (fn_803865B8(field_0x2a8)) {
            if (fn_8037EFB4(field_0x2a8) == 0) {
                if (fn_80236228(field_0x2a8) < 0x14) {
                    if (fn_8038470C(field_0x2a8, 0xF1)) {
                        fn_801E1594(0x11);
                    }
                } else if (fn_80384C58(field_0x2a8, mSerial)) {
                    fn_801E1594(0x12);
                }
            } else if (fn_8038470C(field_0x2a8, 0xF2)) {
                fn_801E1594(0x10);
            }
        }
        break;
    case 16:
        if (fn_803865B8(field_0x2a8) && fn_80384C58(field_0x2a8, mSerial)) {
            fn_8023061C(field_0x2a8, false);
            enterFromEdge(this);
        }
        break;
    case 17:
        if (fn_803865B8(field_0x2a8) && fn_80384C58(field_0x2a8, mSerial)) {
            fn_801E1594(0x12);
        }
        break;
    case 18: {
        f32 height = 5.0f;
        if (mFlags & 0x20000) {
            height = 20.0f;
        } else if (field_0x2ac >= 0 && getActorManager_801F856C()->findActorBySerial_801F666C(field_0x2ac) != NULL) {
            height = 20.0f;
        } else if (field_0x2b0 >= 0 && getActorManager_801F856C()->findActorBySerial_801F666C(field_0x2b0) != NULL) {
            height = 20.0f;
        }
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

// FIXME(shotgun): 88.71 %, missing/extra
void cls_804A51E4::vfunc_0x94() {
    if (field_0x2ce != 0) {
        field_0x2ce--;
    }
    if (field_0x2ce == 0) {
        if (mState == 7 && mAnim.mAnim < 0x11 && mAnim.mAnim >= 0xF) {
            mAnim.startAnim_801D22DC(field_0x98, 2, false);
        } else {
            EnemyBase_804A4808::vfunc_0x94();
        }
    }
}


// tu_801DE714 (0x801DE9C0): vtable slot 0x9C. The step is EnemyBase's
// fn_801CE2BC (a plain bl), then the view check.
void cls_804A51E4::vfunc_0x9C(u8 mapHit) {
    EnemyBase_804A4808::fn_801CE2BC(mapHit);
    fn_8021835C();
}

// FIXME(shotgun): 96.02 %, missing/extra + reloc-name
void cls_804A51E4::vfunc_0x88() {
    bool blocked = fn_801CCF7C();
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    if (mState != 0xA) {
        field_0x3c.z = 0.0f;
    }
    if (mState != 0) {
        if (mSpeed != 0.0f && !blocked) {
            JGeometry::TVec2f d(0.0f, 0.0f);
            if (mFlags & 0x4000) {
                fn_801D01A0((const JGeometry::TVec3f*)&field_0x2c4, &d, mSpeed);
            } else if (mFlags & 0x20000) {
                if (field_0x2a8 >= 0) {
                    fn_801D00B4(field_0x2a8, &d, mSpeed);
                }
            } else {
                ActorBase_804A5770* target = NULL;
                s32 serial = field_0x2ac;
                if (serial >= 0) {
                    target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                }
                if (target == NULL) {
                    serial = field_0x2b0;
                    if (serial >= 0) {
                        target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    }
                }
                if (target == NULL) {
                    serial = field_0x2b4;
                    if (serial >= 0) {
                        target = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    }
                }
                if (target != NULL) {
                    fn_801D01A0(&target->mPos, &d, mSpeed);
                }
            }
            field_0x3c.x = d.x;
            field_0x3c.y = d.y;
        }
        switch (mState) {
        case 2:
            if (!blocked) {
                field_0x3c.z = -0.15f;
            }
            break;
        case 4:
            if (!blocked) {
                if (mFlags & 0x80000) {
                    field_0x3c.z = 0.1f;
                } else if (mTargetSerial >= 0) {
                    field_0x3c.z = 0.15f;
                } else {
                    field_0x3c.z = 0.3f;
                }
            }
            break;
        case 10:
            field_0x3c.z += -0.1f;
            if (field_0x3c.z < -lbl_8053EFBC) {
                field_0x3c.z = -lbl_8053EFBC;
            }
            break;
        case 18:
            if (!blocked) {
                field_0x3c.z = -0.1f;
            }
            break;
        }
    }
}

// FIXME(shotgun): 87.41 %, missing/extra
void cls_804A51E4::vfunc_0x8C() {
    if (fn_801E1D7C()) {
        if (!fn_801CD2F8(field_0x4)) {
            fn_80212AC0()->fn_80212AF0(&mPos);
        }
    }
}


// tu_801DE714 (0x801DE714): vtable slot 0x80. In state 0xA the floor check
// is EnemyBase's, called directly (a plain bl, not through the vtable).
void cls_804A51E4::vfunc_0x80() {
    if (mState == 0xA) {
        EnemyBase_804A4808::vfunc_0x80();
    }
}
