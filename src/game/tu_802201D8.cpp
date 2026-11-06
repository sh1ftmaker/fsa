#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/tu_802201D8.h"
#include "game/DrawShadow.h"

cls_804A6828::cls_804A6828() {
    mEmitter = NULL;
}

cls_804A6828::~cls_804A6828() {
    releaseEmitter_801EE434(mEmitter);
}

void cls_804A6828::init() {
    cls_804A48B0::init();
    mEmitter = spawnEmitter_8013CC50(field_0x4, &mPos, 0x290, 0, 0, NULL, NULL, true, 1.0f);
}

void cls_804A6828::update() {
    cls_804A48B0::update();
    setEmitterPos_8013CB44(mEmitter, &mPos, 0, 0, true);
}

void cls_804A6828::draw() {
    drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 24.0f);
}
