// The BGWC actor ("BG-rewrite check switch", docs/names/actors.tsv, medium
// confidence). Spawned by fn_801F9484: __nw__ 0x234, inline constructor
// fn_80202FDC (base constructor and vtable only, so the word at 0x230 is left
// uninitialized; nothing in this unit touches it). Overrides the destructor,
// init(), update() and draw().

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/act_bg_rewrite_check_switch.h"

BgRewriteCheckSwitch_804A8998::~BgRewriteCheckSwitch_804A8998() {}

void BgRewriteCheckSwitch_804A8998::draw() {}

void BgRewriteCheckSwitch_804A8998::update() {
    if (getGameView_802D6A9C()->field_0x131de) {
        vfunc_0x30(0xFF, true);
        fn_801F3668();
    }
}

void BgRewriteCheckSwitch_804A8998::init() {}
