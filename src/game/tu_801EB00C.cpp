#include "game/GameAudio.h"
#include "game/cls_801EB8CC.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/EnemyBase.h"
#include "game/EmitterDelete.h"
#include "game/GameMath.h"
#include "JSystem/JMath/JMath.h"

cls_801EBA14::cls_801EBA14() {
    field_0x0 = 1;
    field_0x4 = 1;
    field_0x8 = 90.0f;
    field_0xc = 70.0f;
    field_0x10 = 20.0f;
    field_0x14 = 0.05f;
    field_0x18 = 0.7f;
    field_0x1c = 0.9f;
    field_0x20 = 0.7f;
    field_0x24 = 0x600;
}

// Constructor (fn_801EB950, now __ct__12cls_801EB8CCFv): zeroes the vector,
// builds field_0xc (its constructor inlined), clears the rest, takes the timer
// from fn_801CD664(0x10000) and then clears the seven emitter pointers.
cls_801EB8CC::cls_801EB8CC()
    : field_0x0(0.0f, 0.0f, 0.0f), field_0x50(NULL), field_0x54(0.0f), field_0x58(0.0f), field_0x5c(0),
      field_0x5e(fn_801CD664(0x10000)) {
    for (s32 i = 0; i < 7; i++) {
        field_0x34[i] = NULL;
    }
}

// The deleting form (__dl__FPv when asked) is the compiler's; the body is just the
// cleanup of the eight emitter pointers.
cls_801EB8CC::~cls_801EB8CC() {
    for (s32 i = 0; i < 7; i++) {
        releaseEmitter_801EE434(field_0x34[i]);
    }
    releaseEmitter_801EE434(field_0x50);
}

// fn_801EB648: the flag at field_0x54 (truncated to a byte) gates the scan of the
// seven emitter pointers.
bool cls_801EB8CC::fn_801EB648() const {
    bool found = false;
    if ((u8)field_0x54 != 0) {
        for (s32 i = 0; i < 7; i++) {
            if (field_0x34[i] != NULL) {
                found = true;
                break;
            }
        }
    }
    return found;
}

// One emitter of fn_801EB418's ring: moved to *pos with the given alpha, or
// deleted once the ring has faded out (both of its uses inline it).
static inline void updateEmitter(JPABaseEmitter** emitter, const JGeometry::TVec3f* pos,
                                 const cls_801EBA14& param, bool remove, u8 alpha) {
    if (*emitter != NULL) {
        if (remove) {
            JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
            manager->forceDeleteEmitter(*emitter);
            *emitter = NULL;
        } else {
            setEmitterPos_8013CB44(*emitter, pos, param.field_0x0, param.field_0x4, true);
            (*emitter)->setGlobalAlpha(alpha);
        }
    }
}

// fn_801EB418: follows *pos with the ring of seven emitters (radius 14, turning
// 0x16A a frame, 0x1F70 apart) and the one at 0x50 on the first spot; field_0x54
// eases towards field_0x58 and is the emitters' alpha. Once it reaches 0 with
// field_0x5c set, the emitters are deleted.
// FIXME: 89.25. missing/extra (4 fewer): the original turns the u8 fade-out flag into
// the bool with neg / or / srwi (ours: clrlwi) and keeps the alpha's clrlwi; its
// loop-invariant loads are ordered differently (registers follow).
void cls_801EB8CC::fn_801EB418(const JGeometry::TVec3f* pos) {
    field_0x0.set(*pos);
    field_0x5e += 0x16A;
    if (field_0x54 != field_0x58) {
        f32 step = 2.5f;
        if (field_0x54 > field_0x58) {
            step = 5.0f;
        }
        fn_801CD950(&field_0x54, field_0x58, step);
    }
    u8 done = false;
    if (field_0x5c != 0 && 0.0f == field_0x54) {
        done = true;
    }
    bool remove = done;
    u8 alpha = field_0x54;
    JGeometry::TVec2f offset(0.0f, 14.0f);
    for (s32 i = 0; i < 7; i++) {
        s16 angle = -(s16)(field_0x5e + i * 0x1F70);
        f32 s = JMASSin(angle);
        f32 c = JMASCos(angle);
        JGeometry::TVec3f spot;
        spot.z = pos->z;
        spot.x = pos->x + (offset.x * c - offset.y * s);
        spot.y = pos->y + (offset.x * s + offset.y * c);
        updateEmitter(&field_0x34[i], &spot, field_0xc, remove, alpha);
        if (i == 0) {
            updateEmitter(&field_0x50, &spot, field_0xc, remove, alpha);
        }
    }
}

// fn_801EB1F4: inside field_0xc.field_0x10 of field_0x0 (in x / y, and not
// above field_0xc.field_0x8), the step in *out that turns *pos by
// -field_0xc.field_0x24 about field_0x0 and eases its distance towards
// field_0xc.field_0x10 (fn_801CD950, step field_0xc.field_0x1c); z is set to
// field_0xc.field_0x20. Else *out is zero.
// FIXME: 98.21. regalloc-only (f3/f4, f1/f3, f5/f6, f4/f7 swapped).
void cls_801EB8CC::fn_801EB1F4(const JGeometry::TVec3f* pos, JGeometry::TVec3f* out) const {
    out->set(0.0f, 0.0f, 0.0f);
    if (pos->z <= field_0xc.field_0x8) {
        f32 dx = pos->x - field_0x0.x;
        f32 dy = pos->y - field_0x0.y;
        if (square(dx) + square(dy) <= square(field_0xc.field_0x10)) {
            s16 angle = -field_0xc.field_0x24;
            f32 s = JMASSin(angle);
            f32 c = JMASCos(angle);
            JGeometry::TVec2f v(dx * c - dy * s, dx * s + dy * c);
            if (v.squared() <= JGeometry::TUtilf::epsilon()) {
                v.set(0.0f, 0.1f);
            } else {
                f32 dist = v.length();
                fn_801CD950(&dist, field_0xc.field_0x10, field_0xc.field_0x1c);
                v.normalize();
                v.scale(dist);
            }
            JGeometry::TVec3f target;
            target.x = v.x + field_0x0.x;
            target.y = v.y + field_0x0.y;
            target.z = pos->z;
            out->sub(target, *pos);
            out->z = field_0xc.field_0x20;
        }
    }
}

// fn_801EB00C: the push for a point at *pos: towards field_0x0 in x / y (up to
// field_0xc.field_0x8 high) inside field_0xc.field_0xc, faster nearer
// field_0xc.field_0x10 (field_0xc.field_0x14 to field_0xc.field_0x18, never past
// field_0x0), plus fn_801EB1F4's step.
// FIXME: 97.42. missing/extra: one frsp after the fsel (the intrinsic returns a
// double; the original scales by the fsel result directly).
void cls_801EB8CC::fn_801EB00C(const JGeometry::TVec3f* pos, JGeometry::TVec3f* out) const {
    JGeometry::TVec3f push(0.0f, 0.0f, 0.0f);
    if (pos->z <= field_0xc.field_0x8) {
        f32 dx = pos->x - field_0x0.x;
        f32 dy = pos->y - field_0x0.y;
        f32 dist = JMAFastSqrt(square(dx) + square(dy));
        if (dist <= field_0xc.field_0xc) {
            f32 t = 0.0f;
            if (field_0xc.field_0xc != field_0xc.field_0x10) {
                t = (dist - field_0xc.field_0xc) * (1.0f / (field_0xc.field_0x10 - field_0xc.field_0xc));
            }
            if (t > 1.0f) {
                t = 1.0f;
            } else if (t < 0.0f) {
                t = 0.0f;
            }
            f32 speed = t * (field_0xc.field_0x18 - field_0xc.field_0x14) + field_0xc.field_0x14;
            push.sub(field_0x0, *pos);
            push.z = 0.0f;
            push.normalize();
            // Never past field_0x0: the MWCC fsel intrinsic, as the original's fsel shows.
            f32 len = __fsel(dist - speed, speed, dist);
            push.scale(len);
        }
    }
    JGeometry::TVec3f step;
    fn_801EB1F4(pos, &step);
    out->set(push);
    *out += step;
}
