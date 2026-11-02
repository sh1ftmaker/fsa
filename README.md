# The Legend of Zelda: Four Swords Adventures: Decompilation

A work-in-progress byte-matching decompilation of *The Legend of Zelda: Four
Swords Adventures* (GameCube, USA). The goal is C/C++ source that the original
Metrowerks compilers build into a `main.dol` identical to the retail one.

| Property | Value |
|----------|-------|
| Game | The Legend of Zelda: Four Swords Adventures |
| Platform | Nintendo GameCube |
| Game ID | G4SE01 (USA) |
| Compilers | Metrowerks CodeWarrior GC/2.6 (game, JSystem); the SDK's own builds |
| Build System | ninja + [decomp-toolkit](https://github.com/encounter/decomp-toolkit) |

## Progress

Measured by the honest objdiff report (every unit diffed, relocations
checked: `ninja build/G4SE01/report_honest.json && python3 tools/progress.py`),
2026-11-02:

| | Functions | Code |
|---|---|---|
| Total | 1,165 / 11,560 | 4.84 % |
| Dolphin SDK | 615 / 636 | |
| JSystem | 550 / 1,014 | |

58 units are Matching; `ninja` links them into a `main.dol` whose SHA-1
matches the original.

## Building

### Prerequisites

- Python 3.8+
- [ninja](https://github.com/ninja-build/ninja/releases)
- A copy of the game (not provided). Extract it so that `orig/sys/main.dol`
  exists, e.g. with `dtk disc extract` or Dolphin's *Export Partition Root*.

The Metrowerks compilers, dtk, objdiff-cli and (on Linux) wibo are downloaded
into `build/` on the first run.

```sh
python3 configure.py
ninja build/G4SE01/report_honest.json
```

A plain `ninja` also links `main.dol` and checks its SHA-1 against
`config/G4SE01/build.sha1` (see `CLAUDE.md`). On Windows the compilers run
natively.

### objdiff

Download [objdiff](https://github.com/encounter/objdiff/releases) and open the
repository root; it reads `objdiff.json`. Every function of the DOL is
browsable, and matching ones turn green.

## Contributing

1. Pick a function or file in objdiff.
2. Write the source under `libs/` or `src/` and add the file to `configure.py`.
3. Map its `.text` and data ranges in `config/G4SE01/splits.txt`.
4. Rebuild the report and check the unit in objdiff.

`CLAUDE.md` has the matching rules and the code style. Functions can also be
worked on at [decomp.me](https://decomp.me/preset/228) with the
*The Legend of Zelda: Four Swords Adventures (DOL)* preset.

### Splitting out a new source file

When a range moves into a new source file, split `extabindex` to match:
every `extabindex` entry references its function and must live in the same
object. Otherwise `dtk dol split` aborts with `Bad extabindex relocation @ <address>`.

## License

The decompiled source code in this repository is the result of reverse
engineering and is provided for educational and preservation purposes only.
The original game and its assets are the property of Nintendo. Files under
`libs/` come from other decompilation projects under CC0; see their LICENSE
files.
