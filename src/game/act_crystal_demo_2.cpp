// Crystal (cutscene) actor: 'DCR2', spawned by tu_801F9484.cpp.
// Declared in include/game/act/tu_801F9484.h (cls_804AE088).
// Inherits from cls_8049D140 (tu_8010DF44).

#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801F9484.h"
#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"
#include "game/Unk_8045A080.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JMath/JMath.h"

// Texture name string (.rodata): "kk_black.bti"
static const char lbl_80474C38[] = "kk_black.bti";

// 8 state indices (.rodata 0x20): maps draw state to frame index.
static const s32 lbl_80474C48[] = {0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3, 0x21B, 0x21B};

// Large config/data table (.rodata 0x1E0 bytes): effect IDs, colors, state data.
// FIXME(shotgun): not decomposed — 0x1E0 byte table, too large for shotgun pass.
// FIXME(shotgun): two packed filenames (.rodata 0x28): "enemy_gc_sob.bin"/"enemy_both2_sob.bin".
// FIXME(shotgun): PTMF dispatch table (.data, lbl_804AE070): state-dispatch table.
// All used only in FIXME'd function bodies; references omitted from compiled stubs.

// Sprite animation frame tables (.sdata2)
static const s16 lbl_80542C48[] = {9, -1};
static const s16 lbl_80542C50[] = {9, -1};

// SpriteAnim tables (.sdata): pointer arrays
static const s16* lbl_8053A1C0[] = {lbl_80542C48, lbl_80542C50};
// lbl_8053A1C8: {(s16*)lbl_80474C38, NULL}: filename used as anim data reference.
static const s16* lbl_8053A1C8[] = {(const s16*)lbl_80474C38, NULL};

// Functions are listed in descending address order (highest first, lowest last).

// FIXME(shotgun): 100 %, class
s32 cls_804AE088::vfunc_0x58() {
    return field_0x240;
}

// FIXME(shotgun): 90 %, class
cls_804AE088::cls_804AE088() {
    // Calls cls_8049D140 ctor, installs vtable, zero-inits fields,
    // calls __construct_array for field_0x2a8 (6 TVec3f), ctors SpriteAnim,
    // zero-inits emitter arrays, then fn_80458F9C(0x3c).
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 65 %, class
cls_804AE088::~cls_804AE088() {
    // Destroys field_0x314 (J2DPicture), calls fn_8036A65C, tears down SpriteAnim,
    // calls ~cls_8049D140. FIXME(shotgun): not written
}

// FIXME(shotgun): 55 %, class
void cls_804AE088::init() {
    // Calls cls_8049D140::init(), sets field_0x23c from param bits [0:4], field_0x248
    // from bits [5:9]. Sets up SpriteAnim, spawns emitters. FIXME(shotgun): not written
}

// FIXME(shotgun): 65 %, class
void cls_804AE088::update() {
    // Counts down field_0x2f0; plays sounds at 0xe1 and 0xb4; dispatches ptmf state.
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 65 %, class
void cls_804AE088::draw() {
    // Saves pos, checks field_0x23c==5 and field_0x240 range to decide draw path.
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 55 %, class
void cls_804AE088::vfunc_0x54() {
    // Reads fn_8011921C()->field_0x8, dispatches to vfunc_0x60 based on frame.
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 30 %, class
void cls_804AE088::vfunc_0x60(s32 arg) {
    // Large state machine via jumptable_804AE184 (39 entries).
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 90 %, class
s32 cls_804AE088::vfunc_0x5C() {
    s32 s = field_0x240;
    if (s == 0x11 || s == 0x26)
        return 1;
    return 0;
}

// FIXME(shotgun): 45 %, class
void cls_804AE088::fn_8036AC58(s32 arg) {
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 55 %, class
void cls_804AE088::fn_8036AA60() {
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 60 %, class
void cls_804AE088::fn_8036A894() {
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 55 %, class
void cls_804AE088::fn_8036A65C() {
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 48 %, class
void cls_804AE088::fn_80369DDC() {
    // Large state machine via jumptable_804AE13C (18 entries).
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 41 %, class
void cls_804AE088::fn_80368CD8() {
    // Large state machine via jumptable_804AE0EC (20 entries).
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 55 %, class
void cls_804AE088::fn_80368660() {
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 73 %, class
void cls_804AE088::fn_803683AC() {
    // FIXME(shotgun): not written
}

// FIXME(shotgun): 85 %, class
void cls_804AE088::fn_80368198() {
    // FIXME(shotgun): not written
}
