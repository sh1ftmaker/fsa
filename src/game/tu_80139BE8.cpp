#include "game/ActorManager.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/act/tu_8044B80C.h"

void GameRootSub04_801F856C::fn_80139BE8() {
    mActorManager->fn_801F857C();
    lbl_8053ACD0 = 0;
    fn_8030361C();
    fn_8038D454();
    fn_801184F0(fn_8011921C(), true);
    lbl_8053AB10->field_0x20->field_0x235c[0] = 0;
    lbl_8053AB10->field_0x20->field_0x235c[1] = 0;
    lbl_8053AB10->field_0x20->field_0x235c[2] = 0;
    lbl_8053AB10->field_0x20->field_0x235c[3] = 0;
}

extern "C" void fn_80139C5C() {
    lbl_8053AED0->fn_8043D5F0();
    fn_8038D4A8();
}

void GameRootSub04_801F856C::fn_80139C84() {
    fn_8043DC98(lbl_8053AED0);
    if (field_0x2c != NULL) {
        field_0x2c->fn_8044B8EC();
    }
}

extern "C" void fn_80139D70() {
    fn_8043D644(lbl_8053AED0);
    fn_8043D8B4(lbl_8053AED0);
}

void GameRootSub04_801F856C::fn_80139CC4() {
    lbl_8053AB10->fn_8013B438();
    mView->vfunc_0x14();
    lbl_8053AB10->fn_8013B438();
    fn_8023F2BC();
    mActorManager->fn_801F8658();
    fn_801902D0(field_0x20);
    if (lbl_8053AB10->field_0x39 == 0) {
        field_0x24->fn_8016DA6C();
    }
}

void GameRootSub04_801F856C::fn_80139D3C() {
    if (lbl_8053AB10->field_0x39 == 0) {
        fn_8016DB60(field_0x24);
    }
}
