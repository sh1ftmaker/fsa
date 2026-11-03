// Actor TALK ("Talking NPC" in docs/names/actors.tsv): the spawner
// (fn_801F9484) allocates 0x23C bytes for it and calls its constructor
// fn_802033F4, which stores the vtable 0x804AF548. The spawn parameter
// holds the player (top 4 bits) and the message id (low 28 bits).

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/act/act_talking_npc.h"

TalkingNPC_804AF548::~TalkingNPC_804AF548() {}

// FIXME: 91.93 %. Only the order around the by-value position: the
// original loads field_0x4 (r6) and sets r7 before the psq_l of mPos and
// moves `mr r3, r31` between the psq_l and the lfs; ours loads r6 last.
// tu_802F3B74's init call of fn_8038BD08 differs the same way, so it is
// probably the TVec3f copy (mkdd's JGeometry has a user copy constructor
// through setTVec3f; include/JSystem/JGeometry.h has none), not this code.
void TalkingNPC_804AF548::update() {
    u32 player = field_0x90 >> 28;
    u32 msg = field_0x90 & 0x0FFFFFFF;
    switch (mState) {
    case 0:
        if (fn_8038AD24(player, msg, field_0x234, field_0x4, mPos, 0x1004)) {
            mState = 1;
        } else {
            mAlive = false;
        }
        break;
    case 1:
        field_0x238--;
        if (field_0x238 <= 0 || fn_802D6A9C()->fn_802DCD0C()) {
            if (fn_80384C58(player, field_0x234)) {
                mAlive = false;
            }
        }
        break;
    }
}

void TalkingNPC_804AF548::init() {
    mState = 0;
    field_0x238 = 0x78;
    field_0x234 = mSerial;
    field_0x11f = 1;
    field_0x11e = 1;
}
