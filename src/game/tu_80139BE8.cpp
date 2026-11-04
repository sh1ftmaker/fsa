#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/act/tu_8044B80C.h"

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

void GameRootSub04_801F856C::fn_80139D3C() {
    if (lbl_8053AB10->field_0x39 == 0) {
        fn_8016DB60(field_0x24);
    }
}
