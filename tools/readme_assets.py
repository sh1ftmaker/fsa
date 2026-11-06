"""README visuals and badges, made from the same data as the progress website.

Called by tools/site_data.py after it has written site/data/. Writes:

  site/badges/hero.svg                 banner: the emblem (a gem per game category, a gold
                                       one for the libraries) and the headline numbers
  site/badges/regions-{dark,light}.svg the overview: where the remaining work is, the
                                       libraries as one group, the eight game categories
  site/badges/treemap/*.svg            the whole DOL: one linked image per game category and
                                       per library region (the README treemap), plus its legend
  site/badges/roadmap-{dark,light}.svg docs/roadmap.yml's phases as gems
  site/data/badge-*.json               shields.io endpoint badges (served by Pages)
  README.md                            the text between <!-- <name>:start --> and
                                       <!-- <name>:end --> markers (progress, treemap,
                                       libraries, game, now)
  CLAUDE.md                            the same for its <!-- status:start/end --> marker

All of it is original SVG drawn here: no fonts, logos or art from the game.
GitHub shows README images through <img>, so the files carry no script and
no external references; the only motion is CSS inside the SVG, switched off
under prefers-reduced-motion.
"""

import math
import re
from pathlib import Path
from xml.sax.saxutils import escape

ROOT = Path(__file__).resolve().parent.parent
BADGES = ROOT / "site/badges"
DATA = ROOT / "site/data"
README = ROOT / "README.md"
CLAUDE_MD = ROOT / "CLAUDE.md"
SITE = "https://sh1ftmaker.github.io/fsa/"

SANS = "-apple-system, BlinkMacSystemFont, 'Segoe UI', Helvetica, Arial, sans-serif"
MONO = "ui-monospace, SFMono-Regular, 'SF Mono', Menlo, Consolas, monospace"

# The site's colours and glyphs (site/app.js REGION, PART, HUE): the game code's
# eight categories (tools/site_data.py CATEGORIES; a categorical palette checked
# for colour-vision deficiency on dark and light panels), the boot-path parts of
# the site's toggle in the four Links' colours, the libraries in gold, a shade
# per region.
REGION = {
    "game": ("#a9afe0", "diamond", "Game"),
    "jsystem": ("#d29a2e", "triangle", "JSystem"),
    "sdk": ("#e9be5a", "circle", "SDK"),
    "gx": ("#c4862b", "square", "GX"),
    "runtime": ("#f2d384", "star", "MSL/RT/TRK"),
}
HUE = {"green": "#3aae5c", "red": "#ef5b52", "blue": "#3d9cf0", "purple": "#c35ee6", "gold": "#d29a2e",
       # the eight game categories
       "leaf": "#4aa75c", "crimson": "#c52607", "rose": "#a96087", "azure": "#508ae1",
       "teal": "#00a8a1", "violet": "#8c55b4", "cyan": "#269ac2", "olive": "#608c3d"}
LIB_REGIONS = ("sdk", "gx", "jsystem", "runtime")
PART_SHORT = {"player": "Player", "enemies": "Enemies", "npcs": "NPCs", "objects": "Objects",
              "stage": "Stage", "menus": "Menus", "core": "Core", "media": "Media"}
PART_LEGEND = {"enemies": "Enemies", "objects": "Objects"}  # shorter legend names; the rest use their own


def game_parts(summary):
    """The game region's libraries: the eight categories (tools/site_data.py CATEGORIES)."""
    game = next(r for r in summary["regions"] if r["id"] == "game")
    return [l for l in game["libs"] if l.get("key")]


def part_color(lib):
    return HUE.get(lib.get("hue"), HUE["gold"])


def lib_regions(summary):
    reg = {r["id"]: r for r in summary["regions"]}
    return [reg[i] for i in LIB_REGIONS if i in reg]


def libraries(summary):
    """summary["libraries"], or the same sums for an older summary.json."""
    if summary.get("libraries"):
        return summary["libraries"]
    regs = lib_regions(summary)
    return {"n": "Libraries", **{k: sum(r.get(k, 0) for r in regs) for k in ("c", "cm", "wm", "f", "fm")}}

THEMES = {
    "dark": {"bg": "#0b0d1a", "bg2": "#10132a", "panel": "#161a34", "line": "#262b4e", "line2": "#353b66",
             "ink": "#eceefb", "ink2": "#b4b9d8", "ink3": "#7c82a8", "track": "#1d2142", "gold": "#f3c969"},
    "light": {"bg": "#ffffff", "bg2": "#f6f7fb", "panel": "#f6f7fb", "line": "#dfe2ef", "line2": "#c9cde3",
              "ink": "#1b1f3a", "ink2": "#474d70", "ink3": "#6b7096", "track": "#e6e8f3", "gold": "#a8761a"},
}

MOTION = ("@media (prefers-reduced-motion: reduce) { * { animation: none !important; } }")


# ---------- small helpers ----------

def num(n):
    return f"{n:,}"


def pct(a, b, digits=None):
    """The site's pct(): 2 decimals under 10 %, 1 above; exact 0 and 100."""
    if not b:
        return "–"
    x = 100.0 * a / b
    if a == 0:
        return "0 %"
    if a >= b:
        return "100 %"
    if digits is None:
        digits = 2 if x < 10 else 1
    s = f"{x:.{digits}f}"
    if float(s) >= 100:  # never round a partial match up to 100
        s = f"{99.99 if digits >= 2 else 99.9:.{digits}f}"
    return f"{s} %"


def esc(s):
    return escape(str(s), {'"': "&quot;"})


def rhombus(cx, cy, w, h):
    return f"M{cx:g} {cy - h:g}L{cx + w:g} {cy:g}L{cx:g} {cy + h:g}L{cx - w:g} {cy:g}Z"


def rhombus_fill(a, h):
    """Height of a bottom-up fill in a rhombus of half-height h whose AREA is the fraction a."""
    a = max(0.0, min(1.0, a))
    return h * math.sqrt(2 * a) if a <= 0.5 else 2 * h - h * math.sqrt(2 * (1 - a))


def glyph(region, x, y, s, col=None):
    col = col or REGION[region][0]
    shape = REGION[region][1]
    h = s / 2
    if shape == "diamond":
        d = f'<path d="M{x + h:g} {y:g}L{x + s:g} {y + h:g}L{x + h:g} {y + s:g}L{x:g} {y + h:g}Z"/>'
    elif shape == "triangle":
        d = f'<path d="M{x + h:g} {y:g}L{x + s:g} {y + s:g}L{x:g} {y + s:g}Z"/>'
    elif shape == "circle":
        d = f'<circle cx="{x + h:g}" cy="{y + h:g}" r="{h:g}"/>'
    elif shape == "square":
        d = f'<rect x="{x:g}" y="{y:g}" width="{s:g}" height="{s:g}" rx="1.5"/>'
    else:
        pts = [(0.5, 0), (0.62, 0.38), (1, 0.5), (0.62, 0.62), (0.5, 1), (0.38, 0.62), (0, 0.5), (0.38, 0.38)]
        d = '<path d="M' + "L".join(f"{x + px * s:g} {y + py * s:g}" for px, py in pts) + 'Z"/>'
    return f'<g fill="{col}">{d}</g>'


def svg(w, h, body, title, style=""):
    return (f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="{h}" viewBox="0 0 {w} {h}" '
            f'role="img" aria-label="{esc(title)}">\n<title>{esc(title)}</title>\n'
            f'<style>text {{ font-family: {SANS}; }} .mono {{ font-family: {MONO}; }} {style} {MOTION}</style>\n'
            f'{body}\n</svg>\n')


def write_if_changed(path, text):
    path.parent.mkdir(parents=True, exist_ok=True)
    if not path.exists() or path.read_text() != text:
        path.write_text(text)


# ---------- hero ----------

def emblem_layout(keys):
    """The site's emblem layout (site/app.js drawEmblem), in a -140..140 box: the
    game categories as gems on a ring, clockwise from the top, each labelled
    outside the ring; the libraries' gem in the middle."""
    out = []
    n = len(keys)
    for i, key in enumerate(keys):
        a = 2 * math.pi * i / n - math.pi / 2
        cx, cy = 60 * math.cos(a), 60 * math.sin(a)
        lx, ly = 96 * math.cos(a), 96 * math.sin(a) + 3
        anchor = "middle" if abs(lx) < 20 else "start" if lx > 0 else "end"
        out.append((key, round(cx, 2), round(cy, 2), 15, 21, round(lx, 2), round(ly, 2), anchor))
    out.append(("libs", 0, 0, 20, 20, 0, 0, "middle"))
    return out


def emblem(summary, cx, cy, k, t):
    """The emblem: one rhombus per game category in its colour and a gold one
    in the middle for the libraries, each filled from the bottom to its share
    of matched code; the label outside gives the category and its share."""
    parts = {l["key"]: l for l in game_parts(summary)}
    L = libraries(summary)
    out = [f'<g transform="translate({cx} {cy}) scale({k})">',
           f'<circle class="spin" r="148" fill="none" stroke="{t["line2"]}" stroke-width="0.8" stroke-dasharray="40 6 2 6"/>',
           f'<circle r="84" fill="none" stroke="{t["line2"]}" stroke-width="0.8" stroke-dasharray="1 10"/>']
    defs = []
    for i, (key, gx, gy, w, h, lx, ly, anchor) in enumerate(emblem_layout(list(parts))):
        r = L if key == "libs" else parts.get(key)
        if not r:
            continue
        col = HUE["gold"] if key == "libs" else part_color(r)
        frac = r["cm"] / r["c"] if r["c"] else 0
        fh = rhombus_fill(frac, h)
        defs.append(f'<clipPath id="ec-{key}"><path d="{rhombus(gx, gy, w, h)}"/></clipPath>'
                    f'<linearGradient id="eg-{key}" x1="0" y1="1" x2="0" y2="0">'
                    f'<stop offset="0" stop-color="{col}"/><stop offset="1" stop-color="{col}" stop-opacity="0.65"/></linearGradient>')
        out.append(f'<path d="{rhombus(gx, gy, w, h)}" fill="{col}" fill-opacity="0.12" stroke="{col}" '
                   f'stroke-opacity="0.75" stroke-width="1.5"/>')
        if fh > 0:
            out.append(f'<g clip-path="url(#ec-{key})"><rect class="rise" style="animation-delay:{0.2 + 0.1 * i:.2f}s" '
                       f'x="{gx - w}" y="{gy + h - fh:.2f}" width="{2 * w}" height="{fh:.2f}" fill="url(#eg-{key})"/></g>')
        out.append(f'<path d="M{gx} {gy - h}L{gx + w} {gy}L{gx} {gy}Z" fill="#fff" fill-opacity="0.07"/>')
        if key != "libs":
            out.append(f'<text x="{lx}" y="{ly - 5}" text-anchor="{anchor}" font-size="9" font-weight="600" '
                       f'letter-spacing="0.08em" fill="{t["ink2"]}">{esc(PART_SHORT.get(key, key).upper())}</text>')
            out.append(f'<text x="{lx}" y="{ly + 7}" text-anchor="{anchor}" font-size="10.5" font-weight="700" '
                       f'fill="{t["ink"]}" class="mono">{esc(pct(r["cm"], r["c"]))}</text>')
    out.append("</g>")
    return "<defs>" + "".join(defs) + "</defs>" + "".join(out)


def hero(summary):
    t = THEMES["dark"]
    tot = summary["totals"]
    W, H = 1200, 400
    sha = tot["sha1_ok"]
    seal = ("SHA-1 OK" if sha else "SHA-1 NOT OK" if sha is not None else "SHA-1 not checked")
    seal_col = "#6fdc8c" if sha else HUE["red"]
    weighted_line = ""
    if tot.get("code_weighted"):
        weighted_line = (f'<text x="290" y="346" class="mono" font-size="14" fill="{t["gold"]}">weighted '
                         f'{esc(pct(tot["code_weighted"], tot["code"], 2))} with partial credit</text>')
    glows = "".join(
        f'<radialGradient id="glow{i}" cx="{x}" cy="{y}" r="0.6"><stop offset="0" stop-color="{c}" stop-opacity="0.22"/>'
        f'<stop offset="1" stop-color="{c}" stop-opacity="0"/></radialGradient>'
        for i, (x, y, c) in enumerate([(0.05, -0.1, HUE["green"]), (0.98, 0.0, HUE["red"]),
                                       (1.0, 1.1, HUE["blue"]), (0.0, 1.1, HUE["purple"])]))
    body = f"""<defs>{glows}
<linearGradient id="sky" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="{t['bg2']}"/><stop offset="1" stop-color="{t['bg']}"/></linearGradient>
<linearGradient id="four" x1="0" y1="0" x2="1" y2="0"><stop offset="0" stop-color="{HUE['green']}"/><stop offset="0.34" stop-color="{HUE['red']}"/><stop offset="0.67" stop-color="{HUE['blue']}"/><stop offset="1" stop-color="{HUE['purple']}"/></linearGradient>
<linearGradient id="goldtext" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#f3c969"/><stop offset="1" stop-color="{HUE['gold']}"/></linearGradient>
<pattern id="lattice" width="34" height="34" patternUnits="userSpaceOnUse" patternTransform="rotate(45)"><path d="M0 0H34M0 0V34" stroke="#fff" stroke-opacity="0.035"/></pattern>
<linearGradient id="fade" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="#fff"/><stop offset="0.8" stop-color="#fff" stop-opacity="0"/></linearGradient>
<mask id="fadeMask"><rect width="{W}" height="{H}" fill="url(#fade)"/></mask>
<clipPath id="card"><rect width="{W}" height="{H}" rx="20"/></clipPath>
</defs>
<g clip-path="url(#card)">
<rect width="{W}" height="{H}" fill="url(#sky)"/>
{''.join(f'<rect width="{W}" height="{H}" fill="url(#glow{i})"/>' for i in range(4))}
<rect width="{W}" height="{H}" fill="url(#lattice)" mask="url(#fadeMask)"/>
</g>
<rect x="0.5" y="0.5" width="{W - 1}" height="{H - 1}" rx="20" fill="none" stroke="{t['line2']}"/>
<text x="64" y="92" class="mono" font-size="14" font-weight="600" letter-spacing="0.18em" fill="{t['ink3']}">GAMECUBE · G4SE01 · MAIN.DOL</text>
<text x="60" y="178" font-size="82" font-weight="700" letter-spacing="-1.5" fill="url(#four)">Four Swords</text>
<text x="66" y="224" font-size="30" font-weight="500" letter-spacing="0.12em" fill="{t['ink']}">ADVENTURES</text>
<text x="66" y="262" font-size="19" fill="{t['ink2']}">A byte-matching decompilation, built with the original compilers</text>
<text x="64" y="336" class="mono" font-size="46" font-weight="700" fill="url(#goldtext)">{esc(pct(tot['code_matched'], tot['code'], 2))}</text>
<text x="290" y="306" font-size="15" fill="{t['ink2']}">of the code byte-identical (honest)</text>
<text x="290" y="326" class="mono" font-size="14" fill="{t['ink3']}">{num(tot['fns_matched'])} / {num(tot['fns'])} functions · {num(tot['matching'])} units Matching</text>
{weighted_line}
<g transform="translate(64 356)"><rect width="{24 + 8.4 * len(seal)}" height="24" rx="12" fill="{t['bg']}" fill-opacity="0.6" stroke="{t['line2']}"/>
<text x="12" y="16.5" class="mono" font-size="12" font-weight="600" fill="{seal_col}">{esc(seal)}</text></g>
{emblem(summary, 912, 200, 1.3, t)}"""
    style = (".spin { transform-box: fill-box; transform-origin: center; animation: spin 90s linear infinite; } "
             "@keyframes spin { to { transform: rotate(360deg); } } "
             ".rise { transform-box: fill-box; transform-origin: 50% 100%; animation: rise 1.6s cubic-bezier(.2,.8,.2,1) both; } "
             "@keyframes rise { from { transform: scaleY(0); } }")
    title = (f"Four Swords Adventures decompilation: {pct(tot['code_matched'], tot['code'], 2)} of the code matched, "
             f"{num(tot['fns_matched'])} of {num(tot['fns'])} functions")
    return svg(W, H, body, title, style)


# ---------- overview (regions-*.svg) ----------

def kib(b):
    return f"{b / 1048576:.2f} MiB" if b >= 1048576 else f"{b / 1024:.0f} KiB"


def regions_svg(summary, theme):
    """The overview: where the remaining work is, the libraries as one row
    (with each library's share), then the eight game categories with honest bars
    and a lighter weighted extension."""
    t = THEMES[theme]
    tot = summary["totals"]
    parts = game_parts(summary)
    L = libraries(summary)
    W, BX, BW = 960, 330, 440
    out = []
    # remaining work: one bar, a segment per category, width = bytes not yet byte-identical
    segs = [(p["n"], part_color(p), p["c"] - p["cm"]) for p in parts] + [("Libraries", HUE["gold"], L["c"] - L["cm"])]
    left = sum(x[2] for x in segs)
    game_left = left - segs[-1][2]
    out.append(f'<text x="28" y="40" font-size="16" font-weight="700" fill="{t["ink"]}">Remaining work</text>')
    out.append(f'<text x="28" y="60" font-size="12" fill="{t["ink3"]}">code not yet byte-identical, by category and the libraries</text>')
    out.append(f'<clipPath id="rw"><rect x="{BX}" y="28" width="{BW}" height="16" rx="4"/></clipPath><g clip-path="url(#rw)">')
    x = BX
    for _, col, b in segs:
        w = BW * b / left if left else 0
        out.append(f'<rect class="grow" x="{x:.2f}" y="28" width="{max(w - 1.5, 0.5):.2f}" height="16" fill="{col}"/>')
        x += w
    out.append("</g>")
    out.append(f'<text x="{BX}" y="62" class="mono" font-size="11.5" fill="{t["ink3"]}">'
               f'{num(left)} bytes left · game code {esc(pct(game_left, left))} of it</text>')
    out.append(f'<text x="{W - 28}" y="42" text-anchor="end" class="mono" font-size="20" font-weight="700" fill="{t["ink"]}">'
               f'{esc(pct(tot["code_matched"], tot["code"], 2))}</text>')
    wtxt = f'weighted {pct(tot["code_weighted"], tot["code"], 2)}' if tot.get("code_weighted") else f'{num(tot["fns_matched"])} fns'
    out.append(f'<text x="{W - 28}" y="62" text-anchor="end" class="mono" font-size="11.5" fill="{t["gold"]}">{esc(wtxt)}</text>')

    rows = [{"n": "Libraries", "col": HUE["gold"], "c": L["c"], "cm": L["cm"], "wm": L.get("wm", L["cm"]),
             "f": L["f"], "fm": L["fm"], "lib": True,
             "sub": f'4 regions · {kib(L["c"])}' + (f' · {L["matching"]} / {L["units"]} files Matching' if L.get("units") else ""),
             "line": " · ".join(f'{REGION[r["id"]][2]} {pct(r["cm"], r["c"])}' for r in lib_regions(summary))}]
    for p in parts:
        rows.append({"n": p["n"], "col": part_color(p), "c": p["c"], "cm": p["cm"], "wm": p.get("wm", p["cm"]),
                     "f": p["f"], "fm": p["fm"],
                     "sub": f'{len(p["files"])} files · {kib(p["c"])} · {sum(1 for f in p["files"] if f["s"] == "M")} Matching'
                            + (f' · {u} unconfirmed' if (u := sum(1 for f in p["files"] if f.get("u"))) else "")})
    ROW, TOP = 66, 100
    H = TOP + ROW * len(rows) + 30
    out.insert(0, f'<rect x="0.5" y="0.5" width="{W - 1}" height="{H - 1}" rx="14" fill="{t["panel"]}" stroke="{t["line"]}"/>')
    out.append(f'<line x1="20" x2="{W - 20}" y1="{TOP - 16}" y2="{TOP - 16}" stroke="{t["line"]}"/>')
    for i, r in enumerate(rows):
        y = TOP + i * ROW + (12 if i else 0)
        if i == 1:
            out.append(f'<text x="28" y="{y - 10}" class="mono" font-size="10.5" letter-spacing="0.14em" fill="{t["ink3"]}">'
                       f'GAME CODE · {esc(pct(sum(p["cm"] for p in parts), sum(p["c"] for p in parts)))}</text>')
        if r.get("lib"):
            out.append(glyph("jsystem", 28, y + 7, 13, HUE["gold"]))
        else:
            out.append(f'<g fill="{r["col"]}"><path d="M{34.5} {y + 7}L41 {y + 13.5}L34.5 {y + 20}L28 {y + 13.5}Z"/></g>')
        out.append(f'<text x="50" y="{y + 19}" font-size="16" font-weight="700" fill="{t["ink"]}">{esc(r["n"])}</text>')
        out.append(f'<text x="50" y="{y + 39}" font-size="12" fill="{t["ink3"]}">{esc(r["sub"])}</text>')
        fc = r["cm"] / r["c"] if r["c"] else 0
        fw = r["wm"] / r["c"] if r["c"] else 0
        out.append(f'<rect x="{BX}" y="{y + 6}" width="{BW}" height="14" rx="4" fill="{t["track"]}"/>')
        if fw > fc:
            out.append(f'<rect class="grow" style="animation-delay:{0.1 * i:.1f}s" x="{BX}" y="{y + 6}" '
                       f'width="{max(BW * fw, 3):.2f}" height="14" rx="4" fill="{r["col"]}" fill-opacity="0.35"/>')
        if fc > 0:
            out.append(f'<rect class="grow" style="animation-delay:{0.1 * i:.1f}s" x="{BX}" y="{y + 6}" '
                       f'width="{max(BW * fc, 3):.2f}" height="14" rx="4" fill="{r["col"]}"/>')
        wnote = f' · weighted {pct(r["wm"], r["c"])}' if fw > fc else ""
        out.append(f'<text x="{BX}" y="{y + 39}" class="mono" font-size="11.5" fill="{t["ink3"]}">'
                   f'{esc(r.get("line") or "honest " + pct(r["cm"], r["c"]) + wnote)}</text>')
        out.append(f'<text x="{W - 28}" y="{y + 20}" text-anchor="end" class="mono" font-size="20" font-weight="700" '
                   f'fill="{r["col"]}">{esc(pct(r["cm"], r["c"]))}</text>')
        out.append(f'<text x="{W - 28}" y="{y + 40}" text-anchor="end" class="mono" font-size="11.5" fill="{t["ink3"]}">'
                   f'{num(r["fm"])} / {num(r["f"])} fns</text>')
    out.append(f'<text x="28" y="{H - 14}" font-size="11.5" fill="{t["ink3"]}">solid = byte-identical (honest) · light = weighted, '
               f'with partial credit for structurally matched functions</text>')
    style = (".grow { transform-box: fill-box; transform-origin: 0 50%; animation: grow 1.4s cubic-bezier(.2,.8,.2,1) both; } "
             "@keyframes grow { from { transform: scaleX(0); } }")
    title = "Progress overview: " + "; ".join(f"{r['n']} {pct(r['cm'], r['c'])} of code, {r['fm']} of {r['f']} functions"
                                              for r in rows)
    return svg(W, H, "\n".join(out), title, style)


# ---------- treemap ----------

def squarify(items, x, y, w, h):
    """Squarified treemap (Bruls et al.). items: [(value, payload)] sorted descending.
    Returns [(payload, x, y, w, h)]."""
    items = [it for it in items if it[0] > 0]
    out = []
    total = sum(v for v, _ in items)
    if not items or w <= 0 or h <= 0:
        return out
    scale = w * h / total
    rest = [(v * scale, p) for v, p in items]

    def worst(row, side):
        s = sum(a for a, _ in row)
        mx, mn = max(a for a, _ in row), min(a for a, _ in row)
        return max(side * side * mx / (s * s), (s * s) / (side * side * mn))

    while rest:
        side = min(w, h)
        row = [rest.pop(0)]
        while rest and worst(row + [rest[0]], side) <= worst(row, side):
            row.append(rest.pop(0))
        s = sum(a for a, _ in row)
        if w >= h:  # lay the row out as a column on the left
            cw = s / h
            cy = y
            for a, p in row:
                out.append((p, x, cy, cw, a / cw))
                cy += a / cw
            x, w = x + cw, w - cw
        else:
            rh = s / w
            cx = x
            for a, p in row:
                out.append((p, cx, y, a / rh, rh))
                cx += a / rh
            y, h = y + rh, h - rh
    return out


# The README can't run script, and GitHub strips <map>/<area>, inline <svg>
# and style attributes, so an image can carry one link at most. The README
# treemap is therefore cut into one image per block, each inside its own <a>
# to that block on the site (with a title= tooltip): the top row is the game
# code's eight categories (their files squarified inside), the bottom row the four
# library regions (their libraries inside), every area still proportional to
# bytes of code. Within a row the images sit with no whitespace between them
# and their width= percentages add up to just under 100 %, so they never wrap.
# The tiles are drawn once, on their own dark card like the hero: GitHub's
# README renderer lifts the <img> out of a <picture> nested in an <a> (it
# links the image to its raw file instead), so light/dark <picture> variants
# would cost the links.

TM_W, TM_H = 960, 440


def treemap_blocks(summary):
    """[(row, [block])]: a block is {id, name, col, glyph, c, cm, f, fm, items, link}."""
    game, libs = [], []
    for p in game_parts(summary):
        game.append({"id": p["key"], "name": p["n"], "col": part_color(p), "glyph": None,
                     "c": p["c"], "cm": p["cm"], "f": p["f"], "fm": p["fm"],
                     "items": [{"n": f["n"].rsplit(".", 1)[0], "c": f["c"], "cm": f["cm"]} for f in p["files"]],
                     "link": f"game/{p['id']}"})
    for r in lib_regions(summary):
        libs.append({"id": r["id"], "name": r["n"], "col": REGION[r["id"]][0], "glyph": r["id"],
                     "c": r["c"], "cm": r["cm"], "f": r["f"], "fm": r["fm"],
                     "items": [{"n": l["n"], "c": l["c"], "cm": l["cm"]} for l in r["libs"]], "link": r["id"]})
    if not game:  # an older summary.json without game categories: the game region as one block
        g = next(r for r in summary["regions"] if r["id"] == "game")
        game = [{"id": "game", "name": g["n"], "col": REGION["game"][0], "glyph": "game", "c": g["c"], "cm": g["cm"],
                 "f": g["f"], "fm": g["fm"], "items": [{"n": l["n"], "c": l["c"], "cm": l["cm"]} for l in g["libs"]],
                 "link": "game"}]
    return [row for row in (game, sorted(libs, key=lambda b: -b["c"])) if row]


def treemap_rows(summary):
    """[(row height, [(block, width percent)])] for the README treemap."""
    rows_in = treemap_blocks(summary)
    total = sum(b["c"] for row in rows_in for b in row)
    rows = []
    for row in rows_in:
        s = sum(b["c"] for b in row)
        if not s:
            continue
        h = round(TM_H * s / total)
        if len(row) == 1:
            rows.append((h, [(row[0], 100.0)]))
            continue
        ws = [math.floor(10000 * b["c"] / s) / 100 for b in row]
        over = sum(ws) - 99.8  # a hair of slack so subpixel rounding can't wrap the row
        if over > 0:
            ws[ws.index(max(ws))] = round(max(ws) - over, 2)
        rows.append((h, list(zip(row, ws))))
    return rows


def treemap_tile(blk, w, h, theme="dark"):
    """One block's tile: its files or libraries squarified, each filled to its matched share."""
    t = THEMES[theme]
    col = blk["col"]
    M = 3  # margin: neighbouring tiles read as separate cards
    out = [f'<rect x="{M}" y="{M}" width="{w - 2 * M:.1f}" height="{h - 2 * M}" rx="8" fill="{t["panel"]}"/>']
    labels = []
    items = sorted(blk["items"], key=lambda l: -l["c"])
    for it, x, y, lw, lh in squarify([(l["c"], l) for l in items], M + 3, M + 3, w - 2 * M - 6, h - 2 * M - 6):
        g = 1.0 if lw > 3 and lh > 3 else 0.4
        x, y, lw, lh = x + g / 2, y + g / 2, max(lw - g, 0.3), max(lh - g, 0.3)
        frac = it["cm"] / it["c"] if it["c"] else 0
        out.append(f'<rect x="{x:.1f}" y="{y:.1f}" width="{lw:.1f}" height="{lh:.1f}" rx="{1.5 if lw > 6 else 0}" fill="{col}" '
                   f'fill-opacity="0.13"/>')
        if frac > 0:
            fh = lh * frac
            out.append(f'<rect class="rise" x="{x:.1f}" y="{y + lh - fh:.1f}" width="{lw:.1f}" height="{fh:.1f}" '
                       f'fill="{col}" fill-opacity="0.85"/>')
        if lw > 64 and lh > 30:
            name = it["n"]
            maxc = int((lw - 12) / 7)
            if len(name) > maxc:
                name = name[:max(maxc - 1, 1)] + "…"
            labels.append(f'<text x="{x + 6:.1f}" y="{y + 16:.1f}" font-size="11.5" font-weight="600" fill="{t["ink"]}">{esc(name)}</text>')
            labels.append(f'<text x="{x + 6:.1f}" y="{y + 29:.1f}" class="mono" font-size="10" fill="{t["ink"]}">'
                          f'{esc(pct(it["cm"], it["c"]))}</text>')
    out.append(f'<rect x="{M + 0.75}" y="{M + 0.75}" width="{w - 2 * M - 1.5:.1f}" height="{h - 2 * M - 1.5}" rx="7.5" '
               f'fill="none" stroke="{col}" stroke-opacity="0.8" stroke-width="1.5"/>')
    out += labels
    # the block's badge in the bottom-right corner, as much of it as fits
    full = f'{blk["name"]} {pct(blk["cm"], blk["c"])}'
    for text in (full, pct(blk["cm"], blk["c"]), ""):
        bw = 30 + 7.4 * len(text) if text else 22
        if bw <= w - 2 * M - 8 or not text:
            break
    if bw <= w - 2 * M - 4 and h >= 40:
        bx, by = w - M - 5 - bw, h - M - 5 - 22
        out.append(f'<rect x="{bx:.1f}" y="{by}" width="{bw:.1f}" height="22" rx="11" fill="{t["bg"]}" '
                   f'fill-opacity="0.88" stroke="{col}" stroke-opacity="0.7"/>')
        if blk["glyph"]:
            out.append(glyph(blk["glyph"], bx + 6, by + 5, 12, col))
        else:
            out.append(f'<path d="M{bx + 12:.1f} {by + 5}L{bx + 18:.1f} {by + 11}L{bx + 12:.1f} {by + 17}L{bx + 6:.1f} {by + 11}Z" fill="{col}"/>')
        if text:
            out.append(f'<text x="{bx + 24:.1f}" y="{by + 15.5}" font-size="12" font-weight="700" fill="{t["ink"]}">{esc(text)}</text>')
    style = (f"text {{ paint-order: stroke; stroke: {t['panel']}; stroke-opacity: 0.55; stroke-width: 2.5px; stroke-linejoin: round; }} "
             ".rise { transform-box: fill-box; transform-origin: 50% 100%; animation: rise 1.6s cubic-bezier(.2,.8,.2,1) .2s both; } "
             "@keyframes rise { from { transform: scaleY(0); } }")
    title = (f"{blk['name']}: {pct(blk['cm'], blk['c'])} of {num(blk['c'])} bytes of code matched, "
             f"{num(blk['fm'])} of {num(blk['f'])} functions")
    return svg(f"{w:g}", h, "\n".join(out), title, style)


def treemap_legend(summary, theme="dark"):
    """The README treemap's legend: the game categories in their order, then the
    libraries, wrapped onto as many lines as the width needs."""
    t = THEMES[theme]
    W = TM_W
    entries = [(PART_LEGEND.get(p["key"], p["n"]), part_color(p), p["cm"], p["c"], None) for p in game_parts(summary)]
    L = libraries(summary)
    entries.append(("Libraries", HUE["gold"], L["cm"], L["c"], "jsystem"))
    items, x, y = [], 16, 11
    for name, col, cm, c, gl in entries:
        label = f'{name} {pct(cm, c)}'
        wid = 17 + 6.6 * len(label) + 18
        if x + wid > W - 16 and x > 16:
            x, y = 16, y + 20
        if gl:
            items.append(glyph(gl, x, y, 11, col))
        else:
            items.append(f'<path d="M{x + 5.5:g} {y:g}L{x + 11:g} {y + 5.5:g}L{x + 5.5:g} {y + 11:g}L{x:g} {y + 5.5:g}Z" fill="{col}"/>')
        items.append(f'<text x="{x + 17:g}" y="{y + 10:g}" font-size="12.5" fill="{t["ink2"]}">{esc(label)}</text>')
        x += wid
    H = y + 37
    out = [f'<rect x="3.5" y="3.5" width="{W - 7}" height="{H - 7}" rx="8" fill="{t["panel"]}" stroke="{t["line"]}"/>'] + items
    out.append(f'<text x="16" y="{H - 11}" font-size="11.5" fill="{t["ink3"]}">'
               f'area = bytes of code · fill = byte-identical · top row the game code by category, bottom row the libraries · '
               f'click a block to explore it on the site ↗</text>')
    return svg(W, H, "\n".join(out), "Treemap legend: the game code's categories and the libraries, and their matched share of code")


def treemap_files(summary):
    """{path under site/badges: svg text} for every README treemap image."""
    files = {}
    for h, row in treemap_rows(summary):
        for blk, wp in row:
            files[f"treemap/{blk['id']}.svg"] = treemap_tile(blk, round(TM_W * wp / 100, 1), h)
    files["treemap/legend.svg"] = treemap_legend(summary)
    return files


def tile_img(name, width, alt):
    return f'<img src="site/badges/treemap/{name}.svg" width="{width:g}%" alt="{esc(alt)}">'


def treemap_block(summary):
    """The README treemap: one linked image per block (one line per row, no
    whitespace between images), then the legend linking to the site's map."""
    lines = []
    for _, row in treemap_rows(summary):
        cells = []
        for blk, wp in row:
            tip = (f"{blk['name']}: {pct(blk['cm'], blk['c'])} of {num(blk['c'])} bytes matched, "
                   f"{num(blk['fm'])} / {num(blk['f'])} functions. Open it on the progress site.")
            alt = f"{blk['name']}: {pct(blk['cm'], blk['c'])} matched"
            cells.append(f'<a href="{SITE}#/{blk["link"]}" title="{esc(tip)}">{tile_img(blk["id"], wp, alt)}</a>')
        lines.append("".join(cells))
    lines.append(f'<a href="{SITE}#treemap" title="Open the interactive treemap">'
                 f'{tile_img("legend", 100, "area = bytes of code, fill = matched")}</a>')
    return '<p align="center">\n' + "<br>\n".join(lines) + "\n</p>"


# ---------- roadmap ----------

def phase_frac(ph):
    """site/app.js phaseFrac()."""
    if ph["status"] == "done":
        return 1.0
    ms = ph["milestones"]
    acc = 0.0
    for m in ms:
        if m["status"] == "done":
            acc += 1
        elif m["status"] == "active" and m.get("measure") and m["measure"]["total"]:
            acc += m["measure"]["done"] / m["measure"]["total"]
    return acc / (len(ms) or 1)


def wrap(text, width):
    lines, cur = [], ""
    for word in text.split():
        if cur and len(cur) + 1 + len(word) > width:
            lines.append(cur)
            cur = word
        else:
            cur = f"{cur} {word}".strip()
    return lines + ([cur] if cur else [])


def roadmap_svg(rm, theme):
    t = THEMES[theme]
    phases = rm["phases"]
    W, H = 960, 230
    n = len(phases)
    step = (W - 80) / n
    cy = 62
    words = {"done": "done", "active": "under way", "next": "next", "later": "later"}
    out = [f'<rect x="0.5" y="0.5" width="{W - 1}" height="{H - 1}" rx="14" fill="{t["panel"]}" stroke="{t["line"]}"/>']
    xs = [40 + step * (i + 0.5) for i in range(n)]
    for i in range(n - 1):
        solid = phases[i]["status"] == "done" and phases[i + 1]["status"] in ("done", "active")
        out.append(f'<line x1="{xs[i] + 22:.1f}" x2="{xs[i + 1] - 22:.1f}" y1="{cy}" y2="{cy}" stroke="{t["line2"]}" '
                   f'stroke-width="2"{"" if solid else " stroke-dasharray=\"3 5\""}/>')
    defs = []
    for i, ph in enumerate(phases):
        x, col, st = xs[i], HUE.get(ph.get("hue"), HUE["gold"]), ph["status"]
        w, h = 18, 26
        f = phase_frac(ph) if st in ("done", "active") else 0
        if st == "active":
            f = max(f, 0.12)
            out.append(f'<path class="pulse" d="{rhombus(x, cy, w + 7, h + 9)}" fill="{col}" fill-opacity="0.18"/>')
        defs.append(f'<clipPath id="rc{i}"><path d="{rhombus(x, cy, w, h)}"/></clipPath>')
        out.append(f'<path d="{rhombus(x, cy, w, h)}" fill="{col}" fill-opacity="{0.1 if st != "later" else 0.04}" '
                   f'stroke="{col}" stroke-width="1.6"{" stroke-dasharray=\"3 3\"" if st == "later" else ""}'
                   f'{" stroke-opacity=\"0.6\"" if st in ("next", "later") else ""}/>')
        if f > 0:
            fh = rhombus_fill(f, h)
            out.append(f'<g clip-path="url(#rc{i})"><rect class="rise" style="animation-delay:{0.12 * i:.2f}s" '
                       f'x="{x - w}" y="{cy + h - fh:.2f}" width="{2 * w}" height="{fh:.2f}" fill="{col}"/></g>')
            out.append(f'<path d="M{x} {cy - h}L{x + w} {cy}L{x} {cy}Z" fill="#fff" fill-opacity="0.16"/>')
        out.append(f'<text x="{x:.1f}" y="{cy + h + 26}" text-anchor="middle" class="mono" font-size="10.5" '
                   f'letter-spacing="0.12em" fill="{t["ink3"]}">PHASE {i + 1}</text>')
        for j, line in enumerate(wrap(ph["title"], max(int(step / 7.2), 10))[:3]):
            out.append(f'<text x="{x:.1f}" y="{cy + h + 46 + 16 * j}" text-anchor="middle" font-size="12.5" '
                       f'font-weight="600" fill="{t["ink"] if st in ("done", "active") else t["ink2"]}">{esc(line)}</text>')
        done = sum(1 for m in ph["milestones"] if m["status"] == "done")
        tag = f'{words[st]} · {done}/{len(ph["milestones"])}'
        out.append(f'<text x="{x:.1f}" y="{H - 16}" text-anchor="middle" class="mono" font-size="11" '
                   f'fill="{col if st in ("done", "active") else t["ink3"]}">{esc(tag)}</text>')
    style = (".rise { transform-box: fill-box; transform-origin: 50% 100%; animation: rise 1.4s cubic-bezier(.2,.8,.2,1) both; } "
             "@keyframes rise { from { transform: scaleY(0); } } "
             ".pulse { animation: pulse 2.8s ease-in-out infinite; } "
             "@keyframes pulse { 0%, 100% { opacity: 0.35; } 50% { opacity: 1; } }")
    title = "Roadmap: " + "; ".join(f"phase {i + 1}, {ph['title']}: {words[ph['status']]}" for i, ph in enumerate(phases))
    return svg(W, H, "<defs>" + "".join(defs) + "</defs>\n" + "\n".join(out), title, style)


# ---------- badges ----------

def badges(summary):
    tot = summary["totals"]
    sha = tot["sha1_ok"]
    items = {
        "code": ("code", pct(tot["code_matched"], tot["code"], 2), "d29a2e"),
        "functions": ("functions", f"{num(tot['fns_matched'])} / {num(tot['fns'])}", "3d9cf0"),
        "units": ("units Matching", f"{num(tot['matching'])} / {num(tot['units'])}", "c35ee6"),
        "sha1": ("main.dol SHA-1", "OK" if sha else "not OK" if sha is not None else "not checked",
                 "3aae5c" if sha else "ef5b52"),
    }
    import json
    for key, (label, message, color) in items.items():
        write_if_changed(DATA / f"badge-{key}.json", json.dumps(
            {"schemaVersion": 1, "label": label, "message": message, "color": color, "labelColor": "161a34"}) + "\n")


# ---------- README markers ----------

def md(text):
    return re.sub(r"([\\*_\[\]|])", r"\\\1", text)  # names like enemy_*_sob are not emphasis


def readme_blocks(summary, rm):
    tot = summary["totals"]
    day = summary["generated"][:10]
    sha = {True: "links byte-identically (SHA-1 OK)", False: "does **not** link byte-identically",
           None: "was not linked for this run"}[tot["sha1_ok"]]
    progress = [f"As of {day}:", "",
                f"- **Honest: {pct(tot['code_matched'], tot['code'], 2)}** of the code is byte-identical, "
                f"**{num(tot['fns_matched'])} / {num(tot['fns'])}** functions "
                f"({num(tot['code_matched'])} of {num(tot['code'])} bytes; 100 % functions only)."]
    if tot.get("code_weighted"):
        progress.append(f"- **Weighted: {pct(tot['code_weighted'], tot['code'], 2)}**, the honest bytes plus partial "
                        f"credit for structurally matched functions (a separate, secondary measure).")
    if tot.get("fns_structural"):
        progress.append(f"- **Structurally matched:** {num(tot['fns_structural'])} / {num(tot['fns'])} functions "
                        f"(same instructions, register or order differences only), "
                        f"+{num(tot['fns_structural_close'])} close.")
    progress.append(f"- **{num(tot['matching'])} of {num(tot['units'])}** split units are Matching, and `main.dol` "
                    f"built from them {sha}.")

    # libraries: one combined line, then a row per library region, each with its libraries folded away
    L = libraries(summary)
    lib = [f"**{pct(L['cm'], L['c'])}** of {num(L['c'])} bytes of library code and "
           f"**{num(L['fm'])} / {num(L['f'])}** functions match"
           + (f" (weighted {pct(L['wm'], L['c'])})" if L.get("wm", 0) > L["cm"] else "") + ".", "",
           "| Library | Code matched | Functions | Left |", "|---|--:|--:|--:|"]
    for r in lib_regions(summary):
        lib.append(f"| [{r['n']}]({SITE}#/{r['id']}) | {pct(r['cm'], r['c'])} of {num(r['c'])} bytes "
                   f"| {num(r['fm'])} / {num(r['f'])} | {num(r['f'] - r['fm']) if r['f'] > r['fm'] else '–'} |")
    lib += ["", "<details><summary>Every library</summary>"]
    for r in lib_regions(summary):
        n = len(r["libs"])
        if n < 2:
            continue
        lib += ["", f"<details><summary>{esc(r['n'])}: {n} libraries</summary>", "",
                "| Library | Code matched | Functions |", "|---|--:|--:|"]
        for l in sorted(r["libs"], key=lambda l: -l["c"]):
            lib.append(f"| [{md(l['n'])}]({SITE}#/{r['id']}/{l['id']}) | {pct(l['cm'], l['c'])} of {num(l['c'])} bytes "
                       f"| {num(l['fm'])} / {num(l['f'])} |")
        lib += ["", "</details>"]
    lib += ["", "</details>"]

    # game code: a row per category
    parts = game_parts(summary)
    g = next(r for r in summary["regions"] if r["id"] == "game")
    game = [f"**{pct(g['cm'], g['c'])}** of {num(g['c'])} bytes of game code and **{num(g['fm'])} / {num(g['f'])}** "
            f"functions match" + (f" (weighted {pct(g['wm'], g['c'])})" if g.get("wm", 0) > g["cm"] else "") + ".", "",
            "| Category | Code matched | Weighted | Functions | Files Matching | Unconfirmed |", "|---|--:|--:|--:|--:|--:|"]
    for p in parts:
        m = sum(1 for f in p["files"] if f["s"] == "M")
        u = sum(1 for f in p["files"] if f.get("u"))
        game.append(f"| [{md(p['n'])}]({SITE}#/game/{p['id']}) | {pct(p['cm'], p['c'])} of {num(p['c'])} bytes "
                    f"| {pct(p.get('wm', p['cm']), p['c'])} | {num(p['fm'])} / {num(p['f'])} | {m} / {len(p['files'])} "
                    f"| {u or '–'} |")
    game.append("")
    for p in parts:
        game.append(f"- **{md(p['n'])}:** {md(p['desc'])}.")
    unconfirmed = sum(1 for p in parts for f in p["files"] if f.get("u"))
    game += ["", f"Each game file's category, its evidence and its confidence are in "
             f"[docs/names/categories.tsv](docs/names/categories.tsv) (method: "
             f"[docs/names/METHOD.md](docs/names/METHOD.md#categories)); the {unconfirmed} files placed on thin "
             f"evidence (low confidence) are marked unconfirmed. The progress site can also group the game code "
             f"by boot-path phase (" + ", ".join(md(b["n"]) for b in summary.get("bootpath", [])) + ", from "
             f"[docs/BOOT_PATH.md](docs/BOOT_PATH.md)) with its *Group by* switch."]

    now = []
    focus = [ph for ph in rm["phases"] if ph.get("focus")]
    for status, word in (("active", "Now"), ("next", "Next")):
        for i, ph in enumerate(rm["phases"]):
            if status == "next" and focus and ph not in focus:
                continue  # "Next" lists the focus phase's upcoming work only
            for m in ph["milestones"]:
                if m["status"] == status:
                    desc = m["desc"].rstrip()
                    desc += "" if desc.endswith(".") else "."
                    if m.get("measure") and m["measure"]["total"]:
                        desc += f" Currently {pct(m['measure']['done'], m['measure']['total'])} {m['measure']['label']}."
                    now.append(f"- **{word}** (phase {i + 1}): **{m['title']}.** {desc}")
    return {"progress": "\n".join(progress), "treemap": treemap_block(summary), "libraries": "\n".join(lib),
            "game": "\n".join(game), "now": "\n".join(now)}


def status_block(summary):
    """CLAUDE.md's status paragraph, from the same honest-report totals."""
    tot = summary["totals"]
    day = summary["generated"][:10]
    sha = {True: "`ninja` ends in SHA-1 OK", False: "`ninja` does **not** end in SHA-1 OK",
           None: "`main.dol` was not linked for this run"}[tot["sha1_ok"]]
    parts = ", ".join(f"{p['n']} {pct(p['cm'], p['c'])}" for p in game_parts(summary))
    regions = "; ".join(f"{r['n']} {num(r['fm'])} / {num(r['f'])} functions ({pct(r['cm'], r['c'])}"
                        + (f"; {parts}" if r["id"] == "game" and parts else "") + ")"
                        for r in summary["regions"])
    weighted = ""
    if tot.get("code_weighted"):
        weighted = (f"; weighted {pct(tot['code_weighted'], tot['code'], 2)} ({num(tot['code_weighted'])} bytes, honest "
                    f"plus partial credit for {num(tot['fns_structural'] - tot['fns_matched'] + tot['fns_structural_close'])} "
                    f"structurally matched functions)")
    return (f"Status ({day}, honest report, written by `tools/site_data.py`): "
            f"{num(tot['fns_matched'])} of {num(tot['fns'])} functions, {num(tot['code_matched'])} of "
            f"{num(tot['code'])} bytes of code ({pct(tot['code_matched'], tot['code'], 2)}){weighted}. {regions}. "
            f"{num(tot['matching'])} of {num(tot['units'])} split units are Matching; {sha}.")


def update_markers(path, blocks):
    if not path.exists():
        return
    text = path.read_text()
    new = text
    for name, body in blocks.items():
        new = re.sub(rf"(<!-- {name}:start -->).*?(<!-- {name}:end -->)",
                     lambda m: f"{m.group(1)}\n{body}\n{m.group(2)}", new, flags=re.S)
    if new != text:
        path.write_text(new)


def update_readme(blocks):
    update_markers(README, blocks)


# ---------- entry ----------

OUTPUTS = ["site/badges", "site/data", "README.md", "CLAUDE.md"]


def write_all(summary, rm):
    write_if_changed(BADGES / "hero.svg", hero(summary))
    tm = treemap_files(summary)
    for name, text in tm.items():
        write_if_changed(BADGES / name, text)
    for old in (BADGES / "treemap").glob("*.svg"):  # blocks that no longer exist
        if f"treemap/{old.name}" not in tm:
            old.unlink()
    for theme in THEMES:
        write_if_changed(BADGES / f"regions-{theme}.svg", regions_svg(summary, theme))
        write_if_changed(BADGES / f"roadmap-{theme}.svg", roadmap_svg(rm, theme))
    badges(summary)
    update_readme(readme_blocks(summary, rm))
    update_markers(CLAUDE_MD, {"status": status_block(summary)})
