// Actor TLK2 ("Talking NPC 2" in docs/names/actors.tsv): the spawner
// (spawnActor_801F9484) allocates 0x240 bytes for it and calls its constructor
// fn_80203054, which stores the vtable 0x804AFAD8.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "JSystem/JUtility/TColor.h"
#include "game/act/act_talking_npc_2.h"

void TalkingNpc2_804AFAD8::init() {
    field_0x234 = field_0x90 & 0xFFFF;
    field_0x23c = (u32)field_0x90 >> 28;
    field_0x238 = ((u32)field_0x90 >> 16) & 0xFFF;
    fn_8037F2FC(field_0x234, field_0x238, field_0x23c, mPos.x, mPos.y, mPos.z, mPos.z,
                JUtility::TColor(0xFF0000FF));
}

void TalkingNpc2_804AFAD8::update() {}

void TalkingNpc2_804AFAD8::draw() {}

TalkingNpc2_804AFAD8::~TalkingNpc2_804AFAD8() {}
