#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "JSystem/JKernel/JKRArchive.h"

// Frees the loader's archives.
// FIXME(shotgun): 100 %
void cls_804B0F50::fn_803FF2C4() {
    if (field_0x18 != NULL) {
        delete field_0x18;
        field_0x18 = NULL;
    }
    if (field_0x1c != NULL) {
        delete field_0x1c;
        field_0x1c = NULL;
    }
    if (field_0x20 != NULL) {
        delete field_0x20;
        field_0x20 = NULL;
    }
    if (field_0x24 != NULL) {
        delete field_0x24;
        field_0x24 = NULL;
    }
    if (field_0x28 != NULL) {
        delete field_0x28;
        field_0x28 = NULL;
    }
    if (field_0x2c != NULL) {
        delete field_0x2c;
        field_0x2c = NULL;
    }
    if (field_0x30 != NULL) {
        delete field_0x30;
        field_0x30 = NULL;
    }
}
