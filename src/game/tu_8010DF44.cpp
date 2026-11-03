#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_8010DF44.h"

void cls_8049D140::vfunc_0x54() {}

s32 cls_8049D140::vfunc_0x58() {
    return -1;
}

s32 cls_8049D140::vfunc_0x5C() {
    return 0;
}

void cls_8049D140::draw() {
    ActorBase_804A5770::draw();
}

void cls_8049D140::update() {
    ActorBase_804A5770::update();
}

void cls_8049D140::init() {
    ActorBase_804A5770::init();
    fn_8011888C(fn_8011921C(), this);
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
}

cls_8049D140::~cls_8049D140() {
    fn_80118798(fn_8011921C(), this);
}

cls_8049D140::cls_8049D140() {}
