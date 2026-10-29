#ifndef JUGEMITEM_H
#define JUGEMITEM_H

#include "Sato/AnmController.h"
#include "Sato/GeographyObj.h"
#include "Shiraiwa/AnmPlayer.h"
#include "Shiraiwa/JugemRodItem.h"

class TJugemItem : public GeographyObj {
public:
    TJugemItem(u32 id) : GeographyObj(id), mJugemParentPtr(nullptr) {
        setObjFlagHidding();
    }
    virtual ~TJugemItem() {}
    virtual void calc() {} 
    virtual void setPosition(const JGeometry::TVec3f &pos) { mPos.set(pos); } // 80
    virtual void setRMtx(const JGeometry::TPos3f &rMtx) { mRotMtx.set(rMtx.mMtx);  } // 84
    virtual void hideModel(u32 p1) { mModel.clipAll(p1, false); } // 88
    virtual void show(u8) { mGeoObjFlag &= ~0x20; } // 8c
    virtual void hide() { mGeoObjFlag |= 0x20; } // 90
    virtual void hideAll() { hide(); } // 94
    virtual void hold(u32) {} // 98
    virtual void changeAnimation(u32 typre) // 9C TODO: OK?
    {
        if(mAnmCtrl == nullptr)
            return;
        
        if(mAnmCtrl->IsAvailableTrans())
            mAnmCtrl->StopTrans();
    }

public:
    int mJugemParentPtr;
    TAnmPlayer mJugemAnmPlayer;
};

class TJugemVoidRod : public TJugemItem {
public:
    TJugemVoidRod(); // 0x80291404
    ~TJugemVoidRod(); // 0x80291630
    virtual void reset(); // 0x8029175c
    virtual void loadAnimation(); // 0x802917e4
    virtual const char *getShadowBmdFileName(); // 0x80291890
    virtual const char *getBmdFileName(); // 0x80291898
    virtual void createModel(JKRSolidHeap *, u32); // 0x802918c0
    virtual void calc(); // 0x802919dc
    virtual void createColModel(J3DModelData *); // 0x80291a0c
    virtual void setRMtx(const JGeometry::TPos3f &rMtx); // 0x80291a2c
    virtual void setPosition(const JGeometry::TVec3f &pos); // 0x80291a10
    virtual void hideModel(u32); // 0x80291a60
    virtual void show(u8); // 0x80291ab4
    virtual void update(); // 0x80291b08
    virtual void hide(); // 0x80291b58
    virtual void hideAll(); // 0x80291b9c
    virtual void hold(u32); // 0x80291cb4
    virtual void changeAnimation(u32); // 0x80291d74
    
    void setJugemRodItem(TJugemRodItem *, u32); // 0x80291c34
    static bool nodeCallBack(J3DJoint *, int); // 0x80291ecc

    TJugemRodSignal *getSignal() const { return (TJugemRodSignal *)_188[2]; }

    static TAnmInfo sAnmInfos_Rod_Lap[4]; // 0x803a3ac0
    static TAnmInfo sAnmInfos_Rod_Start[5]; // 0x803a3b10
    static TAnmInfo sAnmInfos_Rod_Reverse[2]; // 0x803a3b74
    static TAnmInfo sAnmInfos_Rod_Rescue[3]; // 0x803a3b9c
    static TAnmInfo sAnmInfos_Rod_Signal[3]; // 0x803a3bd8
    static TAnmInfo sAnmInfos_Rod_PukuS[3]; // 0x803a3c14
    static TAnmInfo sAnmInfos_Rod_PukuL[3]; // 0x803a3c50
    static TJugemAnmTableEntry sAnmStateTable[4]; // 0x803a3c8c
    static TJugemAnmTableEntry sDemoAnmStateTable[3]; // 0x803a3cac

    //  Inline/Unused
    // void getPointPos(JGeometry::TVec3f *);

    AnmController *mRodAnmController[4];
    AnmController *mDemoRodAnmController[3];
    TJugemRodItem *_184;
    TJugemRodItem *_188[6];
    u8 _1a0[0x1a4 - 0x1a0]; // What is this?
    int _1a4;   // 1A4 - current model index?
}; // Size: 0x1a8

class TJugemFlag : public TJugemItem {
public:
    TJugemFlag();
    virtual ~TJugemFlag();
    virtual void reset();
    virtual void loadAnimation();
    virtual const char *getShadowBmdFileName();
    virtual const char *getBmdFileName();
    virtual void createModel(JKRSolidHeap *, u32);
    virtual void createColModel(J3DModelData *);
    virtual void changeAnimation(u32);

    static J3DAnmTransform *sJugemFlagBckAnmTrans;
    static J3DMtxCalc *sJugemFlagBckMtxCalc;
};

#endif
