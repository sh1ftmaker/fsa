#include "game/GameAudio.h"
#include "game/act/act_bti_npc.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "JSystem/JMath/JMath.h"

static const char* lbl_804A90F0[99] = {
    "d_iriguti_a.bti",
    "yaminosinden.bti",
    "koorinoshinden.bti",
    "honounotou.bti",
    "tenkuunotou.bti",
    "bg_opening.bti",
    "bg_tenkuu2.bti",
    "ana.bti",
    "d_iriguti_b.bti",
    "d_iriguti_d.bti",
    "d_iriguti_f.bti",
    "d_iriguti_g.bti",
    "d_iriguti_h.bti",
    "d_iriguti_i.bti",
    "d_iriguti_k.bti",
    "BTI_boss_d.bti",
    "BTI_boss_e.bti",
    "BTI_boss_f.bti",
    "BTI_boss_g.bti",
    "BTI_boss_h.bti",
    "BTI_boss_i.bti",
    "rouya.bti",
    "okujyo.bti",
    "nakaniwa_l.bti",
    "nakaniwa_r.bti",
    "hyrule_mamorigami.bti",
    "saidan.bti",
    "chijo_e.bti",
    "chijo_e.bti",
    "chijo_e.bti",
    "chijo_e.bti",
    "sekihi.bti",
    "sabaku_ganon.bti",
    "higasi_sinden.bti",
    "jen_8_3.bti",
    "jen_blue.bti",
    "jen_green.bti",
    "jen_purple.bti",
    "jen_red.bti",
    "phantom_ganon.bti",
    "iwart.bti",
    "jeek.bti",
    "phantom_ganon2.bti",
    "degtail.bti",
    "phantom_ganon1_2.bti",
    "big_poo.bti",
    "big_poo_2.bti",
    "ganon.bti",
    "gufu.bti",
    "tent.bti",
    "end_daiza.bti",
    "kairo.bti",
    "naporu_tent.bti",
    "okujyo_kabuse.bti",
    "jeek_kabuse.bti",
    "umaana_1_hidari.bti",
    "umaana_1_migi.bti",
    "umaana_1_mae.bti",
    "ana_1.bti",
    "ana_2.bti",
    "jeek_ki.bti",
    "kazari_piramid.bti",
    "piramid_s.bti",
    "kanban.bti",
    "sabaku_sinden_l.bti",
    "sabaku_sinden_s.bti",
    "kanban_hidari_b.bti",
    "kanban_migi_b.bti",
    "yaguraheya.bti",
    "egrass.bti",
    "ura_gate.bti",
    "jen_blue_koware.bti",
    "jen_green_koware.bti",
    "jen_purple_koware.bti",
    "jen_red_koware.bti",
    "jen_blue_tsume.bti",
    "jen_green_tsume.bti",
    "jen_purple_tsume.bti",
    "jen_red_tsume.bti",
    "hiroba.bti",
    "honoonotou_soto.bti",
    "d_iriguti_a_light.bti",
    "yaminosinden.bti",
    "koorinoshinden.bti",
    "honounotou.bti",
    "tenkuunotou.bti",
    "bg_opening.bti",
    "bg_tenkuu2.bti",
    "ana.bti",
    "d_iriguti_b_light.bti",
    "d_iriguti_d_light.bti",
    "d_iriguti_f_light.bti",
    "d_iriguti_g_light.bti",
    "d_iriguti_h_light.bti",
    "d_iriguti_i_light.bti",
    "d_iriguti_k_light.bti",
    "umaana_1_hidari_kabuse.bti",
    "umaana_1_migi_kabuse.bti",
    "umaana_1_mae_kabuse.bti",
};

static const s8 lbl_80540E28[8] = {
    0, 1, 2, -1, -2, -3, 0, 0,
};

// The rotation of game/RotMatrix.h's setRotScaleMtx, with the angles as s16
// read from JMath's sincos table and a scale per column.
static inline void setRotScaleMtxS16(Mtx m, s16 x, s16 y, s16 z, f32 scaleX, f32 scaleY, f32 scaleZ) {
    f32 sx = JMath::sincosTable_.sinShort(x);
    f32 sy = JMath::sincosTable_.sinShort(y);
    f32 sz = JMath::sincosTable_.sinShort(z);
    f32 cx = JMath::sincosTable_.cosShort(x);
    f32 cy = JMath::sincosTable_.cosShort(y);
    f32 cz = JMath::sincosTable_.cosShort(z);
    m[0][0] = scaleX * (cy * cz);
    m[1][0] = scaleX * (cy * sz);
    m[2][0] = scaleX * -sy;
    m[0][1] = scaleY * (sx * sy * cz - cx * sz);
    m[1][1] = scaleY * (sx * sy * sz + cx * cz);
    m[2][1] = scaleY * (sx * cy);
    m[0][2] = scaleZ * (cx * cz * sy + sx * sz);
    m[1][2] = scaleZ * (cx * sz * sy - sx * cz);
    m[2][2] = scaleZ * (cx * cy);
}

void BTINPC_804A927C::init() {
    field_0x234 = field_0x90 & 0xFFF;
    field_0x23c = (field_0x90 >> 12) & 0xF;
    field_0x238 = (field_0x90 >> 16) & 0xF;
    field_0x240 = (field_0x90 >> 20) & 0xF;
    field_0x244 = (field_0x90 >> 25) & 1;
    if (field_0x244 != 0) {
        field_0x259 = 1;
    } else {
        field_0x259 = 0;
    }
    if ((field_0x90 & 0x4000000) != 0) {
        field_0x258 = 1;
    } else {
        field_0x258 = 0;
    }
    if (field_0x240 == 0 || field_0x240 == 2 || field_0x240 == 4) {
        field_0x230 = 1;
    } else {
        field_0x230 = 0;
    }
    field_0x24c = 1.0f;
    field_0x250 = 1.0f;
    field_0x254 = 1.0f;
    field_0x248 = 0;
    if (field_0x234 == 0x1B) {
        field_0x24c = 1.5f;
        field_0x250 = 1.5f;
        field_0x254 = 1.5f;
        field_0x248 = 0x4000;
    } else if (field_0x234 == 0x1C) {
        field_0x24c = 1.5f;
        field_0x250 = 1.5f;
        field_0x254 = 1.5f;
        field_0x248 = -0x4000;
    } else if (field_0x234 == 0x1D) {
        field_0x24c = 1.5f;
        field_0x250 = 1.5f;
        field_0x254 = 1.5f;
        field_0x248 = -0x8000;
    } else if (field_0x234 == 0x1E) {
        field_0x24c = 1.5f;
        field_0x250 = 1.5f;
        field_0x254 = 1.5f;
        field_0x248 = 0;
    }
    update();
    field_0x11f = 1;
}

void BTINPC_804A927C::update() {
    JGeometry::TVec3f pos;
    switch (field_0x230) {
    case 0:
        if ((field_0x90 >> 27) == 0 || vfunc_0x34() == true) {
            if (field_0x258 != 0) {
                if (field_0x4 >= 8) {
                    pos.x = mPos.x + 8.0f;
                    pos.y = mPos.y + 8.0f;
                    pos.z = 0.0f;
                    getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4 + field_0x259, &pos, 0, -1, -1);
                } else {
                    spawnEmitter_8013CC50(field_0x4 + field_0x259, &mPos, 0x564, 0, 0, NULL, NULL, true, 0.5f);
                    spawnEmitter_8013CC50(field_0x4 + field_0x259, &mPos, 0x503, 1, 0, NULL, NULL, true, 1.0f);
                }
                startSound_801F0E34(0x30003, 0);
            }
            field_0x230 = 1;
        }
        break;
    case 1:
        if (field_0x240 == 2 || field_0x240 == 3) {
            if (!vfunc_0x34()) {
                if (field_0x258 != 0) {
                    if (field_0x4 >= 8) {
                        pos.x = mPos.x + 8.0f;
                        pos.y = mPos.y + 8.0f;
                        pos.z = 0.0f;
                        getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4 + field_0x259, &pos, 0, -1, -1);
                    } else {
                        spawnEmitter_8013CC50(field_0x4 + field_0x259, &mPos, 0x564, 0, 0, NULL, NULL, true, 0.5f);
                        spawnEmitter_8013CC50(field_0x4 + field_0x259, &mPos, 0x503, 1, 0, NULL, NULL, true, 1.0f);
                    }
                    startSound_801F0E34(0x30003, 0);
                }
                field_0x230 = 0;
                break;
            }
        }
        if (field_0x240 == 4) {
            if (vfunc_0x34() == true) {
                field_0x230 = 2;
            }
        }
        break;
    }
}

// FIXME: 91.39%, missing/extra + reloc-name: ours has 3 more instrs (490 vs 487). The original hoists lbl_804A90F0's base into r29 and schedules setRotScaleMtxS16's sincosTable_ products differently.
void BTINPC_804A927C::draw() {
    if (field_0x230 == 1) {
        Mtx mtx;
        SpriteParams_801F5AE0 params;
        params.field_0x1c = lbl_80540E28[field_0x238];
        params.field_0x1d = lbl_80540E28[field_0x23c];
        params.field_0x24 = 1;
        if (field_0x234 >= 0x1B && field_0x234 <= 0x1E) {
            PSMTXIdentity(mtx);
            setRotScaleMtxS16(mtx, 0, 0, field_0x248, field_0x24c, field_0x250, field_0x254);
            params.mMtx = mtx;
        } else if (field_0x234 == 0x15) {
            params.field_0x2d = 1;
        } else {
            params.field_0x21 = 1;
            params.field_0x23 = 1;
        }
        if (field_0x234 == 0x22) {
            if (vfunc_0x34() == true) {
                void* res = fn_804032E8('TIMG', "en_8_3_koware.bti");
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
                }
            } else {
                void* res = fn_804032E8('TIMG', lbl_804A90F0[field_0x234]);
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
                }
            }
        } else {
            void* res = fn_804032E8('TIMG', lbl_804A90F0[field_0x234]);
            if (field_0x4 + field_0x259 < 8) {
                gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
            }
            params.reset();
            params.field_0x1c = lbl_80540E28[field_0x238] + 1;
            params.field_0x1d = lbl_80540E28[field_0x23c];
            if (field_0x234 == 0 || (field_0x234 >= 8 && field_0x234 <= 0xE)) {
                params.field_0x34 = 1;
                params.field_0x10.a = 0x96;
                params.field_0x2c = 0;
                params.field_0x24 = 0;
                void* res2 = fn_804032E8('TIMG', (lbl_804A90F0 + 0x51)[field_0x234]);
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res2, &params);
                }
            } else if (field_0x234 == 0x15) {
                params.field_0x2d = 1;
                void* res2 = fn_804032E8('TIMG', "rouya_a.bti");
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res2, &params);
                }
            } else if (field_0x234 == 0x31) {
                void* res2 = fn_804032E8('TIMG', "tent_a.bti");
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res2, &params);
                }
            } else if (field_0x234 == 0x33) {
                params.field_0x24 = 1;
                void* res2 = fn_804032E8('TIMG', "kairo_ue.bti");
                JGeometry::TVec3f pos(mPos.x, mPos.y - 144.0f, 0.0f);
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, res2, &params);
                }
            } else if (field_0x234 == 0x34) {
                void* res2 = fn_804032E8('TIMG', "naporu_tent_a.bti");
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res2, &params);
                }
            } else if (field_0x234 >= 0x37 && field_0x234 <= 0x39) {
                params.field_0x2c = 0;
                params.field_0x24 = 1;
                void* res2 = fn_804032E8('TIMG', (lbl_804A90F0 + 0x60)[field_0x234 - 0x37]);
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res2, &params);
                }
            } else if (field_0x234 == 0x3E) {
                void* res2 = fn_804032E8('TIMG', "piramid_s_kabuse.bti");
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res2, &params);
                }
            } else if (field_0x234 == 0x42) {
                params.field_0x24 = 1;
                void* res2 = fn_804032E8('TIMG', "kanban_hidari_a.bti");
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res2, &params);
                }
            } else if (field_0x234 == 0x43) {
                params.field_0x24 = 1;
                void* res2 = fn_804032E8('TIMG', "kanban_migi_a.bti");
                if (field_0x4 + field_0x259 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res2, &params);
                }
            }
        }
    }
}

BTINPC_804A927C::~BTINPC_804A927C() {}
