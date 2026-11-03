#ifndef JASVOICEBANK_H
#define JASVOICEBANK_H

#include "JSystem/JAudio/System/JASBank.h"

// Layout from FSA's JASVoiceBank vtable (0x80493670): ~JASVoiceBank,
// getInstParam(int, int, int, JASInstParam *) const, getType ('VOIC'), as in
// tp.
class JASVoiceBank : public JASBank
{
public:
    virtual ~JASVoiceBank();
    virtual bool getInstParam(int, int, int, JASInstParam *) const;
    virtual u32 getType() const;
};

#endif
