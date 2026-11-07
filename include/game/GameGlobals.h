#ifndef GAME_GAMEGLOBALS_H
#define GAME_GAMEGLOBALS_H

// Globals most game units touch. Names are dtk's placeholders (they are the
// link names, so keep them until a unit that defines them names them);
// types are placeholders too. Field types come from the loads (lwz -> a
// 32-bit value or pointer, lbz -> u8); pointers whose target is unknown
// are void*.
//
// Class names: provisional descriptive names carry an address (docs/PLAN.md,
// "Placeholder names"): the vtable when known, else the constructor, else
// the first function seen reaching the object (said at each). Vectors:
// JGeometry::TVec3f for game data, Vec only where a mangled name or a
// PSMTX/PSVEC call proves it (Camera_804AA9E4::fn_8030C5F0's mangled name
// says Vec).
//
// Owner: the shared-header owner (docs/COORDINATOR.md). Propose new fields
// as SHARED-EDITS, with the offset, access width and the function that shows
// it.

#include "types.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/TColor.h"
#include "game/Player.h"
#include "game/cls_8016ED88.h"
// The view (getGameView_802D6A9C) and the effect system (getEffectSystem_8013C824) moved to
// their own headers; included here so existing users keep compiling.
#include "game/Effect.h"
#include "game/GameView.h"
// The map, its collision queries (fn_80226850, ...) and the camera.
#include "game/Camera.h"
#include "game/Map.h"

class ActorManager_804A5800;
// The object lbl_8053AD68 holds (game/tu_80379F84.cpp); defined in tu_80379F84.h.
struct cls_804AE9A0;
class JAISeMgr;
class JAISeqMgr;
class JAISound;
struct JASAudioReseter;
class JAUSoundMgr;
class JAUSectionHeap;
class JAUDopplerAudienceState;
class JAUAudience_withSetting_doppler;
template <int N> class JAUDopplerAudible;
template <int N, class Audible, class State, class Audience> class JAUAudience_;
class JPAEmitterManager;
// JSystem/J2D/J2DGrafContext.h (mkdd's J2D, which FSA's J2DGraph units use).
class J2DOrthoGraph;
struct J2DPrint;
struct J2DPicture;
class JKRSolidHeap;

// The object at offset 0x4 of *gGameRoot_8053AAF8 (type unknown; named after
// getActorManager_801F856C, the first function seen reading it).
class cls_804B1D10;
class BgRewrite_804A7C98;
class cls_804AE9B0;
class cls_804B1920;
class cls_804B13D4;
class cls_804B146C;
class cls_804B1AE8;
class cls_804B1BB0;
class cls_804B1BE8;
class cls_804B1C20;
class cls_804B1C58;
class cls_804B1C90;
class cls_804B1CC8;
class cls_804B1D00;
class cls_804B1D20;
class cls_804B1E1C;
struct J2DScreen;
class PaneCtrl_804AF86C;
class JAISoundID; // JSystem/JAudio/Interface/JAISound.h (AudioMgr_8045A080::fn_80457F80)
class cls_804B17A0;
class JPABaseEmitter;
struct J2DAnmBase;
class PlayerManager_804A6F28;
struct GameRootSub20_801902C0;

struct GameRootSub04_801F856C {
    // 0x8013A538 (game/tu_80139BE8.cpp, not written yet): GameRoot's
    // fn_80135020 makes it with __nw__(0x38) and passes GameRoot's heap.
    GameRootSub04_801F856C(JKRHeap* heap);
    // vtable at 0x0 (0x8049F080); its one slot is the destructor below.
    virtual ~GameRootSub04_801F856C();

    /* 0x4 */ PlayerManager_804A6F28* field_0x4; // constructor fn_8013A538 (tu_80139BE8)
    /* 0x8 */ ActorManager_804A5800* mActorManager; // getActorManager_801F856C; allocated in
                                           // game/tu_80139BE8.cpp
    /* 0xC */ GameView_804A9950* mView;             // getGameView_802D6A9C
    // fn_802A3D80 is called on it by fn_8013A1A4 (game/tu_80139BE8.cpp).
    /* 0x10 */ BgRewrite_804A7C98* field_0x10;
    // fn_8043EDF8 is called with it by fn_8013A1A4 (game/tu_80139BE8.cpp); the
    // object fn_8043F504 builds (cls_804B1920).
    /* 0x14 */ cls_804B1920* field_0x14;
    // fn_80439AB0 is called with it by fn_8013A1A4 (game/tu_80139BE8.cpp); the
    // object fn_80439C78 builds (cls_804B17A0).
    /* 0x18 */ cls_804B17A0* field_0x18;
    // fn_8037AFC0 is called on it by fn_8013A1A4 (game/tu_80139BE8.cpp).
    /* 0x1C */ cls_804AE9B0* field_0x1c;
    // Passed to fn_801902D0 by fn_80139CC4 (game/tu_80139BE8.cpp).
    /* 0x20 */ GameRootSub20_801902C0* field_0x20;
    // fn_8016A3AC returns it (game/tu_8016A2B4.cpp).
    /* 0x24 */ struct GameRootSub24_8016A3AC* field_0x24;
    /* 0x28 */ u8 field_0x28[0x2C - 0x28];
    // The staff-roll page loader (game/act/tu_8044B80C.h); fn_80139C84 draws it.
    /* 0x2C */ cls_804B1D10* field_0x2c;
    // A heap: game/tu_8044B80C.cpp makes it current, frees its J2DScreens
    // and calls freeAll on it (becomeCurrentHeap / freeAll calls on r3).
    /* 0x30 */ JKRHeap* field_0x30;
    // Cleared (stw 0) by fn_8013A1A4 (game/tu_80139BE8.cpp); type unknown.
    /* 0x34 */ s32 field_0x34;

    // 0x8013A1A4 (game/tu_80139BE8.cpp, not written yet in this file's copy).
    // Calls fn_8016EDB0 on field_0x24, fn_801F8E4C on mActorManager and
    // vfunc_0xC on mView; fn_8023F088 when gGameRoot_8053AAF8->field_0x7e is 1; then
    // fn_8023F78C and fn_8023F728, sets lbl_8053ACD0, runs the calls on
    // field_0x10, field_0x14, field_0x18, field_0x1c and field_0x20, fn_8038D870,
    // and clears field_0x34 and lbl_80529DEC's 0x240 and 0x1F4 words.
    void fn_8013A1A4();

    // 0x80139D9C (game/tu_80139BE8.cpp): the per-frame update; runs the player manager, the
    // map, the background rewriter and the actors in the modes that step the game.
    void fn_80139D9C();

    // 0x80139BE8 (game/tu_80139BE8.cpp, not written yet). Calls fn_801F857C on
    // mActorManager, clears lbl_8053ACD0 and the four bytes at 0x235C of
    // gGameRoot2_8053AB10->0x20, and runs fn_801184F0 on fn_8011921C()'s object.
    void fn_80139BE8();

    // 0x80139C84 (game/tu_80139BE8.cpp, not written yet). Calls
    // fn_8043DC98 on lbl_8053AED0, then draws field_0x2c when it is set.
    void fn_80139C84();

    // 0x80139CC4 (game/tu_80139BE8.cpp, not written yet). Calls fn_8013B438 on
    // gGameRoot2_8053AB10 twice around the vfunc_0x14 of mView, fn_8023F2BC, fn_801F8658
    // on mActorManager and fn_801902D0 on field_0x20; then fn_8016DA6C on
    // field_0x24 unless gGameRoot2_8053AB10->field_0x39 is set.
    void fn_80139CC4();

    // 0x80139D3C (game/tu_80139BE8.cpp, not written yet). Calls fn_8016DB60 on
    // field_0x24 unless gGameRoot2_8053AB10->field_0x39 is set.
    void fn_80139D3C();

    // 0x80139C5C (game/tu_80139BE8.cpp): fn_8043D5F0 on lbl_8053AED0, then
    // fn_8038D4A8. It reads no field, but GameRoot's fn_80136828 loads its
    // field_0x4 into r3 for the call.
    void fn_80139C5C();
};

// *gGameRoot_8053AAF8: the most-used game singleton (309 of 529 units read it).
// Set in game/tu_801344C0.cpp (0x80134998) and game/tu_80134C04.cpp
// (0x80139340). Named after its vtable, 0x8049EF20 (constructor
// fn_80139300, allocated with __nw__(0x11C)); its real layout is unknown.
struct AudioMgr_8045A080;
class cls_804A8390;
class cls_804AA8C8;
// The 4-byte object of tu_803FF2C4 (lbl_80529DEC's field_0x60), defined in
// game/tu_80301CD0.cpp (constructor fn_8030905C); named after its vtable,
// 0x804AA8B8. GameRoot's destructor deletes it through slot 0x8.
class cls_804AA8B8 {
public:
    cls_804AA8B8();
    /* 0x00 */ virtual ~cls_804AA8B8();
    void fn_80308E90();
};
class cls_804B0FC0;
class J2DTextBox;
class MessageManager_804AF070;
class cls_804B04F8;
class cls_804B19F0;
class cls_804B1988;
class cls_804B1634;
class Title_804AF820;
class FileSelect_804AFF60;
class GameMode_804B0688;
class cls_804AF878;
class cls_804AF480;
struct cls_804AE9A0;
struct GameRoot_8049EF20 {
    // vtable 0x8049EF20: one slot, the destructor (0x80138E78).
    virtual ~GameRoot_8049EF20();

    // The screen objects fn_80135020 (0x80135020) creates for a game mode
    // (field_0x4c) and fn_801352F4 (0x801352F4) deletes again; each is made in
    // field_0x98 with its heap group ID set to 1.
    /* 0x04 */ GameRootSub04_801F856C* field_0x4; // mode 7 (0x38 bytes); -> 0x8: the actor manager
    // Copied from lbl_80529DEC's field_0x5c / field_0x60 by the constructor
    // (the input object cls_804AA8C8 and the 4-byte cls_804AA8B8); the
    // destructor deletes both through their vtables.
    /* 0x08 */ cls_804AA8C8* field_0x8;
    /* 0x0C */ cls_804AA8B8* field_0xc;
    // The fade object (also lbl_8053ACA0); fn_80138C9C calls fn_802ACB58 on it.
    /* 0x10 */ cls_804A8390* field_0x10;
    /* 0x14 */ u8 field_0x14[0x18 - 0x14];
    /* 0x18 */ MessageManager_804AF070* field_0x18; // new 0x158 in the constructor
    /* 0x1C */ cls_804AE9A0* field_0x1c;         // mode 12 (0x20 bytes)
    /* 0x20 */ Title_804AF820* field_0x20;       // mode 1 (0x158 bytes)
    /* 0x24 */ FileSelect_804AFF60* field_0x24;  // mode 3 (0x1AC bytes)
    /* 0x28 */ GameMode_804B0688* field_0x28;    // modes 2 and 5 (0x120 bytes)
    /* 0x2C */ cls_804AF878* field_0x2c;         // mode 4 (0x10 bytes)
    /* 0x30 */ cls_804AF480* field_0x30;         // mode 6 (0x10 bytes)
    // Made by the constructor (0x24, 0x28, 0x14 and 0xF0 bytes).
    /* 0x34 */ cls_804B04F8* field_0x34;
    /* 0x38 */ cls_804B19F0* field_0x38;
    /* 0x3C */ cls_804B1988* field_0x3c;
    /* 0x40 */ cls_804B1634* field_0x40;
    /* 0x44 */ u8 field_0x44[0x4C - 0x44];
    // The screen mode: fn_801367E0 stores it (10 also resets the fade object);
    // fn_80134CC8 tests 11 (paused), act_sandstorm compares it with 11 and
    // isGameModeSet (game/GameMode.h) with 7-0x16 (lwz at ~250 sites).
    /* 0x4C */ s32 field_0x4c;
    // Set (with field_0x58 = 1) by fn_80136798, fn_8013670C and fn_801365E8 (game/tu_80134C04.cpp).
    /* 0x50 */ s32 field_0x50;
    // Stored by fn_8019CDC4 (game/tu_80197070.cpp, 0x8019CDC4, stw of r3).
    /* 0x54 */ s32 field_0x54;
    // The fade state tested by cls_804A8390::fn_802AC8B4 (tu_802AC8AC): 1 and
    // 5 step the black picture up, 4 steps it down, 6 counts field_0x14 down.
    /* 0x58 */ s32 field_0x58;
    // A frame count: Title_804AF820::fn_803A44D8 (game/tu_803A3484.cpp)
    // passes it to fn_8016D398, which takes it modulo the cloud animations'
    // frame counts (divw). Cleared by fn_80138C9C.
    /* 0x5C */ s32 field_0x5c;
    // Cleared by fn_8013670C (game/tu_80134C04.cpp, stw of 0).
    /* 0x60 */ s32 field_0x60;
    /* 0x64 */ s32 field_0x64;
    // Alphas (clrlwi 24 into J2DPicture/J2DTextBox::setAlpha in fn_80136828):
    // field_0x68 for the rival picture field_0xb8 and the text boxes,
    // field_0x6c for field_0xbc.
    /* 0x68 */ s32 field_0x68; // cleared by fn_80138C9C
    /* 0x6C */ s32 field_0x6c;
    /* 0x70 */ s32 field_0x70; // cleared by fn_80138C9C
    // Both cleared by fn_8019CDC4 (game/tu_80197070.cpp, stw of 0).
    /* 0x74 */ s32 field_0x74;
    /* 0x78 */ s32 field_0x78;
    /* 0x7C */ s16 field_0x7c; // cleared by the constructor (sth)
    // A flag (lbz; cmplwi 0 at ~180 sites): set to 1 by fn_80138C9C and by
    // ConnectScreen_804B0F10::fn_803FB6F4 once its controllers are connected,
    // cleared by cls_804B0B00::fn_803EC804 when it enters player 0 alone
    // (mPlayerMask 0x11). While set, fn_80138AB0 checks the input slots of all
    // four players instead of player 0's only. Its meaning is not proven, so
    // it keeps its offset name.
    /* 0x7E */ u8 field_0x7e;
    // A flag (lbz at ~420 sites): set to 1 by fn_80138C9C; the game mode
    // screen (GameMode_804B0688::fn_803DE05C) stores 1 for menu entry 0 and 0
    // for entry 1. cls_804AF878 shows cls_804B0B00 while it is set and
    // ConnectScreen_804B0F10 while it is clear; isGameModeSet (game/GameMode.h)
    // picks its mode set by it. Its meaning is not proven (the menu entries
    // aren't), so it keeps its offset name.
    /* 0x7F */ u8 field_0x7f;
    /* 0x80 */ u8 field_0x80; // cleared by fn_80138C9C
    /* 0x81 */ u8 field_0x81; // cleared by fn_80138C9C
    // Per-player bits (lbz; and. with 1 << player at ~200 sites). Bits 0-3:
    // player i takes part (fn_80138A30 counts them, fn_8023F98C tests one,
    // fn_801F6068); bits 4-7: player i's controller is connected (the connect
    // screens cls_804B0B00::fn_803ED83C and ConnectScreen_804B0F10 set them).
    // Cleared by fn_80138C9C.
    /* 0x82 */ u8 mPlayerMask;
    // Four 2-bit fields, player i's at bits 2i-2i+1 ((x >> 2 * i) & 3 at ~25
    // sites): the connect screens store 1 for a taking-part player and 0 for
    // the others; cls_804B0B00 stores 2 for player 0 alone without a
    // controller in slot 0 (fn_8030A308). Cleared by fn_80138C9C.
    /* 0x83 */ u8 field_0x83;
    /* 0x84 */ u8 field_0x84;
    // Set to 1 by fn_8019CDC4 (game/tu_80197070.cpp, stb of 1).
    /* 0x85 */ u8 field_0x85;
    // Toggled between 0 and 1 by fn_80134DC0 (game/tu_80134C04.cpp, 0x80134DC0,
    // stb of r3 at 0x86 only).
    /* 0x86 */ u8 field_0x86;
    // Cleared when fn_80135020 creates the title screen (mode 1).
    /* 0x87 */ u8 field_0x87;
    // Stored by fn_80134D14 (game/tu_80134C04.cpp, 0x80134D14, stb of r4).
    /* 0x88 */ u8 field_0x88;
    /* 0x89 */ u8 field_0x89[0x8C - 0x89];
    // Scales fn_80136828 multiplies the picture / font sizes by (lfs):
    // field_0x8c for field_0xb8 (413 x 38) and the text boxes (30 x 60),
    // field_0x90 for field_0xbc (256 x 128).
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ f32 field_0x90;
    // Stored by fn_80134FC4 (game/tu_80134C04.cpp, 0x80134FC4, stb of r4), which
    // sets field_0x95 to 1 unless the value is 1.
    /* 0x94 */ u8 field_0x94;
    // Set to 1 by fn_80134FC4 (game/tu_80134C04.cpp) when field_0x94 is not 1.
    /* 0x95 */ u8 field_0x95;
    // Stored by fn_80134FBC (game/tu_80134C04.cpp, 0x80134FBC, stb of r4).
    /* 0x96 */ u8 field_0x96;
    /* 0x97 */ u8 field_0x97[0x98 - 0x97];
    // The heap the screens of fn_80135020 are made in (lbl_80529DEC's mHeap,
    // copied by the constructor).
    /* 0x98 */ JKRHeap* field_0x98;
    /* 0x9C */ JKRSolidHeap* field_0x9c; // lbl_80529DEC's field_0x4; destroyed by the destructor
    // The audio object (fn_80458880 / fn_804593D4 in fn_80134D20, game/tu_80134C04.cpp).
    /* 0xA0 */ AudioMgr_8045A080* field_0xa0;
    // Copied from lbl_80529DEC+0x14 by the constructor (0x80139360). Game
    // code calls it through vtable slot 0x18 = getResource(u32 type,
    // const char* name) with a 4CC and a name ('ENM ' in fn_801F62AC, 'TIMG'
    // in the constructor at 0x801395D8), and through slot 0x30 =
    // getResSize(const void*) (24 sites, e.g. in tu_801F6058). JKRFileLoader declares
    // both slots too, so the code is the same either way; the game's loaders
    // build JKRAramArchive/JKRMemArchive (fn_80401AC8, fn_80401D20).
    /* 0xA4 */ JKRArchive* field_0xa4;
    /* 0xA8 */ JKRArchive* field_0xa8; // lbl_80529DEC's field_0x1c (msgus.arc)
    // The archives fn_80135A04 starts loading for a game mode (fn_80401E10;
    // the boss archive, game2D.arc, gamemode.arc, ...); fn_80135494 waits for
    // their byte 0x4 and deletes them.
    /* 0xAC */ cls_804B0FC0* field_0xac;
    /* 0xB0 */ cls_804B0FC0* field_0xb0;
    // Constructed with __ct__Q25JMath13TRandom_fast_FUl at this+0xB4 by the
    // constructor fn_80139300 (0x8013932C-0x80139338). 132 units inline
    // TRandom_fast_::get()/get_ufloat_1() on it (lwz 0xB4, mullw 0x19660D,
    // + 0x3C6EF35F, stw, srwi 9, oris 0x3F80, - 1.0f), e.g. 0x803D6578.
    /* 0xB4 */ JMath::TRandom_fast_ mRandom;
    /* 0xB8 */ J2DPicture* field_0xb8; // made from 'TIMG' resources by the constructor
    /* 0xBC */ J2DPicture* field_0xbc;
    // Two text boxes the constructor makes; fn_80138C9C copies field_0xc8
    // (message 0x42, getMessageText_803832BC) into both.
    /* 0xC0 */ J2DTextBox* field_0xc0;
    /* 0xC4 */ J2DTextBox* field_0xc4;
    /* 0xC8 */ char field_0xc8[0xF0 - 0xC8];
    /* 0xF0 */ f32 field_0xf0; // 0.0f after fn_80138C9C
    /* 0xF4 */ f32 field_0xf4; // getMessageText_803832BC's result for field_0xc8
    // Cleared to 0 by fn_801365C8 (game/tu_80134C04.cpp, 0x801365C8), together
    // with field_0xfc, when it is stored a 1 (stb of r4 at 0xFE first).
    /* 0xF8 */ s32 field_0xf8;
    /* 0xFC */ u8 field_0xfc;
    /* 0xFD */ u8 field_0xfd; // cleared by the constructor; tested by fn_80135494
    // Stored by fn_801365C8 (game/tu_80134C04.cpp, 0x801365C8, stb of r4).
    /* 0xFE */ u8 field_0xfe;
    // SHARED-EDIT (tu_803BC6F0): the file select screen sets it to 1 before
    // it clears the save rows (fn_803D0264, fn_803CDEE8) or rebuilds them.
    /* 0xFF */ u8 field_0xff;
    /* 0x100 */ u8 field_0x100[0x104 - 0x100];
    /* 0x104 */ void* field_0x104; // cleared by the constructor, deleted by the destructor
    // Stored by fn_80134CC0 (game/tu_80134C04.cpp, 0x80134CC0), which
    // tu_803A60E4 calls on gGameRoot_8053AAF8 with 0, 1 and 0x1E; what the value means
    // is not known yet.
    /* 0x108 */ s32 field_0x108;
    // Cleared with field_0x108 by fn_80134D20 (game/tu_80134C04.cpp, stw of 0).
    /* 0x10C */ s32 field_0x10c;
    /* 0x110 */ u8 field_0x110[0x11C - 0x110]; // main allocates 0x11C bytes
    // Owner: game/tu_80134C04.cpp (0x80134CC0; defined there). Stores r4 at
    // this+0x108 (stw).
    void fn_80134CC0(s32 value);

    // 15 units / 17 calls. Stores at this+0x50, 0x58 and 0x84 (per review);
    // act_ending_map_skipper calls it with (15, 0).
    void fn_80136798(s32 a, s32 b);
    // ~28 units. In tu_80134C04 with GameRoot's constructor; uses this->0xA0.
    // r3 only.
    void fn_80138B7C();
    // Flips this+0x86 between 0 and 1 (lbz, then stb of 1 or 0). Defined in
    // game/tu_80134C04.cpp (0x80134DC0); no callers found yet.
    void fn_80134DC0();
    // Stores r4 at gGameRoot_8053AAF8+0x88 (stb). Defined in game/tu_80134C04.cpp;
    // the body reads the global itself, r3 (this) is gGameRoot_8053AAF8 at every call.
    void fn_80134D14(u8 value);
    // Stores r4 at this+0x96 (stb). Defined in game/tu_80134C04.cpp; called
    // by act_stop_npc (0x802ED1B4, 0).
    void fn_80134FBC(u8 value);
    // Stores r4 at this+0x94 (stb), then sets this+0x95 to 1 unless the value
    // is 1. Defined in game/tu_80134C04.cpp (0x80134FC4); no callers found yet.
    void fn_80134FC4(u8 value);
    // Owner: game/tu_80134C04.cpp (0x80134FE0, not written yet). Stores r4 at
    // this+0x94 (stb), reads it back, and calls fn_80458658 on gAudioMgr_8053AF80 when
    // it is 1, else sets this+0x95 to 1. Callers: game/tu_8029C584.cpp
    // (UnknownFORM_804A7BE8's destructor, with 0), game/tu_8023FBF0.cpp and game/tu_802D39B8.cpp.
    void fn_80134FE0(u8 value);
    // Owner: game/tu_80134C04.cpp (0x80134CC8). Whether the game is paused
    // (field_0x4c == 11 or field_0x88 == 2) and the fade object gGameRoot2_8053AB10->0x2C
    // has its byte at 0xC clear (0xC is 0 when it is idle).
    u8 fn_80134CC8();
    // Owner: game/tu_80134C04.cpp (0x80134D20; called by tu_8044C7F0).
    void fn_80134D20(bool arg);
    // Owner: game/tu_80134C04.cpp (0x80136320; 8 calls in tu_803099FC).
    bool fn_80136320();
    // Owner: game/tu_80134C04.cpp (0x801365E8; called by tu_801B4D0C). Stores
    // 7 at this+0x50, 1 at this+0x58, starts the fade with c, stores a and b
    // in the selected stage and sets this+0x84.
    void fn_801365E8(s32 a, s32 b, s32 c);
    // Owner: game/tu_80134C04.cpp (0x801367E0). Stores r4 at this+0x4C; value 10
    // also resets the fade object (lbl_8053ACA0).
    void fn_801367E0(s32 value);
    // Stores r4 at this+0xFE (stb). When it is 1, clears this+0xFC (stb) and
    // this+0xF8 (stw). Defined in game/tu_80134C04.cpp (0x801365C8); no callers found yet.
    void fn_801365C8(u8 value);
    // Owner: game/tu_80134C04.cpp (next to fn_80134FBC; not written yet).
    // 1 unit. act_wait_switch calls it with (10, count) when its switch is
    // seen; r3 is gGameRoot_8053AAF8 (the only object this unit's calls pass).
    void fn_80134DE4(s32 a, s32 b);
    // Owner: game/tu_80134C04.cpp (0x80135020, 0x801352F4). Create / delete
    // the screen object of game mode `mode` (switch on 0-12; see 0x04-0x30).
    void fn_80135020(s32 mode);
    void fn_801352F4(s32 mode);
    // Owner: game/tu_80134C04.cpp (0x80135A04). Deletes the other modes'
    // screens, reports the heap and starts loading the archives of `mode`.
    void fn_80135A04(s32 mode);
    // game/tu_80134C04.cpp, called by main (tu_801344C0) only. The
    // constructor allocated with __nw__(0x11C); no arguments.
    GameRoot_8049EF20();
    // game/tu_80134C04.cpp (0x80138C9C), called once by main right after the
    // constructor. No further arguments.
    void fn_80138C9C();
    // game/tu_80134C04.cpp (0x80136828 and 0x80137300): main calls them each
    // frame once the loader is done, between beginRender / endRender and
    // endRender / endFrame; the first only while lbl_80529DEC's byte 0x251 is 0.
    void fn_80136828();
    void fn_80137300();
    // Owner: game/tu_80134C04.cpp (0x80138A30; not written yet). 107 units.
    // The number of players that take part: the set bits 0-3 of
    // mPlayerMask. Its body reads gGameRoot_8053AAF8 itself; callers load it into r3.
    s32 fn_80138A30();
    // game/tu_8022AD64.cpp (0x8023F98C): whether bit `player` of mPlayerMask is set.
    bool fn_8023F98C(s32 player);
    // Owner: not written yet (0x8013670C, 0x8C bytes). Called by
    // fn_8019CD9C (game/tu_80197070.cpp) with its s32 argument; r3 is
    // gGameRoot_8053AAF8 (the only object that unit's call passes).
    void fn_8013670C(s32 value);
    // Owner: game/tu_80134C04.cpp (0x80136500; called by tu_80139BE8). Counts
    // field_0xfc up to 60 frames, then field_0xf8 up by one, while field_0xfe
    // is 1 and none of field_0x94, field_0x96 or (with field_0x7e clear)
    // fn_8037F1E4(0) holds the game.
    void fn_80136500();
    // 0x80136364 (game/tu_80134C04.cpp): called by GameRootSub04's fn_80139D9C.
    // 0x80135494: applies the requested mode (field_0x50) once its archives are loaded.
    void fn_80135494(s32 mode);
    void fn_80136364();
    // Owner: game/tu_80134C04.cpp (0x8013665C; act_tingle_tower, tu_8044C7F0).
    // fn_801365E8(a, b, 0), then stage (10, 2) also sets lbl_80529DEC's 0x24B
    // (2 in mode 9, else 1) and 0x237.
    void fn_8013665C(s32 a, s32 b);
}; // Size: 0x11C

extern GameRoot_8049EF20* gGameRoot_8053AAF8;

// Owner: game/tu_801E667C.cpp (.sdata). Read by tu_80441318 and act_horse_race_man.
extern s32 lbl_8053A160;
// .sbss flags: game/tu_80197070.cpp (the ZLD2 actor) and act_dodongo; the
// player's state tick hurts the player while either is set.
extern u8 lbl_8053ABA8;
extern u8 lbl_8053ABF0;
// .sdata word in act_navi's data (0x8053A188-0x8053A198): fn_8023F78C stores -1 to it.
extern s32 lbl_8053A190;
// .sbss bytes (8) of game/act_all_enemies_defeated.cpp (unwritten), indexed by player
// in tu_80256C74 fn_8025785C (item 4 is refused while the player's byte is set).
extern u8 lbl_8053ABB8[8];

// The object fn_801902C0 returns: see game/GameRootSub20.h.

// The object fn_80119DD0 returns: a function-local static (lbl_804BC504,
// registered with __register_global_object) built on first use. Named after
// the function fn_80119DD0 registers with __register_global_object (as the
// static's destructor), fn_80119E5C. Owner: game/tu_80118384.cpp.
// 20+ units call fn_80119998 on it, e.g. game/tu_8012EA00.cpp
// passes (0x3C, 0, 1, 0, 1, 0x5A, 0).
class J2DPicture;
class cls_80119E5C {
public:
    // Inline in game/tu_80118384.cpp: zeroes the fields the static needs.
    cls_80119E5C();
    // The destructor fn_80119DD0 registers: deletes the picture.
    ~cls_80119E5C();

    /* 0x00 */ u8 field_0x0; // tested by tu_801286C0's update (states 1 and 3)
    /* 0x01 */ u8 field_0x1; // set to 1 by tu_801286C0's update (state 3)
    /* 0x02 */ u8 field_0x2; // fn_80119998 sets it to field_0x10 > 0
    /* 0x04 */ s32 field_0x4; // compared with 0xB3 by tu_801286C0's update
    /* 0x08 */ s32 field_0x8; // fn_80119998's first argument
    /* 0x0C */ s32 field_0xc;
    /* 0x10 */ s32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ s32 field_0x1c; // 0, 1 or 2 selects the effect set / colour step
    /* 0x20 */ s32 field_0x20;
    /* 0x24 */ s32 field_0x24; // 2 starts the effects 0x3EC-0x3EF / 0x3F0-0x3F3
    /* 0x28 */ s32 field_0x28;
    /* 0x2C */ J2DPicture* field_0x2c; // "kk_black.bti", made by fn_80119D48
    void fn_80119998(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    // 0x80119468 (game/tu_80118384.cpp): fn_80119998 with the fade length picked by `mode`.
    void fn_80119468(s32 g, s32 mode);
    // 0x80119958: resets every field but the picture.
    void fn_80119958();
    // 0x80119D48: makes the picture from "kk_black.bti" when there is none.
    void fn_80119D48();
    // 0x80119628 / 0x801194C4 (game/tu_80118384.cpp): the fade's frame step (not in
    // game modes 9 and 0x16) and its draw (the picture over the screen, alpha
    // field_0x14). GameRoot's fn_80136828 calls both, in this order, on fn_80119DD0().
    void fn_80119628();
    void fn_801194C4();
}; // Size: 0x30
// The function-local static (lbl_804BC504) of 0x80119DD0; game/tu_80118384.cpp.
cls_80119E5C* fn_80119DD0();
// Owner: game/tu_80134C04.cpp (0x80138AB0). No arguments; whether the input
// slot of player 0 (all four players when gGameRoot_8053AAF8->field_0x7e is 1) is
// connected (cls_804AA8C8::fn_8030A308).
bool fn_80138AB0();

// fn_80212ECC is cls_804A6340::fn_80212ECC (game/act/tu_80212E3C.h) and
// fn_802AC09C is ItemPedestal_804A82A8::fn_802AC09C (game/act/act_item_pedestal.h).

// AudioMgr_8045A080, the audio manager gAudioMgr_8053AF80 points to, is declared in
// game/Unk_8045A080.h (included by game/GameAudio.h).
extern AudioMgr_8045A080* gAudioMgr_8053AF80;

// game/tu_80401848.cpp: the DVD loader thread's commands. fn_804019A8,
// fn_80401BA4 and fn_80401E10 allocate one (0x20 / 0x1C / 0x1C bytes) for a
// file found by path and queue it on the thread; the thread calls each
// command's vfunc_0xC once. They are named after their vtables (the base
// 0x804B0FD0 is stored first, then the command's own).
// Base class (vtable 0x804B0FD0; vfunc_0xC is 0 there).
class cls_804B0FD0 {
public:
    /* 0x00 */ cls_804B0FD0* mNext; // the thread's command queue
    /* 0x04 */ u8 field_0x4;        // 1 once vfunc_0xC has run

    // The members come first: MWCC puts the vtable pointer where the first
    // virtual function is declared (0x8 in the original).
    inline cls_804B0FD0();
    /* 0x08 */ virtual ~cls_804B0FD0();
    /* 0x0C */ virtual BOOL vfunc_0xC() = 0;
    static inline void* operator new(size_t size);
    static inline void operator delete(void* p);
}; // Size: 0xC

// Loads a file into main RAM (JKRDvdRipper::loadToMainRAM).
class cls_804B0FA0 : public cls_804B0FD0 {
public:
    inline cls_804B0FA0(u8 direction);
    /* 0x08 */ virtual ~cls_804B0FA0();
    /* 0x0C */ virtual BOOL vfunc_0xC();

    /* 0x0C */ u8 mDirection; // 1: allocate from the head, else the tail; 0: lbl_8053A218
    /* 0x10 */ s32 mEntryNum;
    /* 0x14 */ void* field_0x14; // the loaded data
    /* 0x18 */ s32 mSize;
    /* 0x1C */ JKRHeap* mHeap;
}; // Size: 0x20

// Mounts a file as a JKRAramArchive (the given one, or a new one).
class cls_804B0FB0 : public cls_804B0FD0 {
public:
    inline cls_804B0FB0(u8 direction, JKRAramArchive* archive);
    /* 0x08 */ virtual ~cls_804B0FB0();
    /* 0x0C */ virtual BOOL vfunc_0xC();

    /* 0x0C */ u8 mDirection;
    /* 0x10 */ s32 mEntryNum;
    /* 0x14 */ JKRAramArchive* mArchive;
    /* 0x18 */ JKRHeap* mHeap;
}; // Size: 0x1C

// Mounts a file as a JKRMemArchive.
class cls_804B0FC0 : public cls_804B0FD0 {
public:
    inline cls_804B0FC0(u8 direction);
    /* 0x08 */ virtual ~cls_804B0FC0();
    /* 0x0C */ virtual BOOL vfunc_0xC();

    /* 0x0C */ u8 mDirection;
    /* 0x10 */ s32 mEntryNum;
    /* 0x14 */ JKRMemArchive* field_0x14; // the mounted archive
    /* 0x18 */ JKRHeap* mHeap;
}; // Size: 0x1C

// Owner: game/tu_80401848.cpp. (path, direction byte, heap); NULL when the
// path doesn't exist.
cls_804B0FA0* fn_804019A8(const char* path, u8 direction, JKRHeap* heap);
cls_804B0FB0* fn_80401BA4(const char* path, JKRAramArchive* archive, u8 direction, JKRHeap* heap);
cls_804B0FC0* fn_80401E10(const char* path, u8 direction, JKRHeap* heap);

extern "C" {
// JSystem/JMath/JMath.cpp: fast vector magnitude
f32 fn_80093280(const JGeometry::TVec3f* v);
// game/tu_801CB270.cpp (not written yet; clrlwi.): a map test of a point on `layer`.
bool fn_801CCBD0(const JGeometry::TVec3f* pos, s32 layer);
}
// game/tu_801CB270.cpp (0x801CCB1C; callers test it with clrlwi.): whether the tiles at a point
// (x, y - z) and 16 below it are both code 2 on `layer`.
bool fn_801CCB1C(const JGeometry::TVec3f* pos, s32 layer);

// Random helpers on gGameRoot_8053AAF8->mRandom, defined in game units.
f32 fn_80154744();          // [0, 1); game/act_sword_soldier.cpp
s32 fn_801CD664(s32 n);     // [0, n); game/tu_801CB270.cpp
s32 fn_801CD580(s32 except); // [0, 4) except `except`; game/tu_801CB270.cpp

// game/tu_801CB270.cpp: steps *angle towards target by at most step (a
// wrapper of fn_801CD970; the 0/1 it passes on is unused by all 9 callers).
void fn_801CD930(s16* angle, s32 target, s32 step);

// act_shadow_link_2.cpp (returns 0, no arguments). Callers (tu_801C5B08,
// tu_801C71BC) test the result as a byte (clrlwi. 24).
bool fn_801C5340();

// game/tu_801CB270.cpp: the stepper fn_801CD930 wraps; true when *angle
// reached target.
bool fn_801CD970(s16* angle, s32 target, s32 step);

// game/tu_801CB270.cpp function its source doesn't define yet.
extern "C" {
s32 fn_801CD398(s32 a, s32 b); // [0, 4) avoiding a (and b)
}

// game/tu_804023D0.cpp; called from game/tu_80376FA0.cpp and others.
// Returns lbl_80529DEC->field_0x10->getResource(type, name) (vtable slot
// 0x18); the callers pass the 4CC 'TIMG' and a .bti name.
void* fn_804032E8(u32 type, const char* name);

// game/act_block.cpp; called from game/tu_80376FA0.cpp.
// Returns 1 or 0 (li 1 / li 0); its body reads *pos and writes it back with
// psq on the 1 path. The layer goes to fn_802D7544 on GameView.
extern "C" {
u8 fn_8028D4DC(JGeometry::TVec3f* pos, s32 index, s32 layer);
}

// game/act_strange_block.cpp (not written yet); called from game/tu_80376FA0.cpp
// on one axis. Returns center + radius, or center - radius on the side pos is
// not on (the side test is on pos against center).
extern "C" {
f32 fn_8033D4AC(f32 pos, f32 center, f32 radius);
}

// Loads the TIMG resource `name` from gGameRoot_8053AAF8->field_0xa4 into texObj
// (and tlutObj, if given, for a palette texture); returns height / width.
// game/tu_801CB270.cpp; called from game/tu_801E9AFC.cpp.
f32 fn_801CB270(const char* name, GXTexObj* texObj, GXTlutObj* tlutObj);

// game/tu_801CB270.cpp; called from game/tu_801BEDBC.cpp. Whether the tile at pos
// is one of the codes 0x40-0x49.
bool fn_801CCC78(const JGeometry::TVec3f* pos, s32 layer);

// game/tu_801CB270.cpp; called from game/tu_801D2C60.cpp.
// Two effects (spawnEmitter_8013CC50 0x117 and 0x118) at pos on `layer`; arg3 is passed
// on and arg4 is passed on plus 1 (as the depth).
void fn_801CB538(s32 layer, const JGeometry::TVec3f* pos, s32 arg3, s32 arg4);

// game/tu_801CB270.cpp; called from game/tu_80207548.cpp and
// game/tu_80220F54.cpp. Forwards to ActorManager_804A5800::fn_801F8544 with
// 'RUPY' and the layer and pos; the flag selects 4 or 5 in the low bits of
// its last argument. Returns the result (an actor serial).
s32 fn_801CB434(s32 layer, const JGeometry::TVec3f* pos, bool flag);

// game/tu_801CB270.cpp; called from game/tu_801BD8C0.cpp.
// Maps a 16-bit angle to the one mirrored about the 0x4000 axes (a piecewise
// 0x4000 - a, a - 0x4000 negated, ... on the four quadrants). The body
// clrlwi 16s the argument for its unsigned compares and ends every path in
// extsh; the caller (0x801BD908) extends the result again, so the return
// type is an int.
s32 fn_801CB7CC(s16 angle);

// game/tu_801CB270.cpp; called from game/tu_801DE714.cpp. Whether fn_80230188
// holds for every player that takes part (fn_80138A30); takes no arguments.
bool fn_801CCF7C();

// The objects at gGameRoot2_8053AB10->0x2C and 0x30, named after their vtables
// (stored at 0 by their constructors); no members declared yet. 0x20
// (TileMap_804AA858) is in game/Map.h, 0x24 (Camera_804AA9E4) in game/Camera.h.
// 0x804A0160: game/tu_80158624.cpp, constructor fn_801594E8, 0x10 bytes.
// Read by fn_80158B3C / drawShadow_80158B5C and directly (+0x4, +0xC) in 28 units.
class JUTTexture;
class ShadowCircle_804A0160 {
public:
    // Constructor (0x801594E8, game/tu_80158624.cpp): loads "circle.bti" from
    // gGameRoot_8053AAF8->field_0xa4 into a new JUTTexture on `heap`. Its vtable
    // (lbl_804A0160) has one virtual destructor, at offset 0 (vptr).
    ShadowCircle_804A0160(JKRHeap* heap);
    virtual ~ShadowCircle_804A0160();

    /* 0x4 */ JUtility::TColor field_0x4; // colour: fn_80158B3C sets r,g,b = 0xFF, a = 0
    /* 0x8 */ JUTTexture* field_0x8;
    /* 0xC */ u8 field_0xc;      // act_post_warp_pillar_handling init stores 0xFF
    /* 0xD */ u8 field_0xd[3];   // not yet known

    // 0x80158624 (game/tu_80158624.cpp): draws a searchlight beam, a fan
    // from `top` to a ring of `radius` around `base`, then the disc at `base`
    // (act_searchlight, tu_8018A8D8). Skipped when 0xC is 0xFF.
    void fn_80158624(const JGeometry::TVec3f* top, const JGeometry::TVec3f* base, JUtility::TColor color, f32 radius);

    // 0x80158B3C (owner unit game/tu_80158624.cpp, not written yet). Resets
    // the colour bytes: 0x4-0x6 = 0xFF, 0x7 = 0, 0xC = 0xFF. `this` is
    // gGameRoot2_8053AB10->0x2C; called by tu_8039003C's constructor.
    void fn_80158B3C();

    // 0x80158B5C (owner unit game/tu_80158624.cpp, not written yet). Draws a
    // shadow-like sprite on `layer` (< 8) at *pos with a colour and scale;
    // `this` is gGameRoot2_8053AB10->0x2C. Called by tu_802201D8's draw (colour
    // 0,0,0,0x80; scale 24.0f). The colour is a TColor passed by value (a
    // stack copy, its address in r6).
    void drawShadow_80158B5C(s32 layer, const JGeometry::TVec3f* pos, JUtility::TColor color, f32 scale);

    // 0x8015945C (owner unit game/tu_80158624.cpp, not written yet). Resets the
    // colour and flag bytes 0x4-0x7, 0xC and 0xD-0xE of this object.
    void fn_8015945C();

    // 0x80159330 (owner unit game/tu_80158624.cpp, not written yet). Returns at
    // once when gGameRoot_8053AAF8->0x4C is 0xC; otherwise sets 0xE to 0xFF and, unless
    // the GameView check (getGameView_802D6A9C()->fn_802DCD0C()) passes, clears 0xD[0]
    // when gGameRoot_8053AAF8's 0x94 and 0x96 bytes are 0.
    void fn_80159330();

    // 0x80158F70 (owner unit game/tu_80158624.cpp, not written yet). Draws up
    // to three quads the size of the render mode (JUTVideo::sManager); `this`
    // is gGameRoot2_8053AB10->0x2C. Skipped when the colour is 0xFFFFFF00 and 0xC is 0xFF.
    void fn_80158F70();
    // 0x80158E88 (game/tu_80158624.cpp). Sets up the two lights; reads no
    // member. Called with this = gGameRoot2_8053AB10->0x2C by fn_8013BB18.
    void fn_80158E88();
}; // Size: 0x10
// 0x80543F28 (.sbss2, 8 bytes; no owner split yet). Its word is the colour
// that ShadowCircle_804A0160::fn_80158F70 (game/tu_80158624.cpp) puts in TEV register 1
// with its own alpha byte.
extern JUtility::TColor lbl_80543F28;
// 0x80543F20 (.sbss2, 8 bytes; no owner split yet). Its word is TEV register 1's
// colour in GameRoot2_8049F108::fn_8013B1E8 (game/tu_8013AAD4.cpp).
extern JUtility::TColor lbl_80543F20;
// 0x804A03A8: game/tu_8015F454.cpp, constructor fn_801602E8, 0x298 bytes.
// Its +0x284 is read in 56 places; fn_8015F8FC takes it.
// Only the flag and the float that tu_801286C0's update sets are known.
class cls_804A03A8;
// Owner: game/tu_8022AD64.cpp (0x80239D38).
// Free function, no arguments (reads globals, r3 is not read); returns 0 or 1
// (clrlwi 24); tested by GameRoot2_8049F108::fn_8013BBE0.
bool fn_80239D38();
// Owner: game/tu_8043A0E8.cpp (0x8043A170, defined there). Called by the
// player's update fn_80253E6C with its index.
void fn_8043A170(s32 player, bool force);
// Owner: game/tu_8043A0E8.cpp (0x8043A0E8, defined there). act_kaepora_gaebora's
// fn_80219AFC calls it with true.
void fn_8043A0E8(bool force);
class cls_804A03A8 {
public:
    // Constructor 0x801602E8 (called by GameRoot2_8049F108's constructor).
    cls_804A03A8(JKRHeap* heap);
    /* 0x000 vtable */
    // Virtual destructor: GameRoot2_8049F108's destructor calls vtable slot 2.
    virtual ~cls_804A03A8();
    // 0x8015F9C0, 0x8016005C, 0x801601E0 (game/tu_8015F454.cpp, not written
    // yet): called with this = gGameRoot2_8053AB10->0x30 by tu_8013AAD4.
    void fn_8015F9C0();
    // 0x8015F8FC (not written yet): (layer, position, 1) from the player's
    // update fn_80253E6C; layers 8 and up are ignored.
    u8 fn_8015F8FC(s32 layer, const JGeometry::TVec3f* pos, s32 mode);
    void fn_8016005C();
    void fn_801601E0();
    // Spot slots: the mode given to fn_8015F8FC, the position and the frames left.
    /* 0x004 */ s32 field_0x4[0x20];
    /* 0x084 */ JGeometry::TVec3f field_0x84[0x20];
    /* 0x204 */ s32 field_0x204[0x20];
    /* 0x284 */ u8 field_0x284; // 0 until set to 1 (tu_801286C0 update)
    /* 0x285 */ u8 field_0x285[0x288 - 0x285];
    /* 0x288 */ f32 field_0x288;
    /* 0x28C */ f32 field_0x28c; // set to 0.05f with the flag
    // circle.bti / circle2.bti
    /* 0x290 */ JUTTexture* field_0x290;
    /* 0x294 */ JUTTexture* field_0x294;
};

struct J3DDrawBuffer; // JSystem/J3D/J3DDrawBuffer.h

// *gGameRoot2_8053AB10: the second most-used singleton (267 units). Set in
// game/tu_8013AAD4.cpp (0x8013C024) and game/tu_80134C04.cpp (0x801393C4).
// Named after its vtable, 0x8049F108 (constructor fn_8013BFEC, which
// allocates every member below and stores it at the offset given).
struct GameRoot2_8049F108 {
    // Constructor (0x8013BFEC, game/tu_8013AAD4.cpp): stores itself in
    // gGameRoot2_8053AB10 and allocates every member below on a solid heap or on
    // `heap`. The vtable (0x8049F108) has only the destructor (0x8013BE60).
    GameRoot2_8049F108(JKRHeap* heap);
    virtual ~GameRoot2_8049F108();
    // JKRSolidHeap::create(-1, heap); holds the two draw buffers; destroyed
    // by the destructor.
    /* 0x04 */ JKRSolidHeap* field_0x4;
    // Two J3DDrawBuffers (initialize, allocBuffer(0x80) in the constructor,
    // 0x8013C0xx); fn_8013B618 calls frameInit on both.
    /* 0x08 */ J3DDrawBuffer* field_0x8;
    /* 0x0C */ J3DDrawBuffer* field_0xc;
    // new J2DOrthoGraph(...) (__ct__13J2DOrthoGraphFffffff, stored at
    // 0x8013C11C); 36 units pass it to J2DScreen::draw as the
    // J2DGrafContext (67 sites).
    /* 0x10 */ J2DOrthoGraph* field_0x10;
    // The second J2DOrthoGraph, same arguments (stored at 0x8013C190);
    // fn_8013BBE0 places and scissors it.
    /* 0x14 */ J2DOrthoGraph* field_0x14;
    // new J2DPrint(font from lbl_8053AD78->0x154, 0, font height) (0x8013C1F8).
    /* 0x18 */ J2DPrint* field_0x18;
    // new J2DPicture of "kk_black.bti" (0x8013C338), filled with a colour
    // through vtable slot 0x130.
    /* 0x1C */ J2DPicture* field_0x1c;
    /* 0x20 */ TileMap_804AA858* field_0x20;
    /* 0x24 */ Camera_804AA9E4* field_0x24;
    /* 0x28 */ EffectSystem_8049F178* field_0x28;
    /* 0x2C */ ShadowCircle_804A0160* field_0x2c;
    /* 0x30 */ cls_804A03A8* field_0x30;
    // new JUTTexture(efb width, efb height, 4) (0x8013C230).
    /* 0x34 */ JUTTexture* field_0x34;
    /* 0x38 */ u8 field_0x38;
    // Read as a byte by fn_80139CC4 and fn_80139D3C (game/tu_80139BE8.cpp);
    // set by fn_8013BBE0.
    /* 0x39 */ u8 field_0x39;

    // 0x8013B414 (game/tu_8013AAD4.cpp): field_0x24->fn_8030DD8C(box, light).
    void fn_8013B414(const JGeometry::TBox2f* box, bool light);
    // 0x8013B4C0: field_0x28->fn_8013D5B0(), then field_0x20->fn_802FF548().
    void fn_8013B4C0();
    // 0x8013B4F8: field_0x28->fn_8013D5D4(), frameInit on both draw buffers.
    void fn_8013B4F8();
    // 0x8013B71C: the per-frame draw pass (map, effects, sprites).
    void fn_8013B71C();
    // 0x8013BAF4: field_0x28->fn_8013DB2C().
    void fn_8013BAF4();
    // 0x8013BB18: unless field_0x39, field_0x20->fn_802FF3F0() and
    // field_0x24->fn_8030E150(); then fn_80158E88().
    void fn_8013BB18();
    // 0x8013BB64: field_0x24->fn_8030DFE8(false), the draw buffers into
    // j3dSys, and the camera matrix as j3dSys's view matrix.
    void fn_8013BB64();
    // 0x8013BBBC: field_0x28->fn_8013DC1C().
    void fn_8013BBBC();
    // 0x8013BBE0: per-frame update of the members.
    void fn_8013BBE0();
    // 0x8013BE04: resets the members for a new scene.
    void fn_8013BE04();

    // game/tu_8013AAD4.cpp: hand the video manager the NTSC interlaced
    // (lbl_8049F090) / progressive (lbl_8049F0CC) render mode. Members: the
    // only caller, Title_804AF820::fn_803A44D8 (game/tu_803A3484.cpp), loads
    // gGameRoot2_8053AB10 into r3 before each call.
    void fn_8013AAD4();
    void fn_8013AB00();

    // game/tu_8013AAD4.cpp (owner unit, not written yet). Calls
    // field_0x24->fn_8030DFE8(0); `this` is gGameRoot2_8053AB10. Called twice by
    // fn_80139CC4 (game/tu_80139BE8.cpp).
    void fn_8013B438();
    // 0x8013B460 (between fn_8013B438 and fn_8013B490; body not yet known).
    // Called with `this` = gGameRoot2_8053AB10 by ShadowCircle_804A0160::fn_80158F70
    // (game/tu_80158624.cpp), before its GX setup.
    void fn_8013B460();

    // game/tu_8013AAD4.cpp (owner unit, not written yet; same unit as the
    // constructor). Calls vtable slot 5 of field_0x10 (the J2DOrthoGraph);
    // `this` is gGameRoot2_8053AB10. Called by tu_803DA6B4's fn_803DA6B4 before
    // J2DScreen::draw.
    void fn_8013B490();
    // game/tu_8013AAD4.cpp. Called by GameRoot_8049EF20::fn_80137300
    // (tu_80134C04); calls field_0x20->fn_802FF0EC() in the modes
    // isGameModeSet() (game/GameMode.h) accepts.
    void fn_8013B538();
    // game/tu_8013AAD4.cpp. Called 12 times by tu_80134C04: the same mode
    // test as fn_8013B538, then field_0x28->fn_8013D5D4() and frameInit on
    // field_0x8 / field_0xc.
    void fn_8013B618();
    // game/tu_8013AAD4.cpp. GX vertex / channel / TEV setup for the map's
    // sprites (GXClearVtxDesc, GXSetVtxDesc 9 and 0xD, ...); no arguments.
    // Called by fn_802F5040 (game/tu_802F5040.cpp) after the matrix.
    void fn_8013B1E8();
    // game/tu_8013AAD4.cpp: field_0x24->fn_8030DD20(m) (r4 passed on).
    void fn_8013B3CC(const Mtx m);
    // game/tu_8013AAD4.cpp: field_0x24->fn_8030DD68().
    void fn_8013B3F0();
};

extern GameRoot2_8049F108* gGameRoot2_8053AB10;
// game/tu_8013AAD4.cpp (0x8013B004): fills box with color (an untextured
// quad); TileMap_804AA858::fn_802FB69C draws its panel tints with it.
void fn_8013B004(const JGeometry::TBox2f* box, const JUtility::TColor* color);

// The input object *lbl_8053AD30 (game/tu_803099FC.cpp, owner not written
// yet): named after its vtable, 0x804AA8C8 (constructor fn_8030AC4C, which
// stores it in lbl_8053AD30). Other members unknown.
class cls_804AA8C8 {
public:
    // fn_8030AC4C (constructor, 0x8030AC4C): stores the vtable and itself
    // in lbl_8053AD30; the destructor (fn_8030ABFC) clears it. Allocated with
    // 0xEC bytes by tu_803FF2C4.
    cls_804AA8C8();
    /* 0x08 */ virtual ~cls_804AA8C8();
    // Four-slot arrays: each fn_8030A43C round copies 0x4 -> 0x14, 0x84 -> 0x94,
    // 0x24 -> 0x84 (fn_8030AB38 clears them in a loop of four). Slot -0x3E7 means no pad.
    /* 0x04 */ u32 field_0x4[4];
    /* 0x14 */ u32 field_0x14[4];
    /* 0x24 */ u32 field_0x24[4];
    /* 0x34 */ u32 field_0x34[4];
    /* 0x44 */ s32 field_0x44[4][4]; // key-repeat counters per direction bit (fn_8030A078)
    /* 0x84 */ u32 field_0x84[4];
    /* 0x94 */ u32 field_0x94[4];
    /* 0xA4 */ u32 field_0xa4[4];
    /* 0xB4 */ u32 field_0xb4[4];
    /* 0xC4 */ u32 field_0xc4[4];
    /* 0xD4 */ u8 field_0xd4[4];
    /* 0xD8 */ u8 field_0xd8[4];
    /* 0xDC */ u8 field_0xdc[4];
    /* 0xE0 */ u8 field_0xe0[4];
    /* 0xE4 */ u8 field_0xe4[2];
    /* 0xE6 */ u8 field_0xe6[6];
    // Owner: game/tu_803099FC.cpp (0x80309A00 / 0x80309A38 / 0x80309A54 /
    // 0x80309A78 / 0x80309D48, same unit): accessors for the arrays above.
    bool fn_80309A00(s32 slot);
    u8 fn_80309A38(s32 slot);
    u32 fn_80309A54(s32 slot, u32 mask);
    u32 fn_80309A78(s32 slot, u32 mask);
    u32 fn_80309D48(s32 slot, u32 mask);
    // game/tu_803099FC.cpp (0x8030AB38): clears the arrays above (and two sbss
    // byte arrays); tu_803FF2C4 calls it right after the constructor.
    void fn_8030AB38();
    // game/tu_803099FC.cpp (0x8030A078): per slot, edge bits (pressed at 0x24, released at 0xA4) and key repeat (0x34).
    void fn_8030A078();
    // game/tu_803099FC.cpp (0x80309F20 / 0x80309FCC): the horizontal and
    // vertical stick value (-1, 0 or 1; scaled by 1/sqrt 2 on diagonals) built from
    // the button bits of the array at 0x4.
    f32 fn_80309F20(s32 slot);
    f32 fn_80309FCC(s32 slot);
    // Owner: game/tu_803099FC.cpp. Return the word at 4 + 4*slot (resp.
    // 0x24 + 4*slot) of the object ANDed with mask, or 0 for slot -0x3E7.
    // act_dummy_conversation's update tests the first for held buttons and
    // the second for fresh presses.
    u32 fn_80309D24(s32 slot, u32 mask);
    u32 fn_80309D6C(s32 slot, u32 mask);
    // Owner: game/tu_803099FC.cpp. A float for input slot `slot` (the
    // value at lbl_80541670 for slot -0x3E7, else from the bits of the word
    // at 4 + 4*slot); Player_804A6F38::fn_80246F28 returns it.
    f32 fn_80309D90(s32 slot);
    // Owner: game/tu_803099FC.cpp (0x80309C4C, same unit). Called by
    // cls_804B19F0::fn_80441A1C with mask 0x1300 and slot -1; returns a
    // pointer-sized word that is tested for zero.
    u32 fn_80309C4C(u32 mask, s32 slot);
    // Owner: game/tu_803099FC.cpp (0x8030A258 / 0x8030A2E4 / 0x8030A364 /
    // 0x8030A388, same unit). Each takes an input slot from getPlayerInputSlot_8030AAF4 and
    // returns a bool (byte) that cls_804B19F0::fn_80441A1C tests.
    u8 fn_8030A258(s32 slot);
    u8 fn_8030A2E4(s32 slot);
    u8 fn_8030A364(s32 slot);
    u8 fn_8030A388(s32 slot);
    // SHARED-EDIT (tu_803F0A54 fn_803FC304): owner game/tu_803099FC.cpp
    // (0x8030A308, same unit). Takes an input slot from getPlayerInputSlot_8030AAF4 and
    // returns a byte the caller tests after clrlwi 24 (the controller in
    // that slot is connected).
    u8 fn_8030A308(s32 slot);
    // Owner: game/tu_803099FC.cpp (0x8030A3B0, same unit). Called with a slot
    // from getPlayerInputSlot_8030AAF4 by Player_804A6F38::fn_80247564, result passed on.
    bool fn_8030A3B0(s32 slot);
    // SHARED-EDIT (tu_803F0A54 fn_803F8050): owner game/tu_803099FC.cpp
    // (0x80309A9C, same unit). Called like fn_80309C4C (r4 a button mask 4 /
    // 8, r5 an input slot); the result is tested with cmplwi 0.
    u32 fn_80309A9C(u32 mask, s32 slot);
    // 0x80309B74: the same "any player" test over field_0x4 (fn_80309D24).
    u32 fn_80309B74(u32 mask, s32 slot);
};
extern cls_804AA8C8* lbl_8053AD30;
// Owner: game/tu_80301CD0.cpp (its sbss, 0x8053AD34; dtk sized it 1 byte).
// game/tu_803099FC.cpp fn_8030AB38 clears four bytes from it in its loop.
extern u8 lbl_8053AD34[4];
// game/tu_803099FC.cpp (0x8030AAF4). Maps a player index (0-3) to an
// input slot (-0x3E7 for none); masks used: 0x100/0x200/0x400/0x20 and 1/2.
s32 getPlayerInputSlot_8030AAF4(s32 player);

// The object *lbl_8053AED0 (game/tu_8043A0E8.cpp, owner not written yet):
// named after its vtable, 0x804B1920 (constructor fn_8043F504, which stores
// it in lbl_8053AED0). Only the bytes at 0x1F0 are known (act_dummy_conversation
// stores 2 at 0x1F0 + player); its size and the rest are unknown.
class cls_804B1920 {
public:
    cls_804B1920(); // fn_8043F504 (game/tu_8043A0E8.cpp, not written yet)
    // Virtual destructor first: the vtable pointer goes where it is declared.
    virtual ~cls_804B1920(); // owner game/tu_8043A0E8.cpp (not written yet)
    // 0x8043D39C / 0x8043DB58 (tu_8043A0E8.cpp, not written yet): GameRoot's
    // per-frame draw (fn_80136828) calls both on lbl_8053AED0 after the game
    // view; the first tests gGameRoot_8053AAF8->field_0x7f, the second sets the
    // ortho context (fn_8013B490) and switches on the byte at 0x256.
    void fn_8043D39C();
    // 0x8043E80C: draws digit `digit` (0-9) at (x, y) scaled by `scale` in the two colours, with alpha.
    void fn_8043E80C(u32 digit, u8 alpha, u32 top, u32 bottom, f32 x, f32 y, f32 scale);
    void fn_8043DB58();
    /* 0x004 */ J2DScreen* field_0x4;
    /* 0x008 */ J2DScreen* field_0x8;
    /* 0x00C */ J2DScreen* field_0xc; // its pane 'Ndata05' is moved and drawn by fn_8043DC98
    /* 0x010 */ J2DPicture* field_0x10; // "TIMG" resource, built in the constructor (mode 3)
    /* 0x014 */ J2DPicture* field_0x14[5]; // fn_8043DB58 draws the one the byte at 0x256 selects
    /* 0x028 */ u8 pad_0x28[0x40 - 0x28];
    /* 0x040 */ PaneCtrl_804AF86C* field_0x40[12]; // the destructor's loop
    /* 0x070 */ PaneCtrl_804AF86C* field_0x70[2]; // text panes (fn_8043D034 copies field_0x78 into both)
    /* 0x078 */ char field_0x78[0xDC - 0x78]; // message text (fn_803837A4 in fn_8043D034); ends at the next known field
    // SHARED-EDIT (act_battle_royale_end_check fn_803D6AE8): a flag byte that
    // the battle royale end check ORs with 0x10 when it picks its winner.
    /* 0xDC */ u8 field_0xdc;
    /* 0xDD */ u8 field_0xdd;  // per-player bits (fn_8043A170), 0xF set by fn_8043A0E8
    /* 0xDE */ u8 field_0xde;  // per-player bits: message 0x1B5 already shown (fn_8043D67C)
    /* 0xDF */ u8 pad_0xdf;
    /* 0xE0 */ s32 field_0xe0; // fn_8043EDE8's argument
    /* 0xE4 */ s32 field_0xe4; // 0x1E after fn_8043EDE8, 0 at construction
    /* 0xE8 */ s32 field_0xe8;
    /* 0xEC */ f32 field_0xec; // where fn_8043A638 moves the stage picture
    /* 0xF0 */ f32 field_0xf0;
    /* 0xF4 */ f32 field_0xf4; // fn_8043A268's position
    /* 0xF8 */ f32 field_0xf8;
    /* 0xFC */ s32 field_0xfc;
    /* 0x100 */ s32 field_0x100;
    /* 0x104 */ s16 field_0x104; // countdown, stepped by fn_8043CEA0
    /* 0x106 */ s16 field_0x106;
    /* 0x108 */ s16 field_0x108; // fn_8043A268's s16 argument
    /* 0x10A */ u8 field_0x10a;
    /* 0x10B */ u8 field_0x10b; // selects the pane tested by fn_8043D514 / fn_8043D57C
    /* 0x10C */ u8 field_0x10c;
    /* 0x10D */ u8 field_0x10d;
    /* 0x10E */ u8 pad_0x10e[0x110 - 0x10E];
    /* 0x110 */ f32 field_0x110; // where fn_8043DC98 moves 'Ndata05' of field_0xc
    /* 0x114 */ f32 field_0x114;
    /* 0x118 */ u8 pad_0x118[0x1C0 - 0x118];
    /* 0x1C0 */ f32 field_0x1c0; // four floats set by fn_8043A208, zero at construction
    /* 0x1C4 */ f32 field_0x1c4;
    /* 0x1C8 */ f32 field_0x1c8;
    /* 0x1CC */ f32 field_0x1cc;
    /* 0x1D0 */ u8 pad_0x1d0[0x1F0 - 0x1D0];
    /* 0x1F0 */ u8 field_0x1f0[4]; // per player
    /* 0x1F4 */ u8 field_0x1f4[0x254 - 0x1F4]; // not yet known
    /* 0x254 */ u8 field_0x254; // set to 1 by fn_8043A208
    /* 0x255 */ u8 field_0x255; // cleared by fn_8043EDF8
    // SHARED-EDIT (act_battle_royale_end_check draw, 0x256 = the byte its
    // mFocus is stored into when mFocus is not 0xFF).
    /* 0x256 */ u8 field_0x256;
    /* 0x257 */ u8 field_0x257; // gGameRoot_8053AAF8->field_0x85 != 0, set by fn_8043D8B4
    /* 0x258 */ J2DPicture* field_0x258[10]; // the digit pictures fn_8043E80C draws
    /* 0x280 */ J2DPicture* field_0x280[4];
    /* 0x290 */ cls_804B13D4* field_0x290; // used when gGameRoot_8053AAF8->field_0x7f is set
    /* 0x294 */ cls_804B146C* field_0x294; // used otherwise
    // The objects fn_8043F504 allocates (by constructor); fn_8043EDF8 resets
    // each one that exists.
    /* 0x298 */ cls_804B1AE8* field_0x298; // also lbl_8053AEF0
    /* 0x29C */ cls_804B1BB0* field_0x29c;
    /* 0x2A0 */ cls_804B1BE8* field_0x2a0;
    /* 0x2A4 */ cls_804B1C20* field_0x2a4;
    /* 0x2A8 */ cls_804B1C58* field_0x2a8;
    /* 0x2AC */ cls_804B1C90* field_0x2ac;
    /* 0x2B0 */ cls_804B1CC8* field_0x2b0;
    /* 0x2B4 */ cls_804B1D00* field_0x2b4;
    /* 0x2B8 */ cls_804B1D20* field_0x2b8; // also lbl_8053AF38
    /* 0x2BC */ cls_804B1E1C* field_0x2bc; // also lbl_8053AF50

    // 0x8043D67C (game/tu_8043A0E8.cpp): 27 units call it with *lbl_8053AED0
    // in r3, an id in r4 (tu_8020FE50's actor passes 0x1A8 or 0x1C1) and two
    // more arguments (0 in most calls). Starts message `id` unless one is
    // showing (field_0x10a) or it was shown once already; returns whether it
    // did (li r3, 0 / 1; no caller reads it).
    bool fn_8043D67C(s32 id, u32 arg1, u32 arg2);

    // 0x8043A208 (game/tu_8043A0E8.cpp): when field_0x10 exists, stores the four
    // floats at 0x1C0, sets its alpha (vtable slot 0x24) and flags 0x254.
    void fn_8043A208(u8 alpha, f32 a, f32 b, f32 c, f32 d);
    // 0x8043ABCC (game/tu_8043A0E8.cpp): shows the rank picture `index` (0-3, clamped
    // at 0) on the pane "Pdf01" of field_0x8.
    void fn_8043ABCC(s32 index);
    // 0x8043CEA0 (game/tu_8043A0E8.cpp): counts field_0x104 down; at 0 (or when
    // GameRoot's mode is not 7) resets the twelve panes' step counters, state 3.
    void fn_8043CEA0();
    // 0x8043CDA0, 0x8043CF40, 0x8043D034 (game/tu_8043A0E8.cpp): the steps of
    // fn_8043D644's table, picked by field_0x10b (3, 1 and 0; fn_8043CEA0 is 2).
    void fn_8043CDA0();
    void fn_8043CF40();
    void fn_8043D034();
    // 0x8043A638 (game/tu_8043A0E8.cpp): fades the stage picture in and out
    // while field_0x106 counts, then draws field_0x8.
    void fn_8043A638();
    // 0x8043ACE4 and 0x8043BB84 (game/tu_8043A0E8.cpp, not written yet): draw
    // steps fn_8043DC98 calls with only the object.
    void fn_8043ACE4();
    void fn_8043BB84();
    // 0x8043A268 (game/tu_8043A0E8.cpp): fn_8043A450 that also stores a
    // position and a value; tu_802E9428 calls it from its tables.
    void fn_8043A268(s32 stage, s32 sub, s16 value, f32 x, f32 y);
    // 0x8043A450 (game/tu_8043A0E8.cpp, not written yet): a stage / sub-stage
    // pair (GameState's 0x118 / 0x11C).
    void fn_8043A450(s32 stage, s32 sub);
    // 0x8043A608 (game/tu_8043A0E8.cpp): fn_8043A450 for the selected stage.
    void fn_8043A608();
    // 0x8043D834 (game/tu_8043A0E8.cpp): picks the object by field_0x7f.
    void fn_8043D834();
    // 0x8043DF24 (game/tu_8043A0E8.cpp): this is unused. True when (px, py) is
    // outside the box around (x, y), grown by margin.
    bool fn_8043DF24(f32 x, f32 y, f32 px, f32 py, f32 margin);
    // 0x8043E398 (act_giant_bomb calls it with two world positions): projects them to the screen and
    // sets the bits of field_0xdc (1, 2, 4, 8) for the four panels (fn_8043DF24 boxes) the span's centre is inside.
    void fn_8043E398(const Vec* a, const Vec* b);
    // 0x8043DF8C (game/tu_8043A0E8.cpp, not written yet): takes a screen x, y.
    void fn_8043DF8C(f32 x, f32 y);
    // 0x8043EDE8 (game/tu_8043A0E8.cpp): field_0xe0 = arg, field_0xe4 = 30.
    void fn_8043EDE8(s32 arg);
    // 0x8043EDF8 (game/tu_8043A0E8.cpp): clears the flag bytes and resets the
    // objects at 0x290 / 0x294 and 0x298..0x2B4; GameRootSub04_801F856C::fn_8013A1A4
    // calls it.
    void fn_8043EDF8();

    // 0x8043D5F0 (game/tu_8043A0E8.cpp, not written yet): takes no arguments
    // besides the object; fn_80139C5C calls it, then fn_8038D4A8.
    void fn_8043D5F0();

    // 0x8043D874 (game/tu_8043A0E8.cpp, not written yet): called with the id in
    // r4, a value in r5 (a table entry or 300 in act_shooting_gallery) and 0 in r6.
    void fn_8043D874(s32 id, s32 arg1, u32 arg2);

    // 0x8043CB24 (game/tu_8043A0E8.cpp): queues the decimal number `value`
    // for drawing with `digits` digits (r7), centred around (x, y) at `scale`.
    // Returns false when all ten slots (the byte at 0x252) are taken. r5 is
    // stored as a byte at 0x244 + slot, r6 (at most 5) at 0x23A + slot, and r8
    // (`flag`) switches the leading-zero handling. act_digging_game and
    // tu_8010089C call it too; tu_801ED3DC passes (n, 255, 0, 1, false).
    bool fn_8043CB24(s32 value, u8 arg2, u8 arg3, u8 digits, bool flag, f32 x, f32 y, f32 scale);

    // 0x8043E7CC (game/tu_8043A0E8.cpp): act_gba_appear's update calls it with a
    // position passed by value.
    void fn_8043E7CC(JGeometry::TVec3f pos);

    // 0x8043D514 (game/tu_8043A0E8.cpp, not written yet): returns a float in
    // f1 with only `this` (*lbl_8053AED0) in r3. game/tu_8044998C.cpp passes
    // it as y to PaneCtrl_804AF86C::moveFromCentre_80402E08 on both of its panes.
    f32 fn_8043D514();

    // 0x8043D57C (game/tu_8043A0E8.cpp, not written yet): like fn_8043D514,
    // returns a float in f1 with only `this` (*lbl_8053AED0) in r3.
    // game/tu_804455E0.cpp's fn_804455E0 passes it as y for the slots 2 and 3.
    f32 fn_8043D57C();

    // 0x8043D644 and 0x8043D8B4 (game/tu_8043A0E8.cpp, not written yet): no
    // arguments besides the object; fn_80139D70 calls them in turn.
    void fn_8043D644();
    // fn_8043D8B4 sends each player present to fn_804408F0, stores the per-player
    // words of lbl_80529DEC at 0xC8 / 0xF8 and re-initialises the objects at
    // 0x290 / 0x294 and 0x298..0x2BC.
    void fn_8043D8B4();
    // 0x8043DC98 (game/tu_8043A0E8.cpp, not written yet): no arguments besides
    // the object; GameRootSub04_801F856C::fn_80139C84 calls it first.
    void fn_8043DC98();
};
extern cls_804B1920* lbl_8053AED0;

// The 0x60-byte object fn_80439C78 builds (game/tu_80438C68.cpp, not written
// yet), stored at GameRootSub04_801F856C::field_0x18. Named after its vtable,
// 0x804B17A0; it keeps a J2DScreen at 0x4 and its layout is not known beyond
// the destructor.
class cls_804B17A0 {
public:
    cls_804B17A0(); // fn_80439C78 (game/tu_80438C68.cpp)
    /* 0x000 */ // vtable, whose one slot is the destructor below
    virtual ~cls_804B17A0(); // owner game/tu_80438C68.cpp (not written yet)
    // 0x804399A0 (not written yet): sets the ortho context (fn_8013B490) and
    // draws; GameRoot's fn_80136828 calls it in screen mode 11.
    void fn_804399A0();
    // 0x80439AB0 (game/tu_80438C68.cpp): the state reset; also what fn_80438E10(flag = 0) does.
    void fn_80439AB0();
    // 0x80438E10: stores flag in field_0x40, then resets unless it is set.
    void fn_80438E10(u8 flag);
    // 0x80438C68: the per-frame state machine (field_0x58): spawns the TNGL actor, then the message and the DETH sweep.
    void fn_80438C68();
    /* 0x04 */ J2DScreen* field_0x4;
    /* 0x08 */ J2DAnmBase* field_0x8;
    /* 0x0C */ JPABaseEmitter* field_0xc[10]; // 0xC..0x30; the last two are single emitters
    /* 0x34 */ J2DPicture* field_0x34;
    /* 0x38 */ s32 field_0x38;
    /* 0x3C */ s32 field_0x3c;
    /* 0x40 */ u8 field_0x40; // state; 3 makes fn_804399A0 raise lbl_8053ADC8's flags
    /* 0x41 */ u8 field_0x41;
    /* 0x42 */ u8 field_0x42;
    /* 0x43 */ u8 field_0x43;
    /* 0x44 */ s32 field_0x44;
    /* 0x48 */ s32 field_0x48;
    /* 0x4C */ f32 field_0x4c;
    /* 0x50 */ f32 field_0x50;
    /* 0x54 */ s16 field_0x54;
    /* 0x56 */ s16 field_0x56;
    /* 0x58 */ u8 field_0x58;
    /* 0x59 */ u8 field_0x59;
    /* 0x5A */ u8 field_0x5a;
    /* 0x5B */ u8 field_0x5b;
    // An alpha: TileMap_804AA858::fn_802FB69C draws black boxes with it while paused.
    /* 0x5C */ u8 field_0x5c;
    // SHARED-EDIT: tested by fn_801914C4 (game/tu_801902C0.cpp) through lbl_8053AEC8.
    /* 0x5D */ u8 field_0x5d;
    // fn_8041E3B4 (game/tu_80418B64.cpp) tests it for 1 and clears it.
    /* 0x5E */ u8 field_0x5e;
    /* 0x5F */ u8 field_0x5f;
};
// .sbss pointer set by cls_804B17A0's constructor fn_80439C78 (0x80439CAC).
extern cls_804B17A0* lbl_8053AEC8;

// The 4-byte pointer at .sbss lbl_8053AB00: the heap GameRootSub04_801F856C's
// constructor (fn_8013A538, game/tu_80139BE8.cpp) stores, which the destructor
// asks for getTotalFreeSize between its steps.
extern JKRHeap* lbl_8053AB00;

// game/act_giant_bomb.cpp (0x80341CB0, not written yet); called from
// game/tu_8020FE50.cpp. Spawns a 'BBMB' actor (spawnActor_801F9484) on `layer` at
// *pos.
extern "C" {
void fn_80341CB0(s32 layer, const JGeometry::TVec3f* pos);
}

// game/tu_8037DF68.cpp. Shows message msg in the message manager's text box
// at (x, y) with a width x height font in colour color (copied into the text
// box), or scrolling in from the right screen edge over `frames` frames when
// that is not 0; skipMask (a byte at 0xE5) are the players whose 0x100 press
// closes it. Returns false while a text box is already shown. Callers pass
// mPos.x, mPos.y, mPos.z, mPos.z.
bool fn_8037F2FC(u32 msg, s32 frames, u8 skipMask, f32 x, f32 y, f32 width, f32 height,
                 JUtility::TColor color);

// game/tu_80301CD0.cpp (per-player lists, not written yet). ~100 calls in ~90
// units, mostly from actors' draw: serial, layer, position, then a sprite id.
// The callee converts the byte d to a 0/1 flag (clrlwi, neg, or, srwi), tests g as a bool
// and reads the two stack arguments as bytes (lbz 0x4b, 0x4f); returns a 0/1 flag. Callers
// that hold d in a wider variable pass `x != 0`. Argument meanings unknown. Review: fsa-wt/_reviews/game__act_force_gem_shadow.review.md.
bool fn_803075AC(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, u8 d, s32 e,
                 s32 player, bool g, u8 h, u8 i);

// game/tu_80301CD0.cpp (0x80301CD0, written there with C++ linkage): whether the
// GBA linked to the player on `layer` (fn_8022F340) is still busy. tu_8028FBA0's
// fn_80291D28 waits on it.
bool fn_80301CD0(s32 layer);

// game/tu_80301CD0.cpp (not written yet). 18 units; same leading arguments as
// fn_803075AC: serial, layer, position, then a sprite id. Adds 0x200 to the
// id only when e == 2; ids below 0x45 then go to fn_80307830, the others get
// 0x400 added and go to fn_803075AC. d and e are converted to 0/1 flags for
// the callee. The result is unused.
void fn_80307AC8(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, s32 d, s32 e,
                 s32 f);

// game/tu_80301CD0.cpp. Same leading arguments as fn_803075AC (serial, layer,
// position, sprite id, then d as a byte, e, f and g as a byte masked with 3);
// returns 0 below layer 8. game/tu_801D3CBC.cpp calls it with (0, 1, 0, player).
bool fn_80307830(s32 serial, s32 layer, const JGeometry::TVec3f* pos, s32 id, u8 d, bool e,
                 s32 f, u8 g);

class ActorBase_804A5770;

// The 0x20-byte object in .bss at 0x804BC540 (function-local static of
// fn_8011921C, built on first use); named after its destructor fn_801192AC. Only the fields
// fn_801186B0 writes are known.
struct cls_801192AC {
    /* 0x00 */ u8 field_0x0;
    /* 0x01 */ u8 field_0x1;
    /* 0x02 */ u8 field_0x2;
    /* 0x04 */ s32 field_0x4; // compared as signed in tu_801286C0's vfunc_0x54
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ u8 field_0xc[0x4]; // not yet known
    // 0x801190E4 (game/tu_80118384.cpp, not written yet): makes room for 20
    // entries (new 0x50) in [mBegin, mEnd) when field_0x18 is below 20.
    // GameRoot's fn_80138C9C calls it on fn_8011921C().
    void fn_801190E4();
    // The actor list: fn_8011888C appends and fn_80118798 removes entries,
    // fn_8011871C searches [mBegin, mEnd) for an actor by mActorId.
    /* 0x10 */ ActorBase_804A5770** mBegin;
    /* 0x14 */ ActorBase_804A5770** mEnd;
    /* 0x18 */ u8 field_0x18[0x8]; // not yet known

    // 0x80118384 (game/tu_80118384.cpp): getActorManager_801F856C()->fn_801F6884(); field_0x0 = 0.
    void fn_80118384();
    // 0x80118E20 (game/tu_80118384.cpp, not written yet): called on fn_8011921C() by GameRootSub04's fn_80139D9C.
    void fn_80118E20();
    // 0x801183BC: field_0x1 = 0, and the same in the fade object fn_80119DD0 returns.
    void fn_801183BC();
    // 0x80118454: field_0x1 = 1, and the same in the fade object fn_80119DD0 returns.
    void fn_80118454();
    // Sets field_0x0 = 1, field_0x1 = field_0x2 = 0, field_0x4 = value, field_0x8 = 0
    // unless fn_801F68E0 (after getActorManager_801F856C) returns 0; returns whether it did. Called
    // with 2 from act_four_sword_demo.
    bool fn_801186B0(u32 value);
    // 0x801184F0 (0x1C0 bytes, not written yet). Called on fn_8011921C() with a
    // flag (1 from game/tu_80139BE8.cpp, 0 from act_opening_goal's vfunc_0x54
    // and tu_801286C0); the flag is tested as a byte.
    void fn_801184F0(bool flag);
    // 0x8011871C (not written yet). Returns the first entry at or after
    // `from` whose actor's mActorId (0x1A0) is `actorId`, or mEnd if none.
    ActorBase_804A5770** fn_8011871C(ActorBase_804A5770** from, u32 actorId);
    // 0x8011888C / 0x80118798 (not written yet). Register / remove an actor (its
    // serial at 0x198 is stored in / removed from the list at 0xC..0x14); called
    // by cls_8049D140's init and destructor (tu_8010DF44) and act_opening_goal.
    void fn_8011888C(ActorBase_804A5770* actor);
    void fn_80118798(ActorBase_804A5770* actor);
}; // Size: 0x20

// game/tu_80118384.cpp (not written yet). Returns the static cls_801192AC.
extern "C" cls_801192AC* fn_8011921C();

// game/tu_801C0D70.cpp. The 0x60-byte object in .bss at 0x804C7E70, built on
// first use by fn_801C0D70 (guard byte lbl_8053AC10); named after that
// function. It tracks the Shadow Link fight: field_0x0 holds state bits, the
// serials of the 'SLK2' actors it spawned are in mSerials[0..mCount), and
// field_0x50 is the serial of the generator (-1 when there is none).
class ShadowLink2_804A43E8;
struct cls_801C0D70 {
    cls_801C0D70(); // game/tu_801C0D70.cpp (inlined into the getter)
    /* 0x00 */ u16 field_0x0;
    /* 0x02 */ u16 field_0x2;
    /* 0x04 */ u16 field_0x4;
    /* 0x06 */ u16 field_0x6;
    /* 0x08 */ u16 field_0x8;
    /* 0x0C */ s32 mCount;
    /* 0x10 */ s32 mSerials[16];
    // act_shadow_link_2's fn_801C53F8 tests bit 0x80000000.
    /* 0x50 */ u32 field_0x50;
    /* 0x54 */ JGeometry::TVec3f field_0x54;
    // Owner tu_801C0D70. True when a target for the player at *pos was found;
    // writes its x/y to *out. Called by cls_8021C2B4::fn_8021C468 and act_shadow_link_2.
    bool fn_801C0FEC(const JGeometry::TVec3f* pos, JGeometry::TVec2f* out) const;
    // Owner tu_801C0D70 (symbol fn_801C12B8). Called by act_shadow_link_2's
    // fn_801C53F8 on the object from fn_801C0D70: marks one of the actors.
    void fn_801C12B8();
    void fn_801C0DCC();
    void fn_801C115C();
    void fn_801C11A4();
    bool fn_801C11F0();
    void fn_801C1224(s32 value) const;
    bool fn_801C1314();
    void fn_801C1374(bool flag);
    void fn_801C1440(s32 serial);
    u16 fn_801C14A8(s32 serial);
    void fn_801C151C();
}; // Size: 0x60
// Getter of that object (returns its address).
extern "C" cls_801C0D70* fn_801C0D70();

// game/tu_80204638.cpp (owner unit, not written yet). The guarded static in
// .bss at 0x804CADE0 (constructed on first use by fn_80204638, flag lbl_8053AC58);
// named after that function. Only the byte at 0x0 is known: act_bandit_herding_npc's
// update sets it to 1 (and the actor manager's reset functions call the getter).
class ActorBase_804A5770;
struct cls_80204638 {
    // The inlined constructor (fn_80204638's first call): zeroes everything, the TVec3f array
    // is built by __construct_array.
    cls_80204638();
    // 0x802046FC: records `pos` as the next entry; with earlier ones, the nearest is
    // pushed away through fn_801CB85C first. `vel` is passed on the same way.
    bool fn_802046FC(JGeometry::TVec3f* vel, JGeometry::TVec3f* pos);
    // 0x80204834: clears mAlive of every actor in the table.
    void fn_80204834();
    // 0x802048F4: drops the layer-0 actors in the table that left the view box.
    void fn_802048F4();
    // 0x80204ABC: with reset, clears the byte at 0x0 and the words 0x4-0x50 (the table;
    // GameView_804A9950::fn_802D8328 passes its first flag).
    void fn_80204ABC(bool reset);
    // 0x802049E0. Removes the actor from the slot the byte
    // at its 0x34a selects in the table (0x4 + 8 * slot): clears the first
    // slot holding it, else the second. Called by act_thief's destructor on
    // fn_80204638()'s result.
    void fn_802049E0(ActorBase_804A5770* actor);
    // 0x80204A20: puts the actor in the first free half of its slot.
    void fn_80204A20(ActorBase_804A5770* actor);

    /* 0x00 */ u8 field_0x0;
    /* 0x04 */ ActorBase_804A5770* mSlots[10][2];
    /* 0x54 */ s32 mCount;
    /* 0x58 */ JGeometry::TVec3f mPositions[10];
    // The .bss symbol is 0x1D0 bytes (the JAudio pools registered by __sinit follow at 0xD0).
};
// Getter of that object (returns its address).
extern "C" cls_80204638* fn_80204638();


// Sequencer_801DAA24::fn_801DA938 / fn_801DA904 are in game/act/tu_801D8400.h;
// cls_804A4F90::fn_801DC264 (BOIT) and cls_804A6708::fn_8021D734 / fn_8021D764 /
// fn_8021D794 (NAVI) in game/act/tu_801F9484.h.

// A shared .sdata2 constant (1.0f), owned by game/act_shadow_link_generator's
// .sdata2 (8053EDF0); act_shadow_link_2's helper loads it and subtracts 0.001f at run time.
extern const f32 lbl_8053EDF0;
// The next one (64.0f, 8053EDF4), same owner; act_shadow_link_2's
// game/tu_801C6A4C.cpp loads it and squares it at run time.
extern const f32 lbl_8053EDF4;
// The next three (s32 constants at 8053EDF8, 8053EDFC, 8053EE00; owners:
// act_shadow_link_generator's .sdata2 for the first two, the following unit
// for the last): act_shadow_link_2's game/tu_801D3CBC.cpp converts each to
// float at run time as the divisor of a fade (255 * mStateTimer / value).
extern const s32 lbl_8053EDF8;
extern const s32 lbl_8053EDFC;
extern const s32 lbl_8053EE00;

// game/act_shadow_link_2.cpp (owner unit, defined there). Packs five fields
// into the u32 an 'SLK2' spawn takes as its arg_0x90: a in bits 0-4, b in
// 5-7, c in 8-11, d in 12-15 and e in 20-27. act_shadow_link_generator passes
// (8, 0, slot, 0, 0) for each spawn.
u32 fn_801C5660(u32 a, u32 b, u32 c, u32 d, u32 e);

// Shared .sdata2 constants owned by game/tu_801CB270.cpp (not written yet),
// global consts read by several Shadow Link and enemy units: the gravity step
// (act_shadow_link_2 vfunc_0x84, act_shadow_link_jump, act_terrorpin,
// act_frostare_tail, tu_801D8400) and the fall-speed limit (act_shadow_link_2,
// tu_801DE714).
extern const f32 lbl_8053EFB8;
extern const f32 lbl_8053EFBC;

// Shared .sdata2 constants defined in game/act_vaati_boss.cpp:
// 90.0f and 1.7f. game/tu_801EDBA4.cpp passes the first to fn_802F5FF4 and
// multiplies -12.0f by the second at run time (fmuls), so neither is a literal.
extern const f32 lbl_8053F0B8;
extern const f32 lbl_8053F0BC;

// Owner: game/tu_802F5040.cpp (defined there). f1 in, f1 out: 192 / (192 -
// min(x, 96)) (0x802F5FF4); game/tu_801EDBA4.cpp scales by it.
f32 fn_802F5FF4(f32 x);

// Owner: game/tu_80301CD0.cpp
bool fn_80304C10(s32 player, const char* name, u32 address, u8 arg3, s32 arg4);

// Objects and functions of the boot sequence main (game/tu_801344C0.cpp)
// runs; their owner units are not written yet. Named after their vtables.

// The loader (0x7C bytes; vtable 0x804B0F50, constructor fn_804009CC),
// owner game/tu_803FF2C4.cpp. main allocates it in the main heap and calls
// fn_803FF420 / fn_803FFAA8 each frame while mState is below 4, then
// fn_803FF2C4 (which frees its archives) once it reaches 4.
class cls_804B0F50 {
public:
    cls_804B0F50();
    void fn_803FF2C4();
    void fn_803FF420();
    void fn_803FFAA8();

    /* 0x00 */ void* field_0x0; // vtable 0x804B0F50, kept opaque: its slots are not known yet
    /* 0x04 */ u8 field_0x4[0x78 - 0x4];
    /* 0x78 */ u8 mState;
    /* 0x79 */ u8 field_0x79; // fn_802B1610 tests it
    /* 0x7A */ u8 field_0x7a[0x7C - 0x7A];
}; // Size: 0x7C
extern cls_804B0F50* lbl_8053AAF0;

// The memory card manager, owner game/tu_803B9940.cpp (0x2020 bytes at
// lbl_8051C1A0, 0x2020 bytes, vtable 0x804AFB48 holds only the destructor): fn_803BA980
// runs CARDInit, initialises its mutex and condition and starts its thread
// (fn_803B9F40 is the thread's loop); main calls fn_803B9E2C on it every frame.
// The game side posts a request in field_0x1fcc under the mutex and signals
// the condition; the thread works it off and leaves the result in mState.
class cls_803B9940 {
public:
    virtual ~cls_803B9940();
    void fn_803BA980();
    void fn_803B9E2C();
    void fn_803B9F40();
    // SHARED-EDIT (tu_803BC6F0): request polls; each try-locks the mutex
    // and reads mState. fn_803B9A94 copies the save
    // data at this + offset + 0x20 / + 0x280 into dst1 / dst2 when the state
    // is 3; fn_803B9BDC / fn_803B9C5C return 1 when the request finished
    // (state 4 / 5), 0 or 2 otherwise.
    // SHARED-EDIT (tu_803BC6F0 fn_803CE11C): the card state (mState
    // mapped through a 14-entry switch; 0xE while the mutex is busy). The
    // argument is unused (every caller passes 0).
    u32 fn_803B9940(s32 unused);
    // Hands back the save data (state 3 -> 1): 1 when the state was 3, 2
    // when it was not, 0 when the mutex was busy.
    s32 fn_803B9A24();
    // Sets the request word 0x1FCC to 1 and signals the condition.
    void fn_803B9B8C();
    s32 fn_803B9A94(void* dst1, u32 size1, void* dst2, u32 size2, s32 offset);
    // Sets the request word to 2 and signals the condition.
    void fn_803B9B3C();
    s32 fn_803B9BDC();
    s32 fn_803B9C5C();
    // Copy src into the save data at offset + 0x20 (n bytes), request 3.
    void fn_803B9CCC(void* src, u32 n, s32 offset);
    // Copy src1 / src2 into the save data at offset + 0x20 / + 0x280, request 3.
    void fn_803B9D54(void* src1, u32 n1, void* src2, u32 n2, s32 offset);
    // Sets the request word 0x1FCC to 4 and signals the condition at 0x1FEC
    // (fn_803CC17C, before it waits in state 0xD).
    void fn_803B9DDC();

    /* 0x0004 */ u8 field_0x4[0x20 - 0x4];
    /* 0x0020 */ u8 mHeader[0x260];
    /* 0x0280 */ u8 mSave[0x1FCC - 0x280];
    // fn_802B1558 (the reset path) waits, one VIWaitForRetrace per pass, while
    // this word is nonzero.
    /* 0x1FCC */ s32 field_0x1fcc;
    /* 0x1FD0 */ s32 mState;
    /* 0x1FD4 */ OSMutex mMutex;
    /* 0x1FEC */ OSCond mCond;
    // SHARED-EDIT (tu_803BC6F0 fn_803CC53C): the card channel passed to
    // CARDGetSerialNo.
    /* 0x1FF4 */ u8 field_0x1ff4;
    // SHARED-EDIT (tu_803BC6F0): fn_803CE9EC stores 2 here every frame;
    // fn_803CBE38 / fn_803CC3DC copy it into the save data's serial
    // (lbl_8050EC80.field_0x8, game/tu_803B8C74.h) after a load.
    /* 0x1FF5 */ u8 field_0x1ff5;
    /* 0x1FF6 */ u8 field_0x1ff6[0x1FF8 - 0x1FF6];
    /* 0x1FF8 */ u64 field_0x1ff8; // CARDGetSerialNo's serial number (fn_803CC53C)
    /* 0x2000 */ u8 field_0x2000[0x2020 - 0x2000];
}; // Size: 0x2020
extern cls_803B9940 lbl_8051C1A0;

extern "C" {
// Owner: game/tu_803B8C74.cpp. Tests flag `index` (0..7)
// of save file `file` in lbl_8050EC80; returns 1 or 0. FileSelect_804AFF60's
// constructor reads flags 0..7 of file 0.
bool fn_803B8CAC(s32 file, s32 index);
// Owner: game/tu_803B8C74.cpp. True for index < 3, else whether save file
// `file`'s flag word (lbl_8050EC80.mFiles[file].mFlags) has its top bit set (li 1 / li 0).
bool fn_803B90E4(s32 file, s32 index);
// SHARED-EDIT (tu_80414F6C BattleStageSelect_804B1378's constructor): 0x803B9120, same
// owner. Whether battle stage `stage` (0..7) is open: stages 0-2 always,
// the others by a bit of the current save file; returns 1 or 0.
bool fn_803B9120(s32 stage);
}

// Object with vtable 0x804A8578 (constructor fn_802B1EFC stores it in
// lbl_8053ACA8), owner game/tu_802B1558.cpp. The first three read bytes of
// the save region in lbl_80529DEC (see GameState_804B0FE0::SaveRegion); fn_802B1D98
// clears one and stores 0 at this+6.
class cls_804A8578 {
public:
    // fn_802B1EFC: stores this in lbl_8053ACA8 and clears the bytes below.
    // FIXME: the vtable (__vt__12cls_804A8578, .data 0x804A8578) is 88.57 % in
    // try.py. The original's 4th word is 0; the cause is not diagnosed yet.
    cls_804A8578();
    // fn_802B1EB4: the delete flag > 0 frees the object (__dl__FPv).
    virtual ~cls_804A8578();

    bool fn_802B1DCC();
    bool fn_802B1D7C();
    s32 fn_802B1D60();
    void fn_802B1D98();
    // fn_802B1DB0: tests save-region byte 0xA (tu_80134C04 calls it on
    // lbl_8053ACA8).
    bool fn_802B1DB0();
    // fn_802B1EB0: empty; tu_803FF2C4 calls it on the object it allocated.
    void fn_802B1EB0();
    // fn_802B1DE8: clears this+4/+5 and the save-region bytes 0x9/0xA, then
    // copies the 0x20-byte save region in lbl_80529DEC to 0x80700000.
    void fn_802B1DE8();
    // fn_802B1610: the JUTGamePad C3ButtonReset handler (tu_80134C04's
    // fn_80139994 forwards its port and argument to it). Not written yet.
    void fn_802B1610(s32 port, void* arg);
    // SHARED-EDIT (tu_803DAC3C fn_803DD764): fn_802B1558, the reset path
    // (r3 = lbl_8053ACA8), called before OSSetBootDol / OSResetSystem.
    // Not written yet.
    void fn_802B1558();
    // SHARED-EDIT (tu_80400DC4 fn_80401518): fn_802B1BE0, the reset itself:
    // stops the pads, GX and video, copies the save region, then
    // OSResetSystem(reset, code, menu) (menu clrlwi'd: a bool). Callers pass
    // lbl_8053ACA8 and (1, 0x80000000, 0) or (0, 0, 0). Not written yet;
    // the body never reads `this`.
    void fn_802B1BE0(s32 reset, u32 code, bool menu);

    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u8 field_0x6; // fn_802B1D98 clears it
    /* 0x7 */ u8 field_0x7; // 0xFF preset by the constructor
};
extern cls_804A8578* lbl_8053ACA8;

// game/tu_80021840.cpp (0x80021840): a stub that returns 0; tu_80134C04
// compares the result with 0, 1 and 2 (a screen mode query).
s32 fn_80021840();

// Global object with vtable 0x804B0F90 (destructor 0x80401848, emitted after
// that unit's __sinit, which registers it), owner game/tu_80400DC4.cpp
// (its .text, .data and extab end at 0x80401890 / 0x804B0FA0 / 0x80010154:
// the 0xC-byte vtable is followed by the 4 bytes of padding before
// tu_80401848's first vtable); main polls fn_80401518 each frame (it watches the
// DVD drive status) and goes on once it returns 0.
class cls_804B0F90 {
public:
    // Inline: emitted after the unit's __sinit, as a header inline is.
    virtual ~cls_804B0F90() {}
    // Draws message `index` of the screen's message file (the error texts;
    // tp d_error_msg.cpp messageSet).
    void fn_80400DC4(u32 index);
    // Draws the screen for DVD drive status `status` (tp dDvdErrorMsg_c::draw).
    void fn_80401318(s32 status);
    s32 fn_80401518();
    s32 field_0x4;
};
extern cls_804B0F90 lbl_8053ADE8;

// game/tu_80401848.cpp (0x80401FEC): creates and resumes the DVD loader
// thread with the given priority. The heaps: the commands' (main passes an
// expanded heap of 0x800 bytes) and the default one the commands load into
// when they were given none (main passes NULL).
void fn_80401FEC(s32 priority, JKRHeap* commandHeap, JKRHeap* loadHeap);

// .rodata render modes (GXRenderModeObj); main picks lbl_8048C1B8 when
// progressive scan is wanted and active, lbl_8048C17C otherwise.
extern const GXRenderModeObj lbl_8048C1B8;
extern const GXRenderModeObj lbl_8048C17C;

// The object fn_8016A3AC returns (gGameRoot_8053AAF8->field_0x4->field_0x24; named
// after that getter). Owner: game/tu_8016A2B4.cpp (not written yet). Only
// the bytes act_environment_control_2 writes are declared.
struct GameRootSub24_8016A3AC {
    GameRootSub24_8016A3AC(JKRHeap* heap); // fn_8016F0BC (not written yet)
    // Unknown pointers: 0x0, 0x30, 0x38, 0x54, 0x68.
    /* 0x000 */ void* field_0x0;
    // The J3DModels below (0x4 ... 0xEC) are passed to fn_8016A7DC.
    /* 0x004 */ struct J3DModel* field_0x4;
    /* 0x008 */ u8 field_0x8[0xC - 0x8];
    /* 0x00C */ struct J3DModel* field_0xc;
    /* 0x010 */ u8 field_0x10[0x14 - 0x10];
    /* 0x014 */ struct J3DModel* field_0x14;
    /* 0x018 */ u8 field_0x18[0x1C - 0x18];
    /* 0x01C */ struct J3DModel* field_0x1c;
    /* 0x020 */ u8 field_0x20[0x28 - 0x20];
    /* 0x028 */ struct J3DModel* field_0x28;
    /* 0x02C */ u8 field_0x2c[0x30 - 0x2C];
    /* 0x030 */ void* field_0x30;
    /* 0x034 */ struct J3DModel* field_0x34;
    /* 0x038 */ void* field_0x38;
    // Six models, placed by fn_8016B0B0 (loop over i < 6).
    /* 0x03C */ struct J3DModel* field_0x3c[6];
    // The texture-SRT and TEV-register animations fn_8016DCEC passes to fn_8016D398.
    /* 0x054 */ struct J3DAnmTextureSRTKey* field_0x54;
    /* 0x058 */ u8 field_0x58[0x68 - 0x58];
    /* 0x068 */ void* field_0x68;
    /* 0x06C */ struct J3DAnmTevRegKey* field_0x6c;
    /* 0x070 */ u8 field_0x70[0x84 - 0x70];
    /* 0x084 */ struct J3DModel* field_0x84;
    /* 0x088 */ u8 field_0x88[0x90 - 0x88];
    /* 0x090 */ struct J3DModel* field_0x90;
    /* 0x094 */ u8 field_0x94[0x9C - 0x94];
    /* 0x09C */ struct J3DModel* field_0x9c;
    /* 0x0A0 */ u8 field_0xa0[0xA8 - 0xA0];
    /* 0x0A8 */ struct J3DModel* field_0xa8;
    /* 0x0AC */ u8 field_0xac[0xB4 - 0xAC];
    /* 0x0B4 */ struct J3DModel* field_0xb4;
    /* 0x0B8 */ u8 field_0xb8[0xC0 - 0xB8];
    /* 0x0C0 */ struct J3DModel* field_0xc0;
    /* 0x0C4 */ u8 field_0xc4[0xCC - 0xC4];
    /* 0x0CC */ struct J3DModel* field_0xcc;
    /* 0x0D0 */ u8 field_0xd0[0xD4 - 0xD0];
    /* 0x0D4 */ struct J3DModel* field_0xd4;
    /* 0x0D8 */ u8 field_0xd8[0xE0 - 0xD8];
    /* 0x0E0 */ struct J3DModel* field_0xe0;
    /* 0x0E4 */ u8 field_0xe4[0xEC - 0xE4];
    /* 0x0EC */ struct J3DModel* field_0xec;
    /* 0x0F0 */ u8 field_0xf0[0xF4 - 0xF0];
    // The full-screen backdrop texture fn_8016ADE0 draws.
    /* 0x0F4 */ struct JUTTexture* field_0xf4;
    /* 0x0F8 */ u8 field_0xf8[0x110 - 0xF8];
    // Backdrop textures: fn_8016B4D4 draws 0x114 / 0x110 (or 0x11C / 0x118),
    // fn_8016B248 draws 0x120.
    /* 0x110 */ struct JUTTexture* field_0x110;
    /* 0x114 */ struct JUTTexture* field_0x114;
    /* 0x118 */ struct JUTTexture* field_0x118;
    /* 0x11C */ struct JUTTexture* field_0x11c;
    /* 0x120 */ struct JUTTexture* field_0x120;
    // The three emitters fn_8016A2B4 deletes.
    /* 0x124 */ struct JPABaseEmitter* field_0x124;
    /* 0x128 */ struct JPABaseEmitter* field_0x128;
    /* 0x12C */ struct JPABaseEmitter* field_0x12c;
    // Words cleared by fn_8016EDB0 (types unknown).
    /* 0x130 */ s32 field_0x130;
    /* 0x134 */ s32 field_0x134;
    /* 0x138 */ s32 field_0x138;
    /* 0x13C */ s32 field_0x13c;
    // Four entries; cleared by fn_8016EDB0 (fn_8016ED88 inlined) and passed
    // to fn_8016D398 by fn_8016DCEC.
    /* 0x140 */ cls_8016ED88 field_0x140;
    /* 0x1B0 */ s32 field_0x1b0;
    /* 0x1B4 */ u8 field_0x1b4;
    /* 0x1B5 */ u8 field_0x1b5[0x1B8 - 0x1B5];
    /* 0x1B8 */ s32 field_0x1b8;
    /* 0x1BC */ u8 field_0x1bc;
    /* 0x1BD */ u8 field_0x1bd[0x1C0 - 0x1BD];
    /* 0x1C0 */ s32 field_0x1c0;
    /* 0x1C4 */ u8 field_0x1c4;
    /* 0x1C5 */ u8 field_0x1c5;
    /* 0x1C6 */ u8 field_0x1c6;
    /* 0x1C7 */ u8 field_0x1c7;
    /* 0x1C8 */ u8 field_0x1c8;
    /* 0x1C9 */ u8 field_0x1c9;
    /* 0x1CA */ u8 field_0x1ca;
    /* 0x1CB */ u8 field_0x1cb[4];
    /* 0x1CF */ u8 field_0x1cf;
    /* 0x1D0 */ u8 field_0x1d0;
    /* 0x1D1 */ u8 field_0x1d1;
    /* 0x1D2 */ u8 field_0x1d2;
    /* 0x1D3 */ u8 field_0x1d3;
    /* 0x1D4 */ u8 field_0x1d4[4];
    /* 0x1D8 */ s32 field_0x1d8;
    /* 0x1DC */ u8 field_0x1dc;
    /* 0x1DD */ u8 field_0x1dd;
    /* 0x1DE */ u8 field_0x1de[0x1E4 - 0x1DE];
    /* 0x1E4 */ f32 field_0x1e4;
    /* 0x1E8 */ u8 field_0x1e8[0x1EC - 0x1E8];
    /* 0x1EC */ u8 field_0x1ec;
    /* 0x1ED */ u8 field_0x1ed;
    /* 0x1EE */ u8 field_0x1ee;
    /* 0x1EF */ u8 field_0x1ef;
    /* 0x1F0 */ u8 field_0x1f0;
    /* 0x1F1 */ u8 field_0x1f1;
    /* 0x1F2 */ u8 field_0x1f2;
    /* 0x1F3 */ u8 field_0x1f3;
    /* 0x1F4 */ u8 field_0x1f4;
    /* 0x1F5 */ u8 field_0x1f5;

    // game/tu_8016A2B4.cpp (0x8016A2B4): deletes the three emitters.
    void fn_8016A2B4();
    // 0x8016A330: clears the flag bytes, sets field_0x1d8 to -1.
    void fn_8016A330();
    // 0x8016A7DC: places model at pos plus an offset that follows the
    // 2D camera (x / y flags), scaled by `scale`, then runs the model's
    // calc / entry / viewCalc (vtable slots 0x10, 0xC, 0x1C). `this` is not read.
    void fn_8016A7DC(struct J3DModel* model, const JGeometry::TVec3f& pos, s32 dir, bool followX, bool followY, f32 parallax, f32 scale);

    // game/tu_8016A2B4.cpp: each draws one model when its flag is set (fn_8016A7DC).
    void fn_8016A9EC();
    void fn_8016AA48();
    void fn_8016AAA4();
    void fn_8016AB00();
    void fn_8016AB5C();
    void fn_8016ABB8();
    void fn_8016AC14();
    void fn_8016AC70();
    void fn_8016ACCC();
    void fn_8016AD28();
    void fn_8016AD84();
    void fn_8016B054();
    void fn_8016D284();
    void fn_8016D2E0();
    void fn_8016D33C();
    // 0x8016B8AC.
    void fn_8016B8AC();
    // 0x8016B0B0: places the six models of field_0x3c against the 2D camera.
    void fn_8016B0B0();
    // 0x8016ADE0: draws field_0xf4 over the screen (called by fn_8016DA6C / fn_8016DB60).
    void fn_8016ADE0();
    // 0x8016B248: draws field_0x120 over the screen.
    void fn_8016B248();
    // 0x8016DCEC: in the isGameModeSet() modes (not mode 0xB), while
    // field_0x1cf is set, runs fn_8016D398 with the camera matrix.
    void fn_8016DCEC();
    // 0x8016DE30 (game/tu_8016A2B4.cpp, not written yet): called by GameRootSub04's fn_80139D9C.
    void fn_8016DE30();

    // game/tu_8016A2B4.cpp (0x8016DA6C, not written yet). Takes `this` as its
    // only argument; called by fn_80139CC4 (game/tu_80139BE8.cpp).
    void fn_8016DA6C();
    // game/tu_8016A2B4.cpp (0x8016DB60, not written yet): runs a sequence of
    // fn_8016xxxx calls on this; called by fn_80139D3C.
    void fn_8016DB60();
    // game/tu_8016A2B4.cpp (0x8016EDB0, not written yet); called by fn_8013A1A4.
    void fn_8016EDB0();
    // game/tu_8016A2B4.cpp (0x8016EE84, 0x238 bytes, not written yet). Called
    // with 1 by GameRootSub04_801F856C's destructor, without a null check,
    // before it clears field_0x24.
    void fn_8016EE84(s32 arg);
};
// game/tu_8016A2B4.cpp (0x8016A3AC): returns gGameRoot_8053AAF8->field_0x4->field_0x24.
GameRootSub24_8016A3AC* fn_8016A3AC();
class J3DModel;
class J3DAnmTextureSRTKey;
class J3DAnmTevRegKey;
// Owner: game/tu_8016A2B4.cpp (0x8016A3BC, not written yet). Loads a model
// (getResource('J3D ', bmd) on `archive`, built in `heap`, `flags` its J3D
// load flags) and up to two texture-SRT and two TEV-register animations, each
// skipped when its name is NULL; the pointers receive the results (an output
// is set to NULL when its animation is not loaded). Called by
// GameRootSub20_801902C0's constructor (game/tu_801902C0.cpp).
void fn_8016A3BC(JKRArchive* archive, const char* bmd, const char* btk1, const char* btk2,
                 const char* brk1, const char* brk2, u32 flags, JKRHeap* heap, J3DModel** model,
                 J3DAnmTextureSRTKey** btk1Out, J3DAnmTextureSRTKey** btk2Out,
                 J3DAnmTevRegKey** brk1Out, J3DAnmTevRegKey** brk2Out);
// game/tu_8013AAD4.cpp: draws gGameRoot2_8053AB10's two J3D draw buffers
// (fn_8013AB2C), places and enters a model (fn_8013ABCC) and sets them and
// the J3D view matrix (fn_8013AC54).
void fn_8013AB2C();
void fn_8013ABCC(J3DModel* model, const Vec& scale, Mtx mtx);
void fn_8013AC54(Mtx view);
// Owner: game/tu_8022AD64.cpp (0x8023F728, 0x8023F78C). Called by fn_8013A1A4: each
// runs a per-player routine on all four players (fn_8023F78C also stores -1 to lbl_8053A190).
// game/tu_8022AD64.cpp (0x8023F088): shuffles the manager's four start entries.
void fn_8023F088();
void fn_8023F728();
void fn_8023F78C();
// game/tu_8022AD64.cpp (0x8023F6C4): runs each player's reset (fn_80254660);
// called by GameView_804A9950::fn_802DBD60 (game/tu_802D39B8.cpp).
void fn_8023F6C4();
// Owner: game/tu_8022AD64.cpp. The player manager's draw (0x8023F2BC, called
// by fn_80139CC4 between the two fn_8013B438 calls) and update (0x8023F390,
// called by GameRootSub04's fn_80139D9C).
void fn_8023F2BC();
void fn_8023F390();
struct cls_8016ED88; // game/cls_8016ED88.h
extern "C" {
// Owner: game/tu_8016A2B4.cpp (0x8016D398, not written yet). Steps the four
// cloud-effect entries of `entries` (respawning an effect in each idle one,
// timers in *timer1 / *timer2, a flag in *flag) relative to the view's
// translation, then sets the frames of the six btk / brk animations from
// `frame`. Called by fn_8016DCEC and Title_804AF820::fn_803A44D8
// (game/tu_803A3484.cpp).
void fn_8016D398(Mtx view, s32 frame, bool arg2, bool arg3, cls_8016ED88* entries, s32* timer1,
                 s32* timer2, u8* flag, J3DAnmTextureSRTKey** btk, J3DAnmTevRegKey** brk, f32 scale);
// Owner: game/tu_801902C0.cpp (0x801902D0, not written yet). Takes the object
// at field_0x20 of GameRootSub04_801F856C (fn_80139CC4), a GameRootSub20_801902C0.
void fn_801902D0(void* self);
// Owners not known (0x803B8C90, 0x80439AB0; not written yet). Called by
// fn_8013A1A4 (game/tu_80139BE8.cpp); arguments are its fields.
u8 fn_803B8C90(void);
}
// game/tu_8037DF68.cpp (0x8038D870); called by fn_8013A1A4 (game/tu_80139BE8.cpp).
void fn_8038D870(void);

// The 4-byte pointer at .sbss lbl_8053AD68 (the cls_804AE9A0 object, declared
// in game/tu_80379F84.h with its members fn_8037A708, fn_8037A580 and
// fn_8037A664). Used by game/tu_80134C04.cpp's fn_80139958 and the code
// around 0x80136FC4.
extern cls_804AE9A0* lbl_8053AD68;

// FireRodFire_804A89F0::fn_802BA9F0 / fn_802BA9F8: see game/act/tu_801F9484.h.

// SHARED-EDIT (tu_803F0A54, review): the singleton *lbl_8053AEC0
// (game/tu_8042E964.cpp, owner not written yet), named after its vtable
// 0x804B1634; its constructor fn_80431D84 stores `this` in lbl_8053AEC0.
// Size and most fields unknown.
class J2DScreen;
class PaneCtrl_804AF86C;
class cls_804B1600;
class cls_804B1634 {
public:
    // fn_80431D84 (game/tu_8042E964.cpp, not written yet); GameRoot's
    // constructor makes it with __nw__(0xF0).
    cls_804B1634();
    // Slot 0x8 of the vtable 0x804B1634 (fn_80431C38, game/tu_8042E964.cpp).
    virtual ~cls_804B1634();
    // 0x804316EC: draws field_0x8 with gGameRoot2_8053AB10's grafcontext unless
    // the byte 0xEC is 0. Called by cls_804B1D58::fn_804522C8 (tu_8044C7F0).
    void fn_804316EC();
    // 0x804310CC: no arguments besides `this` (lwz 8(r3), J2DPane search at
    // vtable 0x3C, stores at 0xC and 0xEB). ConnectScreen_804B0F10::fn_803FC5E4.
    void fn_804310CC();
    // 0x80431A9C: r4 an id (0xC4, 0x10B), r5 a byte (clrlwi 24 in
    // ConnectScreen_804B0F10::fn_803F2AEC), r6 an optional out byte (callers pass the
    // address of a u8 field or 0; it receives this->0xEA). Reads the byte 0xEC,
    // calls fn_8042EF9C on `this` and returns 1 or 0 (li r3 before the blr).
    bool fn_80431A9C(s32 id, u8 value, u8* out);
    // SHARED-EDIT (tu_803BC6F0 fn_803CE9EC): r4 a byte (clrlwi. 24), the
    // file select screen's yes/no cursor while its message window is open.
    bool fn_80431174(u8 cursor);
    // SHARED-EDIT (tu_803BC6F0 fn_803CE9EC): step the message window open /
    // closed; return 1 when done, else 0 (callers test with cmpwi, BOOL).
    BOOL fn_804317AC();
    BOOL fn_80431740();
    // 0x80431900: the same function as fn_80431A9C, kept twice (tu_8044C7F0's
    // fn_804522C8-area callers pass 0x10A, 0 and the address of a u8 field).
    bool fn_80431900(s32 id, u8 value, u8* out);
    // 0x8042EF9C: lays the text `id` of the message window out into `str`
    // (game/tu_8042E964.cpp, not written yet). Called by fn_80431900 / fn_80431A9C.
    void fn_8042EF9C(char* str, s32 id, u8 value);
    // 0x8042F6DC: second layout pass over the same window (not written yet).
    void fn_8042F6DC();
    // 0x80430368: appends the counted noun (the nouns of cls_804AF774::fn_803A2CB0) to out; 0.0f.
    f32 fn_80430368(u8 noun, s32 count, char* out);
    // 0x804310BC: clears field_0xe8 and field_0xec (owner game/tu_8042E964.cpp).
    void fn_804310BC();

    /* 0x04 */ cls_804B1600* field_0x4;  // new in fn_80431D84, a 0x14-byte object
    /* 0x08 */ J2DScreen* field_0x8;     // "warning_window.blo", drawn by fn_804316EC
    /* 0x0C */ PaneCtrl_804AF86C* field_0xc;  // pane wrapper
    /* 0x10 */ PaneCtrl_804AF86C* field_0x10[5];  // panes "bb01".."bb05" (fn_80431D84)
    /* 0x24 */ PaneCtrl_804AF86C* field_0x24[5];  // panes "wb01".."wb05"
    // Frame sizes copied from the panes' 0x112 (fn_80431D84): per bb pane (field_0x10) and wb pane (field_0x24) four pairs.
    /* 0x38 */ JGeometry::TVec2<s16> field_0x38[16];
    /* 0x78 */ JGeometry::TVec2<s16> field_0x78[16];
    /* 0xB8 */ u8 field_0xb8[0xBC - 0xB8];
    // SHARED-EDIT: floats read by fn_80431174 (cursor placement): the window's
    // pane sizes and offsets, laid out by fn_8042F6DC.
    /* 0xBC */ f32 field_0xbc;
    /* 0xC0 */ f32 field_0xc0;
    /* 0xC4 */ u8 field_0xc4[0xC8 - 0xC4];
    /* 0xC8 */ f32 field_0xc8;
    /* 0xCC */ f32 field_0xcc;
    /* 0xD0 */ f32 field_0xd0;
    /* 0xD4 */ u8 field_0xd4[0xD8 - 0xD4];
    /* 0xD8 */ f32 field_0xd8[2];       // indexed by the cursor (0 or 1; fn_8042F6DC's loop stores i < 2)
    /* 0xE0 */ u8 field_0xe0[0xE4 - 0xE0];
    /* 0xE4 */ s32 field_0xe4;
    /* 0xE8 */ s16 field_0xe8;           // frame counter of the window's open/close steps
    /* 0xEA */ u8 field_0xea;            // out byte of fn_80431A9C
    /* 0xEB */ u8 field_0xeb;
    /* 0xEC */ u8 field_0xec;            // window shown / animating
};
extern cls_804B1634* lbl_8053AEC0;

// game/tu_801A0710.cpp (.sdata 0x8053A100, not written yet; -1 in the
// original): act_terrorpin's vfunc_0x44 compares its field_0x2a8 with it.
extern s32 lbl_8053A100;

// SHARED-EDIT (act_battle_royale_end_check fn_803D6BDC): the only caller
// known so far. Owner: game/tu_8022AD64.cpp (0x80239F10). (fn_802B3A48 is
// cls_804A8638::fn_802B3A48, game/cls_804A8638.h.)
bool fn_80239F10();

// game/tu_803006E8.cpp (0x803007A0, not written yet). Takes no arguments; calls
// vtable slot 0xC of *lbl_8053ACB8. GameRoot's destructor calls it.
void fn_803007A0();
// game/tu_80301CD0.cpp (0x8030361C, 0x134 bytes, not written yet). Takes no
// arguments; called by game/tu_80139BE8.cpp (fn_80139BE8).
void fn_8030361C();
// game/tu_8037DF68.cpp (0x8038D454). Takes no arguments; called by
// game/tu_80139BE8.cpp (fn_80139BE8) and tu_802D39B8.
void fn_8038D454();
// .sbss byte at 0x8053ACD0, owned by game/tu_80301CD0.cpp (its split starts
// there). Read with lbz by that unit; stored as 0 by game/tu_80139BE8.cpp.
extern u8 lbl_8053ACD0;
// Per-player byte flags (.sbss 0x8053ACD8); game/tu_8037DF68.cpp's fn_8038D570 sets the first
// fn_80138A30() of them to 1 (at most 4 players).
extern u8 lbl_8053ACD8[4];
// .sbss byte at 0x8053AAA0 (8 bytes), owned by game/tu_8011CEC8.cpp (its split starts
// there). Cleared by tu_801213EC fn_80121ABC, read with lbz (!= 0) by act_escort_woman,
// set to 1 by tu_801B6C50 and tu_803D9944.
extern u8 lbl_8053AAA0;
// .sbss byte at 0x8053AB60 (8 bytes). Read with lbz by game/act_chief_soldier.cpp
// (draw), which skips the draw when it is 1; no writer found yet.
extern u8 lbl_8053AB60;

// .sbss word at 0x8053AB9C, owned by game/tu_80197070.cpp (not written yet).
// Set to 4 by act_recovery_heart (update) when ZLD2 exists; act_fairy writes it too.
extern s32 lbl_8053AB9C;
// .sbss word at 0x8053ABEC, owned by game/act_dodongo.cpp (data not defined yet).
// Set to 4 by act_recovery_heart (update) when ZLD4 exists; act_fairy writes it too.
extern s32 lbl_8053ABEC;

// game/tu_80301CD0.cpp (0x8030381C, not written yet). Takes a layer; returns a
// byte (0 when fn_8030A308 on lbl_8053AD30 fails, else a per-layer table entry).
// Called by game/tu_8023FBF0.cpp (fn_8024F54C).
u8 fn_8030381C(s32 layer);
// game/tu_80301CD0.cpp (0x803037D4, not written yet). Raises a per-layer
// maximum to the value (unsigned compare). Called by game/tu_8023FBF0.cpp
// (fn_8024F54C).
void fn_803037D4(s32 layer, u32 value);
// game/tu_80301CD0.cpp (0x80308D0C, not written yet). Takes no arguments;
// called by game/tu_802D39B8.cpp (GameView_804A9950::fn_802DCDF4).
void fn_80308D0C();
// game/tu_80301CD0.cpp (0x80303870, not written yet). Takes a player index;
// called by game/tu_802D39B8.cpp (GameView_804A9950::vfunc_0xC) for each
// taking-part player with a usable controller.
extern "C" void fn_80303870(s32 player);
// game/tu_803B8C74.cpp (0x803B8EBC, not written yet). Takes an index 0-7 and
// returns 0 or 1 (callers clrlwi. 24); called by vfunc_0xC (game/tu_802D39B8.cpp)
// and game/tu_80403D84.cpp.
extern "C" bool fn_803B8EBC(s32 index);

// Object with vtable 0x804AF860, owner game/tu_803A60E4.cpp (not written
// yet). Its constructor (fn_803A7898, 0x84 bytes allocated) loads a
// J2DScreen from lbl_80529DEC's archives. The game mode screen (tu_803DAC3C)
// allocates one and deletes it through the destructor's vtable slot.
class cls_804AF860 {
public:
    cls_804AF860(); // fn_803A7898
    virtual ~cls_804AF860();
    // 0x803A71F0 (not written yet): starts by setting the display's clear colour;
    // GameRoot's fn_80136828 calls it on lbl_8053AD90 in screen mode 5.
    void fn_803A71F0();
    // 0x803A7708 is the destructor.

    /* 0x00 */ // vtable
    /* 0x04 */ J2DScreen* field_0x4;
    /* 0x08 */ J2DScreen* field_0x8;
    /* 0x0C */ PaneCtrl_804AF86C* field_0xc[6];
    /* 0x24 */ PaneCtrl_804AF86C* field_0x24;
    /* 0x28 */ PaneCtrl_804AF86C* field_0x28;
    /* 0x2C */ PaneCtrl_804AF86C* field_0x2c;
    /* 0x30 */ PaneCtrl_804AF86C* field_0x30;
    /* 0x34 */ u8 field_0x34[0x80 - 0x34];
    /* 0x80 */ s32 field_0x80; // returned by fn_803A65EC (5 without the screen)
    /* 0x84 */ u8 field_0x84[0];
}; // Size: 0x84 (__nw__(0x84) in tu_803DAC3C's constructor)
// .sbss pointer the constructor sets (0x803A8350).
extern cls_804AF860* lbl_8053AD90;

// game/act_searchlight.cpp (owner unit, .bss:0x804C0EA0, size 0x108).
// Position of the searchlight / last broken pot.
extern f32 lbl_804C0EA0[];

// game/tu_801618D0.cpp (owner unit, .sbss:0x8053AB68, size 0x8).
// Layer of the searchlight / last broken pot.
extern s32 lbl_8053AB68;

// .data:0x8049C3B8, size 0x28, s16 values (no unit splits it yet; read by
// act_temple_bti_display and tu_800F7FCC).
extern s16 lbl_8049C3B8[];

// game/act_dodongo.cpp's .sbss (0x8053AC00, size 0x8): a flag act_dodongo does not touch
// itself; tu_801B6C50 sets it in init() and tests it in update().
extern u8 lbl_8053AC00;

#endif
