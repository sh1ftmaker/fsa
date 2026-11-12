// Unnamed unit (0x801CB270-0x801CDC8C): the texture loader at its start and
// small helpers the enemy code shares (random picks, steppers, actor tests).

#include <float.h>

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
#include "game/act/tu_801688AC.h"
#include "game/Map.h"
#include "game/ViewLayer.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/ResTIMG.h"
#include "dolphin/gx.h"
#include "JSystem/JMath/JMath.h"

// Forward declaration (defined at its place below fn_801CC9F8).
f32 tileCenter(f32 v);




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

// Not auto-inlined: fn_801CD950 calls fn_801CD9F0 and fn_801CD930 calls fn_801CD970 out of line
// in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
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

// FIXME: 90.62 %, scheduling+regalloc; behaviour clean.
// Same instructions modulo registers: the target extends target and step
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
#pragma auto_inline reset

// Moves *value toward target by `step`; nonzero while it has not reached the
// target (see EnemyBase.h).
s32 fn_801CD950(f32* value, f32 target, f32 step) {
    return fn_801CD9F0(value, target, step);
}

// Steps *angle towards target by at most step; the result of fn_801CD970 is
// not used.
void fn_801CD930(s16* angle, s32 target, s32 step) {
    fn_801CD970(angle, target, step);
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

// Moves *cur toward *target by `factor` of the way, the step clamped between minStep and
// maxStep; true once the point is on the target.
// FIXME(shotgun): 85.76 %, missing/extra; behaviour drift.
bool fn_801CD754(JGeometry::TVec2f* cur, const JGeometry::TVec2f* target, f32 factor, f32 minStep, f32 maxStep) {
    if (factor > 1.0f) {
        factor = 1.0f;
    } else if (factor < 0.0f) {
        factor = 0.0f;
    }
    JGeometry::TVec2f d(target->x - cur->x, target->y - cur->y);
    f32 len = d.length();
    if (len > 0.0f) {
        f32 step = len * factor;
        if (step < minStep) {
            f32 f = (1.0f / len) * minStep;
            if (f > 1.0f) {
                f = 1.0f;
            } else if (f < factor) {
                f = factor;
            }
            factor = f;
        } else if (step > maxStep) {
            f32 f = (1.0f / len) * maxStep;
            if (f > factor) {
                f = factor;
            } else if (f < 0.0f) {
                f = 0.0f;
            }
            factor = f;
        }
        f32 fx = factor;
        if (factor > 1.0f) {
            fx = 1.0f;
        }
        cur->x = fx * (target->x - cur->x) + cur->x;
        if (factor > 1.0f) {
            factor = 1.0f;
        }
        cur->y = factor * (target->y - cur->y) + cur->y;
    }
    bool arrived = false;
    if (cur->x == target->x && cur->y == target->y) {
        arrived = true;
    }
    return arrived;
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

// A random integer in [0, n).
s32 fn_801CD664(s32 n) {
    return n * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
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
    return choices[(s32)(count * gGameRoot_8053AAF8->mRandom.get_ufloat_1())];
}

// A random integer in [0, 4) other than a and b.
// FIXME(shotgun): 69.26 %, missing/extra; behaviour drift.
s32 fn_801CD398(s32 a, s32 b) {
    s32 choices[4];
    s32 count = 0;
    s32* out = choices;
    if (a == b) {
        for (s32 i = 0; i < 4; i++) {
            if (i != a) {
                *out++ = i;
                count++;
            }
        }
    } else {
        for (s32 i = 0; i < 4; i++) {
            if (i != a && i != b) {
                *out++ = i;
                count++;
            }
        }
    }
    return choices[(s32)(count * gGameRoot_8053AAF8->mRandom.get_ufloat_1())];
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

// Whether `layer` is one of the upper layers (8 and up).
bool fn_801CD2F8(s32 layer) {
    bool result = false;
    if (layer >= 8) {
        result = true;
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

// FIXME: 79.09 %, scheduling+regalloc; behaviour clean.
// As fn_801CD0D0 (scheduling of the x / y loads and subtracts).
// The other way round: from the 0xCC/0xD0 box space to the 0xDC/0xE0 one.
void fn_801CD150(const JGeometry::TVec3f* in, JGeometry::TVec3f* out) {
    GameView_804A9950* view = getGameView_802D6A9C();
    GameView_804A9950* upper = getGameView_802D6A9C();
    out->x = upper->field_0xdc.i.x + (in->x - view->mVisibleBox.i.x);
    out->y = upper->field_0xdc.i.y + (in->y - view->mVisibleBox.i.y);
    out->z = in->z;
}

// FIXME: 79.09 %, scheduling+regalloc; behaviour clean.
// Same instructions; the target loads and subtracts both
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

// FIXME(shotgun): 98.14 %, missing/extra; behaviour clean.
bool fn_801CCFE4(s32 player) {
    bool result = false;
    if (fn_8023F264(player)) {
        if (!fn_80236B50(player) && !fn_8022DCD8(player) && !fn_80234C5C(player) && !fn_8023EA28(player) &&
            !fn_80230188(player) && !fn_8022D534(player) && !fn_802349EC(player)) {
            s32 mode = fn_8022CAF8(player);
            if (mode < 6 && mode >= 4) {
                if (!fn_8022F478(player)) {
                    result = true;
                }
            } else {
                result = true;
            }
        }
    }
    return result;
}

// Whether fn_80230188 holds for every player that takes part (fn_80138A30).
bool fn_801CCF7C() {
    s32 count = gGameRoot_8053AAF8->fn_80138A30();
    for (s32 i = 0; i < count; i++) {
        if (!fn_80230188(i)) {
            return false;
        }
    }
    return true;
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

// FIXME: 96.94 %, missing/extra; behaviour near.
// FIXME(shotgun): the original does not contract dx*dx + dy*dy into fmadds.
s32 fn_801CCE3C(const JGeometry::TVec3f* pos, s32 layer) {
    s32 best = -1;
    f32 bestDist = FLT_MAX;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i) && layer == getPlayerLayer_8023DE58(i) && !fn_8022F514(i)) {
            f32 dy = getPlayerPos_8023E724(i)->y - pos->y;
            f32 dx = getPlayerPos_8023E724(i)->x - pos->x;
            f32 d = dx * dx + dy * dy;
            if (d < bestDist) {
                bestDist = d;
                best = i;
            }
        }
    }
    return best;
}

// The player (fn_8023F264) whose fn_80236228 value is the lowest, or -1 when
// none qualifies or the lowest is shared.
s32 fn_801CCDA4() {
    // Declared in this order for the original's register assignment.
    s32 best = -1;
    s32 tie = 0x7FFFFFFF;
    s32 lowest = tie;
    s32 count = gGameRoot_8053AAF8->fn_80138A30();
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

// Like fn_801CCCC4 but the snapped (x, y) of `in` goes to `out`.
void fn_801CCD34(const JGeometry::TVec3f* in, JGeometry::TVec3f* out) {
    out->x = tileCenter(in->x);
    out->y = tileCenter(in->y);
}

// Snaps pos (x, y) down to a multiple of 16 and adds 8 (the centre of a 16x16
// tile).
void fn_801CCCC4(JGeometry::TVec3f* pos) {
    pos->x = tileCenter(pos->x);
    pos->y = tileCenter(pos->y);
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

// FIXME(shotgun): 97.12 %, missing/extra; behaviour clean.
bool fn_801CCBD0(const JGeometry::TVec3f* pos, s32 layer) {
    JGeometry::TVec3f p;
    p.x = pos->x;
    p.y = pos->y;
    bool found = false;
    for (s32 i = 0; i < 2; i++) {
        s32 tile = fn_80226998(&p, layer);
        if (tile >= 0x19C && tile < 0x19E) {
            found = true;
        }
        p.y -= 16.0f;
    }
    return found;
}

// Whether the tile (fn_802265FC) at pos (x, y - z) and the one 16 units further down are both
// code 2, on a layer with the view's flag. fn_802265FC reads only x and y: the probes are TVec2f
// on the stack (act_bomb passes its centre the same way).
bool fn_801CCB1C(const JGeometry::TVec3f* pos, s32 layer) {
    bool result = false;
    JGeometry::TVec2f p(pos->x, pos->y - pos->z);
    if (viewLayerFlag(layer) && fn_802265FC((const JGeometry::TVec3f*)&p, layer) == 2) {
        JGeometry::TVec2f below(p.x, 16.0f + p.y);
        if (fn_802265FC((const JGeometry::TVec3f*)&below, layer) == 2) {
            result = true;
        }
    }
    return result;
}

// Whether pos and the four points 7 units left, right, up and down of it all have tile code 0
// (fn_802265FC), on a layer with the view's flag. The probes are TVec2f, as in fn_801CCB1C.
bool fn_801CC9F8(const JGeometry::TVec3f* pos, s32 layer) {
    bool result = false;
    if (viewLayerFlag(layer) && fn_802265FC(pos, layer) == 0) {
        JGeometry::TVec2f right(7.0f + pos->x, pos->y);
        if (fn_802265FC((const JGeometry::TVec3f*)&right, layer) == 0) {
            JGeometry::TVec2f left(pos->x - 7.0f, pos->y);
            if (fn_802265FC((const JGeometry::TVec3f*)&left, layer) == 0) {
                JGeometry::TVec2f down(pos->x, 7.0f + pos->y);
                if (fn_802265FC((const JGeometry::TVec3f*)&down, layer) == 0) {
                    JGeometry::TVec2f up(pos->x, pos->y - 7.0f);
                    if (fn_802265FC((const JGeometry::TVec3f*)&up, layer) == 0) {
                        result = true;
                    }
                }
            }
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

// The centre of the 16-unit tile holding v. A plain function the linker dead-stripped: its four
// copies are inlined in fn_801CCD34 and fn_801CCCC4, and its 8.0f sits in .sdata2 between
// fn_801CC350's literals and fn_801CC9F8's, where its own copy was emitted (RULES_CARD,
// "Literals before their first visible use" 2-3).
f32 tileCenter(f32 v) {
    return 8.0f + (u32)((s32)v & ~0xF);
}

// FIXME(shotgun): 95.47 %, scheduling+regalloc; behaviour clean.
bool fn_801CC820(s32 player, JGeometry::TVec3f* out) {
    bool found = false;
    if (out != NULL) {
        out->zero();
    }
    s32 serial = fn_8023B2B0(player);
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL && actor->mAlive && actor->mActorId == ACTOR_ARRW && actor->mLife > 0) {
            found = true;
            if (out != NULL) {
                out->x = actor->field_0x3c.x;
                out->y = actor->field_0x3c.y;
                out->z = actor->field_0x3c.z;
            }
            actor->fn_801F3668();
        }
    }
    return found;
}

// FIXME(shotgun): 96.22 %, scheduling+regalloc; behaviour clean.
bool fn_801CC730(s32 player, s32* outPlayer, JGeometry::TVec3f* outVel) {
    bool found = false;
    if (outPlayer != NULL) {
        *outPlayer = -1;
    }
    if (outVel != NULL) {
        outVel->zero();
    }
    s32 serial = fn_8023B2B0(player);
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL && actor->mAlive && actor->mActorId == ACTOR_ARRW && actor->mLife > 0) {
            found = true;
            if (outPlayer != NULL) {
                *outPlayer = serial;
            }
            if (outVel != NULL) {
                outVel->x = actor->field_0x3c.x;
                outVel->y = actor->field_0x3c.y;
                outVel->z = actor->field_0x3c.z;
            }
            actor->mLife = 0;
        }
    }
    return found;
}

// FIXME(shotgun): 96.22 %, scheduling+regalloc; behaviour clean.
bool fn_801CC640(s32 player, s32* outPlayer, JGeometry::TVec3f* outVel) {
    bool found = false;
    if (outPlayer != NULL) {
        *outPlayer = -1;
    }
    if (outVel != NULL) {
        outVel->zero();
    }
    s32 serial = fn_8023B358(player);
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL && actor->mAlive && actor->mActorId == ACTOR_PCDM && actor->mLife > 0) {
            found = true;
            if (outPlayer != NULL) {
                *outPlayer = serial;
            }
            if (outVel != NULL) {
                outVel->x = actor->field_0x3c.x;
                outVel->y = actor->field_0x3c.y;
                outVel->z = actor->field_0x3c.z;
            }
            actor->mLife = 0;
        }
    }
    return found;
}

// FIXME(shotgun): 77.73 %, missing/extra + reloc-name; behaviour clean.
bool fn_801CC538(s32 player, const JGeometry::TVec2f* pos, JGeometry::TVec3f* out, s32* outPlayer) {
    bool found = false;
    if (outPlayer != NULL) {
        *outPlayer = -1;
    }
    s32 serial = fn_8023B22C(player);
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL && actor->mAlive && actor->mActorId == ACTOR_BOME) {
            found = true;
            if (outPlayer != NULL) {
                *outPlayer = serial;
            }
            ((cls_804A0738*)actor)->field_0x254 = 1;
            if (out != NULL) {
                JGeometry::TVec3f v(pos->x, pos->y, 0.0f);
                JGeometry::TVec3f d = actor->field_0x3c * 3.0f;
                out->sub(v, d);
            }
        }
    }
    return found;
}

// Reflects the xy of *vel off *normal's xy (z is kept), at the original speed; straight back when
// either is zero or the reflection is (nearly) zero.
void fn_801CC350(const JGeometry::TVec3f* normal, JGeometry::TVec3f* vel) {
    f32 z = vel->z;
    JGeometry::TVec3f v(vel->x, vel->y, 0.0f);
    if (v.isZero()) {
        vel->set(-v.x, -v.y, z);
        return;
    }
    JGeometry::TVec3f n(normal->x, normal->y, 0.0f);
    if (n.isZero()) {
        vel->set(-v.x, -v.y, z);
        return;
    }
    n.normalize();
    JGeometry::TVec3f r;
    C_VECReflect(&v, &n, &r);
    if (__fabsf(r.x) < 0.05f && __fabsf(r.y) < 0.05f) {
        vel->set(-v.x, -v.y, z);
        return;
    }
    r.scale(PSVECMag(&v));
    vel->set(r.x, r.y, z);
}

// The distance between two angles as a fraction of a half turn (0-1).
static inline f32 angleDistance(s16 a, s16 b) {
    return (1.0f / 32768.0f) * __fabsf((s16)(a - b));
}

// How far box a sticks out of box b, as the move that puts it back (out); true if it does.
// FIXME(shotgun): 99.11 %, regalloc-only; behaviour clean.
bool fn_801CC29C(const JGeometry::TBox2f* a, const JGeometry::TBox2f* b, JGeometry::TVec2f* out) {
    bool outside = false;
    f32 d = a->i.x - b->i.x;
    out->x = 0.0f;
    out->y = 0.0f;
    if (d > 0.0f) {
        out->x += d;
        outside = true;
    }
    d = b->f.x - a->f.x;
    if (d > 0.0f) {
        out->x -= d;
        outside = true;
    }
    d = a->i.y - b->i.y;
    if (d > 0.0f) {
        out->y += d;
        outside = true;
    }
    d = b->f.y - a->f.y;
    if (d > 0.0f) {
        out->y -= d;
        outside = true;
    }
    return outside;
}

// tan / cot of a reduced angle for sliding along a box edge (0 when the cosine is ~0).
static inline f32 edgeSlope(s16 a) {
    f32 c = JMASCos(a);
    f32 t = 0.0f;
    if (__fabsf(c) > JGeometry::TUtilf::epsilon()) {
        t = (1.0f / c) * JMASSin(a);
    }
    return __fabsf(t);
}

static inline u16 cornerAngle(f32 dx, f32 dy) {
    return (s32)(JMath::atanTable_.atan2_(dx, dy) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
}

// Pushes a point inside box along `angle` out through the box edge the ray leaves by, then
// `speed` further along that direction. Writes the result to out; leaves the point alone (out
// = point) when it is outside the box.
// FIXME(shotgun): 64.79 %, missing/extra + reloc-name; behaviour drift.
void fn_801CBB98(const JGeometry::TBox2f* box, const JGeometry::TVec3f* p, s32 angleArg, f32 speed, JGeometry::TVec2f* out) {
    s16 angle = angleArg;
    out->x = p->x;
    out->y = p->y;
    bool inside = false;
    if (p->x >= box->i.x && p->y >= box->i.y && p->x < box->f.x && p->y < box->f.y) {
        inside = true;
    }
    if (!inside) {
        return;
    }
    u16 ua = angle;
    f32 sn = JMASSin(angle);
    f32 cs = JMASCos(angle);
    if (__fabsf(sn) <= JGeometry::TUtilf::epsilon()) {
        if (cs > 0.0f) {
            out->y = box->f.y + speed;
        } else {
            out->y = box->i.y - speed;
        }
        return;
    }
    if (__fabsf(cs) <= JGeometry::TUtilf::epsilon()) {
        if (sn > 0.0f) {
            out->x = box->f.x + speed;
        } else {
            out->x = box->i.x - speed;
        }
        return;
    }
    if (ua <= 0x4000) {
        u16 ca = cornerAngle(box->f.x - p->x, box->f.y - p->y);
        if (ua < ca) {
            out->x = p->x + __fabsf(box->f.y - p->y) * edgeSlope(angle);
            out->y = box->f.y;
        } else {
            out->x = box->f.x;
            out->y = p->y + __fabsf(box->f.x - p->x) * edgeSlope((s16)(0x4000 - angle));
        }
        out->x += speed * JMASSin(angle);
        out->y += speed * JMASCos(angle);
    } else if (ua >= 0x4000 && ua <= 0x8000) {
        s16 a2 = 0x8000 - ua;
        u16 ca = cornerAngle(box->f.x - p->x, box->i.y - p->y);
        if ((s32)(ca - ua) < 0) {
            out->x = p->x + __fabsf(p->y - box->i.y) * edgeSlope(a2);
            out->y = box->i.y;
        } else {
            out->x = box->f.x;
            out->y = p->y - __fabsf(box->f.x - p->x) * edgeSlope((s16)(0x4000 - a2));
        }
        out->x += speed * JMASSin(a2);
        out->y -= speed * JMASCos(a2);
    } else if (ua >= 0x8000 && ua <= 0xC000) {
        s16 a3 = ua - 0x8000;
        u16 ca = cornerAngle(box->i.x - p->x, box->i.y - p->y);
        if ((s32)(ua - ca) < 0) {
            out->x = p->x - __fabsf(p->y - box->i.y) * edgeSlope(a3);
            out->y = box->i.y;
        } else {
            out->x = box->i.x;
            out->y = p->y - __fabsf(p->x - box->i.x) * edgeSlope((s16)(0x4000 - a3));
        }
        out->x -= speed * JMASSin(a3);
        out->y -= speed * JMASCos(a3);
    } else {
        s16 a4 = 0x10000 - ua;
        u16 ca = cornerAngle(box->i.x - p->x, box->f.y - p->y);
        if ((s32)(ca - ua) < 0) {
            out->x = p->x - __fabsf(box->f.y - p->y) * edgeSlope(a4);
            out->y = box->f.y;
        } else {
            out->x = box->i.x;
            out->y = p->y + __fabsf(p->x - box->i.x) * edgeSlope((s16)(0x4000 - a4));
        }
        out->x -= speed * JMASSin(a4);
        out->y += speed * JMASCos(a4);
    }
}

// FIXME: 96.55 %, scheduling+regalloc; behaviour clean.
// Scheduling + regalloc: in both 45-degree turns the original computes the new y
// (fmadds) before the new x (fmsubs); ours the other way round, so the float registers differ.
// Steps pos (xy) away from center by speed when it is within radius: straight out, but turned to
// the nearer side of dir (a quarter turn off it) when that direction is more than a quarter turn
// from dir; on center itself, along dir (or (0, 1)) turned 45 degrees either way at random.
// False (pos unchanged) outside radius. Callers pass TVec3f positions (only x and y are read).
bool fn_801CB85C(const JGeometry::TVec2f* center, const JGeometry::TVec2f* dir, JGeometry::TVec2f* pos, f32 radius,
                 f32 speed) {
    bool moved = false;
    JGeometry::TVec2f d(pos->x - center->x, pos->y - center->y);
    f32 sq = d.squared();
    if (sq <= JGeometry::TUtilf::epsilon()) {
        moved = true;
        JGeometry::TVec2f v(dir->x, dir->y);
        if (v.squared() <= JGeometry::TUtilf::epsilon()) {
            v.set(0.0f, 1.0f);
        }
        if ((s32)(2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) == 0) {
            f32 s = JMASSin(-0x2000);
            f32 c = JMASCos(-0x2000);
            d.set(v.x * c - v.y * s, v.x * s + v.y * c);
        } else {
            f32 s = JMASSin(0x2000);
            f32 c = JMASCos(0x2000);
            d.set(v.x * c - v.y * s, v.x * s + v.y * c);
        }
    } else if (sq <= radius * radius) {
        moved = true;
        s16 dirAngle = JMath::atanTable_.atan2_(dir->x, dir->y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        s16 angle = JMath::atanTable_.atan2_(d.x, d.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        if (angleDistance(dirAngle, angle) > 0.5f) {
            s16 left = dirAngle + 0x4000;
            s16 right = dirAngle - 0x4000;
            if (angleDistance(angle, left) < angleDistance(angle, right)) {
                d.set(JMASSin(left), JMASCos(left));
            } else {
                d.set(JMASSin(right), JMASCos(right));
            }
        }
    }
    if (moved) {
        d.normalize();
        d.scale(speed);
        pos->add(d);
    }
    return moved;
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

// Makes the emitter immortal and then invalid (it stops creating particles and
// is deleted once they are gone). Does nothing for a NULL emitter.
void fn_801CB7A0(JPABaseEmitter* emitter) {
    if (emitter != NULL) {
        emitter->setStatus(JPAEMIT_Immortal);
        emitter->becomeInvalidEmitter();
    }
}

// Moves the emitter in *emitter to pos with setEmitterPos_8013CB44 (adjust true); or,
// when it is flagged (bit 0x8 of its word 0xF4) and has no particles left
// (0xD0 + 0xDC == 0), deletes it and sets *emitter to NULL. Does nothing for
// a NULL emitter.
void fn_801CB71C(JPABaseEmitter** emitter, const JGeometry::TVec3f* pos, s32 height, s32 depth) {
    if (*emitter != NULL) {
        if ((*emitter)->isEnableDeleteEmitter()) {
            JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
            manager->forceDeleteEmitter(*emitter);
            *emitter = NULL;
        } else {
            setEmitterPos_8013CB44(*emitter, pos, height, depth, true);
        }
    }
}

// FIXME: 86.49 %, scheduling+regalloc; behaviour clean.
// As fn_801CB4A4: saved registers are assigned in a different order.
// Two effects (spawnEmitter_8013CC50 0x24A and 0x24B) at pos on `layer`; arg3 is passed
// on and arg4 is passed on plus 1 (as the depth).
void fn_801CB688(s32 layer, const JGeometry::TVec3f* pos, s32 arg3, s32 arg4) {
    spawnEmitter_8013CC50(layer, pos, 0x24A, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x24B, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
}

// FIXME(shotgun): 88.94 %, scheduling+regalloc; behaviour clean.
void fn_801CB5CC(s32 layer, const JGeometry::TVec3f* pos, s32 arg5, s32 arg6) {
    spawnEmitter_8013CC50(layer, pos, 0xFF, arg5, arg6 + 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x100, arg5, arg6 + 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x101, arg5, arg6 + 1, NULL, NULL, true, 1.0f);
}

// FIXME: 86.49 %, scheduling+regalloc; behaviour clean.
// As fn_801CB4A4: saved registers are assigned in a different order.
// Two effects (spawnEmitter_8013CC50 0x117 and 0x118) at pos on `layer`; arg3 is passed
// on and arg4 is passed on plus 1 (as the depth).
void fn_801CB538(s32 layer, const JGeometry::TVec3f* pos, s32 arg3, s32 arg4) {
    spawnEmitter_8013CC50(layer, pos, 0x117, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x118, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
}

// FIXME: 86.49 %, scheduling+regalloc; behaviour clean.
// Same instructions; the target keeps layer, depth, height and pos in
// r31, r30, r29, r28 (ours: the reverse of the parameter order).
// Two effects (spawnEmitter_8013CC50 0x473 and 0x474) at pos on `layer`; arg3 and arg4 + 1
// are the height and depth of both.
void fn_801CB4A4(s32 layer, const JGeometry::TVec3f* pos, s32 arg3, s32 arg4) {
    spawnEmitter_8013CC50(layer, pos, 0x473, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(layer, pos, 0x474, arg3, arg4 + 1, NULL, NULL, true, 1.0f);
}

// Asks the actor manager for a 'RUPY' (fn_801F8544 with id ACTOR_RUPY) at pos on
// `layer`; the flag picks 5 or 4 in the low bits of its last argument.
s32 fn_801CB434(s32 layer, const JGeometry::TVec3f* pos, bool flag) {
    return getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, layer, pos, 0x261E00 | (flag ? 5 : 4));
}

// Looks up the TIMG resource `name` in the game archive and initialises
// texObj (and tlutObj for a palette texture, if given) from it. Returns the
// image's height / width.
// FIXME: 89.71 %, missing/extra + reloc-name; behaviour clean.
// Same instructions apart from scheduling: the target computes
// the mipmap flag straight into r10 and orders the GXInitTexObjLOD argument
// loads differently. JUTTexture::initTexObj's forms (? GX_TRUE : GX_FALSE,
// image +=) and isMIPmapEnabled() were worse.
f32 fn_801CB270(const char* name, GXTexObj* texObj, GXTlutObj* tlutObj) {
    ResTIMG* timg = (ResTIMG*)gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', name);
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
