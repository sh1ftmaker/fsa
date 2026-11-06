#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801F9484.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Sets the map's field_0x216 flag (gGameRoot2_8053AB10->0x20, TileMap_804AA858; 9 units
// set it to 1). Takes no arguments; the caller is the fn_8017C4C8 branch of
// the actor update.
void fn_8017ED08() {
    gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
}

// Out-of-line copy of JUtility::TColor::set(r, g, b, a).
void fn_8017ECF4(JUtility::TColor* color, u8 r, u8 g, u8 b, u8 a) {
    color->set(r, g, b, a);
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8017ECE8(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// FIXME: 89. The original stores the result's own register (1) to field_0x178; ours loads
// the constant twice (li r0 and li r4).
// Sets the camera's field_0x178 flag unless field_0x179 is set.
u8 fn_8017ECC0() {
    s32 result = 0;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x179 == 0) {
        result = 1;
        camera->field_0x178 = 1;
    }
    return result;
}

// FIXME: 93.55. The original stores the box corners in the order f.x, i.x, f.y, i.y (ours
// i.x, i.y, f.x, f.y) and keeps the constant 1 of field_0x840 in the result's register.
// Fixes the camera on pos (both corners of its box) with `value`, once.
u8 fn_8017EC70(const JGeometry::TVec3f* pos, f32 value) {
    s32 result = 0;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x840 == 0) {
        result = 1;
        camera->field_0x840 = 1;
        camera->field_0x844.set(pos->x, pos->y, pos->x, pos->y);
        camera->field_0x854 = value;
        camera->field_0x1d4 = -1.0f;
        camera->field_0x1d8 = -1.0f;
    }
    return result;
}

// Out-of-line copy of TBox2<f32>::set(x0, y0, x1, y1).
void fn_8017EC5C(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1) {
    box->set(x0, y0, x1, y1);
}

// Out-of-line copy of TBox2<f32>'s copy (word by word).
void fn_8017EC38(JGeometry::TBox2f* dst, const JGeometry::TBox2f* src) {
    *dst = *src;
}

// Whether both corners of box are on screen. corner2 is declared before corner and
// camera before result only to steer the stack slots and register order to the original's.
bool fn_8017EB94(const JGeometry::TBox2f* box) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    bool result = false;
    JGeometry::TVec3f corner2;
    JGeometry::TVec3f corner(box->i.x, box->i.y, 0.0f);
    if (camera->fn_8030C2A8(&corner)) {
        corner2.set(box->f.x, box->f.y, 0.0f);
        if (camera->fn_8030C2A8(&corner2)) {
            result = true;
        }
    }
    return result;
}

// FIXME: 83.75. The original keeps the constant 1 (stb 0x7f4) in r4 and copies it to the result
// (mr r5, r4), and stores the 0 for field_0x7f6 from the result's zero register; ours loads
// separate constants.
// Starts a scripted camera move (mode 4) to pos unless one is already set.
u8 fn_8017EB44(const JGeometry::TVec3f* pos) {
    s32 result = 0;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 4;
        camera->field_0x7fc.set(*pos);
        camera->field_0x7f6 = 0;
        result = 1;
    }
    return result;
}

// Sets the sub-object's field_0x284 flag and its field_0x28c value, once.
bool fn_8017EB1C(f32 value) {
    cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
    if (sub->field_0x284 == 0) {
        sub->field_0x284 = 1;
        sub->field_0x28c = value;
    }
    return false;
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_8017EB0C(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

void ActorBase_804A5770::fn_8017EAF8(const JGeometry::TVec3f& v) {
    field_0x3c = v;
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8017EADC(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

// Out-of-line copy of JPABaseEmitter::setGlobalEnvColor(r, g, b).
void fn_8017EACC(JPABaseEmitter* emitter, u8 r, u8 g, u8 b) {
    emitter->setGlobalEnvColor(r, g, b);
}

// Out-of-line copy of JPABaseEmitter::becomeInvalidEmitter().
void fn_8017EAB4(JPABaseEmitter* emitter) {
    emitter->becomeInvalidEmitter();
}

// The 'FGAN' actor's (cls_804A1318, vtable 0x804A1318) vfunc_0x24.
void cls_804A1318::vfunc_0x24() {
    startSound_801F0E34(0x40047, 0);
    vfunc_0x28(field_0x2ba, 8.0f, -16.0f);
}
