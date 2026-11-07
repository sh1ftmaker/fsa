#include "game/GameAudio.h"
#include "game/act/tu_801D8400.h"

// Tables of the sequencer (.data 0x804A4DA8.., not written as source here): the string for each
// value, and per animation (field_0x0) the value list (0xFF ends it) and the frame counts.
extern const char* const lbl_804A4DA8[];
extern const u8* const lbl_804A4E68[];
extern const u8* const lbl_804A4EA8[];

Sequencer_801DAA24::Sequencer_801DAA24() {
    field_0x0 = 0;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xc = 0;
    field_0x10 = 2;
    field_0x14 = 0;
}

void Sequencer_801DAA24::fn_801DA9E8(s32 a, s32 b) {
    field_0x0 = a;
    field_0x8 = 0;
    field_0xc = 0;
    field_0x10 = b;
    field_0x14 = 0;
    field_0x4 = lbl_804A4E68[field_0x0][field_0xc];
}

bool Sequencer_801DAA24::fn_801DA994(s32 a, s32 b) {
    bool changed = false;
    if (field_0x0 != a) {
        field_0x0 = a;
        field_0x8 = 0;
        changed = true;
        field_0xc = 0;
        field_0x10 = b;
        field_0x14 = 0;
        field_0x4 = lbl_804A4E68[field_0x0][field_0xc];
    }
    return changed;
}

// FIXME(shotgun): 88.50 %, missing/extra. The original copies the value to the result register before the clamp tests.
s32 Sequencer_801DAA24::fn_801DA96C() {
    s32 value = field_0x4;
    if (value > 0x2F) {
        value = 0x2F;
    } else if (value < 0) {
        value = 0;
    }
    return value;
}

const char* Sequencer_801DAA24::fn_801DA938() {
    s32 value = field_0x4;
    if (value > 0x2F) {
        value = 0x2F;
    } else if (value < 0) {
        value = 0;
    }
    return lbl_804A4DA8[value];
}

bool Sequencer_801DAA24::fn_801DA904() {
    bool result = false;
    s32 value = field_0x4;
    if (value > 0x2F) {
        value = 0x2F;
    } else if (value < 0) {
        value = 0;
    }
    if (value >= 0x1C) {
        result = true;
    }
    return result;
}

bool Sequencer_801DAA24::fn_801DA894() {
    bool atEnd = false;
    if (field_0x10 == 2 && !(field_0x14 & 1)) {
        if (field_0x8 + 1 == lbl_804A4EA8[field_0x0][field_0xc] && lbl_804A4E68[field_0x0][field_0xc + 1] == 0xFF) {
            atEnd = true;
        }
    }
    return atEnd;
}

void Sequencer_801DAA24::fn_801DA7C0() {
    if (!(field_0x14 & 1)) {
        field_0x8++;
        if (field_0x8 >= lbl_804A4EA8[field_0x0][field_0xc]) {
            field_0x8 = 0;
            field_0xc++;
            if (lbl_804A4E68[field_0x0][field_0xc] == 0xFF) {
                if (field_0x10 == 0) {
                    field_0x14 |= 1;
                    field_0xc--;
                } else {
                    field_0x14 |= 2;
                    field_0xc = 0;
                }
            }
        }
    }
    field_0x4 = lbl_804A4E68[field_0x0][field_0xc];
}
