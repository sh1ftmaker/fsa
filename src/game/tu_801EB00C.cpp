#include "game/GameAudio.h"
#include "game/cls_801EB8CC.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"

// This unit's .sdata2 (0x8053F678..0x8053F6B8); defined at the end of the file.
extern const f32 lbl_8053F678;
extern const f32 lbl_8053F67C;
extern const f32 lbl_8053F680;
extern const f32 lbl_8053F684;
extern const f32 lbl_8053F688;
extern const f32 lbl_8053F68C;
extern const f32 lbl_8053F690;
extern const f32 lbl_8053F694;
extern const f32 lbl_8053F698;
extern const f32 lbl_8053F69C;
extern const f32 lbl_8053F6A0;
extern const f32 lbl_8053F6A4;
extern const f32 lbl_8053F6A8;
extern const f32 lbl_8053F6AC;
extern const f32 lbl_8053F6B0[2];

// FIXME: 98.42 %. The six lfs of the named .sdata2 constants (lbl_8053F69C ..
// lbl_8053F6B0) have the same symbol names as the original but compare
// arg_mismatch; the instruction order and the stores match, and the values
// are the original's (dtk's split: 90, 70, 20, 0.05, 0.7, 0.9). Inline literals
// instead of the named constants were tried: the lfs then load @NNN pool
// entries, which compare worse, so the original does reference the named
// symbols. The cause of the arg_mismatch is still open.
cls_801EBA14::cls_801EBA14() {
    field_0x0 = 1;
    field_0x4 = 1;
    field_0x8 = lbl_8053F69C;
    field_0xc = lbl_8053F6A0;
    field_0x10 = lbl_8053F6A4;
    field_0x14 = lbl_8053F6A8;
    field_0x18 = lbl_8053F6AC;
    field_0x1c = lbl_8053F6B0[0];
    field_0x20 = lbl_8053F6AC;
    field_0x24 = 0x600;
}

// Constructor (fn_801EB950, now __ct__12cls_801EB8CCFv): zeroes the floats and
// emitter pointers, builds field_0xc and reads the timer from fn_801CD664(1).
// FIXME: 42.08 %. The original builds field_0xc (the cls_801EBA14 constructor,
// inlined) between the float stores at 0x0..0x8 and the pointer clears; ours
// constructs field_0xc as a member before the body, so the body's stores come
// after it (missing/extra, reloc-name on the sdata2 constants).
cls_801EB8CC::cls_801EB8CC() {
    for (s32 i = 0; i < 3; i++) {
        field_0x0[i] = lbl_8053F678;
    }
    for (s32 i = 0; i < 7; i++) {
        field_0x34[i] = NULL;
    }
    field_0x50 = NULL;
    field_0x54 = lbl_8053F678;
    field_0x58 = lbl_8053F678;
    field_0x5c = 0;
    field_0x5e = fn_801CD664(1);
}

// The deleting form (__dl__FPv when asked) is the compiler's; the body is just the
// cleanup of the eight emitter pointers.
cls_801EB8CC::~cls_801EB8CC() {
    for (s32 i = 0; i < 7; i++) {
        fn_801EE434(field_0x34[i]);
    }
    fn_801EE434(field_0x50);
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

// Definitions of the constants above, in address order.
const f32 lbl_8053F678 = 0.0f;
const f32 lbl_8053F67C = 1.0f;
const f32 lbl_8053F680 = 32.0f;
const f32 lbl_8053F684 = 0.5f;
const f32 lbl_8053F688 = 3.0f;
const f32 lbl_8053F68C = 0.1f;
const f32 lbl_8053F690 = 2.5f;
const f32 lbl_8053F694 = 5.0f;
const f32 lbl_8053F698 = 14.0f;
const f32 lbl_8053F69C = 90.0f;
const f32 lbl_8053F6A0 = 70.0f;
const f32 lbl_8053F6A4 = 20.0f;
const f32 lbl_8053F6A8 = 0.05f;
const f32 lbl_8053F6AC = 0.7f;
const f32 lbl_8053F6B0[2] = {0.9f, 0.0f};
