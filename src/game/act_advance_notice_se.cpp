#include "game/GameAudio.h"
#include "game/act/act_advance_notice_se.h"
#include "game/ActorManager.h"

static const u32 sSoundIds[4] = {0x00030033, 0x0004004D, 0x00030030, 0};
static const u8 sVariants[8] = {0, 1, 2, 3, 0, 0, 0, 0};

cls_804A9C78::~cls_804A9C78() {}

void cls_804A9C78::draw() {}

void cls_804A9C78::update() {
    ActorSoundObj_804578F4* sound = fn_801F1184();
    sound->fn_804561D8(sSoundIds[mSoundIndex], sVariants[mVariant], 0, mFlag);
}

void cls_804A9C78::init() {
    mSoundIndex = field_0x90 & 0xFFF;
    mVariant = (field_0x90 >> 24) & 7;
    mFlag = (field_0x90 >> 23) & 1;
    update();
}
