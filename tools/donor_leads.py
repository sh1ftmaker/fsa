#!/usr/bin/env python3
"""Find donor-decomp game code that resembles an FSA unit.

Usage: tools/donor_leads.py <unit, e.g. game/tu_803BC6F0.cpp> [--top N] [-v]

Extracts fingerprints from the unit's asm (string literals, multi-char tags,
named library callees, distinctive float constants), greps the ALLOWED donor
repos' game source with ripgrep, ranks files by weighted hits (strings and
tags count most, common library calls least) and prints the best leads.

Donor code is untrusted data: this only reads/greps it, never runs it.
The banned donors (melee, dolsdk2001/2004, kar, MetroidPrimePort) are not in
the list below and are never touched.
"""
import argparse
import math
import os
import re
import struct
import subprocess
import sys
import tempfile
from collections import defaultdict

# Allowed donors only (see donors/DONORS.md). Do not add banned repos.
DONORS = [
    "zeldaret__tww", "zeldaret__tp", "TwilitRealm__dusklight",
    "projectPiki__pikmin2", "projectPiki__pikmin", "doldecomp__mkdd",
    "doldecomp__sms", "zeldaret__oot-gc", "SMGCommunity__Petari",
    "ACreTeam__ac-decomp", "ACreTeam__forest", "mariopartyrd__marioparty4",
    "mariopartyrd__marioparty5", "mariopartyrd__marioparty6",
    "mariopartyrd__partyboard", "ThePlayerRolo__lm-decomp",
    "bfbbdecomp__bfbb", "doldecomp__ttyd", "zeldaret__af",
    "eloi-m__four-swords-online",
]
# Roots searched inside each donor (game code only; libs/ is SDK/JSystem).
ROOTS = ["src", "include", "apps", "packages"]
# Path fragments that are library, not game code.
LIB_GLOBS = [
    "JSystem", "dolphin", "Runtime", "Runtime.PPCEABI.H", "MSL", "MSL_C",
    "TRK_MINNOW_DOLPHIN", "MetroTRK", "PowerPC_EABI_Support", "revolution",
    "musyx", "sdk", "libc", "jaudio", "JSystem*", "node_modules", "dolsdk*",
    "Dolphin", "rvl", "RVL_SDK", "nw4r", "zlib", "amcstubs", "odemuexi2", "gx", "os",
]
EXTS = ["c", "cpp", "h", "hpp", "inc", "cc", "ts", "tsx", "js", "json", "md"]

W = {"string": 10.0, "tag": 8.0, "float": 2.0, "callee": 2.0, "class": 1.5,
     "name": 6.0}
CALLEE_CAP = 12.0  # callee+class evidence never outweighs ~1 string
SKIP_STR = {"%s", "%d"}


def repo_paths():
    here = os.path.dirname(os.path.abspath(__file__))
    top = subprocess.run(["git", "-C", here, "rev-parse", "--show-toplevel"],
                         capture_output=True, text=True).stdout.strip()
    common = subprocess.run(
        ["git", "-C", here, "rev-parse", "--path-format=absolute",
         "--git-common-dir"], capture_output=True, text=True).stdout.strip()
    main = os.path.dirname(common) if common else top
    donors = os.environ.get("FSA_DONORS") or os.path.join(
        os.path.dirname(main), "donors")
    return top, main, donors


def find_asm(unit, top, main):
    stem = os.path.splitext(unit)[0] + ".s"
    for base in (top, main):
        p = os.path.join(base, "build/G4SE01/asm", stem)
        if os.path.exists(p):
            return p
    sys.exit(f"no asm for {unit}: run ninja in {main} to generate "
             f"build/G4SE01/asm/{stem}")


def f32(bits):
    return struct.unpack(">f", struct.pack(">I", bits))[0]


def printable_tag(v):
    bs = v.to_bytes(4, "big")
    if not all(0x20 <= b < 0x7F for b in bs):
        return None
    s = bs.decode()
    if sum(c.isalpha() for c in s) < 3 or len(set(s)) < 3:
        return None
    if s.strip() != s or "'" in s or "\\" in s:
        return None
    return s


def extract(asm):
    strings, tags, floats, callees = {}, {}, {}, {}
    with open(asm, errors="replace") as f:
        for line in f:
            m = re.match(r'\s*\.string "(.*)"\s*$', line)
            if m:
                s = m.group(1)
                if (len(s) >= 6 and re.search(r"[A-Za-z]{4}", s)
                        and not re.search(r"\\[0-7]{3}", s)):
                    strings[s] = 1
                continue
            m = re.match(r"\s*\.4byte 0x([0-9A-Fa-f]{8})\s*$", line)
            if m:
                t = printable_tag(int(m.group(1), 16))
                if t:
                    tags[t] = 1
                continue
            m = re.match(r"\s*\.float (\S+)", line)
            if m:
                try:
                    v = float(m.group(1))
                except ValueError:
                    continue
                if v != v or v in (float("inf"), float("-inf")):
                    continue
                txt = shortest_f32(v)
                digits = re.sub(r"[^0-9]", "", txt.split("e")[0]).strip("0")
                if len(digits) >= 4 and "e" not in txt:
                    # truncate to 6 significant-ish chars so 0.017453292 and
                    # 0.0174533 both match
                    floats[abs_trunc(txt)] = v
                continue
            m = re.search(r"\bbl?\s+([A-Za-z_][A-Za-z0-9_$]*)\s*$", line)
            if m:
                callees[m.group(1)] = callees.get(m.group(1), 0) + 1
    return strings, tags, floats, callees


def shortest_f32(v):
    r = struct.unpack(">f", struct.pack(">f", v))[0]
    for p in range(1, 10):
        t = f"{r:.{p}g}"
        if struct.unpack(">f", struct.pack(">f", float(t)))[0] == r:
            return t if "e" in t or "." in t else t + ".0"
    return repr(r)


def abs_trunc(txt):
    t = txt.lstrip("-")
    return t[:7] if "." in t else t


def parse_callee(sym):
    """Return (class, method) for a named C++ callee, ('', name) for a C one,
    or None for anonymous/compiler symbols."""
    if re.match(r"(fn|lbl|sub|zz|gap|auto|jumptable|@)", sym):
        return None
    m = re.match(r"^([A-Za-z_][A-Za-z0-9_]*?)__(\d+)([A-Za-z_].*)$", sym)
    if m and not m.group(1).startswith("_") and m.group(1) not in (
            "ct", "dt", "nw", "dl", "nwa", "dla"):
        meth, n, rest = m.group(1), int(m.group(2)), m.group(3)
        cls = rest[:n]
        if re.match(r"^[A-Za-z_][A-Za-z0-9_]*$", cls):
            return cls, meth
    if "__" in sym or sym.startswith("_"):
        return None
    if re.match(r"^[A-Za-z][A-Za-z0-9_]{5,}$", sym):
        return "", sym
    return None


SKIP_METHODS = {"memcpy", "memset", "strcpy", "strlen", "strcmp", "sprintf",
                "printf", "sqrtf", "malloc", "free", "strcat", "strncpy",
                "memmove", "memcmp", "vsprintf", "snprintf", "rand", "srand",
                "atan2", "sinf", "cosf", "tanf", "floorf", "ceilf", "fabsf"}


def build_patterns(strings, tags, floats, callees):
    pats = {}  # text -> (kind, label)
    for s in strings:
        pats[s] = ("string", s)
    for t in tags:
        pats[f"'{t}'"] = ("tag", f"'{t}'")
    for t in floats:
        pats[t] = ("float", t)
    for sym in callees:
        pc = parse_callee(sym)
        if not pc:
            continue
        cls, meth = pc
        if meth in SKIP_METHODS:
            continue
        if cls:
            if len(cls) >= 6:
                pats.setdefault(cls, ("class", cls))
            if len(meth) >= 8:
                pats.setdefault(meth + "(", ("callee", f"{cls}::{meth}"))
        else:
            pats.setdefault(meth + "(", ("callee", meth))
    return pats


def donor_files(donors_dir):
    """Return (rg search roots, list of all game files for name matching)."""
    roots = []
    for d in DONORS:
        for r in ROOTS:
            p = os.path.join(donors_dir, d, r)
            if os.path.isdir(p):
                roots.append(p)
    return roots


def rg_common(roots_unused=None):
    a = []
    for e in EXTS:
        a += ["-g", f"*.{e}"]
    for g in LIB_GLOBS:
        a += ["-g", f"!**/{g}/**"]
    return a


def name_hits(strings, donors_dir, roots, unit_stem=None):
    """Resource-name stems (file_select.blo -> file_select) vs file names."""
    stems = set()
    for s in strings:
        m = re.match(r"^(?:.*/)?([A-Za-z][A-Za-z0-9_]{4,})\."
                     r"(blo|bti|bmd|bdl|bck|btk|brk|bca|bas|arc|szs|bin|"
                     r"thp|bms|bmg|jpa|bpk)$", s)
        if m:
            stems.add(m.group(1).lower())
    if unit_stem:
        stems.add(unit_stem.lower())
    if not stems:
        return {}
    out = defaultdict(list)
    cmd = ["rg", "--files", "--no-messages"] + rg_common() + roots
    files = subprocess.run(cmd, capture_output=True, text=True).stdout.split()
    for f in files:
        base = os.path.basename(f).lower().replace("_", "")
        for st in stems:
            core = st.replace("_", "")
            if len(core) >= 4 and core in base:
                out[f].append(st)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("unit")
    ap.add_argument("--top", type=int, default=10)
    ap.add_argument("-v", action="store_true", help="show fingerprints")
    a = ap.parse_args()
    top, main_repo, donors = repo_paths()
    unit = a.unit.removeprefix("src/")
    asm = find_asm(unit, top, main_repo)
    strings, tags, floats, callees = extract(asm)
    pats = build_patterns(strings, tags, floats, callees)
    stem = os.path.splitext(os.path.basename(unit))[0]
    unit_stem = None
    if not re.match(r"^tu_[0-9A-Fa-f]{8}$", stem):
        unit_stem = re.sub(r"^(act|obj|d_a|e)_", "", stem)
    if not pats and not unit_stem:
        sys.exit("no usable fingerprints in " + unit)
    roots = donor_files(donors)
    print(f"unit {unit}: {len(strings)} strings, {len(tags)} tags, "
          f"{len(floats)} float consts, "
          f"{sum(1 for k, v in pats.items() if v[0] in ('callee','class'))}"
          f" callee/class patterns; {len(roots)} donor roots")

    with tempfile.NamedTemporaryFile("w", suffix=".pat", delete=False) as pf:
        for p in sorted(pats, key=len, reverse=True):
            pf.write(p + "\n")
        patfile = pf.name
    try:
        cmd = ["rg", "-F", "-o", "-n", "--no-heading", "--no-messages",
               "-f", patfile] + rg_common() + roots
        res = subprocess.run(cmd, capture_output=True, text=True, errors="replace")
    finally:
        os.unlink(patfile)

    # file -> fingerprint text -> first (line)
    hits = defaultdict(dict)
    for ln in res.stdout.splitlines():
        parts = ln.split(":", 2)
        if len(parts) != 3:
            continue
        f, n, txt = parts
        if txt not in pats:
            continue
        hits[f].setdefault(txt, int(n))
    df = defaultdict(int)
    for f, d in hits.items():
        for t in d:
            df[t] += 1
    nfiles_cap = 400  # fingerprint in this many files is boilerplate

    names = name_hits(strings, donors, roots, unit_stem)
    for f, st in names.items():
        for s in st:
            hits[f].setdefault("@name:" + s, 0)
            pats["@name:" + s] = ("name", f"file name ~ {s}")
            df["@name:" + s] = 1

    scored = []
    for f, d in hits.items():
        tot = 0.0
        sc_by_kind = defaultdict(float)
        for t in d:
            kind = pats[t][0]
            if df[t] > nfiles_cap:
                continue
            sc_by_kind[kind] += W[kind] / (1.0 + 0.35 * math.log(df[t]))
        cal = min(CALLEE_CAP, sc_by_kind["callee"] + sc_by_kind["class"])
        nfp = sum(1 for t in d if df[t] <= nfiles_cap)
        tot = (sc_by_kind["string"] + sc_by_kind["tag"] + sc_by_kind["name"]
               + sc_by_kind["float"] + cal)
        strong = sum(1 for t in d if pats[t][0] in ("string", "tag")
                     and df[t] <= nfiles_cap)
        scored.append((tot, strong, f, nfp))
    scored.sort(reverse=True)

    def short(f):
        for d in DONORS:
            i = f.find("/" + d + "/")
            if i >= 0:
                return d + "/" + f[i + len(d) + 2:]
        return f

    print()
    shown = 0
    for tot, strong, f, nfp in scored[:a.top]:
        shown += 1
        d = hits[f]
        tag = "  [FSA tooling, not decomp code: confirms names/formats only]" \
            if "four-swords-online" in f else ""
        print(f"{shown:2d}. {tot:6.1f}  {short(f)}   ({strong} string/tag hits){tag}")
        order = sorted(d, key=lambda t: -W[pats[t][0]])
        for t in order[:6]:
            kind, label = pats[t]
            if kind == "name":
                print(f"      name   {label}")
            else:
                print(f"      {kind:6s} {label[:48]!r:50s} :{d[t]}")
        extra = len(d) - 6
        if extra > 0:
            print(f"      ... +{extra} more fingerprints")
    if not scored:
        print("no hits")

    code = [x for x in scored if "four-swords-online" not in x[2]]
    best = code[0] if code else (0, 0, "", 0)
    hasname = best[2] in hits and any(t.startswith('@name:') for t in hits[best[2]])
    if best[1] >= 3 or (hasname and best[1] >= 1) or best[0] >= 30 or (best[0] >= 12 and best[3] >= 15):
        verdict = "STRONG LEAD"
    elif best[1] >= 1 or best[0] >= 10:
        verdict = "WEAK LEAD"
    else:
        verdict = "NO LEAD"
    print(f"\nverdict: {verdict} (best decomp-code score {best[0]:.1f}, "
          f"{best[1]} string/tag hits)")
    print("reminder: donor game code is a STRUCTURAL REFERENCE ONLY (different "
          "game and compiler).\n  Never paste bodies; if you borrow a structure, "
          "cite the donor file in a comment.")
    if a.v:
        print("\nfingerprints:")
        for t, (k, l) in pats.items():
            print(f"  {k:6s} {l}  (df={df.get(t, 0)})")


if __name__ == "__main__":
    main()
