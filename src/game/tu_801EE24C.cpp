// The actor base class (include/game/ActorBase.h) and the code around it.

#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "JSystem/JParticle/JPAEmitter.h"

// Lets an emitter that is still on screen finish its particles; deletes one
// that is off screen at once.
static inline void releaseEmitter(JPABaseEmitter* emitter) {
    GameView_804A9950* view = fn_802D6A9C();
    f32 x = emitter->mGlobalTrs.x;
    f32 y = emitter->mGlobalTrs.y - emitter->mGlobalTrs.z;
    bool onScreen = false;
    if (x >= view->mVisibleBox.i.x && y >= view->mVisibleBox.i.y && x < view->mVisibleBox.f.x &&
        y < view->mVisibleBox.f.y)
    {
        onScreen = true;
    }
    if (onScreen) {
        emitter->clearStatus(JPAEMIT_Immortal);
        emitter->becomeInvalidEmitter();
    } else {
        fn_8013C824()->mEmitterManager->forceDeleteEmitter(emitter);
    }
}

ActorSoundObj_804578F4* ActorBase_804A5770::fn_801F1184() const {
    return lbl_8053AAF8->field_0x4->mActorManager->fn_801F6644(mSlot);
}

// Whether pos is blocked on layer: by a tile, or by another actor's
// collision box from the last frame.
// FIXME: 89.5 %. Same instructions; the target keeps layer in r29 and pos
// in r28, this code the other way round (so the two argument moves before
// fn_802265DC are scheduled differently).
bool ActorBase_804A5770::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    bool blocked = true;
    if (!fn_802265DC(pos, layer, false)) {
        s32 serial = mSerial;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (!boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
            blocked = false;
        }
    }
    return blocked;
}

// 32.0f is lbl_8053F748; fn_801F26CC (reloc-name) and fn_801F2718 pair with
// it only once the unit's .sdata2 order is fixed (see the constructor's FIXME).
s32 ActorBase_804A5770::fn_801F26CC(const u32* ids) {
    return fn_801F856C()->fn_801F7F58(this, ids, mSerial, 32.0f);
}

// FIXME: 99.5 %. Only the literal differs (lbl_8053F748, see above).
s32 ActorBase_804A5770::fn_801F2718(u32 actorId) const {
    return fn_801F8108(this, actorId, mSerial, 32.0f);
}

u8 ActorBase_804A5770::fn_801F2B20(s32 arg) {
    u8 prev = field_0x164;
    field_0x168 = arg;
    fn_801F856C()->fn_801F84C4(this);
    if (field_0x164 != 0) {
        field_0x164 = 0;
    }
    return prev;
}

// The body of fn_801F329C. fn_801F320C inlines it, and MWCC only inlines a
// function defined before the caller, so it is an inline of its own.
static inline bool isPlayerBelowLayer8(const ActorBase_804A5770* actor, s32 player) {
    bool result = false;
    if (fn_8023DE58(player) < 8 && actor->getField_0x4() == 8) {
        result = true;
    }
    return result;
}

// Whether `player` is on this actor's layer (fn_8023DE58), or counts as on
// it through fn_801F329C.
bool ActorBase_804A5770::fn_801F320C(s32 player) const {
    s32 layer = field_0x4;
    bool result = false;
    s32 playerLayer = fn_8023DE58(player);
    if (playerLayer == layer || isPlayerBelowLayer8(this, player)) {
        result = true;
    }
    return result;
}

// Whether `player` is on a layer below 8 while this actor is on layer 8.
bool ActorBase_804A5770::fn_801F329C(s32 player) {
    return isPlayerBelowLayer8(this, player);
}

void ActorBase_804A5770::draw() {}

void ActorBase_804A5770::savePrevPos() {
    mPrevPos = mPos;
}

// The box `ofs` (relative to pos) in top-down screen space (x, y - z).
static inline void toScreenBox(JGeometry::TBox2f* out, const JGeometry::TBox2f& ofs,
                               const Vec& pos) {
    out->set(ofs.i.x + pos.x, ofs.i.y + pos.y - pos.z, ofs.f.x + pos.x, ofs.f.y + pos.y - pos.z);
}

// FIXME: float registers differ (the player's z lands in f7, the target uses
// f5; the overlap box's max in f5/f6 instead of f6/f7); the rest matches.
void ActorBase_804A5770::vfunc_0x18() {
    if (mPos.z <= 16.0f && mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        JGeometry::TBox2f box;
        toScreenBox(&box, mBox, mPos);
        for (int i = 0; i < 4; i++) {
            if (fn_8023F264(i) && getField_0x4() == fn_8023DE58(i)) {
                JGeometry::TBox2f playerBox;
                toScreenBox(&playerBox, JGeometry::TBox2f(-12.0f, -28.0f, 12.0f, 12.0f),
                            *fn_8023E724(i));
                JGeometry::TBox2f overlap = box;
                if (overlap.intersect(playerBox)) {
                    fn_802397B4(&box, getField_0x4(), mSerial);
                    return;
                }
            }
        }
    }
}

void ActorBase_804A5770::vfunc_0x40() {}

// The actor's bit in the screen and stage tables of the view (game/GameView.h):
// the top five bits of field_0x90, 0 for none.
s32 ActorBase_804A5770::vfunc_0x3C() {
    return field_0x90 >> 27;
}

s32 ActorBase_804A5770::vfunc_0x38() {
    return fn_802D6A9C()->fn_802D7E04(vfunc_0x3C());
}

bool ActorBase_804A5770::vfunc_0x34() {
    if (vfunc_0x3C()) {
        return fn_802D6A9C()->fn_802D800C(vfunc_0x3C());
    }
    return false;
}

void ActorBase_804A5770::vfunc_0x30(u8 value, bool set) {
    if (vfunc_0x3C()) {
        fn_802D6A9C()->fn_802D8050(vfunc_0x3C(), set, value);
    }
}

void ActorBase_804A5770::update() {}

void ActorBase_804A5770::vfunc_0x10() {}

void ActorBase_804A5770::init() {}

ActorBase_804A5770::~ActorBase_804A5770() {
    if (mEmitter100 != NULL) {
        releaseEmitter(mEmitter100);
    }
    if (mEmitter104 != NULL) {
        releaseEmitter(mEmitter104);
    }
    fn_801F856C()->fn_801F8448(this);
}

// FIXME: the float literals (0, 1, -8, 8, 40) pair with the target's .sdata2
// only once the unit's other functions fix its order. 40.0f also needs
// lbl_8053F7F8's size in symbols.txt corrected (0x8 there: 40.0f plus a zero
// word that is probably padding) before a 4-byte literal can pair with it.
ActorBase_804A5770::ActorBase_804A5770()
    : field_0x4(0),
      field_0x8(0),
      mPos(0.0f, 0.0f, 0.0f),
      mPrevPos(0.0f, 0.0f, 0.0f),
      mSpawnPos(0.0f, 0.0f, 0.0f),
      field_0x30(0.0f, 0.0f, 0.0f),
      field_0x3c(0.0f, 0.0f, 0.0f),
      field_0x48(1.0f, 1.0f, 1.0f),
      field_0x54(1.0f, 1.0f, 1.0f),
      mBox(-8.0f, -8.0f, 8.0f, 8.0f),
      mBox2(0.0f, 0.0f, 0.0f, 0.0f),
      field_0x80(0.0f, 0.0f, 0.0f, 0.0f),
      field_0x90(0),
      field_0x94(0),
      field_0x98(0),
      field_0x9c(-1),
      field_0xa0(-1),
      field_0xa4(0),
      field_0xa8(0),
      field_0xac(0),
      field_0xb0(0),
      field_0xb4(0),
      field_0xb8(0),
      field_0xbc(1),
      field_0xc0(1),
      field_0xc4(0),
      field_0xc8(-1),
      field_0xcc(0),
      field_0xd0(-1),
      field_0xd4(0),
      field_0xd5(0),
      field_0xd8(0),
      field_0xdc(0x18),
      field_0xe0(0),
      field_0xe4(0),
      field_0xe8(0),
      field_0xec(0.0f, 0.0f, 0.0f),
      field_0xf8(0),
      field_0xfc(0),
      mEmitter100(NULL),
      mEmitter104(NULL),
      mLife(4),
      field_0x10c(-1),
      field_0x110(-1),
      field_0x114(0),
      field_0x118(0),
      mAlive(true),
      mDrawEnabled(true),
      field_0x11e(0),
      field_0x11f(0),
      mSpriteParams(),
      mListPrev(NULL),
      mListNext(NULL),
      field_0x164(0),
      mInList(0),
      field_0x168(0),
      field_0x16c(-1),
      field_0x174(0),
      field_0x178(0),
      field_0x17c(0),
      field_0x17d(0),
      field_0x180(20),
      field_0x184(40.0f),
      field_0x188(-1),
      field_0x18c(0),
      field_0x190(0),
      field_0x194(0),
      field_0x195(0),
      field_0x196(0),
      mSerial(-1),
      mSlot(0),
      mActorId('NULL'),
      field_0x1a4(0),
      field_0x1a8(false)
{
    for (int i = 0; i < 4; i++) {
        field_0x1ac[i].set(0.0f, 0.0f, 0.0f);
    }
}
