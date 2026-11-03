# JSystem/JAudio/System/JASHeapCtrl.cpp

w10-jas (2026-11-02). Baseline: progress.py total 4787 / 11561, 864000 bytes (this worktree's base);
unit 21/21, all sections 100 %, but `link_check --unit` failed (trailing weak copies in the wrong order).
Now: 21/21, Matching; `link_check --unit` SHA-1 OK; gate: ok (main link with every Matching unit
SHA-1 OK). Full honest report unchanged otherwise (0 differences against the pre-change report).

## Why the order differed (MWCC 2.6, -sym on)
- FSA's tail: __sinit, ~JASHeap, onDispose (JASDisposer.h), getFirstLink (JSUList.h).
- With -sym on, a user-declared inline's out-of-line copy goes into a trailing .text section per header,
  sections in order of first use; a compiler-generated destructor goes into the main section right after
  its first user (scratch compiles; same mechanism as JASBasicWaveBank's TWaveInfo).
- ~JASHeap right after __sinit = implicit dtor whose first user is __sinit (audioAramHeap's registration).
  TP's JASHeap has no user-declared dtor either.
- The ctor's `mTree(this)` marks JSUTree<JASHeap>'s dtor as used even with exceptions off, so with
  mkdd's user-declared `~JSUTree() {}` / `~JSULink() {}` the JSUList.h section opened before
  JASDisposer.h's. FSA has JASDisposer.h's first, i.e. those dtors are implicit: their copies now sit
  after the ctor in the main section, and the linker drops them as duplicates (trial map).

## Changes
- JASHeap.h: no user-declared `~JASHeap() {}` (TP).
- JSUList.h: no user-declared `~JSULink() {}` and `~JSUTree() {}` (JSUList<T> already had none).
  Shared header (54 includers): full honest report identical, main link SHA-1 OK.
- dump()/dump(int) are dead-stripped in FSA (our body is a guess); its calls emit getFirstLink etc.

## Resistant
None.

## SHARED-EDITS
- JSUList.h and JASHeap.h above (header layout fixes, report and link checked).
