# Translation-unit map

Infers the game's original source files (translation units) from `main.dol`,
since FSA has no symbol map. Output: `config/G4SE01/tu_map.tsv` (draft; one
row per unit with evidence and confidence, plus each unit's data spans).

Signals, most reliable first:
1. `.ctors`: one static initializer per file, the file's last function
   (483 of 527 game files share the JAudio pool-allocator initializer).
2. `extab`/`extabindex` order matches code order (no boundary may invert it).
3. `.sdata2` float pools: per file, in first-use order; a repeated value
   means a new file; padding before an 8-aligned float marks a file start.
4. Data order in `.rodata`/`.data`/`.bss`/`.sbss`/`.sdata` follows code order.
5. Strings (resource names) give naming hints only.

Run after `ninja build/G4SE01/report.json` (needs dtk's `build/G4SE01/asm`),
from a scratch directory (outputs `refs.json`, `rows.json`, `tu_map.tsv` in
the current directory):

```sh
mkdir -p build/tu_map && cd build/tu_map
python3 ../../tools/tu_map/refs.py && python3 ../../tools/tu_map/build_map.py
python3 ../../tools/tu_map/validate.py   # checks the method against splits.txt
```

## Into splits.txt

`to_splits.py` turns the map into `splits.txt` stanzas (`game/<name>.cpp`)
with every section the evidence covers, and writes
`config/G4SE01/game_units.tsv` (unit, text range, confidence, sections, name
evidence). Run it after `refs.py`, from the same directory:

```sh
python3 ../../tools/tu_map/to_splits.py           # dry run: game_splits.txt, game_units.tsv, notes
python3 ../../tools/tu_map/to_splits.py --apply   # replace the game/ stanzas in splits.txt
```

- extab/extabindex come from the DOL's extabindex entries, `.ctors` from
  the slot pointing at the unit's `__sinit`.
- Data ownership is computed over all data sections at once: data only one
  unit's code references, then (transitively, across sections) data only
  referenced from data one unit owns. References from the actor spawner's
  file don't count when another unit also references the symbol (it holds
  inline copies of other files' constructors). Each section is tiled from
  the owned data; a unit's owned data inside another unit's run (extern
  consts, shared buffers) is reported and dropped; cuts between units go
  where the fewest symbols land on the wrong side, then down to an 8-byte
  boundary when that takes nothing the previous unit references. Data that
  still points into another unit's `.rodata`/`.sdata2` is reported.
- The spawner's compare tree is decoded along its branches; an ID counts
  only from `lis`/`addi r0`/`cmpw r29, r0`/`beq`. Where actors.tsv
  suggests one file stem for several IDs, each unit gets its ID appended.
- `MERGE` holds the corrections dtk or the data proved (see docs/PLAN.md).
- Names: a `*.cpp` string the unit references (`RailBase.cpp`), else the
  actor table (`docs/names/actors.tsv`, high/medium names only, all of a
  unit's actors agreeing) through the actor spawner `fn_801F9484`: the
  case's constructor, or the vtable it stores when the constructor is an
  inline copy in the spawner's file, places the actor in a unit. Anything
  else is `game/tu_<start>.cpp`.

Rerunning after `symbols.txt` gains names can change data cuts slightly;
diff `game_splits.txt` against `splits.txt` before applying.
