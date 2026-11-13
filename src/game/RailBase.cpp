#include "game/GameAudio.h"
#include "game/RailBase.h"
#include "game/ActorManager.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

// FIXME(shotgun): 97.18 %, scheduling+regalloc; behaviour clean.
// 0x800FE3EC
bool cls_800FE3EC::fn_800FE3EC(u8 group, JGeometry::TVec3f pos, int layer) {
    field_0x0 = group;
    clear();
    for (u32 i = 0; i < 0x31; i++) {
        field_0x4[i] = NULL;
    }
    ActorBase_804A5770* found[0x31];
    s32 count = 0;
    bool ok = getActorManager_801F856C()->fn_801F71E8(field_0x0, found, &count, 0x31);
    field_0x2 = count;
    if (!ok || field_0x2 == 0) {
        return false;
    }
    for (s32 i = 0; i < field_0x2; i++) {
        u8 index = (found[i]->field_0x90 >> 16) & 0xFF;
        if (index >= field_0x2 || field_0x4[index] != NULL) {
            return false;
        }
        field_0x4[index] = found[i];
    }
    for (s32 i = 0; i < field_0x2; i++) {
        if (field_0x4[i] == NULL) {
            return false;
        }
    }
    JGeometry::TVec3f target = pos;
    f32 best = 10000.0f;
    for (u16 i = 0; i < field_0x2; i++) {
        if (layer == fn_800FE25C(i)) {
            JGeometry::TVec3f diff = field_0x4[i]->mPos;
            diff.sub(target);
            f32 dist = diff.length();
            if (dist < best) {
                best = dist;
                field_0x3 = i;
            }
        }
    }
    return true;
}

// FIXME(shotgun): 90.48 %, missing/extra; behaviour near.
// 0x800FE2E4
u8 cls_800FE3EC::fn_800FE2E4(const JGeometry::TVec3f* pos, s32 layer) {
    f32 best = 10000.0f;
    for (u16 i = 0; i < field_0x2; i++) {
        if (layer == fn_800FE25C(i)) {
            JGeometry::TVec3f diff;
            diff.sub(field_0x4[i]->mPos, *pos);
            f32 dist = diff.length();
            if (dist < best) {
                best = dist;
                field_0x3 = i;
            }
        }
    }
    return field_0x3;
}

void fn_800FE294(JGeometry::TVec3f* out, cls_800FE3EC* buffer, u8 index) {
    if (index == 0xFF) {
        *out = buffer->field_0x4[buffer->field_0x3]->mPos;
    } else {
        *out = buffer->field_0x4[index]->mPos;
    }
}

s32 cls_800FE3EC::fn_800FE25C(u8 index) {
    if (index == 0xFF) {
        return field_0x4[field_0x3]->field_0x4;
    }
    return field_0x4[index]->field_0x4;
}

u8 cls_800FE3EC::fn_800FE21C(u8 index) {
    if (index == 0xFF) {
        return field_0x4[field_0x3]->field_0x90 >> 24;
    }
    return field_0x4[index]->field_0x90 >> 24;
}

// FIXME(shotgun): 88.9 %, missing/extra + reloc-name (the original has one shared tail for the *out copy)
// 0x800FE090
// auto_inline off: the original's fn_800FDEA0 (bl at 0x800FDEFC) calls it out of line.
#pragma auto_inline off
// FIXME: 90.29 %, missing/extra; behaviour near.
s32 cls_800FE3EC::fn_800FE090(const JGeometry::TVec3f* pos, JGeometry::TVec3f* out) {
    s32 result = 0;
    f32 dx = field_0x4[field_0x3]->mPos.x - pos->x;
    f32 dy = field_0x4[field_0x3]->mPos.y - pos->y;
    if (JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy) < 10.0f) {
        u8 cur = field_0x3;
        u32 mode = field_0x4[cur]->field_0x90 & 3;
        result = 2;
        bool advance = true;
        if (cur == 0) {
            result = 1;
            if ((field_0x4[field_0x2 - 1]->field_0x90 & 3) == 2 && field_0x1 == 1) {
                field_0x1 = 0;
            }
        } else if (cur == field_0x2 - 1) {
            result = 3;
            if (mode == 2) {
                field_0x1 ^= 1;
            } else if (mode != 1) {
                advance = false;
            }
        }
        if (advance) {
            if (field_0x1 == 0) {
                field_0x3++;
            } else {
                field_0x3--;
            }
            if (field_0x3 == field_0x2) {
                field_0x3 = 0;
            }
            if (field_0x3 == 0xFF) {
                field_0x3 = field_0x2 - 1;
            }
        }
    }
    const JGeometry::TVec3f& p = field_0x4[field_0x3]->mPos;
    out->x = p.x;
    out->y = p.y;
    out->z = p.z;
    return result;
}
#pragma auto_inline reset

// FIXME(shotgun): 87.20 %, missing/extra; behaviour near.
// 0x800FDEA0
s32 cls_800FE3EC::fn_800FDEA0(JGeometry::TVec3f* pos, f32 speed, JGeometry::TVec3f* outStep, s32* layer,
                              u16* outAngle, s32 arg) {
    JGeometry::TVec3f target;
    JGeometry::TVec3f here = *pos;
    s32 result = fn_800FE090(&here, &target);
    f32 dx = target.x - pos->x;
    f32 dy = target.y - pos->y;
    s32 angle = (s32)(JMath::atanTable_.atan2_(dx, dy) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
    if (layer != NULL && *layer != field_0x4[field_0x3]->field_0x4) {
        *layer = field_0x4[field_0x3]->field_0x4;
        outStep->x = dx;
        outStep->y = dy;
        return 4;
    }
    if (outStep->x == 0.0f && outStep->y == 0.0f && !((field_0x4[field_0x3]->field_0x90 >> 2) & 1)) {
        // keep the angle to the point
    } else {
        s32 current = (s32)(JMath::atanTable_.atan2_(outStep->x, outStep->y) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
        s16 diff = angle - current;
        if (diff >= 0) {
            if (diff > (s16)arg) {
                angle = current + arg;
            }
        } else if (diff < -(s16)arg) {
            angle = current - arg;
        }
    }
    outStep->x = speed * JMASinShort(angle);
    outStep->y = speed * JMACosShort(angle);
    if (outAngle != NULL) {
        *outAngle = angle;
    }
    return result;
}

void cls_800FE3EC::fn_800FDE38() {
    field_0x1 ^= 1;
    if (field_0x1 == 0) {
        field_0x3++;
    } else {
        field_0x3--;
    }
    if (field_0x3 == 0xFF) {
        field_0x3 = field_0x2 - 1;
        return;
    }
    if (field_0x3 == field_0x2) {
        field_0x3 = 0;
    }
}

// FIXME(shotgun): 95.97 %, missing/extra; behaviour clean.
// 0x800FDC70
void fn_800FDC70(JGeometry::TVec3f* out, cls_800FE3EC* rail, JGeometry::TVec3f pos, s32 arg4, u8 index) {
    JGeometry::TVec3f target;
    if (index == 0xFF) {
        target = rail->field_0x4[rail->field_0x3]->mPos;
    } else {
        target = rail->field_0x4[index]->mPos;
    }
    JGeometry::TVec3f diff;
    diff.sub(target, pos);
    f32 len = diff.length();
    if (arg4 == 0) {
        *out = diff;
        return;
    }
    diff.normalize();
    diff.scale(len / (f32)arg4);
    *out = diff;
}

// FIXME(shotgun): 80.59 %, missing/extra; behaviour clean.
// (the original copies the target point through a stack temp and keeps the sum/difference in registers)
// 0x800FD998
JGeometry::TVec3f cls_800FE3EC::fn_800FD998(JGeometry::TVec3f pos, s32 step, s32 steps, u8 index) {
    JGeometry::TVec3f target;
    if (index == 0xFF) {
        target = field_0x4[field_0x3]->mPos;
    } else {
        target = field_0x4[index]->mPos;
    }
    JGeometry::TVec3f mid;
    mid.add(pos, target);
    mid.scale(0.5f);
    JGeometry::TVec3f diff;
    diff.sub(target, pos);
    f32 radius = 0.5f * diff.length();
    if (steps == 0) {
        return diff;
    }
    if (step == 0) {
        diff.set(0.0f, 0.0f, 0.0f);
        return diff;
    }
    u16 angle = 0x8000 / steps;
    JGeometry::TVec3f dir;
    dir.sub(pos, mid);
    dir.normalize();
    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    f32 cosNow = radius * sincos.cosShort(step * angle);
    f32 cosPrev = radius * sincos.cosShort((step - 1) * angle);
    JGeometry::TVec3f now = dir;
    now.scale(cosNow);
    JGeometry::TVec3f prev = dir;
    prev.scale(cosPrev);
    JGeometry::TVec3f result;
    result.sub(now, prev);
    return result;
}

// FIXME(shotgun): 99.85 %, reloc-name; behaviour clean.
// 0x800FD894
u8 cls_800FE3EC::fn_800FD894(s32 layer, u8* out, s32 max) {
    if (layer == -1) {
        if (out != NULL) {
            for (u8 i = 0; i < field_0x2; i++) {
                out[i] = i;
            }
        }
        return field_0x2;
    }
    u8 n = 0;
    for (u8 i = 0; i < field_0x2; i++) {
        if (layer == fn_800FE25C(i)) {
            if (max == n) {
                OSPanic(__FILE__, 0x1A3, "RailBase: Buffsize Error");
            }
            out[n++] = i;
        }
    }
    return n;
}

// FIXME(shotgun): 85.32 %, missing/extra; behaviour drift.
// 0x800FD818
void cls_800FE3EC::fn_800FD818(s32 dir) {
    u8 count = field_0x2;
    s16 pos = field_0x3 + (s8)dir;
    u32 mode = field_0x4[count - 1]->field_0x90 & 3;
    if (pos < 0) {
        if (mode == 1) {
            field_0x3 = count + pos;
        } else {
            field_0x3 = 0;
        }
        return;
    }
    if (pos >= count) {
        if (mode == 1) {
            field_0x3 = pos - count;
        } else {
            field_0x3 = count - 1;
        }
        return;
    }
    field_0x3 = pos;
}

// FIXME(shotgun): 88.68 %, missing/extra; behaviour drift.
// 0x800FD5E8
bool cls_800FE3EC::fn_800FD5E8(u8 index) {
    u8 count = field_0x2;
    u32 mode = field_0x4[count - 1]->field_0x90 & 3;
    if (index == 0xFF) {
        u8 cur = field_0x3;
        s16 next = field_0x1 != 0 ? cur - 1 : cur + 1;
        if (next < 0) {
            if (mode != 1) {
                return false;
            }
            next = count - 1;
        } else if (next == count) {
            if (mode != 1) {
                return false;
            }
            next = 0;
        }
        if (fn_800FE25C(cur) != fn_800FE25C(next)) {
            return true;
        }
        return false;
    }
    int next = index + 1;
    int prev = index - 1;
    if (next == count) {
        if (mode == 1) {
            next = 0;
        } else {
            next = next - 1;
        }
    } else if (prev < 0) {
        if (mode == 1) {
            prev = count - 1;
        } else {
            prev = 0;
        }
    }
    if (fn_800FE25C(index) != fn_800FE25C(next)) {
        return true;
    }
    if (fn_800FE25C(index) != fn_800FE25C(prev)) {
        return true;
    }
    return false;
}

bool cls_800FE3EC::fn_800FD594(u8 index) {
    // Both arms feed one shared test; the original merges them the same way.
    u32 flag;
    if (index == 0xFF) {
        flag = field_0x4[field_0x3]->field_0x90 & 0x4;
    } else {
        flag = field_0x4[index]->field_0x90 & 0x4;
    }
    if (flag != 0) {
        return false;
    }
    return true;
}

bool cls_800FE3EC::fn_800FD548(u8 index) {
    if (index == 0xFF) {
        index = field_0x3;
    }
    if (field_0x1 != 0) {
        return (field_0x4[index]->field_0x90 & 0x8) != 0;
    }
    return (field_0x4[index]->field_0x90 & 0x10) != 0;
}

// 0x800FD4C0
bool cls_800FE3EC::fn_800FD4C0(u8 a, u8 b) {
    u8 first = 0;
    u8 second = 0;
    u32 flags = field_0x4[a]->field_0x90;
    if (flags & 0x8) {
        first = 1;
    } else if (flags & 0x10) {
        second = 1;
    }
    flags = field_0x4[b]->field_0x90;
    if (flags & 0x8) {
        first = 1;
    } else if (flags & 0x10) {
        second = 1;
    }
    if (first == 1 && second == 1) {
        return true;
    }
    return false;
}

void cls_8049C770::init() {
    mPos.x += 8.0f;
    mPos.y += 8.0f;
}

void cls_8049C770::update() {}

void cls_8049C770::draw() {}

cls_8049C770::~cls_8049C770() {}
