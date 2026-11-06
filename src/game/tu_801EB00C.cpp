#include "game/GameAudio.h"
#include "game/cls_801EB8CC.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"

// Constructor (fn_801EB950, now __ct__12cls_801EB8CCFv): zeroes the vector,
// builds field_0xc (its constructor inlined), clears the rest, takes the timer
// from fn_801CD664(0x10000) and then clears the seven emitter pointers.
// FIXME: 98.1 %. Only the literal labels (@NNN vs lbl_8053F678...: the
// literals of fn_801EB00C..fn_801EB710, not written yet, come first).
cls_801EB8CC::cls_801EB8CC()
    : field_0x0(0.0f, 0.0f, 0.0f), field_0x50(NULL), field_0x54(0.0f), field_0x58(0.0f), field_0x5c(0),
      field_0x5e(fn_801CD664(0x10000)) {
    for (s32 i = 0; i < 7; i++) {
        field_0x34[i] = NULL;
    }
}

// FIXME: 98.42 %. Only the .sdata2 offsets of its six literals differ: the
// unit's pool (0x8053F678) also holds the literals of fn_801EB00C..fn_801EB710,
// which aren't written yet.
cls_801EBA14::cls_801EBA14() {
    field_0x0 = 1;
    field_0x4 = 1;
    field_0x8 = 90.0f;
    field_0xc = 70.0f;
    field_0x10 = 20.0f;
    field_0x14 = 0.05f;
    field_0x18 = 0.7f;
    field_0x1c = 0.9f;
    field_0x20 = 0.7f;
    field_0x24 = 0x600;
}

// The deleting form (__dl__FPv when asked) is the compiler's; the body is just the
// cleanup of the eight emitter pointers.
cls_801EB8CC::~cls_801EB8CC() {
    for (s32 i = 0; i < 7; i++) {
        releaseEmitter_801EE434(field_0x34[i]);
    }
    releaseEmitter_801EE434(field_0x50);
}

// fn_801EB648: the flag at field_0x54 (truncated to a byte) gates the scan of the
// seven emitter pointers.
s32 cls_801EB8CC::fn_801EB648() const {
    s32 found = 0;
    if ((u8)field_0x54 != 0) {
        for (s32 i = 0; i < 7; i++) {
            if (field_0x34[i] != NULL) {
                found = 1;
                break;
            }
        }
    }
    return found;
}
