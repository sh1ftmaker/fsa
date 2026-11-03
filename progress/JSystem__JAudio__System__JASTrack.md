# JSystem/JAudio/System/JASTrack.cpp

Baseline (start): progress.py total 1398 / 11560 functions, 260384 bytes; unit 43/60 functions.
Now: unit 62/65 functions (.text extended over 5 weak functions), .text 97.58 %; total 1422 / 11560, 269364 bytes. gate: ok.
Flags: already -O4,p (stmw only for 5+ registers in the target).

## w10-jas (2026-11-02)
Baseline: progress.py total 4784 / 11561, 863464 bytes; unit 64/65, .bss 0 %.
Now: 64/65, .bss 100 %, __sinit 100 % (it was 99.8 % on objdiff's data-flow check of `addi r5, r31, 0xc..0x30`);
NonMatching (setChannelPauseFlag). gate: ok.
- Split lbl_804B2940 (0x40 placeholder) into the five 0xC destructor-chain nodes @243, @244, @2097,
  @2098, @2099 (our object's own local names and offsets; same form as the other units' @NNN nodes).
- setChannelPauseFlag (83.1 %, codegen): FSA 0x288 bytes, ours 0x234; FSA inlines the self-recursion one
  level deeper (frame 0x50, stmw r16; ours 0x40, stmw r18). Same source in tp/SMG. Earlier variants
  (compilers 1.3.2-3.0a3, -inline settings) are listed below; no pragma has donor precedent. Not
  blocked by another unit.

## Done
- Source from TP where FSA differs from mkdd: setChannelMgrCount (no init of mgr 0, no else branch),
  init (inline delete of mgrs 1..3, no setChannelMgrCount call), setChannelPauseFlag recursing into
  children, pause calling only setChannelPauseFlag. Dropped inert JASFakeMatch.h include (FIXME, like JASSeqParser).
- Names (evidence: call sites in this unit's matched code + donor bodies/order):
  setChannelMgrCount, setChannelPauseFlag, pause (positions + asm), __sinit_JASTrack_cpp (.ctors 0x8045BD7C),
  __dt__Q28JASTrack5TListFv (registered as sTrackList dtor in __sinit), getBank__17JASBankTable<256>CFUl (vtable/asm),
  free/alloc__17JASGenericMemPool, registerSubFrameCallback__9JASDriver, JASBank::noteOn,
  JASRegisterParam ct/init (TP order ct, init, write, read), __construct_array (MSL order),
  sInstance__40JASGlobalInstance<19JASDefaultBankTable> (dtor/sinit), sDefaultBankTable__8JASTrack (0x40C = our .comm size; rest is lbl_8052E73C).
- Splits: .text end 0x800397D0 -> 0x80039964 (free/__dl weak pairs, getBank); .bss 0x804B2940..0x804B2980 (5 destructor-chain nodes).

## Resistant
- setChannelPauseFlag (83 %): target inlines the self-recursion 7 levels, every compiler (1.3.2, 2.0, 2.5, 2.6, 2.7, 3.0a3; -inline auto/all/level=N/deferred) gives 6. Variants (no temps, getChild, int index, file position) don't change depth.
- __sinit_JASTrack_cpp (40 %) and .bss (0 %): need the three memPool_ definitions (TChannelMgr, JASTrack, JASChannel) - see COORDINATOR "JASFakeMatch findings".
- __dt__Q28JASTrack5TListFv (99.77 %): calls fn_80087868 = JGadget::TNodeLinkList::~TNodeLinkList (empty body + delete; first function of linklist.cpp in TP). Naming it breaks the link because Matching linklist.cpp's .text starts at 0x800878F0. SHARED-EDIT: extend linklist.cpp .text to 0x80087868 (fn_800878A4 = erase(iterator)) and name it.
- Our object emits weak functions (MoveParam_ ct, free/__dl, Lock dtors) after first use; FSA has them at the end of the file. Only matters for linking.

## SHARED-EDITS
- symbols.txt names in other units' ranges (JASHeapCtrl alloc/free, JASDriver, JASBank, JASRegisterParam, __construct_array).
- linklist.cpp split/name above (not applied).
