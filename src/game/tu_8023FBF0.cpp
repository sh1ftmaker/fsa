#include "JSystem/JUtility/JUTGamePad.h"
#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/GameState.h"
#include "game/PlayerBit.h"
#include "game/PlayerFlags.h"
#include "game/StageFlag.h"
#include "game/ViewLayer.h"
#include "game/cls_804A71A8.h"
#include "game/cls_804AE9B0.h"
#include "game/cls_804B20F8.h"
#include "game/tu_8030AE84.h"
#include "game/tu_80379F84.h"

void Player_804A6F38::fn_8023FBF0() {
    if (!field_0x42c) {
        return;
    }
    field_0x42d = 1;
}

bool Player_804A6F38::fn_8023FC08() {
    return field_0x42c;
}

void Player_804A6F38::fn_8023FC10(s32 value) {
    if (field_0xca8 < value) {
        field_0xca8 = value;
    }
}

cls_804AA8D8* Player_804A6F38::fn_8023FC24() {
    return field_0xbec;
}

s32 Player_804A6F38::fn_8023FC2C() {
    if (field_0xd2d == 1) {
        return 0;
    }
    if (field_0x498 == 0) {
        return 0;
    }
    return field_0x494;
}

bool Player_804A6F38::fn_8023FF50() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_8023BD70(team);
    }
    return field_0x1202;
}

bool Player_804A6F38::fn_8023FF88() {
    return field_0x1200;
}

void Player_804A6F38::fn_8023FF90(const JGeometry::TVec3f* arg) {
    field_0xc90.set(arg->x, arg->y, arg->z);
}

s32 Player_804A6F38::fn_8023FFAC() {
    return field_0xc84.z < 1.0f;
}

s32 Player_804A6F38::fn_80240628() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return team;
    }
    return field_0x4;
}

s32 Player_804A6F38::fn_80240644() {
    return field_0xd60;
}

s32 Player_804A6F38::fn_8024064C() const {
    return fn_8022EA0C(field_0x4);
}

bool Player_804A6F38::fn_80240808() {
    return field_0xd60 >= 0;
}

bool Player_804A6F38::fn_80240818() {
    return field_0x1240 == 0x49 || field_0x1244 == 0x49;
}

bool Player_804A6F38::fn_80240840(s32 arg) {
    if (field_0x11f4 <= 0) {
        field_0x11f4 = arg;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_80240860() {
    return field_0x436;
}

s32 Player_804A6F38::fn_80240868() {
    return field_0x4;
}

bool Player_804A6F38::fn_80240870() {
    return field_0xcfe;
}

void Player_804A6F38::fn_80240878() {
    field_0x1224++;
}

s32 Player_804A6F38::fn_80240888() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80234004(team);
    }
    return field_0x1224;
}

bool Player_804A6F38::fn_802408C0() {
    return field_0x1217;
}

bool Player_804A6F38::fn_80240AD4() {
    if (field_0xce8) {
        return false;
    }
    return field_0x1216;
}

bool Player_804A6F38::fn_80240AF0() {
    return field_0x1215;
}

bool Player_804A6F38::fn_80240AF8() {
    return field_0x1214;
}

bool Player_804A6F38::fn_80240B00() {
    return field_0x1240 == 0xf || field_0x1240 == 0x10;
}

bool Player_804A6F38::fn_80240B20() {
    return field_0x1240 == 0x11;
}

bool Player_804A6F38::fn_80240B34() {
    return field_0x1240 == 0x51 && field_0x46c >= 0xd2;
}

bool Player_804A6F38::fn_80240B5C() {
    return field_0x1240 == 0x51;
}

bool Player_804A6F38::fn_80240B70() {
    return field_0x1240 == 0x50;
}

bool Player_804A6F38::fn_80240D50() {
    return field_0x1240 == 0x4f;
}

bool Player_804A6F38::fn_80240D64() {
    return field_0x1240 == 0x4c || field_0x1240 == 0x4d || field_0x1240 == 0x4e;
}

bool Player_804A6F38::fn_80240D8C() {
    return field_0x1244 == 0x38 || field_0x1244 == 0x13;
}

bool Player_804A6F38::fn_80240DAC() {
    return field_0x1240 == 0x38 || field_0x1240 == 0x13;
}

bool Player_804A6F38::fn_80240ED4() {
    return field_0x1240 == 0x1e || field_0x1240 == 0x32;
}

bool Player_804A6F38::fn_80240EF4() {
    return field_0x1240 == 0x42 || field_0x1240 == 0x43;
}

bool Player_804A6F38::fn_80240F14() {
    return field_0x1240 == 0x41;
}

bool Player_804A6F38::fn_8024119C() {
    return field_0x1240 == 0xc && field_0x11e4 != 0xc;
}

bool Player_804A6F38::fn_802411C4() {
    return field_0x1240 == 0xc;
}

bool Player_804A6F38::fn_802411D8() {
    return field_0x1240 == 0xa;
}

bool Player_804A6F38::fn_802411EC() {
    return field_0x1240 == 1;
}

bool Player_804A6F38::fn_80241200() {
    return field_0x1240 == 1 || (field_0x3c4 >= 0x1bf && field_0x3c4 < 0x1c6);
}

bool Player_804A6F38::fn_80241230() {
    return field_0x1240 == 0xb || field_0x1244 == 0xb;
}

bool Player_804A6F38::fn_80241258() {
    return fn_8027218C() == 4;
}

bool Player_804A6F38::fn_80241284() {
    return field_0x1240 == 9 || field_0x1240 == 0xa;
}

bool Player_804A6F38::fn_802412A4() {
    return field_0x1240 == 0x4a || field_0x1240 == 0x4b;
}

u8 Player_804A6F38::fn_802412C4() {
    return field_0x458;
}

bool Player_804A6F38::fn_802412CC() {
    return field_0x460;
}

void Player_804A6F38::fn_802412D4() {
    field_0xcaf = 1;
}

bool Player_804A6F38::fn_802412E0() {
    return field_0x1240 == 0xd || field_0x1240 == 0xe || field_0x1240 == 0x11 || field_0x1240 == 0x12 ||
           field_0x1240 == 0x1a || field_0x1240 == 0x3c || field_0x1240 == 0x3b || field_0x1240 == 0x3a ||
           field_0x1240 == 0x47;
}

bool Player_804A6F38::fn_80241338() {
    return field_0x1240 == 4 || field_0x1240 == 8 || field_0x1240 == 0x39 || field_0x1240 == 0x44 ||
           field_0x1240 == 0x45 || field_0x1240 == 0x3d || field_0x1240 == 0x37;
}

bool Player_804A6F38::fn_80241380() {
    return field_0x1240 == 0 || field_0x1240 == 2 || field_0x1240 == 3 || field_0x1240 == 4 ||
           field_0x1240 == 8 || field_0x1240 == 0x4a || field_0x1240 == 0x4b;
}

bool Player_804A6F38::fn_802413C8() {
    return field_0x1240 == 8 || field_0x1240 == 0x37;
}

bool Player_804A6F38::fn_802413E8() {
    return field_0x454;
}

bool Player_804A6F38::fn_802413F0() {
    return playerFlags_802413F0(this);
}

void Player_804A6F38::fn_80241454(u8 value) {
    field_0xc80 = value;
}

JGeometry::TVec3f* Player_804A6F38::fn_8024145C() {
    if (field_0xd64 == 1) {
        return &field_0x2f4;
    }
    return &mPos;
}

bool Player_804A6F38::fn_80241478() {
    return field_0x1240 == 0xe;
}

void Player_804A6F38::fn_8024148C() {
    field_0xb7b = true;
}

bool Player_804A6F38::fn_80241498() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80236D14(team);
    }
    return field_0xb7b;
}

void Player_804A6F38::fn_802414D0() {
    field_0xb7a = true;
}

bool Player_804A6F38::fn_802414DC() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80236E3C(team);
    }
    return field_0xb7a;
}

bool Player_804A6F38::fn_80241514(s32 value) {
    field_0xc18 = value;
    if (!lbl_8053AAF8->field_0x7f) {
        if (field_0x464 < 0x3c) {
            field_0x464 = 0x3c;
        }
    }
    return true;
}

s32 Player_804A6F38::fn_80241544() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80237060(team);
    }
    return field_0xc18;
}

s32 Player_804A6F38::fn_802415CC() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_8023725C(team);
    }
    if (field_0xc10 == 0) {
        return 0;
    }
    return field_0xc14;
}

s32 Player_804A6F38::fn_802416C4() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_802373EC(team);
    }
    return field_0xc10;
}

// FIXME: 97.1. After the fn_80270A8C test the target branches (beq) over an extra `b` to the epilogue where ours
// emits a single bne to the epilogue.
void Player_804A6F38::fn_802416FC(s32 arg) {
    s32 team = field_0xd60;
    if (team >= 0) {
        fn_80237524(team, arg);
        return;
    }
    if (!lbl_8053AAF8->field_0x7f) {
        if (field_0xbfc <= 0) {
            return;
        }
        if (fn_80270A8C()) {
            return;
        }
    }
    field_0xc08 += arg;
    if (field_0xc08 > 0x180) {
        field_0xc08 = 0x180;
    }
}

s32 Player_804A6F38::fn_80241794() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_802375B0(team);
    }
    return field_0xc08;
}

s32 Player_804A6F38::fn_802417CC() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80237810(team);
    }
    return field_0xbf8;
}

void Player_804A6F38::fn_80241804(s32 value) {
    s32 team = field_0xd60;
    if (team >= 0) {
        fn_8023764C(team, value);
    } else if (value < 0) {
        field_0xbfc = 0;
    } else if (value >= field_0xbf8) {
        field_0xbfc = field_0xbf8;
    } else {
        field_0xbfc = value;
    }
}

s32 Player_804A6F38::fn_80241864() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_802376D8(team);
    }
    return field_0xbfc;
}

s32 Player_804A6F38::fn_8024189C() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80237774(team);
    }
    if (field_0xc00) {
        return 0;
    }
    return field_0xbfc;
}

s32 Player_804A6F38::fn_802418E8() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80236100(team);
    }
    return field_0xc04;
}

s32 Player_804A6F38::fn_80241920() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80236228(team);
    }
    s32 value = field_0xc04;
    return value >= 0 ? value : 0;
}

bool Player_804A6F38::fn_802426E0(s32 serial) {
    if (field_0xd8c != serial) {
        return false;
    }
    field_0xd90 = 1;
    return true;
}

bool Player_804A6F38::fn_80243194(s32 other) {
    return fn_8022E438(field_0x4, other);
}

bool Player_804A6F38::fn_802431B8() {
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023697C(i) >= 0) {
            if (fn_8022E438(fn_8023697C(i), field_0x4)) {
                return true;
            }
        }
    }
    return false;
}

s32 Player_804A6F38::fn_8024322C() {
    return field_0xd08;
}

// The size of this player's group: itself, its leader (field_0xd60) and
// every active follower (fn_8022F514) whose leader (fn_8022F3DC) is that
// leader, or this player when it leads. fn_80248970's body; MWCC inlines it
// into fn_802486A0 and fn_802487FC, defined before it, so it is a file inline.
static inline s32 groupSize(const Player_804A6F38* player) {
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i == player->field_0x4) {
            count++;
        } else if (fn_8022F514(i)) {
            s32 team = player->field_0xd60;
            if (team >= 0) {
                if (team == fn_8022F3DC(i)) {
                    count++;
                }
            } else {
                s32 self = player->field_0x4;
                if (self == fn_8022F3DC(i)) {
                    count++;
                }
            }
        } else if (i == player->field_0xd60) {
            count++;
        }
    }
    return count;
}

// The group's mode: 5 alone, else the leader's (fn_8022CAF8) or, leading,
// field_0x120c. fn_802486A0's body (inlined before its definition, as above).
static inline s32 groupMode(const Player_804A6F38* player) {
    if (groupSize(player) <= 1) {
        return 5;
    }
    if (player->field_0xd60 >= 0) {
        return fn_8022CAF8(player->field_0xd60);
    }
    return player->field_0x120c;
}

// The place of this player in its leader's line (1 for the first follower),
// or 0 when it leads. fn_80248778's body (inlined before its definition).
static inline s32 linePlace(const Player_804A6F38* player) {
    if (player->field_0xd60 < 0) {
        return 0;
    }
    s32 count = 0;
    for (s32 i = 0; i < player->field_0x4; i++) {
        s32 partner = player->field_0xd60;
        if (partner == fn_8022F3DC(i)) {
            count++;
        }
    }
    return count + 1;
}

// The gate in front of every pad query of the player (fn_80246904 and the
// functions around it): the player flags (fn_802413F0); in group mode 4
// only the player whose turn it is (fn_80240888 == its place in line); and
// field_0x11ea.
static inline bool teamCheck(Player_804A6F38* player) {
    if (!playerFlags_802413F0(player)) {
        return false;
    }
    if ((groupMode(player) == 4 && player->fn_80240888() != linePlace(player)) || !player->field_0x11ea) {
        return false;
    }
    return true;
}

// FIXME: 96.8 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
bool Player_804A6F38::fn_802454C0() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309A54(fn_8030AAF4(field_0x11ec), 0x20) != 0;
}

// FIXME: 96.8 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
bool Player_804A6F38::fn_802456FC() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D24(fn_8030AAF4(field_0x11ec), 0x20) != 0;
}

// FIXME: 96.8 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
bool Player_804A6F38::fn_80245938() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D6C(fn_8030AAF4(field_0x11ec), 0x20) != 0;
}

// FIXME: 96.8 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
bool Player_804A6F38::fn_80245B74() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309A78(fn_8030AAF4(field_0x11ec), 0x100) != 0;
}

// FIXME: 96.8 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
bool Player_804A6F38::fn_80245DB0() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D6C(fn_8030AAF4(field_0x11ec), 0x100) != 0;
}

// FIXME: 96.8 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
bool Player_804A6F38::fn_80245FEC() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D24(fn_8030AAF4(field_0x11ec), 0x100) != 0;
}

// FIXME: 96.8 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
bool Player_804A6F38::fn_80246228() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D6C(fn_8030AAF4(field_0x11ec), 0x100) != 0;
}

// FIXME: 96.9 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
bool Player_804A6F38::fn_80246464() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (fn_8022B244()) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D24(fn_8030AAF4(field_0x11ec), 0x200) != 0;
}

// FIXME: 96.9 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
bool Player_804A6F38::fn_802466B4() {
    if (teamCheck(this) == false) {
        return false;
    }
    if (fn_8022B244()) {
        return false;
    }
    if (field_0x456) {
        return false;
    }
    if (field_0x11f0 > 0) {
        return false;
    }
    return lbl_8053AD30->fn_80309D6C(fn_8030AAF4(field_0x11ec), 0x200) != 0;
}

// FIXME: 96.5 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
s32 Player_804A6F38::fn_80246904(u32 mask) {
    if (teamCheck(this) == false) {
        return 0;
    }
    return lbl_8053AD30->fn_80309A78(fn_8030AAF4(field_0x11ec), mask);
}

// FIXME: 96.5 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
u32 Player_804A6F38::fn_80246B10(u32 mask) {
    if (teamCheck(this) == false) {
        return 0;
    }
    return lbl_8053AD30->fn_80309D24(fn_8030AAF4(field_0x11ec), mask);
}

// FIXME: 96.5 %. As fn_80246F28 (the 0xCE9 byte test of the inlined
// playerFlags_802413F0, and the group loop's registers).
u32 Player_804A6F38::fn_80246D1C(u32 mask) {
    if (teamCheck(this) == false) {
        return 0;
    }
    return lbl_8053AD30->fn_80309D6C(fn_8030AAF4(field_0x11ec), mask);
}

// FIXME: 96.5 %. The playerFlags_802413F0 chain ends in a byte test that
// the original branches on (cmplwi / bne, li 1 / li 0) and ours converts
// (neg / or / srwi, as fn_802413F0's own out-of-line copy does): the if form
// `if (!field_0xce9) return false; return true;` gives the inlined copies
// exactly (99.5 %, registers only) but breaks fn_802413F0 (100 -> 84.6 %),
// so the header keeps the form fn_802413F0 proves. Also the group loop's
// count / index / leader get r27 / r26 / r28 where the original has r28 / r27 / r26.
f32 Player_804A6F38::fn_80246F28() {
    // `== false`: the check's bool is materialised and tested (li 0 / li 1,
    // clrlwi.; bne to the call), as with `x == true` (docs/RULES_CARD.md).
    if (teamCheck(this) == false) {
        return 0.0f;
    }
    return lbl_8053AD30->fn_80309D90(fn_8030AAF4(field_0x11ec));
}

u8 Player_804A6F38::fn_80247534() {
    return lbl_8053AD30->fn_8030A308(fn_8030AAF4(field_0x11ec));
}

bool Player_804A6F38::fn_80247564() {
    return lbl_8053AD30->fn_8030A3B0(fn_8030AAF4(field_0x11ec));
}

bool Player_804A6F38::fn_80247594(s32 arg) {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80237488(team, arg);
    }
    if (field_0x1201 == 1) {
        return true;
    }
    if (field_0xc08 >= arg) {
        field_0xc08 -= arg;
        field_0x1201 = 1;
        return true;
    }
    if (field_0xc08 > 0) {
        field_0xc08 = 0;
        field_0x1201 = 1;
        return true;
    }
    field_0x1202 = true;
    return false;
}

s32 Player_804A6F38::fn_80247B9C() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_8023BEA8(team);
    }
    return field_0xbc0;
}

s32 Player_804A6F38::fn_80247BD4() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_8023C36C(team);
    }
    return field_0x48c;
}

void Player_804A6F38::fn_80247F0C() {
    if (field_0xc9c > 0) {
        field_0xc9c--;
        return;
    }
    if (field_0xc84.z < 1.0f) {
        field_0xc84.z += 0.05f;
        if (field_0xc84.z >= 1.0f) {
            field_0xc84.z = 1.0f;
        }
    }
}

void Player_804A6F38::fn_80247F58() {
    s32 team = field_0xd60;
    if (team >= 0 && fn_80236B50(team)) {
        if (fn_80251BFC(0x33)) {
            // positions are read as TVec2f (x, y) by the callee
            fn_80272160(0, (const JGeometry::TVec2f*)&mPos, 0x12);
        }
    } else if (fn_80239D38() && !field_0xd34 && fn_80251BFC(0x33)) {
        fn_80272160(0, (const JGeometry::TVec2f*)&mPos, 0x12);
    }
}

// FIXME: 88.2. Loop body identical; the original keeps the index zero in r6 (set before li r0, 0x31) and
// copies it to r4 as the false stored to field_0x284, where ours uses separate li r7 / li r4.
void Player_804A6F38::fn_80248604() {
    if (field_0x1216) {
        return;
    }
    if (field_0x454) {
        fn_8024B2E0();
        return;
    }
    for (s32 i = 0; i < 0x31; i++) {
        field_0x38[i].x = mPos.x;
        field_0x38[i].y = mPos.y;
        field_0x38[i].z = mPos.z;
        if (mPos.z > 0.0f) {
            field_0x284[i] = true;
        } else {
            field_0x284[i] = false;
        }
    }
}

// FIXME: 97.7 %. Register allocation only: the original gives the count r29,
// the index r30 and the leader r28, ours r28 / r29 / r30. Const: the loop
// test's field_0x4 is hoisted out of the loop (r31), as in fn_80248778.
s32 Player_804A6F38::fn_802486A0() const {
    return groupMode(this);
}

// The place of this player in its partner's line (1 for the first follower),
// or 0 without a partner. fn_80254744 inlines it and reloads both fields on
// every pass; this const copy keeps them in registers.
s32 Player_804A6F38::fn_80248778() const {
    if (field_0xd60 < 0) {
        return 0;
    }
    s32 count = 0;
    for (s32 i = 0; i < field_0x4; i++) {
        s32 partner = field_0xd60;
        if (partner == fn_8022F3DC(i)) {
            count++;
        }
    }
    return count + 1;
}

void Player_804A6F38::fn_80248DAC() {
    field_0x121e = 0;
}

void Player_804A6F38::fn_80248DB8() {
    field_0x1217 = 0;
    field_0x1218 = 3;
}

void Player_804A6F38::fn_80248DCC() {
    field_0x1238 = 0xb4;
}

void Player_804A6F38::fn_80248DD8() {
    field_0x1214 = true;
    if (field_0xd60 >= 0) {
        field_0x1215 = true;
        const JGeometry::TVec3f* pos = fn_8023E724(field_0xd60);
        mPos.x = pos->x;
        mPos.y = pos->y;
        mPos.z = pos->z;
        if (fn_80251BFC(3)) {
            field_0x1244 = 3;
            field_0x124c = true;
            if (field_0xd60 < 0 && field_0xcfe == 1 && field_0x1244 != 0) {
                field_0xcfe = false;
            }
        }
    }
    field_0x1224 = 0;
    field_0x1217 = false;
}

bool Player_804A6F38::fn_80249068() {
    return field_0x121f;
}

void Player_804A6F38::fn_80249070() {
    if (field_0x121f == 1) {
        field_0x121f = 0;
        if (field_0x11f0 < 5) {
            field_0x11f0 = 5;
        }
    }
}

void Player_804A6F38::fn_8024909C() {
    field_0x121f = 1;
}

// FIXME: 97.4 %. Every call, field and branch is in place; the 13 inlined
// pad gates (fn_80246B10 / fn_80246D1C) differ only in the 0xCE9 byte test
// of playerFlags_802413F0 (see fn_80246F28: 26 instructions); with the if
// form there this is 99.9 %, registers only (field_0x4 copy r30 vs r26).
// The player's input each frame (update A, fn_80253E6C): the 'FORM' call
// of a group leader and the four direction picks (fn_80248004(dir, true))
// on the pad buttons or the C stick.
void Player_804A6F38::fn_802490A8() {
    if (field_0x1220 > 0) {
        return;
    }
    s32 player = field_0x4;
    if (fn_802486A0() == 4) {
        player = fn_8022EA0C(field_0x4);
    }
    if (fn_80230188(player)) {
        return;
    }
    if (fn_80236B50(player)) {
        return;
    }
    if (fn_8023E724(player)->z > 0.0f) {
        return;
    }
    if (fn_8023EA28(player)) {
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022E438(field_0x4, i)) {
            if (fn_8023E98C(i) && !fn_8022E438(i, fn_8023EC54(i))) {
                return;
            }
            if (fn_8022DEAC(i) && fn_802486A0() != 4) {
                return;
            }
            if (fn_8022D930(i)) {
                return;
            }
            if (fn_8022D894(i)) {
                return;
            }
            if (fn_8022D75C(i)) {
                return;
            }
            if (fn_80235274(i)) {
                return;
            }
            if (fn_80234CF8(i)) {
                return;
            }
            if (fn_80234C5C(i)) {
                return;
            }
            if (fn_80235744(i)) {
                return;
            }
            if (fn_802353AC(i)) {
                return;
            }
            if (fn_80233EDC(i)) {
                return;
            }
            if (fn_80234E30(i)) {
                return;
            }
        }
    }
    if (lbl_8053AAF8->field_0x7e && field_0xcfe == 1) {
        return;
    }
    if (fn_8022AF5C()) {
        return;
    }
    if (field_0x121e == 1) {
        return;
    }
    JUTGamePad* pad = JUTGamePad::getGamePad(fn_8030AAF4(field_0x11ec));
    if (field_0xd60 < 0 && groupSize(this) >= 2 &&
        lbl_8053AD30->fn_80309D6C(fn_8030AAF4(field_0x11ec), 0x40)) {
        if (fn_802486A0() == 5) {
            if (!lbl_8053AAF8->field_0x7e) {
                lbl_8053AAF8->fn_80134FE0(1);
            }
            field_0x1230 = fn_801F856C()->fn_801F9484(ACTOR_FORM, field_0x3b4, &mPos, field_0x4 & 3, -1, -1);
        } else {
            fn_80248004(5, true);
        }
        return;
    }
    if (!viewLayerFlag(field_0x3b4) && fn_80246B10(0x40)) {
        if (fn_80246D1C(8)) {
            if (!fn_80246B10(1) && !fn_80246B10(2)) {
                fn_80248004(2, true);
                return;
            }
        }
        if (fn_80246D1C(1)) {
            if (!fn_80246B10(8) && !fn_80246B10(4)) {
                fn_80248004(3, true);
                return;
            }
        }
        if (fn_80246D1C(4)) {
            if (!fn_80246B10(1) && !fn_80246B10(2)) {
                fn_80248004(1, true);
                return;
            }
        }
        if (fn_80246D1C(2)) {
            if (!fn_80246B10(8) && !fn_80246B10(4)) {
                fn_80248004(0, true);
                return;
            }
        }
    }
    bool busy = false;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022E438(field_0x4, i) && fn_8022DEAC(i)) {
            busy = true;
            break;
        }
    }
    if (lbl_8053AD30->fn_8030A308(fn_8030AAF4(field_0x11ec))) {
        return;
    }
    if (pad == NULL) {
        return;
    }
    if (busy) {
        return;
    }
    if (!(pad->getSubStickValue() > 0.5f)) {
        return;
    }
    s32 angle = pad->getSubStickAngle();
    if (angle < -0x7000 || angle >= 0x7000) {
        fn_80248004(2, true);
    } else if (angle >= -0x5000 && angle < -0x3000) {
        fn_80248004(3, true);
    } else if (angle >= -0x1000 && angle < 0x1000) {
        fn_80248004(1, true);
    } else if (angle >= 0x3000 && angle < 0x5000) {
        fn_80248004(0, true);
    }
}

bool Player_804A6F38::fn_8024B24C() {
    return field_0xbf0->isField98Or99Set();
}

void Player_804A6F38::fn_8024B274() {
    field_0xbf0->fn_80269420();
}

void Player_804A6F38::fn_8024B298(s32 index, JGeometry::TVec3f* out) {
    field_0xbf0->fn_8026942C(index, out);
}

void Player_804A6F38::fn_8024B2BC(JGeometry::TVec3f* out) {
    field_0xbf0->fn_8026947C(out);
}

void Player_804A6F38::fn_8024BAEC() {
    if (field_0x1216) {
        return;
    }
    if (field_0x454) {
        fn_8024B2E0();
        return;
    }
    for (s32 i = 0; i < 0x31; i++) {
        field_0x38[i].x = mPos.x;
        field_0x38[i].y = mPos.y;
        field_0x38[i].z = mPos.z;
        if (mPos.z > 0.0f) {
            field_0x284[i] = true;
        } else {
            field_0x284[i] = false;
        }
    }
}

void Player_804A6F38::fn_8024BB88() {
    field_0xced = 1;
}

s32 Player_804A6F38::fn_8024BB94() {
    if (field_0xce8) {
        return field_0xcf4;
    }
    return 0;
}

bool Player_804A6F38::fn_8024BBB0() {
    return field_0xce8;
}

bool Player_804A6F38::fn_8024C35C() {
    return field_0x3b4 >= 8;
}

bool Player_804A6F38::fn_8024C448() {
    return field_0xd40 >= 0;
}

bool Player_804A6F38::fn_8024C458() {
    return field_0xd4c;
}

bool Player_804A6F38::fn_8024C460(s32 arg) {
    if (!field_0xd4c) {
        return true;
    }
    if (field_0xd44 != arg) {
        return true;
    }
    field_0xd44 = -1;
    field_0xd3c = -1;
    field_0xd4c = false;
    field_0xd4d = 0;
    if (field_0xd74 < 3) {
        field_0xd74 = 3;
    }
    field_0xd50 = 0x14;
    return true;
}

bool Player_804A6F38::fn_8024C4C4(s32 serial) {
    if (field_0xd4c == true) {
        return false;
    }
    field_0xd4c = true;
    field_0xd44 = serial;
    field_0xd4d = 1;
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
    if (!actor || actor->mPos.x < mPos.x) {
        fn_801F856C()->fn_801F9484(ACTOR_PEFF, field_0x3b4, &mPos, (field_0x4 & 3) | 0x18, -1, -1);
    } else {
        fn_801F856C()->fn_801F9484(ACTOR_PEFF, field_0x3b4, &mPos, (field_0x4 & 3) | 0x1c, -1, -1);
    }
    return true;
}

bool Player_804A6F38::fn_8024CA0C(s32 arg) {
    return field_0xd44 == arg;
}

void Player_804A6F38::fn_8024CA20() {
    if (field_0xd50 > 0) {
        field_0xd50--;
    }
    if (!field_0xd4d) {
        if (field_0xd3c < 0) {
            field_0xd44 = -1;
            field_0xd4c = false;
        } else if (field_0xd44 < 0) {
            if (!field_0xd4e || fn_8022C1D8(field_0x4)) {
                field_0xd44 = field_0xd3c;
            }
        }
    }
    field_0xd40 = field_0xd3c;
    field_0xd3c = -1;
}

bool Player_804A6F38::fn_8024D98C() {
    field_0x455 = 1;
    return true;
}

JGeometry::TVec3f* Player_804A6F38::fn_8024E018() {
    return &field_0x2b8;
}

void Player_804A6F38::fn_8024E020(const JGeometry::TVec3f* arg) {
    field_0x2b8.set(arg->x, arg->y, arg->z);
}

bool Player_804A6F38::fn_8024E03C() {
    if (field_0x48c > 0) {
        field_0x48c = 1;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8024E060() {
    if (field_0x454) {
        return false;
    }
    if (field_0xce8 == 1) {
        return false;
    }
    return field_0x1216 != 1;
}

// FIXME: 99.86. Code identical; the float literal lbl_80540078 (20.0f) is not yet in the original pool order (@NNN vs lbl_).
void Player_804A6F38::fn_8024E344() {
    if (field_0x3b4 < 8) {
        fn_8024E694((s32)(20.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 0x32, 0);
    } else {
        fn_8024E694(0x32, 0);
    }
}

// FIXME: 99.86. Code identical; the float literal lbl_8054007C (10.0f) is not yet in the original pool order (@NNN vs lbl_).
void Player_804A6F38::fn_8024E3D8() {
    if (field_0x3b4 < 8) {
        fn_8024E694((s32)(10.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 0x14, 0);
    } else {
        fn_8024E694(0x14, 0);
    }
}

// FIXME: 99.86. Code identical; the float literal lbl_80540080 (5.0f) is not yet in the original pool order (@NNN vs lbl_).
void Player_804A6F38::fn_8024E46C() {
    if (field_0x3b4 < 8) {
        fn_8024E694((s32)(5.0f * lbl_8053AAF8->mRandom.get_ufloat_1()) + 5, 0);
    } else {
        fn_8024E694(5, 0);
    }
}

s8 Player_804A6F38::fn_8024EE0C() {
    return field_0x3d9;
}

s8 Player_804A6F38::fn_8024EE14() {
    return field_0x3d8;
}

u16 Player_804A6F38::fn_8024EE1C() {
    return field_0x3c4;
}

bool Player_804A6F38::fn_8024EFA0(bool arg) {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80230E1C(team, arg);
    }
    if (field_0x11dc == 0) {
        field_0x11dc = 0x70;
        field_0x11e0 = arg;
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8024F54C(s32 arg) {
    if (field_0x3b4 >= 8 && fn_8030381C(fn_80240628()) && arg == 0x1000002) {
        fn_803037D4(fn_80240628(), 0x10);
    } else {
        lbl_8053AF80->fn_80458F04(arg);
    }
}

bool Player_804A6F38::fn_8024F5E4() {
    if (!fn_8025F448() && field_0x1240 == 0x19) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8024F630() {
    return field_0xbf0->field_0xb4;
}

bool Player_804A6F38::fn_8024FE78() {
    if (fn_8024F63C()) {
        return fn_80243C64();
    }
    return false;
}

bool Player_804A6F38::fn_8024FEBC() {
    if (field_0x1240 == 0x1a && fn_80243C64()) {
        return true;
    }
    return false;
}

// fn_8024FEFC (0x8024FEFC, `fn_8024FEBC() || fn_8024FE78()` as a bool) is not
// written: the original calls both out of line, while MWCC inlines them here.

s32 Player_804A6F38::fn_8024FF54() {
    s32 value = field_0xdac;
    if (value < 0) {
        return -1;
    }
    return value;
}

s32 Player_804A6F38::fn_8024FF6C() {
    s32 value = field_0xda4;
    if (value < 0) {
        return -1;
    }
    return value;
}

s32 Player_804A6F38::fn_8024FF84() {
    s32 value = field_0xd94;
    if (value < 0) {
        return -1;
    }
    return value;
}

void Player_804A6F38::fn_8024FF9C(s32 arg) {
    if (field_0xd94 == arg) {
        field_0xd94 = -1;
    }
}

void Player_804A6F38::fn_8024FFB4(s32 arg) {
    if (field_0xd94 < 0) {
        field_0xd94 = arg;
    }
}

bool Player_804A6F38::fn_8024FFC8(s32 arg) {
    if (fn_8024FEFC() && arg == fn_8024FF6C()) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_80250030(s32 arg) {
    if (fn_80250D30() && fn_802729F0(arg)) {
        return true;
    }
    return false;
}

u8 Player_804A6F38::fn_802517E4() {
    return field_0x1240 == 0x1a;
}

bool Player_804A6F38::fn_802517F8(bool a) {
    cls_804A71A8* sub = field_0xbf0;
    if (sub->field_0x48[1] == 1) {
        return true;
    }
    if (sub->field_0x48[0] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[4] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[5] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[6] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[7] == 1) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802518AC(bool a) {
    cls_804A71A8* sub = field_0xbf0;
    if (sub->field_0x48[2] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[4] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[5] == 1) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8025190C(bool a) {
    cls_804A71A8* sub = field_0xbf0;
    if (sub->field_0x48[3] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[6] == 1) {
        return true;
    }
    if (a == true && sub->field_0x48[7] == 1) {
        return true;
    }
    return false;
}

// Requests state `state` (field_0x1244) when fn_80251BFC allows it. A player
// not taking part (bit field_0x4 of lbl_8053AAF8->field_0x82 clear, see
// game/PlayerBit.h) with no partner asking for state 0 gets
// state 1 instead; with field_0x434 set, state 0 becomes 2.
bool Player_804A6F38::fn_80251AEC(s32 state) {
    if (!fn_80251BFC(state)) {
        return false;
    }
    if (!isPlayerBitSet(lbl_8053AAF8, field_0x4) && field_0xd60 < 0 && state == 0) {
        field_0x1244 = 1;
    } else if (field_0x434 && state == 0) {
        if (field_0x1240 == 2) {
            return false;
        }
        field_0x1244 = 2;
    } else {
        field_0x1244 = state;
    }
    field_0x124c = true;
    if (state != 0 && state != 2 && state != 3) {
        field_0x124d = true;
    }
    if (field_0xd60 < 0 && field_0xcfe == true && field_0x1244 != 0) {
        field_0xcfe = false;
    }
    return true;
}

// Whether a request for `state` may be made now.
bool Player_804A6F38::fn_80251BFC(s32 state) {
    if (field_0x124d == true) {
        return false;
    }
    if (field_0x124c == true && field_0x1244 != 0 && !(field_0x1244 == 2 || field_0x1244 == 3)) {
        return false;
    }
    if (field_0x124c == true && field_0x1244 == 2 && (state == 0 || state == 3)) {
        return false;
    }
    s32 requested = field_0x1244;
    if (requested == 1 && state != 0) {
        return false;
    }
    if (field_0xcee == true) {
        return false;
    }
    if (field_0xd60 >= 0 && !field_0xce8 && field_0x1214 && state != 3) {
        return false;
    }
    if (field_0x1215 && state != 3) {
        return false;
    }
    if (field_0x1240 == state) {
        return false;
    }
    if (requested == state) {
        return false;
    }
    if (lbl_8053AAF8->field_0x7e && fn_80236890()) {
        return false;
    }
    return true;
}

void Player_804A6F38::fn_80252BAC(const JGeometry::TVec3f* offset) {
    field_0x2e8 += *offset;
}

void Player_804A6F38::fn_80252F14(const JGeometry::TVec3f* offset) {
    field_0x2d0 += *offset;
    field_0x434 = 1;
}

void Player_804A6F38::fn_80252F40(s32 arg) {
    field_0xd28 = arg;
}

void Player_804A6F38::fn_80252F50(s32 arg) {
    field_0xbec->field_0x29c = arg;
}

void Player_804A6F38::fn_80252F5C(s32 arg) {
    field_0xbec->field_0x298 = arg;
}

s32 Player_804A6F38::fn_80252F68() {
    return field_0xbec->field_0x294;
}

s32 Player_804A6F38::fn_80252F74() {
    return field_0xbec->field_0x290;
}

void Player_804A6F38::fn_80252F80(const JGeometry::TVec3f* pos, s32 a, s32 b, u8 c) {
    field_0x2c4 = *pos;
    field_0x3e0 = a;
    field_0xd00 = b;
    if (b == 1) {
        field_0xcfe = true;
    } else {
        field_0xcfe = false;
    }
    field_0xd04 = c;
}

void Player_804A6F38::fn_80252FBC() {}

// The per-player cue of the update's join sequence: at frame `at` of the
// counter field_0xd38 the 'here' effect, pad rumble and sound 0x25; at
// `off` fn_8043A170. Written once per player in the original (the four
// copies differ only in the frames), so a file-local inline.
static inline void joinCue(Player_804A6F38* player, s32 at, s32 off) {
    if (player->field_0xd38 == at) {
        lbl_8053AB10->field_0x30->fn_8015F8FC(player->field_0x3b4, &player->mPos, 1);
        if (lbl_8053AD30->fn_8030A308(fn_8030AAF4(player->field_0x11ec))) {
            fn_8030381C(player->field_0x4);
        }
        if (player->field_0x3b4 >= 8) {
            fn_8030381C(player->field_0x4);
        }
        // The original converts the index to s8 (extsb) for the sound call.
        lbl_8053AF80->fn_80458880(0x25, (s8)player->field_0x4);
    } else if (player->field_0xd38 == off) {
        fn_8043A170(player->field_0x4, false);
    }
}

// FIXME: 99.99. Code identical; the float literal lbl_80540094 (18.0f) is @NNN until the
// unit's earlier literals are written in pool order.
// Update A: the player's per-frame update, called for every slot by the
// manager update fn_8023F390.
void Player_804A6F38::fn_80253E6C() {
    if (!mActive) {
        return;
    }
    if (field_0x1234 == 1 && lbl_8053AD30->fn_80309D90(fn_8030AAF4(field_0x11ec)) > 0.0f) {
        field_0x1234 = 0;
    }
    if (field_0x11f0 > 0) {
        field_0x11f0--;
    }
    if (field_0x13b8) {
        lbl_8053AB10->field_0x20->field_0x216 = 1;
    }
    if (field_0x12c4 != NULL) {
        if (lbl_8053AAF8->field_0x94 == 1 || field_0x121f || field_0x454) {
            field_0x12c4->setStatus(JPAEMIT_StopDraw);
            field_0x12c4->setStatus(JPAEMIT_StopCalc);
        } else {
            field_0x12c4->clearStatus(JPAEMIT_StopDraw);
            field_0x12c4->clearStatus(JPAEMIT_StopCalc);
        }
    }
    if (field_0x12c8 != NULL) {
        if (lbl_8053AAF8->field_0x94 == 1 || field_0x121f || field_0x454) {
            field_0x12c8->setStatus(JPAEMIT_StopDraw);
            field_0x12c8->setStatus(JPAEMIT_StopCalc);
        } else {
            field_0x12c8->clearStatus(JPAEMIT_StopDraw);
            field_0x12c8->clearStatus(JPAEMIT_StopCalc);
        }
    }
    if (lbl_8053AAF8->field_0x94 == 1 || field_0x121f) {
        fn_80270D08();
        fn_80286688();
        fn_80265E88();
        for (int i = 0; i < 0x50; i++) {
            field_0xdb0[i] = -1;
            field_0x1030[i] = -1;
        }
        return;
    }
    field_0x2f4.set(0.0f, 0.0f, 18.0f);
    field_0x2f4 += mPos;
    field_0xc0c = field_0xc08;
    cls_804A71A8* sub = field_0xbf0;
    sub->field_0x14 = 0.0f;
    sub->field_0x18 = 0.0f;
    if (!lbl_8053AAF8->field_0x80) {
        fn_802490A8();
    }
    fn_8026FD14();
    fn_80251D50();
    if (lbl_8053AAF8->field_0x80) {
        field_0xd38++;
        field_0x1208++;
        if (field_0xd38 == 0x10) {
            fn_8013CC50(field_0x3b4, &mPos, 0x418, 0, 1, NULL, NULL, true, 1.0f);
            fn_8013CC50(field_0x3b4, &mPos, 0x419, 0, 1, NULL, NULL, true, 1.0f);
            fn_8013CC50(field_0x3b4, &mPos, 0x41a, 0, 1, NULL, NULL, true, 1.0f);
            fn_8024F0E8(0x4004e);
        } else if (field_0xd38 == 0x83) {
            field_0xc80 = 1;
            fn_8024F0E8(0x20043);
        }
        if (isPlayerBitSet(lbl_8053AAF8, field_0x4)) {
            switch (field_0x4) {
            case 0:
                joinCue(this, 0xc0, 0xe0);
                break;
            case 1:
                joinCue(this, 0xe0, 0x100);
                break;
            case 2:
                joinCue(this, 0xf8, 0x118);
                break;
            case 3:
                joinCue(this, 0x108, 0x128);
                break;
            }
        }
    } else {
        fn_80285E58();
    }
    fn_8024BDBC();
    fn_80276538();
    fn_80276078();
    field_0xbec->fn_8030B774();
    fn_8024E0A0();
    if (field_0xd48 > 0) {
        field_0xd48--;
    }
}

// FIXME: 99.95. Code identical; lbl_80540098 (256/3) is @NNN, as in fn_80254660.
// Like fn_80254660, then clears the counters 0xC04-0xC7C.
void Player_804A6F38::fn_802544B0() {
    field_0xc10 = fn_802D6A9C()->field_0x48;
    field_0xc14 = fn_802D6A9C()->field_0x4c;
    field_0xc18 = 1;
    field_0xb7a = false;
    field_0xb7b = false;
    if (lbl_8053AAF8->field_0x7f) {
        if (field_0xd60 >= 0) {
            field_0xbf8 = 0x10;
        } else {
            field_0xbf8 = fn_8040305C(fn_804033D4(field_0x4)) * 4;
        }
    } else {
        field_0xbf8 = fn_8040305C(fn_804033D4(field_0x4)) * 4;
    }
    if (lbl_8053AAF8->field_0x7f) {
        field_0xb90 = 0.0f;
    } else {
        field_0xb90 = 256.0f / 3.0f;
    }
    field_0xc08 = 0x180;
    field_0xc0c = 0x180;
    field_0xbfc = field_0xbf8 - 4;
    if (lbl_8053AAF8->field_0x7f) {
        if (field_0xd60 >= 0) {
            field_0xc04 = 0;
        } else {
            field_0xc04 = 0;
        }
    } else {
        field_0xc04 = 0;
    }
    for (int i = 0; i < 13; i++) {
        field_0xc1c[i] = 0;
    }
    field_0xc50 = 0;
    field_0xc54 = 0;
    field_0xc58 = 0;
    field_0xc5c = 0;
    field_0xc60 = 0;
    field_0xc64 = 0;
    field_0xc68 = 0;
    field_0xc6c = 0;
    field_0xc70 = 0;
    field_0xc74 = 0;
    field_0xc78 = 0;
    field_0xc7c = 0;
}

// FIXME: 99.91. Code identical; the float literal lbl_80540098 (256/3) is @NNN until the
// unit's earlier literals are written in pool order.
void Player_804A6F38::fn_80254660() {
    field_0xc10 = fn_802D6A9C()->field_0x48;
    field_0xc14 = fn_802D6A9C()->field_0x4c;
    field_0xc18 = 1;
    field_0xb7a = false;
    field_0xb7b = false;
    if (lbl_8053AAF8->field_0x7f) {
        if (field_0xd60 >= 0) {
            field_0xbf8 = 0x10;
        } else {
            field_0xbf8 = fn_8040305C(fn_804033D4(field_0x4)) * 4;
        }
    } else {
        field_0xbf8 = fn_8040305C(fn_804033D4(field_0x4)) * 4;
    }
    if (lbl_8053AAF8->field_0x7f) {
        field_0xb90 = 0.0f;
    } else {
        field_0xb90 = 256.0f / 3.0f;
    }
    field_0xc08 = 0x180;
    field_0xc0c = 0x180;
    field_0xbfc = field_0xbf8 - 4;
}

// FIXME: 98.34. The original keeps the view's stage in a saved register and tests
// isStageFlagSet's result as an int (cmpwi), ours reloads it and tests the bool
// (clrlwi.). Besides the unit's float literals (@NNN vs lbl_, not yet in
// pool order), only scheduling and register numbers differ: at two of the
// seven isPlayerBitSet sites (the 2-player and the else branch) the original
// loads field_0x4 before lbl_8053AAF8 (the registers after them shift by one),
// and the first 0x31-entry loop's setup makes the index 0 first and copies it
// for the byte 0 (li r5,0 / mr r3,r5), ours loads li r6,0 / li r3,0 after
// li r4,1.
// The per-slot reset, run for every slot on scene enter and stage change
// (fn_8023F78C). Picks the slot's part (field_0xcac, the partner field_0xd60,
// the pad owner field_0x11ec and the start state 0 or 1) from the game mode,
// places the player and clears nearly every per-player field.
void Player_804A6F38::fn_80254744() {
    if (field_0xd00 >= 2 && isStageFlagSet(fn_802D6A9C()->field_0x4, fn_802D6A9C()->field_0x8)) {
        field_0xd00 = 0;
        field_0xcfe = false;
    }
    if (!lbl_8053AAF8->field_0x7f && !isPlayerBitSet(lbl_8053AAF8, field_0x4)) {
        mActive = false;
        field_0xd60 = -1;
        field_0x11ec = -1;
        field_0x1244 = 1;
        field_0x1240 = 1;
        field_0x1248 = 1;
    } else if (lbl_8053AAF8->field_0x7f && fn_802DE814() &&
               !isPlayerBitSet(lbl_8053AAF8, field_0x4)) {
        mActive = false;
        field_0xd60 = -1;
        field_0x11ec = -1;
        field_0x1244 = 1;
        field_0x1240 = 1;
        field_0x1248 = 1;
    } else if (fn_802DEBF8() || !lbl_8053AAF8->field_0x7e) {
        mActive = true;
        field_0x11ec = 0;
        if (field_0x4 == 0) {
            field_0xd60 = -1;
        } else {
            field_0xd60 = 0;
        }
        field_0x1244 = 0;
        field_0x1240 = 0;
        field_0x1248 = 0;
    } else if (lbl_8053AAF8->fn_80138A30() == 2) {
        mActive = true;
        if (isPlayerBitSet(lbl_8053AAF8, field_0x4)) {
            field_0xd60 = -1;
            field_0x11ec = field_0x4;
        } else {
            field_0xd60 = field_0x4 - 2;
            field_0x11ec = field_0xd60;
        }
        field_0x1244 = 0;
        field_0x1240 = 0;
        field_0x1248 = 0;
    } else if (lbl_8053AAF8->fn_80138A30() == 3) {
        mActive = true;
        if (isPlayerBitSet(lbl_8053AAF8, field_0x4)) {
            field_0xd60 = -1;
            field_0x11ec = field_0x4;
        } else {
            s32 partner = lbl_80529DEC.field_0x64;
            if (partner < 0 || partner >= 3) {
                field_0xd60 = 0;
            } else {
                field_0xd60 = partner;
            }
            field_0x11ec = field_0xd60;
        }
        field_0x1244 = 0;
        field_0x1240 = 0;
        field_0x1248 = 0;
    } else {
        mActive = true;
        field_0xd60 = -1;
        if (isPlayerBitSet(lbl_8053AAF8, field_0x4)) {
            field_0x11ec = field_0x4;
            field_0x1244 = 0;
            field_0x1240 = 0;
            field_0x1248 = 0;
        } else {
            field_0x11ec = -1;
            field_0x1244 = 1;
            field_0x1240 = 1;
            field_0x1248 = 1;
        }
    }

    // A partner starts next to the player it follows, 16 units per place in the line.
    if (field_0xd60 >= 0 && !field_0xcfe) {
        field_0x2c4 = *fn_8023E724(field_0xd60);
        if (field_0xd04) {
            field_0x2c4.y += 16.0f * fn_80248778();
        } else {
            field_0x2c4.x += 16.0f * fn_80248778();
        }
    }
    field_0x11ea = 1;
    mPos = field_0x2c4;
    field_0x2f4 = mPos;
    field_0xcbc = field_0x2f4;
    field_0xcb0 = field_0xcbc;
    field_0xcd4 = field_0xcb0;
    field_0xcc8 = field_0xcd4;
    field_0x32c = field_0xcc8;
    field_0xd1c = field_0x32c;
    if (field_0xd00 == 2) {
        mPos.z = 1.0f;
        field_0x32c.z = 1.0f;
    }
    field_0x318 = mPos;
    field_0x14 = mPos;
    field_0x20 = field_0x14;
    field_0x2c = field_0x20;
    field_0x3c0 = false;
    field_0x3c1 = false;
    field_0x3c2 = false;
    if (field_0xcfe == true) {
        field_0x120c = 4;
        field_0x1210 = 4;
    } else {
        field_0x120c = 5;
        field_0x1210 = 5;
    }
    field_0x121c = 0;
    field_0x121d = 0;
    field_0x121f = false;
    field_0x1220 = 0;
    field_0x1258 = mPos;
    field_0x434 = 0;
    field_0x435 = 0;
    field_0x2d0.zero();
    field_0x2dc.zero();
    field_0x2e8.zero();
    field_0x3b4 = 0;
    field_0x3b8 = 0;
    field_0xce4 = 0;
    field_0xce0 = 0;
    field_0x3bc = field_0x3b4;
    field_0xd28 = field_0x3b4;
    field_0x455 = 0;
    field_0x3f0 = field_0x3e4 = field_0x3dc = field_0x3e8 = field_0x3ec = field_0x3e0;
    field_0x124d = 0;
    field_0x124c = false;
    field_0x458 = 0;
    field_0x459 = 0;
    field_0x3f4 = 0;
    field_0x3f9 = 0;
    field_0x3f8 = 0;
    field_0x3fb = 0;
    field_0x3fa = 0;
    field_0x3fd = 0;
    field_0x3fc = 0;
    field_0x3fe = 0;
    field_0x3b0 = 0;
    field_0x437 = field_0x3f8;
    field_0x438 = field_0x3f9;
    field_0x439 = field_0x3fa;
    field_0x43a = field_0x3fb;
    field_0x43b = field_0x3fc;
    field_0x43c = field_0x3fd;
    field_0x3ff = 0;
    field_0x400 = 0;
    field_0x401 = 0;
    field_0x402 = 0;
    field_0x403 = 0;
    field_0x404 = 0;
    field_0x40c = 0;
    field_0x408 = 0;
    field_0x410 = 0.0f;
    if (field_0x1240 == 1) {
        field_0x3c4 = 0x31d;
    } else {
        switch (field_0x3dc) {
        case 0:
            field_0x3c4 = 0x3c;
            break;
        case 0x1c1:
            field_0x3c4 = 0x3d;
            break;
        case 1: // compared in the original's search (cmpwi 1), sharing default's code
        default:
            field_0x3c4 = 0x3e;
            break;
        case 3:
            field_0x3c4 = 0x3f;
            break;
        }
    }
    field_0x3c6 = field_0x3c4;
    field_0x458 = 0;
    field_0x45c = 0;
    field_0x43d = 0;
    field_0x3d8 = 0;
    field_0x3d9 = 0;
    field_0xca3 = -0x80;
    field_0xca4 = -0x80;
    fn_80276538();
    fn_80276078();
    field_0xbf4 = 0;
    field_0x464 = 0;
    field_0x468 = 0;
    field_0x46c = 0;
    field_0x470 = 0;
    field_0x474.set(0.0f, 0.0f, 0.0f);
    field_0x484 = 0;
    field_0x480 = 0;
    field_0x488 = 0;
    field_0x48c = 0;
    field_0x490 = 0;
    field_0x491 = 0;
    field_0x457 = 0;
    field_0xcad = false;
    field_0xcae = 0;
    field_0xcaf = 0;
    field_0xd64 = 0;
    field_0xd6c = 'NULL';
    field_0xbf0->fn_8026F710();
    field_0xce8 = false;
    field_0xce9 = 1;
    field_0xcf4 = 0x29;
    field_0xcf8 = 0x29;
    field_0xcea = 0;
    field_0xceb = 0;
    field_0xcec = 0;
    field_0xcf2 = 0;
    field_0xced = 0;
    field_0xcf1 = 0;
    field_0xcf0 = 0;
    field_0x344.set(0.0f, 0.0f, 0.0f);
    field_0x350.set(0.0f, 0.0f, 0.0f);
    field_0x35c.set(0.0f, 0.0f, 0.0f);
    field_0xd78.set(0.0f, 0.0f, 0.0f);
    field_0x414.set(0.0f, 0.0f, 0.0f);
    field_0x420.set(0.0f, 0.0f, 0.0f);
    field_0x368.set(0.0f, 0.0f, 0.0f);
    field_0x38c = 0.0f;
    field_0x390 = 0.0f;
    field_0x374.set(0.0f, 0.0f, 0.0f);
    field_0x2b8.set(0.0f, 0.0f, 0.0f);
    if (lbl_8053AAF8->field_0x7f) {
        field_0xc80 = 1;
    } else {
        field_0xc80 = 0;
    }
    field_0xc84.set(1.0f, 1.0f, 1.0f);
    field_0xc90.set(1.0f, 1.0f, 1.0f);
    field_0xc9c = 0;
    fn_8027221C(0);
    field_0xcfc = 0;
    field_0xcfd = 0;
    field_0xd08 = -1;
    field_0xd0c = 0;
    field_0xd2c = 0;
    field_0xd2d = 0;
    field_0xd30 = 0;
    field_0x494 = 0;
    field_0x498 = 0;
    field_0x499 = 0;
    field_0x49c = -1;
    field_0xd34 = false;
    field_0xc00 = 0;
    field_0xd35 = 0;
    field_0xd70 = -1;
    field_0xd68 = -1;
    field_0xd88 = -1;
    field_0xd8c = -1;
    field_0xd94 = -1;
    field_0xda0 = -1;
    field_0xd98 = 0;
    field_0xd9c = -1;
    field_0xda4 = -1;
    field_0xda8 = 0;
    field_0xdac = -1;
    for (int i = 0; i < 0x50; i++) {
        field_0xdb0[i] = -1;
        field_0xef0[i] = -1;
        field_0x1030[i] = -1;
    }
    field_0x1170 = 0;
    for (int i = 0; i < 3; i++) {
        field_0xb00[i] = -1;
        field_0xb40[i] = -1;
    }
    field_0xb7c = -1;
    field_0xb74 = 0;
    for (int i = 0; i < 3; i++) {
        field_0xb0c[i] = -1;
        field_0xb4c[i] = -1;
    }
    field_0xb80 = -1;
    for (int i = 0; i < 2; i++) {
        field_0xb30[i] = -1;
    }
    field_0xb18 = -1;
    for (int i = 0; i < 3; i++) {
        field_0xb1c[i] = -1;
        field_0xb5c[i] = -1;
    }
    field_0xb78 = false;
    field_0xb28 = -1;
    field_0xb2c = -1;
    field_0xbb8 = 0;
    field_0xb38 = -1;
    field_0xb6c = -1;
    field_0xb68 = 0;
    field_0xb3c = -1;
    field_0xb70 = -1;
    field_0xb58 = -1;
    field_0xb99 = false;
    field_0xb9e = false;
    field_0xb9f = false;
    field_0xba0 = 0;
    field_0xba4 = 0;
    field_0xb9a = false;
    field_0xb9b = false;
    field_0xb9c = false;
    field_0xbb4 = -1;
    field_0xbb9 = 0;
    field_0xbba = 0;
    field_0xbbc = 3.0f;
    field_0xbd4 = 0;
    field_0xbc0 = 0;
    field_0xbac = 0;
    field_0xb79 = 0;
    field_0x1204 = 0;
    field_0xca0 = 0xff;
    field_0xca1 = 0xff;
    field_0xca2 = 0xff;
    field_0xca8 = 0;
    field_0x1264 = 0;
    field_0x11dc = 0;
    field_0x1274 = NULL;
    field_0x128c = NULL;
    field_0x12b0 = NULL;
    field_0x12b4 = NULL;
    field_0x12b8 = NULL;
    field_0x12bc = NULL;
    field_0x12c0 = -1;
    field_0x12c4 = NULL;
    field_0x12c8 = NULL;
    field_0x12cc = NULL;
    field_0x12e4 = NULL;
    field_0x1308 = NULL;
    field_0x132c = NULL;
    field_0x1350 = NULL;
    field_0x1354 = NULL;
    field_0x1358 = NULL;
    field_0x136c = NULL;
    field_0x1370 = NULL;
    field_0x135c = NULL;
    field_0x1374 = NULL;
    field_0x1378 = NULL;
    field_0x1368 = NULL;
    field_0x1360 = NULL;
    field_0x1364 = NULL;
    field_0x137c = NULL;
    field_0x1380 = NULL;
    field_0x1384 = NULL;
    field_0x1388 = NULL;
    field_0x138c = NULL;
    field_0x1390 = NULL;
    field_0x1394 = NULL;
    field_0x1398.field_0x4 = field_0x4;
    field_0x1398.field_0x8 = 0;
    field_0x13a8.field_0x4 = field_0x4;
    field_0x13a8.field_0x8 = 1;
    field_0xd74 = 0;
    field_0x11e0 = 0;
    field_0x11b0 = 0;
    fn_8027023C();
    field_0x11e9 = 0;
    field_0x1174 = 0;
    field_0x454 = false;
    field_0x456 = 0;
    field_0x452 = 0;
    field_0xd38 = 0;
    field_0xd3c = -1;
    field_0xd44 = -1;
    field_0xd4c = false;
    field_0xd4d = 0;
    field_0xd4e = false;
    field_0xd50 = 0;
    field_0x11f4 = 0;
    field_0xd54 = 0;
    field_0xd58 = 0;
    field_0x11f8 = -1;
    field_0x1230 = -1;
    field_0x1234 = 0;
    field_0xafc = 0;
    field_0xb9d = false;
    field_0xd5c = 0;
    field_0x11fc = -1;
    field_0x460 = false;
    field_0xb84 = false;
    field_0x11b4 = mPos;
    field_0x11c0 = 0;
    field_0xcee = 0;
    field_0xcef = 0;
    field_0x42c = false;
    field_0xbe0 = 0;
    field_0x430 = 0;
    field_0x42d = 0;
    field_0x43e = 0;
    field_0x43f = false;
    field_0x440 = false;
    field_0x441 = false;
    field_0x444 = 0.0f;
    field_0xbd8 = 0;
    field_0xbdc = 0;
    field_0xbe4 = 0;
    field_0xbe8 = 0;
    field_0x1214 = false;
    field_0x1216 = false;
    field_0x1215 = false;
    field_0x1217 = false;
    field_0x1218 = 0;
    field_0x1224 = 0;
    field_0x123c = 0;
    field_0x1228 = 0;
    field_0x122c = 0;
    field_0x1238 = 0;
    field_0x11ac = 0;
    field_0x492 = 0;
    field_0x1200 = false;
    field_0x1201 = 0;
    field_0x1202 = false;
    field_0x11c4 = 0;
    field_0x11c8 = 0;
    field_0x11d4 = 0;
    field_0x11d8 = 0;
    field_0x11cc = 0;
    field_0x3c8 = 0x195;
    field_0x3ca = 0x18f;
    field_0x3cc = 0;
    field_0x3d0 = 0;
    field_0x3d4 = 0;
    field_0xd90 = 0;
    field_0x394 = 0;
    field_0x1208 = 0;
    field_0xd48 = 0;
    field_0xba8 = 0;
    field_0x436 = false;
    field_0x1203 = 0;
    field_0xb94 = 0;
    field_0x448 = 3;
    field_0x44c = 0;
    field_0x13b8 = 0;
    field_0xd84 = 0;
    field_0x11f0 = 0;
    field_0x11e8 = 0;
    if (fn_802DEBF8()) {
        field_0x121e = 1;
        field_0x1214 = true;
        if (field_0xd60 >= 0) {
            field_0x1215 = true;
            mPos.set(*fn_8023E724(field_0xd60));
            fn_80251AEC(3);
        }
        field_0x1224 = 0;
        field_0x1217 = false;
    } else {
        field_0x121e = 0;
    }
    if (!field_0x1216) {
        if (field_0x454) {
            fn_8024B2E0();
        } else {
            for (int i = 0; i < 0x31; i++) {
                field_0x38[i].set(mPos);
                if (mPos.z > 0.0f) {
                    field_0x284[i] = 1;
                } else {
                    field_0x284[i] = 0;
                }
            }
        }
    }
    for (int i = 0; i < 0x31; i++) {
        // An unused copy: the original stores mPos on the stack each pass (without
        // it we are 4 instructions short).
        JGeometry::TVec3f pos = mPos;
        if (field_0xd04) {
            field_0x38[i].set(mPos.x, mPos.y + 16.0f * i / 12.0f, 0.0f);
        } else {
            field_0x38[i].set(mPos.x + 16.0f * i / 12.0f, mPos.y, 0.0f);
        }
        field_0x284[i] = 0;
    }
    if (lbl_8053AAF8->field_0x7f && fn_80241794() <= 0 && !fn_80241200()) {
        if (field_0x3dc == 0) {
            field_0x338.set(16.0f + mPos.x, mPos.y, mPos.z);
        } else if (field_0x3dc == 1) {
            field_0x338.set(mPos.x - 16.0f, mPos.y, mPos.z);
        } else if (field_0x3dc == 2) {
            field_0x338.set(mPos.x, 16.0f + mPos.y, mPos.z);
        } else {
            field_0x338.set(mPos.x, mPos.y - 16.0f, mPos.z);
        }
    } else {
        field_0x338.x = mPos.x;
        field_0x338.y = mPos.y;
        field_0x338.z = mPos.z;
    }
    field_0xbec->fn_8030BD3C(field_0x4);
    if (field_0xcfe == true) {
        if (!isStageFlagSet(fn_802D6A9C()->field_0x4, fn_802D6A9C()->field_0x8)) {
            fn_8024DCD0();
            field_0x32c = mPos;
            field_0x450 = 0xc3;
            field_0xca2 = 0xff;
            field_0xca3 = -0x80;
            field_0xca4 = -0x80;
            // Both operands load mPos (0x8) in the original (mPos - field_0x32c reloads 0x32c: 98.0 %).
            const JGeometry::TVec3f& diff = mPos - mPos;
            if (PSVECMag(&diff) >= 0.1f) {
                fn_8024B2E0();
            }
            lbl_8053AD70->fn_8037AE00();
        }
    } else if (field_0xd00 == 2) {
        fn_8024DCD0();
        field_0x32c = mPos;
        field_0x450 = 0x121;
        field_0xca2 = 0xff;
        field_0xca3 = -0x80;
        field_0xca4 = -0x80;
        // As above: both operands load mPos (mPos - field_0x32c reloads 0x32c).
        const JGeometry::TVec3f& diff = mPos - mPos;
        if (PSVECMag(&diff) >= 0.1f) {
            fn_8024B2E0();
        }
        field_0xcfe = false;
        lbl_8053AD70->fn_8037AE00();
    }
}

Player_804A6F38::~Player_804A6F38() {
    fn_80241E64();
    delete field_0xbf0;
    delete field_0xbec;
    delete field_0x1254;
    delete field_0x1250;
}

Player_804A6F38::Player_804A6F38(s32 index) {
    field_0x4 = index;
    field_0x1258.set(0.0f, 0.0f, 0.0f);
    field_0x1250 = new cls_804B20F8(index, &field_0x1258, NULL, 8);
    field_0x1254 = new cls_804B20F8(index, NULL, NULL, 8);
    field_0xbec = new cls_804AA8D8();
    field_0xbf0 = new cls_804A71A8(this);
    mActive = false;
}
