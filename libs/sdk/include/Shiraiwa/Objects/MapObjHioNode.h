#ifndef MAPOBJHIONODE_H
#define MAPOBJHIONODE_H

#include "Sato/GeographyObj.h"

class TMapObjHioNode : public GeographyObj
{
public:
    TMapObjHioNode(u32 id) : GeographyObj(id) {}
    TMapObjHioNode(const CrsData::SObject &rObj) : GeographyObj(rObj) {}
    virtual ~TMapObjHioNode() {}
};

class TMapObjStamper : public TMapObjHioNode {
public:
    TMapObjStamper(u32 id) : TMapObjHioNode(id) {}
    TMapObjStamper(const CrsData::SObject &rObj) : TMapObjHioNode(rObj) {}

    // TODO: not verified
    virtual ~TMapObjStamper() {}
    virtual void resetFrame(); // 80
    virtual void frameProc();  // 84
    virtual bool checkFrame(int kart_index) { return getReleaseFrame() < mStampFrame[kart_index]; } // 88
    virtual u32 getReleaseFrame() { return 0; } // 8c
    virtual void startStamp(int kart_index) { // 90
        if (mIsStamping[kart_index]) return;

        mIsStamping[kart_index] = true;
        mStampFrame[kart_index] = 0;
        GeographyObj::clrUserFlg1(kart_index);
    }
    virtual void endStamp(int kart_index) { // 94
        mIsStamping[kart_index] = false;
    }

private:
    u32 mStampFrame[8];
    bool mIsStamping[8];
}; // Size: 0x174

#endif
