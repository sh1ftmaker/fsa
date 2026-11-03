# JSystem/JAudio/Utility/JAUSoundTable.cpp

Baseline (progress.py): total 4784 / 11561 functions, 863464 / 4442088 bytes (19.44 %).
After: total 4785 / 11561 functions, 863844 bytes (19.45 %). gate: ok.

Unit: 4/6 -> 5/5 functions; .text, .ctors, .bss 100 %. `link_check --unit` SHA-1 OK;
flipped to Matching.

## Matched
- fn_8003C618 = `__sinit_JAUSoundTable_cpp` (.ctors entry; mkdd's
  __sinit_JAUSoundTable_cpp has the same size 0x17C). It builds six pools:
  TChannelMgr, JASTrack, JASChannel, JAIStream, JAISeq, JAISe. mkdd got the JAI ones
  from an #ifdef MATCHING memPool_ header (retired here); with the generic memPool_
  definitions, including JAISe.h, JAISeq.h, JAIStream.h (in that order; the
  opposite order reverses the JAISe/JAIStream pools) gives FSA's sequence.
- .bss 0x804B4BE0-0x804B4C28 split into this unit (register chain); placeholder
  lbl_804B4BE0 (0x48) split into six 0xC placeholders.
- getNumGroups_inSection / getNumItems_inGroup are dead-stripped in FSA.

## Resistant
None.

## SHARED-EDITS
- fn_8003C794 (0xC, `li r0,0; stw r0,0(r3)`) is `JAUStreamFileTable::JAUStreamFileTable()`
  (`__ct__18JAUStreamFileTableFv`): JAUSectionHeap calls it on a stack object right
  before init__18JAUStreamFileTableFPCv, and mkdd's map has the ctor (0xC) as the
  first function of JAUStreamFileTable.cpp right after __sinit_JAUSoundTable_cpp.
  I ended JAUSoundTable's .text at 0x8003C794. The coordinator should start
  JAUStreamFileTable.cpp's .text at 0x8003C794 (it's Matching; its source already
  has the ctor, which the link currently dead-strips as unreferenced) and rename
  fn_8003C794 -> `__ct__18JAUStreamFileTableFv` in the same change (doing only the
  rename would duplicate the symbol in the link).
