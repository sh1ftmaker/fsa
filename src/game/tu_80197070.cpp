#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameState.h"
#include "game/GameGlobals.h"
#include "game/act/act_ganon.h"
#include "game/act/act_life_meter.h"
#include "game/act/tu_801F9484.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/StageFlag.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/tu_80379F84.h"
#include "game/tu_803B8C74.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/act/act_arrow.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"


// .sbss flags of the ZLD2 actor (cls_804A2944).
u8 lbl_8053AB98;
s32 lbl_8053AB9C;
s32 lbl_8053ABA0;
s32 lbl_8053ABA4;
u8 lbl_8053ABA8;

// Tables of the ZLD2 actor, not written as source here (.rodata 0x80468428: two s32[6] sprite
// frame tables, then the sprite bank names; .data 0x804A2880 / 0x804A28C4 / 0x804A28D8 /
// 0x804A28FC: u16 angles, u8 colours and u16 emitter ids indexed by field_0x94).
extern const s32 lbl_80468428[12];
extern const u16 lbl_804A2880[];
extern const u8 lbl_804A28C4[];
extern const u16 lbl_804A28D8[];
extern const u16 lbl_804A28FC[];

void LifeMeter_804B174C::fn_8019CEF8(s32 serial, f32 yOffset, s32 index) {
    field_0x250 = serial;
    field_0x254 = yOffset;
    field_0x24c = index;
}

void fn_8019CEE8(u8 v) {
    lbl_80529DEC.field_0x24d = v;
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8019CECC(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

void Ganon_804A25F0::fn_8019CEC4(s32 value) {
    field_0x2f8 = value;
}

void Ganon_804A25F0::fn_8019CEBC(s32 value) {
    field_0x28c = value;
}

s32 ActorBase_804A5770::fn_8019CEB4() const {
    return field_0xb0;
}

void ActorBase_804A5770::fn_8019CEAC(s32 v) {
    field_0xb0 = v;
}

BOOL fn_8019CE40(s32 stage, s32 sub) {
    return isStageFlagSet(stage, sub);
}

// Same body as setStageFlag (StageFlag.h, inlined by tu_803A8DFC); not a wrapper call
// because that compiles the early returns differently (90.87 %).
void fn_8019CDE4(s32 stage, s32 sub) {
    if (stage < 0 || stage > 7) {
        return;
    }
    if (sub < 0 || sub > 3) {
        return;
    }
    lbl_8050EC80.getCurrentFile().mFlags |= 1 << (stage * 4 + sub);
}

void fn_8019CDC4(s32 v) {
    GameRoot_8049EF20* root = gGameRoot_8053AAF8;
    root->field_0x85 = 1;
    root->field_0x74 = 0;
    root->field_0x78 = 0;
    root->field_0x54 = v;
}

void fn_8019CD9C(s32 v) {
    gGameRoot_8053AAF8->fn_8013670C(v);
}

// FIXME(shotgun): 95.57 %, missing/extra + reloc-name.
void cls_804A2944::init() {
    lbl_8053ABA8 = 0;
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    for (int i = 0; i < 4; i++) {
        field_0x2a4[i].set(0.0f, 0.0f, 0.0f);
        field_0x2d4[i].set(0.0f, 0.0f, 0.0f);
    }
    field_0x25c = -1;
    field_0x264 = -1;
    field_0x27c = 0.0f;
    field_0x278 = 0.0f;
    field_0x274 = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x94 = (field_0x90 >> 4) & 0xF;
    field_0x90 &= 3;
    field_0x26c = 0.0f;
    field_0x270 = 0.0f;
    field_0x268 = 0;
    field_0x280[0].set(mPos);
    field_0x280[1].set(mPos);
    field_0x280[2].set(mPos);
    for (int i = 0; i < 13; i++) {
        mEmitters[i] = NULL;
    }
    lbl_8053AB9C = 0;
    field_0xb0 = 0xA040004;
    field_0x254 = 0;
    field_0x258 = 5;
    switch (field_0x90) {
    case 0: {
        field_0x98 = 2;
        lbl_8053AB98 = 0;
        field_0x254 = 0x24;
        lbl_8053ABA0 = 0;
        lbl_8053ABA4 = 0;
        getGameView_802D6A9C()->fn_802D7440(false);
        gAudioMgr_8053AF80->fn_80458F9C(60);
        field_0x260 = getActorManager_801F856C()->spawnActor_801F9484('LIMT', field_0x4, &mPos, 0, -1, -1);
        s32 serial = field_0x260;
        if (serial >= 0) {
            ActorBase_804A5770* limit = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (limit != NULL) {
                static_cast<LifeMeter_804B174C*>(limit)->fn_8019CEF8(mSerial, -28.0f, field_0x258);
            }
        }
        if (mEmitters[0] == NULL) {
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &field_0x280[0], 0x1CA, 0, 0, NULL, NULL, true, 1.0f);
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &field_0x280[0], 0x1CB, 0, 0, NULL, NULL, true, 1.0f);
        }
        mPos.y += 148.0f;
        mPos.x = field_0x280[0].x;
        mState = 0;
        break;
    }
    case 1:
        field_0xb0 = 0xA000000;
        if (mEmitters[0] == NULL) {
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x533, 1, 5, NULL, NULL, true, 1.0f);
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x534, 1, 5, NULL, NULL, true, 1.0f);
        }
        if (mEmitters[2] == NULL) {
            mEmitters[2] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x535, 1, 5, NULL, NULL, true, 1.0f);
        }
        mState = 0x46;
        break;
    case 2:
        field_0x270 = 255.0f;
        if (mEmitters[0] == NULL) {
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x32C, 0, 4, NULL, NULL, true, 1.0f);
            const u8* color = &lbl_804A28C4[field_0x94 * 3];
            mEmitters[0]->setGlobalEnvColor(color[0], color[1], color[2]);
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x184, 0, 4, NULL, NULL, true, 1.0f);
            const u8* color = &lbl_804A28C4[field_0x94 * 3];
            mEmitters[1]->setGlobalEnvColor(color[0], color[1], color[2]);
        }
        mState = 0x4A;
        break;
    case 3:
        break;
    default:
        field_0x26c = mPos.z;
        field_0x234 = 9;
        mState = 0x4F;
        break;
    }
}

cls_804A2944::~cls_804A2944() {
    for (s32 i = 0; i < 13; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
    // field_0x90 is u32, but the original tests it with a signed cmpwi.
    s32 param = field_0x90;
    if (param == 0) {
        getGameView_802D6A9C()->fn_802D7440(true);
        lbl_8053AB98 = 0;
        lbl_8053ABA8 = 0;
    }
}

// FIXME(shotgun): 99.55 %, different.
void cls_804A2944::vfunc_0x44() {
    if (field_0x90 == 1 && mState != 0x49) {
        if (getActorManager_801F856C()->findActorBySerial_801F666C(field_0x25c) != NULL) {
            s32 serial = field_0xa0;
            if (serial >= 0) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL && actor->mActorId == 'ARRW') {
                    mPos.set(actor->mPos);
                    if (static_cast<ArrowPlayer_804A05E0*>(actor)->field_0x26c >= 1) {
                        field_0x250 = -1;
                        return;
                    }
                    if (field_0x250 == 0) {
                        field_0x250 = lbl_8053A100;
                        field_0x250++;
                        for (int i = 0; i < 3; i++) {
                            deleteEmitter(&mEmitters[i]);
                        }
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0xB5, 1, 5, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0xB6, 1, 5, NULL, NULL, true, 1.0f);
                        if (mEmitters[0] == NULL) {
                            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x199, 1, 5, NULL, NULL, true, 1.0f);
                        }
                        if (mEmitters[1] == NULL) {
                            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x19A, 1, 5, NULL, NULL, true, 1.0f);
                        }
                    }
                    field_0x3c.set(0.0f, 0.0f, 0.0f);
                    field_0x274 = 0.0f;
                    field_0x278 = 0.0f;
                    field_0x27c = 0.0f;
                    for (int i = 0; i < 13; i++) {
                        setEmitterPos_8013CB44(mEmitters[i], &mPos, 1, 5, true);
                    }
                }
            }
        }
    }
}

// FIXME: 89.43. Scheduling and registers: the original keeps the layer in r31,
// the player index in r29 and the position pointer in r30; ours has the layer
// in r29, the index in r30 and the pointer in r31. The .sdata2 literal pairs
// once the unit's literals are laid out.
void cls_804A2944::fn_8019C500(s32 keepY) {
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i)) {
            s32 layer = field_0x4;
            if (layer == getPlayerLayer_8023DE58(i)) {
                JGeometry::TVec3f pos(field_0x2a4[i].x, field_0x2a4[i].y, field_0x2a4[i].z);
                if (!keepY) {
                    pos.y = 32.0f + mPos.y;
                }
                fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            }
        }
    }
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8019C4F4(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// The actor `actor` has been reached: the ZLD2 actor gives up one of its field_0x250 tokens
// (inlined twice in fn_80197EB0).
static inline void takeToken(cls_804A2944* self, ActorBase_804A5770* actor) {
    actor->field_0x94 = 5;
    self->field_0x250 -= 1;
    if (self->field_0x250 < 0) {
        self->field_0x250 = 0;
    }
    if (fn_8022F514(self->field_0x250)) {
        lbl_80529DEC.field_0x1a4 = fn_8022F3DC(self->field_0x250);
    } else {
        lbl_80529DEC.field_0x1a4 = self->field_0x250;
    }
    self->field_0x238 = 50;
    self->mState = 0x49;
    deleteEmitter(&self->mEmitters[0]);
    deleteEmitter(&self->mEmitters[1]);
}

// FIXME(shotgun): 92.19 %, missing/extra + reloc-name.
void cls_804A2944::fn_80197EB0() {
    switch (mState) {
    case 0x46:
        mState = 0x47;
        // fall through
    case 0x47: {
        startSound_801F0E34(0x30056, 0);
        if (field_0x250 == 0 && field_0x3c.x == 0.0f && field_0x3c.y == 0.0f) {
            break;
        }
        if (mEmitters[2] != NULL) {
            mEmitters[2]->becomeInvalidEmitter();
            mEmitters[2] = NULL;
        }
        if (fn_801F6758(&mPos, field_0x4, 16.0f)) {
            field_0x258 = 0;
            break;
        }
        s32 serial = field_0x25c;
        if (serial < 0) {
            field_0x258 = 0;
            break;
        }
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            field_0x258 = 0;
            break;
        }
        f32 dx = actor->mPos.x - mPos.x;
        f32 dy = actor->mPos.y - mPos.y;
        f32 distSq = dx * dx + dy * dy;
        if (std::sqrtf(distSq) <= 90.0f) {
            field_0x27c = 0.0f;
            field_0x278 = 0.0f;
            field_0x274 = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x248 = (s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f) + 0x8000;
            field_0x24c = (s32)std::sqrtf(distSq);
            for (int i = 0; i < 13; i++) {
                if (mEmitters[i] != NULL) {
                    mEmitters[i]->mGlobalPScl.x = 1.5f;
                    mEmitters[i]->mGlobalPScl.y = 1.5f;
                }
            }
            mBox.i.x = -10.0f;
            mBox.i.y = -10.0f;
            mBox.f.x = 10.0f;
            mBox.f.y = 10.0f;
            mState = 0x48;
        }
        serial = field_0x25c;
        if (serial < 0) {
            break;
        }
        actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL || field_0x250 == 0) {
            break;
        }
        dx = actor->mPos.x - mPos.x;
        dy = actor->mPos.y - mPos.y;
        if (field_0x250 >= 1 && std::sqrtf(dx * dx + dy * dy) <= 80.0f) {
            takeToken(this, actor);
        } else {
            field_0x258 = 0;
        }
        break;
    }
    case 0x48: {
        startSound_801F0E34(0x30056, 0);
        s32 serial = field_0x25c;
        if (serial < 0) {
            field_0x258 = 0;
            break;
        }
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            field_0x258 = 0;
            break;
        }
        if ((u8)actor->field_0xb0 != 0xE) {
            field_0x258 = 0;
            break;
        }
        if (field_0x250 == 0) {
            addCalc2_801F71A4(&mPos.x,
                              actor->mPos.x + field_0x24c * JMath::sincosTable_.sinShort(field_0x248),
                              0.8f, 10.0f);
            addCalc2_801F71A4(&mPos.y,
                              actor->mPos.y + field_0x24c * JMath::sincosTable_.cosShort(field_0x248),
                              0.8f, 10.0f);
            field_0x248 += 0x150;
        } else {
            serial = field_0x25c;
            if (serial < 0) {
                break;
            }
            actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor == NULL || field_0x250 == 0) {
                break;
            }
            f32 dx = actor->mPos.x - mPos.x;
            f32 dy = actor->mPos.y - mPos.y;
            if (field_0x250 >= 1 && std::sqrtf(dx * dx + dy * dy) <= 80.0f) {
                takeToken(this, actor);
            } else {
                field_0x258 = 0;
            }
        }
        break;
    }
    case 0x49:
        if (field_0x238 == 0) {
            field_0x258 = 0;
        }
        break;
    }
    if (field_0x258 <= 0) {
        for (int i = 0; i < 13; i++) {
            if (mEmitters[i] != NULL) {
                mEmitters[i]->becomeInvalidEmitter();
                mEmitters[i] = NULL;
            }
        }
        mAlive = false;
    } else {
        for (int i = 0; i < 13; i++) {
            setEmitterPos_8013CB44(mEmitters[i], &mPos, 1, 5, true);
        }
    }
}

// FIXME(shotgun): 94.26 %, missing/extra.
void cls_804A2944::fn_801979C4() {
    for (int i = 0; i < 13; i++) {
        setEmitterPos_8013CB44(mEmitters[i], &mPos, 0, 4, true);
    }
    field_0x254 += 1;
    if (field_0x254 > 5) {
        field_0x254 = 0;
    }
    switch (mState) {
    case 0x4A:
        field_0x26c = 16.0f;
        mState = 0x4A;
        // fall through
    case 0x4B:
        if (std::fabsf(field_0x26c - mPos.z) < 1.0f) {
            startSound_801F0E34(0x50009, 0);
            mState = 0x4C;
        }
        break;
    case 0x4C: {
        s16 angle = lbl_804A2880[field_0x94];
        f32 tx = field_0x280[0].x + 112.0f * JMath::sincosTable_.sinShort(angle);
        f32 ty = field_0x280[0].y + 112.0f * JMath::sincosTable_.cosShort(angle);
        f32 dx = tx - mPos.x;
        f32 dy = ty - (mPos.y - mPos.z);
        field_0x274 = 2.0f * JMath::sincosTable_.sinShort((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
        field_0x278 = 2.0f * JMath::sincosTable_.cosShort((s16)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
        if (std::sqrtf(dx * dx + dy * dy) < 2.0f) {
            lbl_8053ABA0 += 1;
            mState = 0x4D;
            field_0x27c = 0.0f;
            field_0x278 = 0.0f;
            field_0x274 = 0.0f;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
        }
        break;
    }
    case 0x4D:
        if (lbl_8053ABA4 == 1) {
            const u16* ids = &lbl_804A28D8[field_0x94 * 3];
            spawnEmitter_8013CC50(field_0x4, &mPos, ids[0], 0, 5, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, ids[1], 0, 5, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, ids[2], 0, 5, NULL, NULL, true, 1.0f);
            field_0x248 = 0;
            mState = 0x4E;
        }
        break;
    case 0x4E:
        field_0x248 += 1;
        startSound_801F0E34(0x5000B, 0);
        addCalc2_801F71A4(&field_0x270, 0.0f, 0.3f, 10.0f);
        if (field_0x248 == 60) {
            field_0x24c = 1;
            if (mEmitters[0] != NULL) {
                mEmitters[0]->becomeInvalidEmitter();
                mEmitters[0] = NULL;
            }
            if (mEmitters[1] != NULL) {
                mEmitters[1]->becomeInvalidEmitter();
                mEmitters[1] = NULL;
            }
        }
        if (field_0x248 == 0x14A) {
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('ZLD2', field_0x4, &mPos,
                                                                         (field_0x94 << 4) | 3, -1, -1);
            if (serial >= 0) {
                cls_804A2944* actor = static_cast<cls_804A2944*>(
                    getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (actor != NULL) {
                    actor->field_0x280[0].x = field_0x280[0].x;
                    actor->field_0x280[0].y = field_0x280[0].y;
                    actor->field_0x280[0].z = field_0x280[0].z;
                }
            }
            mAlive = false;
        }
        break;
    }
}

// FIXME(shotgun): 91.86 %, missing/extra.
void cls_804A2944::fn_801976E8() {
    s32 base = field_0x94 * 6;
    addCalc2_801F71A4(&field_0x270, 255.0f, 0.3f, 10.0f);
    switch (mState) {
    case 0x4F:
        lbl_8053ABA0 += 1;
        mState = 0x4F;
        // fall through
    case 0x50:
        if (field_0x234 == 0) {
            field_0x234 = 9;
            field_0x254 += 1;
            if (field_0x254 > 5) {
                field_0x254 = 0;
            }
        }
        if (lbl_8053ABA4 == 2) {
            for (s32 i = 0; i < 6; i++) {
                if (i <= 1 || i == 5) {
                    if (mEmitters[i] == NULL) {
                        mEmitters[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A28FC[base + i], 0, 5,
                                                             NULL, NULL, true, 1.0f);
                    }
                } else {
                    spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A28FC[base + i], 0, 5, NULL, NULL, true,
                                          1.0f);
                }
            }
            field_0x238 = 0x1E0;
            mState = 0x51;
        }
        break;
    case 0x51:
        if (field_0x234 == 0) {
            field_0x234 = 9;
            field_0x254 += 1;
            if (field_0x254 > 5) {
                field_0x254 = 0;
            }
        }
        if (field_0x238 == 0) {
            lbl_8053ABA4 = 3;
            mState = 0x52;
        }
        break;
    case 0x52:
        if (lbl_8053ABA4 == 4) {
            field_0x26c = 0.0f;
            mPos.z = 0.0f;
            for (int i = 0; i < 6; i++) {
                if (mEmitters[i] != NULL) {
                    getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(mEmitters[i]);
                    mEmitters[i] = NULL;
                }
            }
        }
        if (lbl_8053ABA4 == 5) {
            mPos.y = field_0x280[0].y - 16.0f;
            mPos.x = field_0x280[0].x;
            mPos.x += 28.0f * (2.5f - (f32)field_0x94);
            field_0x254 = 8;
        } else if (field_0x234 == 0) {
            field_0x234 = 9;
            field_0x254 += 1;
            if (field_0x254 > 5) {
                field_0x254 = 0;
            }
        }
        break;
    }
}

// FIXME(shotgun): 98.84 %, missing/extra. The last two timer decrements go through a shared base register (this+8) in the original.
void cls_804A2944::update() {
    JGeometry::TVec3f pos = mPos;
    mLife = 10000;
    addCalc2_801F71A4(&field_0x3c.x, field_0x274, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x278, 0.3f, 1.0f);
    addCalc2_801F71A4(&mPos.z, field_0x26c, 0.3f, 1.0f);
    mPos.add(field_0x3c);
    switch (field_0x90) {
    case 0:
        fn_80198828();
        break;
    case 1:
        fn_80197EB0();
        break;
    case 2:
        fn_801979C4();
        pos.x += 14.0f;
        pos.y -= 64.0f;
        break;
    case 3:
        fn_801976E8();
        pos.x += 14.0f;
        pos.y -= 64.0f;
        break;
    }
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
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
}

// FIXME(shotgun): 82.45 %, missing/extra. The original addresses the frame tables and sprite bank names off one .rodata base (r31); here the names are plain literals and the tables extern.
void cls_804A2944::draw() {
    switch (field_0x90) {
    case 0: {
        SpriteParams_801F5AE0 params;
        if (field_0x23c == 0 || (field_0x23c & 2)) {
            params.field_0x1f = 0;
            s32 frame = 0x9B;
            if (field_0x268 == 1) {
                frame = 0x274;
            }
            s32 flags = 7;
            if (mState == 0x15) {
                flags = -1;
            }
            drawSprite_801F06F0("enemy_both2_sob.bin", frame + field_0x254, &params, flags, 1.0f, 1.0f);
        }
        break;
    }
    case 2:
        if (field_0x24c == 0) {
            SpriteParams_801F5AE0 params;
            params.field_0x1d = 5;
            params.field_0x30 = 0x11;
            fn_801F02BC("enemy_both2_sob.bin", field_0x254 + lbl_80468428[field_0x94], &params, -1, 0, false,
                        1.0f, 1.0f);
            params.field_0x10.a = (u8)field_0x270;
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_both2_sob.bin",
                            field_0x254 + lbl_80468428[field_0x94], NULL);
            }
        }
        break;
    case 3: {
        SpriteParams_801F5AE0 params;
        params.field_0x1d = 5;
        params.field_0x1f = 0;
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x254 + lbl_80468428[6 + field_0x94], &params, -1,
                            1.0f, 1.0f);
        params.field_0x10.a = (u8)field_0x270;
        if (mSpriteParams.mMtx == NULL) {
            fn_801EF830(field_0x4, &mPos, &params, 0x407, "enemy_gc_sob.bin",
                        field_0x254 + lbl_80468428[6 + field_0x94], NULL);
        }
        break;
    }
    }
}

// FIXME(shotgun): fn_80198828 (0x3CCC bytes, the state machine of field_0x90 == 0) is not written.
