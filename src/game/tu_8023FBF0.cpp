#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/PlayerFlags.h"

bool Player_804A6F38::fn_80240860() {
    return field_0x436;
}

s32 Player_804A6F38::fn_80240868() {
    return field_0x4;
}

bool Player_804A6F38::fn_80240870() {
    return field_0xcfe;
}

bool Player_804A6F38::fn_80240AF0() {
    return field_0x1215;
}

bool Player_804A6F38::fn_80240AF8() {
    return field_0x1214;
}

bool Player_804A6F38::fn_802408C0() {
    return field_0x1217;
}

bool Player_804A6F38::fn_802413F0() {
    return playerFlags_802413F0(this);
}

void Player_804A6F38::fn_80241454(u8 value) {
    field_0xc80 = value;
}

bool Player_804A6F38::fn_8023FC08() {
    return field_0x42c;
}

bool Player_804A6F38::fn_802413E8() {
    return field_0x454;
}

bool Player_804A6F38::fn_802412CC() {
    return field_0x460;
}

u8 Player_804A6F38::fn_802412C4() {
    return field_0x458;
}

s32 Player_804A6F38::fn_8023FC24() {
    return field_0xbec;
}

s32 Player_804A6F38::fn_80240644() {
    return field_0xd60;
}

s32 Player_804A6F38::fn_8024322C() {
    return field_0xd08;
}

bool Player_804A6F38::fn_8023FF88() {
    return field_0x1200;
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

extern "C" void fn_80252FBC() {}

bool Player_804A6F38::fn_80249068() {
    return field_0x121f;
}

bool Player_804A6F38::fn_8024BBB0() {
    return field_0xce8;
}
