#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_message_check_npc.h"

// The one animation: cel 0 for 20 frames, then the terminator.
static const s16 lbl_8053C4F8[4] = {0, 20, -1, -1};
// The table of animations (one), followed by a zero word in the original.
static const s16* lbl_80539EF0[2] = {lbl_8053C4F8, NULL};

MessageCheckNPC_8049D0E8::~MessageCheckNPC_8049D0E8() {}

void MessageCheckNPC_8049D0E8::draw() {
    ActorBase_804A5770::draw();
    s32 frame = mAnim.mCel + 0x386;
    const JGeometry::TVec3f* pos = &mPos;
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(pos, res, frame, NULL, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, mAnim.mCel + 0x386, false, 1, -1, 0, 0, 0);
}

void MessageCheckNPC_8049D0E8::update() {
    ActorBase_804A5770::update();
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    mAnim.fn_801D1C18();
}

void MessageCheckNPC_8049D0E8::init() {
    ActorBase_804A5770::init();
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
    mAnim.fn_801D2608(lbl_80539EF0, 1);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1F14(0.0f);
}

MessageCheckNPC_8049D0E8::MessageCheckNPC_8049D0E8() {}
