#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/EnemyBase.h"
#include "game/cls_802150B8.h"
#include "game/cls_804A64A8.h"
#include "game/act/act_shadow_link_2.h"

void cls_802150B8::fn_802144F0() {
    mOwner->fn_801F0E34(0x400C3, 0);
    JGeometry::TVec3f pos = mOwner->mPos;
    pos.y += 6.0f;
    fn_8013CC50(mOwner->field_0x4, &pos, 0x3CF, 0, 0, NULL, NULL, true, 1.0f);
}

void cls_802150B8::fn_80214578() {
    JGeometry::TVec3f pos = mOwner->mPos;
    switch (mOwner->field_0x98) {
    case 0:
        pos.x -= 24.0f;
        break;
    case 1:
        pos.x += 24.0f;
        break;
    case 2:
        pos.x += 2.0f;
        pos.y -= 26.0f;
        break;
    case 3:
        pos.x -= 4.0f;
        pos.y += 18.0f;
        break;
    }
    fn_8013CC50(mOwner->field_0x4, &pos, 0xCB, 0, 1, NULL, NULL, true, 1.0f);
    fn_8013CC50(mOwner->field_0x4, &pos, 0xCC, 0, 1, NULL, NULL, true, 1.0f);
}

void cls_802150B8::fn_802146A4() const {
    ShadowLink2_804A43E8* owner = static_cast<ShadowLink2_804A43E8*>(mOwner);
    s32 flag = 0;
    if (owner->field_0x524 & 0x400) {
        flag = 1;
    }
    fn_8013CC50(owner->field_0x4, &owner->mPos, 0x416, 0, flag - 1, NULL, NULL, true, 1.0f);
    fn_8013CC50(mOwner->getField_0x4(), mOwner->getPos(), 0x417, 0, flag, NULL, NULL, true, 1.0f);
}

void cls_802150B8::fn_80214900() {
    s32 serial = field_0x30;
    if (serial >= 0) {
        ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x30 = -1;
    }
    deleteEmitter(&field_0x18);
    static_cast<EnemyBase_804A4808*>(mOwner)->fn_801CDC8C(0x4000E);
}

cls_802150B8::cls_802150B8() : field_0x0(0), field_0x18(0), field_0x30(-1), mOwner(0) {}

cls_804A64A8::~cls_804A64A8() {}

void cls_804A64A8::execute(JPABaseEmitter*, JPABaseParticle* particle) {
    if (field_0x10 != 0) {
        return;
    }
    // The particle's position is written by offset (0x18), not through a
    // JPABaseParticle member. Component stores, not a struct copy: the
    // original does lfs/stfs per component.
    JGeometry::TVec3f* pos = reinterpret_cast<JGeometry::TVec3f*>(reinterpret_cast<u8*>(particle) + 0x18);
    pos->x = field_0x4.x;
    pos->y = field_0x4.y;
    pos->z = field_0x4.z;
}
