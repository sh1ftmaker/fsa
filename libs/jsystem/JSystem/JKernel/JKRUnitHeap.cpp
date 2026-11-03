#include <JSystem/JKernel/JKRUnitHeap.h>
#include <JSystem/JUtility/JUTConsole.h>
#include <JSystem/JUtility/JUTAssert.h>

static const u8 bitTable[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

// FIXME: 92.77 %. The original computes ALIGN_NEXT(unitSize, alignment) as
// (alignment + unitSize) - 1 and keeps the mask in r31; ours reuses alignment - 1
// (add r0, r3, r0), which also shifts the callee-saved registers by one.
JKRUnitHeap *JKRUnitHeap::create(u32 unitSize, u32 size, u32 alignment, JKRHeap *parent, bool errorFlag)
{
    if (alignment == 0)
        alignment = 4;
    if (alignment > unitSize)
        unitSize = alignment;
    u32 alignedUnitSize = ALIGN_NEXT(unitSize, alignment);

    if (parent == nullptr)
        parent = sRootHeap;
    if (size == 0xffffffff)
        size = parent->getMaxAllocatableSize(8);
    if (size < sizeof(JKRUnitHeap))
        return nullptr;

    u8 *memory = (u8 *)JKRAllocFromHeap(parent, size, 0);
    if (memory == nullptr)
        return nullptr;

    u8 *bat = memory + sizeof(JKRUnitHeap);
    u32 unitCount = ALIGN_NEXT(((size - sizeof(JKRUnitHeap)) * 8) / (alignedUnitSize * 8 + 1), 8);
    if (unitCount == 0)
    {
        JKRHeap::free(memory, nullptr);
        return nullptr;
    }

    u8 *memoryEnd = memory + size;
    u8 *units = (u8 *)ALIGN_NEXT((u32)(bat + ALIGN_NEXT(unitCount / 8, 4)), alignment);
    unitCount = (memoryEnd - units) / alignedUnitSize;
    JKRUnitHeap *heap = new (memory)
        JKRUnitHeap(bat, units, alignedUnitSize, unitCount, memoryEnd - units, alignment, parent, errorFlag);
    if (heap == nullptr)
        JKRHeap::free(memory, nullptr);
    return heap;
}

void JKRUnitHeap::do_destroy()
{
    JKRHeap *parent = getParent();
    if (parent)
    {
        this->~JKRUnitHeap();
        JKRHeap::free(this, parent);
    }
}

JKRUnitHeap::JKRUnitHeap(u8 *bat, u8 *units, u32 unitSize, u32 unitCount, u32 availableSize, u32 alignment,
                         JKRHeap *parent, bool errorFlag)
    : JKRHeap(units, unitSize * unitCount, parent, errorFlag)
{
    mUnitSize = unitSize;
    mUnitCount = unitCount;
    mAlignment = alignment;
    mBat = bat;
    mUnits = units;
    mAllocMode = 1;
    clearBatArea();
    mTotalFreeSize = mUnitSize * mUnitCount;
}

JKRUnitHeap::~JKRUnitHeap()
{
    dispose();
}

void JKRUnitHeap::clearBatArea()
{
    u8 *bat = mBat;
    for (int i = 0; i < (int)mUnitCount / 8; i++)
        *bat++ = 0;

    if (mUnitCount & 7)
    {
        *bat = 0;
        for (int bit = mUnitCount & 7; bit < 8; bit++)
            *bat |= bitTable[bit];
        bat++;
    }

    while ((u32)bat & 3)
        *bat++ = 0xff;
}

void *JKRUnitHeap::do_alloc(u32 size, int direction)
{
    if (size <= mUnitSize)
    {
        int index = find1FreeBlock(direction);
        if (index < 0)
            return nullptr;
        setUnitUsed(index);
        mTotalFreeSize -= mUnitSize;
        return indexToAddress(index);
    }

    u32 count = (size + mUnitSize - 1) / mUnitSize;
    int index = findFreeBlock(direction, count);
    if (index < 0)
        return nullptr;

    u8 *bat = mBat + index / 8;
    for (int i = index; i < index + count; i++)
    {
        if ((i & 7) == 0)
            bat = mBat + i / 8;
        *bat |= bitTable[i & 7];
    }
    mTotalFreeSize -= mUnitSize * count;
    return indexToAddress(index);
}

int JKRUnitHeap::find1FreeBlock(int direction)
{
    if (direction >= 0)
    {
        u32 wordCount = (mUnitCount + 31) / 32;
        u32 word;
        u32 *words = (u32 *)mBat;
        for (u32 index = 0; index < wordCount; index++, words++)
        {
            word = *words;
            if (word != 0xffff)
            {
                for (int bit = 0; bit < 32; bit++)
                {
                    if (!(word & (0x80000000 >> bit)))
                        return index * 32 + bit;
                }
            }
        }
    }
    else
    {
        u32 wordCount = (mUnitCount + 31) / 32;
        u32 word;
        u32 *words = (u32 *)(mBat + ALIGN_NEXT(mUnitCount, 32) / 8) - 1;
        for (int index = wordCount - 1; index >= 0; index--, words--)
        {
            word = *words;
            if (word != 0xffff)
            {
                for (int bit = 31; bit >= 0; bit--)
                {
                    if (!(word & (0x80000000 >> bit)))
                        return index * 32 + bit;
                }
            }
        }
    }
    return -1;
}

int JKRUnitHeap::findFreeBlock(int direction, u32 count)
{
    if (count == 1)
        return find1FreeBlock(direction);
    if (direction >= 0)
        return findFreeBlock_fromHead(count);
    return findFreeBlock_fromTail(count);
}

int JKRUnitHeap::findFreeBlock_fromHead(u32 count)
{
    u32 *begin = (u32 *)mBat;
    bool inFreeRun = false;
    int runStart = -1;
    int runLength = 0xffff;
    int bestStart = -1;
    int bestLength = count == 0 ? 0 : 0xffff;
    u32 *end = begin + (mUnitCount + 31) / 32;

    for (u32 *words = begin; words <= end; words++)
    {
        u32 word = words != end ? *words : 0xffffffff;
        if (word == 0)
        {
            if (!inFreeRun)
            {
                runStart = (words - (u32 *)mBat) * 32;
                runLength = 0;
                inFreeRun = true;
            }
            runLength += 32;
            continue;
        }
        if (word == 0xffffffff && !inFreeRun)
            continue;

        for (int bit = 0; bit < 32; bit++)
        {
            if (!(word & (0x80000000 >> bit)))
            {
                if (!inFreeRun)
                {
                    runStart = (words - (u32 *)mBat) * 32 + bit;
                    runLength = 0;
                    inFreeRun = true;
                }
                runLength++;
            }
            else if (inFreeRun)
            {
                if (count == 0)
                {
                    if (runLength > bestLength)
                    {
                        bestStart = runStart;
                        bestLength = runLength;
                    }
                }
                else if (runLength == count)
                {
                    return runStart;
                }
                else if (runLength > count)
                {
                    if (mAllocMode == 1)
                        return runStart;
                    if (runLength < bestLength)
                    {
                        bestStart = runStart;
                        bestLength = runLength;
                    }
                }
                inFreeRun = false;
            }
        }
    }
    return count == 0 ? bestLength : bestStart;
}

int JKRUnitHeap::findFreeBlock_fromTail(u32 count)
{
    u32 *begin = (u32 *)mBat;
    u32 *end = begin - 1;
    bool inFreeRun = false;
    int runStart = -1;
    int runLength = 0xffff;
    int bestStart = -1;
    int bestLength = 0xffff;

    for (u32 *words = begin + (mUnitCount + 31) / 32 - 1; words >= end; words--)
    {
        u32 word = words != end ? *words : 0xffffffff;
        if (word == 0)
        {
            if (!inFreeRun)
            {
                runStart = (words - (u32 *)mBat) * 32 + 31;
                runLength = 0;
                inFreeRun = true;
            }
            runLength += 32;
            continue;
        }
        if (word == 0xffffffff && !inFreeRun)
            continue;

        for (int bit = 31; bit >= 0; bit--)
        {
            if (!(word & (0x80000000 >> bit)))
            {
                if (!inFreeRun)
                {
                    runStart = (words - (u32 *)mBat) * 32 + bit;
                    runLength = 0;
                    inFreeRun = true;
                }
                runLength++;
            }
            else if (inFreeRun)
            {
                if (runLength == count)
                    return runStart - runLength + 1;
                if (runLength > count)
                {
                    if (mAllocMode == 1)
                        return runStart - count + 1;
                    if (runLength < bestLength)
                    {
                        bestStart = runStart;
                        bestLength = runLength;
                    }
                }
                inFreeRun = false;
            }
        }
    }
    return bestStart == -1 ? -1 : bestStart - count + 1;
}

void JKRUnitHeap::do_free(void *ptr)
{
    int index = addressToIndex(ptr);
    if (index >= 0)
    {
        u8 *bat = &mBat[index / 8];
        u8 old = *bat;
        *bat = old & (bitTable[index & 7] ^ 0xff);
        if (old != *bat)
            mTotalFreeSize += mUnitSize;
    }
}

void JKRUnitHeap::do_freeAll()
{
    clearBatArea();
    mTotalFreeSize = mUnitSize * mUnitCount;
}

void JKRUnitHeap::do_freeTail() {}

void JKRUnitHeap::do_fillFreeArea() {}

int JKRUnitHeap::do_resize(void *, u32)
{
    return -1;
}

int JKRUnitHeap::do_getSize(void *ptr)
{
    int index = addressToIndex(ptr);
    if (isUnitUsed(index))
        return mUnitSize;
    return 0;
}

u32 JKRUnitHeap::do_getFreeSize()
{
    return mUnitSize * findFreeBlock_fromHead(0);
}

void *JKRUnitHeap::do_getMaxFreeBlock()
{
    int index = findFreeBlock_fromHead(0);
    return indexToAddress(index);
}

u32 JKRUnitHeap::do_getTotalFreeSize()
{
    return mTotalFreeSize;
}

void *JKRUnitHeap::indexToAddress(int index)
{
    if (index < 0 || index >= mUnitCount)
        return nullptr;
    return mUnits + mUnitSize * index;
}

int JKRUnitHeap::addressToIndex(void *ptr)
{
    int offset = (u8 *)ptr - mUnits;
    if (offset < 0)
        return -1;
    u32 index = offset / mUnitSize;
    if (offset != index * mUnitSize)
        return -1;
    return index < mUnitCount ? index : -1;
}

bool JKRUnitHeap::isUnitUsed(int index) const
{
    if (index < 0 || index >= mUnitCount)
        return false;
    u8 bits = bitTable[index & 7] & mBat[index / 8];
    return bits != 0;
}

// FIXME: 87.50 %. Only the clrlwi for index & 7 is scheduled two instructions
// earlier than in the original.
void JKRUnitHeap::setUnitUsed(int index)
{
    if (index < 0 || index >= mUnitCount)
        return;
    mBat[index / 8] |= bitTable[index & 7];
}

// FIXME: 97.33 %. Only the first bitTable load of the unrolled inner loop is
// scheduled before `li r4, bitTable` instead of after it.
bool JKRUnitHeap::check()
{
    u32 freeSize = 0;
    int byteCount = ((int)mUnitCount + 7) / 8;
    u32 bits;
    u8 *bat = mBat;
    for (int i = 0; i < byteCount; i++, bat++)
    {
        bits = *bat;
        if (bits == 0xff)
            continue;
        if (bits == 0)
        {
            freeSize += mUnitSize * 8;
        }
        else
        {
            for (int bit = 0; bit < 8; bit++)
            {
                if (!(bits & bitTable[bit]))
                    freeSize += mUnitSize;
            }
        }
    }

    if (freeSize != mTotalFreeSize)
        JUTWarningConsole_f("check: bad total memory block size (%08X, %08X)\n", mTotalFreeSize, freeSize);
    return true;
}

bool JKRUnitHeap::dump()
{
    bool result = check();
    lock();
    u32 usedSize = mHeapSize - mTotalFreeSize;
    JUTReportConsole_f("%d / %d bytes (%6.2f%%) used\n", usedSize, mHeapSize, 100.0f * ((f32)usedSize / (f32)mHeapSize));
    unlock();
    return result;
}

void JKRUnitHeap::state_register(TState *state, u32 id) const
{
    setState_u32ID_(state, id);
    setState_uUsedSize_(state, mHeapSize - const_cast<JKRUnitHeap *>(this)->getTotalFreeSize());
    u32 checkCode = 0;
    for (u32 *word = (u32 *)mBat; word < (u32 *)mUnits; word++)
        checkCode += *word;
    setState_u32CheckCode_(state, checkCode);
}

bool JKRUnitHeap::state_compare(const TState &r1, const TState &r2) const
{
    bool result = true;
    if (r1.getCheckCode() != r2.getCheckCode())
        result = false;
    if (r1.getUsedSize() != r2.getUsedSize())
        result = false;
    return result;
}
