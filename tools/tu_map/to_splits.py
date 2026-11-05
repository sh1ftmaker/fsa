"""Turn tu_map.tsv into splits.txt stanzas for the game region.

Run from build/tu_map after refs.py (see README.md):

    python3 ../../tools/tu_map/to_splits.py            # writes game_splits.txt, game_units.tsv
    python3 ../../tools/tu_map/to_splits.py --apply    # also replaces the game stanzas in splits.txt
                                                       # and writes config/G4SE01/game_units.tsv

Each unit gets the sections its evidence covers:
- .text: the tu_map.tsv row, minus leading this-adjusting thunks, which go
  to the previous unit when its .data holds their vtable (CodeWarrior emits
  thunks at the end of the file that emits the vtable).
- extabindex / extab: the DOL's extabindex entries for the unit's functions
  (12 bytes each, in .text order); extab runs from the unit's first entry to
  the next unit's first entry.
- .ctors: the slot holding a pointer to the unit's __sinit.
- .rodata / .data / .bss / .sdata / .sbss / .sdata2: tiled from the data the
  unit owns ("anchors"): data its code alone references, then, transitively
  across sections, data only its owned data points to; also a .bss/.sbss
  object only one unit's __sinit references, and a vtable several units
  reference, to the one that defines most of its virtuals. Between two units the cut goes
  where the fewest unassigned symbols land on the wrong side (by the code
  and data that reference them); ties go to the later unit's first anchor.
  A section whose anchors interleave with another unit's is left out for
  both (reported), as is anything that would overlap an existing split.
Names come from name_units(): a __FILE__-style source name in the
unit's strings, or the actor table (docs/names/actors.tsv) through the actor
spawner's vtables; otherwise game/tu_<addr>.cpp.
"""
import os, re, sys, json, bisect, struct
from collections import defaultdict, Counter

sys.path.insert(0, os.path.dirname(__file__))
from common import ROOT, rd, u32, load_syms, load_splits

GAME0, TEXT_END = 0x800F39EC, 0x8045BCE4  # .text ends 0x8045BCE4 (tu_map.tsv says ..BD00)
# Game code linked before JSystem (LIBMAP.md): one unit with extab (game
# flags), __sinit 0x80021848. The next piece, 0x80021D60-0x80021F24, is
# JSystem's JAIAudience.cpp (no extab; a library stanza in splits.txt).
EARLY = [(0x80021840, 0x80021D60)]
SEC = {'.rodata': (0x8045C660, 0x804916A0), '.data': (0x804916A0, 0x804B2140),
       '.bss': (0x804B2140, 0x80539BC0), '.sdata': (0x80539BC0, 0x8053A280),
       '.sbss': (0x8053A280, 0x8053AFA0), '.sdata2': (0x8053AFA0, 0x80543EF4)}
EXTABINDEX = (0x80011860, 0x80021814)  # _eti_init_info follows
EXTAB_END = 0x80011854
CTORS = (0x8045BD00, 0x8045C630)
SPAWNER_AT = 0x801F9484  # the actor spawner, by address (its name: spawner())
DATASECS = ['.rodata', '.data', '.bss', '.sdata', '.sbss', '.sdata2']


def secof(a):
    for k, (s, e) in SEC.items():
        if s <= a < e:
            return k


# Corrections to tu_map.tsv that dtk's split proved: unit start -> (starts of
# the following rows it absorbs, why). See docs/PLAN.md, "Phase 1".
MERGE = {
    0x80457DFC: ([0x8045A338], 'dtk: conflicting splits within reversed extab group'),
    # The THP player: tu_map's cuts at 0x804280C0 and 0x8042E780 were
    # placeholders (thp_split). .data and .sbss of 0x80427D2C.. sit inside the
    # player's (8053AEBD is used on both sides), in reversed order like the
    # rest of this -inline deferred file; it ends at the __sinit 0x8042E788.
    0x80427D2C: ([0x804280C0, 0x8042E780], 'shared .sbss/.data with the THP player; reversed (deferred) data order'),
}


def load_units():
    rows = []
    path = ROOT + '/config/G4SE01/tu_map.tsv'
    for l in open(path).read().split('\n')[1:]:
        if not l: continue
        c = l.split('\t')
        rows.append(dict(start=int(c[0], 16), end=min(int(c[1], 16), TEXT_END), conf=c[5], suggested=c[6],
                         hints=c[14] if len(c) > 14 else ''))
    merged = []
    for r in rows:
        last = merged[-1] if merged else None
        if last and last['start'] in MERGE and r['start'] in MERGE[last['start']][0]:
            last['end'] = r['end']
            last['merged'] = last.get('merged', []) + [r['start']]
            last['conf'] = 'med' if last['conf'] == 'high' else last['conf']
            continue
        merged.append(r)
    rows = merged
    for a, b in reversed(EARLY):
        rows.insert(0, dict(start=a, end=b, conf='med', suggested=f'tu_{a:08X}', hints='', early=True))
    for r in rows:
        r['name_start'] = r['start']  # names keep the row's start when thunks move the cut
    return rows


def is_thunk(a):
    """`subi r3, r3, N; b fn`: a this-adjusting thunk (8 bytes)."""
    w0, w1 = u32(a), u32(a + 4)
    return w0 >> 16 == 0x3863 and w1 >> 26 == 18 and not w1 & 3


def vtable_entries(s):
    """Code pointers of a vtable (RTTI off: words 0 and 1 are zero), else None."""
    a, n = s['addr'], s['size']
    if n < 12 or n % 4 or u32(a) or u32(a + 4):
        return None
    ws = [u32(a + i) for i in range(8, n, 4)]
    if not all(w == 0 or 0x80005600 <= w < TEXT_END for w in ws) or not any(ws):
        return None
    return [w for w in ws if w]


def main(apply):
    syms = load_syms()
    R = json.load(open('refs.json'))
    byname = {s['name']: s for s in syms}
    units = load_units()
    starts = [u['start'] for u in units]

    def unit_of(a):
        i = bisect.bisect_right(starts, a) - 1
        return i if i >= 0 and a < units[i]['end'] else None

    fns = sorted([s for s in syms if s['sec'] == '.text' and s['type'] == 'function'], key=lambda s: s['addr'])
    # who references each data symbol: units (code), and outside-game code
    code_refs = defaultdict(set)
    outside = Counter()
    for f in fns:
        u = unit_of(f['addr'])
        for a in R['fn'].get(f['name'], []):
            if u is None:
                outside[a] += 1
            else:
                code_refs[a].add(u)
    # The actor spawner's file holds inline copies of other files' actor
    # constructors, so its references to their vtables are copies: when
    # another unit references the same symbol, the spawner's file doesn't
    # count towards ownership.
    spawner = unit_of(SPAWNER_AT)
    for a, us in code_refs.items():
        if spawner in us and len(us) > 1:
            us.discard(spawner)
    data_refs = defaultdict(set)  # target addr -> set of data symbol addrs referencing it
    for dn, lst in R['data'].items():
        if dn in byname:
            for a in lst:
                data_refs[a].add(byname[dn]['addr'])

    existing = load_splits()
    exist_by_sec = defaultdict(list)
    for name, sec, s, e in existing:
        if name.startswith('game/'):
            continue  # our own earlier output
        exist_by_sec[sec].append((s, e, name))

    def overlaps(sec, s, e):
        return [n for (a, b, n) in exist_by_sec[sec] if a < e and s < b]

    out = defaultdict(dict)   # unit idx -> sec -> (s, e)
    report = []

    # ---- .text
    for i, u in enumerate(units):
        out[i]['.text'] = (u['start'], u['end'])

    # ---- extabindex / extab
    eti = []
    for a in range(EXTABINDEX[0], EXTABINDEX[1], 12):
        fa = u32(a)
        if fa:
            eti.append((a, fa, u32(a + 8)))
    per = defaultdict(list)
    for a, fa, ea in eti:
        u = unit_of(fa)
        if u is not None:
            per[u].append((a, ea))
    have = sorted(per)
    # extabindex entries are in .text order; check each unit's entries are contiguous
    for u in have:
        xs = [a for a, _ in per[u]]
        if xs != list(range(xs[0], xs[-1] + 12, 12)):
            report.append(f'extabindex not contiguous for {units[u]["start"]:08X}')
    all_ea = sorted(ea for _, _, ea in eti)
    for u in have:
        ixs = [a for a, _ in per[u]]
        out[u]['extabindex'] = (min(ixs), max(ixs) + 12)
        eas = sorted(ea for _, ea in per[u])
        lo, hi = eas[0], eas[-1]
        # extab ends where the next entry (of any unit) begins
        j = bisect.bisect_right(all_ea, hi)
        end = all_ea[j] if j < len(all_ea) else EXTAB_END
        # the unit's extab must be a contiguous run of entries
        inner = all_ea[bisect.bisect_left(all_ea, lo):j]
        if len(inner) != len(eas):
            report.append(f'extab of {units[u]["start"]:08X} interleaves with another unit')
            continue
        out[u]['extab'] = (lo, end)

    # ---- .ctors
    for a in range(CTORS[0], CTORS[1], 4):
        p = u32(a)
        u = unit_of(p) if p else None
        if u is not None:
            if '.ctors' in out[u]:
                report.append(f'two .ctors entries for {units[u]["start"]:08X}')
            out[u]['.ctors'] = (a, a + 4)

    # ---- data sections
    symlist = defaultdict(list)
    for s in syms:
        sec = secof(s['addr'])
        if sec and s['sec'] == sec:
            symlist[sec].append(s)
    # Owners over all data sections at once: a symbol only one unit's code
    # references (and no code outside the game) belongs to that unit; then,
    # transitively and across sections, a symbol only referenced from data
    # owned by one unit (a .data table's strings in .rodata, its floats in
    # .sdata2) belongs to that unit too. File-local data can't be pointed at
    # from another file's initialised data.
    alldata = [s for sec in DATASECS for s in symlist[sec]]
    # Each unit's __sinit (its .ctors slot) and what it references: a file's
    # global objects with constructors are built by its own __sinit, so a
    # .bss/.sbss symbol exactly one __sinit references belongs to that unit even when
    # other units' code uses it too (tu_8045B6F0's 49-entry JAISoundID table
    # at 0x8052DEE0, read by tu_8045B010). The JAudio memPools every __sinit
    # constructs have hundreds of such referencers and stay unowned.
    fname = {f['addr']: f['name'] for f in fns}
    sinit_refs = defaultdict(set)
    for a in range(CTORS[0], CTORS[1], 4):
        p = u32(a)
        u = unit_of(p) if p else None
        if u is not None:
            for t in R['fn'].get(fname.get(p), []):
                sinit_refs[t].add(u)
    gowner = {}
    rule_hits = Counter()
    for s in alldata:
        a = s['addr']
        us = code_refs.get(a, set())
        if outside[a]:
            continue
        if len(us) == 1:
            gowner[a] = next(iter(us))
        elif secof(a) in ('.bss', '.sbss') and len(sinit_refs.get(a, ())) == 1:
            gowner[a] = next(iter(sinit_refs[a]))
            rule_hits['sinit'] += 1
            report.append(f'{secof(a)} {a:08X}: built by the __sinit of {units[gowner[a]]["start"]:08X}, '
                          f'also used by {", ".join(f"{units[v]['start']:08X}" for v in sorted(us - {gowner[a]}))} '
                          f'-> owned by the __sinit\'s unit')
    # A vtable several units reference (constructors inlined elsewhere)
    # belongs to a referencing unit that defines some of its virtual
    # functions (non-thunk entries): the one that defines the most, and on a
    # tie the one where it fits the data order (fewest other units' owned
    # .data on the wrong side of it).
    dat_owned = sorted((a, u) for a, u in gowner.items() if secof(a) == '.data')

    def misplaced(a, u):
        return sum(1 for b, v in dat_owned if (v < u and b > a) or (v > u and b < a))

    for s in symlist['.data']:
        a = s['addr']
        us = code_refs.get(a, set())
        if a in gowner or outside[a] or len(us) < 2:
            continue
        ents = vtable_entries(s)
        if not ents:
            continue
        cnt = Counter(unit_of(w) for w in ents if not is_thunk(w))
        cands = sorted((-cnt[u], misplaced(a, u), u) for u in us if cnt[u])
        if not cands:
            continue
        if len(cands) > 1 and cands[0][:2] == cands[1][:2]:
            report.append(f'.data {a:08X}: vtable of several units, no single owner')
            continue
        u = cands[0][2]
        gowner[a] = u
        rule_hits['vtable'] += 1
        report.append(f'.data {a:08X}: vtable used by {len(us)} units, '
                      f'{-cands[0][0]} of its virtuals in {units[u]["start"]:08X} -> owned there')
    changed = True
    while changed:
        changed = False
        for s in alldata:
            a = s['addr']
            if a in gowner or code_refs.get(a) or outside[a] or not data_refs.get(a):
                continue
            us = {gowner.get(d) for d in data_refs[a]}
            if len(us) == 1 and None not in us:
                gowner[a] = next(iter(us))
                changed = True
    for sec in DATASECS:
        L = sorted(symlist[sec], key=lambda s: s['addr'])
        owner = dict(gowner)  # per section copy: foreign runs are dropped below
        anchors = [(s['addr'], owner[s['addr']], s) for s in L if s['addr'] in owner]
        # A unit's code may reference data defined in another unit (an extern
        # const). Split each unit's anchors into runs not interrupted by other
        # units' anchors and keep the longest run; the rest are foreign.
        runs = defaultdict(list)
        prev = None
        for a, u, s in anchors:
            if u != prev:
                runs[u].append([])
                prev = u
            runs[u][-1].append((a, u, s))
        kept = []
        med = {}
        for u, rs in runs.items():
            xs = sorted(a for r in rs for a, _, _ in r)
            med[u] = xs[len(xs) // 2]
        meds = sorted((m, u) for u, m in med.items())

        def inversions(u, r):
            # other units whose data would sit on the wrong side of this run
            lo_, hi_ = r[0][0], r[-1][0]
            return sum(1 for m, v in meds if (v < u and m > hi_) or (v > u and m < lo_))

        for u, rs in runs.items():
            best = min(rs, key=lambda r: (inversions(u, r), -len(r), r[0][0]))
            kept += best
            for r in rs:
                if r is not best:
                    report.append(f'{sec}: {units[u]["start"]:08X} references {r[0][0]:08X}'
                                  f'..{r[-1][0]:08X} inside another unit (extern, not split)')
                    for a, _, _ in r:
                        owner.pop(a, None)
        anchors = sorted(kept, key=lambda x: x[0])
        # per unit lo/hi
        lo, hi = {}, {}
        for a, u, s in anchors:
            lo[u] = min(lo.get(u, a), a)
            hi[u] = max(hi.get(u, 0), a + max(s['size'], 1))
        us = sorted(lo)
        bad = set()
        # interleaving: unit ranges must be ordered like the units
        for x, y in zip(us, us[1:]):
            if hi[x] > lo[y]:
                bad.add(x); bad.add(y)
        # early TU must precede every library split... just check overlaps below
        good = [u for u in us if u not in bad]
        for u in sorted(bad):
            report.append(f'{sec}: {units[u]["start"]:08X} interleaves with a neighbour '
                          f'({lo[u]:08X}-{hi[u]:08X}); left unsplit')
        # choose cuts between consecutive good units
        cut_lo = {u: lo[u] for u in good}
        cut_hi = {u: hi[u] for u in good}
        addrs = [s['addr'] for s in L]
        for x, y in zip(good, good[1:]):
            if any(b in bad for b in range(x + 1, y)):
                continue  # a bad unit sits between them; keep tight ranges
            if units[x].get('early') != units[y].get('early'):
                continue  # libraries sit between the early units and the rest
            g0, g1 = hi[x], lo[y]
            i0 = bisect.bisect_left(addrs, g0)
            i1 = bisect.bisect_left(addrs, g1)
            gap = L[i0:i1]
            cands = [g0] + [s['addr'] for s in gap] + [g1]

            def votes(s):
                v = set(code_refs.get(s['addr'], set())) | {owner.get(d) for d in data_refs.get(s['addr'], ())}
                return v

            best = None
            for c in cands:
                cost = 0
                for s in gap:
                    v = votes(s)
                    if s['addr'] < c and y in v and x not in v:
                        cost += 1
                    if s['addr'] >= c and x in v and y not in v:
                        cost += 1
                if best is None or cost <= best[0]:
                    best = (cost, c)
            c = best[1]
            # objects start 8-aligned in these sections (dtk warns otherwise):
            # move an unaligned cut down when that takes nothing x references
            c8 = c
            while c8 % 8 and (c8 & ~7) >= hi[x]:
                c8 &= ~7
                inside = [t for t in gap if t['addr'] < c8 < t['addr'] + t['size']]
                if inside:
                    c8 = inside[0]['addr']
            if c8 % 8 == 0 and c8 >= hi[x]:
                c = c8
            cut_hi[x] = c
            cut_lo[y] = c
        # The early unit's last range runs up to the next library split: the
        # JAudio memPools its __sinit builds first (0x804B21AC-0x804B2240,
        # template statics, kept from the first file that instantiates them)
        # are referenced by every game __sinit, so nothing anchors them.
        for u in good:
            if units[u].get('early') and u + 1 < len(units) and not units[u + 1].get('early'):
                nxt = [a for (a, b, n) in exist_by_sec[sec] if a >= cut_hi[u]]
                if nxt and not any(cut_hi[u] <= a < min(nxt) for a, v, _ in anchors if v != u):
                    cut_hi[u] = min(nxt)
        for u in good:
            s, e = cut_lo[u], cut_hi[u]
            ov = overlaps(sec, s, e)
            if ov:
                report.append(f'{sec}: {units[u]["start"]:08X} {s:08X}-{e:08X} overlaps {ov}; left unsplit')
                continue
            out[u][sec] = (s, e)

    # ---- thunks: CodeWarrior emits a vtable's this-adjusting thunks
    # (`subi r3, r3, N; b fn`) at the end of the file that emits the vtable,
    # after its __sinit. tu_map cuts at the __sinit's end, so they open the
    # next unit; each goes to the unit whose .data holds its vtable.
    fstart = {f['addr'] for f in fns}
    vt_of = defaultdict(set)  # thunk -> vtable symbols holding it
    for s in symlist['.data']:
        for w in vtable_entries(s) or ():
            if is_thunk(w):
                vt_of[w].add(s['addr'])

    def data_unit(a):
        return next((v for v in out if '.data' in out[v] and out[v]['.data'][0] <= a < out[v]['.data'][1]), None)

    moved = set()
    for t in sorted(vt_of):
        u = unit_of(t)
        if u is None:
            continue  # library code
        owners = {data_unit(v) for v in vt_of[t]}
        lead = t == units[u]['start'] and t in fstart
        if len(owners) == 1 and lead and owners == {u - 1}:
            units[u - 1]['end'] = units[u]['start'] = t + 8
            starts[u] = t + 8
            out[u - 1]['.text'] = (out[u - 1]['.text'][0], t + 8)
            out[u]['.text'] = (t + 8, out[u]['.text'][1])
            moved.add(u)
            report.append(f'.text {t:08X}: thunk in vtable {min(vt_of[t]):08X} of {units[u - 1]["name_start"]:08X} '
                          f'-> moved there from {units[u]["name_start"]:08X}')
        elif owners != {u}:
            report.append(f'.text {t:08X}: thunk in vtables {sorted(vt_of[t])} owned by {owners}, '
                          f'in unit {units[u]["name_start"]:08X}; not moved')
    for u in moved:
        if units[u]['start'] >= units[u]['end']:
            raise SystemExit(f'unit {units[u]["name_start"]:08X} is only thunks')

    # ---- check: a unit's data pointing into another unit's .rodata/.sdata2
    # (file-local strings and constants) means a cut is still wrong there
    flat = sorted((r[0], r[1], u, sec) for u in out for sec, r in out[u].items() if sec in DATASECS)
    fstarts = [f[0] for f in flat]

    def place(a):
        i = bisect.bisect_right(fstarts, a) - 1
        return (flat[i][2], flat[i][3]) if i >= 0 and a < flat[i][1] else (None, None)

    cross = Counter()
    for dn, lst in R['data'].items():
        if dn not in byname:
            continue
        u, _ = place(byname[dn]['addr'])
        if u is None:
            continue
        for a in lst:
            v, sec = place(a)
            if v is not None and v != u and sec in ('.rodata', '.sdata2'):
                cross[(u, v, sec)] += 1
    for (u, v, sec), n in sorted(cross.items()):
        report.append(f'data of {units[u]["start"]:08X} points {n}x into {sec} of {units[v]["start"]:08X} '
                      f'(no owner reaches that table by name, so nothing moves the cut)')

    # ---- names
    names, evid = name_units(units, syms, byname, unit_of, out)

    order = ['.text', 'extab', 'extabindex', '.ctors', '.rodata', '.data', '.bss', '.sdata', '.sbss', '.sdata2']
    with open('game_splits.txt', 'w') as o:
        for i, u in enumerate(units):
            o.write(f'{names[i]}:\n')
            for sec in order:
                if sec in out[i]:
                    s, e = out[i][sec]
                    o.write(f'\t{sec:<11} start:0x{s:08X} end:0x{e:08X}\n')
            o.write('\n')
    with open('game_units.tsv', 'w') as o:
        o.write('unit\ttext_start\ttext_end\tconfidence\tsections\tname_evidence\n')
        for i, u in enumerate(units):
            secs = ','.join(s for s in order if s in out[i])
            o.write(f'{names[i]}\t0x{u["start"]:08X}\t0x{u["end"]:08X}\t{u["conf"]}\t{secs}\t{evid[i]}\n')
    cnt = Counter(s for i in out for s in out[i])
    print('units', len(units), dict(cnt))
    for r in report:
        print('NOTE', r)
    if apply:
        apply_splits(names)


def name_units(units, syms, byname, unit_of, out):
    names, evid, act_id = {}, {}, {}
    # 1. source file names in the unit's own strings (__FILE__)
    for i, u in enumerate(units):
        m = [h for h in u['hints'].split('|') if re.fullmatch(r'[A-Za-z_]\w*\.(cpp|c)', h)]
        if len(m) == 1:
            names[i] = f'game/{m[0]}'
            evid[i] = f'string "{m[0]}" referenced by the unit'
    # 2. actor table: spawner case -> ctor -> vtable (.data) or ctor address -> unit
    actors = {}
    for l in open(ROOT + '/docs/names/actors.tsv').read().splitlines()[1:]:
        c = l.split('\t')
        actors[c[0]] = dict(en=c[2], conf=c[5], file=c[6])
    cases = spawner_cases()
    per_unit = defaultdict(set)
    data_owner = {}
    for i in out:
        for sec in ('.data',):
            if sec in out[i]:
                data_owner[out[i][sec]] = i
    def owner_of_data(a):
        for (s, e), i in data_owner.items():
            if s <= a < e:
                return i
    spawner_unit = unit_of(SPAWNER_AT)
    for aid, ctor in cases.items():
        u = unit_of(ctor) if ctor else None
        how = f'ctor {ctor:08X}'
        if u == spawner_unit:
            vt = ctor_vtable(ctor)
            u = owner_of_data(vt) if vt else None
            how = f'vtable {vt:08X} set by ctor {ctor:08X}' if vt else how
        if u is not None and u != spawner_unit:
            per_unit[u].add((aid, how))
    for i, ids in per_unit.items():
        if i in names:
            evid[i] += '; actors ' + ','.join(sorted(a for a, _ in ids))
            continue
        files = {actors[a]['file'] for a, _ in ids if a in actors and actors[a]['conf'] in ('high', 'med')}
        lows = [a for a, _ in ids if a not in actors or actors[a]['conf'] == 'low']
        idtxt = '; '.join(f'{a} ({h})' for a, h in sorted(ids))
        if len(files) == 1 and not lows:
            names[i] = f'game/{files.pop()}.cpp'
            act_id[i] = '_'.join(sorted(a for a, _ in ids)).lower()
            evid[i] = f'actor spawner {spawner_base()}: {idtxt}; actors.tsv'
        else:
            evid[i] = f'actor spawner {spawner_base()}: {idtxt}; not named (several or low-confidence actors)'
    seen = Counter()
    for i, u in enumerate(units):
        if i not in names:
            names[i] = f'game/tu_{u["name_start"]:08X}.cpp'
            evid.setdefault(i, '')
        seen[names[i]] += 1
    # duplicate names: actors.tsv suggests one file stem for several IDs
    # (SPFR and BFRY are both act_great_fairy); add the actor ID to each,
    # anything else gets its address
    for i, u in enumerate(units):
        if seen[names[i]] > 1:
            stem = names[i][:-4]
            tag = act_id.get(i) or f'{u["name_start"]:08X}'
            names[i] = f'{stem}_{tag}.cpp'
    return names, evid


def asm_fn(name):
    asm = ROOT + '/build/G4SE01/asm'
    for dp, _, fs in os.walk(asm):
        for f in fs:
            p = os.path.join(dp, f)
            txt = open(p, errors='replace').read()
            m = re.search(rf'^\.fn "?{re.escape(name)}"?,.*?^\.endfn "?{re.escape(name)}"?$', txt, re.S | re.M)
            if m:
                return m.group(0)


_fn_cache = {}


def spawner_cases():
    """Actor ID -> constructor address, from the spawner's compare tree."""
    body = asm_fn(spawner())
    lines = body.splitlines()
    # Walk the compare tree along its branches (a register set by `lis` on one
    # path is not live on another), from the entry. An ID is accepted only from
    # `addi r0, rX, lo` / `cmpw r29, r0` / `beq L` with rX known from a `lis`
    # on the same path; case bodies (those beq targets) are not walked.
    label_at = {}
    ins = []
    for l in lines:
        m = re.match(r'^(\.L_\w+):', l)
        if m:
            label_at[m[1]] = len(ins)
            continue
        if '*/' in l:
            ins.append(l.split('*/')[-1].strip())
    case_label = {}
    WRITES = re.compile(r'^(?!st|cmp|b|mt)\w+\.? (r\d+),')
    todo = [(0, ())]
    seen = set()
    while todo:
        pc, regs0 = todo.pop()
        regs = dict(regs0)
        while pc < len(ins):
            key = (pc, tuple(sorted(regs.items())))
            if key in seen:
                break
            seen.add(key)
            i = ins[pc]
            m = re.match(r'lis (r\d+), (0x[0-9a-f]+)$', i)
            if m:
                regs[m[1]] = int(m[2], 16) << 16
                pc += 1
                continue
            m = re.match(r'addi r0, (r\d+), (-?0x[0-9a-f]+)$', i)
            if (m and m[1] in regs and pc + 2 < len(ins) and ins[pc + 1] == 'cmpw r29, r0'
                    and ins[pc + 2].startswith('beq .L_')):
                v = (regs[m[1]] + int(m[2], 16)) & 0xFFFFFFFF
                try:
                    aid = struct.pack('>I', v).decode('ascii')
                except UnicodeDecodeError:
                    aid = None
                if aid and re.fullmatch(r'[A-Za-z0-9]{4}', aid):
                    if aid in case_label and case_label[aid] != ins[pc + 2][4:]:
                        raise SystemExit(f'spawner: {aid} decoded twice')
                    case_label[aid] = ins[pc + 2][4:]
                pc += 3  # fall through: the case body is not part of the tree
                continue
            m = re.match(r'(b|beq|bne|blt|bge|bgt|ble) (\.L_\w+)$', i)
            if m:
                tgt = label_at[m[2]]
                if m[1] == 'b':
                    pc = tgt
                    continue
                todo.append((tgt, tuple(sorted(regs.items()))))
                pc += 1
                continue
            if i == 'blr' or i.startswith('bctr'):
                break
            w = WRITES.match(i)
            if w:
                regs.pop(w[1], None)
            pc += 1
    out = {}
    for aid, lab in case_label.items():
        n = label_at[lab]
        seen_new = False
        ctor = None
        for i in ins[n:n + 40]:
            m = re.match(r'bl (\S+)$', i)
            if m:
                if m[1].startswith('__nw__'):
                    seen_new = True
                    continue
                if seen_new:
                    ctor = m[1]
                    break
            if i.startswith('b .L') and seen_new:
                break
        if ctor:
            out[aid] = ctor
    syms = {s['name']: s['addr'] for s in load_syms()}
    return {a: syms.get(c) for a, c in out.items()}


def ctor_vtable(addr):
    name = None
    for s in load_syms_cached():
        if s['addr'] == addr and s['sec'] == '.text':
            name = s['name']
    if not name:
        return None
    body = asm_fn(name) or ''
    m = re.findall(r'addi r0, r\d+, (lbl_804[9AB][0-9A-F]{4})@l\n.*stw r0, 0x0\(r', body)
    if not m:
        return None
    return int(m[-1][4:], 16)


_S = None


def load_syms_cached():
    global _S
    if _S is None:
        _S = load_syms()
    return _S


def spawner():
    """The spawner's symbols.txt name (mangled), whatever tools/rename.py made it."""
    return next(s['name'] for s in load_syms_cached() if s['sec'] == '.text' and s['addr'] == SPAWNER_AT)


def spawner_base():
    return spawner().split('__')[0]


def apply_splits(names):
    path = ROOT + '/config/G4SE01/splits.txt'
    txt = open(path).read()
    keep = []
    for block in re.split(r'\n(?=\S)', txt):
        head = block.split('\n', 1)[0]
        if head.startswith('game/'):
            continue
        keep.append(block.rstrip('\n'))
    new = '\n\n'.join(keep).rstrip('\n') + '\n\n' + open('game_splits.txt').read().rstrip('\n') + '\n'
    open(path, 'w').write(new)
    import shutil
    shutil.copy('game_units.tsv', ROOT + '/config/G4SE01/game_units.tsv')


if __name__ == '__main__':
    main('--apply' in sys.argv)
