#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "dolphin/os.h"
#include "game/cls_804B1A38.h"

static cls_804B0FA0* lbl_8053AEE8;
static cls_804B0FC0* lbl_8053AEEC;

// Called each frame until both files are loaded.
void cls_804B1A38::fn_80442810() {
    if (field_0x4 == 0) {
        if (lbl_8053AEE8 == NULL) {
            lbl_8053AEE8 = fn_804019A8("/GC4Sword_usa/AudioRes/JaiInit.baa", 2, lbl_80529DEC.field_0x4);
            if (lbl_8053AEE8 == NULL) {
                return;
            }
        }
        if (lbl_8053AEEC == NULL) {
            lbl_8053AEEC = fn_80401E10("/GC4Sword_usa/AudioRes/Seqs/JaiSeqs.arc", 1, lbl_80529DEC.field_0x4);
            if (lbl_8053AEEC == NULL) {
                return;
            }
        }
        if (lbl_8053AEE8->field_0x4 == 1 && lbl_8053AEEC->field_0x4 == 1) {
            OSReport("audio data load end!!\n");
            lbl_80529DEC.field_0xc->fn_80459AE4(lbl_80529DEC.field_0x4, 0x800000,
                                                lbl_8053AEE8->field_0x14, lbl_8053AEEC->field_0x14);
            JKRHeap::free(lbl_8053AEE8->field_0x14, NULL);
            delete lbl_8053AEE8;
            delete lbl_8053AEEC;
            OSReport("auido init end!!\n");
            field_0x4 = 1;
        }
    }
}

cls_804B1A38::~cls_804B1A38() {}

cls_804B1A38::cls_804B1A38() {
    lbl_8053ADD8 = this;
    field_0x4 = 0;
    if (OSGetSoundMode() == 0) {
        lbl_8053AF80->fn_804594E4(0);
        lbl_80529DEC.field_0x245 = 0;
    } else {
        lbl_8053AF80->fn_804594E4(1);
        lbl_80529DEC.field_0x245 = 1;
    }
}
