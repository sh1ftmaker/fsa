#include "game/GameAudio.h"
#include "game/cls_8031709C.h"
#include "game/Effect.h"
#include "game/Message.h"

// One step of a state's sequence: the id for field_0x0 and how many frames it
// lasts (-1 ends the sequence, -2 restarts it).
struct Step_80316E34 {
    /* 0x0 */ u16 id;
    /* 0x2 */ s16 frames;
};

static const Step_80316E34 lbl_80471540[] = {{0x15, 6}, {0x1F, 6}, {0x20, -1}};
static const Step_80316E34 lbl_8047154C[] = {{0x15, 6}, {0x21, 6}, {0x22, -1}};
static const Step_80316E34 lbl_80471558[] = {
    {0x15, 6}, {0x1B, 6}, {0x1C, 4}, {0x1D, 10}, {0x1C, 6}, {0x15, 6}, {0x23, 3}, {0x24, 3},
    {0x25, 3}, {0x23, 3}, {0x24, 3}, {0x25, 3}, {0x23, 3}, {0x24, 3}, {0x25, 3}, {0x15, -1}};
static const Step_80316E34 lbl_80471598[] = {{0x1D, 20}, {0x1C, 4}, {0x1E, 4}, {0x15, -1}};
static const s32 lbl_804715A8[] = {0x50051, 0x50052, 0x50053, 0x50054};
static const Step_80316E34* lbl_804AADB8[] = {lbl_80471540, lbl_8047154C, lbl_80471558,
                                              lbl_80471598};

cls_8031709C::cls_8031709C() {
    field_0x0 = -1;
    field_0x4 = -1;
    field_0x8 = 0;
    field_0x9 = 0;
}

cls_8031709C::~cls_8031709C() {}

s32 cls_8031709C::fn_80316E74(s32 arg) {
    s32 result = -1;
    s32 state = fn_8037E9F4(arg);
    if (state != field_0x4) {
        field_0x4 = state;
        field_0x8 = 0;
        field_0x9 = 0;
        if (state >= 0) {
            result = lbl_804715A8[state];
        }
        if (state == 3 && field_0xc < 8) {
            spawnEmitter_8013CC50(field_0xc, &field_0x10, 0x275, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0xc, &field_0x10, 0x276, 0, 0, NULL, NULL, true, 1.0f);
        }
    }
    if (field_0x4 >= 0) {
        const Step_80316E34* steps = lbl_804AADB8[field_0x4];
        field_0x0 = steps[field_0x8].id;
        s16 frames = steps[field_0x8].frames;
        if (frames != -1) {
            field_0x9++;
            if (field_0x9 >= frames) {
                field_0x8++;
                frames = steps[field_0x8].frames;
                if (frames == -2) {
                    field_0x8 = 0;
                } else if (frames == -1) {
                    if (field_0xc < 8) {
                        if (state == 0) {
                            spawnEmitter_8013CC50(field_0xc, &field_0x10, 0x275, 0, 0, NULL, NULL, true,
                                        1.0f);
                        } else if (state == 1) {
                            spawnEmitter_8013CC50(field_0xc, &field_0x10, 0x276, 0, 0, NULL, NULL, true,
                                        1.0f);
                        }
                    }
                }
                field_0x9 = 0;
            }
        }
    }
    return result;
}

void cls_8031709C::fn_80316E50(s32 layer, const JGeometry::TVec3f& pos) {
    field_0x0 = -1;
    field_0x4 = -1;
    field_0xc = layer;
    field_0x10 = pos;
}

s32 cls_8031709C::fn_80316E34() {
    if (field_0x4 >= 0) {
        return field_0x0;
    }
    return -1;
}
