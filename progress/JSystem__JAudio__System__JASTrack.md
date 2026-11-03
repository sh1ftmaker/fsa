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

## p_JASTrack (2026-11-03)
Baseline: progress.py total 6296 / 11557 functions, 1260584 bytes; unit 64/65, .text 99.23 %, data 236/236.
Now: 65/65 functions, all sections 100 % in objdiff; total 6297 / 11557, 1261232 bytes. gate: ok. Still NonMatching.
- setChannelPauseFlag 83.11 -> 100 %: `if (JASTrack *child = mChildren[i])` (declaration in the
  condition). MWCC's recursive auto-inline depth depends on the condition's tree shape: an
  assignment/declaration as the whole condition gives 7 bodies (FSA), `child != NULL`, `!= 0`,
  `if (child)` after a separate declaration all give 6. Measured on a minimal TU (scratch); same
  holds for `if ((m = mChannelMgrs[i]))` in the channel loop. Precedent: assignment-in-condition is
  JSystem style (tp JAudio2/JAISound.cpp:184, J2DScreen.cpp:144).
- Flip to Matching BLOCKED (tried; reverted): linking our JASTrack.o breaks main.dol SHA-1
  (gate: 106 problems, other units' dedup evidence lost, JASChannel/mtx drop below 100 %).
  Cause: deferred-function layout at the end of .text. With -sym on MWCC puts weak/deferred
  functions in one .text section per defining header; __sinit lands in JASHeapCtrl.h's section
  (the memPool_ definitions), after free/__dl:
    ours: MoveParam_ ct | TList dt, BankTable dt | free<JASTrack>, free<TChannelMgr>, __dl x2, __sinit | getBank
    FSA:  MoveParam_ ct | __sinit | TList dt | BankTable dt | free x2 | __dl x2 | getBank
  Every bl into free/__dl/__sinit shifts by 0x18C (and .ctors 0x8045BD7C). Same mechanism in
  JASSeqParser/JASChannel (__sinit first in the JASHeapCtrl.h section, fine there because no
  free/__dl precede it). Fix likely needs __sinit before the out-of-line free/__dl emission, i.e. how
  FSA defined memPool_ / where its JASHeapCtrl inlines live: shared header JASHeapCtrl.h, not mine.
  Moving sDefaultBankTable/sTrackList to the end of the file changes nothing.

## Next
- Solve the end-of-.text order above (JASHeapCtrl.h memPool_/free/__dl), then flip; that unblocks
  game/tu_80021840.

## jastrack-sinit (2026-11-03): end-of-.text order, measured; no evidenced fix
Measured on scratch copies (same flags as ninja, GC/2.6 -sym on), reading each weak .text section's
DWARF1 .line block to get its file. Nothing in the repo changed; still NonMatching.
- Sections are per defining file; functions without line info (__sinit, implicit dtors such as
  JASDefaultBankTable's) go to the "current" section. For __sinit that is the file holding the
  `template <typename T> JASMemPool_MultiThreaded<T> JASPoolAllocObject_MultiThreaded<T>::memPool_;`
  definition, NOT where memPool_ is first used: an inline function at the end of JASTrack.h that uses
  all three memPool_s (memPool_ made public for the test) leaves __sinit in the JASHeapCtrl.h section.
- Definition in JASHeapCtrl.h (today): MoveParam ct, TList dt, TLinkList dt, BankTable dt | Lock dts |
  CritSec dt | free x2, __dl x2, __sinit, memPool dts | GlobalInstance dt | getBank (the bad order).
- No free/__dl at all (all `delete child` removed): __sinit still opens its own JASHeapCtrl.h section.
- Definition moved to JASTrack.cpp (after the includes): __sinit goes to the end of the main .text.
- Definition moved to the end of JASTrack.h: MoveParam ct, __sinit, memPool dts x3, TList dt,
  TLinkList dt, BankTable dt | Lock dts | CritSec dt | free x2, __dl x2 | GlobalInstance dt | getBank.
  That is exactly FSA's order once the weak duplicates drop out. TList/BankTable dts are generated after
  __sinit (it registers them), so FSA's __sinit-before-TList-dt says FSA's __sinit was in the same
  section as MoveParam ct: JASTrack.h's (or the main file's, if MoveParam_ ct and ~TList were also in the .cpp).
- The definition can't be in both headers: MWCC errors "object 'JASPoolAllocObject_MultiThreaded::memPool_'
  redefined". Moving it out of JASHeapCtrl.h into JASTrack.h leaves every memPool_ user that
  doesn't include JASTrack.h without one. That covers JASChannel.cpp, JASBank, JASDrumSet,
  JASBasicInst, JASAiCtrl and the other JAudio units, plus the ~500 game units through GameAudio.h.
  So a fix is a shared header reorganisation with no donor evidence yet. Not done.
- Other units are consistent with either placement. In FSA, __sinit directly follows the last main
  function in JASSeqParser, JASAiCtrl and JASChannel; their weak functions in between are dropped
  duplicates. JASAiCtrl's ours has __sinit inside clamp<s,l>'s JASCalc.h section, so the current file
  can also be set by a template function instantiated after the memPool_ data.
- Next: find evidence of where FSA (or another game with this JAudio vintage) defines the
  MultiThreaded memPool_. One idea: a per-type definition next to each class, which would need
  the guard flags `__init__memPool_...` that FSA's __sinit uses, so a generic (template)
  definition, not an explicit specialisation. Then decide on the header move with the coordinator.
  game/tu_80021840 stays blocked on this.

## Donor check (2026-11-03)
- mkdd (licensed) has no generic memPool_ definition in JASHeapCtrl.h; it puts explicit `template <>` specialisations in JASFakeMatch*.h headers included by JASTrack.cpp and others. Tried in FSA's JASTrack.cpp: __sinit_JASTrack_cpp drops to 32 % (no `__init__memPool_` guard tests), main.dol SHA-1 fails. FSA used the generic, guarded definition.
- TP (JAudio2) uses a function-local static in `getMemPool_()`: a later design, no template static member, not FSA's.
- TWW and Pikmin 2 use the older JAudio (JASystem / JAS), with no JASPoolAllocObject memPool_ to compare.

## abs-jastrack (2026-11-03): source-organisation hypotheses for the end-of-.text order; no natural fix
Nothing in the repo changed (scratch overlays in front of `-i libs/sdk/include`, same ninja flags, GC/2.6;
read each weak `.text` section's function list). Still NonMatching; game/tu_80021840 not flipped.

Rules (measured, they explain every case so far):
1. Weak sections are per defining file, ordered by the first function emitted into each. JASTrack's emission
   order is MoveParam ct, Lock dts, CritSec dt, free x2, __dl x2, __sinit, memPool dts, TList dt, TLinkList dt,
   BankTable dt, GlobalInstance dt, getBank (B1 below shows it in a single section).
2. Template static data is instantiated at end of TU in reverse order of the class template instantiation
   (LIFO): guards/inits run TChannelMgr, JASTrack, JASChannel, but JASChannel's base is instantiated first
   (JASChannel.h). Non-member function templates are instantiated after that.
3. __sinit and implicit dtors go to the "current file": the file of the last thing that end-of-TU pass
   instantiated (template static's definition, or a function template's definition).
FSA's order needs __sinit in a section opened before free/__dl, i.e. JASTrack.h's (MoveParam ct) section,
because `~TList() {}` is explicit in JASTrack.h and is emitted after __sinit.

Experiments:
- H1 memPool_ definition at the end of JASChannel.h (every MT memPool_ user includes JASChannel.h; 12 units
  include it without JASTrack.h): __sinit opens a new JASChannel.h section after free/__dl. Wrong.
- GC/2.0, 2.5, 2.7: same layout as 2.6 (3.0a3 differs throughout). Not a compiler-version effect.
- B1 implicit TList dtor (drop `~TList() {}`): same code size; TList dt follows __sinit into JASHeapCtrl.h.
  Only helps if __sinit's file were one whose section opens before free (JASTrack.h, JASHeap.h with
  JASThreadingModel, or JASCriticalSection.h). memPool_'s definition can't be in the last two (they come
  before the class template).
- D1 generic `template <class T> T *JASGlobalInstance<T>::sInstance;` in JASGadget.h (like Petari's
  JASGlobal.hpp; FSA's sInstance<JASDefaultBankTable> is scope:weak, so FSA probably had a generic def):
  sInstance becomes a weak definition, is instantiated after memPool_ (rule 2), and __sinit moves into
  JASGadget.h's section, which opens after free. Wrong order, but confirms rule 3 with zero-init data.
- D2 a function template defined in JASTrack.h and used by JASTrack.cpp (diagnostic only):
  MoveParam ct, tmpl, __sinit, memPool dts, TList dt, TLinkList dt, BankTable dt | Lock | CritSec | free x2,
  __dl x2 | GI dt | getBank. This is FSA's order if the template copy is a dropped weak duplicate. So one
  explanation is a function template in FSA's JASTrack.h, instantiated in JASTrack.cpp and also emitted
  by an earlier-linked unit (JAI*, JASSeqCtrl, JASSeqParser include JASTrack.h). No candidate found: no
  JASTrack.h template in mkdd/TP/SMG, and the earlier units' symbols have no such template function.

Ruled out by reasoning:
- memPool_ definition only in JASTrack.h: JASBank, JASDrumSet, JASChannel, JASBasicInst, JASVoiceBank,
  JASAudioThread, JASBasicBank, JASAramStream, JASInstSense, JASBNKParser, JASAiCtrl, JASInstRand have one
  pool and an __sinit, so they need the definition without JASTrack.h. Including JASTrack.h there would
  instantiate all three pools (game units show inclusion alone instantiates them).
- Definition in two headers: MWCC "redefined"; guarding it on JASTrack.h's guard is a trick.
- A separate definitions header (included by JASChannel.h/JASTrack.h): its own section, opened at __sinit
  after free (like H1).
- Explicit `template <>` per class (mkdd JASFakeMatch): no guards, __sinit 32 % (earlier).
- MoveParam ct non-inline in JASTrack.cpp: __sinit would still sit in JASHeapCtrl.h after free.
- TP (-ipa file, function-local static memPool_) has MoveParam ct, memPool dts, __sinit, TList dt, but
  different mechanics, so it isn't evidence for FSA's headers.

Next: look for a function template that FSA's JASTrack.h could have held, e.g. an unreferenced small
weak function in JAI*/JASSeq* .text. Otherwise the unit stays NonMatching (100 % in objdiff).
