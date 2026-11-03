// The debug warp settings actor (actor DWAP, "Debug warp settings" in
// docs/names/actors.tsv). The spawner (fn_801F9484) allocates 0x240 bytes
// and constructs it with the weak constructor fn_80200310, which calls
// ActorBase_804A5770's and stores this class's vtable (0x8049C9E8). init
// unpacks two bytes of the spawn argument at 0x90; update hands the actor
// to fn_801F3668, which clears mAlive and reports it to the game view.

#include "game/GameAudio.h"
#include "game/ActorBase.h"

// Named after its vtable (0x8049C9E8).
class DebugWarpSettings_8049C9E8 : public ActorBase_804A5770 {
public:
    DebugWarpSettings_8049C9E8() {}

    /* 0x08 */ virtual ~DebugWarpSettings_8049C9E8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ s32 field_0x234;
    /* 0x238 */ s32 field_0x238; // bits 16-23 of field_0x90
    /* 0x23C */ s32 field_0x23c; // bits 8-15 of field_0x90
}; // Size: 0x240

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
