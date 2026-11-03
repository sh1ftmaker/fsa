// The stage info NPC (actor STGN, "Stage info NPC" in docs/names/actors.tsv).
// The spawner (fn_801F9484) constructs it with the weak constructor
// fn_801FF7BC, which calls ActorBase_804A5770's and stores this class's
// vtable (0x804AA7A8). init passes the low four bits of the spawn argument
// at 0x90 to fn_802D7934 and removes the actor at once.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"

// Named after its vtable (0x804AA7A8).
class StageInfoNpc_804AA7A8 : public ActorBase_804A5770 {
public:
    StageInfoNpc_804AA7A8() {}

    /* 0x08 */ virtual ~StageInfoNpc_804AA7A8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
};

StageInfoNpc_804AA7A8::~StageInfoNpc_804AA7A8() {}

void StageInfoNpc_804AA7A8::draw() {}

void StageInfoNpc_804AA7A8::update() {}

void StageInfoNpc_804AA7A8::init() {
    fn_802D7934(fn_802D6A9C(), field_0x90 & 0xF);
    mAlive = false;
}
