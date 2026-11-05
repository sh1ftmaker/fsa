// Unnamed unit (0x801CB270-0x801CDC8C): the texture loader at its start and
// small helpers the enemy code shares (random picks, steppers, actor tests).

#include "game/GameAudio.h"

#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_bombchu.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/ResTIMG.h"
#include "dolphin/gx.h"

// Looks up the TIMG resource `name` in the game archive and initialises
// texObj (and tlutObj for a palette texture, if given) from it. Returns the
// image's height / width.
// FIXME: 89.7 %. Same instructions apart from scheduling: the target computes
// the mipmap flag straight into r10 and orders the GXInitTexObjLOD argument
// loads differently. JUTTexture::initTexObj's forms (? GX_TRUE : GX_FALSE,
// image +=) and isMIPmapEnabled() were worse.
f32 fn_801CB270(const char* name, GXTexObj* texObj, GXTlutObj* tlutObj) {
    ResTIMG* timg = (ResTIMG*)lbl_8053AAF8->field_0xa4->getResource('TIMG', name);
    GXBool mipmap = timg->mIsMIPmapEnabled > 0;
    u32 imageOffset = timg->mImageDataOffset != 0 ? timg->mImageDataOffset : 0x20;
    void* image = (u8*)timg + imageOffset;

    if (timg->mPaletteFormat) {
        GXInitTexObjCI(texObj, image, timg->mSizeX, timg->mSizeY, (GXCITexFmt)timg->mTextureFormat,
                       (GXTexWrapMode)timg->mWrapS, (GXTexWrapMode)timg->mWrapT, mipmap, 0);
        if (tlutObj != NULL) {
            GXInitTlutObj(tlutObj, (u8*)timg + timg->mPaletteOffset, (GXTlutFmt)timg->mLutFormat,
                          timg->mPaletteEntryCount);
        }
    } else {
        GXInitTexObj(texObj, image, timg->mSizeX, timg->mSizeY, (GXTexFmt)timg->mTextureFormat,
                     (GXTexWrapMode)timg->mWrapS, (GXTexWrapMode)timg->mWrapT, mipmap);
    }
    GXInitTexObjLOD(texObj, (GXTexFilter)timg->mMinFilterType, (GXTexFilter)timg->mMagFilterType,
                    timg->mMinLOD * 0.125f, timg->mMaxLOD * 0.125f, timg->mLODBias * 0.01f,
                    timg->mIsBiasClamp, timg->mDoEdgeLOD, (GXAnisotropy)timg->mIsMaxAnisotropy);
    return timg->mSizeY * (1.0f / timg->mSizeX);
}

// Asks the actor manager for a 'RUPY' (fn_801F8544 with id ACTOR_RUPY) at pos on
// `layer`; the flag picks 5 or 4 in the low bits of its last argument.
s32 fn_801CB434(s32 layer, const JGeometry::TVec3f* pos, bool flag) {
    return getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, layer, pos, 0x261E00 | (flag ? 5 : 4));
}

// The kind of a Force Gem ('RUPY', act_force_gem): the low byte of its
// spawn bits while field_0x94 is 0 or 3, else -1. The actor manager's
// queries (fn_801F7274, fn_801F7464, fn_801F750C) and tu_801DE714 filter
// actors with these three.
static inline s32 forceGemKind(ActorBase_804A5770* actor) {
    s32 kind = -1;
    if (actor->field_0x94 == 0 || actor->field_0x94 == 3) {
        kind = actor->field_0x90 & 0xFF;
    }
    return kind;
}

// FIXME: 86.5 %. Same instructions; the target keeps layer, depth, height and pos in
// r31, r30, r29, r28 (ours: the reverse of the parameter order).
// Two effects (spawnEmitter_8013CC50 0x473 and 0x474) at pos on `layer`; arg3 and arg4 + 1
// are the height and depth of both.
void fn_801CB4A4(s32 layer, const JGeometry::TVec3f* pos, s32 arg3, s32 arg4) {
    spawnEmitter_8013CC50(layer, pos, 0x473, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x474, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
}

// FIXME: 86.5 %. As fn_801CB4A4: saved registers are assigned in a different order.
// Two effects (spawnEmitter_8013CC50 0x117 and 0x118) at pos on `layer`; arg3 is passed
// on and arg4 is passed on plus 1 (as the depth).
void fn_801CB538(s32 layer, const JGeometry::TVec3f* pos, s32 arg3, s32 arg4) {
    spawnEmitter_8013CC50(layer, pos, 0x117, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x118, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
}

// FIXME: 86.5 %. As fn_801CB4A4: saved registers are assigned in a different order.
// Two effects (spawnEmitter_8013CC50 0x24A and 0x24B) at pos on `layer`; arg3 is passed
// on and arg4 is passed on plus 1 (as the depth).
void fn_801CB688(s32 layer, const JGeometry::TVec3f* pos, s32 arg3, s32 arg4) {
    spawnEmitter_8013CC50(layer, pos, 0x24A, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x24B, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
}

// FIXME: 93.9 %. The target has no second NULL test of *emitter in front of the
// manager call (deleteEmitter()'s own); everything else is identical.
// Moves the emitter in *emitter to pos with setEmitterPos_8013CB44 (adjust true); or,
// when it is flagged (bit 0x8 of its word 0xF4) and has no particles left
// (0xD0 + 0xDC == 0), deletes it and sets *emitter to NULL. Does nothing for
// a NULL emitter.
void fn_801CB71C(JPABaseEmitter** emitter, const JGeometry::TVec3f* pos, s32 height, s32 depth) {
    if (*emitter != NULL) {
        if ((*emitter)->isEnableDeleteEmitter()) {
            deleteEmitter(emitter);
        } else {
            setEmitterPos_8013CB44(*emitter, pos, height, depth, true);
        }
    }
}

// Makes the emitter immortal and then invalid (it stops creating particles and
// is deleted once they are gone). Does nothing for a NULL emitter.
void fn_801CB7A0(JPABaseEmitter* emitter) {
    if (emitter != NULL) {
        emitter->setStatus(JPAEMIT_Immortal);
        emitter->becomeInvalidEmitter();
    }
}

// Maps a 16-bit angle to the one mirrored about the 0x4000 axes (0x8000 - a
// on every quadrant of the unsigned angle).
s32 fn_801CB7CC(s16 angle) {
    u16 a = angle;
    if (a <= 0x4000) {
        return (s16)((u16)(0x4000 - a) + 0x4000);
    }
    if (a >= 0x4000 && a <= 0x8000) {
        return (s16)(0x4000 - (u16)(a - 0x4000));
    }
    if (a >= 0x8000 && a <= 0xC000) {
        return (s16)((u16)(0xC000 - a) + 0x10000 - 0x4000);
    }
    return (s16)(0xC000 - (u16)(a - 0x10000 + 0x4000));
}

// Whether `actor` is a live Bombchu ('BMTY'): its field_0x230 is not 3.
bool fn_801CC8E4(ActorBase_804A5770* actor) {
    bool result = false;
    if (actor->mActorId == ACTOR_BMTY) {
        if (static_cast<Bombchu_804AB464*>(actor)->field_0x230 != 3) {
            result = true;
        }
    }
    return result;
}

// Whether `actor` is a Force Gem of kind 8-11.
bool fn_801CC910(ActorBase_804A5770* actor) {
    bool result = false;
    if (actor->mActorId == ACTOR_RUPY) {
        s32 kind = forceGemKind(actor);
        if (kind >= 8 && kind <= 11) {
            result = true;
        }
    }
    return result;
}

// Whether `actor` is a Force Gem of kind 4-7.
bool fn_801CC960(ActorBase_804A5770* actor) {
    bool result = false;
    if (actor->mActorId == ACTOR_RUPY) {
        s32 kind = forceGemKind(actor);
        if (kind >= 4 && kind <= 7) {
            result = true;
        }
    }
    return result;
}

// Whether `actor` is a Force Gem with a kind (0 or more).
bool fn_801CC9B0(ActorBase_804A5770* actor) {
    bool result = false;
    if (actor->mActorId == ACTOR_RUPY) {
        s32 kind = forceGemKind(actor);
        if (kind >= 0) {
            result = true;
        }
    }
    return result;
}

// Whether the tile at pos is one of the codes 0x40-0x49.
bool fn_801CCC78(const JGeometry::TVec3f* pos, s32 layer) {
    bool result = false;
    switch (fn_80226998(pos, layer)) {
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
        result = true;
        break;
    }
    return result;
}

// Snaps pos (x, y) down to a multiple of 16 and adds 8 (the centre of a 16x16
// tile).
void fn_801CCCC4(JGeometry::TVec3f* pos) {
    pos->x = 8.0f + (u32)((s32)pos->x & ~0xF);
    pos->y = 8.0f + (u32)((s32)pos->y & ~0xF);
}

// Like fn_801CCCC4 but the snapped (x, y) of `in` goes to `out`.
void fn_801CCD34(const JGeometry::TVec3f* in, JGeometry::TVec3f* out) {
    out->x = 8.0f + (u32)((s32)in->x & ~0xF);
    out->y = 8.0f + (u32)((s32)in->y & ~0xF);
}

// The player (fn_8023F264) whose fn_80236228 value is the lowest, or -1 when
// none qualifies or the lowest is shared.
s32 fn_801CCDA4() {
    // Declared in this order for the original's register assignment.
    s32 best = -1;
    s32 tie = 0x7FFFFFFF;
    s32 lowest = tie;
    s32 count = lbl_8053AAF8->fn_80138A30();
    for (s32 i = 0; i < count; i++) {
        if (fn_8023F264(i)) {
            s32 value = fn_80236228(i);
            if (value < lowest) {
                lowest = value;
                best = i;
            } else if (value == lowest) {
                tie = lowest;
            }
        }
    }
    if (tie == lowest) {
        best = -1;
    }
    return best;
}

// fn_80236228 for the player that `player` stands for: the one
// fn_8022F3DC names while fn_8022F514 says it has one, else itself.
s32 fn_801CCF34(s32 player) {
    s32 target = player;
    if (fn_8022F514(player)) {
        target = fn_8022F3DC(player);
    }
    return fn_80236228(target);
}

// Whether fn_80230188 holds for every player that takes part (fn_80138A30).
bool fn_801CCF7C() {
    s32 count = lbl_8053AAF8->fn_80138A30();
    for (s32 i = 0; i < count; i++) {
        if (!fn_80230188(i)) {
            return false;
        }
    }
    return true;
}

// FIXME: 79.1 %. Same instructions; the target loads and subtracts both
// coordinates first, then adds the box corner and loads z last (ours interleaves x and y).
// Converts `in` from the view's 0xDC/0xE0 box space to the 0xCC/0xD0 one:
// out = (in.x, in.y) - (0xDC, 0xE0) + (0xCC, 0xD0); z is copied.
void fn_801CD0D0(const JGeometry::TVec3f* in, JGeometry::TVec3f* out) {
    GameView_804A9950* view = getGameView_802D6A9C();
    GameView_804A9950* upper = getGameView_802D6A9C();
    out->x = view->mVisibleBox.i.x + (in->x - upper->field_0xdc.i.x);
    out->y = view->mVisibleBox.i.y + (in->y - upper->field_0xdc.i.y);
    out->z = in->z;
}

// FIXME: 79.1 %. As fn_801CD0D0 (scheduling of the x / y loads and subtracts).
// The other way round: from the 0xCC/0xD0 box space to the 0xDC/0xE0 one.
void fn_801CD150(const JGeometry::TVec3f* in, JGeometry::TVec3f* out) {
    GameView_804A9950* view = getGameView_802D6A9C();
    GameView_804A9950* upper = getGameView_802D6A9C();
    out->x = upper->field_0xdc.i.x + (in->x - view->mVisibleBox.i.x);
    out->y = upper->field_0xdc.i.y + (in->y - view->mVisibleBox.i.y);
    out->z = in->z;
}

// Whether `player` has no owner (fn_8022F514 is false) and fn_80303750 says yes.
bool fn_801CD1D0(s32 player) {
    bool result = false;
    if (!fn_8022F514(player)) {
        if (fn_80303750(player)) {
            result = true;
        }
    }
    return result;
}

// Whether every player's layer (getPlayerLayer_8023DE58) is `layer`.
bool fn_801CD228(s32 layer) {
    bool result = true;
    for (s32 i = 0; i < 4; i++) {
        if (layer != getPlayerLayer_8023DE58(i)) {
            result = false;
            break;
        }
    }
    return result;
}

// Whether any player's layer (getPlayerLayer_8023DE58) is `layer`.
bool fn_801CD290(s32 layer) {
    bool result = false;
    for (s32 i = 0; i < 4; i++) {
        if (layer == getPlayerLayer_8023DE58(i)) {
            result = true;
            break;
        }
    }
    return result;
}

// Whether `layer` is one of the upper layers (8 and up).
bool fn_801CD2F8(s32 layer) {
    bool result = false;
    if (layer >= 8) {
        result = true;
    }
    return result;
}

// Maps 0 to 3, 1 to 2, 3 to 1; 2 and anything else to 0.
s32 fn_801CD30C(s32 value) {
    s32 result = 0;
    switch (value) {
    case 0:
        result = 3;
        break;
    case 1:
        result = 2;
        break;
    case 3:
        result = 1;
        break;
    }
    return result;
}

// Swaps the pairs 0<->1 and 2<->3; any other value maps to 0.
s32 fn_801CD354(s32 value) {
    s32 result = 0;
    switch (value) {
    case 0:
        result = 1;
        break;
    case 2:
        result = 3;
        break;
    case 3:
        result = 2;
        break;
    }
    return result;
}

// A random integer in [0, 4) other than `except` (0-3, so at most three
// candidates remain).
s32 fn_801CD580(s32 except) {
    s32 choices[3];
    s32* out = choices;
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i != except) {
            *out++ = i;
            count++;
        }
    }
    return choices[(s32)(count * lbl_8053AAF8->mRandom.get_ufloat_1())];
}

// A random integer in [0, n).
s32 fn_801CD664(s32 n) {
    return n * lbl_8053AAF8->mRandom.get_ufloat_1();
}

// Catmull-Rom spline through p1 (t = 0) and p2 (t = 1), with p0 and p3 as
// the neighbours.
f32 fn_801CD6D8(f32 p0, f32 p1, f32 p2, f32 p3, f32 t) {
    // half before t2: the original's register assignment.
    f32 half = 0.5f * t;
    f32 t2 = t * t;
    f32 t3 = t2 * t;
    return p0 * (-0.5f * t3 + t2 - half) + p1 * (1.0f + (1.5f * t3 - 2.5f * t2)) +
           p2 * (half + (-1.5f * t3 + 2.0f * t2)) + p3 * (0.5f * t3 - 0.5f * t2);
}

// Moves *value towards target by the fraction `factor` (at most 1) of the
// distance; true while it is further than `threshold` away.
bool fn_801CD8E4(f32* value, f32 target, f32 factor, f32 threshold) {
    if (factor > 1.0f) {
        factor = 1.0f;
    }
    threshold = __fabsf(threshold);
    *value += factor * (target - *value);
    if (__fabsf(*value - target) <= threshold) {
        return false;
    }
    return true;
}

// Steps *angle towards target by at most step; the result of fn_801CD970 is
// not used.
void fn_801CD930(s16* angle, s32 target, s32 step) {
    fn_801CD970(angle, target, step);
}

// Moves *value toward target by `step`; nonzero while it has not reached the
// target (see EnemyBase.h).
s32 fn_801CD950(f32* value, f32 target, f32 step) {
    return fn_801CD9F0(value, target, step);
}

// FIXME: 90.6 %. Same instructions modulo registers: the target extends target and step
// once up front (extsh r7, r4; extsh. r0, r5) and keeps them in r7 / r5; ours keeps the
// extended target in r6 and the step in r7. With (s16, s16) parameters this is 100 %, but
// then fn_801CD930's callers (it forwards r4 / r5 unextended) would need extsh.
// Steps *angle towards target by at most step; true while it has not reached
// the target.
bool fn_801CD970(s16* angle, s32 target, s32 step) {
    s16 diff = (s16)target - *angle;
    s16 amount = step;
    if (amount < 0) {
        amount = -amount;
    }
    if (diff > 0) {
        diff -= amount;
        if (diff > 0) {
            *angle = (s16)target - diff;
        } else {
            *angle = target;
        }
    } else {
        diff += amount;
        if (diff < 0) {
            *angle = (s16)target - diff;
        } else {
            *angle = target;
        }
    }
    bool result = true;
    if (*angle == (s16)target) {
        result = false;
    }
    return result;
}

// Moves *value towards target by at most step; nonzero while it has not
// reached the target.
s32 fn_801CD9F0(f32* value, f32 target, f32 step) {
    f32 diff = target - *value;
    if (step < 0.0f) {
        step = -step;
    }
    if (diff > 0.0f) {
        diff -= step;
        if (diff > 0.0f) {
            *value = target - diff;
        } else {
            *value = target;
        }
    } else {
        diff += step;
        if (diff < 0.0f) {
            *value = target - diff;
        } else {
            *value = target;
        }
    }
    s32 result = 1;
    if (*value == target) {
        result = 0;
    }
    return result;
}
