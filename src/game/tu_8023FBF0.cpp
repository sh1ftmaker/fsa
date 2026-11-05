#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/PlayerFlags.h"

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

s32 Player_804A6F38::fn_8023FC24() {
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
    return field_0xc8c < 1.0f;
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

// FIXME: 86.8 %. The clamp to zero of the word at 0xC04 is srwi / subi / and
// in the original; the ternary compiles to srawi / andc here.
s32 Player_804A6F38::fn_80241920() {
    s32 team = field_0xd60;
    if (team >= 0) {
        return fn_80236228(team);
    }
    s32 value = field_0xc04;
    return value < 0 ? 0 : value;
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

// The team checks fn_80246F28 makes before it reads the input: the player
// flags, then, when more than one player is on this team and the team mode
// (fn_8022CAF8 / field_0x120c) is 4, the team count must match, and
// field_0x11ea must be set.
static inline bool teamCheck(Player_804A6F38* player) {
    if (!player->fn_802413F0()) {
        return false;
    }
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
    s32 mode;
    if (count <= 1) {
        mode = 5;
    } else if (player->field_0xd60 >= 0) {
        mode = fn_8022CAF8(player->field_0xd60);
    } else {
        mode = player->field_0x120c;
    }
    if (mode == 4) {
        s32 a;
        if (player->field_0xd60 >= 0) {
            a = fn_80234004(player->field_0xd60);
        } else {
            a = player->field_0x1224;
        }
        s32 b;
        if (player->field_0xd60 < 0) {
            b = 0;
        } else {
            s32 n = 0;
            for (s32 j = 0; j < player->field_0x4; j++) {
                s32 team = player->field_0xd60;
                if (team == fn_8022F3DC(j)) {
                    n++;
                }
            }
            b = n + 1;
        }
        if (a != b) {
            return false;
        }
    }
    if (!player->field_0x11ea) {
        return false;
    }
    return true;
}

// FIXME: 94.8 %. The fn_802413F0 chain matches up to its last byte: there
// the original tests 0xCE9 with cmplwi / bne and li 0 / li 1, ours converts
// it with neg / or / srwi (as fn_802413F0's own copy does in both). Also the
// original branches on a != b (bne) to the false result it shares with the
// field_0x11ea test, ours on a == b with a false block of its own, and the
// loop locals get r27-r29 in another order (field_0x4 in r30 there).
f32 Player_804A6F38::fn_80246F28() {
    // `== false`: the check's bool is materialised and tested (li 0 / li 1,
    // clrlwi.; bne to the call), as with `x == true` (docs/RULES_CARD.md).
    if (teamCheck(this) == false) {
        return 0.0f;
    }
    return lbl_8053AD30->fn_80309D90(fn_8030AAF4(field_0x11ec));
}

bool Player_804A6F38::fn_80247534() {
    return lbl_8053AD30->fn_8030A308(fn_8030AAF4(field_0x11ec));
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
    if (field_0xc8c < 1.0f) {
        field_0xc8c += 0.05f;
        if (field_0xc8c >= 1.0f) {
            field_0xc8c = 1.0f;
        }
    }
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

bool Player_804A6F38::fn_8024CA0C(s32 arg) {
    return field_0xd44 == arg;
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

bool Player_804A6F38::fn_8024F5E4() {
    if (!fn_8025F448() && field_0x1240 == 0x19) {
        return true;
    }
    return false;
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

// FIXME: 88.0 %. The result is converted with extrwi r3, r0, 8, 19 in the
// original (a u8 return) and srwi r3, r0, 5 here; fn_8022D894 passes the byte
// on without converting it, so a u8 return would break that wrapper.
bool Player_804A6F38::fn_802517E4() {
    return field_0x1240 == 0x1a;
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
