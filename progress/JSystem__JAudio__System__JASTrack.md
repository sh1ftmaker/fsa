# JSystem/JAudio/System/JASTrack.cpp

Baseline (start): progress.py total 1398 / 11560 functions, 260384 bytes; unit 43/60 functions.
Now: unit 62/65 functions (.text extended over 5 weak functions), .text 97.58 %; total 1422 / 11560, 269364 bytes. gate: ok.
Flags: already -O4,p (stmw only for 5+ registers in the target).

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
