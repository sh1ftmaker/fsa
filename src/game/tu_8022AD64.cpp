// The player manager (include/game/Player.h) and the per-player queries
// other units call. Written so far: the manager's flag accessors and one
// query of each shape (a getter call, a setter call with an argument, a
// field, a field's address), the templates for the rest.

#include "game/BgRewrite.h"
#include "game/cls_804AE9B0.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/PlayerBit.h"

// What the position queries return when there is no such player. The
// unit's __sinit (0x8023F9B8) stores 0.0f to its three floats before the
// JAudio pools; dtk's lbl_804CF460 (0x108) also covers the pools'
// destructor chain after it.
JGeometry::TVec3f lbl_804CF460(0.0f, 0.0f, 0.0f);

void fn_8022AD64() {
    gGameRoot_8053AAF8->fn_80138B7C();
}

void fn_8022AD88() {
    gBgRewrite_8053AC98->fn_8029D898(0x19);
}

// fn_8022ADFC inlines the flag getters below, which come after it.

static inline bool getFlag_0x6c() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    return gPlayerManager_8053AC90->field_0x6c;
}

static inline bool getFlag_0x6d() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    return gPlayerManager_8053AC90->field_0x6d;
}

static inline bool getFlag_0x6e() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    return gPlayerManager_8053AC90->field_0x6e;
}

static inline bool getFlag_0x6f() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    return gPlayerManager_8053AC90->field_0x6f;
}

static inline bool getFlag_0x70() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    return gPlayerManager_8053AC90->field_0x70;
}

static inline bool getFlag_0x71() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    return gPlayerManager_8053AC90->field_0x71;
}

static inline bool getFlag_0x72() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    return gPlayerManager_8053AC90->field_0x72;
}

bool fn_8022ADB0() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    return gPlayerManager_8053AC90->field_0x73;
}

void fn_8022ADCC() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x73 = false;
    }
}

void fn_8022ADE4() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x73 = true;
    }
}

// Whether any of the flags 0x6C-0x72 is set.
bool fn_8022ADFC() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (getFlag_0x6c()) {
        return true;
    }
    if (getFlag_0x6d()) {
        return true;
    }
    if (getFlag_0x6e()) {
        return true;
    }
    if (getFlag_0x6f()) {
        return true;
    }
    if (getFlag_0x70()) {
        return true;
    }
    if (getFlag_0x71()) {
        return true;
    }
    if (getFlag_0x72()) {
        return true;
    }
    return false;
}

bool fn_8022AF10() {
    return getFlag_0x72();
}

void fn_8022AF2C() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x72 = false;
    }
}

void fn_8022AF44() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x72 = true;
    }
}

bool fn_8022AF5C() {
    return getFlag_0x71();
}

// Clears the flag 0x71 (fn_8022B064 sets it), then on every present player fn_8024D98C,
// fn_8024BAEC, fn_80248E94(false) and fn_8026FC8C.
void fn_8022AF78() {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    gPlayerManager_8053AC90->field_0x71 = false;
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_8024D98C();
            gPlayerManager_8053AC90->mPlayers[i]->fn_8024BAEC();
            gPlayerManager_8053AC90->mPlayers[i]->fn_80248E94(false);
            gPlayerManager_8053AC90->mPlayers[i]->fn_8026FC8C();
        }
    }
}

// Sets the flag 0x71, then fn_8024D98C and fn_80248DD8 on every present player.
void fn_8022B064() {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    gPlayerManager_8053AC90->field_0x71 = true;
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_8024D98C();
            gPlayerManager_8053AC90->mPlayers[i]->fn_80248DD8();
        }
    }
}

void fn_8022B130() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x70 = false;
    }
}

void fn_8022B148() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x70 = true;
    }
}

bool fn_8022B160() {
    return getFlag_0x6f();
}

void fn_8022B17C() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x6f = false;
    }
}

void fn_8022B194() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x6f = true;
    }
}

bool fn_8022B1AC() {
    return getFlag_0x6e();
}

void fn_8022B1C8() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x6e = false;
    }
}

void fn_8022B1E0() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x6e = true;
    }
}

bool fn_8022B1F8() {
    return getFlag_0x6d();
}

void fn_8022B214() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x6d = false;
    }
}

void fn_8022B22C() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x6d = true;
    }
}

bool fn_8022B244() {
    return getFlag_0x6c();
}

void fn_8022B260() {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->field_0x6c = false;
    }
}

// Sets the flag 0x6C and calls fn_8026FC8C on every present player.
void fn_8022B278() {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    gPlayerManager_8053AC90->field_0x6c = true;
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_8026FC8C();
        }
    }
}

bool fn_8022B7C4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8023FC08();
    }
    return false;
}

void fn_8022B860(s32 player, s32 value) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8023FC10(value);
    }
}

void fn_8022B8EC(s32 player, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80241B44(out);
    }
}

bool fn_8022B978(s32 layer, const JGeometry::TBox2f* box, bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80241BA0(layer, box, flag)) {
            return true;
        }
    }
    return false;
}

bool fn_8022BA58(s32 layer, const JGeometry::TVec3f* pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80241DD0(layer, pos)) {
            return true;
        }
    }
    return false;
}

bool fn_8022BB48(s32 layer, const JGeometry::TVec3f* pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80241E28(layer, pos)) {
            return true;
        }
    }
    return false;
}

bool fn_8022BC38(s32 player, s32 a, const JGeometry::TVec3f* b) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241E28(a, b);
    }
    return false;
}

cls_804AA8D8* fn_8022BCD4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return NULL;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8023FC24();
    }
    return NULL;
}

void fn_8022BD70() {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_80241E64();
        }
    }
}

// The position is passed on as a copy.
void fn_8022BE24(s32 player, const JGeometry::TVec3f* pos) {
    if (gPlayerManager_8053AC90 != NULL && isPlayerActive(player)) {
        JGeometry::TVec3f copy(*pos);
        gPlayerManager_8053AC90->mPlayers[player]->fn_8023FF90(&copy);
    }
}

s32 fn_8022BECC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8023FFAC();
    }
    return 0;
}

bool fn_8022BF68(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802454C0();
    }
    return false;
}

bool fn_8022C004(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802456FC();
    }
    return false;
}

bool fn_8022C0A0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80245938();
    }
    return false;
}

bool fn_8022C13C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80245B74();
    }
    return false;
}

bool fn_8022C1D8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80245DB0();
    }
    return false;
}

bool fn_8022C274(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80245FEC();
    }
    return false;
}

bool fn_8022C310(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80246228();
    }
    return false;
}

bool fn_8022C3AC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (player < 0) {
        return false;
    }
    if (player >= 4) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802466B4();
    }
    return false;
}

s32 fn_8022C46C(s32 player, u32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (player < 0) {
        return 0;
    }
    if (player >= 4) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80246904(arg);
    }
    return 0;
}

u32 fn_8022C52C(s32 player, u32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (player < 0) {
        return 0;
    }
    if (player >= 4) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80246B10(arg);
    }
    return 0;
}

u32 fn_8022C5EC(s32 player, u32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (player < 0) {
        return 0;
    }
    if (player >= 4) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80246D1C(arg);
    }
    return 0;
}

f32 fn_8022C6AC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0.0f;
    }
    if (player < 0) {
        return 0.0f;
    }
    if (player >= 4) {
        return 0.0f;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80246F28();
    }
    return 0.0f;
}

f32 fn_8022C76C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0.0f;
    }
    if (player < 0) {
        return 0.0f;
    }
    if (player >= 4) {
        return 0.0f;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024712C();
    }
    return 0.0f;
}

f32 fn_8022C82C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0.0f;
    }
    if (player < 0) {
        return 0.0f;
    }
    if (player >= 4) {
        return 0.0f;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80247330();
    }
    return 0.0f;
}

u8 fn_8022C8EC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (player < 0) {
        return false;
    }
    if (player >= 4) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80247534();
    }
    return false;
}

bool fn_8022C9AC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (player < 0) {
        return false;
    }
    if (player >= 4) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80247564();
    }
    return false;
}

void fn_8022CA6C(s32 player, s32 a, bool b) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80248004(a, b);
    }
}

s32 fn_8022CAF8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802486A0();
    }
    return 0;
}

void fn_8022CB94(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024B9E8();
    }
}

void fn_8022CC20(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024BAEC();
    }
}

bool fn_8022CCAC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80249068();
    }
    return false;
}

void fn_8022CD48(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80248FF0();
    }
}

// fn_80249070 on a present `player` and every present player of its group (fn_8022E438).
void fn_8022CDD4(s32 player) {
    if (gPlayerManager_8053AC90 != NULL && isPlayerActive(player)) {
        for (s32 i = 0; i < 4; i++) {
            if (isPlayerActive(i) && (player == i || fn_8022E438(player, i))) {
                gPlayerManager_8053AC90->mPlayers[i]->fn_80249070();
            }
        }
    }
}

// fn_8024909C on `player` and every present player of its group (fn_8022E438).
void fn_8022CF08(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && (player == i || fn_8022E438(player, i))) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_8024909C();
        }
    }
}

// fn_8024BB88 on every present player of a present `player`'s group (fn_8022E438),
// `player` itself only as fn_8022E438 includes it.
void fn_8022D36C(s32 player) {
    if (gPlayerManager_8053AC90 != NULL && isPlayerActive(player)) {
        for (s32 i = 0; i < 4; i++) {
            if (isPlayerActive(i) && fn_8022E438(player, i)) {
                gPlayerManager_8053AC90->mPlayers[i]->fn_8024BB88();
            }
        }
    }
}

s32 fn_8022D498(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024BB94();
    }
    return 0;
}

bool fn_8022D75C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_8024BBB0();
}

s32 fn_8022D7F8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (!isPlayerActive(player)) {
        return 0;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_8024FF6C();
}

u8 fn_8022D894(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802517E4();
    }
    return false;
}

bool fn_8022D930(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241478();
    }
    return false;
}

bool fn_8022D9CC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80250090();
    }
    return false;
}

bool fn_8022DA68(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80250ACC();
    }
    return false;
}

bool fn_8022DB04(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80250D30();
    }
    return false;
}

bool fn_8022DBA0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024F630();
    }
    return false;
}

bool fn_8022DC3C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8028FC2C();
    }
    return false;
}

bool fn_8022DCD8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240818();
    }
    return false;
}

bool fn_8022DD74(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024AFF8();
    }
    return false;
}

bool fn_8022DE10(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024B108(arg);
    }
    return false;
}

bool fn_8022DEAC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024B24C();
    }
    return false;
}

void fn_8022DF48(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024B274();
    }
}

// The fallbacks return the address of a zeroed local (the original does: stfs 0.0f twice to
// the frame, addi r3, r1).
const JGeometry::TVec2f* fn_8022DFD4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        JGeometry::TVec2f zero(0.0f, 0.0f);
        return &zero;
    }
    if (!isPlayerActive(player)) {
        JGeometry::TVec2f zero(0.0f, 0.0f);
        return &zero;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_802422C4();
}

void fn_8022E088(s32 player, s32 index, JGeometry::TVec3f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024B298(index, out);
    }
}

void fn_8022E114(s32 player, JGeometry::TVec3f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024B2BC(out);
    }
}

bool fn_8022E1A0(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025AA24(arg);
    }
    return false;
}

bool fn_8022E23C(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025AA8C(arg);
    }
    return false;
}

bool fn_8022E39C(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240840(arg);
    }
    return false;
}

s32 fn_8022E8D4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80248778();
    }
    return 0;
}

s32 fn_8022E970(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80248970();
    }
    return 0;
}

s32 fn_8022F340(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240628();
    }
    return -1;
}

s32 fn_8022F3DC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (!isPlayerActive(player)) {
        return -1;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80240644();
}

bool fn_8022F478(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return true;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80240670();
}

bool fn_8022F514(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80240808();
}

bool fn_8022F5B0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024C35C();
    }
    return false;
}

bool fn_8022F64C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240DCC();
    }
    return false;
}

bool fn_8022F6E8(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025EF68(arg);
    }
    return false;
}

bool fn_8022F784(s32 player, s32 a, const JGeometry::TVec3f* pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025EDD4(a, pos);
    }
    return false;
}

void fn_8022F820(s32 player, s32 a, const JGeometry::TVec3f* b) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8025EE0C(a, b);
    }
}

bool fn_8022F8AC(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025EE34(arg);
    }
    return false;
}

bool fn_8022F948(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025EE48(arg);
    }
    return false;
}

bool fn_8022F9E4(s32 player, s32 a, bool b, bool c) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025EFFC(a, b, c);
    }
    return false;
}

bool fn_8022FA80(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024C458();
    }
    return false;
}

bool fn_8022FB1C(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024C460(arg);
    }
    return false;
}

bool fn_8022FBB8(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024C4C4(arg);
    }
    return false;
}

bool fn_8022FC54(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024C5A0(arg);
    }
    return false;
}

bool fn_8022FCF0(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024C828(arg);
    }
    return false;
}

s32 fn_8022FD8C(s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && !fn_8037F1E4(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_8024CA0C(serial)) {
            return i;
        }
    }
    return -1;
}

// Like fn_8022FF8C: no isPlayerActive test.
void fn_8022FE80(s32 player, s32 layer, s32 serial, bool a, bool b) {
    if (gPlayerManager_8053AC90 != NULL) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024CAC8(layer, serial, a, b);
    }
}

void fn_8022FEB8(const JGeometry::TBox2f* box, s32 layer, s32 serial, bool a, bool b) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_8024CD20(box, layer, serial, a, b);
        }
    }
}

void fn_8022FF8C(s32 player, const JGeometry::TBox2f* box, s32 layer, s32 serial, bool a, bool b) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    gPlayerManager_8053AC90->mPlayers[player]->fn_8024CD20(box, layer, serial, a, b);
}

void fn_8022FFC4(s32 player, const JGeometry::TVec3f* arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024E020(arg);
    }
}

bool fn_80230050(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024E03C();
    }
    return false;
}

bool fn_802300EC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024E060();
    }
    return false;
}

bool fn_80230188(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_802413E8();
}

bool fn_80230224(s32 player, s32 flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024D058(flag);
    }
    return false;
}

bool fn_802302C4(s32 player, s32 a, bool b) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024DA64(a, b);
    }
    return false;
}

bool fn_80230360(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024D98C();
    }
    return false;
}

bool fn_802303FC(s32 player, const JGeometry::TVec3f* pos, bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024D99C(pos, flag);
    }
    return false;
}

bool fn_80230498(s32 player, const JGeometry::TVec3f* pos, s32 id, u8 a, s8 b, s8 c, bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024DC00(pos, id, a, b, c, flag);
    }
    return false;
}

// fn_8024D058(flag) on every present player; false as soon as one refuses, else
// lbl_8053AD70's fn_8037AE30 and true.
bool fn_80230534(bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && !gPlayerManager_8053AC90->mPlayers[i]->fn_8024D058(flag)) {
            return false;
        }
    }
    lbl_8053AD70->fn_8037AE30();
    return true;
}

// Whether fn_8024D058(flag) accepts for every present player of a present `player`'s
// group (fn_8022E438); false without the player.
bool fn_8023061C(s32 player, bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && fn_8022E438(player, i) &&
            !gPlayerManager_8053AC90->mPlayers[i]->fn_8024D058(flag)) {
            return false;
        }
    }
    return true;
}

// Whether every present player passes fn_8024E060 and then fn_8024DCD0 (each pass
// stops at the first that fails); when all do, fn_8037AE00 on lbl_8053AD70. False
// without a manager.
bool fn_8023077C() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && !gPlayerManager_8053AC90->mPlayers[i]->fn_8024E060()) {
            return false;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && !gPlayerManager_8053AC90->mPlayers[i]->fn_8024DCD0()) {
            return false;
        }
    }
    lbl_8053AD70->fn_8037AE00();
    return true;
}

// Whether every present player of a present `player`'s group (fn_8022E438) passes
// fn_8024E060, and then every one passes fn_8024DCD0 (each pass stops at the first
// that fails).
bool fn_802308E0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && fn_8022E438(player, i) &&
            !gPlayerManager_8053AC90->mPlayers[i]->fn_8024E060()) {
            return false;
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && fn_8022E438(player, i) &&
            !gPlayerManager_8053AC90->mPlayers[i]->fn_8024DCD0()) {
            return false;
        }
    }
    return true;
}

bool fn_80230AD4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024DCD0();
    }
    return 0;
}

s32 fn_80230B70(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 2;
    }
    // The bool test is materialised before the branch (`== false` shape, RULES_CARD bool materialisation).
    if (isPlayerActive(player) == false) {
        return 2;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->field_0x3e8;
}

void fn_80230BF4(s32 player, s32 mode) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        Player_804A6F38* p = gPlayerManager_8053AC90->mPlayers[player];
        p->field_0x3e8 = mode;
        p->field_0x3dc = mode;
    }
}

void fn_80230C6C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8025F698(false);
    }
}

s32 fn_80230CFC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 2;
    }
    // The bool test is materialised before the branch (`== false` shape, RULES_CARD bool materialisation).
    if (isPlayerActive(player) == false) {
        return 2;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->field_0x3dc;
}

bool fn_80230D80(s32 player, const JGeometry::TVec3f* pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025FDD0(pos);
    }
    return false;
}

bool fn_80230E1C(s32 player, bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024EFA0(flag);
    }
    return false;
}

void fn_80230EB8(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024EFF8(arg);
    }
}

void fn_80230F44(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024F54C(arg);
    }
}

void fn_80230FD0(s32 player, s32 value) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024F0E8(value);
    }
}

bool fn_8023105C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025FEDC();
    }
    return false;
}

bool fn_802310F8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025FF70();
    }
    return false;
}

bool fn_80231194(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8026000C();
    }
    return false;
}

bool fn_80231230(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80260048(arg);
    }
    return false;
}

bool fn_802312CC(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802600A0(arg);
    }
    return false;
}

void fn_80231368() {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_802422D0();
        }
    }
    gAudioMgr_8053AF80->fn_80457F6C();
}

void fn_80231424() {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_80242560();
        }
    }
}

bool fn_802314D8(s32 player, JGeometry::TVec3f pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80242624(pos);
}

bool fn_80231590(s32 player, s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802426E0(serial);
    }
    return false;
}

bool fn_8023162C(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80242704(arg);
    }
    return false;
}

bool fn_802317F0(s32 player, s32 a, const JGeometry::TVec3f* b, const JGeometry::TVec3f* c, s32 d) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80290070(a, b, c, d);
    }
    return false;
}

bool fn_8023188C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802413F0();
    }
    return false;
}

void fn_80231928(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8023FBF0();
    }
}

bool fn_802319B4(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80251BFC(arg);
    }
    return false;
}

// Out-of-line copy of TBox2<f32>::set(const TBox2&) (JGeometry.h). Its two
// callers in fn_80232A0C copy a box into a stack TBox2f before passing it on.
void fn_80233BA0(JGeometry::TBox2f* dst, const JGeometry::TBox2f* src) {
    dst->set(*src);
}

// Out-of-line copy of TVec3<f32>::operator-: a copy of a, then sub(b).
// Returned through the hidden pointer like any struct value.
JGeometry::TVec3f fn_80233BC4(const JGeometry::TVec3f& a, const JGeometry::TVec3f& b) {
    JGeometry::TVec3f diff = a;
    diff.sub(b);
    return diff;
}

// Out-of-line copy of TVec3<f32>::normalize() (libs/sdk JGeometry/Vec.h: PSVECSquareMag,
// TUtilf::epsilon, inv_sqrt, PSVECScale); fn_80232A0C calls it five times.
// FIXME: 99.73 %. reloc-name only: its 0.5f / 3.0f follow 15.0f (lbl_80540010) in the pool,
// which only the unwritten fn_80232A0C uses first.
f32 fn_80233C08(JGeometry::TVec3f* v) {
    return v->normalize();
}

// Whether the boxes overlap: a copy of a clipped to b is not empty. The final test is written out:
// through TBox::isValid (TVec2::isAbove's `? true : false`) the result is materialised in r0 and
// renormalised (4 more instructions, 83.94 %); the original returns straight from the compares.
bool fn_80233C9C(const JGeometry::TBox2f* a, const JGeometry::TBox2f* b) {
    JGeometry::TBox2f box = *a;
    box.i.setMax(b->i);
    box.f.setMin(b->f);
    return box.f.x >= box.i.x && box.f.y >= box.i.y;
}

bool fn_80233D20(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->field_0x43f;
    }
    return false;
}

bool fn_80233DA4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241230();
    }
    return false;
}

bool fn_80233E40(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802412A4();
    }
    return false;
}

bool fn_80233EDC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240B84();
    }
    return false;
}

void fn_80233F78(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80240878();
    }
}

s32 fn_80234004(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240888();
    }
    return 0;
}

// No final return (see progress/game__tu_8022AD64.md, "Missing final return"): an inactive
// player falls through with r3 still the index, which the caller uses.
s32 fn_802340A0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240868();
    }
}

bool fn_80234134(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240870();
    }
    return false;
}

// Whether a present, taking-part (mPlayerMask) `player` has every other present player
// of its group (fn_8022E438) passing fn_802408C0.
bool fn_802341D0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    if (!isPlayerBitSet(gGameRoot_8053AAF8, player)) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && player != i && fn_8022E438(player, i) &&
            !gPlayerManager_8053AC90->mPlayers[i]->fn_802408C0()) {
            return false;
        }
    }
    return true;
}

// FIXME: 99.95 %. reloc-name only: MWCC inlines the recursion three levels deep, and
// the deepest level calls the player test out of line, fn_8023F264 in the original
// and isPlayerActive's weak copy here (so the original's isPlayerActive is
// fn_8023F264 itself, defined later in this file; see docs/CRACK_PASS.md, inlining
// of later-defined functions). The weak Player::fn_8023F984 copy comes from here.
// fn_80241964 on the player, or on its leader (fn_8022F3DC) when it follows one
// (fn_8022F514), recursively; false without the player.
bool fn_8023434C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    if (fn_8022F514(player)) {
        return fn_8023434C(fn_8022F3DC(player));
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80241964();
}

void fn_80234810() {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_80248DAC();
        }
    }
}

void fn_802348C4(s32 player, s32 a, const JGeometry::TVec3f* b) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_802408C8(a, b);
    }
}

bool fn_80234950(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240AD4();
    }
    return false;
}

bool fn_802349EC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240AF0();
    }
    return false;
}

bool fn_80234A88(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240AF8();
    }
    return false;
}

bool fn_80234B24(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240B00();
    }
    return false;
}

bool fn_80234BC0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240B20();
    }
    return false;
}

bool fn_80234C5C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240B5C();
    }
    return false;
}

bool fn_80234CF8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240B70();
    }
    return false;
}

bool fn_80234D94(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240D50();
    }
    return false;
}

bool fn_80234E30(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240D64();
    }
    return false;
}

bool fn_80234ECC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240DAC();
    }
    return false;
}

bool fn_80234F68(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240ED4();
    }
    return false;
}

bool fn_80235004(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240EF4();
    }
    return false;
}

bool fn_802350A0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240F14();
    }
    return false;
}

bool fn_8023513C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240F28();
    }
    return false;
}

bool fn_802351D8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024119C();
    }
    return false;
}

bool fn_80235274(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802411C4();
    }
    return false;
}

bool fn_80235310(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241258();
    }
    return false;
}

bool fn_802353AC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241284();
    }
    return false;
}

bool fn_80235448(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802412CC();
    }
    return false;
}

bool fn_802354E4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802411EC();
    }
    return false;
}

bool fn_80235580(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241200();
    }
    return false;
}

bool fn_8023561C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241380();
    }
    return false;
}

void fn_802356B8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_802412D4();
    }
}

bool fn_80235744(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802413C8();
    }
    return false;
}

s32 fn_802357E0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802702E4();
    }
    return 0;
}

s32 fn_8023587C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80270310();
    }
    return 0;
}

void fn_80235918(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_802702EC();
    }
}

// The callee (fn_80270318) never reads its argument, yet the target forwarder passes it (r4 live: manager in r5); 98.7 % regalloc-only without it.
void fn_802359A4(s32 player, s32 value) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80270318(value);
    }
}

s32 fn_80235A30(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80270350();
    }
    return 0;
}

s32 fn_80235ACC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80270358();
    }
    return 0;
}

// FIXME: 96.49 %. The original tests the mPlayerMask bit through an inline chain
// whose deeper copies MWCC calls out of line as the recursion is inlined: level 2
// calls fn_8023F9B0 (the game-root getter) and inlines GameRoot::fn_8023F98C, level 3
// calls both, level 4 calls fn_8018A614 (the one-argument form, act_phantom_ganon_2);
// here isPlayerBitSet loads gGameRoot_8053AAF8 directly (one instruction more at
// level 2, and isPlayerBitSet's / isPlayerActive's own copies at the deepest level).
// fn_80270360(serial) on the player when it takes part (mPlayerMask), else, while it
// follows one (fn_8022F514), on its leader (fn_8022F3DC), recursively.
void fn_80235B68(s32 player, s32 serial) {
    if (gPlayerManager_8053AC90 != NULL && isPlayerActive(player)) {
        if (isPlayerBitSet(gGameRoot_8053AAF8, player)) {
            gPlayerManager_8053AC90->mPlayers[player]->fn_80270360(serial);
        } else if (fn_8022F514(player)) {
            fn_80235B68(fn_8022F3DC(player), serial);
        }
    }
}

void fn_80235FE8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024E500();
    }
}

void fn_80236074(s32 player, s32 value, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024E694(value, arg);
    }
}

s32 fn_80236100(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802418E8();
    }
    return 0;
}

void fn_8023619C(s32 player, s32 value) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024ECCC(value);
    }
}

s32 fn_80236228(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241920();
    }
    return 0;
}

void fn_802362C4(s32 player, s32 value) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024ED4C(value);
    }
}

void fn_80236350(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_802708D0();
    }
}

void fn_802363DC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_802708E8();
    }
}

// fn_80270D34(amount) on `player` and every player of its group (fn_8022E438).
void fn_80236468(s32 player, s32 amount) {
    if (gPlayerManager_8053AC90 != NULL && isPlayerActive(player)) {
        for (s32 i = 0; i < 4; i++) {
            if (player == i || fn_8022E438(player, i)) {
                gPlayerManager_8053AC90->mPlayers[i]->fn_80270D34(amount);
            }
        }
    }
}

// fn_80270DDC(amount, flag) on `player` and every player of its group (fn_8022E438).
void fn_80236554(s32 player, s32 amount, bool flag) {
    if (gPlayerManager_8053AC90 != NULL && isPlayerActive(player)) {
        for (s32 i = 0; i < 4; i++) {
            if (player == i || fn_8022E438(player, i)) {
                gPlayerManager_8053AC90->mPlayers[i]->fn_80270DDC(amount, flag);
            }
        }
    }
}

s32 fn_80236630(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8023FFC4();
    }
    return 0;
}

s32 fn_802366CC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024018C();
    }
    return 0;
}

s32 fn_80236768(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80240354();
    }
    return 0;
}

void fn_80236804(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80270F00(arg);
    }
}

// Whether fn_80270A04 holds for every present player whose bit is set in the game root's 0x82.
bool fn_80236890() {
    if (gPlayerManager_8053AC90 == NULL) {
        return true;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) &&
            !gPlayerManager_8053AC90->mPlayers[i]->fn_80270A04()) {
            return false;
        }
    }
    return true;
}

s32 fn_8023697C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024322C();
    }
    return -1;
}

bool fn_80236A18(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return true;
    }
    if (!isPlayerActive(player)) {
        return true;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80270A04();
}

bool fn_80236AB4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return true;
    }
    if (!isPlayerActive(player)) {
        return true;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80270A58();
}

bool fn_80236B50(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return true;
    }
    if (!isPlayerActive(player)) {
        return true;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80270A8C();
}

bool fn_80236BEC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return true;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80270C88();
    }
    return true;
}

void fn_80236C88(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024148C();
    }
}

bool fn_80236D14(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return true;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241498();
    }
    return true;
}

void fn_80236DB0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_802414D0();
    }
}

bool fn_80236E3C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return true;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802414DC();
    }
    return true;
}

// fn_80241514(value) on a present `player`; with the game root's field_0x7f set,
// whether it holds for every present player of its group (fn_8022E438) instead.
bool fn_80236ED8(s32 player, s32 value) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    if (gGameRoot_8053AAF8->field_0x7f) {
        for (s32 i = 0; i < 4; i++) {
            if (isPlayerActive(i) && fn_8022E438(player, i) &&
                !gPlayerManager_8053AC90->mPlayers[i]->fn_80241514(value)) {
                return false;
            }
        }
        return true;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80241514(value);
}

s32 fn_80237060(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241544();
    }
    return 0;
}

// Whether fn_8024157C(arg) accepts for every present player of a present `player`'s
// group (fn_8022E438); false without the player.
bool fn_802370FC(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && fn_8022E438(player, i) &&
            !gPlayerManager_8053AC90->mPlayers[i]->fn_8024157C(arg)) {
            return false;
        }
    }
    return true;
}

s32 fn_8023725C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802415CC();
    }
    return 0;
}

// Whether fn_80241618(item, level) accepts for every present player of `player`'s group.
bool fn_802372F8(s32 player, s32 item, s32 level) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && fn_8022E438(player, i) &&
            !gPlayerManager_8053AC90->mPlayers[i]->fn_80241618(item, level)) {
            return false;
        }
    }
    return true;
}

s32 fn_802373EC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802416C4();
    }
    return 0;
}

bool fn_80237488(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80247594(arg);
    }
    return false;
}

void fn_80237524(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_802416FC(arg);
    }
}

s32 fn_802375B0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241794();
    }
    return 0;
}

void fn_8023764C(s32 player, s32 value) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80241804(value);
    }
}

s32 fn_802376D8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80241864();
    }
    return 0;
}

s32 fn_80237774(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024189C();
    }
    return 0;
}

s32 fn_80237810(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802417CC();
    }
    return 0;
}

bool fn_802378AC(s32 player, const JGeometry::TVec2f* pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80271038(pos);
    }
    return false;
}

bool fn_80237948(s32 player, const JGeometry::TVec2f* pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80271068(pos);
    }
    return false;
}

bool fn_802379E4(s32 player, const JGeometry::TVec3f* pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80271098(pos);
    }
    return false;
}

void fn_802381B8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80248DB8();
    }
}

void fn_80238244(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80248DCC();
    }
}

// fn_80248E94(flag) on a present `player` that takes part (its mPlayerMask bit) and on
// every present player of its group (fn_8022E438).
void fn_802382D0(s32 player, bool flag) {
    if (gPlayerManager_8053AC90 != NULL && isPlayerActive(player) && isPlayerBitSet(gGameRoot_8053AAF8, player)) {
        for (s32 i = 0; i < 4; i++) {
            if (isPlayerActive(i) && (player == i || fn_8022E438(player, i))) {
                gPlayerManager_8053AC90->mPlayers[i]->fn_80248E94(flag);
            }
        }
    }
}

// The player's field_0x324 (a TVec2f; the one caller reads x and y). The
// fallback is the shared no-player position, a TVec3f whose x and y it reads
// in the same way.
const JGeometry::TVec2f* fn_80238430(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return (const JGeometry::TVec2f*)&lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return &gPlayerManager_8053AC90->mPlayers[player]->field_0x324;
    }
    return (const JGeometry::TVec2f*)&lbl_804CF460;
}

void fn_802384BC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8024051C();
    }
}

// How many present players have fn_8022D7F8 equal to `serial`, all facing the same way
// (field_0x3dc); 0 as soon as two of them face differently.
s32 fn_80238A68(s32 serial) {
    s32 facing = -1;
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && serial == fn_8022D7F8(i)) {
            if (facing != -1 && facing != gPlayerManager_8053AC90->mPlayers[i]->field_0x3dc) {
                return 0;
            }
            count++;
            facing = gPlayerManager_8053AC90->mPlayers[i]->field_0x3dc;
        }
    }
    return count;
}

s32 fn_80238BC4(s32 serial) {
    if (fn_80239108(serial) <= 0) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80250030(serial)) {
            return gPlayerManager_8053AC90->mPlayers[i]->field_0x3dc;
        }
    }
    return -1;
}

s32 fn_80238CB4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024FF84();
    }
    return -1;
}

void fn_80238D50(s32 arg) {
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_8024FF9C(arg);
        }
    }
}

void fn_80238E08(s32 serial) {
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80250030(serial)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_8024FFB4(serial);
        }
    }
}

s32 fn_80238EDC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024FF54();
    }
    return -1;
}

s32 fn_80238F78(s32 serial) {
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && serial == gPlayerManager_8053AC90->mPlayers[i]->fn_8024FF6C()) {
            return i;
        }
    }
    return -1;
}

s32 fn_80239040(s32 serial) {
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && serial == gPlayerManager_8053AC90->mPlayers[i]->fn_8024FF84()) {
            return i;
        }
    }
    return -1;
}

// Every present player is tested; the first that passes is returned (0 without a manager).
s32 fn_80239470(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    s32 found = -1;
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_802728AC(box, layer, serial) &&
            found < 0) {
            found = i;
        }
    }
    return found;
}

s32 fn_80239558(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80272768(box, layer, serial)) {
            return i;
        }
    }
    return -1;
}

bool fn_80239638(s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_802729F0(serial)) {
            return true;
        }
    }
    return false;
}

bool fn_80239718(s32 player, s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802729F0(serial);
    }
    return false;
}

void fn_802397B4(const JGeometry::TBox2f* box, s32 arg_0x4, s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_80272C20(box, arg_0x4, serial);
        }
    }
}

bool fn_80239878(s32 player, const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80272C20(box, layer, serial);
    }
    return false;
}

void fn_80239914(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            gPlayerManager_8053AC90->mPlayers[i]->fn_80272DCC(box, layer, serial);
        }
    }
}

bool fn_802399D8(s32 player, const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80272DCC(box, layer, serial);
    }
    return false;
}

bool fn_80239C00(s32 player, const JGeometry::TBox2f* box, s32 layer) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80272E98(box, layer);
    }
    return false;
}

bool fn_80239C9C(s32 player, const JGeometry::TBox2f* box, s32 layer) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802724E0(box, layer);
    }
    return false;
}

// Whether no taking-part (mPlayerMask) present player has fn_80236AB4 set and all of
// them have fn_80236B50; false without a manager.
bool fn_80239D38() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && isPlayerBitSet(gGameRoot_8053AAF8, i)) {
            if (fn_80236AB4(i)) {
                return false;
            }
            if (!fn_80236B50(i)) {
                return false;
            }
        }
    }
    return true;
}

// Outside the game root's field_0x7f mode: whether no taking-part (mPlayerMask) present
// player has fn_80230188 or fn_8022D75C set and at most one of them lacks fn_80236A18.
bool fn_80239F10() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (gGameRoot_8053AAF8->field_0x7f) {
        return false;
    }
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && isPlayerBitSet(gGameRoot_8053AAF8, i)) {
            if (fn_80230188(i)) {
                return false;
            }
            if (fn_8022D75C(i)) {
                return false;
            }
            if (!fn_80236A18(i)) {
                count++;
            }
        }
    }
    return count <= 1;
}

s32 fn_8023A1A4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8026FC20();
    }
    return 0;
}

// The first present player's non-negative fn_8026FC68, else -1.
s32 fn_8023A240() {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i)) {
            s32 result = gPlayerManager_8053AC90->mPlayers[i]->fn_8026FC68();
            if (result >= 0) {
                return result;
            }
        }
    }
    return -1;
}

bool fn_8023A30C() {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_8026FCE0()) {
            return true;
        }
    }
    return false;
}

bool fn_8023A3DC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8026FCB0();
    }
    return false;
}

JGeometry::TVec3f* fn_8023A478(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8026FCC4();
    }
    return &lbl_804CF460;
}

bool fn_8023A51C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8026FCE0();
    }
    return false;
}

bool fn_8023A5B8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8026FC84();
    }
    return false;
}

void fn_8023A654(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->field_0xb9f = true;
    }
}

s32 fn_8023A7BC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80273FB8(box, layer, out)) {
            return i;
        }
    }
    return -1;
}

s32 fn_8023A89C(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80256DCC(box, layer, out)) {
            return i;
        }
    }
    return -1;
}

bool fn_8023A97C(s32 player, const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80256DCC(box, layer, out);
    }
    return false;
}

s32 fn_8023AA18(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80256F44(box, layer, out)) {
            return i;
        }
    }
    return -1;
}

u8 fn_8023AAF8(s32 player, const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80256F44(box, layer, out);
    }
    return false;
}

s32 fn_8023AB94(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_802570BC(box, layer, out)) {
            return i;
        }
    }
    return -1;
}

s32 fn_8023AC74(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_802574CC(box, layer, out)) {
            return i;
        }
    }
    return -1;
}

// The last present player whose fn_802575B8 test passes, else -1.
s32 fn_8023AD54(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    s32 found = -1;
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_802575B8(box, layer, out)) {
            found = i;
        }
    }
    return found;
}

// The last present player whose fn_802576CC test passes, else -1.
s32 fn_8023AE34(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    s32 found = -1;
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_802576CC(box, layer, out)) {
            found = i;
        }
    }
    return found;
}

s32 fn_8023AF14(const JGeometry::TBox2f* box, s32 layer, bool flag, f32 margin) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_802730D8(box, layer, flag, margin)) {
            return i;
        }
    }
    return -1;
}

bool fn_8023B0F4(s32 player, const JGeometry::TBox2f* box, s32 layer, bool flag, f32 margin) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802730D8(box, layer, flag, margin);
    }
    return false;
}

bool fn_8023B190(s32 player, const JGeometry::TBox2f* box, s32 layer) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802732C4(box, layer);
    }
    return false;
}

s32 fn_8023B22C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->field_0xb18;
    }
    return -1;
}

// The serial in field_0xb00 picked by field_0xb7c (0-2), else -1.
s32 fn_8023B2B0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (!isPlayerActive(player)) {
        return -1;
    }
    Player_804A6F38* p = gPlayerManager_8053AC90->mPlayers[player];
    if (p->field_0xb7c < 0 || p->field_0xb7c >= 3) {
        return -1;
    }
    return p->field_0xb00[p->field_0xb7c];
}

// The serial in field_0xb0c picked by field_0xb80 (0-2), else -1.
s32 fn_8023B358(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (!isPlayerActive(player)) {
        return -1;
    }
    Player_804A6F38* p = gPlayerManager_8053AC90->mPlayers[player];
    if (p->field_0xb80 < 0 || p->field_0xb80 >= 3) {
        return -1;
    }
    return p->field_0xb0c[p->field_0xb80];
}

bool fn_8023B400(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->field_0xb78;
    }
    return false;
}

bool fn_8023B484(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802577E0();
    }
    return false;
}

s32 fn_8023B520(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (!isPlayerActive(player)) {
        return 0;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->field_0xb9b ? 16 : 8;
}

s32 fn_8023B5B4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    return isPlayerActive(player) ? 4 : 0;
}

s32 fn_8023B630(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80272184();
    }
    return 0;
}

s32 fn_8023B6CC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_802721A8();
    }
    return 0;
}

s32 fn_8023B768(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80273518(box, layer, out)) {
            return i;
        }
    }
    return -1;
}

bool fn_8023B848(s32 player, const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80273518(box, layer, out);
    }
    return false;
}

s32 fn_8023B8E4(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* pos, bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80274238(box, layer, pos, flag)) {
            return i;
        }
    }
    return -1;
}

u8 fn_8023B9CC(s32 player, const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* pos, bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_80274238(box, layer, pos, flag);
    }
    return 0;
}

s32 fn_8023BA68(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80273C30(box, layer, out, flag)) {
            return i;
        }
    }
    return -1;
}

s32 fn_8023BB50(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && gPlayerManager_8053AC90->mPlayers[i]->fn_80273D44(box, layer, out, flag)) {
            return i;
        }
    }
    return -1;
}

s32 fn_8023BC38(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8023FC2C();
    }
    return 0;
}

u8 fn_8023BCD4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0xFF;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8023FC5C();
    }
    return 0xFF;
}

bool fn_8023BD70(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8023FF50();
    }
    return false;
}

bool fn_8023BE0C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8023FF88();
    }
    return false;
}

// FIXME: 99.95 %. reloc-name only, as fn_8023434C: the deepest inlined level calls the
// out-of-line player test, fn_8023F264 in the original, isPlayerActive's copy here.
// fn_80247B9C on the player or, while it follows one (fn_8022F514), on its leader
// (fn_8022F3DC), recursively; 0 without the player.
s32 fn_8023BEA8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (!isPlayerActive(player)) {
        return 0;
    }
    if (fn_8022F514(player)) {
        return fn_8023BEA8(fn_8022F3DC(player));
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80247B9C();
}

// FIXME: 99.95 %. reloc-name only, as fn_8023434C: the deepest inlined level calls the
// out-of-line player test, fn_8023F264 in the original, isPlayerActive's copy here.
// fn_80247BD4 on the player or, while it follows one (fn_8022F514), on its leader
// (fn_8022F3DC), recursively; 0 without the player.
s32 fn_8023C36C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (!isPlayerActive(player)) {
        return 0;
    }
    if (fn_8022F514(player)) {
        return fn_8023C36C(fn_8022F3DC(player));
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80247BD4();
}

// fn_80247C0C on a present player for which fn_8022F478 does not hold: on the player
// it follows (fn_8022F3DC) when fn_8022F514 holds, else on itself. False otherwise.
bool fn_8023C830(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (!isPlayerActive(player)) {
        return false;
    }
    if (fn_8022F478(player)) {
        return false;
    }
    if (fn_8022F514(player)) {
        return gPlayerManager_8053AC90->mPlayers[fn_8022F3DC(player)]->fn_80247C0C();
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80247C0C();
}

s8 fn_8023CA88(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024EE14();
    }
    return 0;
}

s8 fn_8023CB24(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024EE0C();
    }
    return 0;
}

u16 fn_8023CBC0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0x121;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024EE1C();
    }
    return 0x121;
}

void fn_8023CC5C(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80252F50(arg);
    }
}

void fn_8023CCE8(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80252F5C(arg);
    }
}

s32 fn_8023CD74(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (!isPlayerActive(player)) {
        return 0;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80252F68();
}

s32 fn_8023CE10(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (!isPlayerActive(player)) {
        return 0;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->fn_80252F74();
}

// The screen y of `pos` (y - z) relative to the player's fn_8023CD74, or 0xE0 when
// it is outside [-0x20, 0xE0) or there is no such player. Called by tu_80301CD0, which
// keeps 8 bits of it.
u16 fn_8023D4F8(const JGeometry::TVec3f* pos, s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0xE0;
    }
    if (!isPlayerActive(player)) {
        return 0xE0;
    }
    s32 y = (s32)(pos->y - pos->z) - fn_8023CD74(player);
    if (y >= -0x20 && y < 0xE0) {
        return y;
    }
    return 0xE0;
}

// The screen x of `pos` relative to the player's fn_8023CE10, or 0x180 when it is
// outside [-0x40, 0x140) or there is no such player. Called by tu_80301CD0, which keeps
// 9 bits of it.
u16 fn_8023D9B0(const JGeometry::TVec3f* pos, s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0x180;
    }
    if (!isPlayerActive(player)) {
        return 0x180;
    }
    s32 x = (s32)pos->x - fn_8023CE10(player);
    if (x >= -0x40 && x < 0x140) {
        return x;
    }
    return 0x180;
}

s32 getPlayerLayer_8023DE58(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (!isPlayerActive(player)) {
        return 0;
    }
    return gPlayerManager_8053AC90->mPlayers[player]->field_0x3b4;
}

void fn_8023DEDC(s32 player, const JGeometry::TVec3f* offset) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80252BD0(offset);
    }
}

void fn_8023DF68(s32 player, const JGeometry::TVec3f* pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80252CF0(pos);
    }
}

void fn_8023DFF4(s32 player, const JGeometry::TVec3f* offset) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80252F14(offset);
    }
}

void fn_8023E080(s32 player, s32 arg) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_80252F40(arg);
    }
}

s32 fn_8023E10C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->field_0xce4;
    }
    return 0;
}

s32 fn_8023E190(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->field_0xce0;
    }
    return 0;
}

JGeometry::TVec3f* fn_8023E214(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return &gPlayerManager_8053AC90->mPlayers[player]->field_0xcd4;
    }
    return &lbl_804CF460;
}

JGeometry::TVec3f* fn_8023E2A0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return &gPlayerManager_8053AC90->mPlayers[player]->field_0xcc8;
    }
    return &lbl_804CF460;
}

JGeometry::TVec3f* fn_8023E32C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return &gPlayerManager_8053AC90->mPlayers[player]->field_0xcb0;
    }
    return &lbl_804CF460;
}

JGeometry::TVec3f* fn_8023E3B8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024C378();
    }
    return &lbl_804CF460;
}

u8 fn_8023E45C(s32 player, s32 index) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->field_0x284[index];
    }
    return 0;
}

JGeometry::TVec3f* fn_8023E4E4(s32 player, s32 index) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return &gPlayerManager_8053AC90->mPlayers[player]->field_0x38[index];
    }
    return &lbl_804CF460;
}

JGeometry::TVec3f* fn_8023E578(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return &gPlayerManager_8053AC90->mPlayers[player]->field_0x14;
    }
    return &lbl_804CF460;
}

JGeometry::TVec3f* fn_8023E604(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return &gPlayerManager_8053AC90->mPlayers[player]->field_0x2c4;
    }
    return &lbl_804CF460;
}

void fn_8023E690(s32 player, const JGeometry::TVec3f* pos) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        Player_804A6F38* p = gPlayerManager_8053AC90->mPlayers[player];
        p->mPos.set(*pos);
        p->field_0x2c.set(*pos);
    }
}

JGeometry::TVec3f* getPlayerPos_8023E724(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return &gPlayerManager_8053AC90->mPlayers[player]->mPos;
    }
    return &lbl_804CF460;
}

JGeometry::TVec3f* fn_8023E7B0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return &lbl_804CF460;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024145C();
    }
    return &lbl_804CF460;
}

bool fn_8023E854(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8024F5E4();
    }
    return false;
}

bool fn_8023E8F0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F2CC();
    }
    return false;
}

bool fn_8023E98C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F418();
    }
    return false;
}

bool fn_8023EA28(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F448();
    }
    return false;
}

// Whether a present player of `player`'s group has fn_8025F484.
bool fn_8023EAC4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    for (s32 i = 0; i < 4; i++) {
        if (isPlayerActive(i) && fn_8022E438(player, i) && gPlayerManager_8053AC90->mPlayers[i]->fn_8025F484()) {
            return true;
        }
    }
    return false;
}

bool fn_8023EBB8(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F500();
    }
    return false;
}

s32 fn_8023EC54(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F534();
    }
    return -1;
}

s32 fn_8023ECF0(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return -1;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F55C();
    }
    return -1;
}

bool fn_8023ED8C(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F590();
    }
    return false;
}

void fn_8023EE28(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    if (isPlayerActive(player)) {
        gPlayerManager_8053AC90->mPlayers[player]->fn_8025F5CC();
    }
}

s32 fn_8023EEB4(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F648();
    }
    return 0;
}

s32 fn_8023EF50(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return 0;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F668();
    }
    return 0;
}

bool fn_8023EFEC(s32 player) {
    if (gPlayerManager_8053AC90 == NULL) {
        return false;
    }
    if (isPlayerActive(player)) {
        return gPlayerManager_8053AC90->mPlayers[player]->fn_8025F684();
    }
    return false;
}

// FIXME: 99.88 %. reloc-name only: 4.0f and 1.0f are the original's lbl_8054002C /
// lbl_8054001C, pooled after literals of functions not written yet (the code is identical).
// Shuffles the four start entries (position, a, b, c are moved together only for the
// position: each slot swaps its position with a random slot's), then hands every
// player its entry (fn_80252F80, as fn_8023F1D4 does).
void fn_8023F088() {
    if (gPlayerManager_8053AC90 == NULL) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        s32 j = (s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        if (j >= 4) {
            j = 3;
        }
        JGeometry::TVec3f pos(gPlayerManager_8053AC90->field_0x14[i]);
        gPlayerManager_8053AC90->field_0x14[i] = gPlayerManager_8053AC90->field_0x14[j];
        gPlayerManager_8053AC90->field_0x14[j] = pos;
    }
    for (s32 i = 0; i < 4; i++) {
        gPlayerManager_8053AC90->mPlayers[i]->fn_80252F80(&gPlayerManager_8053AC90->field_0x14[i],
                                                          gPlayerManager_8053AC90->field_0x44[i],
                                                          gPlayerManager_8053AC90->field_0x54[i],
                                                          gPlayerManager_8053AC90->field_0x64[i]);
    }
}

void fn_8023F1D4(s32 player, const JGeometry::TVec3f* pos, s32 a, s32 b, u8 c) {
    gPlayerManager_8053AC90->mPlayers[player]->fn_80252F80(pos, a, b, c);
    gPlayerManager_8053AC90->field_0x14[player] = *pos;
    gPlayerManager_8053AC90->field_0x44[player] = a;
    gPlayerManager_8053AC90->field_0x54[player] = b;
    gPlayerManager_8053AC90->field_0x64[player] = c;
}

bool fn_8023F264(s32 player) {
    return isPlayerActive(player);
}

// The player in slot `i`, or NULL without a manager (the manager loops below
// fetch it this way every time they use it).
static inline Player_804A6F38* playerAt(s32 i) {
    if (gPlayerManager_8053AC90 == NULL) {
        return NULL;
    }
    return gPlayerManager_8053AC90->mPlayers[i];
}

// The players' draw: the slots of players not taking part first, then those
// taking part, each from 3 down to 0.
void fn_8023F2BC() {
    for (s32 i = 3; i >= 0; i--) {
        if (!isPlayerBitSet(gGameRoot_8053AAF8, i)) {
            playerAt(i)->fn_80264998();
        }
    }
    for (s32 i = 3; i >= 0; i--) {
        if (isPlayerBitSet(gGameRoot_8053AAF8, i)) {
            playerAt(i)->fn_80264998();
        }
    }
}

// FIXME: 99.7 %. One instruction: the last loop's slot offset starts as
// `mr r30, r31` (a copy of the counter's zero) in the original and as
// `li r30, 0` here; counter type, while / for and the NULL test's form
// don't change it. Its fn_8022F478 / getPlayerLayer_8023DE58 copies are inlined.
// The players' update: update A for every slot; unless the game is held
// (gGameRoot_8053AAF8->field_0x94 == 1) update B, the per-frame bytes 0x434/0x435,
// the cross-player passes and fn_80252FBC; then the audio flag: 0 when an
// active player without fn_80240670 is on a layer below 8, else 1.
void fn_8023F390() {
    for (s32 i = 0; i < 4; i++) {
        playerAt(i)->fn_80253E6C();
    }
    if (gGameRoot_8053AAF8->field_0x94 == 1) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        playerAt(i)->fn_80252FC0();
    }
    for (s32 i = 0; i < 4; i++) {
        playerAt(i)->field_0x434 = 0;
        playerAt(i)->field_0x435 = 0;
    }
    fn_80232A0C();
    if (gGameRoot_8053AAF8->field_0x7f) {
        fn_80231DE4();
    }
    for (s32 i = 0; i < 4; i++) {
        playerAt(i)->fn_80252FBC();
    }
    if (gPlayerManager_8053AC90 != NULL) {
        for (s32 i = 0; i < 4; i++) {
            if (isPlayerActive(i) && !fn_8022F478(i) && getPlayerLayer_8023DE58(i) < 8) {
                gAudioMgr_8053AF80->fn_80457DFC(0);
                return;
            }
        }
        gAudioMgr_8053AF80->fn_80457DFC(1);
    }
}

void fn_8023F6C4() {
    for (s32 i = 0; i < 4; i++) {
        Player_804A6F38* player;
        if (gPlayerManager_8053AC90 == NULL) {
            player = NULL;
        } else {
            player = gPlayerManager_8053AC90->mPlayers[i];
        }
        player->fn_80254660();
    }
}

// The possibly-NULL getter is inlined per slot (a NULL manager gives a NULL player).
void fn_8023F728() {
    for (s32 i = 0; i < 4; i++) {
        Player_804A6F38* player;
        if (gPlayerManager_8053AC90 == NULL) {
            player = NULL;
        } else {
            player = gPlayerManager_8053AC90->mPlayers[i];
        }
        player->fn_802544B0();
    }
}

// As fn_8023F728.
void fn_8023F78C() {
    for (s32 i = 0; i < 4; i++) {
        Player_804A6F38* player;
        if (gPlayerManager_8053AC90 == NULL) {
            player = NULL;
        } else {
            player = gPlayerManager_8053AC90->mPlayers[i];
        }
        player->fn_80254744();
    }
    lbl_8053A190 = -1;
}

PlayerManager_804A6F28::~PlayerManager_804A6F28() {
    gAudioMgr_8053AF80->fn_8045815C(false);
    for (s32 i = 0; i < 4; i++) {
        if (mPlayers[i] != NULL) {
            delete mPlayers[i];
        }
    }
    gPlayerManager_8053AC90 = NULL;
}

PlayerManager_804A6F28::PlayerManager_804A6F28() {
    gPlayerManager_8053AC90 = this;
    for (s32 i = 0; i < 4; i++) {
        mPlayers[i] = new Player_804A6F38(i);
    }
    field_0x68 = 0;
    field_0x6c = false;
    field_0x6d = false;
    field_0x6e = false;
    field_0x6f = false;
    field_0x70 = false;
    field_0x71 = false;
    field_0x72 = false;
    field_0x73 = false;
    gAudioMgr_8053AF80->fn_8045815C(false);
}

bool GameRoot_8049EF20::fn_8023F98C(s32 player) {
    return isPlayerBitSet(this, player);
}

GameRoot_8049EF20* fn_8023F9B0() {
    return gGameRoot_8053AAF8;
}
