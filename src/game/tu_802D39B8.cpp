// The stage/screen state, GameView_804A9950 (include/game/GameView.h).
// Written so far: the stage predicates that also read GameState_804B0FE0.

#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Player.h"
#include <stdio.h>

// Bit masks, most significant bit first: bit n of a word list is
// list[n / 32] & lbl_8046E1CC[n % 32].
static const u32 lbl_8046E1CC[32] = {
    0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000,
    0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
    0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
    0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001,
};

// Stage names per (stage, level) (field_0x4, field_0x8): fn_802D8128 / fn_802D8228
// build the archive names from them; the last row is empty.
static const char* lbl_804A9530[19][4] = {
    {"010", "011", "200", "013"},
    {"020", "021", "201", "023"},
    {"030", "031", "202", "033"},
    {"040", "041", "203", "043"},
    {"050", "051", "204", "053"},
    {"060", "061", "205", "063"},
    {"070", "071", "206", "073"},
    {"080", "207", "081", "083"},
    {"002", "001", "000", "990"},
    {"004", "005", "006", "991"},
    {"007", "008", "009", "992"},
    {"090", "091", "092", "093"},
    {"094", "095", "096", "097"},
    {"100", "110", "101", "102"},
    {"103", "111", "112", "104"},
    {"105", "106", "107", "108"},
    {"109", "113", "114", "115"},
    {"116", "100", "100", "100"},
    {NULL, NULL, NULL, NULL},
};

// Screen-set letters (fn_802D9C4C, indexed by field_0x40).
static const char* lbl_804A9660[28] = {
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s",
    "t", "u", "v", "w", "x", "y", "z", "1a", "1b",
};

// Hex digits (fn_802D9C1C, indexed by field_0x44).
static const char* lbl_804A96D0[16] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F",
};

// Map tables per (stage, level), and their second variants (fn_802D3D48).
static const char* lbl_804A9710[18][4] = {
    {"map010.csv", "map011.csv", "map200.csv", "map013.csv"},
    {"map020.csv", "map021.csv", "map201.csv", "map023.csv"},
    {"map030.csv", "map031.csv", "map202.csv", "map033.csv"},
    {"map040.csv", "map041.csv", "map203.csv", "map043.csv"},
    {"map050.csv", "map051.csv", "map204.csv", "map053.csv"},
    {"map060.csv", "map061.csv", "map205.csv", "map063.csv"},
    {"map070.csv", "map071.csv", "map206.csv", "map073.csv"},
    {"map080.csv", "map207.csv", "map081.csv", "map083.csv"},
    {"map002.csv", "map001.csv", "map000.csv", "map990.csv"},
    {"map004.csv", "map005.csv", "map006.csv", "map991.csv"},
    {"map007.csv", "map008.csv", "map009.csv", "map992.csv"},
    {"map090.csv", "map091.csv", "map092.csv", "map093.csv"},
    {"map094.csv", "map095.csv", "map096.csv", "map097.csv"},
    {"map100.csv", "map110.csv", "map101.csv", "map102.csv"},
    {"map103.csv", "map111.csv", "map112.csv", "map104.csv"},
    {"map105.csv", "map106.csv", "map107.csv", "map108.csv"},
    {"map109.csv", "map113.csv", "map114.csv", "map115.csv"},
    {"map116.csv", "map100.csv", "map100.csv", "map100.csv"},
};

static const char* lbl_804A9830[18][4] = {
    {"map010_1.csv", "map011_1.csv", "map200.csv", "map013_1.csv"},
    {"map020_1.csv", "map021_1.csv", "map201.csv", "map023_1.csv"},
    {"map030_1.csv", "map031_1.csv", "map202.csv", "map033_1.csv"},
    {"map040_1.csv", "map041_1.csv", "map203.csv", "map043_1.csv"},
    {"map050_1.csv", "map051_1.csv", "map204.csv", "map053_1.csv"},
    {"map060_1.csv", "map061_1.csv", "map205.csv", "map063_1.csv"},
    {"map070_1.csv", "map071_1.csv", "map206.csv", "map073_1.csv"},
    {"map080_1.csv", "map207.csv", "map081_1.csv", "map083_1.csv"},
    {"map002_1.csv", "map001_1.csv", "map000_1.csv", "map990_1.csv"},
    {"map004_1.csv", "map005_1.csv", "map006_1.csv", "map991.csv"},
    {"map007_1.csv", "map008_1.csv", "map009_1.csv", "map992_1.csv"},
    {"map090.csv", "map091.csv", "map092.csv", "map093.csv"},
    {"map094.csv", "map095.csv", "map096.csv", "map097.csv"},
    {"map100.csv", "map110.csv", "map101.csv", "map102.csv"},
    {"map103.csv", "map111.csv", "map112.csv", "map104.csv"},
    {"map105.csv", "map106.csv", "map107.csv", "map108.csv"},
    {"map109.csv", "map113.csv", "map114.csv", "map115.csv"},
    {"map116.csv", "map100.csv", "map100.csv", "map100.csv"},
};

void GameView_804A9950::fn_802D39B8(s32 index, s32 value) {
    field_0x131a8[index] = value;
}

bool GameView_804A9950::fn_802D39CC() {
    if (field_0x18 == field_0x10 && field_0x14 == field_0xc) {
        return true;
    }
    return false;
}

s32 GameView_804A9950::fn_802D39FC() {
    return field_0x60[field_0x18][field_0x14];
}

// FIXME: fn_802D3A18 and fn_802D3D48 are not written yet.


// FIXME: 78.3. Scheduling and regalloc only: the target computes index * 0x200 and word * 4
// before materialising lbl_8046E1CC; ours does the lis/addi of the table first.
bool GameView_804A9950::fn_802D46D8(s32 index, u32 bit, bool set) {
    u32 word = bit / 32;
    if (!set) {
        return (lbl_8053AAF8->field_0x4->mView->field_0x2d48[index][word] & lbl_8046E1CC[bit % 32]) != 0;
    }
    lbl_8053AAF8->field_0x4->mView->field_0x2d48[index][word] |= lbl_8046E1CC[bit % 32];
    return true;
}

// FIXME: 94.8. Scheduling and regalloc: the target adds the view to i * 2 before the slot
// row (view + i * 2 + 0x2948, then lhzx with slot << 7); ours adds the row and i first and
// loads the view (r5) after.
s32 GameView_804A9950::fn_802D4C10(s32 layer, u16 id) {
    s32 slot = layer < 8 ? 0 : layer - 7;
    for (int i = 0; i < 0x40; i++) {
        if (id == lbl_8053AAF8->field_0x4->mView->field_0x2948[slot][i]) {
            return i;
        }
    }
    return -1;
}

GameView_804A9950* fn_802D6A9C(void) {
    return lbl_8053AAF8->field_0x4->mView;
}

void GameView_804A9950::fn_802D6AAC(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f9 = value;
}

// FIXME: 91.0 %. Only scheduling: the target computes bit / 32 (srawi/addze) and
// the row offset earlier; ours places one add later (word & mask order is as
// in the target; a mask local and swapped & operands are worse).
bool GameView_804A9950::fn_802D6AC4(s32 bit) {
    return (field_0xfd68[field_0x18][field_0x14][bit / 32] & (1 << (bit % 32))) != 0;
}

// `^ -1` (not ~): the target clears with xor+and, not andc (as fn_802D78B4).
void GameView_804A9950::fn_802D6B24(s32 bit, bool set) {
    // word before shift: the declaration order decides the registers (as fn_802D7310).
    s32 word = bit / 32;
    s32 shift = bit % 32;
    if (set) {
        field_0xfd68[field_0x18][field_0x14][word] |= 1 << shift;
    } else {
        field_0xfd68[field_0x18][field_0x14][word] &= (1 << shift) ^ -1;
    }
}

void GameView_804A9950::fn_802D6BC8(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f4 = value;
}

void GameView_804A9950::fn_802D6BE0(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f3 = value;
}

void GameView_804A9950::fn_802D6BF8(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131f2 = value;
}

// Componentwise TUtil::epsilonEquals of two vectors (the shape of tu_8027E5BC's epsilonEqualsXY).
static inline bool epsilonEquals(const JGeometry::TVec2f& a, const JGeometry::TVec2f& b, f32 eps) {
    bool equal = false;
    if (JGeometry::TUtilf::epsilonEquals(a.x, b.x, eps) && JGeometry::TUtilf::epsilonEquals(a.y, b.y, eps)) {
        equal = true;
    }
    return equal;
}

// FIXME: 98.0. Regalloc only (the loop's i * 4 / i * 8 counters and the match flag take other
// registers); the 32.0f / 0.0f literals wait for fn_802D3A18, which uses 0.0f first.
void GameView_804A9950::fn_802D6C10(JGeometry::TVec2f pos, u32 id) {
    for (int i = 0; i < 8; i++) {
        if (id == lbl_8053AAF8->field_0x4->mView->field_0x13188[i] &&
            epsilonEquals(lbl_8053AAF8->field_0x4->mView->field_0x13148[i], pos, JGeometry::TUtilf::epsilon())) {
            lbl_8053AAF8->field_0x4->mView->field_0x130e8[i].set(0.0f, 0.0f);
            lbl_8053AAF8->field_0x4->mView->field_0x13128[i] = -1;
            lbl_8053AAF8->field_0x4->mView->field_0x13148[i].set(0.0f, 0.0f);
            lbl_8053AAF8->field_0x4->mView->field_0x13188[i] = -1;
            return;
        }
    }
}

// FIXME: 84.6. Three instructions short: the target loads b.x once before the first loop
// and reloads mView (lwz 0xc) for the id test after the epsilon compare; ours keeps both
// in the loop body / one register. Registers differ throughout.
bool GameView_804A9950::fn_802D6D70(JGeometry::TVec2f a, JGeometry::TVec2f b, s32 value, u32 id) {
    bool found = false;
    for (int i = 0; i < 8; i++) {
        if (epsilonEquals(lbl_8053AAF8->field_0x4->mView->field_0x13148[i], b, JGeometry::TUtilf::epsilon()) &&
            id == lbl_8053AAF8->field_0x4->mView->field_0x13188[i]) {
            found = true;
            break;
        }
    }
    if (!found) {
        for (int i = 0; i < 8; i++) {
            if (lbl_8053AAF8->field_0x4->mView->field_0x13128[i] == -1) {
                lbl_8053AAF8->field_0x4->mView->field_0x130e8[i] = a;
                lbl_8053AAF8->field_0x4->mView->field_0x13148[i] = b;
                lbl_8053AAF8->field_0x4->mView->field_0x13128[i] = value;
                lbl_8053AAF8->field_0x4->mView->field_0x13188[i] = id;
                break;
            }
        }
    }
    return found;
}

// FIXME: 96.4. The free-slot search: the target's loop has no entry test (body first, then
// i < 8 at the bottom, falling into the shift); ours jumps to the test first (b plus a
// second i < 8 test, 3 instructions). A plain for/break is unrolled instead. Registers
// differ in the epsilon compare.
void GameView_804A9950::fn_802D6F38(u32 key, JGeometry::TVec2f pos, s32 value, bool add) {
    int i = 0;
    if (add == true) {
        while (i < 8 && field_0x130c8[i] != -1) {
            i++;
        }
        if (i >= 8) {
            // The list is full: drop the oldest entry.
            for (int j = 0; j < 7; j++) {
                field_0x13068[j] = field_0x13068[j + 1];
                field_0x13088[j] = field_0x13088[j + 1];
                field_0x130c8[j] = field_0x130c8[j + 1];
            }
            i = 7;
        }
        field_0x13068[i] = key;
        field_0x13088[i] = pos;
        field_0x130c8[i] = value;
    } else {
        for (int i = 0; i < 8; i++) {
            if ((field_0x13068[i] & ~0xF80) == (key & ~0xF80) &&
                epsilonEquals(field_0x13088[i], pos, JGeometry::TUtilf::epsilon()) && field_0x130c8[i] == value) {
                for (int j = i; j < 7; j++) {
                    field_0x13068[j] = field_0x13068[j + 1];
                    field_0x13088[j] = field_0x13088[j + 1];
                    field_0x130c8[j] = field_0x130c8[j + 1];
                }
                field_0x13068[7] = 0;
                field_0x13088[7].set(0.0f, 0.0f);
                field_0x130c8[7] = -1;
                return;
            }
        }
    }
}

bool GameView_804A9950::fn_802D72BC(s32 bit) {
    return ((1 << (bit % 32)) & lbl_8053AAF8->field_0x4->mView->field_0x12fe8[bit / 32]) != 0;
}

void GameView_804A9950::fn_802D7310(s32 bit, bool set) {
    // word before shift: the declaration order decides the registers; `^ -1` because the target
    // clears with xor+and, not andc.
    s32 word = bit / 32;
    s32 shift = bit % 32;
    if (set) {
        field_0x12fe8[word] |= 1 << shift;
    } else {
        field_0x12fe8[word] &= (1 << shift) ^ -1;
    }
}

// FIXME: 88.1 %. Scheduling: one add (the word address) is emitted two slots
// earlier than in the target; a mask local and swapped & operands are worse.
bool GameView_804A9950::fn_802D7384(s32 bit) {
    return (field_0x12f68[bit / 32] & (1 << (bit % 32))) != 0;
}

void GameView_804A9950::fn_802D73CC(s32 bit, bool set) {
    // word before shift: the declaration order decides the registers; `^ -1` because the target
    // clears with xor+and, not andc.
    s32 word = bit / 32;
    s32 shift = bit % 32;
    if (set) {
        field_0x12f68[word] |= 1 << shift;
    } else {
        field_0x12f68[word] &= (1 << shift) ^ -1;
    }
}

void GameView_804A9950::fn_802D752C(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131ef = value;
}

u8 GameView_804A9950::fn_802D7544(s32 index) {
    if (index >= 0 && index < 0x100) {
        return field_0x9968[field_0x18][field_0x14][index];
    }
    return 0;
}

void GameView_804A9950::fn_802D7584(s32 index, u8 value) {
    if (index >= 0 && index < 0x100) {
        field_0x9968[field_0x18][field_0x14][index] = value;
    }
}

// FIXME: 91.3. Only scheduling: the word address add is emitted two slots earlier than in the target
// (same as fn_802D6AC4; swapped & operands are worse).
bool GameView_804A9950::fn_802D75BC(s32 bit) {
    return (field_0x6768[field_0x18][field_0x14][bit / 32] & (1 << (bit % 32))) != 0;
}

void GameView_804A9950::fn_802D7618(s32 bit) {
    field_0x6768[field_0x18][field_0x14][bit / 32] |= 1 << (bit % 32);
}

void GameView_804A9950::fn_802D766C(bool flag, s32 layer) {
    if (flag) {
        field_0x6748[layer < 8 ? 0 : layer - 7]++;
    }
}

// FIXME: 91.3. Only scheduling: the word address add is emitted two slots earlier than in the target
// (same as fn_802D6AC4; swapped & operands are worse).
bool GameView_804A9950::fn_802D76A8(s32 bit) {
    return (field_0x3548[field_0x18][field_0x14][bit / 32] & (1 << (bit % 32))) != 0;
}

void GameView_804A9950::fn_802D7704(s32 bit, bool count, s32 layer) {
    field_0x3548[field_0x18][field_0x14][bit / 32] |= 1 << (bit % 32);
    if (count) {
        field_0x6748[layer < 8 ? 0 : layer - 7]++;
    }
}

// `bits ^ 0xFF` (not ~bits): the target clears with xori+and, not andc.
void GameView_804A9950::fn_802D7790(s32 index, u16 bits) {
    lbl_8053AAF8->field_0x4->mView->field_0x131b8[index] &= bits ^ 0xFF;
}

void GameView_804A9950::fn_802D77BC(s32 index, u16 bits) {
    lbl_8053AAF8->field_0x4->mView->field_0x131b8[index] |= bits;
}

void GameView_804A9950::fn_802D77E4(s32 index, u16 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131b8[index] = value;
}

// FIXME: 80.0 %. The clrlwi of bits is emitted before the lhz load in ours,
// after it in the target; swapping the & operands is worse.
bool GameView_804A9950::fn_802D7804(s32 index, u16 bits) {
    return (field_0x131b8[index] & bits) != 0;
}

void GameView_804A9950::fn_802D782C(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131de = value;
}

void GameView_804A9950::fn_802D7844(u8 value, s32 index) {
    if (index >= 0 && index < 4) {
        lbl_8053AAF8->field_0x4->mView->field_0x131d8[index] = value;
    }
}

void GameView_804A9950::fn_802D7870(u16 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x131dc = value;
}

bool GameView_804A9950::fn_802D7888(s32 bit) {
    return ((1 << bit) & lbl_8053AAF8->field_0x4->mView->field_0x174) != 0;
}

// `^ -1` (not ~): the target clears with xor+and, not andc.
void GameView_804A9950::fn_802D78B4(s32 bit, bool set) {
    if (set) {
        lbl_8053AAF8->field_0x4->mView->field_0x174 |= 1 << bit;
    } else {
        lbl_8053AAF8->field_0x4->mView->field_0x174 &= (1 << bit) ^ -1;
    }
}

void GameView_804A9950::fn_802D790C(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x171 = value;
}

void GameView_804A9950::fn_802D7920(u8 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x170 = value;
}

void GameView_804A9950::fn_802D7934(s32 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x5c = value;
}

s32 GameView_804A9950::fn_802D7948() {
    GameView_804A9950* view = lbl_8053AAF8->field_0x4->mView;
    if (view->field_0x5c == 0) {
        return view->field_0x58;
    }
    return view->field_0x5c - 1;
}

void GameView_804A9950::fn_802D7970(s32 value) {
    lbl_8053AAF8->field_0x4->mView->field_0x34 = value;
    if (value < 0x11) {
        lbl_8053AAF8->field_0x4->mView->field_0x1328c = 0;
    }
}

void GameView_804A9950::fn_802D79A4(s32 index, f32 a, f32 b, f32 c) {
    if (field_0x1327c[index] == 0 && field_0x1324c[index] == 0.0f) {
        field_0x1324c[index] = a;
        field_0x1325c[index] = b;
        field_0x1326c[index] = c;
        field_0x1327c[index] = 1;
    }
}

void GameView_804A9950::fn_802D79E8(s32 index, f32 a, f32 b, bool flag) {
    if (field_0xf0[index].x < b) {
        field_0xf0[index].x = b;
    }
    if (field_0xf0[index].y < a) {
        field_0xf0[index].y = a;
    }
    if (!flag) {
        b = 0.75f * b;
        a = 0.75f * a;
        for (int i = 0; i < 16; i++) {
            if (field_0xf0[i].x < b) {
                field_0xf0[i].x = b;
            }
            if (field_0xf0[i].y < a) {
                field_0xf0[i].y = a;
            }
        }
    }
}

u64 GameView_804A9950::fn_802D7AC4(s32 layer) {
    if (layer < 8) {
        return lbl_8053AAF8->field_0x4->mView->field_0x1048[field_0x18][field_0x14][0];
    }
    return lbl_8053AAF8->field_0x4->mView->field_0x1048[field_0x18][field_0x14][layer - 7];
}

// FIXME: 99.0. Regalloc only: the target keeps the shift count in r31 and the slot index in r30; ours swaps them.
s32 GameView_804A9950::fn_802D7B34(s32 layer, s32 slot) {
    s32 idx = layer < 8 ? 0 : layer - 7;
    s32 shift = 60 - slot * 4;
    return (lbl_8053AAF8->field_0x4->mView->field_0x1048[field_0x18][field_0x14][idx] & (0xFULL << shift)) >> shift;
}

// FIXME: 99.4. Regalloc only, as fn_802D7B34: the target keeps the shift in r31 and the slot index in r30.
void GameView_804A9950::fn_802D7BE8(s32 layer, s32 slot, u32 value) {
    s32 idx = layer < 8 ? 0 : layer - 7;
    s32 shift = 60 - slot * 4;
    lbl_8053AAF8->field_0x4->mView->field_0x1048[field_0x18][field_0x14][idx] &= (0xFULL << shift) ^ -1;
    lbl_8053AAF8->field_0x4->mView->field_0x1048[field_0x18][field_0x14][idx] |= (u64)value << shift;
}

void GameView_804A9950::fn_802D7D00(s32 layer, s32 bit, bool set) {
    s32 slot = layer < 8 ? 0 : layer - 7;
    if (set) {
        lbl_8053AAF8->field_0x4->mView->field_0x1048[field_0x18][field_0x14][slot] |= 1ULL << bit;
    } else {
        lbl_8053AAF8->field_0x4->mView->field_0x1048[field_0x18][field_0x14][slot] &= (1ULL << bit) ^ -1;
    }
}

s32 GameView_804A9950::fn_802D7E04(s32 index) {
    return lbl_8053AAF8->field_0x4->mView->field_0x308[index];
}

s32 GameView_804A9950::fn_802D7E1C(s32 index) {
    return lbl_8053AAF8->field_0x4->mView->field_0x328[index];
}

void GameView_804A9950::fn_802D7E34(s32 index) {
    GameView_804A9950* view = lbl_8053AAF8->field_0x4->mView;
    if (view->field_0x308[index] < 0xFF) {
        view->field_0x308[index]++;
    }
}

// FIXME: 85.2 %. Scheduling: the target loads field_0x8 before the stage
// (field_0x4) and the view; ours loads the stage first.
bool GameView_804A9950::fn_802D7F40(s32 bit) {
    return ((1 << bit) & lbl_8053AAF8->field_0x4->mView->field_0x348[field_0x4][field_0x8]) != 0;
}

// `^ -1` (not ~): the target clears with xor+and, not andc (as fn_802D78B4).
void GameView_804A9950::fn_802D7F84(s32 bit, bool set) {
    if (set) {
        lbl_8053AAF8->field_0x4->mView->field_0x348[field_0x4][field_0x8] |= 1 << bit;
    } else {
        lbl_8053AAF8->field_0x4->mView->field_0x348[field_0x4][field_0x8] &= (1 << bit) ^ -1;
    }
}

bool GameView_804A9950::fn_802D800C(s32 bit) {
    return ((1 << bit) & lbl_8053AAF8->field_0x4->mView->field_0x178[field_0x18][field_0x14]) != 0;
}

// `^ -1` (not ~): the target clears with xor+and, not andc (as fn_802D78B4).
void GameView_804A9950::fn_802D8050(s32 bit, bool set, u8 value) {
    if (set) {
        lbl_8053AAF8->field_0x4->mView->field_0x178[field_0x18][field_0x14] |= 1 << bit;
        if (lbl_8053AAF8->field_0x4->mView->field_0x308[bit] < 0xFF) {
            lbl_8053AAF8->field_0x4->mView->field_0x308[bit]++;
        }
        lbl_8053AAF8->field_0x4->mView->field_0x328[bit] = value;
    } else {
        lbl_8053AAF8->field_0x4->mView->field_0x178[field_0x18][field_0x14] &= (1 << bit) ^ -1;
        lbl_8053AAF8->field_0x4->mView->field_0x328[bit] = 0xFF;
    }
}

// FIXME: 96.5. Scheduling and regalloc: the target reads field_0x18 only after the column
// checks and loads field_0x8 before field_0x4 (as if both were arguments of an inlined
// name lookup); the format string waits for the unit's .rodata to be written.
const char* GameView_804A9950::fn_802D8128(s32 layer) {
    s32 col = field_0x14;
    s32 row = field_0x18;
    if (col >= 0 && col < 10 && row >= 0 && row < 10) {
        s32 stage = field_0x4;
        s32 level = field_0x8;
        const char* name;
        if (lbl_8053AAF8->field_0x7f == 1) {
            name = lbl_804A9530[stage][level];
        } else {
            name = "500";
        }
        s32 screen = field_0x60[row][col];
        if (screen == 0xFF) {
            screen = 0;
        }
        sprintf(field_0x1321a, "d_map%s_%d%d_mmm_2_%d.szs", name, screen / 10, screen % 10, layer - 7);
        return field_0x1321a;
    }
    return NULL;
}

// FIXME: 96.5. Scheduling and regalloc: the target reads field_0x18 only after the column
// checks and loads field_0x8 before field_0x4 (as if both were arguments of an inlined
// name lookup); the format string waits for the unit's .rodata to be written.
const char* GameView_804A9950::fn_802D8228(s32 layer) {
    s32 col = field_0x14;
    s32 row = field_0x18;
    if (col >= 0 && col < 10 && row >= 0 && row < 10) {
        s32 stage = field_0x4;
        s32 level = field_0x8;
        const char* name;
        if (lbl_8053AAF8->field_0x7f == 1) {
            name = lbl_804A9530[stage][level];
        } else {
            name = "500";
        }
        s32 screen = field_0x60[row][col];
        if (screen == 0xFF) {
            screen = 0;
        }
        sprintf(field_0x131fa, "d_map%s_%d%d_mmm_1_%d.szs", name, screen / 10, screen % 10, layer - 7);
        return field_0x131fa;
    }
    return NULL;
}

const char* GameView_804A9950::fn_802D9C1C() {
    if (field_0x44 >= 0 && field_0x44 < 16) {
        return lbl_804A96D0[field_0x44];
    }
    return "0";
}

const char* GameView_804A9950::fn_802D9C4C() {
    if (field_0x40 >= 0 && field_0x40 < 28) {
        return lbl_804A9660[field_0x40];
    }
    return "a";
}

const char* GameView_804A9950::fn_802D9C7C(s32 stage, s32 level) {
    if (lbl_8053AAF8->field_0x7f == 1) {
        return lbl_804A9530[stage][level];
    }
    return "500";
}

void GameView_804A9950::fn_802D9CB0() {
    lbl_8053AB10->field_0x24->fn_8030C85C();
}

void GameView_804A9950::fn_802D9FBC() {
    if (lbl_8053AAF8->field_0x7f == 1) {
        field_0xc4 = 0x8CA0;
    } else {
        field_0xc4 = 0x2A30;
    }
}

void GameView_804A9950::fn_802D9FE8(s32 a, s32 b) {
    field_0x1c = a;
    field_0x20 = b;
}

void GameView_804A9950::fn_802D9FF4(s32 a, s32 b) {
    field_0x24 = a;
    field_0x28 = b;
}

u8 GameView_804A9950::fn_802DA000() {
    return lbl_8053AB10->field_0x24->field_0x1e8;
}

u8 GameView_804A9950::fn_802DCD0C() {
    return lbl_8053AB10->field_0x24->field_0x1e8;
}

// FIXME: 94.2. One instruction: the target keeps the result in r6 (li r6, 0 at entry) and
// returns it through clrlwi r3, r6, 24; ours builds it in r3 directly.
bool GameView_804A9950::fn_802DCD1C(const s32* dir) {
    bool moved = false;
    if (lbl_8053AB10->field_0x24->field_0x1e8 == 0) {
        if (dir[0] > 0) {
            field_0x14++;
        } else if (dir[0] < 0) {
            field_0x14--;
        } else if (dir[1] > 0) {
            field_0x18++;
        } else {
            field_0x18--;
        }
        fn_80231424();
        fn_801F856C()->fn_801F7E88();
        fn_802D8D50(true);
        fn_802D8328(false, false);
        lbl_8053AB10->field_0x24->fn_8030C928(&mVisibleBox);
        moved = true;
    }
    return moved;
}

void GameView_804A9950::fn_802DCDF4() {
    fn_801F856C()->fn_801F7CAC();
    fn_801F856C()->fn_801F6204(0);
    fn_80308D0C();
    lbl_8053AAF8->field_0x4->mView->field_0x131de = 0;
    fn_8013C824()->fn_8013C8CC();
    field_0xc9 = 0;
}

void GameView_804A9950::vfunc_0x14() {}

// FIXME: the other GameView_804A9950 functions before 0x802DE814 are not written yet.

GameView_804A9950::~GameView_804A9950() {}

GameView_804A9950::GameView_804A9950() {
    field_0x4 = 0;
    field_0x8 = 0;
    field_0x14 = 0;
    field_0x18 = 0;
    field_0x34 = 0;
    field_0x38 = 0;
    field_0x3c = 0;
    field_0x40 = 0;
    field_0x44 = 0;
    field_0x48 = 0;
    field_0x4c = 0;
    field_0x50 = 0;
    field_0x54 = 0;
    field_0x58 = 0;
    field_0xec = 0;
    field_0xdc.set(0.0f, 0.0f, 512.0f, 512.0f);
    field_0x131f7 = 0;
    field_0x131f8 = 0;
}

bool fn_802DE764(s32 stage, s32 level, s32 value) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    GameView_804A9950* view = lbl_8053AAF8->field_0x4->mView;
    if (stage == view->field_0x4 && level == view->field_0x8 && value == view->field_0x60[view->field_0x18][view->field_0x14]) {
        return true;
    }
    return false;
}

bool fn_802DE7CC(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 0 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 0) {
        return true;
    }
    return false;
}

bool fn_802DE814(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    s32 stage = lbl_80529DEC.field_0x118;
    if ((stage == 0 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 1 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 2 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 3 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 4 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 5 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 6 && lbl_80529DEC.field_0x11c == 2) ||
        (stage == 7 && lbl_80529DEC.field_0x11c == 1) ||
        (stage == 10 && lbl_80529DEC.field_0x11c == 2))
    {
        return true;
    }
    return false;
}

// FIXME: 91.0 %. Same as fn_802DE9A0: the target keeps lbl_8053AAF8->0x4 in r4
// and reloads the view (lwz 0xc) for the second pair; ours loads it once.
bool fn_802DE93C(void) {
    if (lbl_8053AAF8->field_0x7f != 0) {
        if ((lbl_8053AAF8->field_0x4->mView->field_0x4 == 4 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 0) ||
            (lbl_8053AAF8->field_0x4->mView->field_0x4 == 7 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 2))
        {
            return true;
        }
        return false;
    }
    return false;
}

// FIXME: 93.9 %. The target keeps lbl_8053AAF8->0x4 in r4 and reloads its
// mView (lwz 0xc) and the view's field_0x4 for the second stage pair; ours
// loads both once. No-ops: a local for lbl_8053AAF8->0x4, an inline view
// getter defined in this file (with or without a local), a free inline on
// GameRootSub04_801F856C; an inline isStage(stage, level) helper drops to 78 %.
bool fn_802DE9A0(void) {
    if (lbl_8053AAF8->field_0x7f != 0) {
        if ((lbl_8053AAF8->field_0x4->mView->field_0x4 == 2 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 3) ||
            (lbl_8053AAF8->field_0x4->mView->field_0x4 == 7 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 1))
        {
            return true;
        }
        return false;
    }
    s32 n = lbl_80529DEC.field_0x12c + lbl_80529DEC.field_0x128 * 4;
    if (n == 1 || n == 6) {
        return true;
    }
    return false;
}

// FIXME: 93.5. Same family as fn_802DEABC: the target reloads the view (lwz 0xc) before the second
// pair and reuses it for the third; ours loads it once. An || of the last two blocks is 87 %.
bool fn_802DEA34(void) {
    if (lbl_8053AAF8->field_0x7f != 0) {
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 6 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 0) {
            return true;
        }
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 6 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 1) {
            return true;
        }
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 6 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 3) {
            return true;
        }
        return false;
    }
    return false;
}

// FIXME: 94.2 %. The target keeps lbl_8053AAF8->0x4 in r4 and reloads its
// mView (lwz 0xc) and the view's field_0x4 for the second stage pair; ours
// loads both once. An inline view getter in this file is a no-op. Re-reading
// the view into a local before each pair gives 96.9 % (field_0x4 reloaded,
// mView still not) but has no precedent, so it is not used.
bool fn_802DEABC(void) {
    if (lbl_8053AAF8->field_0x7f != 0) {
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 7 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 0) {
            return true;
        }
        if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 7 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 2) {
            return true;
        }
        return false;
    }
    s32 n = lbl_80529DEC.field_0x12c + lbl_80529DEC.field_0x128 * 4;
    if (n == 4 || n == 9) {
        return true;
    }
    return false;
}

bool fn_802DEB58(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 3 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 1) {
        return true;
    }
    return false;
}

// FIXME: 89.8 %. Same as fn_802DE9A0: the target reloads the view (lwz 0xc off
// the kept lbl_8053AAF8->0x4) for the stage >= 13 test; ours loads it once.
bool fn_802DEBA0(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    if ((lbl_8053AAF8->field_0x4->mView->field_0x4 == 9 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 3) || lbl_8053AAF8->field_0x4->mView->field_0x4 >= 13) {
        return true;
    }
    return false;
}

bool fn_802DEBF8(void) {
    if (lbl_8053AAF8->field_0x7f == 0) {
        return false;
    }
    if (lbl_8053AAF8->field_0x4->mView->field_0x4 == 8 && lbl_8053AAF8->field_0x4->mView->field_0x8 == 3) {
        return true;
    }
    return false;
}

bool fn_802DEDFC(const s32* dir) {
    return lbl_8053AAF8->field_0x4->mView->fn_802DCE5C(dir);
}
