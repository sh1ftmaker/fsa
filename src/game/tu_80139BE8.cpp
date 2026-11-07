#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameRootSub20.h"
#include "game/GameState.h"
#include "game/GameMode.h"
#include "game/Message.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_8044B80C.h"
#include "game/cls_804AE9B0.h"
#include "game/BgRewrite.h"
#include "game/tu_802EF76C.h"
#include "JSystem/JKernel/JKRExpHeap.h"

// Shift-JIS report strings (.rodata of this unit); not written as source because the bytes are escaped.
extern const char lbl_80465920[];
extern const char lbl_80465960[];

// FIXME(shotgun): 33.1 %, missing/extra (the colour is copied byte by byte; fn_8013A884 takes a u32* in tu_802EF76C.h)
// 0x8013A884
// auto_inline off: the original GameRootSub04 destructor (bl at 0x8013A2B8) calls it out of line.
#pragma auto_inline off
void fn_8013A884(u32* color) {
    gGameRoot2_8053AB10->field_0x2c->field_0x4.set(*color);
}
#pragma auto_inline reset

// FIXME(shotgun): 96.0 %, missing/extra + reloc-name
// 0x8013A538
GameRootSub04_801F856C::GameRootSub04_801F856C(JKRHeap* heap) {
    lbl_8053AB00 = heap;
    heap->getTotalFreeSize();
    mView = new (heap, 0) GameView_804A9950();
    lbl_8053AB00->getTotalFreeSize();
    field_0x4 = new (heap, 0) PlayerManager_804A6F28();
    heap->getTotalFreeSize();
    mActorManager = new (heap, 0) ActorManager_804A5800(heap);
    heap->getTotalFreeSize();
    field_0x10 = new (heap, 0) BgRewrite_804A7C98();
    heap->getTotalFreeSize();
    heap->getTotalFreeSize();
    field_0x14 = new (heap, 0) cls_804B1920();
    heap->getTotalFreeSize();
    field_0x18 = new (heap, 0) cls_804B17A0();
    heap->getTotalFreeSize();
    field_0x1c = new (heap, 0) cls_804AE9B0();
    heap->getTotalFreeSize();
    field_0x20 = new (heap, 0) GameRootSub20_801902C0(heap);
    heap->getTotalFreeSize();
    field_0x24 = new (heap, 0) GameRootSub24_8016A3AC(heap);
    if ((lbl_80529DEC.field_0x118 == 9 && lbl_80529DEC.field_0x11c == 3) ||
        (lbl_80529DEC.field_0x118 == 8 && lbl_80529DEC.field_0x11c == 2)) {
        field_0x30 = JKRExpHeap::create(0x86470, heap, false);
        if (field_0x30 == NULL) {
            static s32 reported = 0;
            if (reported == 0) {
                reported = 1;
                f32 total = (f32)heap->getTotalFreeSize() * (1.0f / 1024.0f);
                OSReport(lbl_80465920, 600000.0 / 1024.0, (f32)heap->getFreeSize() * (1.0f / 1024.0f), total);
            }
        } else {
            OSReport(lbl_80465960);
        }
        heap->becomeCurrentHeap();
        field_0x2c = new (heap, 0) cls_804B1D10();
    }
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
    field_0x18->fn_80439AB0();
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

// The stage-9 sub-stage-3 room with field_0x7f set: the per-player tables below stay as they are.
static inline bool isFrozenRoom() {
    if (gGameRoot_8053AAF8->field_0x7f != 0 && lbl_80529DEC.field_0x118 == 9 &&
        lbl_80529DEC.field_0x11c == 3) {
        return true;
    }
    return false;
}

// FIXME(shotgun): 93.4 %, missing/extra + reloc-name
// 0x80139D9C
void GameRootSub04_801F856C::fn_80139D9C() {
    GameRoot_8049EF20* root = gGameRoot_8053AAF8;
    s32 mode = root->field_0x4c;
    bool step = false;
    if (mode == GAME_MODE_7 && root->field_0x60 >= 4) {
        step = true;
    } else if (isGameMode(GAME_MODE_B) || (root->field_0x88 == 2 ? true : false)) {
        step = true;
    } else if (root->field_0x7f == 0 && (mode == GAME_MODE_A || mode == GAME_MODE_C)) {
        step = true;
    }
    if (step) {
        root->fn_80136500();
        mView->vfunc_0x10();
        fn_8023F390();
        field_0x20->fn_801914C4();
        field_0x24->fn_8016DE30();
        mActorManager->fn_801F871C();
        field_0x20->fn_80190DC8();
        field_0x24->fn_8016DCEC();
        gBgRewrite_8053AC98->fn_802A3A0C();
        fn_8038D570();
        gGameRoot_8053AAF8->fn_80136364();
        if (field_0x2c != NULL) {
            field_0x2c->fn_8044B960();
        }
    } else {
        fn_8023F390();
        gBgRewrite_8053AC98->fn_802A3A0C();
    }
    fn_8011921C();
    fn_80118E20();
    field_0x34++;
    for (s32 i = 0; i < 4; i++) {
        s32 v = fn_80236100(i);
        if (!isFrozenRoom()) {
            lbl_80529DEC.field_0x150[i] = v;
        }
        v = fn_80237774(i);
        if (!isFrozenRoom()) {
            lbl_80529DEC.field_0x160[i] = v;
        }
        v = fn_80235A30(i);
        if (!isFrozenRoom()) {
            lbl_80529DEC.field_0x170[i] = v;
        }
        v = fn_8023587C(i);
        if (!isFrozenRoom()) {
            lbl_80529DEC.field_0x180[i] = v;
        }
        v = fn_80235ACC(i);
        if (!isFrozenRoom()) {
            lbl_80529DEC.field_0x190[i] = v;
        }
    }
    s32 word = gGameRoot_8053AAF8->field_0xf8;
    if (!isFrozenRoom()) {
        lbl_80529DEC.field_0x1b0 = word;
    }
    u8 byte = gGameRoot_8053AAF8->field_0xfc;
    if (!isFrozenRoom()) {
        lbl_80529DEC.field_0x214 = byte;
    }
    u8 bits = lbl_80529DEC.field_0x24a;
    if ((bits & 1) || (bits & 2) || (bits & 4) || (bits & 8)) {
        if (!isFrozenRoom()) {
            lbl_80529DEC.field_0x215 = 0;
        }
    } else {
        if (!isFrozenRoom()) {
            lbl_80529DEC.field_0x215 = 1;
        }
    }
}

void fn_80139D70() {
    lbl_8053AED0->fn_8043D644();
    lbl_8053AED0->fn_8043D8B4();
}

void GameRootSub04_801F856C::fn_80139D3C() {
    if (gGameRoot2_8053AB10->field_0x39 == 0) {
        field_0x24->fn_8016DB60();
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

void GameRootSub04_801F856C::fn_80139C84() {
    lbl_8053AED0->fn_8043DC98();
    if (field_0x2c != NULL) {
        field_0x2c->fn_8044B8EC();
    }
}

void GameRootSub04_801F856C::fn_80139C5C() {
    lbl_8053AED0->fn_8043D5F0();
    fn_8038D4A8();
}

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
