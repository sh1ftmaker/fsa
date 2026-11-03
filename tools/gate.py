#!/usr/bin/env python3
"""Submission gate: reject matches that come from tricks rather than source.

Usage:
  python3 tools/gate.py [--base REF]     # check the branch's changes against REF (default: matching)
  python3 tools/gate.py --all-units      # only the report and link checks, over every unit
  python3 tools/gate.py --no-link        # skip the link (quick iteration only; merges link)

Checks (see docs/PLAN.md, "Submission gate"):
  * changed C/C++ sources: no inline asm, #pragma or goto that wasn't
    already in that file (or in the donor file of the same name), no m2c
    leftovers, no field access through casts and offsets, no .incbin;
  * protected files untouched: measuring tools, objdiff/report settings, tool
    versions, orig/;
  * symbols.txt: real (non-placeholder) symbols keep their address, size,
    type and name; placeholder symbols (fn_, lbl_, *_80xxxxxx) may be
    renamed, resized or split into the real symbols they cover;
  * no unit marked Matching contains a fakematch (FakeMatch headers,
    force_order dummies, "fakematch" comments), donor-imported or not;
  * every unit marked Matching has source and a compiled object (configure.py
    would otherwise link dtk's split of the original in its place) and is
    100 % in code and data in the honest report
    (build/G4SE01/report_honest.json, every unit diffed, no symbol_mappings);
  * objdiff.json has no symbol_mappings (objdiff's report honors them);
  * build/G4SE01/weakdedup_manifest.json: every extab entry the honest report
    left out belongs to a WEAK FUNC, and its map evidence (UNREFERENCED
    DUPLICATE in this object, extab and extabindex labels UNUSED) is in the
    map of a current link that includes this exact object and whose DOL
    matches build.sha1; every stripped object differs from the compiled one
    only by those entries, and no other unit is diffed against a copy;
    every dead-stripped data object it left out is listed UNUSED (with its
    size) for this object in that map, every allocated location referencing
    it lies in a function or object the map lists UNUSED for this object,
    only zero padding goes with it, and the map places every kept symbol of
    its section where the stripped copy has it;
  * binary blobs (check_blobs): config.yml (read with yaml.safe_load, fail
    closed) has only a top-level extract: list of {symbol, header: assets/...,
    header_type: raw}, 1:1 with config/G4SE01/blobs.txt's `blob` lines; each
    is a data-section object in symbols.txt of exactly the listed address and
    size (at most BLOB_MAX), inside its unit's splits.txt range, not a
    vtable/RTTI/compiler symbol, and without relocations in dtk's split of
    the original. Include paths are normalized as MWCC resolves them (line
    continuations spliced, backslash as /, ./ dropped, any case): the blob header is
    included once, unconditionally, by the listed unit only; no other
    include names assets/, a blob's or blob unit's file name, or build/; an
    initializer includes nothing but a blob header; no computed #include;
    configure.py names no include root under build/ but
    build/{config.version}/include; git tracks no symlink, nothing under a
    directory named assets (any case) and no file named like a blob header;
  * hand-typed bytes (check_byte_arrays, a tripwire; review is the real
    check): in a changed file of any source-like kind, an edited initializer
    of more than 64 integer literals (or 128 bytes' worth), a gain of 256
    literals (or 512 bytes' worth) across the file's initializers, a longer
    bare literal list over 64, or a gain of more than 128 escaped bytes in
    strings, unless blobs.txt has an `array` line for the file;
  * main.dol links and its SHA-1 equals build.sha1 (ninja build/G4SE01/ok);
    if not, tools/link_check.py names the Matching units at fault.

The gate runs ninja itself (report_honest.json, then the link), so never run
it while another ninja runs in the same worktree.

Exit status 0 when everything passes.
"""

import argparse
import hashlib
import json
import struct
import re
import subprocess
import sys
from pathlib import Path

APPROVALS = Path("config/G4SE01/approved_symbol_changes.txt")
PROTECTED = [
    "config/G4SE01/approved_symbol_changes.txt", "config/G4SE01/provenance_reviewed.txt",
    "tools/gate.py", "tools/fdiff.py", "tools/progress.py", "tools/project.py",
    "tools/honest_report.py", "tools/link_check.py",
    # The SHA-1 the link is checked against, and config.yml's hash: of the original DOL.
    "config/G4SE01/build.sha1", "config/G4SE01/config.yml",
    # Approved binary blobs (and large literal arrays), coordinator-only.
    "config/G4SE01/blobs.txt",
    "tools/data_splits.py", "orig/",
    # Build artifacts (maps, weak-dedup evidence, objects) are never committed.
    "build/",
]
PROTECTED_CONFIGURE = re.compile(r"functionRelocDiffs|progress_report_args|_tag\s*=|report|objdiff"
                                 r"|-(?:i|I|ir|I-)\s*\S*(?i:build)[/\\]")  # include roots under build/ (extracted blobs)
# Link and SHA-1 settings in configure.py (unit Object(...) lines, which carry
# FIXME(link) comments, are exempt).
LINK_CONFIGURE = re.compile(r"check_sha|sha1|ldflags|linker_version|link_order|ldscript|custom_build"
                            r"|non_matching|^[+-]\s*(Matching|NonMatching|Equivalent)\s*=")
SOURCE = re.compile(r"\.(c|cpp|cp|cc|cxx|h|hpp|hh|inc|inl)$", re.I)
def _donors_dir():
    """fsadecomp/donors, found through the main checkout (git's common dir), so
    the gate finds it from the main checkout and from every worktree's copy."""
    common = subprocess.run(["git", "rev-parse", "--path-format=absolute", "--git-common-dir"],
                            capture_output=True, text=True).stdout.strip()
    return Path(common).parent.parent / "donors" if common else Path(__file__).resolve().parent.parent.parent / "donors"


DONORS = _donors_dir()

BANNED = [
    (re.compile(r"\bregister\b[^;]*\basm\b"), "register bound to a machine register"),
    (re.compile(r"\bM2C_|\bsaved_reg_|\bunksp|\bvar_[rf]\d+\b|\bsp[0-9A-F]{2,}\b|\bMULT_HI\b|\?\s*/\*\s*unknown"), "m2c leftover"),
    (re.compile(r"\.incbin|INCBIN"), "embedded binary"),
]
RESTRICTED = [
    # Donor library code (MSL alloc.c) does this in its matched source; new code may not.
    (re.compile(r"\*\s*\(\s*[\w\s]+\*+\s*\)\s*\(\s*\(\s*(u8|s8|char|unsigned char|u32|uintptr_t)\s*\*?\s*\)"), "field access through a cast and an offset"),
    # Original asm functions from a donor's matched source are source; new asm is not.
    (re.compile(r"\basm\b\s*[({]|__asm\b|\basm\s+\w+[^;]*\("), "inline asm"),
    (re.compile(r"^\s*#\s*pragma\b.*"), "#pragma"),
    (re.compile(r"\bgoto\s+\w+\s*;"), "goto"),
]
# Placeholders: dtk's names, address-suffixed provisional names (also inside mangled names,
# e.g. init__18ActorBase_804A5770Fv), cls_<addr> classes and provisional game-unit __sinit names.
PLACEHOLDER = re.compile(r"^(fn_|lbl_|auto_|func_|jumptable_|@)|_80[0-9A-Fa-f]{6}(?![0-9a-z])|cls_[0-9A-Fa-f]{8}|__sinit_tu_[0-9A-Fa-f]{8}")
SYMBOL_LINE = re.compile(r"^(\S+) = (\S+):(0x[0-9A-Fa-f]+); // (.*)$")


def git(*args):
    return subprocess.run(["git", *args], capture_output=True, text=True, check=True).stdout


def added_lines(base, path):
    diff = git("diff", "-U0", f"{base}...HEAD", "--", path) + git("diff", "-U0", "HEAD", "--", path)
    return [line[1:] for line in diff.splitlines() if line.startswith("+") and not line.startswith("+++")]


def file_at(base, path):
    try:
        return git("show", f"{base}:{path}")
    except subprocess.CalledProcessError:
        return ""


# Only donors whose code may be copied count as a precedent: a LICENSE file at
# the top, and not MetroidPrimePort (its NOTICE excludes extern/sdk and the game).
UNUSABLE_DONORS = {"Odrannnn__MetroidPrimePort"}


def licensed_donors():
    for repo in sorted(DONORS.iterdir()):
        if repo.name in UNUSABLE_DONORS or not repo.is_dir():
            continue
        if any(p.name.upper().startswith(("LICENSE", "COPYING", "UNLICENSE")) for p in repo.iterdir()):
            yield repo


def donor_texts(path, suffix=False):
    """Same-named files in licensed donors (with suffix=True also e.g. dsp_GBAKey.c for GBAKey.c)."""
    name = Path(path).name
    if not DONORS.is_dir():
        return []
    texts = []
    pattern = f"**/*{name}" if suffix else f"**/{name}"
    for hit in (h for repo in licensed_donors() for h in repo.glob(pattern)):
        try:
            texts.append(hit.read_text(errors="replace"))
        except OSError:
            pass
    return texts


IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]{3,}")


COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)


def strip_comments(text):
    return COMMENT.sub(" ", text)


INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)
INCLUDE_ROOTS = ("include", "libs/sdk/include", "libs/sdk/msl_include")


def included_identifiers(path, text, skip=()):
    """Identifiers declared in the headers this file includes directly (the API it
    uses, which any independent writer shares). A header the same change added
    doesn't count: it could carry copied names."""
    ids = set()
    for inc in INCLUDE.findall(text):
        for root in (Path(path).parent, *map(Path, INCLUDE_ROOTS)):
            h = root / inc
            if h.is_file() and str(h) not in skip:
                ids |= set(IDENT.findall(strip_comments(h.read_text(errors="replace"))))
                break
    return ids


def unlicensed_texts(path):
    """Same-named files in donors whose code may not be copied."""
    name = Path(path).name
    if not DONORS.is_dir():
        sys.exit(f"gate: donors directory {DONORS} not found; the provenance check can't run")
    ok = {r.name for r in licensed_donors()}
    texts = []
    for repo in sorted(DONORS.iterdir()):
        if repo.is_dir() and repo.name not in ok:
            for hit in repo.glob(f"**/{name}"):
                try:
                    texts.append((repo.name, hit.read_text(errors="replace")))
                except OSError:
                    pass
    return texts


def check_provenance(base, changed, problems):
    """A changed source whose distinctive identifiers (those no licensed donor's
    same-named file has) mostly come from an unlicensed donor's file was copied."""
    new_headers = {p for p in changed if p.endswith((".h", ".hpp")) and base and not file_at(base, p)}
    reviewed = Path("config/G4SE01/provenance_reviewed.txt")
    cleared = {l.split()[0] for l in reviewed.read_text().splitlines() if l.strip() and not l.startswith("#")} if reviewed.exists() else set()
    for path in changed:
        if not SOURCE.search(path) or not Path(path).exists() or path in cleared:
            continue
        text = Path(path).read_text(errors="replace")
        ours = set(IDENT.findall(strip_comments(text)))
        licensed = included_identifiers(path, text, skip=new_headers)
        for t in donor_texts(path, suffix=True):
            licensed |= set(IDENT.findall(t))
        distinctive = ours - licensed
        if len(distinctive) < 8:
            continue
        for repo, text in unlicensed_texts(path):
            shared = distinctive & set(IDENT.findall(strip_comments(text)))
            if len(shared) >= 8 and len(shared) >= 0.5 * len(distinctive):
                problems.append(f"{path}: {len(shared)}/{len(distinctive)} distinctive identifiers match "
                                f"unlicensed {repo}'s {Path(path).name}; write it from the asm and licensed donors")


def check_sources(base, changed, problems):
    for path in changed:
        if not SOURCE.search(path) or not Path(path).exists():
            continue
        old = file_at(base, path)
        donors = None
        for line in added_lines(base, path):
            code = line.split("//")[0]
            for pattern, what in BANNED:
                if pattern.search(code):
                    problems.append(f"{path}: {what}: {line.strip()}")
            for pattern, what in RESTRICTED:
                m = pattern.search(code)
                if not m:
                    continue
                text = m.group(0).strip()
                if text in old:
                    continue
                if donors is None:
                    donors = donor_texts(path)
                if any(text in d for d in donors):
                    continue
                problems.append(f"{path}: new {what} not in the file before or its donor: {line.strip()}")


def check_protected(base, changed, problems):
    for path in changed:
        if any(path == p or path.startswith(p) for p in PROTECTED if p.endswith("/")) or path in PROTECTED:
            problems.append(f"{path}: protected file changed")
    if "configure.py" in changed:
        diff = git("diff", "-U0", f"{base}...HEAD", "--", "configure.py") + git("diff", "-U0", "HEAD", "--", "configure.py")
        for line in diff.splitlines():
            if line[:1] not in "+-" or line.startswith(("+++", "---")):
                continue
            if PROTECTED_CONFIGURE.search(line):
                problems.append(f"configure.py: report or tool setting changed: {line}")
            elif "Object(" not in line and LINK_CONFIGURE.search(line):
                problems.append(f"configure.py: link or SHA-1 setting changed: {line}")


def parse_symbols(text):
    """Key by (section, address, n): several symbols (labels, functions) can share an address."""
    out, seen = {}, {}
    for line in text.splitlines():
        m = SYMBOL_LINE.match(line)
        if m:
            attrs = dict(a.split(":", 1) for a in m.group(4).split() if ":" in a)
            place = (m.group(2), int(m.group(3), 16))
            n = seen.get(place, 0)
            seen[place] = n + 1
            out[(*place, n)] = (m.group(1), attrs.get("size"), attrs.get("type"))
    return out


def check_symbols(base, changed, problems):
    path = "config/G4SE01/symbols.txt"
    if path not in changed:
        return
    approved = set()
    if APPROVALS.exists():
        for line in APPROVALS.read_text().splitlines():
            parts = line.split()
            if len(parts) >= 3 and not line.startswith("#"):
                approved.add((parts[0], parts[1], parts[2]))
    old = parse_symbols(file_at(base, path))
    new = parse_symbols(Path(path).read_text())
    for key, (name, size, kind) in old.items():
        if PLACEHOLDER.search(name):
            # dtk's guessed symbols may be split into the real ones or resized.
            continue
        if key not in new:
            problems.append(f"symbols.txt: symbol at {key[0]}:0x{key[1]:08X} ({name}) removed")
            continue
        new_name, new_size, new_kind = new[key]
        if (name, size, new_size) in approved and kind == new_kind:
            continue
        if (name, f"{size}/{kind}", f"{new_size}/{new_kind}") in approved:  # size and type, e.g. a label that is really a function
            continue
        if (size, kind) != (new_size, new_kind):
            problems.append(f"symbols.txt: {name} size/type changed ({size},{kind} -> {new_size},{new_kind})")
        if new_name != name and not PLACEHOLDER.search(name) and (name, name, new_name) not in approved:
            problems.append(f"symbols.txt: real name {name} renamed to {new_name}")


FAKEMATCH = re.compile(r"FakeMatch|force_order|FAKEMATCH|fakematch|NO_INLINE\(\)|CW_FORCE_(STRINGS|BSS)|fake_function", re.I)


def check_fakematch(problems):
    """A Matching unit may not rely on (or admit to) a fakematch, even one imported from a donor."""
    roots = ("libs/sdk/", "libs/sdk_gx/", "libs/jsystem/", "src/")
    for unit in json.loads(Path("objdiff.json").read_text())["units"]:
        meta = unit.get("metadata") or {}
        src = meta.get("source_path") or ""
        if meta.get("complete") and src.startswith(roots) and Path(src).exists():
            for n, line in enumerate(Path(src).read_text(errors="replace").splitlines(), 1):
                if FAKEMATCH.search(line):
                    problems.append(f"{src}:{n}: Matching unit with a fakematch: {line.strip()}")
                    break


CONFIG_YML = Path("config/G4SE01/config.yml")
BLOBS = Path("config/G4SE01/blobs.txt")
BLOB_SECTIONS = {".data", ".rodata", ".sdata", ".sdata2"}  # initialized data only, never code
BLOB_MAX = 0x2000  # a larger blob needs this cap raised, on the record
EXTRACT_KEYS = {"symbol", "header", "header_type"}
# The one include root under build/ configure.py may name (dtk-template's: PCH and extracted blobs).
BUILD_ROOT = "build/{config.version}/include"
CONFIGURE_ROOT = re.compile(r"-(?:i|I|ir|I-)\s*([^\s\"',\]]*build/[^\s\"',\]]*)", re.I)


def read_blobs(problems):
    """config/G4SE01/blobs.txt: `blob` lines (approved extracts) and `array`
    lines (files allowed a large integer-literal initializer)."""
    blobs, arrays = [], {}
    if not BLOBS.is_file():
        return blobs, arrays
    for n, line in enumerate(BLOBS.read_text().splitlines(), 1):
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        f = line.split()
        if f[0] == "blob" and len(f) >= 9:
            try:
                addr, size = int(f[3], 16), int(f[4], 16)
            except ValueError:
                problems.append(f"{BLOBS}:{n}: address/size not hex: {line}")
                continue
            blobs.append({"symbol": f[1], "section": f[2], "address": addr, "size": size,
                          "unit": f[5], "header": f[6], "review": f[7], "line": n})
        elif f[0] == "array" and len(f) >= 4:
            arrays[f[1]] = f[2]
        else:
            problems.append(f"{BLOBS}:{n}: not `blob symbol section address size unit header review why...` "
                            f"or `array path review why...`: {line}")
    return blobs, arrays


def _yaml_extracts(problems):
    """config.yml's extract: list via yaml.safe_load; anything unexpected fails."""
    try:
        import yaml
    except ImportError:
        problems.append("check_blobs needs PyYAML (pip install pyyaml) to read config.yml")
        return []
    try:
        cfg = yaml.safe_load(CONFIG_YML.read_text())
    except yaml.YAMLError as err:
        problems.append(f"{CONFIG_YML}: not valid YAML: {err}")
        return []
    if not isinstance(cfg, dict):
        problems.append(f"{CONFIG_YML}: not a mapping")
        return []

    def nested(node, path):
        if isinstance(node, dict):
            for k, v in node.items():
                if k == "extract" and path:
                    problems.append(f"{CONFIG_YML}: extract: under {'/'.join(path)} (only the top-level list is allowed)")
                nested(v, path + [str(k)])
        elif isinstance(node, list):
            for i, v in enumerate(node):
                nested(v, path + [str(i)])
    nested(cfg, [])
    extracts = cfg.get("extract")
    if extracts is None:
        return []
    if not isinstance(extracts, list):
        problems.append(f"{CONFIG_YML}: extract: is not a list")
        return []
    out = []
    for e in extracts:
        if not (isinstance(e, dict) and set(e) <= EXTRACT_KEYS and isinstance(e.get("symbol"), str)
                and isinstance(e.get("header"), str) and e.get("header_type") == "raw"):
            problems.append(f"{CONFIG_YML}: extract entry must be exactly symbol, header, header_type: raw: {e!r}")
            continue
        h = e["header"]
        if not h.startswith("assets/") or ".." in Path(h).parts or "\\" in h:
            problems.append(f"{CONFIG_YML}: extracted header {h} must be under assets/")
            continue
        out.append((e["symbol"], h))
    return out


def _split_ranges(unit):
    """{section: (start, end)} of the splits.txt stanza for this source path."""
    stanzas, cur = {}, None
    for line in Path("config/G4SE01/splits.txt").read_text().splitlines():
        if line and not line[0].isspace() and line.rstrip().endswith(":"):
            cur = stanzas.setdefault(line.rstrip()[:-1], {})
        elif cur is not None and line.strip():
            m = re.match(r"^\s+(\S+)\s+start:(0x[0-9A-Fa-f]+)\s+end:(0x[0-9A-Fa-f]+)", line)
            if m:
                cur[m[1]] = (int(m[2], 16), int(m[3], 16))
    hits = [s for s in stanzas if unit == s or unit.endswith("/" + s)]
    return stanzas[hits[0]] if len(hits) == 1 else None


def check_blobs(problems):
    """Binary blobs come only from config.yml's extract: list, each one approved
    in blobs.txt, extracted from orig/ at build time; the source holds an
    #include, never the bytes."""
    extracts = _yaml_extracts(problems)
    blobs, _ = read_blobs(problems)
    listed = {(b["symbol"], b["header"]): b for b in blobs}
    if len(listed) != len(blobs):
        problems.append(f"{BLOBS}: a blob is listed twice")
    for key in set(extracts) ^ set(listed):
        where = "config.yml's extract: list but not blobs.txt" if key in extracts else "blobs.txt but not config.yml's extract: list"
        problems.append(f"blob {key[0]} ({key[1]}): in {where}")
    if len(set(extracts)) != len(extracts) or len({h for _, h in extracts}) != len(extracts):
        problems.append(f"{CONFIG_YML}: a symbol or header is extracted twice")
    symbols = {}
    for line in Path("config/G4SE01/symbols.txt").read_text().splitlines():
        m = SYMBOL_LINE.match(line)
        if m:
            attrs = dict(a.split(":", 1) for a in m.group(4).split() if ":" in a)
            symbols.setdefault(m.group(1), []).append((m.group(2), int(m.group(3), 16), attrs))
    for b in blobs:
        tag = f"{BLOBS}:{b['line']} {b['symbol']}"
        sym = symbols.get(b["symbol"], [])
        if len(sym) != 1:
            problems.append(f"{tag}: not exactly one such symbol in symbols.txt")
            continue
        sec, addr, attrs = sym[0]
        size = int(attrs.get("size", "0"), 16)
        if b["section"] not in BLOB_SECTIONS or sec != b["section"]:
            problems.append(f"{tag}: section {sec} (listed {b['section']}); blobs are data only ({', '.join(sorted(BLOB_SECTIONS))})")
        if attrs.get("type") != "object":
            problems.append(f"{tag}: symbols.txt type {attrs.get('type')}, not object")
        if (addr, size) != (b["address"], b["size"]) or not size:
            problems.append(f"{tag}: symbols.txt has 0x{addr:08X} size 0x{size:X}, blobs.txt 0x{b['address']:08X} size 0x{b['size']:X}")
        if size > BLOB_MAX:
            problems.append(f"{tag}: 0x{size:X} bytes is over the 0x{BLOB_MAX:X} cap")
        if not norm_path(b["header"]).startswith("assets/"):
            problems.append(f"{tag}: header {b['header']} must be under assets/")
        rng = _split_ranges(b["unit"]) if Path(b["unit"]).is_file() else None
        r = (rng or {}).get(sec)
        if not r or not (r[0] <= addr and addr + size <= r[1]):
            problems.append(f"{tag}: not inside {b['unit']}'s {sec} range in splits.txt (or no such source/unit)")
        if not Path(b["review"]).name.endswith(".md"):
            problems.append(f"{tag}: review {b['review']} is not a review file")
    # Relocations: microcode and asset bytes hold no pointers; data that does has a C form.
    for b in blobs:
        if re.search(r"__vt__|__RTTI__|\$\$|@", b["symbol"]) or b["symbol"].startswith("__"):
            problems.append(f"{BLOBS}:{b['line']} {b['symbol']}: vtable, RTTI or compiler symbol, never a blob")
        why = _blob_relocations(b)
        if why:
            problems.append(f"{BLOBS}:{b['line']} {b['symbol']}: {why}")
    # Includes: an allowlisted header only from its unit, exactly once and
    # unconditionally; nothing else from build/'s include root, no computed
    # includes. MWCC (under wibo) splices line continuations, accepts "\" and
    # resolves names case-insensitively, so every path is normalized first.
    allowed = {norm_path(b["header"]): b for b in blobs}
    names = {Path(h).name for h in allowed} | {norm_path(Path(b["unit"]).name) for b in blobs}
    seen = {}
    for entry in git("ls-files", "-s", "-z").split("\0"):
        if "\t" not in entry:
            continue
        info, path = entry.split("\t", 1)
        mode = info.split()[0]
        low = norm_path(path)
        if mode == "120000":
            problems.append(f"{path}: tracked symlink (never; it can point into build/ or orig/)")
        if "assets" in low.split("/")[:-1]:
            problems.append(f"{path}: tracked under an assets/ directory (any case; blobs are extracted at build time, never committed)")
        elif any(low.endswith("/" + h) or low == h for h in allowed) or Path(low).name in {Path(h).name for h in allowed}:
            problems.append(f"{path}: tracked file with the name of an extracted blob")
        if not scanned(path):
            continue
        text = source_text(path)
        if text is None:
            continue
        regions = [(a, z) for a, z, _ in initializer_spans(text)]
        for pos, depth, kind, inc in include_directives(text):
            if kind == "computed":
                problems.append(f"{path}: computed #include {inc} (write the path)")
                continue
            n = norm_path(inc)
            inside = any(a <= pos < z for a, z in regions)
            if "build/" in n or n.startswith("build"):
                problems.append(f"{path}: includes {inc} from build/ by path")
            elif n in allowed:
                b = allowed[n]
                if norm_path(path) != norm_path(b["unit"]):
                    problems.append(f"{path}: includes blob {inc}, which blobs.txt allows only in {b['unit']}")
                elif depth:
                    problems.append(f"{path}: includes blob {inc} under #if/#ifdef (must be unconditional)")
                else:
                    seen[n] = seen.get(n, 0) + 1
            elif "assets" in n.split("/") or Path(n).name in names:
                problems.append(f"{path}: includes {inc}, which no blobs.txt / config.yml extract entry produces")
            elif inside:
                problems.append(f"{path}: includes {inc} inside an initializer; only blobs.txt headers may be")
    for h, b in allowed.items():
        if seen.get(h, 0) != 1:
            problems.append(f"{b['unit']}: includes blob {b['header']} {seen.get(h, 0)} times unconditionally (exactly once)")
    # Include roots: configure.py may name build/ only as build/{config.version}/include.
    for root in CONFIGURE_ROOT.findall(Path("configure.py").read_text().replace("\\\\", "/")):
        if root.rstrip("/") != BUILD_ROOT:
            problems.append(f"configure.py: include root {root} under build/ (only {BUILD_ROOT})")


def _blob_relocations(b):
    """Why a blob is pointer data (relocations in dtk's split of the original), or None."""
    units = json.loads(Path("objdiff.json").read_text())["units"] if Path("objdiff.json").is_file() else []
    target = next((u.get("target_path") for u in units
                   if (u.get("metadata") or {}).get("source_path") == b["unit"]), None)
    if not target:
        return f"no objdiff unit for {b['unit']}"
    if not Path(target).is_file():
        ninja("build/G4SE01/config.json")
    try:
        elf = Elf(target)
    except (OSError, ValueError, StopIteration) as err:
        return f"can't read the original's split object {target}: {err}"
    sym = [s for s in elf.syms if s[0] == b["symbol"] and s[5] < len(elf.names)]
    if len(sym) != 1 or elf.names[sym[0][5]] != b["section"] or sym[0][2] != b["size"]:
        return f"not one {b['section']} symbol of size 0x{b['size']:X} in {target}"
    _, val, size, _, _, sec, _, _ = sym[0]
    hits = [off for off, _, _ in elf.relocs(sec) if val <= off < val + size]
    if hits:
        return f"the original has {len(hits)} relocation(s) in it (pointer data has a C form)"
    return None


# Files whose text may reach the compiler: anything under the source trees and
# any source-like extension elsewhere, in any case.
SOURCE_TREES = ("libs/", "src/", "include/")
NOT_SOURCE = re.compile(r"\.(md|txt|json|ya?ml|py|sh|toml|csv|tsv|html?|css|js|svg|png|jpe?g|gif|ico|pdf|ninja|log)$", re.I)


def scanned(path):
    return (path.lower().startswith(SOURCE_TREES) or SOURCE.search(path)) and not NOT_SOURCE.search(path)


def norm_path(p):
    """An include or file path as MWCC under wibo resolves it: / for \\, no ./, any case."""
    p = re.sub(r"/+", "/", p.replace("\\", "/"))
    parts = [x for x in p.split("/") if x not in ("", ".")]
    return "/".join(parts).casefold()


def source_text(path):
    """Text with line continuations spliced and comments blanked (newlines kept)."""
    try:
        raw = Path(path).read_bytes()
    except OSError:
        return None
    if b"\0" in raw[:4096]:
        return None
    text = raw.decode("latin-1").replace("\r\n", "\n").replace("\r", "\n")
    text = re.sub(r"\\[ \t]*\n", "", text)
    text = re.sub(r"\?\?/[ \t]*\n", "", text)
    return blank_comments(text)


# Strings, char literals and comments in one pass, so "a//b" isn't a comment.
LEXEME = re.compile(r'"(?:[^"\\\n]|\\.)*"|\'(?:[^\'\\\n]|\\.)*\'|//[^\n]*|/\*.*?\*/', re.S)


def blank_comments(text):
    """Comments replaced by spaces (newlines kept); strings untouched."""
    return LEXEME.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)) if m.group(0)[0] == "/" else m.group(0), text)


# MWCC treats \f and \v as whitespace before and after the directive mark.
DIRECTIVE = re.compile(r"^[ \t\f\v]*(?:#|%:|\?\?=)[ \t\f\v]*(\w+)(.*)$", re.M)


def include_directives(text):
    """(offset, #if depth, "quoted"|"computed", argument) of each #include."""
    out, depth = [], 0
    for m in DIRECTIVE.finditer(text):
        word, rest = m.group(1), m.group(2).strip()
        if word in ("if", "ifdef", "ifndef"):
            depth += 1
        elif word == "endif":
            depth = max(0, depth - 1)
        elif word in ("include", "include_next", "import"):
            q = re.match(r'^"([^"]*)"|^<([^>]*)>', rest)
            out.append((m.start(), depth, "quoted", q.group(1) if q.group(1) is not None else q.group(2))
                       if q else (m.start(), depth, "computed", rest))
    return out


def initializer_spans(text):
    """(start, end, body) of each brace initializer `= { ... }` (offsets in text)."""
    out = []
    for m in re.finditer(r"=\s*\{", text):
        depth, i = 0, m.end() - 1
        while i < len(text):
            depth += {"{": 1, "}": -1}.get(text[i], 0)
            if depth == 0:
                break
            i += 1
        out.append((m.start(), i + 1, text[m.end():i]))
    return out


LITERAL = re.compile(r"(?<![\w.])(0[xX][0-9A-Fa-f]+|\d+)[uUlL]*(?![\w.])|'(?:\\.|[^'\\\n])+'")
STRING = re.compile(r'"(?:[^"\\\n]|\\.)*"')
ESCAPE = re.compile(r"\\(x[0-9A-Fa-f]+|[0-7]{1,3})")
PER_INIT, PER_INIT_BYTES, PER_FILE, PER_FILE_BYTES, ESCAPES = 64, 128, 256, 512, 128


def _weight(lit):
    """Bytes a literal stands for: hex by its digits, decimal by its magnitude."""
    if lit.startswith("'"):
        return 1
    if lit[:2].lower() == "0x":
        return max(1, (len(lit) - 2 + 1) // 2)
    v = int(lit)
    return 1 if v < 0x100 else 2 if v < 0x10000 else 4


def literal_counts(body):
    """(integer and char literals, bytes they stand for) in an initializer or list;
    casts and `+0` don't hide them, every number counts."""
    lits = [m.group(1) or m.group(0) for m in LITERAL.finditer(STRING.sub(" ", body))]
    return len(lits), sum(_weight(l) for l in lits)


def _array_stats(text, added=None):
    """Literal statistics of a text: (problems inside edited initializers,
    literals, bytes' worth, longest bare literal run, escaped bytes)."""
    spans = initializer_spans(text)
    lines = text.split("\n")
    found, total = [], [0, 0]
    for start, end, body in spans:
        n, w = literal_counts(body)
        total[0] += n
        total[1] += w
        if added is not None and (n > PER_INIT or w > PER_INIT_BYTES):
            first, last = text.count("\n", 0, start), text.count("\n", 0, end)
            if any(l.strip() in added for l in lines[first:last + 1]):
                found.append(f"{first + 1}: initializer of {n} integer literals ({w} bytes' worth)")
    rest, last = [], 0
    for start, end, _ in spans:
        rest.append(text[last:start])
        last = end
    rest = DIRECTIVE.sub(" ", "".join(rest) + text[last:])
    run, best = 0, 0
    for e in (e.strip("{}(); \t") for e in re.split(r"[,\n]", rest)):
        if e:
            run = run + 1 if LITERAL.fullmatch(e) else 0
            best = max(best, run)
    escapes = sum(len(ESCAPE.findall(s)) for s in STRING.findall(text))
    return found, total[0], total[1], best, escapes


def check_byte_arrays(base, changed, problems):
    """Tripwire for hand-typed bytes in changed files of any source-like kind
    (headers and .inc included); review is the real enforcement. Fails, unless
    blobs.txt has an `array` line for the file:
      * an edited initializer of more than 64 integer literals or 128 bytes' worth;
      * a file that gains 256 or more literals (or 512 bytes' worth) across its
        initializers, and bare literal lists over 64 elements (an .inc or a
        header that is a list of numbers) that grew;
      * a file that gains more than 128 escaped bytes in string literals
        (adjacent strings count together).
    Every number counts, so casts, `+0` and wide words don't hide bytes."""
    _, arrays = read_blobs(problems)
    arrays = {norm_path(p) for p in arrays}
    for path in changed:
        if not scanned(path) or not Path(path).is_file() or norm_path(path) in arrays:
            continue
        text = source_text(path)
        if text is None:
            continue
        added = {l.strip() for l in added_lines(base, path) if re.search(r"\d", l)}
        found, n, w, run, esc = _array_stats(text, added)
        old = file_at(base, path)
        _, n0, w0, run0, esc0 = _array_stats(blank_comments(re.sub(r"\\[ \t]*\n", "", old.replace("\r\n", "\n")))) if old else ([], 0, 0, 0, 0)
        tip = "hand-typed bytes? extract a blob (blobs.txt), write the data's source form, or ask for an array line"
        problems += [f"{path}:{f}; {tip}" for f in found]
        if n - n0 >= PER_FILE or w - w0 >= PER_FILE_BYTES:
            problems.append(f"{path}: gains {n - n0} integer literals ({w - w0} bytes' worth) in initializers; {tip}")
        if run > PER_INIT and run > run0:
            problems.append(f"{path}: a bare list of {run} integer literals; {tip}")
        if esc - esc0 > ESCAPES:
            problems.append(f"{path}: gains {esc - esc0} escaped bytes in string literals; {tip}")


def ninja(*targets):
    return subprocess.run(["timeout", "1800", "ninja", *targets], capture_output=True, text=True)


def check_configure(problems):
    """tools/project.py names Matching objects it can't compile (no source) or
    that no split covers; configure.py then links the original in their place."""
    out = subprocess.run([sys.executable, "configure.py"], capture_output=True, text=True)
    if out.returncode != 0:
        problems.append("configure.py failed:\n" + (out.stdout + out.stderr)[-1500:])
    for line in out.stdout.splitlines():
        if line.startswith(("Matching object without source", "Matching object not in the split")):
            problems.append(f"{line.split(': ', 1)[-1]}: {line.split(':', 1)[0]}")


def check_report(problems):
    report = Path("build/G4SE01/report_honest.json")
    out = ninja(str(report))
    if out.returncode != 0:
        problems.append("ninja build/G4SE01/report_honest.json failed:\n" + (out.stdout + out.stderr)[-3000:])
        return
    units = {u["name"]: u for u in json.loads(report.read_text())["units"]}
    for unit in json.loads(Path("objdiff.json").read_text())["units"]:
        meta = unit.get("metadata") or {}
        if unit.get("symbol_mappings"):
            problems.append(f"{meta.get('source_path', unit['name'])}: objdiff.json has symbol_mappings "
                            f"(the report honors them); remove them")
        if not meta.get("complete"):
            continue
        name = meta.get("source_path", unit["name"])
        base = unit.get("base_path")
        if not base or not Path(base).is_file():
            problems.append(f"{name}: marked Matching but has no compiled object ({base or 'no base_path'})")
            continue
        if unit["name"] not in units:
            problems.append(f"{name}: marked Matching but missing from the honest report")
            continue
        m = units[unit["name"]].get("measures", {})
        code = (int(m.get("matched_code", 0)), int(m.get("total_code", 0)))
        data = (int(m.get("matched_data", 0)), int(m.get("total_data", 0)))
        if not code[1] and not data[1]:
            problems.append(f"{name}: marked Matching but has no code or data in the honest report")
        elif code[0] != code[1] or data[0] != data[1]:
            problems.append(
                f"{meta.get('source_path', unit['name'])}: marked Matching but code {code[0]}/{code[1]}, data {data[0]}/{data[1]}")
    check_weakdedup(problems)


class Elf:
    """Sections, symbols and relocations of a big-endian ELF32 object."""

    def __init__(self, path):
        d = self.d = Path(path).read_bytes()
        if d[:4] != b"\x7fELF" or d[4] != 1 or d[5] != 2:
            raise ValueError(f"{path}: not a big-endian ELF32 object")
        self.ehdr = d[:0x34]
        shoff, = struct.unpack_from(">I", d, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from(">HHH", d, 0x2E)
        self.sh = [struct.unpack_from(">IIIIIIIIII", d, shoff + i * shentsize) for i in range(shnum)]
        self.names = [self.cstr(self.sh[shstrndx][4] + h[0]) for h in self.sh]
        self.symtab = next(i for i, h in enumerate(self.sh) if h[1] == 2)
        strtab = self.sh[self.sh[self.symtab][6]][4]
        self.syms = []
        for k in range(self.sh[self.symtab][5] // 16):
            nm, val, size, info, other, shndx = struct.unpack_from(">IIIBBH", d, self.sh[self.symtab][4] + k * 16)
            self.syms.append((self.cstr(strtab + nm), val, size, info >> 4, info & 15, shndx, nm, other))

    def cstr(self, o):
        return self.d[o:self.d.index(0, o)].decode("latin-1")

    def data(self, i):
        h = self.sh[i]
        return self.d[h[4]:h[4] + h[5]] if h[1] != 8 else b""  # NOBITS: no bytes

    def rela(self, i):
        c = self.data(i)
        return [struct.unpack_from(">IIi", c, k * 12) for k in range(len(c) // 12)]

    def relocs(self, sec):
        out = []
        for i, h in enumerate(self.sh):
            if h[1] == 4 and h[7] == sec:
                out += [(off, info >> 8, add) for off, info, add in self.rela(i)]
        return out


def _sha1(path):
    return hashlib.sha1(Path(path).read_bytes()).hexdigest()


def _map_lines(path, cache={}):
    if path not in cache:
        cache[path] = Path(path).read_text(encoding="latin-1").splitlines()
    return cache[path]


def _map_section(lines, n):
    """The `<section> section layout` a (1-based) map line belongs to."""
    for i in range(n - 1, -1, -1):
        m = re.match(r"^(\S+) section layout\s*$", lines[i])
        if m:
            return m[1]
    return None


def _main_link_inputs():
    text = Path("build.ninja").read_text().replace("$\n", "")
    m = re.search(r"^build build/G4SE01/fsa_game_code\.elf(?: \|[^:]*)?:\s+link\s+(.*?)(?:\||$)", text, re.M)
    return m.group(1).split() if m else []


def _tracked(path):
    return bool(git("ls-files", "--", str(path)).strip())


def check_link_record(key, rec, expected):
    """(problem or None, inputs, objects) for one link the manifest relies on.
    A trial link is linked again (tools/link_check.py --verify-unit)."""
    if not Path(rec.get("map", "")).is_file() or _sha1(rec["map"]) != rec.get("map_sha1"):
        return f"map {rec.get('map')} missing or changed since the report", [], None
    if key == "main":
        mp, elf, dol = (Path("build/G4SE01/fsa_game_code" + x) for x in (".elf.MAP", ".elf", ".dol"))
        inputs = _main_link_inputs()
        if rec["map"] != str(mp) or rec.get("dol") != str(dol):
            return "the main link record doesn't name ninja's map and DOL", inputs, None
        if not elf.is_file() or not dol.is_file():
            return f"{elf} or {dol} missing", inputs, None
        if not (_sha1(dol) == rec.get("dol_sha1") == expected):
            return f"{dol} changed or doesn't match build.sha1", inputs, None
        if elf.stat().st_mtime > mp.stat().st_mtime + 1 or dol.stat().st_mtime + 1 < mp.stat().st_mtime:
            return f"{mp} is not from the link that wrote {dol}", inputs, None
    else:
        ev = Path(key)
        if ev.parent != Path("build/G4SE01/weakdedup_evidence") or not ev.is_file():
            return f"trial evidence {key} missing or outside build/G4SE01/weakdedup_evidence", [], None
        if _tracked(ev) or _tracked(rec["map"]):
            return f"trial evidence {key} is tracked by git", [], None
        try:
            r = json.loads(ev.read_text())
            if not (isinstance(r, dict) and isinstance(r.get("unit"), str)
                    and isinstance(r.get("objects"), dict) and isinstance(r.get("inputs"), list)):
                raise ValueError("not an evidence record (unit, objects, inputs)")
        except (OSError, ValueError) as err:
            return f"trial evidence {key} is malformed: {err}", [], None
        slug = r.get("unit", "").split("/", 1)[-1].replace("/", "__")
        inputs, objects = r.get("inputs", []), r.get("objects", {})
        if ev.name != f"{slug}.json" or not (r.get("map") == rec["map"] == str(ev.with_suffix(".MAP"))):
            return f"trial evidence {key} doesn't name its own unit's map", inputs, objects
        if r.get("map_sha1") != rec["map_sha1"] or r.get("dol_sha1") != expected or len(objects) != 1:
            return f"trial evidence {key} doesn't match the manifest or build.sha1", inputs, objects
        out = subprocess.run([sys.executable, "tools/link_check.py", "--verify-unit", r["unit"]],
                             capture_output=True, text=True)
        try:
            v = json.loads(out.stdout.strip().splitlines()[-1])
        except (ValueError, IndexError):
            return f"trial link of {r['unit']} couldn't be re-run: {(out.stdout + out.stderr)[-300:]}", inputs, objects
        if v["dol_sha1"] != expected:
            return f"re-running the trial link of {r['unit']} gives DOL SHA-1 {v['dol_sha1']}, not build.sha1", inputs, objects
        if v["map_sha1"] != rec["map_sha1"]:
            return f"re-running the trial link of {r['unit']} gives another map", inputs, objects
        if v["object_sha1"] != next(iter(objects.values())):
            return f"{r['unit']}'s object changed since the trial link", inputs, objects
        if v["inputs_sha1"] != hashlib.sha1("\n".join(inputs).encode()).hexdigest():
            return f"the trial link of {r['unit']} now has other inputs", inputs, objects
    if hashlib.sha1("\n".join(inputs).encode()).hexdigest() != rec.get("inputs_sha1"):
        return "link inputs changed since the report", inputs, None
    return None, inputs, None


def expected_stripped(a, removed, undef=frozenset()):
    """Compare an independently rebuilt expectation of the stripped object with b:
    `removed` = {section index: [(offset, size)]} cut from the compiled object a;
    removed symbols become absolute 0, or undefined if their index is in undef
    (weak data kept in another object). Returns a function b -> list of differences."""
    def shift(sec, x):
        return sum(sz for o, sz in removed.get(sec, []) if o + sz <= x)

    def cut(sec, x):
        return any(o <= x < o + sz for o, sz in removed.get(sec, []))

    def differences(b):
        diffs = []
        if a.ehdr != b.ehdr or a.names != b.names or len(a.sh) != len(b.sh):
            return ["ELF header or section list differs"]
        rela_of = {i: h[7] for i, h in enumerate(a.sh) if h[1] == 4}
        for i, n in enumerate(a.names):
            ha, hb = a.sh[i], b.sh[i]
            if i == a.symtab:
                want = []
                for k, (name, val, size, bind, typ, shndx, nm, other) in enumerate(a.syms):
                    if shndx in removed and size and cut(shndx, val):
                        want.append((name, 0, 0, bind, typ, 0 if k in undef else 0xFFF1, nm, other))
                    elif shndx in removed:
                        want.append((name, val - shift(shndx, val), size, bind, typ, shndx, nm, other))
                    else:
                        want.append((name, val, size, bind, typ, shndx, nm, other))
                got = b.syms
            elif i in removed:
                c = a.data(i)
                want = bytes(x for j, x in enumerate(c) if not cut(i, j))
                got = b.data(i)
            elif rela_of.get(i) in removed:
                sec = rela_of[i]
                want = [(off - shift(sec, off), info, add) for off, info, add in a.rela(i) if not cut(sec, off)]
                got = b.rela(i)
            else:
                want, got = (ha, a.data(i)), (hb, b.data(i))
            if want != got:
                diffs.append(f"section {n or i} isn't the compiled one less exactly the listed entries")
            if i in removed or rela_of.get(i) in removed or i == a.symtab:
                if ha[:5] + ha[6:] != hb[:5] + hb[6:]:
                    diffs.append(f"section header {n or i} differs beyond its size")
        return diffs
    return differences

SHT_PROGBITS, SHF_ALLOC, SHF_EXECINSTR, STT_OBJECT, STT_FUNC, STT_SECTION = 1, 2, 4, 1, 2, 3
_UNUSED = re.compile(r"^\s+UNUSED\s+([0-9a-f]+) \.+ (\S+) (\S+)\s*$")
_PLACED = re.compile(r"^\s+([0-9a-f]{8}) ([0-9a-f]{6}) ([0-9a-f]{8})\s+(\d+) (\S+)\s+(\S+)\s*$")


def _map_objects(path, cache={}):
    """{(section, object): {"placed": [(offset, size, label)], "unused": {label: [line numbers]}}}
    from a map's section layouts, read independently of tools/honest_report.py."""
    if path not in cache:
        out, section = {}, None
        for n, line in enumerate(_map_lines(path), 1):
            m = re.match(r"^(\S+) section layout\s*$", line)
            if m:
                section = m[1]
                continue
            if section is None:
                continue
            m = _UNUSED.match(line)
            if m:
                out.setdefault((section, m[3]), {"placed": [], "unused": {}})["unused"].setdefault(m[2], []).append(n)
                continue
            m = _PLACED.match(line)
            if m:
                out.setdefault((section, m[6]), {"placed": [], "unused": {}})["placed"].append(
                    (int(m[1], 16), int(m[2], 16), m[5]))
                if m[5] == section and m[4] == "1":
                    out.setdefault(("starts", section), []).append((int(m[1], 16), m[6]))
        cache[path] = out
    return cache[path]


def _data_section(elf, i):
    if not 0 <= i < len(elf.sh):
        return False
    h = elf.sh[i]
    return (h[1] == SHT_PROGBITS and h[2] & SHF_ALLOC and not h[2] & SHF_EXECINSTR
            and elf.names[i] not in ("extab", "extabindex"))


def _dead_in_map(elf, mp, name, sec, sym, evidence):
    """True if `evidence` is the map line listing sym UNUSED for this object in
    section sec's layout, with sym's size, and the map doesn't also place it."""
    lines = _map_lines(mp)
    if sum(x[0] == sym[0] for x in elf.syms) != 1 or not isinstance(evidence, dict):
        return False
    n = evidence.get("line")
    if not isinstance(n, int) or not 0 < n <= len(lines) or lines[n - 1] != evidence.get("text"):
        return False
    m = _UNUSED.match(lines[n - 1])
    block = _map_objects(mp).get((elf.names[sec], name), {"placed": [], "unused": {}})
    return bool(m and (int(m[1], 16), m[2], m[3]) == (sym[2], sym[0], name)
                and _map_section(lines, n) == elf.names[sec]
                and not any(p[2] == sym[0] for p in block["placed"]))


def check_dead_data(e, elf, name, mp):
    """One dead-stripped data object of the manifest, checked against the object
    and the map: (section index, offset, bytes removed) or why it isn't proven."""
    sec = elf.names.index(e.get("section")) if e.get("section") in elf.names else None
    if sec is None or not _data_section(elf, sec):
        return f"{e.get('section')} is not a data section of the object"
    sym = [x for x in elf.syms if x[0] == e.get("symbol")]
    if len(sym) != 1:
        return "not exactly one symbol of that name in the object"
    sym = sym[0]
    if (sym[5], sym[1], sym[2]) != (sec, e.get("offset"), e.get("size")) or not sym[2] or sym[4] in (STT_FUNC, STT_SECTION):
        return "the object's symbol isn't the listed data object"
    ev = e.get("evidence")
    if not (isinstance(ev, list) and len(ev) == 1 and _dead_in_map(elf, mp, name, sec, sym, ev[0])):
        return f"map evidence missing or doesn't match {mp}"
    # Every allocated location that reaches into it is in a symbol the map lists UNUSED.
    lo, hi = sym[1], sym[1] + sym[2]
    want = set()
    for r, h in enumerate(elf.sh):
        if not h[2] & SHF_ALLOC:
            continue
        for off, si, add in elf.relocs(r):
            t = elf.syms[si]
            if t[5] != sec or not (t is sym or lo <= t[1] + add < hi):
                continue
            src = [x for x in elf.syms if x[5] == r and x[2] and x[1] <= off < x[1] + x[2] and x[4] in (STT_OBJECT, STT_FUNC)]
            if len(src) != 1:
                return f"referenced from {elf.names[r]}+0x{off:x}, which no single function or object contains"
            want.add((elf.names[r], src[0][0]))
    refs = e.get("referencers") or []
    got = {(x.get("section"), x.get("symbol")) for x in refs if isinstance(x, dict)}
    if got != want:
        return f"the manifest's referencers {sorted(got)} aren't the object's {sorted(want)}"
    for x in refs:
        rs = [y for y in elf.syms if y[0] == x["symbol"] and y[5] < len(elf.names) and elf.names[y[5]] == x["section"]]
        if len(rs) != 1 or not _dead_in_map(elf, mp, name, rs[0][5], rs[0], x.get("evidence")):
            return f"{x['symbol']} references it, and the map doesn't list {x['symbol']} UNUSED for {name}"
    # What goes with it: up to the next symbol or the section end, zero padding only.
    nxt = min([x[1] for x in elf.syms if x[5] == sec and x[2] and x[1] >= hi] + [elf.sh[sec][5]])
    if e.get("removed") != nxt - lo or any(elf.data(sec)[hi:nxt]):
        return "the removed range isn't the object plus zero padding up to the next symbol"
    return (sec, lo, nxt - lo)


def _unused_line(elf, mp, name, sec, sym):
    """The map's single UNUSED line for sym in this object and section, as
    evidence _dead_in_map accepts (so its size is checked too), or None."""
    block = _map_objects(mp).get((elf.names[sec], name), {"placed": [], "unused": {}})
    ns = block["unused"].get(sym[0], [])
    if len(ns) != 1:
        return None
    ev = {"line": ns[0], "text": _map_lines(mp)[ns[0] - 1]}
    return ev if _dead_in_map(elf, mp, name, sec, sym, ev) else None


def _container(elf, r, off):
    src = [x for x in elf.syms if x[5] == r and x[2] and x[1] <= off < x[1] + x[2] and x[4] in (STT_OBJECT, STT_FUNC)]
    return src[0] if len(src) == 1 else None


def _stripped_layout(elf, sec, rem, mp, name):
    """Why the map's layout of this object's section isn't the stripped one:
    every kept sized symbol at the offset (from the object's section start)
    and with the size the map places it at, the removed ones not placed, and
    the next object's section 0 to 7 bytes (alignment) after the stripped end."""
    secname = elf.names[sec]
    objs = _map_objects(mp)
    block = objs.get((secname, name), {"placed": [], "unused": {}})["placed"]
    starts = [p for p in block if p[2] == secname]
    if len(starts) != 1 or block[0] != starts[0]:
        return f"the map doesn't place this object's {secname} exactly once"
    start = starts[0][0]
    for x in elf.syms:
        if x[5] != sec or not x[2] or x[4] == STT_SECTION:
            continue
        if any(o <= x[1] < o + n for o, n in rem):
            if any(p[2] == x[0] for p in block[1:]):
                return f"the map places {x[0]}, which is removed"
            continue
        new = x[1] - sum(n for o, n in rem if o + n <= x[1])
        got = [p for p in block[1:] if p[2] == x[0]]
        if len(got) != 1 or (got[0][0] - start, got[0][1]) != (new, x[2]):
            return f"the map doesn't place {x[0]} at 0x{new:x}, size 0x{x[2]:x}, of the stripped section"
    order = objs.get(("starts", secname), [])
    idx = [i for i, o in enumerate(order) if o[1] == name]
    if len(idx) != 1 or idx[0] + 1 >= len(order):
        return f"the map has no object after this one's {secname} to bound its end"
    gap = order[idx[0] + 1][0] - (start + elf.sh[sec][5] - sum(n for _, n in rem))
    if not 0 <= gap < 8:
        return f"the next object in the map starts {gap} bytes after the stripped end, not within alignment"
    return None


_TREE_DUP = re.compile(r"^\s*(\d+)\] >>> UNREFERENCED DUPLICATE (\S+)\s*$")
_TREE_DEF = re.compile(r"^\s*(\d+)\] (\S+) \((\w+),(\w+)\) found in (\S+)\s*$")
_TREE_FOUND = re.compile(r"^\s*(\d+)\] >>> \((\w+),(\w+)\) found in (\S+)\s*$")


def _weak_kept(path, cache={}):
    """{(object, name): {kept object of every link-tree node that lists name as
    an UNREFERENCED DUPLICATE (object,weak) found in object}}, read
    independently of tools/honest_report.py. A node whose line before the
    duplicate line isn't `<name> (object,...) found in <kept>` at the same
    depth counts as kept in None."""
    if path not in cache:
        out, lines = {}, _map_lines(path)
        for n, line in enumerate(lines):
            m = _TREE_DUP.match(line)
            if not m:
                continue
            d = _TREE_DEF.match(lines[n - 1]) if n else None
            kept = d[5] if d and (d[1], d[2], d[3]) == (m[1], m[2], "object") else None
            for nxt in lines[n + 1:]:
                f = _TREE_FOUND.match(nxt)
                if not f:
                    break
                if (f[1], f[2], f[3]) == (m[1], "object", "weak"):
                    out.setdefault((f[4], m[2]), set()).add(kept)
        cache[path] = out
    return cache[path]


def _named_relocs(elf, sym):
    """Relocations inside data symbol sym: sorted (offset in it, type, target
    name, addend), or None if one targets a local, section or unnamed symbol."""
    out = []
    for i, h in enumerate(elf.sh):
        if h[1] != 4 or h[7] != sym[5]:
            continue
        for off, info, add in elf.rela(i):
            if sym[1] <= off < sym[1] + sym[2]:
                t = elf.syms[info >> 8]
                if t[3] == 0 or t[4] == STT_SECTION or not t[0]:
                    return None
                out.append((off - sym[1], info & 0xFF, t[0], add))
    return sorted(out)


def check_weak_data(e, elf, name, mp, inputs, main):
    """One discarded weak data object of the manifest, checked against the
    object, the kept copy's object and the map: (section index, offset, bytes
    removed, symbol index) or why it isn't proven."""
    sec = elf.names.index(e.get("section")) if e.get("section") in elf.names else None
    if sec is None or not _data_section(elf, sec):
        return f"{e.get('section')} is not a data section of the object"
    secname = elf.names[sec]
    idx = [k for k, x in enumerate(elf.syms) if x[0] == e.get("symbol")]
    if len(idx) != 1:
        return "not exactly one symbol of that name in the object"
    k = idx[0]
    sym = elf.syms[k]
    if (sym[5], sym[1], sym[2]) != (sec, e.get("offset"), e.get("size")) or not sym[2] or (sym[3], sym[4]) != (2, STT_OBJECT):
        return "the object's symbol isn't the listed WEAK OBJECT"
    if sym[0] in _map_objects(mp).get((secname, name), {"unused": {}})["unused"]:
        return "the map lists it UNUSED for this object (dead data, not a weak duplicate)"
    # The map's link tree: kept copy, duplicate, found in this object.
    lines = _map_lines(mp)
    ev = e.get("evidence")
    if not (isinstance(ev, list) and len(ev) == 3 and all(isinstance(x, dict) and isinstance(x.get("line"), int)
                                                           and 0 < x["line"] <= len(lines)
                                                           and lines[x["line"] - 1] == x.get("text") for x in ev)):
        return f"map evidence missing or doesn't match {mp}"
    d, u, f = (lines[x["line"] - 1] for x in ev)
    md, mu, mf = _TREE_DEF.match(d), _TREE_DUP.match(u), _TREE_FOUND.match(f)
    if not (md and mu and mf and ev[1]["line"] == ev[0]["line"] + 1 and ev[2]["line"] > ev[1]["line"]
            and all(_TREE_FOUND.match(lines[i]) for i in range(ev[1]["line"], ev[2]["line"] - 1))
            and md[1] == mu[1] == mf[1] and md[2] == mu[2] == sym[0] and md[3] == "object"
            and (mf[2], mf[3], mf[4]) == ("object", "weak", name)):
        return f"the map lines aren't `{sym[0]} (object,...) found in <kept>`, its UNREFERENCED DUPLICATE and `(object,weak) found in {name}`"
    kept = md[5]
    if kept == name or _weak_kept(mp).get((name, sym[0])) != {kept}:
        return f"the map doesn't keep every listed copy in one other object ({sorted(map(str, _weak_kept(mp).get((name, sym[0]), [])))})"
    # The kept copy: a uniquely named input of the link, placed by the map, same bytes and relocations.
    paths = [i for i in inputs if Path(i).name == kept]
    if len(paths) != 1 or e.get("kept_object") != paths[0] or not Path(paths[0]).is_file():
        return f"{kept} isn't a uniquely named input of the link, or the manifest names another file"
    if _sha1(paths[0]) != e.get("kept_object_sha1"):
        return f"{paths[0]} changed since the report"
    if main and Path(paths[0]).stat().st_mtime > Path(mp).stat().st_mtime + 1:
        return f"{paths[0]} is newer than the link map"
    kp = [p for p in _map_objects(mp).get((secname, kept), {"placed": []})["placed"] if p[2] == sym[0]]
    if len(kp) != 1 or kp[0][1] != sym[2]:
        return f"the map doesn't place the kept copy in {kept}'s {secname} with size 0x{sym[2]:x}"
    ke = Elf(paths[0])
    ks = [x for x in ke.syms if x[0] == sym[0] and 0 < x[5] < len(ke.sh)]
    if len(ks) != 1 or ke.names[ks[0][5]] != secname or ks[0][2] != sym[2]:
        return f"{paths[0]} doesn't define exactly one {secname} {sym[0]} of its size"
    ks = ks[0]
    if elf.data(sec)[sym[1]:sym[1] + sym[2]] != ke.data(ks[5])[ks[1]:ks[1] + ks[2]]:
        return f"its bytes differ from the kept copy in {paths[0]}"
    mine = _named_relocs(elf, sym)
    if mine is None or mine != _named_relocs(ke, ks):
        return f"its relocations differ from the kept copy's in {paths[0]} (or name a local symbol)"
    # What goes with it: up to the next symbol or the section end, zero padding only.
    lo, hi = sym[1], sym[1] + sym[2]
    nxt = min([x[1] for x in elf.syms if x[5] == sec and x[2] and x[1] >= hi] + [elf.sh[sec][5]])
    if e.get("removed") != nxt - lo or any(elf.data(sec)[hi:nxt]):
        return "the removed range isn't the object plus zero padding up to the next symbol"
    # Every allocated relocation into it or its padding names it (the link resolves it to the kept copy).
    refs = 0
    for r, h in enumerate(elf.sh):
        if not h[2] & SHF_ALLOC:
            continue
        for off, si, add in elf.relocs(r):
            t = elf.syms[si]
            if t[5] != sec or not lo <= t[1] + add < nxt or (r == sec and lo <= off < hi):
                continue
            if si != k:
                return f"{elf.names[r]}+0x{off:x} reaches it through {t[0] or 'the section symbol'}, not by its name"
            refs += 1
    if e.get("references") != refs:
        return f"the manifest counts {e.get('references')} references, the object has {refs}"
    return (sec, lo, nxt - lo, k)


def check_weak_layout(elf, sec, rem, mp, name):
    """A section that loses weak data must have the stripped layout in the map,
    and nothing (dead code included) may reach it through its section symbol or
    a size-0 label at or past the first removed byte. Returns why not, or None."""
    first = min(o for o, _ in rem)
    for r, h in enumerate(elf.sh):
        if not h[2] & SHF_ALLOC:
            continue
        for off, si, add in elf.relocs(r):
            t = elf.syms[si]
            if t[5] == sec and (t[4] == STT_SECTION or not t[2]) and t[1] + add >= first:
                return f"{elf.names[r]}+0x{off:x} reaches {elf.names[sec]} through {t[0] or 'its section symbol'} past a removed object"
    return _stripped_layout(elf, sec, rem, mp, name)


def check_dead_layout(elf, sec, rem, mp, name, entries):
    """The stripped section must have the layout the link gave this object's
    section: every kept sized symbol at the offset (from the object's section
    start) and with the size the map places it at, and the next object's
    section in the layout 0 to 7 bytes (alignment) after the stripped end.
    No live relocation may reach the section through its section symbol, or a
    size-0 label of it, at or past the first removed byte (its addend isn't
    moved); a size-0 label any allocated relocation targets must be listed
    UNUSED for this object, and so must each of its referencers, as the
    manifest's `anchors` say. Returns why not, or None."""
    secname = elf.names[sec]
    why = _stripped_layout(elf, sec, rem, mp, name)
    if why:
        return why
    first = min(o for o, _ in rem)
    anchors = {}
    for r, h in enumerate(elf.sh):
        if not h[2] & SHF_ALLOC:
            continue
        for off, si, add in elf.relocs(r):
            t = elf.syms[si]
            if t[5] != sec or t[2] and t[4] != STT_SECTION:
                continue
            src = _container(elf, r, off)
            dead = src is not None and _unused_line(elf, mp, name, r, src) is not None
            if t[4] != STT_SECTION:
                if _unused_line(elf, mp, name, sec, t) is None or not dead:
                    return f"its size-0 label {t[0] or '?'} is referenced, and it or {elf.names[r]}+0x{off:x} isn't listed UNUSED"
                anchors.setdefault(t[0], set()).add((elf.names[r], src[0]))
            if t[1] + add >= first and not dead:
                return f"live {elf.names[r]}+0x{off:x} reaches {secname} through its section symbol past a removed object"
    for e in entries:
        got = {a.get("symbol"): {(x.get("section"), x.get("symbol")) for x in a.get("referencers", [])}
               for a in e.get("anchors", []) if isinstance(a, dict)}
        if got != anchors:
            return f"{e.get('symbol')}: the manifest's anchors aren't the object's"
    return None


def check_weakdedup(problems):
    """Re-verify every extab entry the honest report left out (tools/honest_report.py)."""
    path = Path("build/G4SE01/weakdedup_manifest.json")
    if not path.is_file():
        problems.append(f"{path} missing: the honest report didn't run tools/honest_report.py's weak-duplicate check")
        return
    man = json.loads(path.read_text())
    expected = Path("config/G4SE01/build.sha1").read_text().split()[0]
    entries = man.get("entries", [])
    links = {}
    for key, rec in man.get("links", {}).items():
        links[key] = check_link_record(key, rec, expected) + (rec,)
    by_stripped, seen, dd_sections, rec_maps, dd_entries = {}, set(), set(), {}, {}
    wd_sections, undef = set(), {}
    for e in entries:
        tag = f"weakdedup {e.get('unit')}: {e.get('function')}"
        kind = e.get("kind", "weak_extab")
        if kind in ("dead_data", "weak_data"):
            tag = f"{kind.replace('_', ' ')} {e.get('unit')}: {e.get('symbol')}"
        elif kind != "weak_extab":
            problems.append(f"{tag}: unknown kind of removal {kind!r}")
            continue
        # A data object is removed once, as dead or as weak data, never both.
        ident = (e.get("object"), e.get("symbol") if kind != "weak_extab" else e.get("extabindex"), kind == "weak_extab")
        if ident in seen:
            problems.append(f"{tag}: listed twice in the manifest")
            continue
        seen.add(ident)
        key = e.get("link")
        if key not in links:
            problems.append(f"{tag}: no link record {key}")
            continue
        why, inputs, objects, rec = links[key]
        if why:
            problems.append(f"{tag}: stale or unproven link: {why}")
            continue
        obj = e["object"]
        name = Path(obj).name
        if not Path(obj).is_file() or _sha1(obj) != e["object_sha1"]:
            problems.append(f"{tag}: {obj} changed since the report")
            continue
        if obj not in inputs or sum(Path(i).name == name for i in inputs) != 1:
            problems.append(f"{tag}: {obj} is not a uniquely named input of the link")
            continue
        if key == "main" and Path(obj).stat().st_mtime > Path(rec["map"]).stat().st_mtime + 1:
            problems.append(f"{tag}: {obj} is newer than the link map")
            continue
        if key != "main" and json.loads(Path(key).read_text()).get("objects", {}).get(obj) != e["object_sha1"]:
            problems.append(f"{tag}: {obj} isn't the object the trial link included")
            continue
        try:
            elf = Elf(obj)
        except (OSError, ValueError, StopIteration) as err:
            problems.append(f"{tag}: {err}")
            continue
        if kind == "weak_data":
            if rec_maps.setdefault(e["unit"], rec["map"]) != rec["map"]:
                problems.append(f"{tag}: the unit's removals cite different links")
                continue
            try:
                why = check_weak_data(e, elf, name, rec["map"], inputs, key == "main")
            except (OSError, ValueError, StopIteration, IndexError, struct.error) as err:
                why = f"{err}"
            if isinstance(why, str):
                problems.append(f"{tag}: {why}")
            else:
                wd_sections.add((e["unit"], why[0]))
                by_stripped.setdefault((e["unit"], obj, e["stripped_object"]), []).append(why[:3])
                undef.setdefault((e["unit"], obj, e["stripped_object"]), set()).add(why[3])
            continue
        if kind == "dead_data":
            if rec_maps.setdefault(e["unit"], rec["map"]) != rec["map"]:
                problems.append(f"{tag}: the unit's removals cite different links")
                continue
            why = check_dead_data(e, elf, name, rec["map"])
            if isinstance(why, str):
                problems.append(f"{tag}: {why}")
            else:
                dd_sections.add((e["unit"], why[0]))
                dd_entries.setdefault((e["unit"], e["section"]), []).append(e)
                by_stripped.setdefault((e["unit"], obj, e["stripped_object"]), []).append(why)
            continue
        # The object: the extabindex entry points at a WEAK FUNC and at the extab label.
        syms = {}
        for sym in elf.syms:
            syms.setdefault((sym[0], elf.names[sym[5]] if sym[5] < len(elf.names) else None), sym)
        fn = next((x for x in elf.syms if x[0] == e["function"] and x[5] < len(elf.names)), None)
        if not fn or fn[3] != 2 or fn[4] != 2:
            problems.append(f"{tag}: not a WEAK FUNC in {obj}")
            continue
        eti, etb = syms.get((e["extabindex"], "extabindex")), syms.get((e["extab"], "extab"))
        if not eti or not etb or eti[2] != 12 or etb[2] != e["extab_size"]:
            problems.append(f"{tag}: {e['extabindex']}/{e['extab']} aren't its extabindex/extab entries")
            continue
        rel = {off: elf.syms[s][0] for off, s, _ in elf.relocs(eti[5])}
        if rel.get(eti[1]) != e["function"] or rel.get(eti[1] + 8) != e["extab"]:
            problems.append(f"{tag}: {e['extabindex']} doesn't describe {e['function']} with {e['extab']}")
            continue
        # The map lines, read back from the map itself.
        lines = _map_lines(rec["map"])
        ev = e.get("evidence", [])
        texts = [lines[x["line"] - 1] if 0 < x["line"] <= len(lines) else None for x in ev]
        ok = len(ev) == 4 and all(t == x["text"] for t, x in zip(texts, ev))
        if ok:
            dup, found, u_eti, u_etb = ev
            m_dup = re.search(r">>> UNREFERENCED DUPLICATE (.+?)\s*$", dup["text"])
            m_found = re.search(r">>> \(func,weak\) found in (\S+)\s*$", found["text"])
            m_eti = re.match(r"^\s+UNUSED\s+([0-9a-f]+) \.+ (\S+) (\S+)\s*$", u_eti["text"])
            m_etb = re.match(r"^\s+UNUSED\s+([0-9a-f]+) \.+ (\S+) (\S+)\s*$", u_etb["text"])
            ok = bool(m_dup and m_found and m_eti and m_etb
                      and m_dup[1] == e["function"] and m_found[1] == name
                      and found["line"] > dup["line"]
                      and all(">>> (" in lines[i] for i in range(dup["line"], found["line"] - 1))
                      and (int(m_eti[1], 16), m_eti[2], m_eti[3]) == (12, e["extabindex"], name)
                      and _map_section(lines, u_eti["line"]) == "extabindex"
                      and (int(m_etb[1], 16), m_etb[2], m_etb[3]) == (e["extab_size"], e["extab"], name)
                      and _map_section(lines, u_etb["line"]) == "extab")
        if not ok:
            problems.append(f"{tag}: map evidence missing or doesn't match {rec['map']}")
            continue
        by_stripped.setdefault((e["unit"], obj, e["stripped_object"]), []).append((eti[5], eti[1], 12))
        by_stripped[(e["unit"], obj, e["stripped_object"])].append((etb[5], etb[1], etb[2]))
    # Every unit diffed against another object than its compiled one is in the
    # manifest, and that copy is the compiled object less exactly those entries.
    honest = Path("build/G4SE01/honest/objdiff.json")
    if not honest.is_file():
        problems.append(f"{honest} missing")
        return
    orig = {u["name"]: u.get("base_path") for u in json.loads(Path("objdiff.json").read_text())["units"]}
    proven = {u: (o, s) for u, o, s in by_stripped}
    root = Path.cwd()
    for u in json.loads(honest.read_text())["units"]:
        base, want = u.get("base_path"), orig.get(u["name"])
        if not base or not want or Path(base) == root / want:
            continue
        if u["name"] not in proven or Path(base) != root / proven[u["name"]][1]:
            problems.append(f"{u['name']}: the honest report diffs {base}, not its compiled object, without proven entries")
            continue
        obj, stripped = proven[u["name"]]
        removed = {}
        for sec, off, size in by_stripped[(u["name"], obj, stripped)]:
            removed.setdefault(sec, []).append((off, size))
        try:
            a = Elf(obj)
            diffs = [f"{a.names[sec]}: {why}" for (unit, sec) in sorted(dd_sections) if unit == u["name"]
                     for why in [check_dead_layout(a, sec, removed[sec], rec_maps[u["name"]], Path(obj).name,
                                                       dd_entries[(u["name"], a.names[sec])])] if why]
            diffs += [f"{a.names[sec]}: {why}" for (unit, sec) in sorted(wd_sections) if unit == u["name"]
                      for why in [check_weak_layout(a, sec, removed[sec], rec_maps[u["name"]], Path(obj).name)] if why]
            diffs += expected_stripped(a, removed, undef.get((u["name"], obj, stripped), set()))(Elf(stripped))
        except (OSError, ValueError, StopIteration, IndexError, struct.error) as err:
            diffs = [str(err)]
        problems += [f"{u['name']}: {stripped}: {d}" for d in diffs]


def check_link(problems):
    """Every Matching unit must link: main.dol's SHA-1 must equal build.sha1."""
    out = ninja("build/G4SE01/ok")
    if out.returncode == 0:
        return
    tail = (out.stdout + out.stderr)[-1500:]
    result = Path("build/link_check/result.json")
    result.unlink(missing_ok=True)
    subprocess.run(["timeout", "3600", sys.executable, "tools/link_check.py", "--json", str(result)],
                   capture_output=True, text=True)
    failing = [u for u in json.loads(result.read_text())["units"] if not u["identical"]] if result.exists() else []
    for u in failing:
        problems.append(f"{u['unit']}: marked Matching but doesn't link byte-identical ({u['reason']})")
    if not failing:
        problems.append("main.dol doesn't link or its SHA-1 differs, and tools/link_check.py "
                        "couldn't attribute it to a Matching unit:\n" + tail)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--base", default="matching")
    ap.add_argument("--all-units", action="store_true")
    ap.add_argument("--no-link", action="store_true", help="skip the link check (not for merges)")
    args = ap.parse_args()
    problems = []
    if not args.all_units:
        # Compare against where the branch started, not the base's current tip.
        args.base = git("merge-base", args.base, "HEAD").strip()
        changed = set(git("diff", "--name-only", f"{args.base}...HEAD").split()) | set(git("diff", "--name-only", "HEAD").split())
        check_sources(args.base, sorted(changed), problems)
        check_provenance(args.base, sorted(changed), problems)
        check_protected(args.base, sorted(changed), problems)
        check_symbols(args.base, changed, problems)
        check_byte_arrays(args.base, sorted(changed), problems)
    tracked = git("ls-files", "--", "build/").split()
    if tracked:
        problems.append(f"build artifacts tracked by git (never commit build/): {', '.join(tracked[:5])}")
    check_blobs(problems)
    check_configure(problems)
    check_report(problems)
    if not args.no_link:
        check_link(problems)
    check_fakematch(problems)
    for p in problems:
        print("FAIL", p)
    print("gate: ok" if not problems else f"gate: {len(problems)} problem(s)")
    sys.exit(1 if problems else 0)


if __name__ == "__main__":
    main()
