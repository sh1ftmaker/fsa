#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/tu_80447618.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"

// Pane tags of four groups of fourteen (8-byte, zero-padded) names: group i
// is 'Pp<i+1>i01'.., 'Pp<i+1>ib01'.., 'Ndata0<..>' at [6] and 'Ndata2<i>' at
// [13]; the constructor searches [6] and [13] of each group.
static const u64 lbl_80490410[4][14] = {
    {
        0x0000507031693031ull, 0x0000507031693032ull, 0x0050703169623031ull,
        0x0050703169623032ull, 0x0050703169623033ull, 0x0050703169623034ull,
        0x004E646174613031ull, 0x00000050316F3031ull, 0x00000050316F3032ull,
        0x0000005031743031ull, 0x00005031746B3031ull, 0x0000005031743032ull,
        0x00005031746B3032ull, 0x004E646174613230ull,
    },
    {
        0x0000507032693031ull, 0x0000507032693032ull, 0x0050703269623031ull,
        0x0050703269623032ull, 0x0050703269623033ull, 0x0050703269623034ull,
        0x004E646174613034ull, 0x00000050326F3031ull, 0x00000050326F3032ull,
        0x0000005032743031ull, 0x00005032746B3031ull, 0x0000005032743032ull,
        0x00005032746B3032ull, 0x004E646174613231ull,
    },
    {
        0x0000507033693031ull, 0x0000507033693032ull, 0x0050703369623031ull,
        0x0050703369623032ull, 0x0050703369623033ull, 0x0050703369623034ull,
        0x004E646174613037ull, 0x00000050336F3031ull, 0x00000050336F3032ull,
        0x0000005033743031ull, 0x00005033746B3031ull, 0x0000005033743032ull,
        0x00005033746B3032ull, 0x004E646174613232ull,
    },
    {
        0x0000507034693031ull, 0x0000507034693032ull, 0x0050703469623031ull,
        0x0050703469623032ull, 0x0050703469623033ull, 0x0050703469623034ull,
        0x004E646174613130ull, 0x00000050346F3031ull, 0x00000050346F3032ull,
        0x0000005034743031ull, 0x00005034746B3031ull, 0x0000005034743032ull,
        0x00005034746B3032ull, 0x004E646174613233ull,
    },
};

void cls_804B1C20::fn_80447958() {
    lbl_8053AB10->fn_8013B490();
    mScreen->draw(0.0f, 0.0f, lbl_8053AB10->field_0x10);
}

cls_804B1C20::~cls_804B1C20() {
    delete mScreen;
    mScreen = NULL;
    for (s32 i = 0; i < 4; i++) {
        delete field_0x8[i];
        field_0x8[i] = NULL;
        delete field_0x18[i];
        field_0x18[i] = NULL;
    }
    lbl_8053AF08 = NULL;
}

cls_804B1C20::cls_804B1C20() {
    lbl_8053AF08 = this;
    mScreen = new J2DScreen();
    mScreen->setPriority("mg_kokko.blo", 0x1100000, lbl_80529DEC.field_0x10);
    for (s32 i = 0; i < 4; i++) {
        field_0x8[i] = new cls_804AF86C();
        field_0x8[i]->fn_80402F04(mScreen->search(lbl_80490410[i][6]));
        field_0x18[i] = new cls_804AF86C();
        field_0x18[i]->fn_80402F04(mScreen->search(lbl_80490410[i][13]));
    }
    for (s32 i = 0; i < 5; i++) {
        field_0x28[i] = 0;
    }
}
