# Actor names

`actors.tsv` gives an English name for each of the 402 actor IDs: the
4-character type codes in `enemyparam.csv` plus the codes the actor spawner
`fn_801F9484` handles. Use it when naming actor source files and classes.
Each row cites its evidence. No game text is copied here beyond the names.

## Columns

| column | meaning |
|---|---|
| `id` | 4-character actor type code (case-sensitive: `Poon`) |
| `japanese_name` | label from `data.arc/enm/enemyparam.csv` (empty if the ID is only in the spawner) |
| `english_name` | proposed English name |
| `kind` | enemy / boss / npc / object / effect / debug / system |
| `evidence` | source tags (below), then the stages that place the actor |
| `confidence` | high / med / low (below) |
| `suggested_file` | proposed file stem, `act_<english_snake>` |
| `notes` | conflicts, caveats, actors with no class in the spawner |

## Evidence tags

- `msg#N`: message N has the Japanese word in `msgjp.arc` and its
  translation at the same ID in `msgus.arc` (US BMG text). This is the
  strongest evidence because it is the game's own English. Example: msg#317
  shows that グフー is "Vaati" and シャドウリンク is "Shadow Link".
- `zw:<url>`: Zelda Wiki's Nomenclature data, where the Japanese name
  recorded for that page matches the actor's Japanese label. The data was
  queried through the wiki's Cargo API (table `Nomenclature`, `lang=ja`).
  When the match comes from another game (for example ALttP for ポーン =
  Hardhat Beetle), the page must also be listed in "Enemies/Bosses in Four
  Swords Adventures".
- `stage`/`stages`: the ID occurs in that stage's `Boss/bossNNN.arc`
  `d_enemy_map*.bin`. Stage names come from US messages 1–32. For
  `boss0XY`, Y = 0, 1, 3 are stages X-1, X-2 and X-3, and Y = 2 is left
  unnamed. Texture names in the same archive are cited where they confirm a
  boss, for example `wart_*` for Arrghus and `boss_koori_*` for Frostare.
- `mt`: machine translation of the Japanese label only.

## Confidence

- **high**: the in-game English text names it (`msg#`), or the Japanese
  name matches the wiki exactly and the game is FSA or the stage agrees.
- **med**: the name rests on a descriptive or near match (ミイラ "mummy" →
  Gibdo, トンドル ≈ ALttP テンドル Vulture) checked against the wiki's
  FSA lists or the stage. Also used for `mt` rows whose Japanese label is a
  plain technical description (switches, managers, generators). The
  translation of these is reliable, but no official name exists.
- **low**: a guess, a proper noun that could not be identified (オシン,
  もじゃもじゃ), or an ID with no Japanese label (`unknown (XXXX)`).

## Known conflicts

- SPFR 超妖精: Zelda Wiki calls her "Fairy Queen", but US msg#694 says
  "Great Fairy". BFRY 大妖精 is also "Great Fairy".
- The Japanese label ルピー ("rupee") is shown as "Force Gem" in US text
  (msg#215). RUPY, RY02 and the other `*RY` actors are named Force Gem, and
  LPLK ルピーライク is named Force Like.
- Shadow Link has several variants: SLK2, DMLK, BBSL, ZSLA/D/F/H/J, and
  ZNLD/ZNLJ. ZNLD and ZNLJ have no Japanese label but derive from the
  Shadow Link enemy base class. They look like the spawnable classes for
  ZSLD and ZSLJ, which have labels but no spawner case.
- Several actors have both a Japanese label and a separate implementation
  ID: ZORA/ZNRA, BWRT/WRTC (Arrghus and its children, same TU), and
  IWAG/OIWA (same TU).
- GROK ジークロック: the same Japanese name is ALttP's Helmasaur King and
  TWW's Helmaroc King. FSA uses the Helmaroc King, confirmed by the stage
  (3-2) and the `jeek*` textures.

**Rule:** when the game's own text and a wiki disagree, the in-game name wins (e.g. SPFR is the Great Fairy, not the wiki's "Fairy Queen").
