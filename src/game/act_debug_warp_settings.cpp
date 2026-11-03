// The debug warp settings actor (actor DWAP, "Debug warp settings" in
// docs/names/actors.tsv). The spawner (fn_801F9484) allocates 0x240 bytes
// and constructs it with the weak constructor fn_80200310, which calls
// ActorBase_804A5770's and stores this class's vtable (0x8049C9E8). init
// unpacks two bytes of the spawn argument at 0x90; update hands the actor
// to fn_801F3668, which clears mAlive and reports it to the game view.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/act/act_debug_warp_settings.h"

DebugWarpSettings_8049C9E8::~DebugWarpSettings_8049C9E8() {}

void DebugWarpSettings_8049C9E8::draw() {}

void DebugWarpSettings_8049C9E8::update() {
    fn_801F3668();
}

void DebugWarpSettings_8049C9E8::init() {
    field_0x238 = (field_0x90 >> 16) & 0xFF;
    field_0x23c = (field_0x90 >> 8) & 0xFF;
    field_0x230 = 0;
}
