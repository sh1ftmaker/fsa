#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"

void ActorBase_804A5770::fn_8017EAF8(const JGeometry::TVec3f& v) {
    field_0x3c = v;
}

// Sets the map's field_0x216 flag (lbl_8053AB10->0x20, cls_804AA858; 9 units
// set it to 1). Takes no arguments; the caller is the fn_8017C4C8 branch of
// the actor update.
void fn_8017ED08() {
    lbl_8053AB10->field_0x20->field_0x216 = 1;
}

