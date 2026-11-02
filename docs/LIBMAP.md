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
   rows.
3. Overrides from other evidence: `.init` layout and sizes (compared with
   mkdd), extabindex entries, real names already in `symbols.txt`
   (`sinf`, `AMC_IsStub`, `Hu_IsStub`, `JKRTask`).

Confidence: **high** means at least 80 % of the row's bytes are uniquely
byte-matched; **medium** means 30–80 %, or a short tail placed by position;
**low** means less than 30 %, or a long tail placed by position only.

## KiB per library (503.8 KiB: 494.7 .text + 9.1 .init)

| Library | KiB | high / medium / low | Main donor |
|---|---:|---|---|
| J3D | 168.2 | 57.7 / 104.4 / 6.1 | pikmin2 `JSystem/J3D` (paths: TP layout) |
| Other JSystem | 250.0 | 179.8 / 42.0 / 28.2 | J2DGraph 127.1, JAudio 36.8, JKernel 31.1, JUtility 27.2, JParticle 18.9, JMessage 7.9, JGadget/JSupport/JFramework/JMath 1.0 |
| MSL_C | 32.2 | 27.5 / 0.9 / 3.8 | pikmin2 (mkdd cross-check) |
| Runtime | 2.1 | 2.1 / 0 / 0 | pikmin2 / mkdd |
| MetroTRK v2.6 | 32.5 | 30.7 / 1.8 / 0 | pikmin2 `TRK_MINNOW_DOLPHIN` (+ `.init` vector table) |
| THP | 0 | — | not in this region: THP is in the game region, `tu_map.tsv` row 0x804280C0–0x8042E780 |
| SDK leftovers | 17.0 | 8.2 / 3.9 / 4.9 | pikmin2: gba, gd, OdemuExi2, amcstubs, odenotstub, OSAudioSystem, dvderror, fstload, GXDisplayList, `__start`, `__ppc_eabi_init` |
| Game TU at .text start | 1.7 | 0 / 1.7 / 0 | none (0x80021840; see open questions) |
| Unknown | 0 | | |

All confidences together: high 306.1, medium 154.8, low 43.0 KiB. New
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
- 58 new `.text` splits (TRK 23, MSL 24, Runtime 4, SDK 7), code only:
  data splits and `configure.py` objects come with the import.

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
- 0x80021840–0x80021F24 is game code (extabindex entries, so exceptions are
  on), linked before JSystem the way mkdd links `kartLocale.cpp`. Add it to
  the game map.
- Answered (import/jaudio): 0x8002F274–0x80030380 is JASBNKParser.cpp
  (`createBank` plus a second `createBasicBank`, like tp's Ver0/Ver1 parsers).
- JStudio, JStage and J3DU have no unique matches, so they are probably not
  linked.
- THP: pikmin2's THP matches 2 of 22 functions, so FSA has another THP
  version. Try other donors.
- The scan used the splits at 63edbae and earlier. `extend:` rows for
  units merged since then (JUTConsole, JUTDirectPrint, JKRCompArchive,
  JKRArchivePri, JPAFieldBlock) may already be covered. Rows marked
  "claimed by another agent" were listed, not edited.
