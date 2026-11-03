#ifndef GAME_ACTORMANAGER_H
#define GAME_ACTORMANAGER_H

// The actor manager: owns every live actor (up to 0x400), spawns them by
// ActorId (fn_801F9484, in game/tu_801F9484.cpp with the actors' inline
// constructors), updates and draws them, and reads EnemyParam.csv for the
// spawner. Its own code is game/tu_801F6058.cpp (vtable 0x804A5800,
// constructor fn_801F9078). One instance, allocated with
// __nw__(0x3060, heap, 0) in game/tu_80139BE8.cpp and
// stored at offset 0x8 of the object lbl_8053AAF8->0x4 points to; reach it
// with fn_801F856C().
//
// "ActorManager_804A5800" is a provisional name carrying its vtable address
// (docs/PLAN.md, "Placeholder names"); the member names are descriptive and
// the evidence for each is beside it. Vectors follow ActorBase.h's rule.
//
// Owner: the shared-header owner (docs/COORDINATOR.md).

#include "types.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JAudio/JAUSoundObject.h"
#include "JSystem/JKernel/JKRDisposer.h"

class ActorBase_804A5770;
class JKRHeap;
class JAISoundHandle;
class JAISoundID; // JSystem/JAudio/Interface/JAISound.h; complete wherever fn_804561D8 is called

// The per-actor sound object (ActorManager_804A5800::mSounds): built by
// fn_804578F4 (game/tu_80455CA4.cpp) from two bases. JAUSoundObject is at 0
// (vtable 0x804B1E28: slots 0x8-0x20 are its own). The constructor builds a
// JKRDisposer at 0x18 and stores vtable + 0x24 there, so slots 0x24/0x28 are
// that base's vtable header, 0x2C (fn_80457C08) is its destructor thunk
// (this - 0x18, then the destructor) and 0x30 is the deleting destructor
// fn_8045787C. fn_80457840 points the object at the actor's position. Layout
// beyond the bases is opaque until that unit is done.
class ActorSoundObj_804578F4 : public JAUSoundObject, public JKRDisposer {
public:
    // 0x804578F4: r4 = position (stored at 0x34, copied to 0x38), r5 = heap,
    // r6 = handle count; r5 and r6 swap places into JAUSoundObject's
    // constructor (pos, count, heap). Callers: `new ... (NULL, NULL, 8)`
    // (act_wave_se fn_802E73B4, BgRewrite's constructor).
    ActorSoundObj_804578F4(JGeometry::TVec3f* pos, JKRHeap* heap, u8 numHandles);
    virtual ~ActorSoundObj_804578F4();
    // 0x804560D0 (game/tu_80455CA4.cpp): vtable slot 0x10, where
    // JAUSoundObject has stopOK.
    virtual bool stopOK(JAISoundHandle& handle);

    // 0x804561D8 (game/tu_80455CA4.cpp, next to the ctor): r3 is this. `id`
    // is passed by value (the callee re-copies it for fn_80456334), `fade`
    // goes unextended into fn_80456334's u32 fade time.
    void fn_804561D8(JAISoundID id, u8 a, u32 fade, u8 c);

    // 0x80456334 (game/tu_80455CA4.cpp): r3 is this, r4 the id (copied to the
    // stack by the caller), r5 a fade time (fn_804561D8 passes its own);
    // returns the handle the sound was started on, or NULL. Called by
    // cls_804B20F8::startSound (game/tu_8045B010.cpp) with 0.
    JAISoundHandle* fn_80456334(JAISoundID id, u32 fade);

    /* 0x30 */ u8 field_0x30[0x2C]; // not read by any caller yet
}; // Size: 0x5C (every __nw__ of it)

#define ACTOR_MANAGER_MAX_ACTORS 0x400

class ActorManager_804A5800 {
public:
    // 0x801F9078; 1 unit (tu_80139BE8 0x8013A5E8, after __nw__(0x3060,
    // heap, 0), passing that heap). Stores the vtable, makes mHeap with
    // JKRUnitHeap::create(...) under `parent` (room for 0x400 actors, 0x200
    // in two UI modes: lbl_80529DEC 0x118/0x11C == 9/3 or 8/2) and clears
    // the tables.
    ActorManager_804A5800(JKRHeap* parent);

    // The vtable's one slot, fn_801F8F6C, is the deleting destructor (it
    // stores 0x804A5800 at 0x0, releases the sound objects and tests the
    // delete flag in r4). Declared so that the fields below sit after the
    // vtable pointer, at the offsets their users load (mActorEnd: lwz
    // 0x1008 in act_log_monitor_npc 0x80357E8C).
    virtual ~ActorManager_804A5800();

    // Non-virtual members other units call: every export of
    // game/tu_801F6058.cpp whose callers pass the manager in r3 (the result
    // of fn_801F856C at all but a few calls; tu_80139BE8 loads it from
    // GameRoot 0x4->0x8, tu_801EE24C from lbl_8053AAF8). "N units / M
    // calls" counts the other game units that call it and their calls.
    // As in ActorBase.h, the names stay dtk's placeholders and symbols.txt
    // carries the mangled name each declaration produces
    // (fn_XXXXXXXX__21ActorManager_804A5800F...), so a parameter type changed
    // here is a symbols.txt rename too; the return type is not mangled.
    // "Serial" is ActorBase_804A5770::mSerial, the handle actors keep to
    // each other; "live" means in mActors[0, mActorEnd) with mAlive set.
    // Queries that take `layer` compare it with ActorBase field_0x4.

    // 2 units / 2 calls. field_0x3030[i] (0x801F6058).
    s32 fn_801F6058(s32 i);
    // 2 units / 8 calls. field_0x305c = value (stb; callers pass li 0/1).
    void fn_801F6204(u8 value);
    // EnemyParam.csv lookup: the value of `column` (ActorParamColumn) for
    // actor `id`, row 0's default when `id` has no row, 0 when the column
    // is missing. 2 units / 8 calls (the spawner, tu_802D39B8).
    s32 fn_801F620C(u32 id, u32 column);
    // 1 unit / 2 calls (ActorBase_804A5770::fn_801F1184 and fn_801F0E34).
    // mSounds[slot], or field_0x200c when mActors[slot]->field_0x4 != 0.
    ActorSoundObj_804578F4* fn_801F6644(s32 slot);
    // 170 units / 709 calls. The live actor with mSerial == serial
    // (cmpw), or NULL (also for serial < 0). Tries mActors[mLastSlot]
    // first, then scans.
    ActorBase_804A5770* fn_801F666C(s32 serial);
    // 4 units / 8 calls. Spawns a Score ('SCRE', 0x801F6720) actor:
    // fn_801F9484(this, ACTOR_SCRE, layer, pos, (c << 24) | (a << 16) | b,
    // -1, -1) (clrlslwi/rlwimi at 0x801F6728-0x801F6738); returns its serial.
    s32 fn_801F6718(u8 a, s32 layer, const JGeometry::TVec3f* pos, u16 b, u8 c);
    // 24 units / 37 calls. fn_80230224(i, 0) for the four players, clears
    // field_0x3041 and calls fn_8037AE30(lbl_8053AD70).
    void fn_801F6884();
    // 24 units / 37 calls. Per-player checks (fn_8023F264, fn_8023ED8C,
    // fn_802300EC, fn_8023E854); sets bit i of field_0x3041 for each player
    // it calls fn_80230AD4 on, and once every taking-part player's bit is
    // set calls fn_8037AE00(lbl_8053AD70) and returns 1, else 0. Callers
    // test r3 with cmpwi (no clrlwi), so not bool.
    s32 fn_801F68E0();
    // 1 unit / 1 call (RailBase 0x800FE50C). Appends every live 'RAIL' actor
    // whose (field_0x90 >> 8) & 0xFF == group to out[*count] (stwx), and
    // returns 0 as soon as *count == max, else 1. RailBase passes a lbz
    // value for group, a 0x31-entry stack array and max 0x31.
    bool fn_801F71E8(u8 group, ActorBase_804A5770** out, s32* count, s32 max);
    // 1 unit / 1 call (act_thief). Writes to out[] every live actor on
    // `layer` other than excludeSerial with mPos.z <= lbl_8053F820 for
    // which fn_801CC9B0 (a 'RUPY' kind test, tu_801CB270) holds; returns
    // the count.
    s32 fn_801F7274(s32 layer, ActorBase_804A5770** out, s32 excludeSerial);
    // 1 unit / 3 calls (tu_801DE714). Like fn_801F7274's filter, but of the
    // actors within `radius` of center (x, y; fmuls f1 by itself) returns
    // the serial of the one nearest to `from` (x, y), or -1.
    s32 fn_801F732C(s32 layer, const JGeometry::TVec3f* center, const JGeometry::TVec3f* from,
                    s32 excludeSerial, f32 radius);
    // 1 unit / 4 calls each (tu_801DE714). The serial of the first live
    // actor on `layer`, other than excludeSerial, for which fn_801CC960
    // ('RUPY' with kind 4-7) / fn_801CC910 ('RUPY' with kind 8-11) holds,
    // or -1.
    s32 fn_801F7464(s32 layer, s32 excludeSerial);
    s32 fn_801F750C(s32 layer, s32 excludeSerial);
    // 1 unit / 1 call (act_all_enemies_defeated_check_switch, with this +
    // 0x23C). 0 when a live actor on `layer` with field_0x1a8 ("dstr")
    // has (x, y) inside box (min inclusive, max exclusive), else 1. The
    // caller tests r3 with clrlwi 24.
    bool fn_801F75B4(s32 layer, const JGeometry::TBox2f* box);
    // 1 unit / 1 call (tu_80184D64, with &mPos). The serial of the live
    // "dstr" actor nearest to pos (x, y) within lbl_8053F824, or -1; the
    // squared distance goes to *outDistSq when it is not NULL.
    s32 fn_801F7664(const JGeometry::TVec3f* pos, f32* outDistSq);
    // 1 unit / 1 call (act_quicksand). The serial of the live `id` actor on
    // `layer`, other than excludeSerial, nearest to pos (x, y), or -1.
    s32 fn_801F76F0(u32 id, s32 layer, const JGeometry::TVec3f* pos, s32 excludeSerial);
    // 3 units / 3 calls. The serial of the live `id` actor nearest to pos
    // (x, y) on any layer, within lbl_8053F824, or -1; the squared distance
    // goes to *outDistSq when it is not NULL (callers pass a stack f32).
    s32 fn_801F778C(u32 id, const JGeometry::TVec3f* pos, f32* outDistSq);
    // 13 units / 16 calls. Writes every live `id` actor on `layer` other
    // than excludeSerial to out[] and returns the count.
    s32 fn_801F7818(u32 id, s32 layer, ActorBase_804A5770** out, s32 excludeSerial);
    // 4 units / 4 calls. The serial of the first live `id` actor on
    // `layer`, or -1.
    s32 fn_801F7884(u32 id, s32 layer);
    // 24 units / 32 calls. Iterates the live `id` actors: the serial of the
    // next one after the actor afterSerial (from the start when
    // afterSerial < 0 or it is gone), or -1 at the end. The slot found is
    // cached in lbl_8053A168 for the next call.
    s32 fn_801F78F4(u32 id, s32 afterSerial);
    // 18 units / 28 calls. The serial of the first live `id` actor, or -1.
    s32 fn_801F7A08(u32 id);
    // 1 unit / 1 call (tu_801EE24C). The number of live actors with
    // field_0x1a8 ("dstr") set.
    s32 fn_801F7A6C();
    // 2 units / 2 calls. Clears mAlive of every live `id` actor other than
    // excludeSerial; returns how many.
    s32 fn_801F7AB8(u32 id, s32 excludeSerial);
    // 1 unit / 5 calls (tu_802D39B8). Resets the view's per-player entries
    // (fn_802D39B8), then clears mAlive of every live actor except the
    // players' (GameView 0x131A8-0x131B4), TZOK, RANK, GBAO, ENDM, DMGA and
    // an RASN whose field_0x230 != 0; then fn_80204638(), fn_802048F4().
    void fn_801F7B18();
    // 1 unit / 2 calls (tu_802D39B8). fn_801F7B18 for layer-0 actors
    // outside the view's box (0xCC-0xD8) only, also sparing FAZR.
    void fn_801F7CAC();
    // 1 unit / 6 calls (tu_802D39B8). Clears mAlive of every live actor on a
    // layer other than 0, and of the layer-0 ones with the byte at 0x194
    // set that are outside the view's box (0xCC-0xD8).
    void fn_801F7E88();
    // 2 units / 2 calls (ActorBase_804A5770::fn_801F26CC, tu_801E9AFC).
    // The serial of the first live actor on actor's layer, other than
    // excludeSerial, whose mActorId is in `ids` ('NULL'-ended) and whose box
    // (fn_801F31C4) overlaps actor's, or -1. Unless the view's flag for the
    // layer is set (GameView 0x131DF + layer), the two z must also be
    // within maxDz (fabs).
    s32 fn_801F7F58(ActorBase_804A5770* actor, const u32* ids, s32 excludeSerial, f32 maxDz);
    // 13 units / 14 calls. Walks the list at mListHead (mListNext): the
    // serial of the first alive actor on actor's layer with field_0xc4
    // clear, within maxDz in z (unless the view's layer flag is set) and
    // whose box overlaps actor's, skipping 'ARIJ' (Antlion) actors unless
    // `includeArij` (clrlwi. 24; callers pass their byte at 0x195); or -1.
    s32 fn_801F82CC(ActorBase_804A5770* actor, bool includeArij, f32 maxDz);
    // Unlink an actor from the manager's list (true if it was linked).
    // 1 unit / 1 call (ActorBase's destructor).
    bool fn_801F8448(ActorBase_804A5770* actor);
    // Link an actor into the manager's list (true if newly linked).
    // 1 unit / 1 call (ActorBase_804A5770::fn_801F2B20).
    bool fn_801F84C4(ActorBase_804A5770* actor);
    // fn_801F9484 with arg_0x16c = arg_0x110 = -1. 32 units / 43 calls.
    s32 fn_801F8544(u32 id, s32 arg_0x4, const JGeometry::TVec3f* pos, u32 arg_0x90);
    // 1 unit / 1 call each (GameRoot, tu_80139BE8), with GameRoot
    // 0x4->0x8:
    // fn_801F857C deletes every actor (virtual destructor, delete flag 1)
    // and releases its sound object (fn_80456120), then field_0x200c's.
    void fn_801F857C();
    // The draw pass: vfunc_0x20 of every live actor with mDrawEnabled
    // (only those with field_0x11f while paused, fn_80134CC8), each after
    // fn_801A3CFC; then fn_801E9AFC(), fn_801E9C74() unless paused.
    void fn_801F8658();
    // The update pass (0x730 bytes).
    void fn_801F871C();
    // fn_801F857C, then resets the spawn state: mSpawnCount = 0, mLastSlot
    // = mActorEnd = -1, mListHead = NULL, field_0x305c = field_0x3041 = 0.
    void fn_801F8E4C();
    // The spawner (game/tu_801F9484.cpp). Allocates the actor for `id`
    // from mHeap, places it in the first free slot, sets its spawn fields
    // (ActorBase_804A5770's inline setters), calls init() and returns its
    // serial, or -1 when `id` is unknown or no slot is free. `pos` is
    // copied to mPos, mPrevPos and mSpawnPos. arg_0x4 goes to field_0x4
    // (the layer) and field_0x8, arg_0x90 to field_0x90 (u32 there),
    // arg_0x16c to field_0x16c, arg_0x110 to field_0x110. 145 units / 521
    // calls.
    s32 fn_801F9484(u32 id, s32 arg_0x4, const JGeometry::TVec3f* pos, u32 arg_0x90,
                    s32 arg_0x16c, s32 arg_0x110);

    /* 0x0000 vtable (0x804A5800) */
    /* 0x0004 */ JKRHeap* mHeap; // JKRUnitHeap::create(...) in the
                                 // constructor; every spawn allocates here
    /* 0x0008 */ ActorBase_804A5770* mActors[ACTOR_MANAGER_MAX_ACTORS];
    /* 0x1008 */ s32 mActorEnd;  // one past the highest used slot
    /* 0x100C */ ActorSoundObj_804578F4* mSounds[ACTOR_MANAGER_MAX_ACTORS]; // per slot
    /* 0x200C */ ActorSoundObj_804578F4* field_0x200c; // shared by actors
                                                       // with field_0x4 != 0
    /* 0x2010 */ s32 mSpawnCount; // ActorBase_804A5770::mSerial of the next spawn
    /* 0x2014 */ s32 mLastSlot;   // slot of the last spawn
    /* 0x2018 */ ActorBase_804A5770* mListHead; // through ActorBase_804A5770::mListPrev/Next
                                       // (fn_801F84C4 links, fn_801F8448 unlinks)
    /* 0x201C */ s32 field_0x201c[0x400]; // -1 at the start of each update
    /* 0x301C */ s32 field_0x301c;
    /* 0x3020 */ s32 field_0x3020[4]; // fn_801F6068: per player, sorted
    /* 0x3030 */ s32 field_0x3030[4]; // fn_801F6068: player index per entry
    /* 0x3040 */ u8 field_0x3040;
    /* 0x3041 */ u8 field_0x3041; // bit per player (fn_801F68E0 sets,
                                  // fn_801F6884 and fn_801F8E4C clear)
    // EnemyParam.csv, read by fn_801F620C(id, column) (0 if missing):
    /* 0x3044 */ s32 mParamColumnNum;
    /* 0x3048 */ s32 mParamRowNum;
    /* 0x304C */ u32* mParamRowIds;    // ActorId per row
    /* 0x3050 */ void* field_0x3050;
    /* 0x3054 */ u32* mParamColumnIds; // 4-character column keys ('life', ...)
    /* 0x3058 */ s32* mParamValues;    // [row * mParamColumnNum + column];
                                       // row 0 holds the defaults
    /* 0x305C */ u8 field_0x305c;      // set by fn_801F6204
}; // Size: 0x3060

// EnemyParam.csv column keys the spawner reads (fn_801F620C's 2nd argument),
// with the ActorBase_804A5770 field each one sets.
enum ActorParamColumn {
    ACTOR_PARAM_LIFE = 'life', // ActorBase_804A5770::mLife
    ACTOR_PARAM_ATCP = 'atcp', // field_0x114
    ACTOR_PARAM_ATCN = 'atcn', // field_0x118
    ACTOR_PARAM_DSTR = 'dstr', // field_0x1a8 (as != 0)
    ACTOR_PARAM_DASU = 'dasu', // field_0x18c
    ACTOR_PARAM_EFSZ = 'efsz', // field_0xc0
    ACTOR_PARAM_VOIC = 'voic', // field_0x190
};

// Exports of game/tu_801F6058.cpp that are not manager members (their
// callers do not pass the manager in r3).
//
// Defined in game/tu_801F6058.cpp (C++ linkage, renamed in symbols.txt).
// 241 units / 1662 calls. lbl_8053AAF8->0x4->0x8: the actor manager.
ActorManager_804A5800* fn_801F856C(void);

// 29 units / 49 calls. The direction (0-3) of a 16-bit angle: 2 below
// 0x2000 or from 0xE000, 0 below 0x6000, 3 up to 0xA000, else 1. Callers
// pass a clrlwi 16 or lhz value.
s32 fn_801F681C(u16 angle);

// 23 units / 43 calls. The angle of direction dir: lbl_8053F800[dir & 3]
// (lhzx), fn_801F681C's inverse.
u16 fn_801F6874(s32 dir);

// 9 units / 13 calls. Moves *value toward target by (s16)(target -
// *value) / step, at most maxStep either way (lhz/sth; callers pass a
// field address, an lhz target, 1 and 0x1000 at tu_80343A70).
void fn_801F714C(u16* value, u16 target, u16 step, u16 maxStep);

// 79 units / 361 calls. Unless *value == target, adds rate * (target -
// *value), clamped to +-maxStep, to *value.
void fn_801F71A4(f32* value, f32 target, f32 rate, f32 maxStep);

// Not written yet in game/tu_801F6058.cpp: extern "C" keeps their dtk
// placeholder names as the link names until that unit defines them.
extern "C" {

// 21 units / 38 calls. True when pos (x, y) is outside the view's box
// (fn_802D6A9C 0xCC, or 0xDC for layer 8 and up) grown by `margin` on
// each side (<= / >= compares). Callers pass &mPos, field_0x4 and a
// literal, and test r3 with clrlwi. 24.
bool fn_801F6758(const JGeometry::TVec3f* pos, s32 layer, f32 margin);

// 6 units / 12 calls. Gets the manager itself (lbl_8053AAF8->0x4->0x8)
// and does fn_801F7F58's search for the single id `id` (a stack
// {id, 'NULL'} list): the serial of an actor overlapping `actor` on its
// layer, other than excludeSerial, or -1. Callers pass the actor in r3
// (ActorBase_804A5770::fn_801F2718 passes this, its mSerial and
// lbl_8053F748; others pass -1 for excludeSerial).
s32 fn_801F8108(ActorBase_804A5770* actor, u32 id, s32 excludeSerial, f32 maxDz);

} // extern "C"

// Force Gem ('RUPY', act_force_gem) tests, defined in game/tu_801CB270.cpp:
// actor->mActorId == 'RUPY' and its kind (the low byte of field_0x90 while
// field_0x94 is 0 or 3, else -1) is in 8-11 / 4-7 / 0 and up. Callers pass
// an actor (fn_801F666C's result, or mActors[i] in fn_801F7274, fn_801F732C,
// fn_801F7464, fn_801F750C) and test r3 with clrlwi 24. Whether the pointer
// is const is not visible.
bool fn_801CC910(ActorBase_804A5770* actor);
bool fn_801CC960(ActorBase_804A5770* actor);
bool fn_801CC9B0(ActorBase_804A5770* actor);

#endif
