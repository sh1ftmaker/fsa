#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/PlayerFlags.h"

bool Player_804A6F38::fn_80240AF0() {
    return field_0x1215;
}

bool Player_804A6F38::fn_802413F0() {
    return playerFlags_802413F0(this);
}

// The team checks fn_80246F28 makes before it reads the input: the player
// flags, then, when more than one player is on this team and the team mode
// (fn_8022CAF8 / field_0x120c) is 4, the team count must match, and
// field_0x11ea must be set.
inline bool Player_804A6F38::fn_80246F28_check() {
    if (!fn_802413F0()) {
        return false;
    }
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i == field_0x4) {
            count++;
        } else if (fn_8022F514(i)) {
            s32 team = field_0xd60;
            if (team >= 0) {
                if (team == fn_8022F3DC(i)) {
                    count++;
                }
            } else {
                s32 self = field_0x4;
                if (self == fn_8022F3DC(i)) {
                    count++;
                }
            }
        } else if (i == field_0xd60) {
            count++;
        }
    }
    s32 mode;
    if (count <= 1) {
        mode = 5;
    } else if (field_0xd60 >= 0) {
        mode = fn_8022CAF8(field_0xd60);
    } else {
        mode = field_0x120c;
    }
    if (mode == 4) {
        s32 a;
        if (field_0xd60 >= 0) {
            a = fn_80234004(field_0xd60);
        } else {
            a = field_0x1224;
        }
        s32 b;
        if (field_0xd60 < 0) {
            b = 0;
        } else {
            s32 n = 0;
            for (s32 j = 0; j < field_0x4; j++) {
                s32 team = field_0xd60;
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
    if (!field_0x11ea) {
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
    if (fn_80246F28_check() == false) {
        return 0.0f;
    }
    return lbl_8053AD30->fn_80309D90(fn_8030AAF4(field_0x11ec));
}
