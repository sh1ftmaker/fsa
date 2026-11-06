// The IFCK actor ("Ice-floor check NPC", docs/names/actors.tsv, medium
// confidence). init() records the collision tile under it (fn_80226850) and
// registers its serial in the list of tu_8012F3E4; update() watches for the
// tile to change.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Map.h"
#include "game/act/act_ice_floor_check_npc.h"
#include "game/tu_8012F3E4.h"

cls_8049E950::cls_8049E950() : mState(0), mTile(0) {}

cls_8049E950::~cls_8049E950() {}

void cls_8049E950::init() {
    ActorBase_804A5770::init();
    mTile = fn_80226850(&mPos, field_0x4);
    fn_8012F554()->fn_8012F530(this);
}

void cls_8049E950::update() {
    ActorBase_804A5770::update();
    switch (mState) {
    case 0:
        if (mTile != fn_80226850(&mPos, field_0x4)) {
            if (vfunc_0x3C() > 0) {
                vfunc_0x30(0xFF, true);
            }
            mState = 1;
        }
        break;
    case 1:
        break;
    }
}

bool cls_8049E950::fn_8012F008() {
    switch (mState) {
    case 0:
        break;
    case 1:
        return true;
    }
    return false;
}
