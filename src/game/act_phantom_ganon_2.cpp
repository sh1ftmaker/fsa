#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/PlayerBit.h"
#include "game/act/act_phantom_ganon_2.h"
#include "game/SpriteParams.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Tables of the FGN2 actor (.data 0x804A1960, not written as source here): the start
// position and "FGAN" sprite names per player, emitter ids and sprite offsets.
extern const f32 lbl_804A1960[][3];
extern const s32 lbl_804A1990[];
extern const s32 lbl_804A19A8[];
extern const s32 lbl_804A19BC[];
extern const s32 lbl_804A19DC[];
extern const char* const lbl_804A1A10[];
extern const f32 lbl_804A1A28[][2];
extern const char* const lbl_804A1A50[];
extern const char* const lbl_804A1A64[];
extern const char* const lbl_804A1A84[];
extern const u16 lbl_804A19F0[];
extern const u16 lbl_804A1A00[];
extern const u16 lbl_80539FC0[];
extern u32 lbl_804C17A8[4];
extern s32 lbl_80539FB8;
extern s32 lbl_8053AB7C;
extern s32 lbl_8053AB80;
extern u8 lbl_80539FBC;
extern u8 lbl_8053AB84;

void fn_801546F4(JGeometry::TBox2f* dst, const JGeometry::TBox2f* src);
void fn_80154718(const JGeometry::TVec3f* pos);
void fn_801546BC(JGeometry::TVec3f* v);
void fn_801545F0(JGeometry::TVec3f* a, const JGeometry::TVec3f* b);
void fn_80154558(JGeometry::TVec2f* a, const JGeometry::TVec2f* b);
void fn_80154590(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1);
void fn_801546D0(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src);
s32 fn_8015465C(f32 y, f32 x);
f32 fn_801544FC(s16 angle);
f32 fn_801544E4(s16 angle);
f32 fn_80154378(f32 x);
f32 fn_8015436C(f32 x);
ActorBase_804A5770* fn_8015445C(s32 serial);
s32 fn_80154310(u32 id, s32 layer, const JGeometry::TVec3f* pos, u32 arg, s32 a, s32 b);
u8 fn_8017ECC0();
u8 fn_8017EC70(const JGeometry::TVec3f* pos, f32 value);
void fn_8017ED08();
void fn_8017EC5C(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1);
void fn_8017EC38(JGeometry::TBox2f* dst, const JGeometry::TBox2f* src);
bool fn_8017EB94(const JGeometry::TBox2f* box);
u8 fn_8017EB44(const JGeometry::TVec3f* pos);
bool fn_8017EB1C(f32 value);
void fn_8017EACC(JPABaseEmitter* emitter, u8 r, u8 g, u8 b);
void fn_8017EAB4(JPABaseEmitter* emitter);

// .sbss 0x8053AB78 (a mode word; update and init also use it).
s32 lbl_8053AB78;

// FIXME: __sinit_act_phantom_ganon_2_cpp (compiler-generated, no source line of its own)
// is 99.61 %: the JAudio pool records are 0x10 higher in the original's .bss
// (lbl_804C17A8 + 0x10).

AudioMgr_8045A080* fn_8018A6AC() {
    return gAudioMgr_8053AF80;
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8018A6A0(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8018A684(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

s32 fn_8018A660() {
    return getActorManager_801F856C()->fn_801F68E0();
}

void fn_8018A63C() {
    getActorManager_801F856C()->fn_801F6884();
}

// Whether bit `player` of the game root's mPlayerMask is set (game/PlayerBit.h,
// on gGameRoot_8053AAF8 itself).
bool fn_8018A614(s32 player) {
    return isPlayerBitSet(gGameRoot_8053AAF8, player);
}

// The effect system's particle emitter manager, fetched through a getter.
JPAEmitterManager* fn_8018A5F0() {
    return getEffectSystem_8013C824()->mEmitterManager;
}

// Out-of-line copy of JPABaseEmitter::setStatus(4).
void fn_8018A5E0(JPABaseEmitter* emitter) {
    emitter->setStatus(4);
}

// Out-of-line copy of JPABaseEmitter::clearStatus(4).
void fn_8018A5D0(JPABaseEmitter* emitter) {
    emitter->clearStatus(4);
}

// Out-of-line copy of JPABaseEmitter::setGlobalAlpha(alpha).
void fn_8018A5C8(JPABaseEmitter* emitter, u8 alpha) {
    emitter->setGlobalAlpha(alpha);
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_8018A5B8(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

bool ActorBase_804A5770::fn_8018A5B0() const {
    return mAlive;
}

void ActorBase_804A5770::fn_8018A5A8(u16 value) {
    field_0x94 = value;
}

void ActorBase_804A5770::fn_8018A5A0(s32 value) {
    field_0x98 = value;
}

void ActorBase_804A5770::fn_8018A598(bool alive) {
    mAlive = alive;
}

// fn_8018A588 (setLife) is ActorBase's inline setter.

u16 ActorBase_804A5770::fn_8018A590() const {
    return field_0x94;
}

// FIXME(shotgun): 56.79 %, missing/extra + reloc-name. Constants and the stmw prologue are scheduled differently; the tables are extern.
void PhantomGanon2_804A1A98::init() {
    field_0x94 = (field_0x90 >> 4) & 0xF;
    field_0x90 &= 0xF;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x196 = 1;
    field_0x26c = 0;
    field_0x2ac = 0.0f;
    field_0x2b0.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 4; i++) {
        mEmitters2f4[i] = NULL;
        mEmitters304[i] = NULL;
    }
    mEmitter314 = NULL;
    mPos.z = 1.0f;
    field_0x280 = 2.0f;
    field_0x28c = 0.0f;
    field_0x2ef = 0;
    field_0x274 = -1;
    field_0x278 = 2;
    field_0x2ec = 0;
    field_0x294 = 255.0f;
    field_0x2a0.x = lbl_804A1960[field_0x94][0];
    field_0x298 = 255.0f;
    field_0x2a0.y = lbl_804A1960[field_0x94][1];
    field_0x29c = 255.0f;
    field_0x2a0.z = lbl_804A1960[field_0x94][2];
    mLife = 10000;
    field_0x2f0 = 0;
    field_0x290 = 255.0f;
    field_0x284 = 0.0f;
    field_0x288 = 0.0f;
    field_0x2c8.set(1.0f, 1.0f, 1.0f);
    field_0x2bc.set(mPos);
    field_0x2bc.y += 8.0f;
    field_0x2d4.set(mPos);
    switch (field_0x90) {
    case 1:
        mBox.i.x = -16.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 40.0f;
        field_0x80.i.x = -8.0f;
        field_0x80.i.y = -8.0f;
        field_0x80.f.x = 8.0f;
        field_0x80.f.y = 8.0f;
        field_0x290 = 0.0f;
        field_0x2d4.x += 48.0f * (f32)field_0x94;
        field_0x114 = 2;
        field_0x288 = 40.0f;
        field_0x2ee = 1;
        field_0x230 = 5;
        break;
    case 10:
        for (int i = 0; i < 4; i++) {
            mEmitters2f4[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A19F0[i * 2], 1, 2, NULL, NULL, true, 1.0f);
            mEmitters304[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A19F0[i * 2 + 1], 1, 2, NULL, NULL, true, 1.0f);
            if (i != field_0x94) {
                if (mEmitters2f4[i] != NULL) {
                    mEmitters2f4[i]->setStatus(4);
                }
                if (mEmitters304[i] != NULL) {
                    mEmitters304[i]->setStatus(4);
                }
            }
        }
        field_0x2f0 = 11;
        if (lbl_8053AB78 == 1) {
            field_0x2f0 = 6;
        }
        // fall through
    case 11:
        field_0x2c8.set(0.0f, 0.0f, 0.0f);
        field_0x2ee = 2;
        mBox.i.x = -7.0f;
        mBox.i.y = -7.0f;
        mBox.f.x = 7.0f;
        mBox.f.y = 7.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        if (field_0x90 == 11) {
            mEmitters2f4[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x37A, 1, 2, NULL, NULL, true, 1.0f);
            mEmitters304[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x37B, 1, 2, NULL, NULL, true, 1.0f);
        }
        field_0x118 = 4;
        field_0x230 = 29;
        break;
    case 12:
        field_0x234 = 30;
        field_0x2ee = 2;
        mBox.i.x = -2.0f;
        mBox.i.y = -2.0f;
        mBox.f.x = 2.0f;
        mBox.f.y = 2.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        field_0x118 = 0;
        field_0x230 = 30;
        break;
    default:
        mBox.i.x = -16.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 40.0f;
        field_0x80.i.x = -8.0f;
        field_0x80.i.y = -8.0f;
        field_0x80.f.x = 8.0f;
        field_0x80.f.y = 8.0f;
        field_0x290 = 0.0f;
        mPos.x -= 80.0f;
        field_0x2e0.set(mPos);
        field_0x2d4.set(mPos);
        getGameView_802D6A9C()->fn_802D7440(false);
        lbl_80539FB8 = 100;
        lbl_8053AB78 = 0;
        lbl_8053AB7C = 0;
        lbl_8053AB80 = 0;
        lbl_80539FBC = 0;
        lbl_8053AB84 = 0;
        for (int i = 0; i < 4; i++) {
            lbl_804C17A8[i] = 0;
        }
        field_0x114 = 2;
        field_0x288 = 40.0f;
        field_0x2ee = 1;
        field_0x230 = 0;
        break;
    }
    field_0x27c = field_0x94;
    field_0x270 = lbl_80539FB8 << 2;
}

// FIXME: 99.7. Regalloc only: the target keeps the serial in r31 (the register of the
// emitter loop's pointer), we keep it in r30 (the loop counter's).
PhantomGanon2_804A1A98::~PhantomGanon2_804A1A98() {
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters2f4[i]);
        releaseEmitter_801EE434(mEmitters304[i]);
    }
    releaseEmitter_801EE434(mEmitter314);
    if (field_0x90 == 0) {
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
    if (field_0x90 == 10) {
        s32 serial = field_0x274;
        if (serial >= 0) {
            PhantomGanon2_804A1A98* other = (PhantomGanon2_804A1A98*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (other != NULL) {
                other->field_0x28c = 3.0f;
                field_0x274 = -1;
            }
        }
    }
}

// FIXME: 98.9. Regalloc only: the target keeps this in r30 and the two layer reads in r29,
// we use r29 for this and r30/r28 for the counter and the layer reads.
u16 PhantomGanon2_804A1A98::fn_80189DEC() {
    s32 player = fn_8022EA0C(field_0x94);
    if (player < 0) {
        player = 0;
    }
    if (!(fn_8023F264(player) && getField_0x4() == getPlayerLayer_8023DE58(player) && !fn_80230188(player) &&
          !fn_80236B50(player) && lbl_8053AB78 != 1))
    {
        s32 pick = (s32)(3.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        for (s32 i = 0; i < 4; i++) {
            if (pick != player && fn_8023F264(pick) && getField_0x4() == getPlayerLayer_8023DE58(pick) &&
                pick == fn_8022EA0C(pick) && !fn_80230188(pick) && !fn_80236B50(pick) &&
                !fn_8022DCD8(pick))
            {
                field_0x94 = pick;
                if (lbl_8053AB78 == 1) {
                    field_0x27c = field_0x94;
                }
                return field_0x94;
            }
            pick = (pick + 1) & 3;
        }
    }
    field_0x94 = player;
    return field_0x94;
}

// FIXME(shotgun): 94.09 %, missing/extra + reloc-name.
void PhantomGanon2_804A1A98::update() {
    JGeometry::TVec3f pos60;
    JGeometry::TVec3f vec54;
    JGeometry::TBox2f box44;
    fn_801546F4(&box44, &mBox);
    JGeometry::TVec2f vec14;
    fn_8018A6A0(&vec14, field_0x2bc.x, field_0x2bc.y);
    s32 outDir = -1;
    f32 outDist;
    s32 hitPlayer = -1;
    u16 outAngle = 0;
    fn_8018A684(&pos60, &mPos);
    pos60.y -= 40.0f;
    if (field_0x90 == 0) {
        mLife = 10000;
        fn_8017ECC0();
    }
    if (vfunc_0x2C(8.0f, -16.0f, 50.0f)) {
        fn_8017EC70(&mPos, 1.0f);
        fn_8017ED08();
        return;
    }
    fn_80154718(&pos60);
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
    fn_801F3D94(0);
    if (field_0x230 == 22 && (field_0x90 == 0 || field_0x90 == 1)) {
        field_0x250++;
        if (field_0x250 > 250) {
            field_0x26c = 5;
            field_0x280 = 0.0f;
            field_0x234 = lbl_804A1990[field_0x26c];
            field_0x230 = 23;
            return;
        }
        if (fn_801F2FAC()) {
            if (mPos.z == 0.0f && field_0x254 != 0) {
                field_0x26c = 0;
                field_0x254 = 0;
                field_0x270 -= (10000 - mLife);
                lbl_80539FB8 = field_0x270;
                startSound_801F0E34(0x40046, 0);
                if (field_0x270 <= 0 && field_0x90 == 0) {
                    field_0x230 = 25;
                }
            }
            fn_801546BC(&field_0x3c);
        }
    }
    if (field_0x90 == 0 || field_0x90 == 1) {
        addCalc2_801F71A4(&field_0x3c.x, field_0x2b0.x, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x2b0.y, 0.3f, 1.0f);
        addCalc2_801F71A4(&mPos.z, field_0x288, 0.3f, field_0x280);
    }
    fn_801545F0(&mPos, &field_0x3c);
    if (mEmitter314 != NULL) {
        setEmitterPos_8013CB44(mEmitter314, &mPos, 0, 1, true);
    }
    switch (field_0x230) {
    case 0:
        if (lbl_80539FBC == 0) {
            JGeometry::TBox2f box28;
            fn_8017EC5C(&box28, -80.0f, -80.0f, 80.0f, 80.0f);
            fn_8017EC38(&box44, &box28);
            fn_80154558(&box44.i, &vec14);
            fn_80154558(&box44.f, &vec14);
            if (!fn_8017EB94(&box44)) {
                break;
            }
        }
        if (fn_8018A660() == 0) {
            break;
        }
        if (lbl_80539FBC == 0) {
            fn_8018A6AC()->fn_80458F9C(60);
        }
        fn_8022ADE4();
        field_0x280 = 2.0f;
        field_0x26c = 0;
        field_0x234 = 0;
        field_0x250 = 0;
        lbl_8053AB80 = 0;
        field_0x28c = 0.0f;
        field_0x278 = 2;
        field_0x230 = 1;
        // fall through
    case 1:
        fn_8018A684(&pos60, &mPos);
        pos60.y -= 40.0f;
        if (lbl_80539FBC == 0) {
            fn_8017EB44(&pos60);
            fn_8017EB1C(0.05f);
            field_0x250++;
            if (field_0x250 < 100) {
                break;
            }
            if (field_0x250 == 100) {
                startSound_801F0E34(0x40074, 0);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x362, 0, 4, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x363, 0, 4, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x364, 0, 4, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x365, 0, 4, NULL, NULL, true, 1.0f);
            }
        } else {
            fn_8017EC70(&pos60, 1.0f);
        }
        field_0x250 = 0;
        field_0x230 = 2;
        // fall through
    case 2:
        if (lbl_80539FBC == 0) {
            fn_8018A684(&pos60, &mPos);
            pos60.y -= 40.0f;
            fn_8017EB44(&pos60);
            fn_8017EB1C(0.05f);
            field_0x250++;
            if (field_0x250 < 300) {
                break;
            }
        } else {
            fn_8018A684(&pos60, &mPos);
            pos60.y -= 40.0f;
            fn_8017EC70(&pos60, 1.0f);
        }
        field_0x2c8.y = 5.0f;
        field_0x2c8.x = 0.0f;
        field_0x250 = 0;
        field_0x230 = 3;
        // fall through
    case 3:
        fn_8018A684(&pos60, &mPos);
        pos60.y -= 40.0f;
        fn_8017EB44(&pos60);
        fn_8017EB1C(0.05f);
        addCalc2_801F71A4(&field_0x290, 255.0f, 0.5f, 5.0f);
        addCalc2_801F71A4(&field_0x2c8.y, 1.0f, 0.3f, 3.0f);
        addCalc2_801F71A4(&field_0x2c8.x, 1.0f, 0.3f, 0.1f);
        if (field_0x2c8.x >= 0.99f && field_0x290 >= 254.0f) {
            JGeometry::TVec3f vec1C;
            fn_8018A5B8(&vec1C, 1.0f, 1.0f, 1.0f);
            fn_8018A684(&field_0x2c8, &vec1C);
            field_0x290 = 255.0f;
            field_0x250 = 0;
            field_0x238 = 1;
            if (lbl_80539FBC == 0) {
                field_0x238 = 50;
            }
            field_0x264 = 0;
            lbl_8053AB84 = 0;
            lbl_8053AB7C = 0;
            lbl_8053AB78 = 1;
            field_0x230 = 4;
        }
        break;
    case 4:
        fn_8018A684(&pos60, &mPos);
        pos60.y -= 40.0f;
        fn_8017EB44(&pos60);
        fn_8017EB1C(0.05f);
        if (field_0x238 == 0) {
            for (s32 i = 1; i < 4; i++) {
                s32 ser = fn_80154310('FGN2', fn_8015461C(), &mPos, (i << 4) | 1, -1, -1);
                if (ser >= 0) {
                    PhantomGanon2_804A1A98* child = (PhantomGanon2_804A1A98*)fn_8015445C(ser);
                    if (child != NULL && child->fn_8018A5B0() == true) {
                        lbl_8053AB7C++;
                        fn_8018A684(&child->field_0x2e0, &field_0x2e0);
                        fn_8018A684(&child->field_0x2bc, &field_0x2bc);
                        child->field_0x260 = field_0x260;
                        if (fn_8018A614(i)) {
                            lbl_8053AB78++;
                        } else {
                            child->field_0x264 = 1;
                        }
                    }
                }
            }
            field_0x23c = 0;
            if (lbl_80539FBC == 0) {
                startSound_801F0E34(0x40042, 0);
                field_0x23c = 100;
            }
            field_0x230 = 5;
        }
        break;
    case 5:
        field_0x280 = 2.0f;
        field_0x26c = 0;
        field_0x234 = 0;
        field_0x238 = 0;
        field_0x28c = 0.0f;
        field_0x2ac = 0.0f;
        field_0x278 = 2;
        field_0x230 = 6;
        // fall through
    case 6:
        if (field_0x90 == 0) {
            fn_8018A684(&pos60, &mPos);
            pos60.x = field_0x2bc.x - 8.0f;
            pos60.y -= 40.0f;
            fn_8017EB44(&pos60);
            fn_8017EB1C(0.05f);
            if (field_0x23c == 0 && lbl_8053AB80 == 0) {
                lbl_8053AB80 = 1;
                break;
            }
            if (field_0x2ac > 0.9f && lbl_8053AB80 == 1) {
                if (lbl_80539FBC == 0) {
                    startSound_801F0E34(0x40043, 0);
                }
                lbl_8053AB80 = 2;
                break;
            }
        }
        addCalc2_801F71A4(&field_0x290, 255.0f, 0.5f, 5.0f);
        addCalc2_801F71A4(&mPos.x, field_0x2d4.x, 0.5f, 5.0f);
        addCalc2_801F71A4(&field_0x2ac, 1.0f, 0.1f, 0.1f);
        if (lbl_8053AB80 == 2) {
            if (field_0x238 == 0) {
                if (lbl_80539FBC == 0) {
                    if (field_0x234 == 0) {
                        field_0x234 = lbl_804A19A8[field_0x26c];
                        field_0x278 = 1;
                        field_0x26c++;
                        if (field_0x26c == 4) {
                            field_0x238 = 40;
                        }
                    }
                } else {
                    field_0x238 = 2;
                }
            } else if (field_0x238 == 1) {
                if (lbl_80539FBC == 0) {
                    fn_8018A6AC()->fn_80458FF0(0x1000004, false, 0);
                }
                lbl_80539FBC = 1;
                field_0x230 = 7;
            }
        }
        break;
    case 7:
        if (field_0x234 == 0) {
            field_0x26c--;
            field_0x234 = lbl_804A19A8[field_0x26c];
            if (field_0x26c < 0) {
                if (field_0x90 == 0) {
                    fn_8018A63C();
                }
                field_0x26c = 0;
                field_0x278 = 2;
                field_0x230 = 8;
                // fall through
            } else {
                break;
            }
        } else {
            break;
        }
    case 8:
        field_0x248 = 120;
        if (mEmitter314 == NULL) {
            mEmitter314 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x380, 0, 0, NULL, NULL, true, 1.0f);
            if (mEmitter314 != NULL) {
                fn_8017EACC(mEmitter314, (u8)field_0x294, (u8)field_0x298, (u8)field_0x29c);
            }
        }
        field_0x238 = (s32)(200.0f + 100.0f * fn_80154744());
        if (field_0x268 >= 0) {
            lbl_804C17A8[field_0x268] = 0;
            field_0x23c = 0;
        }
        field_0x268 = -1;
        field_0x280 = 2.0f;
        field_0x26c = 0;
        field_0x234 = lbl_804A19BC[field_0x26c];
        if (field_0x28c == 0.0f) {
            field_0x28c = 3.0f;
        }
        field_0x278 = 2;
        field_0x230 = 9;
        // fall through
    case 9:
        addCalc2_801F71A4(&field_0x290, 255.0f, 1.0f, 10.0f);
        if (field_0x234 == 0) {
            field_0x26c++;
            if (field_0x26c > 7) {
                field_0x26c = 0;
            }
            field_0x234 = lbl_804A19BC[field_0x26c];
        }
        {
            f32 dx = field_0x2bc.x - mPos.x;
            f32 dy = field_0x2bc.y - mPos.y;
            if (mPos.z >= 40.0f) {
                s16 rndAngle = (s32)(8192.0f - 16384.0f * fn_80154744());
                s16 angle = fn_8015465C(dx, dy) + rndAngle;
                if (field_0x250 != 0) {
                    angle = fn_8015465C(dx, dy);
                }
                if (field_0x23c == 0) {
                    field_0x2b0.x = 2.0f * fn_801544FC(angle);
                    field_0x2b0.y = 2.0f * fn_801544E4(angle);
                    field_0x23c = (s32)(50.0f + 50.0f * fn_80154744());
                }
            }
            if (fn_80154378(dx * dx + dy * dy) > 64.0f) {
                if (field_0x250 == 0) {
                    fn_801546BC(&field_0x2b0);
                    field_0x250 = 1;
                }
            } else {
                field_0x250 = 0;
            }
        }
        if (field_0x248 == 0) {
            fn_801EEC98(fn_801545E8(), &outAngle, &outDir, &outDist, NULL, 0, 0);
            if (outDir >= 0 && outDist < 50.0f) {
                field_0x230 = 17;
            }
        }
        if (field_0x238 == 0) {
            field_0x268 = (s32)(3.99f * fn_80154744());
            switch (lbl_8053AB7C) {
            case 3:
                for (s32 i = 0; i < 4; i++) {
                    if (lbl_804C17A8[field_0x268] == 0) {
                        field_0x2d4.x = field_0x2bc.x + (104.0f - 72.0f * (f32)field_0x268);
                        field_0x2d4.y = field_0x2bc.y - 60.0f;
                        lbl_804C17A8[field_0x268] = 1;
                        field_0x230 = 10;
                        break;
                    }
                    field_0x268 = (field_0x268 + 1) & 3;
                }
                break;
            case 2:
                if (field_0x268 > 2) {
                    field_0x268 = (s32)(2.99f * fn_80154744());
                }
                for (s32 i = 0; i < 3; i++) {
                    if (lbl_804C17A8[field_0x268] == 0) {
                        field_0x2d4.x = field_0x2bc.x + (144.0f - 152.0f * (f32)field_0x268);
                        field_0x2d4.y = field_0x2bc.y - 60.0f;
                        lbl_804C17A8[field_0x268] = 1;
                        field_0x230 = 10;
                        break;
                    }
                    field_0x268++;
                    if (field_0x268 > 2) {
                        field_0x268 = 0;
                    }
                }
                break;
            case 1:
                if (field_0x268 > 1) {
                    field_0x268 = (s32)(1.99f * fn_80154744());
                }
                for (s32 i = 0; i < 2; i++) {
                    if (lbl_804C17A8[field_0x268] == 0) {
                        if (field_0x268 == 0) {
                            field_0x2d4.x = field_0x2bc.x - 96.0f;
                        } else {
                            field_0x2d4.x = field_0x2bc.x + 96.0f;
                        }
                        field_0x2d4.y = field_0x2bc.y - 60.0f;
                        lbl_804C17A8[field_0x268] = 1;
                        field_0x230 = 10;
                        break;
                    }
                    field_0x268++;
                    if (field_0x268 > 1) {
                        field_0x268 = 0;
                    }
                }
                break;
            default:
                field_0x2d4.x = field_0x2bc.x;
                field_0x2d4.y = field_0x2bc.y - 60.0f;
                field_0x230 = 10;
                break;
            }
        }
        break;
    case 10:
        field_0x2b0.x = 0.0f;
        field_0x2b0.y = 0.0f;
        fn_801546BC(&field_0x3c);
        field_0x230 = 11;
        // fall through
    case 11:
        addCalc2_801F71A4(&mPos.x, field_0x2d4.x, 0.5f, 0.08f * fn_8015436C(mPos.x - field_0x2d4.x));
        addCalc2_801F71A4(&mPos.y, field_0x2d4.y, 0.5f, 0.08f * fn_8015436C(mPos.y - field_0x2d4.y));
        if (fn_8015436C(mPos.x - field_0x2d4.x) <= 2.0f && fn_8015436C(mPos.y - field_0x2d4.y) <= 2.0f) {
            field_0x26c = 0;
            field_0x2ec = 0;
            field_0x230 = 12;
            // fall through
        } else {
            break;
        }
    case 12:
        {
            fn_8018A684(&pos60, &mPos);
            u32 ballArg = 10;
            pos60.x -= 20.0f;
            pos60.y -= 110.0f;
            pos60.z = 0.0f;
            if (field_0x264 != 0) {
                ballArg = 11;
            }
            u16 targetPlayer = fn_80189DEC();
            field_0x274 = fn_80154310('FGN2', fn_8015461C(), &pos60, (targetPlayer << 4) | ballArg, -1, -1);
            if (field_0x274 >= 0) {
                field_0x278 = 2;
                field_0x238 = 40;
                PhantomGanon2_804A1A98* ball = (PhantomGanon2_804A1A98*)fn_8015445C(field_0x274);
                if (ball != NULL) {
                    field_0x258 = -1;
                    startSound_801F0E34(0x3002C, 0);
                    ball->field_0x274 = fn_801545E8();
                    field_0x230 = 13;
                }
            }
        }
        break;
    case 13:
        if (field_0x238 == 0) {
            field_0x250 = 0;
            field_0x26c = 0;
            field_0x234 = lbl_804A19A8[field_0x26c];
            if (field_0x274 >= 0) {
                PhantomGanon2_804A1A98* ball = (PhantomGanon2_804A1A98*)fn_8015445C(field_0x274);
                field_0x278 = 1;
                if (ball != NULL) {
                    JGeometry::TVec3f oldPos;
                    fn_801546D0(&oldPos, &mPos);
                    startSound_801F0E34(0x40043, 0);
                    fn_801546BC(&vec54);
                    fn_8018A684(&mPos, ball->fn_801544DC());
                    if (field_0x27c != field_0x94 && field_0x27c != fn_8022F3DC(field_0x94) && field_0x94 != fn_8022F3DC(field_0x27c)) {
                        field_0x94 = field_0x27c;
                    }
                    if (field_0x258 >= 0) {
                        fn_80189DEC();
                        spawnEmitter_8013CC50(field_0x4, ball->fn_801544DC(), lbl_80539FC0[field_0x94], 1, 2, NULL, NULL, true, 1.0f);
                    } else {
                        field_0x258 = (s32)(1.99f * fn_80154744());
                    }
                    fn_801EEC98(fn_801545E8(), &outAngle, &outDir, NULL, NULL, field_0x94, 2);
                    fn_8018A684(&mPos, &oldPos);
                    if (field_0x258 != 0) {
                        outAngle -= field_0x2ec;
                    } else {
                        outAngle += field_0x2ec;
                    }
                    field_0x258 = (field_0x258 + 1) & 1;
                    field_0x2ec = 0x800;
                    fn_801546BC(&vec54);
                    vec54.x = field_0x28c * fn_801544FC(outAngle);
                    vec54.y = field_0x28c * fn_801544E4(outAngle);
                    ball->field_0x2ef = 0;
                    ball->fn_8018A5A8(field_0x94);
                    ball->fn_8017EAF8(vec54);
                    ball->fn_8018A5A0(fn_801F681C(outAngle));
                } else {
                    field_0x274 = -1;
                }
                field_0x230 = 14;
            } else {
                field_0x230 = 8;
            }
        }
        break;
    case 14:
        if (field_0x26c < 4 && field_0x234 == 0) {
            field_0x234 = lbl_804A19A8[field_0x26c];
            field_0x26c++;
            if (field_0x26c == 4) {
                field_0x234 = 20;
                field_0x230 = 15;
            }
        }
        break;
    case 15:
        if (field_0x234 == 0) {
            field_0x230 = 16;
        } else {
            break;
        }
        // fall through
    case 16:
        fn_801EEC98(fn_801545E8(), &outAngle, &outDir, &outDist, NULL, 0, 0);
        if (outDir >= 0 && outDist < 50.0f) {
            field_0x230 = 17;
            break;
        }
        if (field_0x274 >= 0) {
            if (fn_8015445C(field_0x274) == NULL) {
                field_0x274 = -1;
                field_0x230 = 8;
                break;
            }
        } else {
            field_0x230 = 8;
            break;
        }
        if (field_0x234 == 0) {
            if (field_0x278 == 1) {
                field_0x26c--;
                if (field_0x26c < 0) {
                    field_0x26c = 0;
                    field_0x278 = 2;
                }
                field_0x234 = lbl_804A19A8[field_0x26c];
            } else {
                field_0x26c++;
                if (field_0x26c > 7) {
                    field_0x26c = 0;
                }
                field_0x234 = lbl_804A19BC[field_0x26c];
            }
        }
        break;
    case 20:
        field_0x26c = 0;
        field_0x234 = lbl_804A19DC[field_0x26c];
        lbl_804C17A8[0] = 0;
        lbl_804C17A8[1] = 0;
        lbl_804C17A8[2] = 0;
        lbl_804C17A8[3] = 0;
        fn_8018A684(&pos60, &mPos);
        if (mEmitter314 != NULL) {
            fn_8017EAB4(mEmitter314);
            mEmitter314 = NULL;
        }
        if (fn_8018A660() == 0) {
            break;
        }
        if (field_0x260 < 2) {
            fn_80154310('GOLD', fn_8015461C(), &mPos, 99, -1, -1);
            field_0x260++;
        }
        spawnEmitter_8013CC50(field_0x4, &pos60, 0x370, 0, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos60, 0x371, 0, 1, NULL, NULL, true, 1.0f);
        field_0x234 = 10;
        field_0x278 = 3;
        field_0x230 = 21;
        // fall through
    case 21:
        fn_8018A684(&pos60, &mPos);
        pos60.y -= 16.0f;
        fn_8017EC70(&pos60, 1.0f);
        if (field_0x234 == 0) {
            if (field_0x26c < 4) {
                field_0x26c++;
                field_0x234 = lbl_804A19DC[field_0x26c];
            } else {
                field_0x288 = 0.0f;
                field_0x26c = 0;
                field_0x278 = 0;
                field_0x234 = lbl_804A1990[field_0x26c];
                field_0x280 = 3.0f;
                field_0x250 = 0;
                field_0x254 = 0;
                fn_8018A63C();
                field_0x230 = 22;
            }
        }
        break;
    case 17:
        fn_801546BC(&field_0x2b0);
        fn_801546BC(&field_0x3c);
        field_0x26c = 0;
        field_0x234 += 30;
        fn_8018A684(&pos60, &mPos);
        pos60.x -= 20.0f;
        pos60.y -= 70.0f;
        spawnEmitter_8013CC50(field_0x4, &pos60, 0xCB, 1, 5, NULL, NULL, true, 2.0f);
        spawnEmitter_8013CC50(field_0x4, &pos60, 0xCC, 1, 5, NULL, NULL, true, 2.0f);
        field_0x25c = 0;
        field_0x230 = 18;
        // fall through
    case 18:
        if (field_0x234 == 0) {
            field_0x278 = 1;
            fn_80154590(&mBox, -16.0f, -8.0f, 64.0f, 56.0f);
            if (field_0x26c == 0) {
                startSound_801F0E34(0x40043, 0);
                fn_801EEC98(fn_801545E8(), &outAngle, &outDir, NULL, NULL, 0, 0);
                if (outDir < 0) {
                    field_0x230 = 8;
                    break;
                }
                field_0x3c.x = 13.0f * fn_801544FC(outAngle);
                field_0x3c.y = 13.0f * fn_801544E4(outAngle);
            }
            field_0x26c++;
            field_0x234 = lbl_804A19A8[field_0x26c];
            if (field_0x26c == 4) {
                field_0x234 = 50;
                field_0x230 = 19;
            }
        }
        if (field_0x278 == 1) {
            fn_8018A684(&pos60, &mPos);
            mPos.y -= mPos.z;
            mPos.z = 0.0f;
            fn_801F2B7C(fn_80154510(), -1, 6);
            fn_8018A684(&mPos, &pos60);
        }
        break;
    case 19:
        if (field_0x234 != 0) {
            if (fn_801F6758(&mPos, field_0x4, 48.0f)) {
                fn_801546BC(&field_0x2b0);
                fn_801546BC(&field_0x3c);
            }
        } else if (fn_8015436C(field_0x3c.x - field_0x2b0.x) <= 0.1f && fn_8015436C(field_0x3c.y - field_0x2b0.y) <= 0.1f) {
            field_0x26c--;
            if (field_0x26c >= 0) {
                field_0x234 = lbl_804A19A8[field_0x26c];
            } else {
                field_0x26c = 0;
                fn_80154590(&mBox, -16.0f, -8.0f, 16.0f, 40.0f);
                field_0x26c = 0;
                field_0x234 = lbl_804A19BC[field_0x26c];
                field_0x28c = 3.0f;
                field_0x278 = 2;
                field_0x230 = 8;
            }
        }
        break;
    case 22:
        if (mPos.z < 0.1f) {
            mPos.z = 0.0f;
        }
        if (field_0x254 == 0) {
            if (field_0x26c < 5) {
                if (field_0x234 == 0) {
                    field_0x26c++;
                    field_0x234 = lbl_804A1990[field_0x26c];
                }
            } else {
                field_0x254 = 1;
            }
        } else {
            if ((field_0x250 & 0x1F) == 0) {
                startSound_801F0E34(0x40045, 0);
            }
            if (field_0x234 == 0) {
                field_0x26c ^= 1;
                if (field_0x26c == 4) {
                    field_0x234 = 4;
                } else {
                    field_0x234 = 8;
                }
            }
        }
        break;
    case 23:
        if (field_0x234 == 0) {
            field_0x26c--;
            field_0x234 = lbl_804A1990[field_0x26c];
            field_0x280 = 1.0f;
        }
        if (field_0x26c <= 0) {
            field_0x28c = 0.0f;
            field_0x250 = 0;
            field_0x230 = 24;
        }
        break;
    case 24:
        addCalc2_801F71A4(&field_0x2c8.y, 5.0f, 0.3f, 3.0f);
        addCalc2_801F71A4(&field_0x2c8.x, 0.0f, 0.3f, 0.1f);
        if (field_0x2c8.x < 0.1f) {
            fn_8018A684(&mPos, &field_0x2e0);
            fn_8018A684(&field_0x2d4, &mPos);
            field_0x290 = 0.0f;
            field_0x230 = 0;
        }
        break;
    case 29:
        if (field_0x90 != 11) {
            for (s32 i = 0; i < 4; i++) {
                if (i != field_0x94) {
                    if (mEmitters2f4[i] != NULL) {
                        fn_8018A5E0(mEmitters2f4[i]);
                    }
                    if (mEmitters304[i] != NULL) {
                        fn_8018A5E0(mEmitters304[i]);
                    }
                } else {
                    if (mEmitters2f4[i] != NULL) {
                        fn_8018A5D0(mEmitters2f4[i]);
                    }
                    if (mEmitters304[i] != NULL) {
                        fn_8018A5D0(mEmitters304[i]);
                    }
                }
            }
        }
        if (field_0x3c.x != 0.0f || field_0x3c.y != 0.0f) {
            for (s32 i = 0; i < 4; i++) {
                if (mEmitters2f4[i] != NULL) {
                    setEmitterPos_8013CB44(mEmitters2f4[i], &mPos, 1, 2, true);
                }
                if (mEmitters304[i] != NULL) {
                    setEmitterPos_8013CB44(mEmitters304[i], &mPos, 1, 2, true);
                }
            }
            if (field_0x238 == 0) {
                s32 destroyed = 0;
                if (field_0x254 <= 50) {
                    if (field_0x254 == 0) {
                        startSound_801F0E34(0x3002B, 0);
                    }
                    field_0x254++;
                }
                if (field_0x254 > 50 && fn_801F6758(&mPos, fn_8015461C(), -16.0f)) {
                    destroyed = 1;
                    if (field_0x90 == 10 && field_0x2f0 != 0) {
                        for (s32 i = 0; i < 4; i++) {
                            if (mEmitters2f4[i] != NULL) {
                                fn_8018A5F0()->forceDeleteEmitter(mEmitters2f4[i]);
                                mEmitters2f4[i] = NULL;
                            }
                            if (mEmitters304[i] != NULL) {
                                fn_8018A5F0()->forceDeleteEmitter(mEmitters304[i]);
                                mEmitters304[i] = NULL;
                            }
                        }
                        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A1A00[field_0x94 * 2], 1, 2, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A1A00[field_0x94 * 2 + 1], 1, 2, NULL, NULL, true, 1.0f);
                        u16 baseAngle = (u16)fn_8015465C(250.0f - mPos.x, 200.0f - mPos.y) + 0xD000;
                        fn_801546BC(&vec54);
                        for (s32 i = 0; i < 7; i++) {
                            s32 sparkSer = fn_80154310('FGN2', fn_8015461C(), &mPos, 12, -1, -1);
                            if (sparkSer >= 0) {
                                startSound_801F0E34(0x30037, 0);
                                ActorBase_804A5770* spark = fn_8015445C(sparkSer);
                                if (spark != NULL) {
                                    spark->fn_8018A5A8(field_0x94);
                                    s16 a = (s16)baseAngle;
                                    vec54.x = 5.0f * fn_801544FC(a);
                                    vec54.y = 5.0f * fn_801544E4(a);
                                    spark->fn_8017EAF8(vec54);
                                }
                            }
                            baseAngle += 0x1000;
                        }
                        PhantomGanon2_804A1A98* parent = (PhantomGanon2_804A1A98*)fn_8015445C(field_0x274);
                        if (parent != NULL && parent->fn_8018A5B0() == true) {
                            parent->field_0x274 = -1;
                        }
                        fn_8018A598(false);
                        return;
                    }
                }
                if (destroyed == 0) {
                    hitPlayer = fn_801F285C(NULL, NULL, true, -1);
                    if (hitPlayer >= 0 && (field_0x94 == hitPlayer || field_0x94 == fn_8022F3DC(hitPlayer) || hitPlayer == fn_8022F3DC(field_0x94))) {
                        if (field_0x90 == 10) {
                            PhantomGanon2_804A1A98* parent = (PhantomGanon2_804A1A98*)fn_8015445C(field_0x274);
                            if (parent != NULL) {
                                field_0x94 = hitPlayer;
                                field_0x250++;
                                parent->field_0x28c += 0.13f * (f32)field_0x250;
                                if (parent->field_0x28c > 5.0f) {
                                    parent->field_0x28c = 5.0f;
                                }
                                field_0xd8 = 0;
                                fn_8023B6CC(hitPlayer);
                                f32 dx = parent->fn_801544DC()->x - mPos.x;
                                f32 zOff = 8.0f + parent->fn_801544DC()->z;
                                f32 dy = (parent->fn_801544DC()->y - zOff) - mPos.y;
                                outAngle = fn_8015465C(dx, dy);
                                fn_80230FD0(hitPlayer, 0x20038);
                                spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80539FC0[field_0x94], 1, 2, NULL, NULL, true, 1.0f);
                                field_0x3c.x = parent->field_0x28c * fn_801544FC(outAngle);
                                field_0x3c.y = parent->field_0x28c * fn_801544E4(outAngle);
                                field_0x2ef = 1;
                                field_0x238 = 30;
                                field_0x23c = 100;
                                return;
                            }
                        }
                    }
                }
                if (destroyed == 0) {
                    if (field_0x23c == 0 && fn_801F2B7C(-100, -1, 1)) {
                        startSound_801F0E34(0x40042, 0);
                        startSound_801F0E34(0x30037, 0);
                        destroyed = 1;
                    }
                    if ((hitPlayer >= 0 && field_0x90 == 11) || lbl_8053AB84 == 1) {
                        if (hitPlayer >= 0) {
                            fn_8023B6CC(hitPlayer);
                        }
                        if (fn_80154744() < 0.3f) {
                            fn_80154310('HART', fn_8015461C(), &mPos, 0, -1, -1);
                        }
                        destroyed = 1;
                    }
                }
                if (destroyed != 0 || mLife == 0) {
                    PhantomGanon2_804A1A98* parent = (PhantomGanon2_804A1A98*)fn_8015445C(field_0x274);
                    for (s32 i = 0; i < 4; i++) {
                        if (mEmitters2f4[i] != NULL) {
                            fn_8018A5F0()->forceDeleteEmitter(mEmitters2f4[i]);
                            mEmitters2f4[i] = NULL;
                        }
                        if (mEmitters304[i] != NULL) {
                            fn_8018A5F0()->forceDeleteEmitter(mEmitters304[i]);
                            mEmitters304[i] = NULL;
                        }
                    }
                    if (field_0x90 == 10) {
                        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A1A00[field_0x94 * 2], 1, 2, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A1A00[field_0x94 * 2 + 1], 1, 2, NULL, NULL, true, 1.0f);
                    } else {
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x36E, 1, 2, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x36F, 1, 2, NULL, NULL, true, 1.0f);
                    }
                    if (parent != NULL) {
                        parent->field_0x28c = 3.0f;
                        parent->field_0x274 = -1;
                        field_0x274 = -1;
                    }
                    fn_8018A598(false);
                }
            }
        }
        break;
    case 30:
        field_0x94 &= 3;
        mEmitters2f4[0] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A19F0[field_0x94 * 2], 1, 2, NULL, NULL, true, 0.5f);
        mEmitters304[0] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A19F0[field_0x94 * 2 + 1], 1, 2, NULL, NULL, true, 0.5f);
        field_0x230 = 31;
        // fall through
    case 31:
        if (mEmitters2f4[0] != NULL) {
            setEmitterPos_8013CB44(mEmitters2f4[0], &mPos, 1, 2, true);
            fn_8018A5C8(mEmitters2f4[0], (u8)field_0x290);
        }
        if (mEmitters304[0] != NULL) {
            setEmitterPos_8013CB44(mEmitters304[0], &mPos, 1, 2, true);
            fn_8018A5C8(mEmitters304[0], (u8)field_0x290);
        }
        if (fn_801F2B7C(-100, -1, 1)) {
            if (mEmitters2f4[0] != NULL) {
                fn_8018A5F0()->forceDeleteEmitter(mEmitters2f4[0]);
                mEmitters2f4[0] = NULL;
            }
            if (mEmitters304[0] != NULL) {
                fn_8018A5F0()->forceDeleteEmitter(mEmitters304[0]);
                mEmitters304[0] = NULL;
            }
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x36E, 1, 2, NULL, NULL, true, 0.5f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x36F, 1, 2, NULL, NULL, true, 0.5f);
            startSound_801F0E34(0x30037, 0);
            fn_8018A598(false);
        } else if (field_0x234 == 0 || lbl_8053AB84 == 1) {
            addCalc2_801F71A4(&field_0x290, 0.0f, 0.5f, 5.0f);
            if (field_0x290 < 10.0f) {
                fn_8018A598(false);
            }
        }
        break;
    case 25:
        if (fn_8018A660() != 0) {
            field_0x26c = 0;
            field_0x24c = 0;
            field_0x250 = 0;
            field_0x234 = 0;
            field_0x278 = 3;
            field_0x26c = 4;
            field_0x280 = 10.0f;
            field_0x288 = 40.0f;
            field_0x2ee = 0;
            field_0x230 = 26;
        } else {
            break;
        }
        // fall through
    case 26:
        if (fn_8015436C(mPos.z - field_0x288) <= 1.0f) {
            if (mEmitter314 != NULL) {
                fn_8017EAB4(mEmitter314);
                mEmitter314 = NULL;
            }
            fn_8018A63C();
            vfunc_0x24();
        }
        break;
    case 28:
        addCalc2_801F71A4(&field_0x2c8.y, 5.0f, 0.3f, 3.0f);
        addCalc2_801F71A4(&field_0x2c8.x, 0.0f, 0.3f, 0.1f);
        if (field_0x2c8.x < 0.1f) {
            if (mEmitter314 != NULL) {
                fn_8017EAB4(mEmitter314);
                mEmitter314 = NULL;
            }
            fn_8018A598(false);
        }
        break;
    }
    if (field_0x230 == 9 || field_0x230 == 16 || field_0x230 == 23) {
        field_0x288 = 40.0f + 4.0f * fn_801544E4((s16)field_0x24c);
        field_0x24c += 0x300;
    }
    if (field_0x264 != 0) {
        if (lbl_8053AB84 == 1) {
            startSound_801F0E34(0x40047, 0);
            if (field_0x268 >= 0) {
                lbl_804C17A8[field_0x268] = 0;
            }
            field_0x268 = -1;
            if (field_0x274 >= 0) {
                ActorBase_804A5770* ball = fn_8015445C(field_0x274);
                if (ball != NULL) {
                    ball->fn_8018A598(false);
                }
            }
            field_0x230 = 28;
        }
    } else if (field_0x230 == 15 || field_0x230 == 16 || field_0x230 == 18 || field_0x230 == 19) {
        f32 savedZ = mPos.z;
        mPos.y -= savedZ;
        mPos.z = 0.0f;
        s32 hitSer = fn_801F2718('FGN2');
        mPos.z = savedZ;
        mPos.y += mPos.z;
        if (hitSer >= 0) {
            ActorBase_804A5770* hitAct = fn_8015445C(hitSer);
            if (hitAct != NULL) {
                if (field_0x94 == hitAct->fn_8018A590() ||
                    field_0x94 == fn_8022F3DC(hitAct->fn_8018A590()) ||
                    hitAct->fn_8018A590() == fn_8022F3DC(field_0x94))
                {
                    if (hitAct->fn_801544C8() == 10 && field_0x230 != 18 && field_0x230 != 19) {
                        PhantomGanon2_804A1A98* ball = (PhantomGanon2_804A1A98*)hitAct;
                        if (ball->field_0x2ef == 1) {
                            f32 thresh = 5.0f;
                            if (lbl_8053AB78 > 1) {
                                thresh = 4.0f;
                            }
                            ball->field_0x238 = 0;
                            ball->field_0x23c = 0;
                            if (field_0x28c >= thresh) {
                                ball->fn_8018A598(false);
                                if (field_0x268 >= 0) {
                                    lbl_804C17A8[field_0x268] = 0;
                                }
                                field_0x268 = -1;
                                lbl_8053AB7C--;
                                lbl_8053AB78--;
                                if (lbl_8053AB78 <= 0) {
                                    hitAct->setLife(0);
                                    lbl_8053AB78 = 0;
                                    lbl_8053AB7C = 0;
                                    lbl_8053AB84 = 1;
                                    field_0x90 = 0;
                                    field_0x94 = 0;
                                    field_0x27c = field_0x94;
                                    field_0x270 = lbl_80539FB8;
                                    fn_8018A684(&field_0x2d4, &mPos);
                                    startSound_801F0E34(0x40044, 0);
                                    field_0x230 = 20;
                                } else {
                                    spawnEmitter_8013CC50(field_0x4, hitAct->fn_801544DC(), 0x556, 0, 1, NULL, NULL, true, 0.1f);
                                    startSound_801F0E34(0x40047, 0);
                                    field_0x230 = 28;
                                }
                            } else {
                                ball->field_0x2ef = 0;
                                ball->field_0x254 = 1;
                                startSound_801F0E34(0x3002B, 0);
                                field_0x230 = 13;
                            }
                        }
                    }
                }
            }
        }
    }
}

// FIXME(shotgun): 77.36 %, missing/extra + reloc-name. The original addresses the tables off one base register (r31), here they are extern arrays.
void PhantomGanon2_804A1A98::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    JGeometry::TVec3f pos = mPos;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1f = 0;
    if (field_0x90 == 0 || field_0x90 == 1) {
        mPos.z = 0.0f;
        f32 scaleX = 0.0f;
        f32 scaleY = 0.0f;
        void* res;
        switch (field_0x278) {
        case 0:
            res = fn_804032E8('FGAN', lbl_804A1A10[field_0x26c]);
            break;
        case 1:
            res = fn_804032E8('FGAN', lbl_804A1A50[field_0x26c]);
            scaleX = lbl_804A1A28[field_0x26c][0];
            scaleY = lbl_804A1A28[field_0x26c][1];
            break;
        case 2:
            res = fn_804032E8('FGAN', lbl_804A1A64[field_0x26c]);
            scaleY = -21.0f;
            break;
        case 3:
            res = fn_804032E8('FGAN', lbl_804A1A84[field_0x26c]);
            break;
        }
        params.field_0x1c = 0;
        params.field_0x1d = -2;
        params.field_0x10.set(0, 0, 0, (u8)(0.39f * field_0x290));
        mtx[0][0] = 1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 0.8f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 0.8f;
        params.mMtx = mtx;
        fn_801EFD34(res, &params, -1, scaleX, scaleY, 0.0f);
        mPos.set(pos);
        params.field_0x1d = field_0x2ee;
        params.field_0x30 = 14;
        mtx[0][0] = field_0x2c8.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x2c8.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x2c8.y;
        params.mMtx = mtx;
        params.field_0x10.set(0xFF, 0xFF, 0xFF, (u8)field_0x290);
        fn_801EFD34(res, &params, -1, scaleX, scaleY, 0.0f);
    }
}

// 0x80187144 (vtable slot 0x24): plays sound 0x40047 and re-issues the actor's
// vfunc_0x28 with the s8 at 0x2EE and (8.0, -16.0).
void PhantomGanon2_804A1A98::vfunc_0x24() {
    startSound_801F0E34(0x40047, 0);
    vfunc_0x28(field_0x2ee, 8.0f, -16.0f);
}

