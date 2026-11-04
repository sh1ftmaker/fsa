#include "game/GameAudio.h"
#include "game/Map.h"
#include "game/act/act_obj_lift.h"

OBJLift_804ACA00::~OBJLift_804ACA00() {}

bool OBJLift_804ACA00::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 code = fn_8022461C(pos, layer);
    if (code == 0x3A || code == 0x3 || code == 0xD || code == 0xE || code == 0x3B) {
        return false;
    }
    return true;
}
