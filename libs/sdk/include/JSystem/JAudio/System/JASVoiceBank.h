#ifndef JASVOICEBANK_H
#define JASVOICEBANK_H

#include "JSystem/JAudio/System/JASBank.h"

// Layout from FSA's JASVoiceBank vtable (0x80493670): ~JASVoiceBank,
// slot 2, getType ('VOIC'). In FSA slot 2 is getInstParam(int, int, int,
// JASInstParam *) const, as in tp; JASBank.h still carries mkdd's getInst(u32)
// in that slot, so it is overridden here under that name until JASBank.h is
// brought in line with FSA.
class JASVoiceBank : public JASBank
{
public:
    virtual ~JASVoiceBank();
    virtual JASInst *getInst(u32) const;
    virtual u32 getType() const;
};

#endif
