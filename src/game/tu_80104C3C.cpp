#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/Camera.h"
#include "game/act/tu_80104C3C.h"

cls_8049CB58::~cls_8049CB58() {}

void cls_8049CB58::draw() {}

void cls_8049CB58::update() {
    if (field_0x24c) {
        field_0x24d++;
        if (field_0x24d == 0x32) {
            fn_801F0D20(3);
            field_0x24c = 0;
        }
    }
    if (field_0x24e != 1) {
        s32 count = 0;
        s32 ready = 0;
        switch (field_0x240) {
        case 0:
            for (s32 i = 0; i < 4; i++) {
                if (getGameView_802D6A9C()->fn_802D800C(field_0x230[i]) || field_0x230[i] == 0) {
                    count++;
                }
            }
            if (count == 4) {
                ready = 1;
            }
            break;
        case 1:
            for (s32 i = 0; i < 4; i++) {
                if (getGameView_802D6A9C()->fn_802D800C(field_0x230[i])) {
                    count++;
                }
            }
            if (count != 0) {
                ready = 1;
            }
            break;
        case 2:
            for (s32 i = 0; i < 4; i++) {
                if (getGameView_802D6A9C()->fn_802D800C(field_0x230[i])) {
                    count++;
                }
            }
            if (count != 0 && count != 4) {
                ready = 1;
            }
            break;
        }
        if (ready) {
            if (fn_80104EB0()) {
                vfunc_0x30(0xFF, true);
                if (field_0x90 & 0x20) {
                    field_0x24c = 1;
                }
                field_0x24d = 0;
                if (field_0x244) {
                    field_0x24e = 1;
                }
            }
        } else {
            vfunc_0x30(0xFF, false);
            field_0x24c = 0;
            if (field_0x24f) {
                field_0x24f = 0;
                field_0x24a = 0;
                fn_80230534(false);
            }
        }
    }
}

bool cls_8049CB58::fn_80104EB0() {
    if (!(field_0x90 & 0x40)) {
        return true;
    }
    switch (field_0x24f) {
    case 0:
        if (fn_8023077C()) {
            field_0x24f++;
            field_0x248 = 30;
        }
        break;
    case 1:
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i)) {
                gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(fn_8023E724(i));
            }
        }
        field_0x248--;
        if (field_0x248 == 0) {
            field_0x24a++;
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i)) {
                    fn_80230498(i, fn_8023E724(i), field_0x24a + 0xDB, 0xFF, -0x80, -0x80, true);
                    if (field_0x24a == 1) {
                        fn_80230FD0(i, 0x10001);
                    }
                }
            }
            field_0x248 = 2;
            if (field_0x24a == 0xD) {
                field_0x24f++;
                field_0x248 = 50;
            }
        }
        break;
    case 2:
        field_0x248--;
        if (field_0x248 == 0) {
            field_0x24f = 0;
            field_0x24a = 0;
            fn_80230534(false);
            return true;
        }
        break;
    }
    return false;
}

void cls_8049CB58::init() {
    static const s32 lbl_80463EE0[4] = {22, 17, 12, 7};
    for (s32 i = 0; i < 4; i++) {
        field_0x230[i] = (field_0x90 >> lbl_80463EE0[i]) & 0x1F;
    }
    field_0x240 = field_0x90 & 0xF;
    field_0x244 = field_0x90 & 0x10;
    field_0x24c = 0;
    field_0x24d = 0;
    field_0x24f = 0;
    field_0x24a = 0;
    field_0x248 = 0;
    if (field_0x244) {
        if (vfunc_0x34()) {
            field_0x24e = 1;
        }
    }
}
