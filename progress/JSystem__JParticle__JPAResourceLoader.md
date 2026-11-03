# JSystem/JParticle/JPAResourceLoader.cpp

Baseline (progress.py first line at start): total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
After: total: 4323 / 11561 functions, 773500 / 4442088 bytes of code (17.41 %)

Status: Matching. 2/2 functions (was 0/2), .text 100 %. gate: ok (link SHA-1 OK).

## Changes
- symbols.txt:
  - `fn_800F3358` -> `__ct__17JPAResourceLoaderFPCUcP18JPAResourceManager`: checks the
    '2-10' version at +4 and calls load_jpc; JPAResourceManager's ctor calls it with (p_jpc, this);
    byte-identical to our 2-argument ctor.
  - `fn_800EAF38` -> `__ct__16JPADynamicsBlockFPCUc`: called from load_jpc's 'BEM1' case right
    after `new (heap, 0) JPADynamicsBlock` (our load_jpc calls that name there). It sits in
    JPADynamicsBlock.cpp's range (identical line also on branch unit/JSystem__JParticle__JPADynamicsBlock).
- Removed mkdd's "Fabricated for assert, doesn't match" bodies of the unused 3-argument ctor and
  load_jpa: FSA has neither function nor any of their strings (the unit has no data), and
  fabricated functions are not allowed. Their declarations stay in JPAResource.h.

## SHARED-EDITS
- the `fn_800EAF38` rename above (JPADynamicsBlock's range).
