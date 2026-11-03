// Unnamed unit (0x801CB270-0x801CDC8C). Written so far: the texture loader
// at its start and the random helpers fn_801CD580/fn_801CD664 (the latter is
// called from 52 units).

#include "game/GameAudio.h"

#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/ResTIMG.h"
#include "dolphin/gx.h"

// Looks up the TIMG resource `name` in the game archive and initialises
// texObj (and tlutObj for a palette texture, if given) from it. Returns the
// image's height / width.
// FIXME: 89.7 %. Same instructions apart from scheduling: the target computes
// the mipmap flag straight into r10 and orders the GXInitTexObjLOD argument
// loads differently. JUTTexture::initTexObj's forms (? GX_TRUE : GX_FALSE,
// image +=) and isMIPmapEnabled() were worse.
f32 fn_801CB270(const char* name, GXTexObj* texObj, GXTlutObj* tlutObj) {
    ResTIMG* timg = (ResTIMG*)lbl_8053AAF8->field_0xa4->getResource('TIMG', name);
    GXBool mipmap = timg->mIsMIPmapEnabled > 0;
    u32 imageOffset = timg->mImageDataOffset != 0 ? timg->mImageDataOffset : 0x20;
    void* image = (u8*)timg + imageOffset;

    if (timg->mPaletteFormat) {
        GXInitTexObjCI(texObj, image, timg->mSizeX, timg->mSizeY, (GXCITexFmt)timg->mTextureFormat,
                       (GXTexWrapMode)timg->mWrapS, (GXTexWrapMode)timg->mWrapT, mipmap, 0);
        if (tlutObj != NULL) {
            GXInitTlutObj(tlutObj, (u8*)timg + timg->mPaletteOffset, (GXTlutFmt)timg->mLutFormat,
                          timg->mPaletteEntryCount);
        }
    } else {
        GXInitTexObj(texObj, image, timg->mSizeX, timg->mSizeY, (GXTexFmt)timg->mTextureFormat,
                     (GXTexWrapMode)timg->mWrapS, (GXTexWrapMode)timg->mWrapT, mipmap);
    }
    GXInitTexObjLOD(texObj, (GXTexFilter)timg->mMinFilterType, (GXTexFilter)timg->mMagFilterType,
                    timg->mMinLOD * 0.125f, timg->mMaxLOD * 0.125f, timg->mLODBias * 0.01f,
                    timg->mIsBiasClamp, timg->mDoEdgeLOD, (GXAnisotropy)timg->mIsMaxAnisotropy);
    return timg->mSizeY * (1.0f / timg->mSizeX);
}

// The kind of a Force Gem ('RUPY', act_force_gem): the low byte of its
// spawn bits while field_0x94 is 0 or 3, else -1. The actor manager's
// queries (fn_801F7274, fn_801F7464, fn_801F750C) and tu_801DE714 filter
// actors with these three.
static inline s32 forceGemKind(ActorBase_804A5770* actor) {
    s32 kind = -1;
    if (actor->field_0x94 == 0 || actor->field_0x94 == 3) {
        kind = actor->field_0x90 & 0xFF;
    }
    return kind;
}

// Whether this is a live Bombchu ('BMTY'): its mFlags (0x230, past ActorBase)
// is not 3.
bool ActorBase_804A5770::fn_801CC8E4() {
    bool result = false;
    if (mActorId == ACTOR_BMTY) {
        if ((s32)static_cast<EnemyBase_804A4808*>(this)->mFlags != 3) {
            result = true;
        }
    }
    return result;
}

// Whether `actor` is a Force Gem of kind 8-11.
bool fn_801CC910(ActorBase_804A5770* actor) {
    bool result = false;
    if (actor->mActorId == ACTOR_RUPY) {
        s32 kind = forceGemKind(actor);
        if (kind >= 8 && kind <= 11) {
            result = true;
        }
    }
    return result;
}

// Whether `actor` is a Force Gem of kind 4-7.
bool fn_801CC960(ActorBase_804A5770* actor) {
    bool result = false;
    if (actor->mActorId == ACTOR_RUPY) {
        s32 kind = forceGemKind(actor);
        if (kind >= 4 && kind <= 7) {
            result = true;
        }
    }
    return result;
}

// Whether `actor` is a Force Gem with a kind (0 or more).
bool fn_801CC9B0(ActorBase_804A5770* actor) {
    bool result = false;
    if (actor->mActorId == ACTOR_RUPY) {
        s32 kind = forceGemKind(actor);
        if (kind >= 0) {
            result = true;
        }
    }
    return result;
}

// Whether the tile at pos is one of the codes 0x40-0x49.
bool fn_801CCC78(const JGeometry::TVec3f* pos, s32 layer) {
    bool result = false;
    switch (fn_80226998(pos, layer)) {
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
        result = true;
        break;
    }
    return result;
}

// fn_80236228 for the player that `player` stands for: the one
// fn_8022F3DC names while fn_8022F514 says it has one, else itself.
s32 fn_801CCF34(s32 player) {
    s32 target = player;
    if (fn_8022F514(player)) {
        target = fn_8022F3DC(player);
    }
    return fn_80236228(target);
}

// Whether every player's layer (fn_8023DE58) is `layer`.
bool fn_801CD228(s32 layer) {
    bool result = true;
    for (s32 i = 0; i < 4; i++) {
        if (layer != fn_8023DE58(i)) {
            result = false;
            break;
        }
    }
    return result;
}

// Whether any player's layer (fn_8023DE58) is `layer`.
bool fn_801CD290(s32 layer) {
    bool result = false;
    for (s32 i = 0; i < 4; i++) {
        if (layer == fn_8023DE58(i)) {
            result = true;
            break;
        }
    }
    return result;
}

// Whether `player` has no owner (fn_8022F514 is false) and fn_80303750 says yes.
extern "C" bool fn_801CD1D0(s32 player) {
    bool result = false;
    if (!fn_8022F514(player)) {
        if (fn_80303750(player)) {
            result = true;
        }
    }
    return result;
}

// Whether `layer` is one of the upper layers (8 and up).
bool fn_801CD2F8(s32 layer) {
    bool result = false;
    if (layer >= 8) {
        result = true;
    }
    return result;
}

// A random integer in [0, 4) other than `except` (0-3, so at most three
// candidates remain).
// Swaps the pairs 0<->1 and 2<->3; any other value maps to 0.
s32 fn_801CD354(s32 value) {
    s32 result = 0;
    switch (value) {
    case 0:
        result = 1;
        break;
    case 2:
        result = 3;
        break;
    case 3:
        result = 2;
        break;
    }
    return result;
}

// Maps 0 to 3, 1 to 2, 3 to 1; 2 and anything else to 0.
extern "C" s32 fn_801CD30C(s32 value) {
    s32 result = 0;
    switch (value) {
    case 0:
        result = 3;
        break;
    case 1:
        result = 2;
        break;
    case 3:
        result = 1;
        break;
    }
    return result;
}

s32 fn_801CD580(s32 except) {
    s32 choices[3];
    s32* out = choices;
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i != except) {
            *out++ = i;
            count++;
        }
    }
    return choices[(s32)(count * lbl_8053AAF8->mRandom.get_ufloat_1())];
}

// A random integer in [0, n).
s32 fn_801CD664(s32 n) {
    return n * lbl_8053AAF8->mRandom.get_ufloat_1();
}

// Makes the emitter immortal and then invalid (it stops creating particles and
// is deleted once they are gone). Does nothing for a NULL emitter.
void fn_801CB7A0(JPABaseEmitter* emitter) {
    if (emitter != NULL) {
        emitter->setStatus(JPAEMIT_Immortal);
        emitter->becomeInvalidEmitter();
    }
}

// Steps *angle towards target by at most step; the result of fn_801CD970 is
// not used.
void fn_801CD930(s16* angle, s32 target, s32 step) {
    fn_801CD970(angle, target, step);
}

// Moves *value toward target by `step`; nonzero while it has not reached the
// target (see EnemyBase.h).
s32 fn_801CD950(f32* value, f32 target, f32 step) {
    return fn_801CD9F0(value, target, step);
}
