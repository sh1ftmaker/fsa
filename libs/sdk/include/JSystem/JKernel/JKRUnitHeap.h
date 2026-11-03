#ifndef JKRUNITHEAP_H
#define JKRUNITHEAP_H

#include "JSystem/JKernel/JKRHeap.h"
#include "types.h"

// A heap of fixed-size units, with one bit per unit in a block allocation table (BAT).
class JKRUnitHeap : public JKRHeap
{
public:
    JKRUnitHeap(u8 *bat, u8 *units, u32 unitSize, u32 unitCount, u32 availableSize, u32 alignment,
                JKRHeap *parent, bool errorFlag);

    virtual ~JKRUnitHeap();                                           // _08
    virtual u32 getHeapType() { return 'UNIT'; }                      // _10
    virtual bool check();                                             // _14
    virtual bool dump();                                              // _1C
    virtual void do_destroy();                                        // _20
    virtual void *do_alloc(u32, int);                                 // _24
    virtual void do_free(void *);                                     // _28
    virtual void do_freeAll();                                        // _2C
    virtual void do_freeTail();                                       // _30
    virtual void do_fillFreeArea();                                   // _34
    virtual int do_resize(void *, u32);                               // _38
    virtual int do_getSize(void *);                                   // _3C
    virtual u32 do_getFreeSize();                                     // _40
    virtual void *do_getMaxFreeBlock();                               // _44
    virtual u32 do_getTotalFreeSize();                                // _48
    virtual void state_register(TState *, u32) const;                 // _54
    virtual bool state_compare(const TState &, const TState &) const; // _58

    static JKRUnitHeap *create(u32 unitSize, u32 size, u32 alignment, JKRHeap *parent, bool errorFlag);

    void clearBatArea();
    int find1FreeBlock(int direction);
    int findFreeBlock(int direction, u32 count);
    int findFreeBlock_fromHead(u32 count);
    int findFreeBlock_fromTail(u32 count);
    void *indexToAddress(int index);
    int addressToIndex(void *ptr);
    bool isUnitUsed(int index) const;
    void setUnitUsed(int index);

    // _00     = VTBL
    // _00-_6C = JKRHeap
    u32 mUnitSize;      // _6C
    u32 mUnitCount;     // _70
    u32 mAlignment;     // _74
    u8 *mBat;           // _78
    u8 *mUnits;         // _7C
    u32 mTotalFreeSize; // _80
    u8 mAllocMode;      // _84, Petari's name; 1: take the first fitting run (hides JKRHeap::mAllocMode)
};

#endif
