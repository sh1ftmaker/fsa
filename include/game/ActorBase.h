#ifndef GAME_ACTORBASE_H
#define GAME_ACTORBASE_H

// The base class of every actor the spawner creates (fn_801F9484, see
// game/ActorManager.h). Its code is game/tu_801EE24C.cpp; its vtable is at
// 0x804A5770 (__vt__9ActorBase).
//
// "ActorBase" is a placeholder: FSA has no RTTI and no symbol map, and no
// string names the class. Rename it (and its mangled symbols) only on
// evidence.
//
// Layout: from the constructor (0x801F5AE0), the spawner's inline setters
// (0x801FF630-0x801FF738), the actor manager's loops (game/tu_801F6058.cpp)
// and the destructor (0x801F593C). sizeof == 0x230: the smallest
// __nw__(size, heap, 0) in the spawner is 0x230, for 11 actors whose inline
// constructors only call this one and store their own vtable (RAIL, STGN,
// ENDR, ...).
//
// Owner: the shared-header owner (docs/COORDINATOR.md, "Shared game
// headers"). Game-file agents propose changes as SHARED-EDITS.

#include "types.h"
#include "JSystem/JGeometry.h"

struct JPABaseEmitter;

// Axis-aligned 2D box in the actor's top-down space: (x, y - z) of a point.
// fn_801F5790 checks max > min on both axes and offsets it by the actor's
// position; the spawner copies the whole box with one struct assignment
// (fn_801FF630). Member names are descriptive placeholders.
struct ActorBox {
    ActorBox() {}
    ActorBox(f32 x0, f32 y0, f32 x1, f32 y1) : minX(x0), minY(y0), maxX(x1), maxY(y1) {}

    /* 0x0 */ f32 minX;
    /* 0x4 */ f32 minY;
    /* 0x8 */ f32 maxX;
    /* 0xC */ f32 maxY;
}; // Size: 0x10

// The member at 0x1DC: constructor fn_801EAD68, per-frame update
// fn_801EAB0C (both in game/tu_801EAB0C.cpp), called by the actor manager's
// update loop before the actor's own update. Its purpose is unknown.
struct ActorSub1DC {
    ActorSub1DC();
    /* 0x00 */ u8 field_0x0[4];
    /* 0x04 */ u8 field_0x4[16];
    /* 0x14 */ s32 field_0x14[16]; // -1 after construction
}; // Size: 0x54

// The member at 0x120. Its constructor initializes 0x130 and 0x134 and then
// again after filling the table (the base constructor's stores show both),
// hence the separate reset.
struct ActorSub120 {
    ActorSub120() : field_0x10(-1), field_0x14(-1) { reset(); }
    void reset() {
        for (u8 i = 0; i < 16; i++) {
            field_0x0[i] = i;
        }
        field_0x10 = -1;
        field_0x14 = 0;
    }

    /* 0x00 */ u8 field_0x0[16]; // 0, 1, ..., 15
    /* 0x10 */ s32 field_0x10;
    /* 0x14 */ s32 field_0x14;
}; // Size: 0x18

class ActorBase {
public:
    ActorBase();

    // Virtual table at 0x804A5770. Slot offsets are into the vtable (MWCC:
    // two header words, the first slot at +0x8). Names only where the
    // caller makes the role unambiguous.
    /* 0x08 */ virtual ~ActorBase();
    /* 0x0C */ virtual void init();   // called once by the spawner after the
                                      // spawn parameters are set; empty here
    /* 0x10 */ virtual void vfunc_0x10(); // empty here
    /* 0x14 */ virtual void update(); // the manager's update loop, every frame
                                      // the actor is active; empty here
    /* 0x18 */ virtual void vfunc_0x18(); // fn_801F5790: box vs. player screens
    /* 0x1C */ virtual void savePrevPos(); // mPrevPos = mPos; the update loop
                                           // calls it right before update()
    /* 0x20 */ virtual void draw();   // the manager's draw loop (fn_801F8658)
                                      // while mDrawEnabled; empty here
    /* 0x24 */ virtual void vfunc_0x24(); // fn_801F4F28
    /* 0x28 */ virtual void vfunc_0x28(); // fn_801F4E0C
    /* 0x2C */ virtual void vfunc_0x2C(); // fn_801F4850
    /* 0x30 */ virtual void vfunc_0x30(); // fn_801F47CC
    /* 0x34 */ virtual void vfunc_0x34(); // fn_801F4760
    /* 0x38 */ virtual void vfunc_0x38(); // fn_801F471C
    /* 0x3C */ virtual void vfunc_0x3C(); // fn_801F4710
    /* 0x40 */ virtual void vfunc_0x40(); // fn_801F470C
    /* 0x44 */ virtual void vfunc_0x44(); // fn_801F4708
    /* 0x48 */ virtual void vfunc_0x48(); // fn_801F4704
    /* 0x4C */ virtual void vfunc_0x4C(); // fn_801F15BC
    /* 0x50 */ virtual void vfunc_0x50(); // fn_801F165C

    // Inline accessors. The spawner calls them out of line (it is too large
    // for the inliner), so they exist as weak functions in its unit at
    // 0x801FF630-0x801FF738; the offset each one touches is in its name
    // when the field is unnamed.
    JGeometry::TVec3f* getPos() { return &mPos; }
    void setPos(const JGeometry::TVec3f& pos) {
        mPos = pos;
        mPrevPos = pos;
    }
    void setSpawnPos(const JGeometry::TVec3f& pos) { mSpawnPos = pos; }
    ActorBox* getBox() { return &mBox; }
    ActorBox* getBox2() { return &mBox2; }
    void setBox2(const ActorBox& box) { mBox2 = box; }
    void setActorId(u32 id) { mActorId = id; }
    void setSerial(s32 serial) { mSerial = serial; }
    void setSlot(s32 slot) { mSlot = slot; }
    void setLife(s32 life) { mLife = life; }
    void setField_0x4(s32 v) { field_0x4 = v; }
    void setField_0x8(s32 v) { field_0x8 = v; }
    void setField_0x90(s32 v) { field_0x90 = v; }
    void setField_0xC0(s32 v) { field_0xc0 = v; }
    void setField_0x110(s32 v) { field_0x110 = v; }
    void setField_0x114(s32 v) { field_0x114 = v; }
    void setField_0x118(s32 v) { field_0x118 = v; }
    void setField_0x16C(s32 v) { field_0x16c = v; }
    void setField_0x170(s32 v) { field_0x170 = v; }
    void setField_0x18C(s32 v) { field_0x18c = v; }
    void setField_0x190(s32 v) { field_0x190 = v; }
    void setField_0x1A8(bool v) { field_0x1a8 = v; }

    // The spawner's 3rd argument; spawned children usually get their
    // parent's. fn_801F5790 compares it with fn_8023DE58(player).
    /* 0x000 vtable */
    /* 0x004 */ s32 field_0x4;
    /* 0x008 */ s32 field_0x8; // also the spawner's 3rd argument; the update
                               // loop counts actors with 0 here and
                               // field_0x1a8 == 1
    /* 0x00C */ JGeometry::TVec3f mPos;      // spawn position; the actor's
                                             // sound object follows it
    /* 0x018 */ JGeometry::TVec3f mPrevPos;  // copy of mPos before update()
    /* 0x024 */ JGeometry::TVec3f mSpawnPos; // set only by the spawner
    /* 0x030 */ JGeometry::TVec3f field_0x30;
    /* 0x03C */ JGeometry::TVec3f field_0x3c;
    /* 0x048 */ JGeometry::TVec3f field_0x48; // 1.0f
    /* 0x054 */ JGeometry::TVec3f field_0x54; // 1.0f
    /* 0x060 */ ActorBox mBox;  // (-8, -8, 8, 8)
    /* 0x070 */ ActorBox mBox2; // zero; the spawner copies mBox here if
                                // init() left it all zero
    /* 0x080 */ f32 field_0x80;
    /* 0x084 */ f32 field_0x84;
    /* 0x088 */ f32 field_0x88;
    /* 0x08C */ f32 field_0x8c;
    /* 0x090 */ s32 field_0x90; // spawner's 5th argument
    /* 0x094 */ s16 field_0x94;
    /* 0x098 */ s32 field_0x98;
    /* 0x09C */ s32 field_0x9c;
    /* 0x0A0 */ s32 field_0xa0;
    /* 0x0A4 */ s32 field_0xa4;
    /* 0x0A8 */ s32 field_0xa8;
    /* 0x0AC */ s32 field_0xac;
    /* 0x0B0 */ s32 field_0xb0;
    /* 0x0B4 */ s32 field_0xb4;
    /* 0x0B8 */ s32 field_0xb8;
    /* 0x0BC */ s32 field_0xbc;
    /* 0x0C0 */ s32 field_0xc0; // EnemyParam.csv column "efsz"
    /* 0x0C4 */ u8 field_0xc4;  // update loop: updates the actor if set
    /* 0x0C8 */ s32 field_0xc8;
    /* 0x0CC */ u8 field_0xcc;  // update loop: updates the actor if set
    /* 0x0D0 */ s32 field_0xd0;
    /* 0x0D4 */ u8 field_0xd4;
    /* 0x0D5 */ u8 field_0xd5;
    /* 0x0D8 */ s32 field_0xd8;
    /* 0x0DC */ s32 field_0xdc;
    /* 0x0E0 */ s32 field_0xe0;
    /* 0x0E4 */ s32 field_0xe4;
    /* 0x0E8 */ s32 field_0xe8;
    /* 0x0EC */ JGeometry::TVec3f field_0xec;
    /* 0x0F8 */ s32 field_0xf8;
    /* 0x0FC */ s32 field_0xfc;
    /* 0x100 */ JPABaseEmitter* mEmitter100; // released by the destructor
    /* 0x104 */ JPABaseEmitter* mEmitter104; // released by the destructor
    /* 0x108 */ s32 mLife;       // EnemyParam.csv column "life"
    /* 0x10C */ s32 field_0x10c;
    /* 0x110 */ s32 field_0x110; // spawner's 7th argument (default -1)
    /* 0x114 */ s32 field_0x114; // EnemyParam.csv column "atcp"
    /* 0x118 */ s32 field_0x118; // EnemyParam.csv column "atcn"
    /* 0x11C */ bool mAlive;       // the update loop deletes the actor when false
    /* 0x11D */ bool mDrawEnabled; // the draw loop skips the actor when false
    /* 0x11E */ u8 field_0x11e;    // update loop: updates the actor if set
    /* 0x11F */ u8 field_0x11f;    // draw/update loops: still drawn/updated
                                   // while the game is paused (fn_80134CC8)
    /* 0x120 */ ActorSub120 field_0x120;
    /* 0x138 */ s32 field_0x138;
    /* 0x13C */ u8 field_0x13c;
    /* 0x13D */ u8 field_0x13d;
    /* 0x13E */ u8 field_0x13e;
    /* 0x13F */ u8 field_0x13f;
    /* 0x140 */ u8 field_0x140;
    /* 0x141 */ u8 field_0x141;
    /* 0x142 */ u8 field_0x142;
    /* 0x143 */ u8 field_0x143;
    /* 0x144 */ u8 field_0x144;
    /* 0x145 */ u8 field_0x145;
    /* 0x146 */ u8 field_0x146;
    /* 0x148 */ s32 field_0x148;
    /* 0x14C */ u8 field_0x14c;
    /* 0x14D */ u8 field_0x14d;
    /* 0x14E */ u8 field_0x14e;
    /* 0x150 */ s32 field_0x150;
    /* 0x154 */ s32 field_0x154;
    /* 0x158 */ s32 field_0x158;
    /* 0x15C */ ActorBase* mListPrev; // the manager's actor list at
    /* 0x160 */ ActorBase* mListNext; // ActorManager 0x2018 (fn_801F84C4)
    /* 0x164 */ u8 field_0x164;
    /* 0x165 */ u8 mInList;           // set by fn_801F84C4 when linked
    /* 0x168 */ s32 field_0x168;
    /* 0x16C */ s32 field_0x16c; // spawner's 6th argument (default -1)
    /* 0x170 */ s32 field_0x170; // set by the spawner from fn_802D39FC(fn_802D6A9C())
    /* 0x174 */ s32 field_0x174;
    /* 0x178 */ s32 field_0x178;
    /* 0x17C */ u8 field_0x17c;
    /* 0x17D */ u8 field_0x17d;
    /* 0x180 */ s32 field_0x180; // 20
    /* 0x184 */ f32 field_0x184; // 40.0f
    /* 0x188 */ s32 field_0x188;
    /* 0x18C */ s32 field_0x18c; // EnemyParam.csv column "dasu"
    /* 0x190 */ s32 field_0x190; // EnemyParam.csv column "voic"
    /* 0x194 */ u8 field_0x194;
    /* 0x195 */ u8 field_0x195;
    /* 0x196 */ u8 field_0x196;
    /* 0x198 */ s32 mSerial;  // the spawner's running spawn count, its return value
    /* 0x19C */ s32 mSlot;    // index into ActorManager::mActors
    /* 0x1A0 */ u32 mActorId; // ActorId; 'NULL' until spawned
    /* 0x1A4 */ s32 field_0x1a4;
    /* 0x1A8 */ bool field_0x1a8; // EnemyParam.csv column "dstr" != 0
    /* 0x1AC */ JGeometry::TVec3f field_0x1ac[4];
    /* 0x1DC */ ActorSub1DC field_0x1dc;
}; // Size: 0x230

#endif
