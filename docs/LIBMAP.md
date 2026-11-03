# Library map (0x80003100–0x800F39EC)

`config/G4SE01/lib_map.tsv` proposes a source file for every byte of `.init`
and `.text` below the game code that no unit split covered at the time of
the scan (branch point 63edbae). There is one row per proposed unit, or per
piece that extends an existing unit. Columns: the text range; the library;
the proposed source path; the action (`new`, or `extend:<unit>` for a tail
that belongs to an existing unit); the confidence; the evidence; the donor
file that seeds it, with its compiler and flags; the data addresses the code
references (`data_refs`, outside other units' splits, as min–max per
section, not verified); and `done_here` (`split+names` = split and named on
this branch).

## How it was built

1. Compiled every library object of pikmin2 (413 files, its own flags) and
   mkdd's JSystem, MSL, Runtime and TRK (285 files) with GC/2.6 or
   GC/1.2.5n under wibo. Then ran `tools/donor_match.py` against FSA's
   function starts: 2,866 pikmin2 and 1,349 mkdd functions match exactly
   one FSA function.
2. Each unsplit function takes the donor file of its unique byte matches.
   Functions without a unique match take their position: between two
   functions of one file, they belong to that file; otherwise they extend
   the preceding file (`__sinit_*`, weak templates, thunks). Runs become
   rows. That last rule was wrong at file starts: review corrected the TRK
   splits and eight position-only rows (OSArena, `__init_user`, dvdFatal,
   dsp_debug, MWTrace, critical_regions, random.cpp, JPABaseShape) from the
   real names they contain and the donors' split layout. Treat the remaining
   `extend:` tails the same way: an unmatched leading function may open the
   next file.
3. Overrides from other evidence: `.init` layout and sizes (compared with
   mkdd), extabindex entries, real names already in `symbols.txt`
   (`sinf`, `AMC_IsStub`, `Hu_IsStub`, `JKRTask`).

Confidence: **high** means at least 80 % of the row's bytes are uniquely
byte-matched or already carry real names; **medium** means 30–80 %, or a short tail placed by position;
**low** means less than 30 %, or a long tail placed by position only.

## KiB per library (503.8 KiB: 494.7 .text + 9.1 .init)

| Library | KiB | high / medium / low | Main donor |
|---|---:|---|---|
| J3D | 168.2 | 57.7 / 104.4 / 6.1 | pikmin2 `JSystem/J3D` (paths: TP layout) |
| Other JSystem | 250.0 | 182.6 / 39.2 / 28.2 | J2DGraph 127.1, JAudio 36.8, JKernel 31.1, JUtility 27.2, JParticle 18.9, JMessage 7.9, JGadget/JSupport/JFramework/JMath 1.0 |
| MSL_C | 32.2 | 31.3 / 0.9 / 0 | pikmin2 (mkdd cross-check) |
| Runtime | 2.1 | 2.1 / 0 / 0 | pikmin2 / mkdd |
| MetroTRK v2.6 | 32.5 | 32.4 / 0.2 / 0 | pikmin2 `TRK_MINNOW_DOLPHIN` (+ `.init` vector table) |
| THP | 0 | — | not in this region: inside a game TU, `tu_map.tsv` row 0x804280C0–0x8042E780 (sms THP, see open questions) |
| SDK leftovers | 17.0 | 8.5 / 3.6 / 4.9 | pikmin2: gba, gd, OdemuExi2, amcstubs, odenotstub, OSAudioSystem, dvderror, fstload, GXDisplayList, `__start`, `__ppc_eabi_init` |
| Game TU at .text start | 1.7 | 0 / 1.7 / 0 | none (0x80021840; see open questions) |
| Unknown | 0 | | |

All confidences together: high 314.6, medium 150.0, low 39.2 KiB. New
files: 292.0 KiB; tails of existing units: 211.8 KiB.

## Compiler and flag evidence

pikmin2's JSystem was compiled three ways. The table counts unique function
matches in FSA:

| Library | -O4,p, stmw on | -O4,p, stmw off | -O4,s, stmw on |
|---|---:|---:|---:|
| J2D | **658** | 574 | 413 |
| J3D | **587** | 509 | 395 |
| JKernel | **206** | 177 | 103 |
| JUtility | **168** | 142 | 95 |
| JParticle | **130** | 115 | 94 |
| JAudio (pikmin2's) | **132** | 129 | 92 |
| JSupport | **24** | 24 | 9 |

- JSystem: GC/2.6, `-O4,p -use_lmw_stmw on` (pikmin2's `cflags_pikmin`).
  With this flag MWCC 2.6 at `-O4,p` still saves 2–4 registers with
  separate `stw`, which is the stw-vs-stmw finding in CLAUDE.md. `-O4,s`
  loses everywhere, J2DGraph included, so the `-O4,s` that the J2DGraph
  units copied from mkdd is wrong for FSA.
- TRK: GC/2.6 with pikmin2's TRK flags (`-inline deferred -sdata 0 -sdata2 0`,
  stmw on): 116 matches, against 95 with stmw off.
- MSL_C and Runtime: GC/2.6 with pikmin2's `cflags_runtime` plus
  `-inline deferred` (MSL 75 vs 65, Runtime 31 vs 29 with stmw off).
- SDK leftovers: GC/1.2.5n with pikmin2's SDK flags (`-str noreadonly`).
  Existing SDK units use `cflags_sdk`.
- Not tested: GC/2.7 and GC/1.3.2 against these donors.

## Recommended import order

1. **MetroTRK** (pikmin2, GC/2.6 TRK flags): 32.5 KiB, almost all
   byte-identical, C only. The `.init` pieces are `mem_TRK`,
   `__exception.s` and `__TRK_reset`.
2. **Runtime + MSL_C** (pikmin2, GC/2.6 runtime flags): new files and the
   tails of the existing MSL units (printf, scanf, mem, string, buffer_io).
3. **SDK leftovers** (pikmin2 SDK, GC/1.2.5n): gd, OdemuExi2, amcstubs,
   odenotstub, OSAudioSystem, dvderror, fstload, GXDisplayList, `__start`,
   `__ppc_eabi_init`, `__mem`. Then GBA: pikmin2's gba (only 7–69 %
   matched) plus ac-decomp's CC0 `dsp_GBAKey.c`.
4. **Tails of existing JSystem units** (`extend:` rows): `__sinit_*`, weak
   templates and missing methods. Each owner extends its unit's `.text`.
5. **J2DGraph new files** (pikmin2, `-O4,p`): J2DMatBlock (33 KiB, 96 %),
   J2DPictureEx, J2DWindow, J2DTextBox, J2DMaterialFactory, J2DAnimation,
   J2DBloSaver. Switch the J2DGraph units to `-O4,p` at the same time.
6. **JKernel, JUtility, JMessage, JParticle new files** (pikmin2):
   JKRAram, JKRDvdAramRipper, JKRDvdRipper, JMessage
   resource/processor/control, JPABaseShape, JPAEmitterManager, JGadget
   binary. JKRTask has no donor.
7. **J3D** (pikmin2 `-O4,p`, TP for paths and headers, tww as a
   cross-check): 168 KiB. J3DGD, J3DTransform, J3DCluster, J3DJoint,
   J3DDrawBuffer and J3DSys are 98–100 % matched. J3DMatBlock (60 %),
   J3DMaterialFactory (74 %) and J3DAnimation (55 %) are FSA-era variants
   and need work.
8. **JAudio new files** (mkdd, pikmin2 for JASHeapCtrl): JAISeMgr (91 %),
   JASHeapCtrl. JASAramStream, JAUAudience and JAUSoundObject are low.

## Done on this branch

- 662 `fn_` placeholders renamed after unique donor matches in high rows.
  Names defined by any compiled unit object, names already in
  `symbols.txt` and conflicting proposals were skipped.
- 60 new `.text` splits (TRK 25, MSL 24, Runtime 4, SDK 7), code only.
  Data splits and `configure.py` objects come with the import. dtk also
  wrote three non-`.text` ranges for them: `NMWException.cp`
  extab 0x80005650–0x80005698 and extabindex 0x800118D8–0x80011914, and
  `global_destructor_chain.c` `.dtors` 0x8045C648–0x8045C64C (checked in
  review). The TRK boundaries follow pikmin2's file sizes: nubinit,
  serpoll, usr_put, targimpl and dolphin_trk start at their first function,
  `mutex_TRK.c` and `UDP_Stubs.c` are split, and targimpl absorbs its
  padding word, so the build has no `pad_` units (4,434,096 bytes, 11,560
  functions).

## Open questions

- `J3DGraphBase/J3DSys.cpp` (0x800D3168–0x800D32C0): all three of its
  functions byte-match pikmin2's **J3DGD.cpp**. The real J3DSys is
  0x800BA544–0x800BB618. The unit looks misnamed.
- `J3DGraphBase/J3DShapeMtx.cpp` (0x800CC088–0x800CC094) sits where
  pikmin2 has **J3DBinaryFormat.cpp**. J3DShapeMtx is 0x800E1318–0x800E2B40.
- Answered (import/jaudio): the units `JSystem/JAudio/JASCmdStack.cpp`,
  `JASBNKParser.cpp` and `JASDrumSet.cpp` were TWW-era pieces sitting on single
  functions of JASHeapCtrl, JASBNKParser and JASDrumSet under `System/`;
  they are deleted and their rows point at the `System/` units.
- 0x80021840–0x80021D60 is game code (extabindex entries, so exceptions are
  on), linked before JSystem the way mkdd links `kartLocale.cpp`; it is
  `game/tu_80021840.cpp`. 0x80021D60–0x80021F24 is JSystem's
  `JAIAudience.cpp` (no extab; matches the mkdd source with `-O4,p`).
- 0x8002F274–0x80030380 (5 functions, 4.3 KiB): matching now names
  0x8002F274 `JASBNKParser::createBank`, so this range belongs to
  JASBNKParser, not to a JASBasicWaveBank tail. The TSV row says so.
- JStudio, JStage and J3DU have no unique matches, so they are probably not
  linked.
- Answered (import/thp): FSA's THP is doldecomp/sms's `THPDec.c` /
  `THPAudio.c` (CC0; the version whose `THPInit` has no
  `OSRegisterVersion`, and FSA's DOL has no THP version string), now in
  `libs/sdk/dolphin/thp/` with sms's `thp.h`. pikmin2 matched 2 of 22
  because of the build, not the version: FSA compiles THP with its game
  code, GC/2.6 C++ with `-Cpp_exceptions on -inline off,deferred` (on top
  of `cflags_sdk`). Every licensed SDK-THP donor then matches 26 of 28
  functions (mkdd/oot-gc/bfbb lose `THPInit` to `OSRegisterVersion`,
  MP4/partyboard also lose both IDCTs); with three source changes sms
  matches 28 of 28 at FSA's function starts (`donor_match.py`), and its
  `.rodata` (0x8048F270-0x8048F300) is byte-identical: `__THPInitFlag` is a
  `bool` (FSA loads it with `lbz`), `__THPDecompressiMCURowNxN` is defined
  after `__THPHuffDecodeDCTCompV` (the deferred, reversed `.text` order puts
  it right after `THPInit`'s `OSInitFastCast`), and three `(u8*)` casts C++
  needs. Layout: `THPDec.c` text 0x8042B148-0x8042DC88 (extab
  0x80010ADC-0x80010B44, extabindex 0x800208F0-0x8002098C, rodata
  0x8048F270-0x8048F300, its 0x130-byte `.bss` is 0x8052A9A0-0x8052AAD0),
  `THPAudio.c` text 0x8042DC88-0x8042E1E4 (extab 0x80010B44-0x80010B4C,
  extabindex 0x8002098C-0x80020998, no data).
  They can't be split as units of their own: THP sits inside a game
  translation unit. The game's THP player before it (0x804280C0-0x8042B148,
  C++: sms's `AudioDecoder__FPv` and `THPGXRestore` match, little else) and
  the 0x28-byte wrappers after it (0x8042E1E4-0x8042E780, which call the
  player through a global) have extab entries interleaved with each other
  (no entry is shared), so player and wrappers are one object, and the THP
  code between them is in that object too: the game file includes the THP
  sources. dtk's extab grouping says the same ("Conflicting splits within
  reversed extab group" for any split at 0x8042B148 or 0x8042E1E4). The
  sources are not in `configure.py` yet; they get built when that game TU
  is decompiled, `#include`-ing them.
- The scan used the splits at 63edbae and earlier. `extend:` rows for
  units extended on matching since then (JASTrack, JAUSectionHeap,
  JUTConsole, JUTDirectPrint, JKRCompArchive, JKRArchivePri,
  JPAFieldBlock) carry a `stale` note in the TSV and may already be
  covered. They were listed, not edited.
