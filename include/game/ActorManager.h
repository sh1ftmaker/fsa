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

class ActorBase_804A5770;
class JKRHeap;

// The per-actor sound object (ActorManager_804A5800::mSounds): built by fn_804578F4
// on top of JAUSoundObject (game/tu_80455CA4.cpp); fn_80457840 points it at
// the actor's position. Opaque here until that unit is done.
class ActorSoundObj_804578F4;

#define ACTOR_MANAGER_MAX_ACTORS 0x400

class ActorManager_804A5800 {
public:
    // The vtable's one slot, fn_801F8F6C, is the deleting destructor (it
    // stores 0x804A5800 at 0x0, releases the sound objects and tests the
    // delete flag in r4). Declared so that the fields below sit after the
    // vtable pointer, at the offsets their users load (mActorEnd: lwz
    // 0x1008 in act_log_monitor_npc 0x80357E8C); tu_801F6058 renames
    // fn_801F8F6C when it defines it.
    virtual ~ActorManager_804A5800();

    /* 0x0000 vtable (0x804A5800) */
    /* 0x0004 */ JKRHeap* mHeap; // fn_80085A10(...), sized for 0x400 or
                                 // 0x200 actors; every spawn allocates here
    /* 0x0008 */ ActorBase_804A5770* mActors[ACTOR_MANAGER_MAX_ACTORS];
    /* 0x1008 */ s32 mActorEnd;  // one past the highest used slot
    /* 0x100C */ ActorSoundObj_804578F4* mSounds[ACTOR_MANAGER_MAX_ACTORS]; // per slot
    /* 0x200C */ ActorSoundObj_804578F4* field_0x200c;
    /* 0x2010 */ s32 mSpawnCount; // ActorBase_804A5770::mSerial of the next spawn
    /* 0x2014 */ s32 mLastSlot;   // slot of the last spawn
    /* 0x2018 */ ActorBase_804A5770* mListHead; // through ActorBase_804A5770::mListPrev/Next
                                       // (fn_801F84C4 links, fn_801F8448 unlinks)
    /* 0x201C */ s32 field_0x201c[0x400]; // -1 at the start of each update
    /* 0x301C */ s32 field_0x301c;
    /* 0x3020 */ s32 field_0x3020[4]; // fn_801F6068: per player, sorted
    /* 0x3030 */ s32 field_0x3030[4]; // fn_801F6068: player index per entry
    /* 0x3040 */ u8 field_0x3040;
    // EnemyParam.csv, read by fn_801F620C(this, id, column) (0 if missing):
    /* 0x3044 */ s32 mParamColumnNum;
    /* 0x3048 */ s32 mParamRowNum;
    /* 0x304C */ u32* mParamRowIds;    // ActorId per row
    /* 0x3050 */ void* field_0x3050;
    /* 0x3054 */ u32* mParamColumnIds; // 4-character column keys ('life', ...)
    /* 0x3058 */ s32* mParamValues;    // [row * mParamColumnNum + column];
                                       // row 0 holds the defaults
    /* 0x305C */ u8 field_0x305c;      // set by fn_801F6204
}; // Size: 0x3060

// EnemyParam.csv column keys the spawner reads (fn_801F620C's 3rd argument),
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

// Unnamed functions every actor file calls. extern "C" keeps their dtk
// placeholder names as the link names, so relocations match until their own
// units name them; rename here and in symbols.txt together.
extern "C" {

// lbl_8053AAF8->0x4->0x8: the actor manager.
ActorManager_804A5800* fn_801F856C(void);

// The spawner. Allocates the actor for `id` from mHeap, places it in the
// first free slot, sets its spawn fields (ActorBase_804A5770's inline setters), calls
// init() and returns its serial (ActorBase_804A5770::mSerial), or -1 when `id` is
// unknown or no slot is free. `pos` is copied to mPos, mPrevPos and
// mSpawnPos. arg_0x4 goes to field_0x4 and field_0x8, arg_0x90 to
// field_0x90, arg_0x16c to field_0x16c, arg_0x110 to field_0x110.
s32 fn_801F9484(ActorManager_804A5800* mgr, u32 id, s32 arg_0x4, const JGeometry::TVec3f* pos,
                s32 arg_0x90, s32 arg_0x16c, s32 arg_0x110);

// The same with arg_0x16c = arg_0x110 = -1.
s32 fn_801F8544(ActorManager_804A5800* mgr, u32 id, s32 arg_0x4, const JGeometry::TVec3f* pos,
                s32 arg_0x90);

// EnemyParam.csv lookup: the value of `column` for actor `id`.
s32 fn_801F620C(ActorManager_804A5800* mgr, u32 id, u32 column);

// Unlink an actor from the manager's list (true if it was linked).
bool fn_801F8448(ActorManager_804A5800* mgr, ActorBase_804A5770* actor);

// Link an actor into the manager's list (true if newly linked).
bool fn_801F84C4(ActorManager_804A5800* mgr, ActorBase_804A5770* actor);

} // extern "C"

#endif
