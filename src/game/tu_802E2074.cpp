#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "game/GameState.h"
#include "game/act/tu_802E2074.h"
#include <stdio.h>

// Actor serials, 0x50 slots of the 0x90-word table at 0x804D4768 (the rest
// of the 0x240-byte bss range is not touched by this unit).
s32 lbl_804D4768[0x90];
// Frame counter of the update; 0 after init.
s32 lbl_8053ACB0;

// FIXME: 96.17. Structurally close: the target reads field_0x234 before the getGameView
// call of the tile lookup (field_0x238 after it, as if the column came through an inlined
// accessor) and copies the tile byte from r0 to r9 after the 0xFF test; the format string
// waits for the unit's .rodata split (lbl_8046E9F8). The archive type is 'B   ' plus the
// stage name's chars less ' ', added after the sprintf (GameView fn_802D8328's form).
void cls_804A9BC8::init() {
    for (int i = 0; i < 0x50; i++) {
        lbl_804D4768[i] = -1;
    }
    lbl_8053ACB0 = 0;
    field_0x234 = (field_0x90 >> 23) & 0xF;
    field_0x238 = (field_0x90 >> 19) & 0xF;

    s32 stage = getGameView_802D6A9C()->field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    const char* name = getGameView_802D6A9C()->fn_802D9C7C(stage, view->field_0x8);
    s32 tile = getGameView_802D6A9C()->field_0x60[field_0x238][field_0x234];
    if (tile == 0xFF) {
        tile = 0;
    }
    char buf[0x20];
    u32 type = 'B   ';
    sprintf(buf, "d_enemy_map%s_%d%d.bin", name, tile / 10, tile % 10);
    type += ((name[0] - ' ') << 16) + ((name[1] - ' ') << 8) + (name[2] - ' ');
    field_0x244 = (u8*)fn_804032E8(type, buf);
    field_0x248 = 0;
    field_0x23c = (field_0x90 >> 16) & 7;
    field_0x24c = 0;
    field_0x24e = 8;
    field_0x240 = 0;
    field_0x230 = 2;
}

// The enemy map's rows: u16 offsets per direction (0x28.., 6/4/2/0, ...), in units of 8.
extern const u16 lbl_8046E9D8[4][4]; // FIXME(shotgun): not defined in the source yet (unit .rodata)

// FIXME(shotgun): 76.8 %, missing/extra + reloc-name (loop structure / rodata table extern)
void cls_804A9BC8::update() {
    s32 layer = field_0x4;
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* src = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    JGeometry::TBox2f box = *src;
    s32 offset = 0;
    bool done = false;
    switch (field_0x230) {
    case 0:
        if (lbl_8053ACB0 != 0) {
            for (int i = 0; i < 0x50; i++) {
                s32 serial = lbl_804D4768[i];
                if (serial >= 0 && getActorManager_801F856C()->findActorBySerial_801F666C(serial) == NULL) {
                    lbl_804D4768[i] = -1;
                    lbl_8053ACB0--;
                    if (lbl_8053ACB0 == 0) {
                        ActorBase_804A5770* list[ACTOR_MANAGER_MAX_ACTORS];
                        s32 count = getActorManager_801F856C()->fn_801F7818('GEN2', field_0x4, list, -1);
                        for (s32 k = 0; k < count; k++) {
                            if (list[k] != NULL) {
                                static_cast<cls_804A9BC8*>(list[k])->field_0x230 = 1;
                            }
                        }
                        field_0x230 = 1;
                        return;
                    }
                }
            }
            break;
        }
        field_0x230 = 1;
        // fall through
    case 1:
        field_0x240 = 0;
        for (;;) {
            const u8* entry = field_0x244 + offset;
            u32 tag = (entry[0] << 24) | (entry[1] << 16) | (entry[2] << 8) | entry[3];
            bool go = true;
            if (tag == 0x20202020) {
                field_0x24c += 2;
                offset = 0;
                go = false;
                field_0x240++;
                if (field_0x24c != field_0x24e) {
                    continue;
                }
                done = true;
                if (field_0x24c == 0x40) {
                    fn_801F3668();
                } else {
                    field_0x24e += 8;
                }
            } else if (field_0x23c < 2) {
                if (field_0x24c != entry[6]) {
                    offset += 0xB;
                    continue;
                }
            } else {
                if (field_0x24c != entry[5]) {
                    offset += 0xB;
                    continue;
                }
            }
            if (go == true) {
                entry = field_0x244 + offset;
                offset += 0xB;
                u32 arg = (entry[7] << 24) | (entry[8] << 16) | (entry[9] << 8) | entry[10];
                f32 originX;
                f32 originY;
                s32 spawnLayer;
                if (entry[4] == 0) {
                    originY = box.i.y;
                    originX = box.i.x;
                    spawnLayer = 0;
                } else {
                    originY = 0.0f;
                    spawnLayer = entry[4] + 7;
                    originX = originY;
                }
                JGeometry::TVec3f pos;
                if (field_0x23c < 2) {
                    pos.y = 8.0f * (f32)lbl_8046E9D8[field_0x23c][field_0x240] + originY;
                    pos.x = 8.0f * (f32)(u16)entry[5] + originX;
                } else {
                    pos.x = 8.0f * (f32)lbl_8046E9D8[field_0x23c][field_0x240] + originX;
                    pos.y = 8.0f * (f32)(u16)entry[6] + originY;
                }
                pos.z = 0.0f;
                lbl_804D4768[lbl_8053ACB0] = getActorManager_801F856C()->spawnActor_801F9484(tag, spawnLayer, &pos, arg, 0, -1);
                if (lbl_804D4768[lbl_8053ACB0] >= 0) {
                    lbl_8053ACB0++;
                    if (lbl_8053ACB0 >= 0x50) {
                        fn_801F3668();
                        field_0x230 = 0;
                        break;
                    }
                }
                if (done != true) {
                    continue;
                }
            }
            field_0x230 = 0;
            break;
        }
        break;
    case 2:
        if ((field_0x90 >> 27) == 0 || vfunc_0x34() == true) {
            field_0x230 = 1;
        }
        break;
    }
}

void cls_804A9BC8::draw() {}

cls_804A9BC8::~cls_804A9BC8() {}
