#include "game/GameAudio.h"
#include "game/act/act_vaati_boss.h"
#include "game/Player.h"

// Vaati's per-frame player check: when the boss is in one of its states and
// a player is in its box, the attack states answer with a sound or effect.
bool VaatiBoss_804A4958::fn_801E4348() {
    bool result = false;
    bool stateOk = true;
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
        stateOk = false;
        break;
    }
    if (stateOk && (mFlags & 0x2000)) {
        JGeometry::TBox2f box;
        fn_801F317C(&box);
        JGeometry::TVec2f xy;
        s32 player = fn_8023AE34(&box, field_0x4, &xy);
        if (player >= 0) {
            switch (mState) {
            case 0xc:
            case 0xd:
            case 0x13:
                if (fn_801CC730(player, NULL, NULL)) {
                    result = true;
                }
                break;
            default:
                if (fn_801CC820(player, NULL)) {
                    JGeometry::TVec3f pos(xy.x, xy.y + mPos.z, mPos.z);
                    fn_801CB688(field_0x4, &pos, 1, 1);
                    startSound_801F0E34(0x5003C, 0);
                    switch (mState) {
                    case 0xa:
                    case 0xe:
                    case 0xf:
                    case 0x10:
                    case 0x11:
                    case 0x12:
                    case 0x14:
                        fn_801D5D84(0xb);
                        break;
                    }
                    result = true;
                }
                break;
            }
        }
    }
    return result;
}
