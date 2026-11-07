#ifndef GAME_GAMEROOTSUB20_H
#define GAME_GAMEROOTSUB20_H

#include "game/GameGlobals.h"

class J3DModel;
class J3DAnmTextureSRTKey;
class J3DAnmTevRegKey;
class JKRSolidHeap;
class JKRHeap;
class ActorSoundObj_804578F4;

// The object fn_801902C0 returns (gGameRoot_8053AAF8->field_0x4->field_0x20; named
// after that getter). Owner: game/tu_801902C0.cpp. A set of J3D effect models
// (crystals, waterfalls, castle gate, sand, magic circles, altar, snow wall)
// loaded from the stage archives by the constructor fn_801917E0 into its own
// solid heap (field 0x0), each with a small request that actors raise: a
// position and a flag, once the model pointer is non-NULL and the flag is
// clear. The names of the loaded files give the model names ("suisyo" crystal,
// "taki" waterfall, "osiro" castle, "suna" sand, "ak_magicCircle", "mekakusi",
// "SandFall00", "saidanDark00" / "saidanFlash00" altar, "snowWall00").
// act_special_bg_draw_object: three 0x20-byte requests at 0x2E0, 0x300 and
// 0x320.
struct GameRootSub20_801902C0 {
    // Seven crystal effects at 0x8 (suisyo_%d.bmd/btk/brk); the entry starts
    // at the position, so the active flag is at 0x5C.
    struct Crystal {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ Mtx mMtx;
        /* 0x3C */ J3DModel* mModel;
        /* 0x40 */ J3DAnmTextureSRTKey* mBtk;
        /* 0x44 */ J3DAnmTevRegKey* mBrk;
        /* 0x48 */ f32 field_0x48;
        /* 0x4C */ s32 field_0x4c; // brk frame while field_0x48 is negative (fn_80190DC8)
        /* 0x50 */ s32 field_0x50;
        /* 0x54 */ s32 field_0x54;
        /* 0x58 */ s32 field_0x58;
        /* 0x5C */ u8 mActive;
        /* 0x5D */ u8 field_0x5d[3]; // not yet known
    }; // Size: 0x60
    // Two smaller requests at 0x2A8 and 0x2C4 (act_environment_control_2's
    // update: position copied in, flag raised once the pointer at 0xC is
    // non-NULL and the flag is clear).
    struct SmallRequest {
        /* 0x00 */ Vec mPos; // a Vec: no constructor, the array is not constructed
        /* 0x0C */ J3DModel* field_0xc; // tested against NULL
        /* 0x10 */ J3DModel* field_0x10; // only the first entry loads it
        /* 0x14 */ J3DAnmTextureSRTKey* field_0x14;
        /* 0x18 */ u8 mActive;
        /* 0x19 */ u8 field_0x19[3]; // not yet known
    }; // Size: 0x1C
    struct Request {
        /* 0x00 */ Vec mPos;
        /* 0x0C */ J3DModel* field_0xc; // tested against NULL
        /* 0x10 */ J3DAnmTextureSRTKey* field_0x10;
        /* 0x14 */ f32 mValue;
        /* 0x18 */ s32 field_0x18;
        /* 0x1C */ u8 mActive;
        /* 0x1D */ u8 field_0x1d[3]; // not yet known
    }; // Size: 0x20
    // Up to ten entries, filled by act_quicksand's vfunc_0x9C while
    // mQuicksandCount (0x4A8) is below 10: the quicksand's position, then
    // three of its floats (field_0x54.x, 0x398 and 0x3A8).
    struct QuicksandEntry {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ f32 field_0xc;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ J3DModel* mModel; // suna.bmd
        /* 0x1C */ J3DAnmTextureSRTKey* mBtk;
        /* 0x20 */ J3DAnmTevRegKey* mBrk;
    }; // Size: 0x24
    // Eight magic circles at 0x4AC (ak_magicCircle.*).
    struct MagicCircle {
        /* 0x00 */ JGeometry::TVec3f mPos;
        // SHARED-EDIT (act_magic_circle): MagicCircle_804A7960::update()
        // stores its clamped colour/type index at 0x0C (stw) and (f32)field_0x244
        // at 0x10 (stfs).
        /* 0x0C */ s32 field_0xc;
        /* 0x10 */ f32 field_0x10;
        /* 0x14 */ J3DModel* mModel;
        /* 0x18 */ J3DAnmTextureSRTKey* mBtk;
        /* 0x1C */ J3DAnmTextureSRTKey* mBtkApp; // ak_magicCircleApp.btk
        /* 0x20 */ J3DAnmTevRegKey* mBrk;
        /* 0x24 */ J3DAnmTevRegKey* mBrkColor; // ak_magicCircleColor.brk
    }; // Size: 0x28
    // Up to two positions queued by act_environment_control_2's update
    // (count at 0x618, stored while it is below 2); mekakusi.bmd/btk.
    struct QueuedPos {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ J3DModel* mModel;
        /* 0x10 */ J3DAnmTextureSRTKey* mBtk;
    }; // Size: 0x14
    // Sixteen SandFall00 effects at 0x61C.
    struct SandFall {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ u8 field_0xc[4]; // not yet known
        /* 0x10 */ J3DModel* mModel;
        /* 0x14 */ J3DAnmTextureSRTKey* mBtk;
    }; // Size: 0x18
    // SHARED-EDIT (act_ice_melted_by_four_players): the request the ICEB
    // update (act_ice_melted_by_four_players.cpp) fills at 0x7D0 when its
    // model pointer at 0x10 is NULL and mActive is clear: its position, a
    // percentage (100 - field_0x238 * 100) and mActive = 1 (snowWall00.*).
    struct IceRequest {
        /* 0x00 */ JGeometry::TVec3f mPos;
        /* 0x0C */ s32 mPercent;
        /* 0x10 */ J3DModel* field_0x10; // tested against NULL
        /* 0x14 */ J3DAnmTextureSRTKey* field_0x14;
        /* 0x18 */ J3DAnmTevRegKey* field_0x18;
        /* 0x1C */ u8 mActive;
        /* 0x1D */ u8 field_0x1d[3]; // not yet known
    }; // Size: 0x20

    // 0x801917E0: loads everything into a new solid heap below `parent`.
    GameRootSub20_801902C0(JKRHeap* parent);
    // 0x80191744: destroys the heap and the two sound objects, then frees the
    // object when flag > 0 (a destructor-shaped function that is called
    // directly with 1, by GameRootSub04_801F856C's destructor).
    GameRootSub20_801902C0* fn_80191744(s16 flag);
    // 0x8019169C: clears the active flags and counters.
    void fn_8019169C();
    // 0x801914C4: the per-frame reset, then counts field_0x4.
    void fn_801914C4();
    // 0x80190DC8: steps every loaded texture / register animation to its frame for this tick
    // (field_0x4), each entry only while it is active / present. Called by GameRootSub04's fn_80139D9C.
    void fn_80190DC8();
    // 0x801902D0: draws all active effect models.
    void fn_801902D0();

    /* 0x000 */ JKRSolidHeap* mHeap;
    /* 0x004 */ s32 field_0x4; // counted up by fn_801914C4
    /* 0x008 */ Crystal mCrystals[7];
    /* 0x2A8 */ SmallRequest mSmallRequests[2];
    /* 0x2E0 */ Request mRequests[3];
    /* 0x340 */ QuicksandEntry mQuicksandEntries[10];
    /* 0x4A8 */ s32 mQuicksandCount;
    /* 0x4AC */ MagicCircle mMagicCircles[8];
    /* 0x5EC */ s32 field_0x5ec;
    /* 0x5F0 */ QueuedPos mQueuedPos[2];
    /* 0x618 */ s32 mQueuedPosCount;
    /* 0x61C */ SandFall mSandFalls[16];
    /* 0x79C */ s32 field_0x79c;
    /* 0x7A0 */ s32 field_0x7a0;
    /* 0x7A4 */ s32 field_0x7a4;
    // SHARED-EDIT (act_opening_goal update): set to 1 through fn_801902C0()'s result.
    /* 0x7A8 */ u8 field_0x7a8;
    /* 0x7A9 */ u8 field_0x7a9[3]; // not yet known
    /* 0x7AC */ J3DModel* field_0x7ac;              // saidanDark00.bmd
    /* 0x7B0 */ J3DAnmTextureSRTKey* field_0x7b0;   // .btk
    /* 0x7B4 */ J3DAnmTevRegKey* field_0x7b4;       // .brk
    /* 0x7B8 */ ActorSoundObj_804578F4* mSound7b8;
    /* 0x7BC */ s32 field_0x7bc;
    /* 0x7C0 */ u8 field_0x7c0;
    /* 0x7C1 */ u8 field_0x7c1[3]; // not yet known
    /* 0x7C4 */ J3DModel* field_0x7c4;              // saidanFlash00.bmd
    /* 0x7C8 */ J3DAnmTevRegKey* field_0x7c8;       // .brk
    /* 0x7CC */ ActorSoundObj_804578F4* mSound7cc;
    /* 0x7D0 */ IceRequest mIceRequest;
}; // Size: 0x7F0

// Owner: game/tu_801902C0.cpp (0x801902C0-0x801902C8): gGameRoot_8053AAF8->field_0x4->field_0x20.
GameRootSub20_801902C0* fn_801902C0();

#endif
