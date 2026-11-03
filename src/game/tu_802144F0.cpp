#include "game/GameAudio.h"
#include "game/cls_804A64A8.h"

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
