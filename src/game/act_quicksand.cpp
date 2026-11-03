#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/act/act_quicksand.h"

// FIXME: 91.1 %. JMAFastSqrt, normalize (isZero, TUtilf::inv_sqrt) and
// scale match. The original picks the length with one fsel
// (push - field_0x3a4 >= 0) where ours compares and branches (an explicit
// `push - field_0x3a4 >= 0.0f` gave 88.9 %).
// The push-out vector for a position in the quicksand: towards the middle
// (in x and y), as long as the distance less 2 but at most field_0x3a4; zero
// outside the area or above the ground (pos->z > 0).
void fn_80210E90(const ActorBase_804A5770* actor, const JGeometry::TVec3f* pos, JGeometry::TVec3f* out) {
    const cls_804A6270* sand = static_cast<const cls_804A6270*>(actor);
    out->set(0.0f, 0.0f, 0.0f);
    if (pos->z <= 0.0f && sand->mState != 0) {
        f32 radius = 0.9f * (16.0f * sand->field_0x54.x);
        if (radius > 2.0f) {
            f32 dist = JMAFastSqrt(square(pos->x - sand->mPos.x) + square(pos->y - sand->mPos.y));
            if (2.0f < dist && dist <= radius) {
                out->sub(sand->mPos, *pos);
                out->z = 0.0f;
                out->normalize();
                f32 push = dist - 2.0f;
                out->scale(push >= sand->field_0x3a4 ? sand->field_0x3a4 : push);
            }
        }
    }
}
