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
