#include "game/GameAudio.h"
#include "game/GameMode.h"
#include "game/Map.h"
#include "game/Effect.h"
#include "JSystem/J3D/J3DDrawBuffer.h"

void GameRoot2_8049F108::fn_8013B538() {
    if (isGameModeSet()) {
        field_0x20->fn_802FF0EC();
    }
}

void GameRoot2_8049F108::fn_8013B618() {
    if (isGameModeSet()) {
        field_0x20->fn_802FF0EC();
    }
    field_0x28->fn_8013D5D4();
    field_0x8->frameInit();
    field_0xc->frameInit();
}
