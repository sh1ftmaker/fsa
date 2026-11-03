#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/act/act_vaati_boss.h"

// Clears mAlive of each actor named by a serial in the 16-entry array (serials
// below 0 are empty slots).
extern "C" void fn_801D7638(const s32* serials) {
    const s32* p = serials;
    s32 serial;
    for (s32 i = 0; i < 0x10; i++, p++) {
        serial = *p;
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
            }
        }
    }
}

void cls_801D6A58::fn_801D7874() {
    field_0x0 = -1;
    field_0x4 = 0;
    field_0x5 = 0;
    field_0x8 = 0.0f;
    field_0xc = 0.0f;
    field_0x10 = 0.0f;
}

void cls_801D6A88::fn_801D789C() {
    field_0x0 = 0;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xc.fn_8021BD18(2);
}
