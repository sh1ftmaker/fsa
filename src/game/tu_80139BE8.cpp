#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameRootSub20.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_8044B80C.h"
#include "game/cls_804AE9B0.h"
#include "game/tu_802EF76C.h"

void GameRootSub04_801F856C::fn_80139BE8() {
    mActorManager->fn_801F857C();
    lbl_8053ACD0 = 0;
    fn_8030361C();
    fn_8038D454();
    fn_8011921C()->fn_801184F0(true);
    gGameRoot2_8053AB10->field_0x20->field_0x235c[0] = 0;
    gGameRoot2_8053AB10->field_0x20->field_0x235c[1] = 0;
    gGameRoot2_8053AB10->field_0x20->field_0x235c[2] = 0;
    gGameRoot2_8053AB10->field_0x20->field_0x235c[3] = 0;
}

void fn_80139C5C() {
    lbl_8053AED0->fn_8043D5F0();
    fn_8038D4A8();
}

void GameRootSub04_801F856C::fn_80139C84() {
    lbl_8053AED0->fn_8043DC98();
    if (field_0x2c != NULL) {
        field_0x2c->fn_8044B8EC();
    }
}

void GameRootSub04_801F856C::fn_80139CC4() {
    gGameRoot2_8053AB10->fn_8013B438();
    mView->vfunc_0x14();
    gGameRoot2_8053AB10->fn_8013B438();
    fn_8023F2BC();
    mActorManager->fn_801F8658();
    fn_801902D0(field_0x20);
    if (gGameRoot2_8053AB10->field_0x39 == 0) {
        field_0x24->fn_8016DA6C();
    }
}

void GameRootSub04_801F856C::fn_80139D3C() {
    if (gGameRoot2_8053AB10->field_0x39 == 0) {
        field_0x24->fn_8016DB60();
    }
}

void fn_80139D70() {
    lbl_8053AED0->fn_8043D644();
    lbl_8053AED0->fn_8043D8B4();
}

void GameRootSub04_801F856C::fn_8013A1A4() {
    field_0x24->fn_8016EDB0();
    mActorManager->fn_801F8E4C();
    mView->vfunc_0xC();
    if (gGameRoot_8053AAF8->field_0x7e == 1) {
        fn_8023F088();
    }
    fn_8023F78C();
    fn_8023F728();
    lbl_8053ACD0 = 1;
    field_0x10->fn_802A3D80();
    field_0x14->fn_8043EDF8();
    fn_80439AB0(field_0x18);
    field_0x1c->fn_8037AFC0();
    field_0x20->fn_8019169C();
    fn_8038D870();
    field_0x34 = 0;
    lbl_80529DEC.field_0x240 = 0;
    lbl_80529DEC.field_0x1f4[0] = 0;
    lbl_80529DEC.field_0x1f4[1] = 0;
    lbl_80529DEC.field_0x1f4[2] = 0;
    lbl_80529DEC.field_0x1f4[3] = 0;
    lbl_80529DEC.field_0x24f = fn_803B8C90();
    lbl_80529DEC.field_0x253 = 0;
}

GameRootSub04_801F856C::~GameRootSub04_801F856C() {
    u32 color = 0xFFFFFF00;
    fn_8013A884(&color);
    getEffectSystem_8013C824()->mEmitterManager->forceDeleteAllEmitter();
    if (lbl_80529DEC.field_0x24b != 0) {
        lbl_80529DEC.field_0x24b = 0;
    }
    if (field_0x2c != NULL) {
        delete field_0x2c;
        field_0x2c = NULL;
    }
    if (field_0x30 != NULL) {
        field_0x30->freeAll();
        field_0x30->destroy();
    }
    lbl_8053AB00->getTotalFreeSize();
    field_0x24->fn_8016EE84(1);
    field_0x24 = NULL;
    lbl_8053AB00->getTotalFreeSize();
    field_0x20->fn_80191744(1);
    field_0x20 = NULL;
    lbl_8053AB00->getTotalFreeSize();
    delete field_0x1c;
    field_0x1c = NULL;
    lbl_8053AB00->getTotalFreeSize();
    delete field_0x18;
    field_0x18 = NULL;
    lbl_8053AB00->getTotalFreeSize();
    delete field_0x14;
    field_0x14 = NULL;
    lbl_8053AB00->getTotalFreeSize();
    delete field_0x10;
    field_0x10 = NULL;
    lbl_8053AB00->getTotalFreeSize();
    delete mActorManager;
    mActorManager = NULL;
    lbl_8053AB00->getTotalFreeSize();
    delete field_0x4;
    field_0x4 = NULL;
    lbl_8053AB00->getTotalFreeSize();
    delete mView;
    mView = NULL;
    if (lbl_80529DEC.field_0x10 != NULL) {
        lbl_80529DEC.field_0x10->unmount();
        lbl_80529DEC.field_0x10 = NULL;
    }
    if (lbl_80529DEC.field_0x4c != NULL) {
        lbl_80529DEC.field_0x4c->unmount();
        lbl_80529DEC.field_0x4c = NULL;
    }
}
