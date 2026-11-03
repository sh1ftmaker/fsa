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
- Data sections are tiled from the data only the unit's code references;
  a unit's references inside another unit's data (extern consts, shared
  buffers) are reported and dropped; cuts between units go where the
  fewest unassigned symbols land on the wrong side, then down to an
  8-byte boundary when that takes nothing the previous unit references.
- `MERGE` holds the corrections dtk or the data proved (see docs/PLAN.md).
- Names: a `*.cpp` string the unit references (`RailBase.cpp`), else the
  actor table (`docs/names/actors.tsv`, high/medium names only) through the
  actor spawner `fn_801F9484`: the case's constructor, or the vtable it
  stores when the constructor is an inline copy in the spawner's file,
  places the actor in a unit. Anything else is `game/tu_<start>.cpp`.

Rerunning after `symbols.txt` gains names can change data cuts slightly;
diff `game_splits.txt` against `splits.txt` before applying.
