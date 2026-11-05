// The URAC actor ("Ura (dark-world) colour", docs/names/actors.tsv, medium
// confidence). Spawned by fn_801F9484: __nw__ 0x230, inline constructor
// fn_8020203C (base constructor and vtable only), so no members of its own.
// Each frame it ORs 1 into the view's 16-bit word for its field_0x4
// (fn_802D77BC). Overrides the destructor, init(), update() and draw().

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/act_ura_colour.h"

UraColour_804A9A70::~UraColour_804A9A70() {}

void UraColour_804A9A70::draw() {}

void UraColour_804A9A70::update() {
    getGameView_802D6A9C()->fn_802D77BC(field_0x4, 1);
}

void UraColour_804A9A70::init() {
    update();
}
