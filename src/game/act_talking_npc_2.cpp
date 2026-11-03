// Actor TLK2 ("Talking NPC 2" in docs/names/actors.tsv): the spawner
// (fn_801F9484) allocates 0x240 bytes for it and calls its constructor
// fn_80203054, which stores the vtable 0x804AFAD8.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "JSystem/JUtility/TColor.h"

// Named after its vtable (0x804AFAD8).
class TalkingNpc2_804AFAD8 : public ActorBase_804A5770 {
public:
    TalkingNpc2_804AFAD8() {}
    virtual ~TalkingNpc2_804AFAD8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u32 field_0x234; // low 16 bits of the spawn parameter (0x90)
    /* 0x238 */ s32 field_0x238; // bits 16-27 of the spawn parameter
    /* 0x23C */ u8 field_0x23c;  // bits 28-31 of the spawn parameter
}; // Size: 0x240

TalkingNpc2_804AFAD8::~TalkingNpc2_804AFAD8() {}

void TalkingNpc2_804AFAD8::draw() {}

void TalkingNpc2_804AFAD8::update() {}

void TalkingNpc2_804AFAD8::init() {
    field_0x234 = field_0x90 & 0xFFFF;
    field_0x23c = (u32)field_0x90 >> 28;
    field_0x238 = ((u32)field_0x90 >> 16) & 0xFFF;
    fn_8037F2FC(field_0x234, field_0x238, field_0x23c, mPos.x, mPos.y, mPos.z, mPos.z,
                JUtility::TColor(0xFF0000FF));
}
