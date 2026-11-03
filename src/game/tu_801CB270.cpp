// Unnamed unit (0x801CB270-0x801CDC8C). Written so far: the texture loader
// at its start and the random helpers fn_801CD580/fn_801CD664 (the latter is
// called from 52 units).

#include "game/GameGlobals.h"
#include "game/Player.h"
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

// A random integer in [0, 4) other than `except` (0-3, so at most three
// candidates remain).
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
