# JSystem/JAudio/Utility/JAUSectionHeap.cpp


**Update (infra/jaudio-interface-sym): Matching.** With JAudio/Interface and Utility built without `-sym on` (provisional, configure.py) `link_check.py --unit` gives SHA-1 OK; with it on the trial link differs. The notes below describe the earlier state.
Baseline (progress.py): total 4784 / 11561 functions, 863464 / 4442088 bytes (19.44 %).
After: total 4785 / 11561 functions, 863732 bytes (19.44 %). gate: ok.

Unit: 36/38 -> 37/38 functions; .text 99.99 %, .ctors/.rodata/.data/.bss/.sdata 100 %.
Still NonMatching: newStreamFileTable is blocked by JAUStreamFileTable.cpp's split.

## Matched
- fn_8003ED08 = `__sinit_JAUSectionHeap_cpp` (.ctors entry; TChannelMgr/JASTrack/
  JASChannel/JAISe pools). Placeholder lbl_804B4C98 (0x30, the unit's register
  chain, already in the .bss split) split into four 0xC placeholders.

## Resistant
- newStreamFileTable__10JAUSectionFPCvb (99.81 %), blocked by JAUStreamFileTable.cpp
  (Matching unit, not mine). Code identical; only relocation names differ:
    68/144  bl fn_8003C794             | bl __ct__18JAUStreamFileTableFv
    148/14C lis/addi lbl_804954C8      | __vt__32JAUStreamDataMgr_StreamFileTable

## SHARED-EDITS (JAUStreamFileTable.cpp stanza; evidence: mkdd MarioClub_us map)
mkdd's JAUStreamFileTable.cpp is: __ct__18JAUStreamFileTableFv (0xC, first), init,
getNumFiles, getFilePath, getStreamFileEntry, weak __dt__32JAUStreamDataMgr_
StreamFileTableFv (0x60); .data = __vt__32JAUStreamDataMgr_StreamFileTable (0x10).
FSA has the same objects:
- fn_8003C794 (0xC, `li r0,0; stw r0,0(r3)`, called on a stack object right before
  init__18JAUStreamFileTableFPCv) = `__ct__18JAUStreamFileTableFv`.
- lbl_804954C8 (0x10, in no unit; slots getStreamFileEntry__32JAUStreamDataMgr_
  StreamFileTable and fn_8003C878) = `__vt__32JAUStreamDataMgr_StreamFileTable`.
- fn_8003C878 (0x60, gap before JAUAudience at 0x8003C8D8) =
  `__dt__32JAUStreamDataMgr_StreamFileTableFv`.
Proposed stanza: `.text 0x8003C794-0x8003C8D8`, `.data 0x804954C8-0x804954D8`
(JAUSoundTable's .text then ends at 0x8003C794; done on unit/JSystem__JAudio__
Utility__JAUSoundTable), plus the three renames.
Tested here (reverted): the .text start + ctor rename alone changes only JAUSoundTable
and JAUStreamFileTable in the report (both up). Adding the .data split + vtable rename
without the dtor in range breaks the Matching link (JAUStreamFileTable.o .text 0xF8 vs
range 0xE4, and the main link's SHA-1 fails, so weak-dedup evidence drops for 27 units).
Whoever owns JAUStreamFileTable should apply all of it at once and re-run link_check.

## Earlier wave (kept from the previous checkpoint)
- Matched then: all JAUSection / JAUSectionHeap functions, bitset helpers,
  TStreamDataMgr getStreamFileEntry/dtor, @212@ thunks; newVoiceBank,
  setSeqDataArchive, getSoundTable added (Petari order).
- Still-open SHARED-EDITS from then: JAUSectionHeap.h getSoundTable() decl; new
  JASVoiceBank.h (FSA's JASBank slot 2 is getInstParam(int,int,int,JASInstParam*)
  const; JASBank.h still has mkdd's getInst(u32), fix both together); symbols.txt
  names outside the unit listed in that checkpoint (git show 19834a8~1:<this file>).
