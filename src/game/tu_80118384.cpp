#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/act/tu_8010DF44.h"

namespace JGadget {
namespace vector {
// JSystem/JGadget/std-vector.cpp (vector.h needs the MSL C++ headers game code lacks).
u32 extend_default(u32, u32, u32);
} // namespace vector
} // namespace JGadget

// The 1-byte .sbss object at 0x8053AA90 fn_8011921C copies into the list's allocator.
static JGadget::TAllocator<ActorBase_804A5770*> lbl_8053AA90;

// The texture name fn_80119D48 loads (.sdata 0x80539F10).
const char* lbl_80539F10 = "kk_black.bti";

inline cls_80119E5C::cls_80119E5C() {
    field_0x0 = 0;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    field_0x10 = 0;
    field_0x14 = 0.0f;
    field_0x18 = 0.0f;
    field_0x1c = 0;
    field_0x20 = 0;
    field_0x24 = 0;
    field_0x28 = 0;
    field_0x2c = NULL;
}

// The accessor of the fade object; fn_80119DD0 is its out-of-line copy (callers elsewhere).
static inline cls_80119E5C* fadeObject() {
    static cls_80119E5C instance;
    return &instance;
}

// The fade length for a mode: 0x23 for modes 0 and 1, 0x46 otherwise.
static inline s32 fadeFrames(s32 mode) {
    switch (mode) {
    case 0:
    case 1:
        return 0x23;
    case 2:
    default:
        return 0x46;
    }
}

cls_80119E5C::~cls_80119E5C() {
    if (field_0x2c != NULL) {
        delete field_0x2c;
    }
    field_0x2c = NULL;
}

cls_80119E5C* fn_80119DD0() {
    return fadeObject();
}

// FIXME: 96.91 %, missing/extra; behaviour clean.
// Regalloc: the original keeps `this` in r30 and the picture in r29, the texture
// in r31; here the constructor result takes an extra move through r0.
void cls_80119E5C::fn_80119D48() {
    if (field_0x2c == NULL) {
        void* texture = gGameRoot_8053AAF8->field_0xa4->getResource('????', lbl_80539F10);
        field_0x2c = new J2DPicture((ResTIMG*)texture);
    }
}

// FIXME: 99.62 %, reloc-name; behaviour clean.
void cls_80119E5C::fn_80119998(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    field_0x0 = 1;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x4 = 0;
    field_0x8 = a;
    field_0xc = b;
    field_0x1c = c;
    field_0x20 = d;
    field_0x24 = e;
    field_0x28 = g;
    field_0x10 = f;
    if (field_0x10 > 0) {
        field_0x2 = 1;
    } else {
        field_0x2 = 0;
    }
    if (field_0x24 == 2) {
        field_0x8 = fadeFrames(c);
    }
    switch (field_0x1c) {
    case 0:
        if (field_0x24 == 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EC, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3ED, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EE, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EF, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x14 = 255.0f;
        field_0x18 = 255.0f / (f32)(s32)(field_0x8 - 1);
        break;
    case 1:
        if (field_0x24 == 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F0, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F1, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F2, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F3, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x14 = 0.0f;
        field_0x18 = 255.0f / (f32)(s32)(field_0x8 - 1);
        break;
    case 2:
        if (field_0x24 == 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F0, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F1, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F2, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3F3, 0, 0, NULL, NULL, true, 1.0f);
        }
        field_0x14 = 0.0f;
        field_0x18 = 510.0f / (f32)(s32)(field_0x8 - 1);
        break;
    }
}

void cls_80119E5C::fn_80119958() {
    field_0x0 = 0;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    field_0x14 = 0.0f;
    field_0x18 = 0.0f;
    field_0x1c = 0;
    field_0x20 = 0;
    field_0x24 = 0;
    field_0x28 = 0;
    field_0x10 = 0;
}

// FIXME: 99.88 %, reloc-name; behaviour clean.
// reloc-name only: the unit's .sdata2 pool still lacks 0.05f (fn_80118E20,
// unwritten) and has 1.0f after the double; the original has a 1.0f made before
// fn_801194C4's double (no function here explains it).
// The fade step: field_0x4 counts the frames up to field_0x8 while field_0x14 moves
// by field_0x18 (down for mode 0, up for 1, up then down for 2) within 0..255; at
// the end the fade holds while field_0x10 counts down (when field_0x2 is set), then
// resets. Not during game modes 9 and 0x16.
void cls_80119E5C::fn_80119628() {
    switch (gGameRoot_8053AAF8->field_0x4c) {
    case 9:
    case 0x16:
        return;
    }
    if (field_0x0 == 0) {
        return;
    }
    if (field_0x1 != 0) {
        if (field_0x2 != 0) {
            field_0x10--;
            if (field_0x10 <= 0) {
                field_0x1 = 0;
            }
        }
        return;
    }
    field_0x4++;
    switch (field_0x1c) {
    case 0:
        if (field_0x2 != 0 && field_0x10 > 0) {
            field_0x1 = 1;
        }
        field_0x14 -= field_0x18;
        break;
    case 1:
        field_0x14 += field_0x18;
        break;
    case 2:
        if (field_0x24 == 2 && field_0x4 == field_0x8 / 2) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EC, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3ED, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EE, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, &pos, 0x3EF, 0, 0, NULL, NULL, true, 1.0f);
        }
        if (field_0x4 >= field_0x8 / 2 + 1) {
            if (field_0x2 != 0 && field_0x10 > 0) {
                field_0x1 = 1;
            } else {
                field_0x14 -= field_0x18;
            }
        } else {
            field_0x14 += field_0x18;
        }
        break;
    }
    if (field_0x14 >= 255.0f) {
        field_0x14 = 255.0f;
    } else if (field_0x14 <= 0.0f) {
        field_0x14 = 0.0f;
    }
    if (field_0x4 > field_0x8) {
        if (field_0x2 != 0) {
            if (field_0x10 <= 0) {
                fn_80119958();
            } else {
                field_0x1 = 1;
            }
        } else {
            fn_80119958();
        }
    }
}

// FIXME: 99.94 %, reloc-name; behaviour clean.
// reloc-name only (the double of the u16 to float conversions; see
// fn_80119628).
// Draws the fade picture over the screen with alpha field_0x14: black, or white
// for field_0x24 == 1 (not for field_0x24 == 2).
void cls_80119E5C::fn_801194C4() {
    if (field_0x0 != 0 && field_0x24 != 2 && field_0x14 != 0.0f) {
        gGameRoot2_8053AB10->fn_8013B490();
        switch (field_0x24) {
        case 1:
            field_0x2c->setBlackWhite(JUtility::TColor(0xFFFFFFFF), JUtility::TColor(0));
            break;
        case 0: // the original's switch also has case 0 (bge / b both to the black one)
        default:
            field_0x2c->setBlackWhite(JUtility::TColor(0xFF), JUtility::TColor(0));
            break;
        }
        field_0x2c->setAlpha(field_0x14);
        field_0x2c->draw(0.0f, 0.0f, (s32)JUTVideo::sManager->getFbWidth(),
                         (s32)JUTVideo::sManager->getEfbHeight(), false, false, false);
    }
}

void cls_80119E5C::fn_80119468(s32 g, s32 mode) {
    fn_80119998(fadeFrames(mode), 0, mode, 0, 2, 0, g);
}

// FIXME(shotgun): not written: the JGadget TVector insert machinery fn_801188B8 (push_back) /
// fn_801188E0 / fn_80118938 / fn_801189B0 (Insert_raw) and fn_8011888C (calls them). They need a
// typed TVector member at 0xC, and act_princess_zelda_demo / tu_8011CEC8 read mBegin / mEnd.
inline cls_801192AC::cls_801192AC() {
    field_0x0 = 0;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x4 = 0x13;
    field_0x8 = 0;
    mAllocator = lbl_8053AA90;
    mBegin = NULL;
    mEnd = NULL;
    mCapacity = 0;
    mExtend = JGadget::vector::extend_default;
}

// JGadget::TVector::erase(first, last): copies the tail down, then destroys the rest.
static inline ActorBase_804A5770** copyDown(ActorBase_804A5770** first, ActorBase_804A5770** last,
                                            ActorBase_804A5770** result) {
    for (; first < last; ++first, ++result) {
        *result = *first;
    }
    return result;
}

static inline void eraseRange(ActorBase_804A5770**& end, ActorBase_804A5770** first,
                              ActorBase_804A5770** last) {
    ActorBase_804A5770** newEnd = copyDown(last, end, first);
    for (ActorBase_804A5770** it = newEnd; it != end; it++) {
    }
    end = newEnd;
}

// FIXME: 97.48 %, missing/extra; behaviour near.
cls_801192AC::~cls_801192AC() {
    eraseRange(mEnd, mBegin, mEnd);
    eraseRange(mEnd, mBegin, mEnd);
    delete mBegin;
}

// FIXME(shotgun): 82.22 %, missing/extra + reloc-name; behaviour drift.
extern "C" cls_801192AC* fn_8011921C() {
    static cls_801192AC lbl_804BC540;
    return &lbl_804BC540;
}

// JGadget::TVector<T>::TDestructed_deallocate_: frees its pointer through the allocator on scope exit.
struct ActorVectorDealloc {
    ActorVectorDealloc(JGadget::TAllocator<ActorBase_804A5770*>& alloc, ActorBase_804A5770** pointer) {
        mAllocator = &alloc;
        mPointer = pointer;
    }
    ~ActorVectorDealloc() { mAllocator->deallocate(mPointer, 0); }
    void set(ActorBase_804A5770** p) { mPointer = p; }
    JGadget::TAllocator<ActorBase_804A5770*>* mAllocator;
    ActorBase_804A5770** mPointer;
};

// FIXME: 97.56 %, regalloc-only; behaviour clean.
// FIXME(shotgun): TVector::reserve(20) inline; copy loop and empty destroy loop.
void cls_801192AC::fn_801190E4() {
    if (mCapacity >= 20) {
        return;
    }
    ActorBase_804A5770** p = (ActorBase_804A5770**)operator new(20 * sizeof(ActorBase_804A5770*));
    if (p == NULL) {
        return;
    }
    ActorVectorDealloc guard(mAllocator, p);
    copyDown(mBegin, mEnd, p);
    ActorBase_804A5770** old = mBegin;
    for (ActorBase_804A5770** it = mBegin; it != mEnd; ++it) {
    }
    guard.set(old);
    mEnd = p + (mEnd - mBegin);
    mBegin = p;
    mCapacity = 20;
}

// FIXME(shotgun): 97.68 %, different; behaviour drift.
// The per-frame pass: clears the four player bits of lbl_8053AED0's 0xDD; with field_0x2
// raises the flag at field_0x30's 0x284 (0.05f at 0x28C); unless paused (field_0x1) runs each
// listed actor's vfunc_0x54 and, when all their vfunc_0x5C say done and field_0x4 < 0xE,
// resets (fn_801184F0 with the fade); counts field_0x8 up.
void cls_801192AC::fn_80118E20() {
    if (field_0x0 == 0) {
        return;
    }
    lbl_8053AED0->field_0xdd &= ~1;
    lbl_8053AED0->field_0xdd &= ~2;
    lbl_8053AED0->field_0xdd &= ~4;
    lbl_8053AED0->field_0xdd &= ~8;
    if (field_0x2 != 0) {
        cls_804A03A8* p = gGameRoot2_8053AB10->field_0x30;
        if (p->field_0x284 == 0) {
            p->field_0x284 = 1;
            p->field_0x28c = 0.05f;
        }
    }
    if (field_0x1 != 0) {
        return;
    }
    bool done = true;
    for (ActorBase_804A5770** it = mBegin; it != mEnd; it++) {
        cls_8049D140* actor = static_cast<cls_8049D140*>(*it);
        if (actor != NULL) {
            actor->vfunc_0x54();
            if (!actor->vfunc_0x5C()) {
                done = false;
            }
        }
    }
    if (done && field_0x4 < 0xE) {
        fn_801184F0(true);
    }
    field_0x8++;
}

// The JGadget TVector of actors at cls_801192AC+0xC.
struct ActorVector {
    JGadget::TAllocator<ActorBase_804A5770*> mAllocator;
    ActorBase_804A5770** mBegin;
    ActorBase_804A5770** mEnd;
    u32 mCapacity;
    u32 (*mExtend)(u32, u32, u32);
    ActorBase_804A5770** Insert_raw(ActorBase_804A5770** pos, u32 count);
    void fn_80118938(ActorBase_804A5770** pos, u32 count, ActorBase_804A5770* const* value);
};

// stand-ins for std::copy / std::copy_backward (the MSL <algorithm> is not usable from game code)
static inline ActorBase_804A5770** copyForward(ActorBase_804A5770** first, ActorBase_804A5770** last,
                                               ActorBase_804A5770** result) {
    for (; first < last; ++first, ++result) {
        *result = *first;
    }
    return result;
}

static inline ActorBase_804A5770** copyBackward(ActorBase_804A5770** first, ActorBase_804A5770** last,
                                                ActorBase_804A5770** result) {
    while (last > first) {
        *--result = *--last;
    }
    return result;
}

// FIXME: 95.09 %, missing/extra; behaviour clean.
ActorBase_804A5770** ActorVector::Insert_raw(ActorBase_804A5770** pos, u32 count) {
    if (count == 0) {
        return pos;
    }
    u32 size = mBegin == NULL ? 0 : mEnd - mBegin;
    if (count + size <= mCapacity) {
        ActorBase_804A5770** newEnd = pos + count;
        if (newEnd < mEnd) {
            ActorBase_804A5770** tail = mEnd - count;
            copyForward(tail, mEnd, mEnd);
            copyBackward(pos, tail, mEnd);
            for (ActorBase_804A5770** it = pos; it != newEnd; it++) {
            }
            mEnd += count;
            return pos;
        }
        copyForward(pos, mEnd, newEnd);
        for (ActorBase_804A5770** it = pos; it != mEnd; it++) {
        }
        mEnd += count;
        return pos;
    }
    u32 oldSize = mBegin == NULL ? 0 : mEnd - mBegin;
    u32 needed = oldSize + count;
    u32 extended = mExtend(mCapacity, oldSize, count);
    u32 newSize = needed > extended ? needed : extended;
    ActorBase_804A5770** data = mAllocator.allocate(newSize, 0);
    if (data == NULL) {
        return mEnd;
    }
    ActorVectorDealloc guard(mAllocator, data);
    ActorBase_804A5770** gap = copyForward(mBegin, pos, data);
    copyForward(pos, mEnd, gap + count);
    for (ActorBase_804A5770** it = mBegin; it != mEnd; it++) {
    }
    guard.set(mBegin);
    mEnd = data + (count + (mEnd - mBegin));
    mBegin = data;
    mCapacity = newSize;
    return gap;
}

// fill-insert: opens a gap with Insert_raw and constructs count copies of *value in it
void ActorVector::fn_80118938(ActorBase_804A5770** pos, u32 count, ActorBase_804A5770* const* value) {
    if (count != 0) {
        ActorBase_804A5770** p = Insert_raw(pos, count);
        if (p != mEnd) {
            for (u32 i = count; i != 0; i--, p++) {
                if (p != NULL) {
                    *p = *value;
                }
            }
        }
    }
}

void cls_801192AC::fn_8011888C(ActorBase_804A5770* actor) {
    fn_801188B8(&mAllocator, &actor);
}

// by-value predicate of the std::find_if the original uses: matches an actor by its serial
struct ActorSerialEq {
    s32 mSerial;
    bool operator()(ActorBase_804A5770* actor) const { return mSerial == actor->mSerial; }
};

// FIXME: 90.25 %, missing/extra; behaviour near.
// FIXME: erase of the found element (TVector::erase inline); still differs in the stack frame.
void cls_801192AC::fn_80118798(ActorBase_804A5770* actor) {
    ActorSerialEq pred = {actor->mSerial};
    ActorBase_804A5770** end = mEnd;
    ActorBase_804A5770** it = mBegin;
    while (it != end && !pred(*it)) {
        it++;
    }
    if (it != end) {
        ActorBase_804A5770** next = it + 1;
        while (next < end) {
            *it++ = *next++;
        }
        for (ActorBase_804A5770** p = it; p != mEnd; p++) {
        }
        mEnd = it;
    }
}

// FIXME(shotgun): 54.97 %, missing/extra; behaviour clean.
// The original is TVector size() == 0, then std::find_if with a
// by-value predicate (its actorId is spilled to the stack); same stand-in note as fn_80118798.
ActorBase_804A5770** cls_801192AC::fn_8011871C(ActorBase_804A5770** from, u32 actorId) {
    u32 count;
    if (mBegin == NULL) {
        count = 0;
    } else {
        count = mEnd - mBegin;
    }
    if (count == 0) {
        return mEnd;
    }
    if (from == mEnd) {
        return mEnd;
    }
    while (from != mEnd) {
        if ((*from)->mActorId == actorId) {
            return from;
        }
        from++;
    }
    return mEnd;
}

bool cls_801192AC::fn_801186B0(u32 value) {
    if (getActorManager_801F856C()->fn_801F68E0()) {
        field_0x0 = 1;
        field_0x1 = 0;
        field_0x2 = 0;
        field_0x4 = value;
        field_0x8 = 0;
        return true;
    }
    return false;
}

// FIXME(shotgun): 97.59 %, different; behaviour drift.
// (the vector clear is a plain assignment; the
// erase(begin(), end()) inline: the copy and empty destroy loops; the extra copy of the source pointer differs)
bool cls_801192AC::fn_801184F0(bool flag) {
    getActorManager_801F856C()->fn_801F6884();
    field_0x0 = 0;
    if (flag) {
        fadeObject()->fn_80119958();
    }
    ActorBase_804A5770** first = mBegin;
    ActorBase_804A5770** end = mEnd;
    for (ActorBase_804A5770** last = end; last < end; last++) {
        *first++ = *last;
    }
    for (ActorBase_804A5770** it = first; it != end; it++) {
    }
    mEnd = first;
    field_0x0 = 0;
    field_0x1 = 0;
    field_0x2 = 0;
    field_0x4 = 0x13;
    field_0x8 = 0;
    return true;
}

void cls_801192AC::fn_80118454() {
    field_0x1 = 1;
    fadeObject()->field_0x1 = 1;
}

void cls_801192AC::fn_801183BC() {
    field_0x1 = 0;
    fadeObject()->field_0x1 = 0;
}

void cls_801192AC::fn_80118384() {
    getActorManager_801F856C()->fn_801F6884();
    field_0x0 = 0;
}
