#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"

void Switch_804A7F38::draw() {
}

void Switch_804A7F38::vfunc_0x54(s32 arg) {
    switch (field_0x258) {
    case 0x12:
    case 0x41:
    case 0x68:
        gBgRewrite_8053AC98->fn_802A3948(&mPos, arg, field_0x4, 0x13);
        break;
    case 0x43:
    case 0x50:
    case 0x6a:
        gBgRewrite_8053AC98->fn_802A3948(&mPos, arg, field_0x4, 0x51);
        break;
    case 0x14:
    case 0x52:
        gBgRewrite_8053AC98->fn_802A3948(&mPos, arg, field_0x4, 0x15);
        break;
    case 0x15a:
        gBgRewrite_8053AC98->fn_802A3948(&mPos, arg, field_0x4, 0x360);
        break;
    }
}

Switch_804A7F38::~Switch_804A7F38() {
}
