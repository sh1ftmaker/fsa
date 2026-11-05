#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Map.h"
#include "game/THPMovie.h"
#include "game/cls_8016ED88.h"
#include "game/tu_803A3484.h"
#include "JSystem/J2D/J2DAnmLoader.h"
#include "JSystem/J2D/J2DPicture.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J3D/J3DAnmTevRegKey.h"
#include "JSystem/J3D/J3DAnmTextureSRTKey.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRFileLoader.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/Message.h"
#include <dolphin/os.h>

// The six cloud models the constructor loads from the title archive:
// bmd, btk and brk (NULL: not loaded).
static const char* lbl_804AF7D8[6][3] = {
    {"w_kumo.bmd", "w_kumo.btk", NULL},
    {"w_kumo1.bmd", "w_kumo1.btk", "w_kumo1.brk"},
    {"w_kumo2.bmd", "w_kumo2.btk", "w_kumo2.brk"},
    {"w_kumo3.bmd", "w_kumo3.btk", "w_kumo3.brk"},
    {"w_kumo4.bmd", "w_kumo4.btk", "w_kumo4.brk"},
    {"w_kumo5.bmd", NULL, NULL},
};

Title_804AF820* lbl_8053AD88;

s32 Title_804AF820::fn_803A3484() {
    if (lbl_8053AD88 != NULL) {
        return lbl_8053AD88->field_0x154;
    }
    return 1;
}

// Leaves the progressive-scan prompt: interlaced output, and the save region
// remembers it.
static inline void resetProgressive() {
    lbl_80529DEC.field_0x238 = 0xFF;
    lbl_80529DEC.mSaveRegion.f.field_0x10 = 1;
    OSSetProgressiveMode(OS_PROGRESSIVE_MODE_OFF);
}

// Creates an immortal effect `id` at the origin of the 2D layer.
static inline void createEmitter(JPABaseEmitter** emitter, u16 id) {
    if (*emitter == NULL) {
        JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
        *emitter = fn_8013CC50(0, &pos, id, 0, 0, NULL, NULL, true, 1.0f);
        (*emitter)->setStatus(JPAEMIT_Immortal);
    }
}

// Deletes the emitter once it may be deleted and has no particles left.
static inline void deleteFinishedEmitter(JPABaseEmitter** emitter) {
    if (*emitter != NULL && (*emitter)->isEnableDeleteEmitter()) {
        JPAEmitterManager* manager = fn_8013C824()->mEmitterManager;
        manager->forceDeleteEmitter(*emitter);
        *emitter = NULL;
    }
}

inline void Title_804AF820::stopMovie() {
    mMovie->quit_80427D94();
    if (mMovieState == 2) {
        field_0x58 = 0;
    } else {
        field_0x58++;
        if (field_0x58 >= 2) {
            field_0x58 = 0;
        }
    }
    mMovieState = 0;
    field_0x7a = 0;
    field_0x7e = 0;
    field_0x14d = 0;
    field_0x5c = 0;
    field_0x7c = 0;
    field_0x80 = 0;
    field_0x60 = 0;
    field_0x64 = 0;
    if (field_0x6e != 3 && lbl_8053AF80->field_0x58 != 0x1000006) {
        lbl_8053AF80->fn_80458FF0(0x1000006, false, false);
    }
}

// FIXME: 99.61. Scheduling only: the original adds 400 after `wait` +
// getFrameMax() (ours folds getFrameMax() + 400 first); the rest are .sdata2
// names (the original pool starts 0.5f, 120.0f, then 0.0f, 1.0f, the int
// conversion double, 8.5f) and the jump table's name.
void Title_804AF820::fn_803A34A0() {
    if (mMovieState != 0) {
        BOOL playing = mMovie->update_80427D60();
        bool done = false;
        if (mMovieState == 2) {
            s32 frame = mMovie->mGetTotalFrame() - mMovie->mGetRestFrame();
            if (field_0x80 != 0) {
                field_0x80++;
            }
            if (frame >= 150 && mMovie->mGetTotalFrame() > frame) {
                if (lbl_8053AF80->field_0x58 != 0x1000006) {
                    lbl_8053AF80->fn_80458FF0(0x1000006, false, false);
                    field_0x80++;
                }
                if (lbl_80529DEC.field_0x250 == 1) {
                    done = true;
                }
            }
            if (frame >= 200 && mMovie->mGetTotalFrame() > frame && field_0x44 == NULL) {
                JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
                field_0x44 = fn_8013CC50(0, &pos, 0x48E, 0, 0, NULL, NULL, true, 1.0f);
                field_0x44->setStatus(JPAEMIT_Immortal);
            }
            if (frame >= 201 && mMovie->mGetTotalFrame() > frame) {
                lbl_80529DEC.field_0x249[0] = 1;
                done = true;
                field_0x80 = 0;
            }
        } else if (mMovie->mGetRestFrame() == 0) {
            done = true;
        }
        if (lbl_8053AD30->fn_80309C4C(0x1100, 1) != 0) {
            if (mMovieState == 2) {
                lbl_80529DEC.field_0x249[0] = 1;
                if (field_0x6a < 120) {
                    field_0x6a = 0x99;
                }
            }
            stopMovie();
        } else if (done || lbl_8053ACA8->fn_802B1DCC() || !playing) {
            if (mMovieState == 2) {
                lbl_80529DEC.field_0x249[0] = 1;
                if (field_0x6a < 120) {
                    field_0x6a = 0x99;
                }
            }
            stopMovie();
        }
    } else {
        switch (field_0x6e) {
        case 0:
            field_0x4c += 8.5f;
            if (field_0x4c >= 255.0f) {
                field_0x4c = 255.0f;
                field_0x6e++;
            }
            break;
        case 1:
            if (lbl_80529DEC.field_0x238 == 4) {
                field_0x6e++;
            }
            break;
        case 2:
            field_0x4c -= 8.5f;
            if (field_0x4c <= 0.0f) {
                field_0x4c = 0.0f;
            }
            break;
        }
        GameState_804B0FE0& state = lbl_80529DEC;
        if (state.field_0x238 == 1) {
            if (SIProbe(0) != SI_GBA) {
                field_0x78++;
                if (field_0x78 >= 48) {
                    resetProgressive();
                }
            } else {
                field_0x78 = 0;
            }
            if (lbl_8053AD30->fn_80309A00(fn_8030AAF4(0)) == true) {
                field_0x76++;
                if (field_0x76 >= 48) {
                    if (OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON) {
                        state.field_0x238 = 2;
                    } else {
                        resetProgressive();
                    }
                    field_0x74 = 0;
                    field_0x76 = 0;
                }
            } else {
                field_0x76 = 0;
            }
            if (state.field_0x238 == 1 && lbl_8053AD30->fn_8030A308(fn_8030AAF4(0))) {
                if (field_0x74 < 3) {
                    field_0x74++;
                    if (OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON ||
                        lbl_8053AD30->fn_80309D24(fn_8030AAF4(0), 0x200) != 0) {
                        state.field_0x238 = 2;
                        field_0x74 = 0;
                    }
                } else {
                    resetProgressive();
                    field_0x74 = 0;
                }
            }
        }
        if (state.field_0x238 == 0xFF) {
            if (lbl_80529DEC.field_0x249[0] != 0) {
                if ((lbl_8053AD30->fn_8030A308(fn_8030AAF4(0)) || lbl_8053AD30->fn_8030A388(fn_8030AAF4(0))) &&
                    field_0x82 <= 0 && lbl_8053AD30->fn_80309C4C(0x1100, 1) != 0) {
                    lbl_8053AF80->fn_80458880(0xB, -1);
                    field_0x154 = 2;
                    lbl_8053AF80->fn_80458F9C(60);
                    lbl_80529DEC.field_0x1a0 = 60;
                    field_0x6e = 3;
                } else {
                    field_0x60++;
                    // lbl_8050DB0C's halfword 0x236: the attract movie's delay.
                    s16 delay = lbl_8050DB0C.field_0x4c[(0x236 - 0x4C) / 2];
                    if (field_0x60 == delay + 7440) {
                        lbl_8053AF80->fn_80458F9C(60);
                    } else if (field_0x60 >= delay + 7500) {
                        if (mMovie->start_80427DB4(field_0x58)) {
                            mMovieState = 1;
                        } else {
                            stopMovie();
                        }
                    }
                }
            } else if (mMovie->start_80427DB4(3)) {
                lbl_80529DEC.field_0x250 = 0;
                mMovieState = 2;
            }
        } else {
            switch (state.field_0x238) {
            case 0:
            case 1:
                break;
            case 2:
                field_0x50 += 8.5f;
                if (field_0x50 >= 255.0f) {
                    field_0x50 = 255.0f;
                    state.field_0x238 = 3;
                }
                break;
            case 3:
                field_0x74++;
                if (lbl_8053AD30->fn_80309D6C(fn_8030AAF4(0), 0x100) != 0 || field_0x74 >= 600) {
                    state.field_0x238 = 4;
                } else if (lbl_8053AD30->fn_80309D6C(fn_8030AAF4(0), 2) != 0) {
                    field_0x14c = 1;
                    field_0x72 = 30;
                } else if (lbl_8053AD30->fn_80309D6C(fn_8030AAF4(0), 1) != 0) {
                    field_0x14c = 0;
                    field_0x72 = 30;
                }
                field_0x72++;
                if (field_0x72 > 60) {
                    field_0x72 = 0;
                }
                break;
            case 4:
                if (field_0x6e == 2) {
                    field_0x50 -= 8.5f;
                    if (field_0x50 <= 0.0f) {
                        field_0x50 = 0.0f;
                        if (field_0x14c == 0) {
                            field_0x72 = 0;
                            if (OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON) {
                                state.field_0x238 = 6;
                                OSSetProgressiveMode(OS_PROGRESSIVE_MODE_ON);
                            } else {
                                state.field_0x238 = 5;
                                OSSetProgressiveMode(OS_PROGRESSIVE_MODE_ON);
                            }
                        } else if (OSGetProgressiveMode() == OS_PROGRESSIVE_MODE_ON) {
                            state.field_0x238 = 9;
                            field_0x6e = 0;
                            OSSetProgressiveMode(OS_PROGRESSIVE_MODE_OFF);
                        } else {
                            state.field_0x238 = 9;
                            field_0x6e = 0;
                        }
                    }
                }
                break;
            case 5:
                state.field_0x238 = 7;
                break;
            case 6:
                state.field_0x238 = 8;
                break;
            case 7:
            case 8:
                field_0x72++;
                if (lbl_80529DEC.field_0x251 > 90 && field_0x72 >= 90) {
                    state.field_0x238 = 9;
                    field_0x6e = 0;
                    lbl_80529DEC.field_0x251 = 0;
                }
                break;
            case 9:
                field_0x50 += 8.5f;
                if (field_0x50 >= 255.0f) {
                    field_0x50 = 255.0f;
                    state.field_0x238 = 10;
                    field_0x72 = 0;
                }
                break;
            case 10:
                field_0x72++;
                if (field_0x72 >= 120) {
                    field_0x50 = 120.0f;
                    state.field_0x238 = 11;
                }
                break;
            case 11:
                field_0x50 -= 8.5f;
                if (field_0x50 <= 0.0f) {
                    field_0x50 = 0.0f;
                    state.field_0x238 = 0xFF;
                    lbl_80529DEC.mSaveRegion.f.field_0x10 = 1;
                }
                break;
            }
        }
    }
    if (lbl_80529DEC.field_0x238 == 0xFF) {
        field_0x5c++;
        if (field_0x5c == 1) {
            createEmitter(&field_0x28[0], 0x493);
            createEmitter(&field_0x28[1], 0x494);
            createEmitter(&field_0x28[2], 0x495);
        }
        if (mMovieState != 0) {
            for (int i = 0; i < 3; i++) {
                deleteEmitter(&field_0x28[i]);
            }
            for (int i = 0; i < 4; i++) {
                deleteEmitter(&field_0x34[i]);
            }
        } else {
            if (field_0x5c >= 55) {
                s32 frame = field_0x5c - 55;
                if (frame == 0) {
                    createEmitter(&field_0x34[0], 0x490);
                    createEmitter(&field_0x34[1], 0x48F);
                    createEmitter(&field_0x34[2], 0x492);
                    createEmitter(&field_0x34[3], 0x491);
                }
                if (frame < mAnm->getFrameMax()) {
                    mAnm->setFrame(frame);
                } else {
                    // The original reads mAnm's frame count after drawing the random number.
                    s32 wait = (s32)(200.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
                    if (frame >= wait + mAnm->getFrameMax() + 400) {
                        field_0x5c = 0;
                    }
                    mAnm->setFrame(0.0f);
                }
            } else {
                mAnm->setFrame(0.0f);
            }
            mScreen->animation();
            for (int i = 0; i < 3; i++) {
                deleteFinishedEmitter(&field_0x28[i]);
            }
            for (int i = 0; i < 4; i++) {
                deleteFinishedEmitter(&field_0x34[i]);
            }
        }
    }
    deleteFinishedEmitter(&field_0x44);
    if (field_0x150 == 1) {
        field_0x6a++;
        if (field_0x6a >= 154) {
            field_0x68 -= 10;
            if (field_0x68 <= 0) {
                field_0x68 = 0;
                field_0x150 = 0;
            }
        }
    }
    lbl_80529DEC.field_0x250 = 0;
}

// FIXME: 99.78. Code identical; the string offsets differ by 0x48 (the
// original .rodata has an unreferenced 0x34-byte table and
// "enemy_both2_sob.bin" after "w_kumo5.bmd", presumably from the unwritten
// draw fn_803A44D8) and 0.0f's .sdata2 name.
void Title_804AF820::fn_803A5380() {
    if (lbl_80529DEC.field_0x242 == 0) {
        lbl_80529DEC.field_0x242 = 1;
        lbl_8053AB10->field_0x20->fn_802FF798();
        lbl_8053AB10->field_0x20->fn_802F9714(
            lbl_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_base_sch.szs"), 0, 0, 0, 0);
        lbl_8053AB10->field_0x20->fn_802F9714(
            lbl_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_tr_schE.szs"), 0, 0, 0x7000, 0);
        lbl_8053AB10->field_0x20->fn_802F93F4(
            lbl_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs"), 0, 0, 0, 0, false);
        lbl_8053AB10->field_0x20->fn_802F9714(
            lbl_8053AAF8->field_0xa4->getResource('SCH ', "player_sch.szs"), 0, 2, 0x4000, 0);
        lbl_8053AB10->field_0x20->fn_802F93F4(
            lbl_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs"), 0, 2, 0, 0, false);
        lbl_8053AB10->field_0x20->fn_802F9714(
            lbl_8053AAF8->field_0xa4->getResource('SCH ', "gals_sch.szs"), 0, 3, 0, 0);
        lbl_8053AB10->field_0x20->fn_802F93F4(
            lbl_8053AAF8->field_0xa4->getResource('SCL ', "cu_cl_obj_base_scl.szs"), 0, 3, 0, 0, false);
        lbl_8053AB10->field_0x20->field_0x2364 = 1;
    }
    field_0x154 = 1;
    field_0x14c = 0;
    field_0x50 = 0.0f;
    field_0x4c = 0.0f;
    field_0x6e = 0;
    field_0x72 = 0;
    field_0x7a = 0;
    field_0x7e = 0;
    field_0x14d = 0;
    field_0x74 = 0;
    field_0x76 = 0;
    field_0x78 = 0;
    field_0x60 = 0;
    field_0x64 = 0;
    mMovieState = 0;
    field_0x5c = 0;
    field_0x7c = 0;
    field_0x80 = 0;
    field_0x82 = 60;
    field_0x54 = 0;
    if (lbl_80529DEC.field_0x249[0] != 0) {
        field_0x68 = 0;
    } else {
        field_0x68 = 255;
    }
    field_0x6a = 0;
    field_0x150 = 0;
    field_0x84 = 120;
    field_0x88 = 60;
    field_0x8c = 1;
    field_0x90.fn_8016ED88();
    if (lbl_80529DEC.field_0x249[0] != 0) {
        if (lbl_8053AF80->field_0x58 != 0x1000006) {
            lbl_8053AF80->fn_80458FF0(0x1000006, false, false);
        }
        field_0x82 = 60;
    }
}

Title_804AF820::~Title_804AF820() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    for (int i = 0; i < 4; i++) {
        if (field_0x90.mEntries[i].field_0x18 != NULL) {
            delete field_0x90.mEntries[i].field_0x18;
        }
    }
    if (mMovieState != 0) {
        mMovie->quit_80427D94();
        mMovieState = 0;
    }
    for (int i = 0; i < 3; i++) {
        deleteEmitter(&field_0x28[i]);
    }
    for (int i = 0; i < 4; i++) {
        deleteEmitter(&field_0x34[i]);
    }
    deleteEmitter(&field_0x44);
    fn_8013C824()->mEmitterManager->forceDeleteAllEmitter();
    mHeap->destroy();
    delete mScreen;
    mScreen = NULL;
    delete mAnm;
    mAnm = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete mMovie;
    mMovie = NULL;
    delete field_0xc;
    field_0xc = NULL;
    delete field_0x10;
    field_0x10 = NULL;
    delete field_0x14;
    field_0x14 = NULL;
    delete field_0x18;
    field_0x18 = NULL;
    delete field_0x1c;
    field_0x1c = NULL;
    delete field_0x20;
    field_0x20 = NULL;
    lbl_80529DEC.field_0x24->removeResourceAll();
    lbl_8053AD88 = NULL;
}

// FIXME: 99.78. Code identical; string offsets and 0.0f's .sdata2 name differ
// as in fn_803A5380.
Title_804AF820::Title_804AF820() {
    lbl_80529DEC.mHeap->getTotalFreeSize();
    lbl_8053AD88 = this;
    mScreen = new J2DScreen;
    mScreen->setPriority("title.blo", 0x1100000, lbl_80529DEC.field_0x24);
    mAnm = static_cast<J2DAnmTextureSRTKey*>(
        J2DAnmLoaderDataBase::load(JKRFileLoader::getGlbResource("title.btk", lbl_80529DEC.field_0x24)));
    mScreen->setAnimation(mAnm);
    mScreen->search('Pcmark')->hide();
    field_0x14e = mScreen->search('prsstrt1')->getAlpha();
    field_0x14f = mScreen->search('prsstrt2')->getAlpha();
    const ResTIMG* tex = (const ResTIMG*)fn_80403204('TIMG', "kk_titleCon_01.bti");
    field_0x8 = new J2DPicture(tex);
    mMovie = new THPMovie_804B15D0;
    tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_choice.bti");
    field_0xc = new J2DPicture(tex);
    tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_inter.bti");
    field_0x10 = new J2DPicture(tex);
    tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_pro.bti");
    field_0x14 = new J2DPicture(tex);
    tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_yes.bti");
    field_0x18 = new J2DPicture(tex);
    tex = (const ResTIMG*)fn_80403244('TIMG', "progressive_no.bti");
    field_0x1c = new J2DPicture(tex);
    tex = (const ResTIMG*)fn_804030C4('TIMG', "kk_black.bti");
    field_0x20 = new J2DPicture(tex);
    field_0x20->setBlackWhite(JUtility::TColor(8, 24, 40, 255), JUtility::TColor(0, 0, 0, 0));
    if (lbl_80529DEC.field_0x249[0] != 0) {
        field_0x68 = 0;
    } else {
        field_0x68 = 255;
    }
    field_0x150 = 0;
    JKRHeap* parent = JKRHeap::getCurrentHeap();
    mHeap = JKRSolidHeap::create(0x80000, parent, false);
    mHeap->becomeCurrentHeap();
    for (int i = 0; i < 6; i++) {
        fn_8016A3BC(lbl_80529DEC.field_0x24, lbl_804AF7D8[i][0], lbl_804AF7D8[i][1], NULL, lbl_804AF7D8[i][2],
                    NULL, 0x10240000, mHeap, &field_0x104[i], &field_0x11c[i], NULL, &field_0x134[i], NULL);
        if (field_0x11c[i] != NULL) {
            field_0x11c[i]->setFrame(0.0f);
        }
        if (field_0x134[i] != NULL) {
            field_0x134[i]->setFrame(0.0f);
        }
    }
    mHeap->adjustSize();
    parent->becomeCurrentHeap();
    for (int i = 0; i < 4; i++) {
        field_0x90.mEntries[i].field_0x18 = new ActorSoundObj_804578F4(NULL, NULL, 8);
    }
    field_0x14c = 0;
    field_0x50 = 0.0f;
    field_0x4c = 0.0f;
    field_0x6e = 0;
    field_0x72 = 0;
    field_0x7a = 0;
    field_0x7e = 0;
    field_0x14d = 0;
    field_0x74 = 0;
    field_0x76 = 0;
    field_0x78 = 0;
    field_0x58 = 0;
    field_0x60 = 0;
    field_0x64 = 0;
    mMovieState = 0;
    field_0x5c = 0;
    field_0x7c = 0;
    field_0x80 = 0;
    field_0x82 = 60;
    field_0x28[0] = NULL;
    field_0x28[1] = NULL;
    field_0x28[2] = NULL;
    field_0x34[0] = NULL;
    field_0x34[1] = NULL;
    field_0x34[2] = NULL;
    field_0x34[3] = NULL;
    field_0x44 = NULL;
    lbl_8053AB10->field_0x20->field_0x2364 = 1;
    lbl_80529DEC.field_0x250 = 0;
}
