#include "game/GameAudio.h"
#include "game/act/act_seesaw_lift.h"

SeesawLift_804AD348::~SeesawLift_804AD348() {}

// The state table in the unit's .data (0x804AD30C). It is zeros in the DOL and
// filled at runtime, so it is declared here and not defined.
extern void (SeesawLift_804AD348::*lbl_804AD30C[5])();

void SeesawLift_804AD348::update() {
    (this->*lbl_804AD30C[(field_0x90 >> 7) & 0x1F])();
}
