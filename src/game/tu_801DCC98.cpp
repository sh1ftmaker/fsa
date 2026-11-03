#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/tu_801DCC98.h"

// FIXME: 99.58 %. Only the registers of the position compare differ: the
// target keeps mPos.x in f2 and mPos.y in f1, ours the reverse.
void cls_804A5038::update() {
    mPos += field_0x3c;
    fn_8013CB44(mEmitter2a8, &mPos, 0, 0, true);
    fn_8013CB44(mEmitter2ac, &mPos, 0, 0, true);
    s32 layer = field_0x4;
    GameView_804A9950* view = fn_802D6A9C();
    JGeometry::TBox2f* box = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    bool inside = false;
    if (mPos.x >= box->i.x && mPos.y >= box->i.y && mPos.x < box->f.x && mPos.y < box->f.y) {
        inside = true;
    }
    if (!inside) {
        mAlive = false;
    } else if (fn_801F2B7C(field_0x114, -1, 24)) {
        mAlive = false;
    }
}

void cls_804A5038::init() {
    EnemyBase_804A4808::init();
    mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
    mBox2.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= 0x300;
    field_0x114 = 2;
    field_0x194 = 1;
    mEmitter2a8 = fn_8013CC50(field_0x4, &mPos, 0x256, 0, 0, NULL, NULL, true, 1.0f);
    mEmitter2ac = fn_8013CC50(field_0x4, &mPos, 0x257, 0, 0, NULL, NULL, true, 1.0f);
}

cls_804A5038::~cls_804A5038() {
    fn_801EE434(mEmitter2a8);
    fn_801EE434(mEmitter2ac);
}

cls_804A5038::cls_804A5038() {
    mEmitter2a8 = NULL;
    mEmitter2ac = NULL;
}
