#include "game/GameAudio.h"

#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/GameState.h"

extern const u16 lbl_8046AB2C[0x5E];
extern const u16 lbl_8046AAB0[12];
extern const u16 lbl_8046AAC8[10];
extern const u16 lbl_8046AADC[40];
extern const u16 lbl_8053FF48[2];
extern const u16 lbl_8053FF4C[2];

// Clamps a box-relative coordinate to the 9-bit cell range [0, 0x200).
static inline void clampCell(s32& v) {
    if (v < 0) {
        v = 0;
    } else if (v >= 0x200) {
        v = 0x1FF;
    }
}

template <class V>
static inline bool insideBox(const JGeometry::TBox2f& b, const V* p) {
    return p->x >= b.i.x && p->y >= b.i.y && p->x < b.f.x && p->y < b.f.y;
}

// Whether code is one of the first n entries of list.
static inline bool inList(u16 code, const u16* list, s32 n) {
    for (s32 i = 0; i < n; i++) {
        if (code == list[i]) {
            return true;
        }
    }
    return false;
}

// The raw tile code at pos in the layer's box (0xF outside), as fn_80226850 computes it.
template <class V>
static inline u16 rawTileAt(const V* pos, s32 layer) {
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* src = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    JGeometry::TBox2f box;
    box = *src;
    if (!insideBox(box, pos)) {
        return 0xF;
    }
    s32 x = (s32)(pos->x - box.i.x);
    s32 y = (s32)(pos->y - box.i.y);
    clampCell(x);
    clampCell(y);
    return gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, ((y & 0x100) << 1) | (x & 0x100) | (y & 0xF0) | ((x >> 4) & 0xF)) & 0x3FF;
}

// The tile's collision code: the raw tile code (as fn_80226850) through lbl_8046CB88, adjusted
// when the layer's GameView byte 0x131DF + layer is set (0 -> 2, 0x61 -> 0) and on stage (7, 2) /
// (7, 3) (0xE -> 0x51).
template <class V>
static inline u16 collisionCodeAt(const V* pos, s32 layer) {
    u16 raw = rawTileAt(pos, layer);
    u16 code = lbl_8046CB88[raw];
    if (getGameView_802D6A9C()->field_0x131df[layer] != 0) {
        if (code == 0) {
            return 2;
        }
        if (code == 0x61) {
            return 0;
        }
    } else if (code == 0xE && gGameRoot_8053AAF8->field_0x7f != 0) {
        if ((getGameView_802D6A9C()->field_0x4 == 7 && getGameView_802D6A9C()->field_0x8 == 2) ||
            (getGameView_802D6A9C()->field_0x4 == 7 && getGameView_802D6A9C()->field_0x8 == 3 &&
             getGameView_802D6A9C()->field_0x60[getGameView_802D6A9C()->field_0x18][getGameView_802D6A9C()->field_0x14] == 0x10 &&
             pos->y > 2470.0f)) {
            return 0x51;
        }
    }
    return code;
}

// The corner of cell `index` (the inverse of fn_80226AE0); z is cleared.
// FIXME(shotgun): 77.2 %, missing/extra
void fn_80226CA8(s32 index, s32 layer, JGeometry::TVec3f* out) {
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* src = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    JGeometry::TBox2f box;
    box = *src;
    s32 xOff = (index & 0x100) + ((index & 0xF) << 4);
    s32 yOff = ((index >> 1) & 0x100) + (index & 0xF0);
    JGeometry::TVec3f p;
    p.set(box.i.x + (f32)xOff, box.i.y + (f32)yOff, 0.0f);
    *out = p;
}

// The packed cell index of pos (no lookup; clamped like the queries, no outside test).
// The y term is written first to match the original's instruction order.
s32 fn_80226BC4(const JGeometry::TVec2f* pos, s32 layer) {
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* src = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
    JGeometry::TBox2f box;
    box = *src;
    s32 x = (s32)(pos->x - box.i.x);
    s32 y = (s32)(pos->y - box.i.y);
    clampCell(x);
    clampCell(y);
    return ((y & 0x100) << 1) | (x & 0x100) | (y & 0xF0) | ((x >> 4) & 0xF);
}

// The packed cell index of pos (as fn_80226BC4, on a Vec3).
// FIXME(shotgun): 95.3 %, scheduling+regalloc (forwards to fn_80226BC4)
s32 fn_80226AE0(const JGeometry::TVec3f* pos, s32 layer) {
    return fn_80226BC4((const JGeometry::TVec2f*)pos, layer);
}

// The same as fn_80226850 (only the register allocation differs in the original).
// FIXME(shotgun): 96.1 %, scheduling+regalloc
u16 fn_80226998(const JGeometry::TVec3f* pos, s32 layer) {
    return rawTileAt(pos, layer);
}

// The raw tile code at pos, 0xF outside the layer's box.
// FIXME(shotgun): 97 %, scheduling+regalloc
u16 fn_80226850(const JGeometry::TVec3f* pos, s32 layer) {
    return rawTileAt(pos, layer);
}

// The same result as fn_8022461C (compiled branch-free in the original).
// FIXME(shotgun): 83.6 %, missing/extra + reloc-name
u16 fn_802265FC(const JGeometry::TVec3f* pos, s32 layer) {
    return fn_8022461C(pos, layer);
}

bool fn_802265DC(const JGeometry::TVec3f* pos, s32 layer, bool flag) {
    return fn_802260DC((const JGeometry::TVec2f*)pos, layer, flag);
}

// Whether the tile at pos blocks, from its collision code (and the position inside the cell for
// the half-cell codes); `flag` selects further cases.
// FIXME(shotgun): 87.7 %, missing/extra
bool fn_802260DC(const JGeometry::TVec2f* pos, s32 layer, bool flag) {
    s32 code = collisionCodeAt(pos, layer);
    if (code == 0 || code == 2 || code == 0x50) {
        return false;
    }
    if (code == 0x22 && getGameView_802D6A9C()->field_0x170 == 0) {
        return false;
    }
    if (flag == 1 && (code == 0xC || code == 0x39)) {
        return false;
    }
    if (flag == 1 && (code == 3 || code == 0xE || code == 0x3A)) {
        return false;
    }
    if (flag == 1 && code >= 0x4C && code <= 0x4F) {
        return false;
    }
    if (code == 0x25) {
        return getGameView_802D6A9C()->field_0x171 == 0;
    }
    if (code == 0x26) {
        return getGameView_802D6A9C()->field_0x171 == 1;
    }
    // Half and quarter cells: which 8-unit part of the 16-unit cell is solid.
    if (code == 4) {
        return ((s32)pos->y & 0xF) < 8;
    }
    if (code == 5) {
        return ((s32)pos->y & 0xF) >= 8;
    }
    if (code == 6) {
        return ((s32)pos->x & 0xF) < 8;
    }
    if (code == 7) {
        return ((s32)pos->x & 0xF) >= 8;
    }
    if (code == 0x18) {
        return ((s32)pos->x & 0xF) < 8 && ((s32)pos->y & 0xF) < 8;
    }
    if (code == 0x19) {
        return ((s32)pos->x & 0xF) >= 8 && ((s32)pos->y & 0xF) < 8;
    }
    if (code == 0x1A) {
        return ((s32)pos->x & 0xF) < 8 && ((s32)pos->y & 0xF) >= 8;
    }
    if (code == 0x1B) {
        return ((s32)pos->x & 0xF) >= 8 && ((s32)pos->y & 0xF) >= 8;
    }
    return true;
}

// Whether the raw tile at pos is one of the codes of lbl_8046AAB0.
// FIXME(shotgun): 98 %, scheduling+regalloc
bool fn_80225EF4(const JGeometry::TVec3f* pos, s32 layer) {
    return inList(rawTileAt(pos, layer), lbl_8046AAB0, 12);
}

// Whether the collision code at pos is one of the first nine of lbl_8046AAC8.
// FIXME(shotgun): 89.8 %, missing/extra
bool fn_80225BE4(const JGeometry::TVec3f* pos, s32 layer) {
    return inList(collisionCodeAt(pos, layer), lbl_8046AAC8, 9);
}

// FIXME(shotgun): 89.8 %, missing/extra
bool fn_802258D4(const JGeometry::TVec3f* pos, s32 layer) {
    return inList(collisionCodeAt(pos, layer), lbl_8046AAC8, 9);
}

// Whether the collision code at pos is one of the first five of lbl_8046AADC.
// FIXME(shotgun): 88.7 %, missing/extra
bool fn_80225614(const JGeometry::TVec3f* pos, s32 layer) {
    return inList(collisionCodeAt(pos, layer), lbl_8046AADC, 5);
}

bool fn_802255F4(const JGeometry::TVec3f* pos, s32 layer) {
    return fn_802252F4(pos, layer);
}

// Tests the raw tile at pos against the code lists of the current stage (lbl_8046AADC holds the
// lists: entries 6.., 20.. and 34..).
// FIXME(shotgun): 89.6 %, missing/extra
bool fn_802252F4(const JGeometry::TVec3f* pos, s32 layer) {
    u16 raw = rawTileAt(pos, layer);
    if (gGameRoot_8053AAF8->field_0x7f != 0) {
        if (getGameView_802D6A9C()->field_0x4 == 7 && getGameView_802D6A9C()->field_0x8 == 0) {
            return inList(raw, lbl_8046AADC + 6, 13);
        }
        if (getGameView_802D6A9C()->field_0x4 == 7 && getGameView_802D6A9C()->field_0x8 == 2) {
            if (getGameView_802D6A9C()->field_0x131df[layer] != 0) {
                return inList(raw, lbl_8046AADC + 34, 5);
            }
            return inList(raw, lbl_8046AADC + 20, 13);
        }
        return false;
    }
    if (lbl_80529DEC.field_0x12c + (lbl_80529DEC.field_0x128 << 2) == 4 ||
        lbl_80529DEC.field_0x12c + (lbl_80529DEC.field_0x128 << 2) == 9) {
        return inList(raw, lbl_8046AADC + 6, 13);
    }
    return false;
}

// Whether the raw tile at pos is one of the two codes of lbl_8053FF48.
// FIXME(shotgun): 94 %, missing/extra
bool fn_8022517C(const JGeometry::TVec3f* pos, s32 layer) {
    u16 code = rawTileAt(pos, layer);
    bool result = false;
    if (code == lbl_8053FF48[0] || code == lbl_8053FF48[1]) {
        result = true;
    }
    return result;
}

// FIXME(shotgun): 90.4 %, missing/extra
bool fn_80225000(const JGeometry::TVec3f* pos, s32 layer) {
    u16 code = rawTileAt(pos, layer);
    if (code == lbl_8053FF48[0]) {
        return true;
    }
    if (code == lbl_8053FF48[1]) {
        return true;
    }
    return false;
}

// Whether the raw tile at pos is one of the two codes of lbl_8053FF4C.
// FIXME(shotgun): 94 %, missing/extra
bool fn_80224E88(const JGeometry::TVec3f* pos, s32 layer) {
    u16 code = rawTileAt(pos, layer);
    bool result = false;
    if (code == lbl_8053FF4C[0] || code == lbl_8053FF4C[1]) {
        result = true;
    }
    return result;
}

// FIXME(shotgun): 90.4 %, missing/extra
bool fn_80224D0C(const JGeometry::TVec3f* pos, s32 layer) {
    return inList(rawTileAt(pos, layer), lbl_8053FF4C, 2);
}

// On the upper layers, whether the tile is 0x1B0 / 0x1BB while GameView byte 0x131F1 is set.
// FIXME(shotgun): 97.4 %, scheduling+regalloc
bool fn_80224B88(const JGeometry::TVec3f* pos, s32 layer) {
    u16 code = rawTileAt(pos, layer);
    if (layer < 8 && getGameView_802D6A9C()->field_0x131f1 != 0 && (code == 0x1B0 || code == 0x1BB)) {
        return true;
    }
    return false;
}

// FIXME(shotgun): 97.4 %, scheduling+regalloc
bool fn_80224A04(const JGeometry::TVec3f* pos, s32 layer) {
    u16 code = rawTileAt(pos, layer);
    if (layer < 8 && getGameView_802D6A9C()->field_0x131f1 != 0 && (code == 0x1B0 || code == 0x1BB)) {
        return true;
    }
    return false;
}

// Whether the raw tile at pos is one of the 0x5E codes of lbl_8046AB2C (tile table in this
// unit's data, not in the source).
// FIXME(shotgun): 97.6 %, scheduling+regalloc
bool fn_8022486C(const JGeometry::TVec3f* pos, s32 layer) {
    return inList(rawTileAt(pos, layer), lbl_8046AB2C, 0x5E);
}

// The tile's collision code: the raw tile code (as fn_80226850) through lbl_8046CB88, adjusted
// when the layer's GameView byte 0x131DF + layer is set (0 -> 2, 0x61 -> 0) and on stage (7, 2) /
// (7, 3) (0xE -> 0x51).
// FIXME(shotgun): 87.2 %, missing/extra
u16 fn_8022461C(const JGeometry::TVec3f* pos, s32 layer) {
    return collisionCodeAt(pos, layer);
}
