#!/usr/bin/env python3
"""Generate and check docs/names/categories.tsv: one category per game unit.

  python3 tools/categorize.py           # rewrite the TSV (hand rows kept)
  python3 tools/categorize.py --check   # exit 1 when a unit is missing, a row is
                                        # stale or malformed, or the file is not
                                        # what a rewrite would write
  python3 tools/categorize.py --stats   # counts per category and confidence

Every `game/*.cpp` unit in config/G4SE01/splits.txt gets one row:

  unit  category  confidence  evidence  source

category is a key of CATEGORIES (the progress site's eight groups of game
code, docs/names/METHOD.md "Categories"); confidence is high, med or low (the
evidence's honest grade, like names.tsv); source is `auto` (this tool's rules,
recomputed on every run) or `hand` (a reviewed placement: kept as written, so
edit a row and set its source to hand to pin it). Rows for units that left
splits.txt are dropped, with a warning for hand rows.

The automatic rules, in order (first match wins):

  1. the ten Player units (docs/PLAYER.md "Units")                  -> player
  2. units that define a base class or a game-wide system (BASES)   -> core
  3. actors: the spawner constructs a class of the unit
     (game_units.tsv "actor spawner"), an act_* file, or a class that
     derives from ActorBase_804A5770. The actor's kind in
     docs/names/actors.tsv (by the spawner's IDs, else by suggested_file)
     and the class hierarchy (EnemyBase_804A4808 descendants) decide:
       enemy / boss                    -> enemies
       npc, or a "talking" / "conversation" system actor -> npcs (on EnemyBase: med at most)
       object / system on EnemyBase    -> enemies (low: the two disagree)
       object / effect / system / debug -> objects
       no kind, EnemyBase subclass     -> enemies (med)
       no kind                         -> objects (low)
  4. a unit still without a kind that writes methods of a class another unit
     declares takes that unit's category, one grade lower
  5. anything else stays unplaced: core, low, "no evidence yet" (pin a hand
     row once the unit's role is known).

tools/site_data.py reads the TSV; a unit without a row is shown under Game
core, unconfirmed, and listed as a warning.
"""
import argparse
import csv
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "docs/names/categories.tsv"
SPLITS = ROOT / "config/G4SE01/splits.txt"
GAME_UNITS = ROOT / "config/G4SE01/game_units.tsv"
ACTORS = ROOT / "docs/names/actors.tsv"
PLAYER_MD = ROOT / "docs/PLAYER.md"

# key, name (the site's order); site_data.py adds colours and descriptions
CATEGORIES = [
    ("player", "Player"),
    ("enemies", "Enemies & bosses"),
    ("npcs", "NPCs"),
    ("objects", "Objects & gimmicks"),
    ("stage", "Stage & world"),
    ("menus", "Menus & screens"),
    ("core", "Game core"),
    ("media", "Media & link"),
]
KEYS = [k for k, _ in CATEGORIES]
CONFIDENCE = ("high", "med", "low")
SOURCES = ("auto", "hand")
HEADER = ("# Category of every game unit, for the progress site (tools/site_data.py). "
          "Method: docs/names/METHOD.md \"Categories\".\n"
          "# Columns: unit, category (" + " | ".join(KEYS) + "), confidence (high|med|low), "
          "evidence, source (auto: tools/categorize.py's rules, rewritten on every run | "
          "hand: reviewed, kept as written).\n"
          "# Regenerate: python3 tools/categorize.py   Check: python3 tools/categorize.py --check\n")
COLUMNS = ("unit", "category", "confidence", "evidence", "source")

# Units that hold a base class or a game-wide system: Game core whatever the
# hierarchy rule would say (their classes are the bases the rule looks for).
BASES = {
    "tu_801F6058": "ActorBase_804A5770 (ActorBase.h): the base of every actor",
    "tu_801F9484": "ActorManager_804A5800, spawnActor_801F9484 (ActorManager.h)",
    "tu_801CDC8C": "EnemyBase_804A4808 (EnemyBase.h): the enemies' base class",
    "tu_8013C5C8": "EffectSystem_8049F178, spawnEmitter_8013CC50 (Effect.h): the effect spawner",
}
ENEMY_BASE = "EnemyBase_804A4808"
ACTOR_BASE = "ActorBase_804A5770"
TALK = re.compile(r"talking|conversation", re.I)  # actors.tsv names of the actors that hold conversations
KIND_CAT = {"enemy": "enemies", "boss": "enemies", "npc": "npcs", "object": "objects",
            "effect": "objects", "system": "objects", "debug": "objects"}


def stem_of(unit):
    return re.sub(r"\.[^.]*$", "", unit.rsplit("/", 1)[-1])


def game_units():
    """The `game/*.cpp` units of splits.txt, in file order."""
    return [line[:-1] for line in SPLITS.read_text().splitlines()
            if line.startswith("game/") and line.endswith(":") and not line[0].isspace()]


def player_units():
    text = PLAYER_MD.read_text()
    m = re.search(r"^## Units\n(.*?)(?=^## )", text, flags=re.M | re.S)
    return set(re.findall(r"^\| (tu_[0-9A-F]{8}) \|", m.group(1) if m else "", flags=re.M))


def class_graph():
    """class -> base, from every `class X : public Y` in the game headers and sources."""
    base = {}
    files = list((ROOT / "include/game").rglob("*.h")) + list((ROOT / "src/game").glob("*.cpp"))
    for p in sorted(files):
        for x, y in re.findall(r"\b(?:class|struct)\s+(\w+)\s*:\s*public\s+(\w+)", p.read_text(errors="replace")):
            base.setdefault(x, y)
    return base


def ancestors(cls, base):
    out, seen = [], set()
    while cls in base and cls not in seen:
        seen.add(cls)
        cls = base[cls]
        out.append(cls)
    return out


def unit_classes(stem, base):
    """Classes the unit defines: methods `X::` in its source, classes declared in
    its own act header."""
    found = set()
    src = ROOT / "src/game" / f"{stem}.cpp"
    if src.exists():
        found |= set(re.findall(r"\b(\w+)::~?\w+\s*\(", src.read_text(errors="replace")))
    hdr = ROOT / "include/game/act" / f"{stem}.h"
    if hdr.exists():
        found |= set(re.findall(r"\b(?:class|struct)\s+(\w+)\s*:\s*public", hdr.read_text(errors="replace")))
    return {c for c in found if c in base}


def actor_rows():
    """actor ID -> row, suggested_file stem -> [rows]."""
    by_id, by_file = {}, defaultdict(list)
    with open(ACTORS) as f:
        for r in csv.DictReader(f, delimiter="\t"):
            by_id[r["id"]] = r
            if r.get("suggested_file"):
                by_file[r["suggested_file"]].append(r)
    return by_id, by_file


def spawner_ids():
    """unit -> actor IDs whose constructor the spawner calls in it."""
    ids = defaultdict(list)
    with open(GAME_UNITS) as f:
        for r in csv.DictReader(f, delimiter="\t"):
            ev = r.get("name_evidence") or ""
            if "actor spawner" in ev:
                ids[r["unit"]] += re.findall(r"\b([A-Za-z0-9]{4}) \((?:vtable|ctor)", ev)
    return ids


def auto_row(unit, ctx):
    """(category, confidence, evidence) from the automatic rules, or None."""
    stem = stem_of(unit)
    if stem in ctx["player"]:
        return "player", "high", "Player unit (PLAYER.md)"
    if stem in BASES:
        return "core", "high", BASES[stem]
    base = ctx["base"]
    classes = unit_classes(stem, base)
    anc = {c: ancestors(c, base) for c in classes}
    is_enemy = sorted(c for c, a in anc.items() if ENEMY_BASE in a)
    is_actor = any(ACTOR_BASE in a for a in anc.values())
    ids = ctx["ids"].get(unit, [])
    rows = [ctx["by_id"][i] for i in ids if i in ctx["by_id"]]
    how = "actor spawner"
    if not rows:
        rows = ctx["by_file"].get(stem, [])
        how = "suggested_file"
    if not (ids or stem.startswith("act_") or is_actor or rows):
        return None
    hier = f"EnemyBase subclass ({is_enemy[0]})" if is_enemy else ""
    kinds = Counter(KIND_CAT.get(r["kind"]) for r in rows if KIND_CAT.get(r["kind"]))
    if not kinds:
        if is_enemy:
            return "enemies", "med", hier + "; no actors.tsv kind"
        what = "ActorBase subclass" if is_actor else ("actor spawner: " + ",".join(ids) if ids else "act_* file")
        return "objects", "low", f"{what}; no actors.tsv kind"
    cat, n = kinds.most_common(1)[0]
    talks = lambda r: TALK.search(r["english_name"]) and r["kind"] in ("system", "debug")
    if cat == "objects" and any(talks(r) for r in rows):
        cat = "npcs"  # a conversation actor that actors.tsv files under system
        kinds = Counter({"npcs": n})
    mine = [r for r in rows if KIND_CAT.get(r["kind"]) == cat or cat == "npcs" and talks(r)]
    names = ", ".join(f"{r['id']} {r['english_name']}" for r in mine[:2]) + (" +more" if len(mine) > 2 else "")
    kind = "/".join(sorted({r["kind"] for r in mine}))
    grades = {r["confidence"] for r in mine}
    conf = "high" if "high" in grades else "med" if "med" in grades else "low"
    ev = f"actors.tsv ({how}): {kind} {names}"
    if len(kinds) > 1:
        conf = "low" if conf == "low" else "med"
        ev += "; other IDs: " + ", ".join(f"{k} {v}" for k, v in kinds.items() if k != cat)
    if is_enemy:
        if cat == "enemies":
            conf = "high" if conf != "low" else "med"
            ev = hier + "; " + ev
        elif cat == "npcs":  # an NPC built on the enemy base (Navi, Kaepora Gaebora): the actor wins
            conf = "med" if conf == "high" else "low"
            ev += f"; but {hier}"
        else:  # an object or manager on the enemy base: the hierarchy wins, unconfirmed
            cat, conf = "enemies", "low"
            ev = f"{hier}; but {ev}"
    elif cat == "enemies" and is_actor and conf == "high":
        conf = "med"  # an enemy outside EnemyBase: the name alone says enemy
    if kind in ("debug",):
        conf = "low"
    return cat, conf, ev


def read_tsv():
    if not OUT.exists():
        return {}
    rows = {}
    for n, line in enumerate(OUT.read_text().splitlines(), 1):
        if not line.strip() or line.startswith("#") or line.startswith("unit\t"):
            continue
        f = line.split("\t")
        if len(f) != len(COLUMNS):
            sys.exit(f"{OUT.relative_to(ROOT)}:{n}: expected {len(COLUMNS)} columns, got {len(f)}")
        rows[f[0]] = dict(zip(COLUMNS, f))
    return rows


def problems(row):
    out = []
    if row["category"] not in KEYS:
        out.append(f"category {row['category']!r} (one of {', '.join(KEYS)})")
    if row["confidence"] not in CONFIDENCE:
        out.append(f"confidence {row['confidence']!r}")
    if row["source"] not in SOURCES:
        out.append(f"source {row['source']!r}")
    if not row["evidence"].strip():
        out.append("empty evidence")
    return out


def generate():
    ctx = {"player": player_units(), "base": class_graph(), "ids": spawner_ids()}
    ctx["by_id"], ctx["by_file"] = actor_rows()
    old = read_tsv()
    units = game_units()
    auto = {u: auto_row(u, ctx) for u in units}
    # a unit with no kind of its own that writes methods of a class another unit
    # declares (its act header or source) takes that unit's category, one grade lower
    owner = {}
    for u in units:
        stem = stem_of(u)
        for p in (ROOT / "include/game/act" / f"{stem}.h", ROOT / "src/game" / f"{stem}.cpp"):
            if p.exists():
                for c in re.findall(r"\b(?:class|struct)\s+(\w+)\s*:\s*public", p.read_text(errors="replace")):
                    owner.setdefault(c, u)
    weak = lambda a: a is None or a[2].endswith("no actors.tsv kind") and a[1] == "low"
    for u in units:
        if not weak(auto[u]):
            continue
        for c in sorted(unit_classes(stem_of(u), ctx["base"])):
            o = owner.get(c)
            if o and o != u and not weak(auto[o]) and auto[o][0] != "core":
                cat, conf, _ = auto[o]
                auto[u] = (cat, {"high": "med"}.get(conf, "low"), f"methods of {c} ({stem_of(o)}: {cat})")
                break
    out = []
    for u in units:
        o = old.get(u)
        if o and o["source"] == "hand":
            out.append(o)
            continue
        cat, conf, ev = auto[u] or ("core", "low", "no evidence yet: role not identified")
        out.append({"unit": u, "category": cat, "confidence": conf, "evidence": ev, "source": "auto"})
    gone = [u for u, r in old.items() if u not in set(units)]
    return out, gone, old


def render(rows):
    lines = [HEADER.rstrip("\n"), "\t".join(COLUMNS)]
    lines += ["\t".join(r[c].replace("\t", " ") for c in COLUMNS) for r in rows]
    return "\n".join(lines) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--stats", action="store_true")
    args = ap.parse_args()
    rows, gone, old = generate()
    bad = [f"{r['unit']}: {p}" for r in rows for p in problems(r)]
    for u in gone:
        print(f"warning: {u} is no longer in splits.txt; its {old[u]['source']} row is dropped", file=sys.stderr)
    if args.stats or not args.check:
        cats = Counter(r["category"] for r in rows)
        confs = Counter((r["category"], r["confidence"]) for r in rows)
        for k, n in CATEGORIES:
            print(f"{n:20s} {cats[k]:4d}  " + "  ".join(f"{c} {confs[(k, c)]}" for c in CONFIDENCE))
        print(f"{'total':20s} {len(rows):4d}  hand {sum(r['source'] == 'hand' for r in rows)}")
    if bad:
        sys.exit("categories.tsv:\n  " + "\n  ".join(bad))
    text = render(rows)
    if args.check:
        missing = [u for u in game_units() if u not in old]
        if missing:
            sys.exit(f"categories.tsv: {len(missing)} units have no row: {', '.join(missing[:8])}")
        if not OUT.exists() or OUT.read_text() != text:
            sys.exit("categories.tsv is stale: run python3 tools/categorize.py")
        print(f"categories.tsv: {len(rows)} units OK")
        return
    if not args.stats:
        OUT.write_text(text)


if __name__ == "__main__":
    main()
