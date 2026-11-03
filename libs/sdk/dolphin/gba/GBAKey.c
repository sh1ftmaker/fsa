#include "dolphin/GBAPriv.h"
#include "string.h"

// The two DSP task callbacks have no known names (fn_ placeholders).

// GBA key-exchange DSP microcode (0x380 bytes at 0x804985D0). It is DSP code
// with no source form, so it isn't written here: the build extracts it from
// orig/sys/main.dol (config.yml extract:) into build/G4SE01/include/assets/.
static u8 ucode[0x380] ATTRIBUTE_ALIGN(32) = {
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

    gba->task.priority = 255;
    gba->task.iram_mmem_addr = (u16*)OSCachedToPhysical(ucode);
    gba->task.iram_length = sizeof(ucode);
    gba->task.iram_addr = 0;
    gba->task.dsp_init_vector = 0x10;
    gba->task.init_cb = fn_80076EBC;
    gba->task.res_cb = NULL;
    gba->task.done_cb = fn_80076F98;
    gba->task.req_cb = NULL;
    DSPAddTask(&gba->task);
}
