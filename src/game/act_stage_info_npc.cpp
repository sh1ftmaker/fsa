// The stage info NPC (actor STGN, "Stage info NPC" in docs/names/actors.tsv).
// The spawner (fn_801F9484) constructs it with the weak constructor
// fn_801FF7BC, which calls ActorBase_804A5770's and stores this class's
// vtable (0x804AA7A8). init passes the low four bits of the spawn argument
// at 0x90 to fn_802D7934 and removes the actor at once.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/act_stage_info_npc.h"

StageInfoNpc_804AA7A8::~StageInfoNpc_804AA7A8() {}

void StageInfoNpc_804AA7A8::draw() {}

void StageInfoNpc_804AA7A8::update() {}

void StageInfoNpc_804AA7A8::init() {
    getGameView_802D6A9C()->fn_802D7934(field_0x90 & 0xF);
    mAlive = false;
}
