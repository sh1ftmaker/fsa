#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_8010DF44.h"

cls_8049D140::cls_8049D140() {}

cls_8049D140::~cls_8049D140() {
    fn_8011921C()->fn_80118798(this);
}

void cls_8049D140::init() {
    ActorBase_804A5770::init();
    fn_8011921C()->fn_8011888C(this);
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
}

void cls_8049D140::update() {
    ActorBase_804A5770::update();
}

void cls_8049D140::draw() {
    ActorBase_804A5770::draw();
}

s32 cls_8049D140::vfunc_0x5C() {
    return 0;
}

s32 cls_8049D140::vfunc_0x58() {
    return -1;
}

void cls_8049D140::vfunc_0x54() {}
