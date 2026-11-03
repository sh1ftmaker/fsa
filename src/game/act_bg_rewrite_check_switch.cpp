// The BGWC actor ("BG-rewrite check switch", docs/names/actors.tsv, medium
// confidence). Spawned by fn_801F9484: __nw__ 0x234, inline constructor
// fn_80202FDC (base constructor and vtable only, so the word at 0x230 is left
// uninitialized; nothing in this unit touches it). Overrides the destructor,
// init(), update() and draw().

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"

// Named after its vtable, 0x804A8998.
class BgRewriteCheckSwitch_804A8998 : public ActorBase_804A5770 {
public:
    BgRewriteCheckSwitch_804A8998() {}
    virtual ~BgRewriteCheckSwitch_804A8998();
    virtual void init();
    virtual void update();
    virtual void draw();

    /* 0x230 */ u32 field_0x230; // not initialized or used here
}; // Size: 0x234

BgRewriteCheckSwitch_804A8998::~BgRewriteCheckSwitch_804A8998() {}

void BgRewriteCheckSwitch_804A8998::draw() {}

void BgRewriteCheckSwitch_804A8998::update() {
    if (fn_802D6A9C()->field_0x131de) {
        vfunc_0x30(0xFF, true);
        fn_801F3668();
    }
}

void BgRewriteCheckSwitch_804A8998::init() {}
