#include "game/ActorBase.h"
#include "game/Effect.h"
#include "game/GameAudio.h"
#include "game/cls_802150B8.h"
#include "game/cls_804A64A8.h"

// .sdata2 floats: 6.0f (raises the effect's position) and 1.0f (the effect's
// scale). Arrays, so the names survive into the object.
static const f32 lbl_8053FBA0[1] = {6.0f};
static const f32 lbl_8053FBA4[1] = {1.0f};

cls_802150B8::cls_802150B8() : field_0x0(0), field_0x18(0), field_0x30(-1), mOwner(0) {}

void cls_802150B8::fn_802144F0() {
    mOwner->fn_801F0E34(0x400C3, 0);
    JGeometry::TVec3f pos = mOwner->mPos;
    pos.y += lbl_8053FBA0[0];
    fn_8013CC50(mOwner->field_0x4, &pos, 0x3CF, 0, 0, NULL, NULL, true, lbl_8053FBA4[0]);
}

cls_804A64A8::~cls_804A64A8() {}

void cls_804A64A8::execute(JPABaseEmitter*, JPABaseParticle* particle) {
    if (field_0x10 != 0) {
        return;
    }
    // JPABaseParticle is incomplete in game units (JPAParticle.h needs <cmath>).
    // The stored position starts at 0x18, across mOffsetPosition.z and
    // mLocalPosition, so it is written by offset. Component stores, not a
    // struct copy: the original does lfs/stfs per component.
    JGeometry::TVec3f* pos = reinterpret_cast<JGeometry::TVec3f*>(reinterpret_cast<u8*>(particle) + 0x18);
    pos->x = field_0x4.x;
    pos->y = field_0x4.y;
    pos->z = field_0x4.z;
}
