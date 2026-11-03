#ifndef JASBANK_H
#define JASBANK_H

#include "JSystem/JAudio/System/JASChannel.h"
#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JKernel/JKRHeap.h"

class JASInstParam;
class JASWaveBank;

class JASBank
{
public:
    JASBank() { mWaveBank = NULL; }
    virtual ~JASBank(){};
    virtual bool getInstParam(int, int, int, JASInstParam *) const = 0;
    virtual u32 getType() const = 0;

    JASWaveBank *getWaveBank() const { return mWaveBank; }
    void assignWaveBank(JASWaveBank *waveBank) { mWaveBank = waveBank; }

    static JASChannel *noteOn(const JASBank *pBank, int, u8, u8, u16, JASChannel::Callback channelCb, void *);
    static JASChannel *noteOnOsc(int, u8, u8, u16, JASChannel::Callback channelCb, void *);

private:
    JASWaveBank *mWaveBank; // 04
};

// FSA layout (tp/SMG): __vt__12JASBasicBank is dtor, getInstParam, getType;
// JASBNKParser news 0x3C8 bytes, and getInst (0x8002E2F0) returns NULL
// from index 0xF0 on.
class JASBasicBank : public JASBank
{
public:
    JASBasicBank();
    void newInstTable(u8, JKRHeap *);
    virtual bool getInstParam(int, int, int, JASInstParam *) const;
    void setInst(int prg_no, JASInst *inst);
    JASInst *getInst(int prg_no) const;
    virtual ~JASBasicBank() {}
    virtual u32 getType() const { return 0x42534943; } // BSIC


    static const int INST_MAX = 0xF0;
private:
    JASInst *mInstTable[INST_MAX]; // 08
};

#endif
