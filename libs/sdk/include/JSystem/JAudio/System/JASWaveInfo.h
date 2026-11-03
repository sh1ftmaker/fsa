#ifndef JAUDIO_JASWAVEINFO_H
#define JAUDIO_JASWAVEINFO_H

#include "types.h"

class JASWaveArc;

struct JASWaveInfo
{
    JASWaveInfo()
    {
        _01 = 60;
        _24 = &one;
    }

    u8 _00;         // 00
    u8 _01;         // 01
    f32 _04;        // 04
    int _08;        // 08
    int _0c;        // 0c
    u32 _10;        // 10
    u32 _14;        // 14
    int _18;        // 18
    int _1c;        // 1C
    s16 _20;        // 20
    s16 _22;        // 22
    const u32 *_24; // 24

    static u32 one;
};

class JASWaveHandle
{
public:
    virtual ~JASWaveHandle() {}
    virtual const JASWaveInfo *getWaveInfo() const = 0;
    virtual int getWavePtr() const = 0;
};

class JASWaveBank
{
public:
    JASWaveBank() {}
    virtual ~JASWaveBank() {}
    virtual JASWaveHandle *getWaveHandle(u32) const = 0;
    virtual JASWaveArc *getWaveArc(u32) = 0;
    virtual u32 getArcCount() const = 0;

    bool isLoading(); // 0x8009a704
};

#endif
