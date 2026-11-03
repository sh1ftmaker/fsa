#include "dolphin/GBAPriv.h"
#include "string.h"

// The two DSP task callbacks and the microcode have no known names, so they
// keep their symbols.txt placeholders (fn_, lbl_).

// GBA key-exchange DSP microcode (0x380 bytes at 0x804985D0). It is DSP code
// with no source form, so it isn't written here: the build extracts it from
// orig/sys/main.dol (config.yml extract:) into build/G4SE01/include/assets/.
// No ATTRIBUTE_ALIGN(32): the original is only 16-byte aligned (0x...D0).
static u8 lbl_804985D0[0x380] = {
#include "assets/dolphin/gba/GBAKey_ucode.inc"
};

static inline s32 F152(DSPTaskInfo* task) {
    s32 chan;
    GBAControl* gba;

    for (chan = 0; chan < 4; chan++) {
        gba = &__GBA[chan];
        if (&gba->task == task) {
            return chan;
        }
    }

    OSPanic(__FILE__, 169, "GBA - unexpected dsp call");
    return -1;
}

static void fn_80076EBC(void* task) {
    s32 chan;
    GBAControl* gba;

    chan = F152((DSPTaskInfo*)task);
    gba = &__GBA[chan];

    DSPSendMailToDSP(0xABBA0000);
    while (DSPCheckMailToDSP()) {
    }
    DSPSendMailToDSP((u32)gba->param);
    while (DSPCheckMailToDSP()) {
    }
}

static void fn_80076F98(void* task) {
    s32 chan;

    chan = F152((DSPTaskInfo*)task);
    __GBAX01(chan, 0);
}

void __GBAX02(s32 chan, u8* readbuf) {
    GBAControl* gba = &__GBA[chan];
    GBABootInfo* bootInfo = &__GBA[chan].bootInfo;
    GBASecParam* param = gba->param;

    memcpy(param, readbuf, 4);
    param->paletteColor = bootInfo->paletteColor;
    param->paletteSpeed = bootInfo->paletteSpeed;
    param->length = bootInfo->length;
    param->out = &param->keyA;
    DCInvalidateRange(&param->keyA, 32);
    DCFlushRange(param, 32);

    // The task pointer is taken only now, in its own block: the original
    // computes it after the cache calls (a function-scope local set at the
    // top, or no local at all, allocates registers differently).
    {
        DSPTaskInfo* task = &gba->task;

        task->priority = 255;
        task->iram_mmem_addr = (u16*)OSCachedToPhysical(lbl_804985D0);
        task->iram_length = sizeof(lbl_804985D0);
        task->iram_addr = 0;
        task->dsp_init_vector = 0x10;
        task->init_cb = fn_80076EBC;
        task->res_cb = NULL;
        task->done_cb = fn_80076F98;
        task->req_cb = NULL;
        DSPAddTask(task);
    }
}
