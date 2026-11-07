#include "game/GameAudio.h"
#include "game/act/act_door.h"

// FIXME(shotgun): not written (size 0x22E0): update (0x802C59D8)

// FIXME(shotgun): not written (size 0x614): init (0x802C7CC8)

void Door_804A8D88::fn_802C59B0(const JGeometry::TVec3f* delta) {
    field_0x258 += delta->x;
    field_0x25c += delta->y;
}

// Plain TVec3 setter that update calls everywhere.
void fn_802C7CB8(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

void Door_804A8D88::draw() {}

Door_804A8D88::~Door_804A8D88() {}
