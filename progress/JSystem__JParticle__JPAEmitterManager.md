# JSystem/JParticle/JPAEmitterManager.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4787 / 11561 functions, 864028 / 4442088 bytes of code (19.45 %)

Status: **Matching**. 16/16 functions (was 13/16); .text/.sdata2 100 %; `link_check.py --unit`
SHA-1 OK; gate ok.

## Changes
- symbols.txt:
  - fn_800EBEF8 -> calc__17JPAEmitterManagerFUc and fn_800EC2BC ->
    clearResourceManager__17JPAEmitterManagerFUc: same position in the file as tp's (between
    createSimpleEmitterID and draw / after entryResourceManager), bodies are tp's (calc walks
    pEmtrUseList[gid] calling JPAResource::calc; clearResourceManager compares
    getResourceManagerID at 0x113, inlined forceDeleteEmitter, then pResMgrAry[id] = NULL).
    pikmin2 has clearResourceManager__17JPAEmitterManagerFUc size 0xC8 = FSA's.
  - fn_800EB250 -> init__14JPABaseEmitterFP17JPAEmitterManagerP11JPAResource (JPAEmitter.cpp
    range; called by createSimpleEmitterID exactly where tp/mkdd/pikmin2 call
    JPABaseEmitter::init; size 0x350 vs tp 0x354 / mkdd 0x34C / pikmin2 0x35C).
  - lbl_8053BDB8 (placeholder) 0x8 -> 0x4 (one float 1.0; rest is padding).
- calc(u8) and clearResourceManager: tp's loop form (pNext declared before the for loop,
  `pLink = pNext` in the increment). The previous form had the emitter/next registers
  swapped; clearResourceManager also had a real bug (`next = link = link->getNext()`
  skipped the first emitter and dereferenced the end).

## SHARED-EDITS
- symbols.txt init__14JPABaseEmitterFP17JPAEmitterManagerP11JPAResource (placeholder in
  JPAEmitter.cpp's range).
