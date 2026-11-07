#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Unk_8045A080.h"
#include "JSystem/JUtility/JUTTexture.h"

// 0x801602E8
cls_804A03A8::cls_804A03A8(JKRHeap* heap) {
    heap->getTotalFreeSize();
    field_0x290 = new (heap, 0) JUTTexture(
        static_cast<const ResTIMG*>(gGameRoot_8053AAF8->field_0xa4->getResource('?\?\?\?', "circle.bti")), 0);
    field_0x294 = new (heap, 0) JUTTexture(
        static_cast<const ResTIMG*>(gGameRoot_8053AAF8->field_0xa4->getResource('?\?\?\?', "circle2.bti")), 0);
}

// 0x80160278
cls_804A03A8::~cls_804A03A8() {
    delete field_0x294;
    delete field_0x290;
}

// FIXME(shotgun): 0.0 %, missing/extra (the original stores the 32 zeros straight-line; ours loops)
// 0x801601E0
void cls_804A03A8::fn_801601E0() {
    for (s32 i = 0; i < 0x20; i++) {
        field_0x204[i] = 0;
    }
    field_0x284 = 0;
    field_0x288 = 0.0f;
    field_0x28c = 0.0f;
}

// FIXME(shotgun): 98.6 %, missing/extra + reloc-name
// 0x8016005C
void cls_804A03A8::fn_8016005C() {
    for (s32 i = 0; i < 0x20; i++) {
        if (field_0x204[i] > 0) {
            field_0x204[i]--;
        }
    }
    f32 old = field_0x288;
    if (field_0x284 != 0) {
        f32 value = old + field_0x28c;
        field_0x288 = value >= 1.0f ? 1.0f : value;
    } else {
        f32 value = old - field_0x28c;
        field_0x288 = value >= 0.0f ? value : 0.0f;
    }
    field_0x284 = 0;
    if (field_0x288 > 0.0f && old == 0.0f) {
        gAudioMgr_8053AF80->fn_80457E04(true);
    } else if (field_0x288 == 0.0f && old > 0.0f) {
        gAudioMgr_8053AF80->fn_80457E04(false);
    }
}

// FIXME(shotgun): 81.4 %, missing/extra
// 0x8015F8FC
bool cls_804A03A8::fn_8015F8FC(s32 layer, const JGeometry::TVec3f* pos, s32 mode) {
    bool placed = false;
    if (layer < 8) {
        for (s32 i = 0; i < 0x20; i++) {
            if (field_0x204[i] == 0) {
                field_0x4[i] = mode;
                field_0x84[i] = *pos;
                placed = true;
                switch (mode) {
                case 0:
                    field_0x204[i] = 0x1E;
                    break;
                case 1:
                    field_0x204[i] = 0x1E;
                    break;
                case 4:
                    field_0x204[i] = 0x73;
                    break;
                default:
                    field_0x204[i] = 1;
                    break;
                }
                break;
            }
        }
    }
    return placed;
}

// fn_8015F9C0 (0x69C) and fn_8015F454 (0x4A8), the two GX draw passes of cls_804A03A8, are not written yet.
