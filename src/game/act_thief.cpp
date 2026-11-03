#include "game/ActorBase.h"
#include "game/Camera.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/act_thief.h"

static const f32 lbl_8053F860[1] = {0.0f};

void Thief_804A5A30::vfunc_0xA0() {
    if (mState != 9 && mState != 8) {
        lbl_8053AB10->field_0x24->fn_8030C210(&mPos);
    }
}

bool ActorBase_804A5770::fn_80204D44() {
    bool result = false;
    if (field_0x94 == 1 || field_0xc4 != 0) {
        result = true;
    }
    return result;
}

bool Thief_804A5A30::fn_802064E0() {
    bool result = false;
    s32 bit = field_0x354;
    if (bit != 0) {
        if (fn_802D6A9C()->fn_802D7F40(bit)) {
            result = true;
        }
    }
    return result;
}

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

// True in state 1 when no fn_801CD2F8 hit is set for the layer and mPos lies
// outside the view box of that layer.
bool Thief_804A5A30::fn_8020609C() const {
    s32 layer;
    bool result = false;
    if (field_0x94 == 1) {
        if (!fn_801CD2F8(field_0x4)) {
            layer = field_0x4;
            GameView_804A9950* view = fn_802D6A9C();
            const JGeometry::TBox2f& box = layer < 8 ? view->mVisibleBox : view->field_0xdc;
            if (!contains(box, mPos)) {
                result = true;
            }
        }
    }
    return result;
}

Thief_804A5A30::~Thief_804A5A30() {
    fn_801EE434(mEmitter358);
    fn_802049E0(fn_80204638(), this);
}

Thief_804A5A30::Thief_804A5A30() {
    field_0x340 = -1;
    field_0x344 = 0;
    field_0x348 = 0;
    field_0x349 = 0;
    field_0x34a = 0;
    field_0x34b = 0;
    field_0x34c = 0;
    mType = 0;
    field_0x354 = 0;
    mEmitter358 = NULL;
    field_0x35c.frames = 0;
    field_0x35c.vel.x = lbl_8053F860[0];
    field_0x35c.vel.y = lbl_8053F860[0];
    field_0x368.fn_80217484();
    field_0x374.fn_8021BA30(0);
}
