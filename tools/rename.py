#!/usr/bin/env python3
"""Apply or revert the evidence-based names manifest (docs/names/names.tsv).

Usage:
  python3 tools/rename.py                 # dry run: state of every row, nothing written
  python3 tools/rename.py --apply         # old -> new for every applicable row
  python3 tools/rename.py --revert        # new -> old for every applicable row
  python3 tools/rename.py ... --verify    # then run ninja and compare the DOL SHA-1
  python3 tools/rename.py ... --only NAME[,NAME]   # restrict to rows whose old or new name is listed

Method and evidence rules: docs/names/METHOD.md. The manifest is the only
input: the tool is re-runnable after other branches merge (rows already
applied are skipped, rows a merge reintroduced old names for are finished)
and --revert undoes it exactly, because every new name carries its address
(or, at high confidence, was checked to exist nowhere else) and so never
occurred in the tree before the rename.

Manifest columns (tab separated, `#` comments allowed):
  address     8 hex digits: the vtable (class), the function, or the object (global)
  kind        class | function | global
  old         the current identifier (dtk placeholder or provisional name)
  new         the new identifier; must end in _<address> unless confidence is high
  confidence  high | med | low   (low rows are recorded, never applied)
  evidence    citations (string address + text, donor file + function, msg#, actor ID, ...)

What a row rewrites, in every tracked text file except the excluded ones:
  * the plain identifier `old` (word-bounded; `old__...` mangled tails count,
    `old.h` / `old.cpp` file names do not), and
  * for a class, its CodeWarrior-mangled form `<len(old)>old` (e.g.
    `__dt__12cls_804B1378Fv`), with the length prefix re-computed.
A class name inside a mangled template argument (`<...>`) would change the
template's own length prefix: such a row is refused.

A row is BLOCKED (reported, not applied) when its old name occurs in a file
the rename must not touch: the tools (tools/**/*.py), configure.py outside
its `#` comment lines, or the protected config files. Such names need a
coordinator change to the tool first (refer to the entity by address).

Generator-owned files (the units of config/G4SE01/gen_game.tsv: their
sources and act headers) are renamed like any source: tools/gen_game.py
follows the manifest. It takes the names it emits from symbols.txt (by
address) and reads its own records, the gen_game.tsv rows and the
pre-generation blobs they pin, through to_current() below, which brings a
text to the names the tree currently uses (an applied row's old name ->
new, an unapplied row's new name -> old). So gen_game.tsv and the pinned
blobs stay as recorded, and --apply / --revert need no generator edit.
configure.py's comment lines are renamed too (its code never is).
"""

import argparse
import hashlib
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MANIFEST = ROOT / "docs/names/names.tsv"
SYMBOLS = "config/G4SE01/symbols.txt"
SHA1_FILE = ROOT / "config/G4SE01/build.sha1"

APPLY_CONF = {"high", "med"}
KINDS = {"class", "function", "global"}

# Never rewritten. Paths are relative to the repository root.
EXCLUDE_PREFIX = ("build/", "orig/", "site/", ".github/", "tools/", "config/")
EXCLUDE_FILES = {"docs/names/names.tsv", "docs/names/METHOD.md",
                 "objdiff.json", "progress/claims.json"}
INCLUDE_EXTRA = {SYMBOLS}  # the one config file that is rewritten
COMMENT_ONLY = {"configure.py"}  # only its full-line `#` comments are rewritten
TEXT_SUFFIX = {".c", ".cpp", ".cp", ".h", ".hpp", ".inc", ".s", ".md", ".txt", ".tsv", ".py", ".yml", ""}

# Blocking: an old name here means "do not rename" (generator / tooling / protected).
BLOCK_PREFIX = ("tools/",)
BLOCK_FILES = {"config/G4SE01/config.yml",
               "config/G4SE01/approved_symbol_changes.txt", "config/G4SE01/provenance_reviewed.txt",
               "config/G4SE01/splits.txt"}

IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
ADDR_SUFFIX = re.compile(r"_([0-9A-F]{8})\Z")


def git_files():
    out = subprocess.run(["git", "ls-files", "-z"], cwd=ROOT, capture_output=True, check=True)
    return [f for f in out.stdout.decode().split("\0") if f]


def comment_lines(text, keep):
    """configure.py: its full-line `#` comments (keep=True) or everything
    else (keep=False), the other lines blanked so line structure stays."""
    return "\n".join(l if l.lstrip().startswith("#") == keep else ""
                     for l in text.split("\n"))


def on_comments(text, a, b, kind):
    """substitute() on the full-line comments of text only."""
    out, n = [], 0
    for l in text.split("\n"):
        if l.lstrip().startswith("#"):
            l, k = substitute(l, a, b, kind)
            n += k
        out.append(l)
    return "\n".join(out), n


def read_manifest():
    rows, problems = [], []
    for n, line in enumerate(MANIFEST.read_text().splitlines(), 1):
        if not line.strip() or line.startswith("#"):
            continue
        f = line.split("\t")
        if f[0] == "address":
            continue
        if len(f) != 6:
            problems.append(f"names.tsv:{n}: expected 6 columns, got {len(f)}")
            continue
        addr, kind, old, new, conf, ev = f
        r = {"line": n, "addr": addr, "kind": kind, "old": old, "new": new, "conf": conf, "ev": ev}
        if not re.fullmatch(r"[0-9A-F]{8}", addr):
            problems.append(f"names.tsv:{n}: address {addr!r} is not 8 upper-case hex digits")
        if kind not in KINDS:
            problems.append(f"names.tsv:{n}: kind {kind!r} not one of {sorted(KINDS)}")
        if conf not in ("high", "med", "low"):
            problems.append(f"names.tsv:{n}: confidence {conf!r}")
        for x in (old, new):
            if not IDENT.match(x):
                problems.append(f"names.tsv:{n}: {x!r} is not an identifier")
        if conf != "high":
            m = ADDR_SUFFIX.search(new)
            if not m or m[1] != addr:
                problems.append(f"names.tsv:{n}: {new} must keep its address (_{addr}) below high confidence")
        if not ev.strip():
            problems.append(f"names.tsv:{n}: no evidence")
        rows.append(r)
    seen_old, seen_new = {}, {}
    for r in rows:
        for key, seen in (("old", seen_old), ("new", seen_new)):
            if r[key] in seen:
                problems.append(f"names.tsv:{r['line']}: {key} name {r[key]} also on line {seen[r[key]]}")
            seen[r[key]] = r["line"]
    for r in rows:
        if r["new"] in seen_old or r["old"] in seen_new:
            problems.append(f"names.tsv:{r['line']}: chained rename ({r['old']} -> {r['new']}); not allowed")
    return rows, problems


def patterns(a, kind):
    """Regexes for identifier a: (plain, mangled or None)."""
    plain = re.compile(r"(?<![A-Za-z0-9_])" + re.escape(a) +
                       r"(?=__|(?![A-Za-z0-9_]))(?!\.(?:h|hpp|cpp|cp|c|s|inc|o|md)\b)")
    mangled = None
    if kind == "class":
        # The length prefix delimits a mangled name; what follows is the
        # next mangled component (F, C, Fv, ...), so no lookahead.
        mangled = re.compile(re.escape(f"{len(a)}{a}"))
    return plain, mangled


def substitute(text, a, b, kind):
    plain, mangled = patterns(a, kind)
    n = 0
    if mangled:
        text, k = mangled.subn(f"{len(b)}{b}", text)
        n += k
    text, k = plain.subn(b, text)
    return text, n + k


def count(text, a, kind):
    plain, mangled = patterns(a, kind)
    return len(plain.findall(text)) + (len(mangled.findall(text)) if mangled else 0)


def parse_symbols(text):
    """{name: address} plus the list of (name, address)."""
    out = []
    for line in text.splitlines():
        m = re.match(r"(\S+) = [^:]+:0x([0-9A-Fa-f]{8});", line)
        if m:
            out.append((m[1], m[2].upper()))
    return out


def base_name(sym):
    """fn_801F666C__21ActorManager_804A5800Fl -> fn_801F666C; __vt__... unchanged."""
    if sym.startswith("__"):
        return sym
    i = sym.find("__")
    return sym if i <= 0 else sym[:i]


def symbol_problems(r, syms, name):
    """Does symbols.txt agree that `name` (old or new) is the entity at r['addr']?"""
    a, kind = r["addr"], r["kind"]
    if kind in ("function", "global"):
        hits = [ad for s, ad in syms if base_name(s) == name]
        if not hits:
            return None  # not in symbols.txt (yet); checked by the caller
        bad = [ad for ad in hits if ad != a]
        return f"{name} names symbol(s) at {', '.join(bad)}, not {a}" if bad else ""
    vt = [ad for s, ad in syms if s == f"__vt__{len(name)}{name}"]
    if vt:
        return "" if vt == [a] else f"__vt__ of {name} is at {', '.join(vt)}, not {a}"
    return None


def row_state(r, syms):
    """True when symbols.txt holds the row's new name (applied), False when
    it holds the old one, None when it holds neither."""
    def present(name):
        if r["kind"] == "class":
            pre = f"{len(name)}{name}"
            return any(pre in s for s, _ in syms)
        return any(base_name(s) == name and ad == r["addr"] for s, ad in syms)
    if present(r["new"]):
        return True
    if present(r["old"]):
        return False
    return None


_current = None


def current_renames():
    """[(from, to, kind)] that bring a text to the tree's current names: an
    applied row old -> new, an unapplied one new -> old (high/med rows whose
    state symbols.txt shows)."""
    global _current
    if _current is None:
        rows, problems = read_manifest()
        if problems:
            raise SystemExit("names.tsv: " + "; ".join(problems[:3]))
        syms = parse_symbols((ROOT / SYMBOLS).read_text())
        _current = []
        for r in rows:
            st = row_state(r, syms) if r["conf"] in APPLY_CONF else None
            if st is True:
                _current.append((r["old"], r["new"], r["kind"]))
            elif st is False:
                _current.append((r["new"], r["old"], r["kind"]))
    return _current


def to_current(text):
    """text (a generator record: a pinned blob, a gen_game.tsv field) in the
    names the tree uses now. Every new name carries its address (or is
    unique, for high rows), so neither direction can hit another name."""
    if text is None:
        return None
    for a, b, kind in current_renames():
        text, _ = substitute(text, a, b, kind)
    return text


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--apply", action="store_true")
    g.add_argument("--revert", action="store_true")
    ap.add_argument("--verify", action="store_true", help="run ninja and compare the DOL SHA-1 afterwards")
    ap.add_argument("--only", default="", help="comma-separated old or new names")
    ap.add_argument("-q", "--quiet", action="store_true")
    args = ap.parse_args(argv)

    rows, problems = read_manifest()
    if problems:
        print("\n".join(problems))
        return 2
    only = {x for x in args.only.split(",") if x}
    if only:
        rows = [r for r in rows if r["old"] in only or r["new"] in only]

    files = git_files()
    scan = [f for f in files if (f in INCLUDE_EXTRA or not (f.startswith(EXCLUDE_PREFIX) or f in EXCLUDE_FILES))
            and Path(f).suffix in TEXT_SUFFIX]
    self_ = {"tools/rename.py", "tools/names_compare.py"}  # their docs cite example names
    block = [f for f in files if f.startswith(BLOCK_PREFIX) and f.endswith(".py") and f not in self_] + \
            [f for f in BLOCK_FILES if (ROOT / f).exists()]
    texts = {}
    for f in scan:
        try:
            texts[f] = (ROOT / f).read_text(encoding="utf-8")
        except (UnicodeDecodeError, FileNotFoundError, IsADirectoryError):
            pass
    btexts = {}
    # blobs.txt: only a `blob` line's symbol column is functional (array lines are prose).
    blobs = ROOT / "config/G4SE01/blobs.txt"
    if blobs.exists():
        btexts["config/G4SE01/blobs.txt"] = "\n".join(
            l.split()[1] for l in blobs.read_text().splitlines() if l.startswith("blob ") and len(l.split()) > 1)
    for f in block:
        try:
            btexts[f] = (ROOT / f).read_text(encoding="utf-8", errors="replace")
        except (FileNotFoundError, IsADirectoryError):
            pass
    for f in COMMENT_ONLY:  # code blocks, comments are renamed
        if f in texts:
            btexts[f] = comment_lines(texts[f], keep=False)
    ctexts = {f: comment_lines(t, keep=True) if f in COMMENT_ONLY else t for f, t in texts.items()}
    syms = parse_symbols(texts[SYMBOLS])

    forward = not args.revert
    todo, report = [], {"apply": 0, "done": 0, "blocked": 0, "low": 0, "missing": 0, "refused": 0}
    for r in rows:
        src, dst = (r["old"], r["new"]) if forward else (r["new"], r["old"])
        tag = f"{r['old']} -> {r['new']}"
        if r["conf"] not in APPLY_CONF:
            report["low"] += 1
            continue
        # Blocked: the old name is spelled out where the rename must not go.
        hits = [f for f, t in btexts.items() if count(t, r["old"], r["kind"])]
        if hits:
            report["blocked"] += 1
            if not args.quiet:
                print(f"BLOCKED  {tag}: in {', '.join(hits[:4])}{' ...' if len(hits) > 4 else ''}")
            continue
        n_src = sum(count(t, src, r["kind"]) for t in ctexts.values())
        n_dst = sum(count(t, dst, r["kind"]) for t in ctexts.values())
        why = []
        if r["kind"] == "class":
            tmpl = re.compile(r"<[^<>]*" + re.escape(f"{len(src)}{src}"))
            if any(tmpl.search(t) for t in texts.values()):
                why.append("used inside a mangled template argument")
        # symbols.txt must agree on the address for whichever name it holds.
        s_src = symbol_problems(r, syms, src)
        s_dst = symbol_problems(r, syms, dst)
        for s in (s_src, s_dst):
            if s:
                why.append(s)
        if r["kind"] in ("function", "global") and s_src is None and s_dst is None:
            why.append(f"neither {src} nor {dst} is a symbol at {r['addr']} in symbols.txt")
        if r["kind"] == "class" and s_src is None and s_dst is None and not src.endswith(r["addr"]):
            why.append(f"no __vt__ for {src} and its name does not carry {r['addr']}")
        if why:
            report["refused"] += 1
            print(f"REFUSED  {tag}: {'; '.join(why)}")
            continue
        if n_src == 0:
            report["done" if n_dst else "missing"] += 1
            if not n_dst:
                print(f"MISSING  {tag}: neither name occurs")
            continue
        report["apply"] += 1
        todo.append((r, src, dst))
        if not args.quiet:
            print(f"{'APPLY ' if forward else 'REVERT'}   {tag}: {n_src} occurrence(s)"
                  + (f" ({n_dst} already renamed)" if n_dst else ""))

    print("rename: " + ", ".join(f"{k} {v}" for k, v in report.items()))
    if report["refused"]:
        print("rename: refused rows above; nothing written")
        return 1
    if not (args.apply or args.revert):
        return 0

    changed = set()
    for r, src, dst in todo:
        for f, t in texts.items():
            t2, k = (on_comments if f in COMMENT_ONLY else substitute)(t, src, dst, r["kind"])
            if k:
                texts[f] = t2
                changed.add(f)
    for f in sorted(changed):
        (ROOT / f).write_text(texts[f], encoding="utf-8")
    print(f"rename: {len(changed)} file(s) written")

    if args.verify:
        return verify()
    return 0


def verify():
    out = subprocess.run(["ninja"], cwd=ROOT)
    if out.returncode:
        print("verify: ninja failed")
        return 1
    want, path = SHA1_FILE.read_text().split()[:2]
    have = hashlib.sha1((ROOT / path).read_bytes()).hexdigest()
    print(f"verify: {path} {have} {'== build.sha1' if have == want else '!= build.sha1 ' + want}")
    return 0 if have == want else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
