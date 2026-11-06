/* FSA decomp progress site. Data: data/summary.json, data/lib/<id>.json, data/history.json,
   data/roadmap.json (written by tools/site_data.py from the honest objdiff report and
   docs/roadmap.yml). */
"use strict";

// Colours: the game code's eight categories each have one (HUE, from summary.json's
// game libraries; a categorical palette checked for colour-vision deficiency), the
// boot-path view's four parts take the four Links' colours; the libraries are one
// gold group, a shade per region.
const REGION = {
  game:    { color: "#a9afe0", glyph: "diamond", short: "Game" },
  jsystem: { color: "#d29a2e", glyph: "triangle", short: "JSystem" },
  sdk:     { color: "#e9be5a", glyph: "circle", short: "SDK" },
  gx:      { color: "#c4862b", glyph: "square", short: "GX" },
  runtime: { color: "#f2d384", glyph: "star", short: "MSL/RT/TRK" },
};
const LIB_REGIONS = ["sdk", "gx", "jsystem", "runtime"];
const PART = {}; // game category or boot-path part key -> { color, name, desc, short }
const PART_SHORT = { player: "Player", enemies: "Enemies", npcs: "NPCs", objects: "Objects", stage: "Stage", menus: "Menus",
  core: "Core", media: "Media", boot: "Boot", battle: "Battle", actors: "Actors", other: "Other" };
// How the game code is grouped: "cat" (the eight categories, the default) or "boot"
// (the boot-path phases of docs/BOOT_PATH.md, the old four parts).
const GROUPS = { cat: { word: "Category", libs: [] }, boot: { word: "Boot path", libs: [] } };
let GROUP = "cat";
try { if (localStorage.getItem("fsa-group") === "boot") GROUP = "boot"; } catch (e) { /* storage blocked */ }
const REPO = "https://github.com/sh1ftmaker/fsa";
const COARSE = window.matchMedia("(pointer: coarse)").matches;
const REDUCED = window.matchMedia("(prefers-reduced-motion: reduce)").matches;

const $ = (s) => document.querySelector(s);
const esc = (s) => String(s).replace(/[&<>"]/g, (c) => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;" }[c]));
const nf = new Intl.NumberFormat("en-US");
const num = (n) => nf.format(n);
function bytes(b) {
  if (b < 1024) return `${b} B`;
  if (b < 1048576) return `${(b / 1024).toFixed(b < 10240 ? 2 : 1)} KiB`;
  return `${(b / 1048576).toFixed(2)} MiB`;
}
function pct(x) {
  if (!isFinite(x)) return "–";
  if (x === 0) return "0 %";
  if (x >= 100) return "100 %";
  return `${x < 10 ? x.toFixed(2) : x.toFixed(1)} %`;
}
const ratio = (a, b) => (b ? (100 * a) / b : 0);
const hex = (a) => "0x" + a.toString(16).toUpperCase().padStart(8, "0");

// a node's colour: its game part's, else its region's
const colOf = (n) => (n.part && PART[n.part] ? PART[n.part].color : REGION[n.region].color);
const gradOf = (n) => (n.part && PART[n.part] ? `tg-p-${n.part}` : `tg-${n.region}`);
function partGlyph(key, size = 10) {
  const s = size, h = s / 2;
  return `<svg class="glyph" width="${s}" height="${s}" viewBox="0 0 ${s} ${s}" fill="${PART[key].color}" aria-hidden="true"><path d="M${h} 0L${s} ${h}L${h} ${s}L0 ${h}Z"/></svg>`;
}
const nodeGlyph = (n, size = 10) => (n.part && PART[n.part] ? partGlyph(n.part, size) : glyph(n.region, size));
function glyph(region, size = 10) {
  const c = REGION[region].color, s = size, h = s / 2;
  const shapes = {
    diamond: `<path d="M${h} 0L${s} ${h}L${h} ${s}L0 ${h}Z"/>`,
    triangle: `<path d="M${h} 0L${s} ${s}L0 ${s}Z"/>`,
    circle: `<circle cx="${h}" cy="${h}" r="${h}"/>`,
    square: `<rect width="${s}" height="${s}" rx="1.5"/>`,
    star: `<path d="M${h} 0L${s * 0.62} ${s * 0.38}L${s} ${h}L${s * 0.62} ${s * 0.62}L${h} ${s}L${s * 0.38} ${s * 0.62}L0 ${h}L${s * 0.38} ${s * 0.38}Z"/>`,
  };
  return `<svg class="glyph" width="${s}" height="${s}" viewBox="0 0 ${s} ${s}" fill="${c}" aria-hidden="true">${shapes[REGION[region].glyph]}</svg>`;
}

/* ---------- data model ---------- */

let S, TOTAL, ROOT;
const byId = new Map();

function buildTree(summary) {
  const t = summary.totals;
  ROOT = { id: "", kind: "root", name: "main.dol", c: t.code, cm: t.code_matched, f: t.fns, fm: t.fns_matched, children: [] };
  byId.set("", ROOT);
  for (const r of summary.regions) {
    const rn = { id: r.id, kind: "region", region: r.id, name: r.n, desc: r.desc, c: r.c, cm: r.cm, wm: r.wm, f: r.f, fm: r.fm, parent: ROOT, children: [] };
    ROOT.children.push(rn);
    byId.set(rn.id, rn);
    const fileNode = (ln, f, i, src, srcIdx) => {
      const fnode = { id: `${ln.id}/${i}`, idx: i, kind: "file", region: r.id, part: ln.part, name: f.n, path: f.p, addr: f.a, s: f.s,
        c: f.c, cm: f.cm, f: f.f, fm: f.fm, d: f.d || 0, dm: f.dm || 0, t: f.t || 0, u: !!f.u, bp: f.bp, src, srcIdx, parent: ln, children: null };
      ln.children.push(fnode);
      byId.set(fnode.id, fnode);
      return fnode;
    };
    for (const l of r.libs) {
      if (l.key) PART[l.key] = { color: HUE[l.hue] || HUE.gold, name: l.n, desc: l.desc, short: PART_SHORT[l.key] || l.n };
      const ln = { id: `${r.id}/${l.id}`, lib: l.id, kind: "lib", region: r.id, part: l.key, group: l.key ? "cat" : null, desc: l.desc, name: l.n, c: l.c, cm: l.cm, wm: l.wm, f: l.f, fm: l.fm, parent: rn, children: [], loaded: false };
      rn.children.push(ln);
      byId.set(ln.id, ln);
      l.files.forEach((f, i) => fileNode(ln, f, i, l.id, i));
    }
    if (r.id !== "game") continue;
    GROUPS.cat.libs = rn.children.slice();
    // the boot-path view: the same files regrouped by their "bp" part (no number changes;
    // weighted progress is only known per category, so these groups show honest numbers)
    for (const b of summary.bootpath || []) {
      PART[b.key] = { color: HUE[b.hue] || HUE.gold, name: b.n, desc: b.desc, short: PART_SHORT[b.key] || b.n };
      const ln = { id: `game/game-${b.key}`, lib: `game-${b.key}`, kind: "lib", region: "game", part: b.key, group: "boot", desc: b.desc, name: b.n,
        c: 0, cm: 0, f: 0, fm: 0, parent: rn, children: [], loaded: false };
      const files = GROUPS.cat.libs.flatMap((cl) => cl.children.map((fn) => [cl, fn])).filter(([, fn]) => fn.bp === b.key)
        .sort((a, b2) => (a[1].addr || 0) - (b2[1].addr || 0));
      files.forEach(([cl, fn], i) => {
        const x = fileNode(ln, { n: fn.name, p: fn.path, a: fn.addr, s: fn.s, c: fn.c, cm: fn.cm, f: fn.f, fm: fn.fm, d: fn.d, dm: fn.dm, t: fn.t, u: fn.u, bp: fn.bp }, i, cl.lib, fn.srcIdx);
        x.cat = cl.part;
        for (const k of ["c", "cm", "f", "fm"]) ln[k] += fn[k];
      });
      byId.set(ln.id, ln);
      GROUPS.boot.libs.push(ln);
    }
    for (const cl of GROUPS.cat.libs) for (const fn of cl.children) fn.cat = cl.part;
    if (!GROUPS.boot.libs.length) GROUP = "cat";
    rn.children = GROUPS[GROUP].libs;
  }
}

// The game region's groups in the current view.
const gameGroups = () => GROUPS[GROUP].libs;

const libCache = new Map();
async function ensureLoaded(lib) {
  if (lib.loaded) return;
  const load = (id) => {
    if (!libCache.has(id)) libCache.set(id, fetch(`data/lib/${id}.json`).then((r) => r.json()));
    return libCache.get(id);
  };
  // a boot-path group's files come from several categories' files
  const srcs = [...new Set(lib.children.map((f) => f.src || lib.lib))];
  const data = new Map(await Promise.all(srcs.map(async (id) => [id, await load(id)])));
  for (const file of lib.children) {
    const d = data.get(file.src || lib.lib), k = String(file.srcIdx ?? file.idx);
    const fns = d.files[k] || [];
    if (d.ev && d.ev[k]) [file.conf, file.ev] = d.ev[k];
    file.children = fns.map(([name, addr, size, m], j) => ({
      id: `${file.id}/${j}`, kind: "fn", region: file.region, part: file.part, name, addr, c: size, cm: m === 100 ? size : 0,
      m, f: 1, fm: m === 100 ? 1 : 0, parent: file,
    }));
  }
  lib.loaded = true;
}

const libOf = (n) => { while (n && n.kind !== "lib") n = n.parent; return n; };

/* ---------- emblem ---------- */

function rhombusPath(cx, cy, w, h) {
  return `M${cx} ${cy - h}L${cx + w} ${cy}L${cx} ${cy + h}L${cx - w} ${cy}Z`;
}
// Height of a bottom-up fill inside a rhombus so that the filled AREA is the fraction a.
function rhombusFill(a, h) {
  a = Math.max(0, Math.min(1, a));
  return a <= 0.5 ? h * Math.sqrt(2 * a) : 2 * h - h * Math.sqrt(2 * (1 - a));
}

// The emblem: the game code's eight categories as gems on a ring (clockwise from the
// top, in their order), each filled to its matched share; the gold gem in the middle
// is the libraries, all four together. Same layout as tools/readme_assets.py emblem_layout.
function drawEmblem() {
  const svg = d3.select("#emblem");
  const L = S.libraries;
  const cats = GROUPS.cat.libs.map((l) => l.part);
  const gems = cats.map((part, i) => {
    const a = (2 * Math.PI * i) / cats.length - Math.PI / 2, lx = 96 * Math.cos(a);
    return { part, cx: 60 * Math.cos(a), cy: 60 * Math.sin(a), w: 15, h: 21, lx, ly: 96 * Math.sin(a) + 3,
      anchor: Math.abs(lx) < 20 ? "middle" : lx > 0 ? "start" : "end" };
  });
  if (L) gems.push({ lib: true, cx: 0, cy: 0, w: 20, h: 20 });
  const defs = svg.append("defs");
  svg.append("circle").attr("class", "ring spin").attr("r", 148);
  svg.append("circle").attr("class", "ring").attr("r", 84).attr("stroke-dasharray", "1 10");
  for (const g of gems) {
    const key = g.lib ? "libs" : g.part;
    const node = g.lib ? null : byId.get(`game/game-${g.part}`);
    const r = g.lib ? { name: "Libraries", c: L.c, cm: L.cm } : node;
    const col = g.lib ? "#d29a2e" : PART[g.part].color;
    const frac = r.c ? r.cm / r.c : 0;
    const grad = defs.append("linearGradient").attr("id", `eg-${key}`).attr("x1", 0).attr("y1", 1).attr("x2", 0).attr("y2", 0);
    grad.append("stop").attr("offset", 0).attr("stop-color", col);
    grad.append("stop").attr("offset", 1).attr("stop-color", d3.color(col).brighter(0.9));
    defs.append("clipPath").attr("id", `ec-${key}`).append("path").attr("d", rhombusPath(g.cx, g.cy, g.w, g.h));
    const grp = svg.append("g").attr("class", "gem").attr("tabindex", 0).attr("role", "button")
      .attr("aria-label", `${r.name}: ${pct(100 * frac)} of code matched`);
    grp.append("path").attr("class", "gem-bg").attr("d", rhombusPath(g.cx, g.cy, g.w, g.h))
      .attr("fill", d3.color(col).copy({ opacity: 0.12 })).attr("stroke", d3.color(col).copy({ opacity: 0.7 }));
    const fillH = rhombusFill(frac, g.h);
    grp.append("rect").attr("clip-path", `url(#ec-${key})`).attr("x", g.cx - g.w).attr("width", 2 * g.w)
      .attr("y", g.cy + g.h).attr("height", 0).attr("fill", `url(#eg-${key})`)
      .transition().delay(300).duration(REDUCED ? 0 : 1600).ease(d3.easeCubicOut)
      .attr("y", g.cy + g.h - fillH).attr("height", fillH);
    grp.append("path").attr("d", `M${g.cx} ${g.cy - g.h}L${g.cx + g.w} ${g.cy}L${g.cx} ${g.cy}Z`).attr("fill", "rgba(255,255,255,0.06)").attr("pointer-events", "none");
    if (!g.lib) {
      grp.append("text").attr("class", "gem-name").attr("x", g.lx).attr("y", g.ly - 5).attr("text-anchor", g.anchor).text(PART[g.part].short);
      grp.append("text").attr("class", "gem-label").attr("x", g.lx).attr("y", g.ly + 8).attr("text-anchor", g.anchor).text(pct(100 * frac));
    }
    grp.on("click keydown", (ev) => {
      if (ev.type === "keydown" && ev.key !== "Enter" && ev.key !== " ") return;
      if (g.lib) { $("#libs-panel").scrollIntoView({ behavior: REDUCED ? "auto" : "smooth" }); return; }
      setGroup("cat", false); go(node.id); $("#treemap").scrollIntoView({ behavior: REDUCED ? "auto" : "smooth" });
    });
    grp.on("pointermove", (ev) => {
      if (COARSE) return;
      if (node) { showTip(node, ev); return; }
      tip.classList.remove("sheet");
      tip.innerHTML = `<div class="t-name">Libraries</div><div class="t-path">Dolphin SDK, GX, JSystem, MSL/Runtime/TRK</div>
        <div class="bar"><i style="width:${ratio(L.cm, L.c)}%;background:${col}"></i></div>
        <dl><dt>Matched</dt><dd>${pct(ratio(L.cm, L.c))} of ${bytes(L.c)}</dd><dt>Functions</dt><dd>${num(L.fm)} / ${num(L.f)}</dd></dl>`;
      tip.hidden = false; place(ev);
    }).on("pointerleave", hideTip);
  }
}

/* ---------- headline ---------- */

function countUp(el, to, fmt) {
  if (REDUCED) { el.textContent = fmt(to); return; }
  const t0 = performance.now(), dur = 1300;
  const step = (t) => {
    const k = Math.min(1, (t - t0) / dur), e = 1 - Math.pow(1 - k, 3);
    el.textContent = fmt(to * e);
    if (k < 1) requestAnimationFrame(step);
  };
  requestAnimationFrame(step);
}

function drawStats() {
  const t = S.totals;
  const tiles = [
    { k: "Code matched", v: ratio(t.code_matched, t.code), fmt: (x) => x.toFixed(2), unit: "%", d: `${num(t.code_matched)} of ${num(t.code)} bytes, byte-identical functions only`, c: "var(--green)" },
    ...(t.code_weighted == null ? [] : [{ k: "Weighted progress", v: ratio(t.code_weighted, t.code), fmt: (x) => x.toFixed(2), unit: "%",
      d: `${num(t.code_weighted)} of ${num(t.code)} bytes <br>includes partial credit for functions that are structurally matched. Honest (${pct(ratio(t.code_matched, t.code))}) counts only byte-identical functions`,
      title: "Weighted progress: honest matched bytes plus, for each structurally matched function, its size times its match percent. Honest counts only byte-identical functions.", c: "var(--gold)" }]),
    { k: "Functions", v: t.fns_matched, fmt: (x) => num(Math.round(x)), unit: `/ ${num(t.fns)}`, d: `${pct(ratio(t.fns_matched, t.fns))} of all functions`, m: ratio(t.fns_matched, t.fns), c: "var(--red)" },
    { k: "Data matched", v: ratio(t.data_matched, t.data), fmt: (x) => x.toFixed(2), unit: "%", d: `${num(t.data_matched)} of ${num(t.data)} bytes`, c: "var(--blue)" },
    { k: "Units Matching", v: t.matching, fmt: (x) => num(Math.round(x)), unit: `/ ${num(t.units)}`,
      d: t.sha1_ok ? `all ${num(t.matching)} link byte-identical &middot; ${num(t.files_unsplit)} files not split yet` : `link NOT verified: main.dol SHA-1 mismatch`,
      m: ratio(t.matching, t.units), c: "var(--purple)" },
  ];
  const host = $("#stats");
  host.classList.toggle("five", tiles.length === 5);
  host.innerHTML = tiles.map((x, i) => `<div class="stat" style="--c:${x.c}"${x.title ? ` title="${esc(x.title)}"` : ""}><div class="k">${x.k}</div>
    <div class="v"><span data-i="${i}">0</span> <small>${x.unit}</small></div><div class="d">${x.d}</div>
    <div class="meter"><i data-w="${x.m ?? x.v}"></i></div></div>`).join("");
  tiles.forEach((x, i) => countUp(host.querySelector(`[data-i="${i}"]`), x.v, x.fmt));
  requestAnimationFrame(() => host.querySelectorAll(".meter i").forEach((el) => (el.style.width = `${Math.max(0.6, +el.dataset.w)}%`)));

  // secondary measure, shown under the honest tiles
  const sx = $("#structural");
  if (sx) {
    sx.innerHTML = t.fns_structural == null ? "" :
      `<span class="sx-k">structurally matched</span> <b>${num(t.fns_structural)}</b> / ${num(t.fns)} functions &middot; ${num(t.code_structural)} bytes`
      + ` <span class="sx-d">(same instructions, register/order differences only; the honest numbers above count byte-identical functions only)</span>`
      + (t.code_weighted == null ? "" : (g => ` <span class="sx-d">Weighted game code: ${g ? pct(ratio(g.wm, g.c)) : "–"} (honest ${g ? pct(ratio(g.cm, g.c)) : "–"}).</span>`)(S.regions.find((r) => r.id === "game")));
  }

  const seal = $("#seal");
  seal.innerHTML = t.sha1_ok
    ? `<span class="ok">&#10003; main.dol SHA-1 OK</span> <span>${esc(S.sha1.slice(0, 12))}&hellip;</span> <span>&middot; honest objdiff report</span>`
    : `<span class="bad">&#10007; main.dol SHA-1 mismatch</span> <span>&middot; numbers from the honest objdiff report</span>`;
}

/* ---------- treemap ---------- */

const map = { focus: null, layer: null, W: 0, H: 0, sel: null, layoutRects: new Map() };

function viewDepth(focus) {
  if (focus.kind === "file") return 1;
  return 2;
}

function layoutFor(focus) {
  const depth = viewDepth(focus);
  const kids = (n, d) => (d < depth && n.children ? n.children.filter((k) => k.c > 0) : null);
  const wrap = (n, d) => {
    const ch = kids(n, d);
    return { node: n, children: ch && ch.length ? ch.map((k) => wrap(k, d + 1)) : undefined };
  };
  const h = d3.hierarchy(wrap(focus, 0)).sum((d) => (d.children ? 0 : d.node.c)).sort((a, b) => b.value - a.value);
  const small = map.W < 560;
  d3.treemap().size([map.W, map.H]).tile(d3.treemapSquarify.ratio(1.25)).round(true)
    .paddingInner((d) => (d.depth === 0 ? 3 : 1))
    .paddingOuter((d) => (d.depth === 1 ? 2 : 0))
    .paddingTop((d) => (d.depth === 1 && d.children && (d.x1 - d.x0) > 40 ? (small ? 16 : 19) : d.depth === 1 && d.children ? 2 : 0))(h);
  return h;
}

function tileFill(n) { return d3.color(colOf(n)); }

function drawLayer(focus) {
  const svg = d3.select("#map-svg");
  const h = layoutFor(focus);
  const layer = svg.append("g").attr("class", "layer");
  map.layoutRects = new Map();
  const nodes = h.descendants().slice(1);
  for (const d of nodes) map.layoutRects.set(d.data.node.id, [d.x0, d.y0, d.x1, d.y1]);

  // a milestone picked on the quest path lights its part of the map and dims the rest
  if (map.hl) for (const d of nodes) d.hl = inQuest(d.data.node) || (d.children || []).some((k) => inQuest(k.data.node));
  map.hlMix = !!map.hl && nodes.some((d) => !d.hl);  // only mark tiles when some are left out

  // groups (depth 1 with children)
  const groups = nodes.filter((d) => d.depth === 1 && d.children);
  const gg = layer.selectAll("g.grp").data(groups).join("g").attr("class", (d) => `grp${hlClass(d)}`);
  gg.append("rect").attr("x", (d) => d.x0).attr("y", (d) => d.y0).attr("width", (d) => d.x1 - d.x0).attr("height", (d) => d.y1 - d.y0)
    .attr("rx", 4).attr("fill", (d) => tileFill(d.data.node).copy({ opacity: 0.07 }))
    .attr("stroke", (d) => tileFill(d.data.node).copy({ opacity: 0.55 })).attr("stroke-width", 1)
    .attr("stroke-dasharray", (d) => (d.data.node.s === "U" ? "4 3" : null));
  const head = gg.filter((d) => d.x1 - d.x0 > 40 && d.y1 - d.y0 > 24).append("g").attr("class", "grp-head")
    .on("click", (ev, d) => drill(d.data.node, d))
    .on("pointermove", (ev, d) => !COARSE && showTip(d.data.node, ev)).on("pointerleave", hideTip);
  head.append("rect").attr("x", (d) => d.x0).attr("y", (d) => d.y0).attr("width", (d) => d.x1 - d.x0).attr("height", 18).attr("fill", "transparent");
  head.append("text").attr("class", "grp-label").attr("x", (d) => d.x0 + 6).attr("y", (d) => d.y0 + 13)
    .each(function (d) {
      const n = d.data.node, w = d.x1 - d.x0 - 12;
      const p = ` ${pct(ratio(n.cm, n.c))}`;
      const label = fit(n.name, w - p.length * 6.6, 6.8);
      const t = d3.select(this);
      t.append("tspan").text(label);
      if (label.length && w > (label.length + p.length) * 6.5) t.append("tspan").attr("class", "p").text(p);
    });

  // leaves
  const leaves = nodes.filter((d) => !d.children && d.x1 - d.x0 >= 1 && d.y1 - d.y0 >= 1);
  const lg = layer.selectAll("g.tile").data(leaves).join("g")
    .attr("class", (d) => `tile${canDrill(d) ? "" : " nodrill"}${hlClass(d)}`)
    .on("click", (ev, d) => onTileClick(ev, d))
    .on("pointermove", (ev, d) => !COARSE && showTip(d.data.node, ev))
    .on("pointerleave", hideTip);
  lg.append("rect").attr("class", "base").attr("x", (d) => d.x0).attr("y", (d) => d.y0)
    .attr("width", (d) => d.x1 - d.x0).attr("height", (d) => d.y1 - d.y0).attr("rx", (d) => (d.x1 - d.x0 > 8 ? 2 : 0))
    .attr("fill", (d) => tileFill(d.data.node).copy({ opacity: unsplit(d.data.node) ? 0.07 : 0.16 }))
    .attr("stroke", (d) => tileFill(d.data.node).copy({ opacity: unsplit(d.data.node) ? 0.45 : 0.4 }))
    .attr("stroke-dasharray", (d) => (unsplit(d.data.node) && d.x1 - d.x0 > 10 ? "3 2" : null));
  // the liquid: matched share fills the tile from the bottom (area-exact)
  lg.filter((d) => d.data.node.cm > 0).append("rect").attr("class", "fill")
    .attr("x", (d) => d.x0).attr("width", (d) => d.x1 - d.x0)
    .attr("y", (d) => d.y1).attr("height", 0)
    .attr("fill", (d) => `url(#${gradOf(d.data.node)})`)
    .attr("pointer-events", "none")
    .transition().delay((d, i) => (REDUCED ? 0 : 200 + Math.min(i, 200) * 2)).duration(REDUCED ? 0 : 800).ease(d3.easeCubicOut)
    .attr("y", (d) => d.y1 - (d.y1 - d.y0) * (d.data.node.cm / d.data.node.c))
    .attr("height", (d) => (d.y1 - d.y0) * (d.data.node.cm / d.data.node.c));
  // fuzzy hint for functions that are close but not matched
  lg.filter((d) => d.data.node.kind === "fn" && d.data.node.m > 0 && d.data.node.m < 100 && d.x1 - d.x0 > 6)
    .append("rect").attr("pointer-events", "none")
    .attr("x", (d) => d.x0).attr("y", (d) => d.y1 - 2).attr("height", 2)
    .attr("width", (d) => (d.x1 - d.x0) * d.data.node.m / 100).attr("fill", (d) => tileFill(d.data.node).copy({ opacity: 0.8 }));
  // labels
  lg.filter((d) => d.x1 - d.x0 > 46 && d.y1 - d.y0 > 20).each(function (d) {
    const n = d.data.node, w = d.x1 - d.x0 - 10, g = d3.select(this);
    const name = fit(displayName(n), w, 6.2);
    if (!name) return;
    g.append("text").attr("class", "tl").attr("x", d.x0 + 5).attr("y", d.y0 + 14).text(name);
    if (d.y1 - d.y0 > 34) g.append("text").attr("class", "tl p").attr("x", d.x0 + 5).attr("y", d.y0 + 28)
      .text(n.kind === "fn" ? fit(bytes(n.c), w, 6) : fit(`${pct(ratio(n.cm, n.c))} · ${bytes(n.c)}`, w, 6));
  });
  // unconfirmed category (low confidence in docs/names/categories.tsv): a small dashed ring
  // with "?" in the top-right corner of the file's tile (a leaf, or a group of its functions)
  const unconf = (d) => d.data.node.kind === "file" && d.data.node.u && d.x1 - d.x0 >= 14 && d.y1 - d.y0 >= 14;
  const mark = function (d) {
    const r = Math.min(5.5, (d.x1 - d.x0) / 4), cx = d.x1 - r - 2.5, cy = d.y0 + r + 2.5;
    const g = d3.select(this).append("g").attr("class", "unconf").attr("pointer-events", "none");
    g.append("circle").attr("cx", cx).attr("cy", cy).attr("r", r);
    if (r >= 4.5) g.append("text").attr("x", cx).attr("y", cy + 2.6).attr("text-anchor", "middle").text("?");
  };
  lg.filter(unconf).each(mark);
  gg.filter(unconf).each(mark);
  return layer;
}

function unsplit(n) { return (n.kind === "file" && n.s === "U") || (n.kind === "fn" && n.m === -1); }
function displayName(n) {
  if (n.kind !== "fn") return n.name;
  return n.name.replace(/\(.*$/, "");
}
function fit(s, w, cw) {
  const max = Math.floor(w / cw);
  if (max < 3) return "";
  return s.length <= max ? s : s.slice(0, max - 1) + "…";
}
function canDrill(d) {
  const n = d.data.node;
  return n.kind !== "fn";
}

function onTileClick(ev, d) {
  const n = d.data.node;
  if (COARSE) { showSheet(n, d); return; }
  if (n.kind === "fn") { pinTip(n, ev); return; }
  drill(n, d);
}

function drill(n) {
  hideTip();
  go(n.id);
}

function go(id) {
  const h = "#/" + id + (map.ms ? `?m=${map.ms}` : "");
  if (location.hash !== h) location.hash = h; else navigate();
}

function setLoading(on) { $("#map-loading").hidden = !on; }

async function navigate() {
  const [path, query] = location.hash.replace(/^#\/?/, "").split("?");
  const id = decodeURIComponent(path);
  setQuest(new URLSearchParams(query || "").get("m"));
  let target = byId.get(id);
  if (!target) {
    // functions and deep links into libs not loaded yet
    const parts = id.split("/");
    if (parts.length >= 3) {
      const lib = byId.get(parts.slice(0, 2).join("/"));
      if (lib) { setLoading(true); await ensureLoaded(lib); setLoading(false); target = byId.get(parts.slice(0, 3).join("/")); }
    }
  }
  target = target || ROOT;
  if (target.kind === "fn") target = target.parent;
  const tl = libOf(target);
  if (tl && tl.group && tl.group !== GROUP) setGroup(tl.group, false);
  if (target.kind === "lib" || target.kind === "file") {
    const lib = libOf(target);
    if (!lib.loaded) { setLoading(true); try { await ensureLoaded(lib); } finally { setLoading(false); } }
  }
  show(target);
}

function isAncestor(a, b) { for (let n = b.parent; n; n = n.parent) if (n === a) return true; return false; }

function show(focus) {
  const prev = map.focus, prevRects = map.layoutRects, old = map.layer;
  map.focus = focus;
  const layer = drawLayer(focus);
  const W = map.W, H = map.H, dur = REDUCED || !prev ? 0 : 650;
  const tf = (r) => `translate(${r[0]},${r[1]}) scale(${(r[2] - r[0]) / W},${(r[3] - r[1]) / H})`;
  const inv = (r) => { const sx = W / (r[2] - r[0]), sy = H / (r[3] - r[1]); return `translate(${-r[0] * sx},${-r[1] * sy}) scale(${sx},${sy})`; };
  if (prev && old && dur) {
    // zooming in: the new view grows out of the clicked tile
    let fromNode = focus; while (fromNode && fromNode.parent !== prev) fromNode = fromNode.parent;
    const fromRect = fromNode && prevRects.get(fromNode.id);
    let toNode = prev; while (toNode && toNode.parent !== focus) toNode = toNode.parent;
    const toRect = toNode && map.layoutRects.get(toNode.id);
    if (isAncestor(prev, focus) && fromRect) {
      layer.attr("transform", tf(fromRect)).attr("opacity", 0.2)
        .transition().duration(dur).ease(d3.easeCubicInOut).attr("transform", "translate(0,0) scale(1,1)").attr("opacity", 1);
      old.transition().duration(dur).ease(d3.easeCubicInOut).attr("transform", inv(fromRect)).attr("opacity", 0).remove();
    } else if (isAncestor(focus, prev) && toRect) {
      layer.attr("transform", inv(toRect)).attr("opacity", 0)
        .transition().duration(dur).ease(d3.easeCubicInOut).attr("transform", "translate(0,0) scale(1,1)").attr("opacity", 1);
      old.transition().duration(dur).ease(d3.easeCubicInOut).attr("transform", tf(toRect)).attr("opacity", 0).remove();
    } else {
      layer.attr("opacity", 0).transition().duration(dur).attr("opacity", 1);
      old.transition().duration(dur / 2).attr("opacity", 0).remove();
    }
  } else if (old) old.remove();
  map.layer = layer;
  drawCrumbs();
}

function drawCrumbs() {
  const chain = [];
  for (let n = map.focus; n; n = n.parent) chain.unshift(n);
  const el = $("#crumbs");
  const back = `<button class="back" type="button" aria-label="Zoom out" ${map.focus === ROOT ? "disabled" : ""}>&#8592;</button>`;
  el.innerHTML = back + chain.map((n, i) => {
    const label = n.kind === "root" ? "main.dol" : esc(n.name);
    const g = n.region ? nodeGlyph(n, 9) + " " : "";
    return i === chain.length - 1 ? `<span class="cur">${g}${label} <span class="pill ${n.s || ""}" style="${n.s ? "" : "display:none"}">${statusText(n.s)}</span></span>`
      : `<button type="button" data-id="${esc(n.id)}">${g}${label}</button><span class="sep">/</span>`;
  }).join("");
  el.querySelector(".back").onclick = zoomOut;
  el.querySelectorAll("button[data-id]").forEach((b) => (b.onclick = () => go(b.dataset.id)));
  drawQuestline();
}

function zoomOut() { if (map.focus && map.focus.parent) go(map.focus.parent.id); }

function statusText(s) { return s === "M" ? "Matching" : s === "N" ? "NonMatching" : s === "U" ? "not split yet" : ""; }

/* ---------- tooltip ---------- */

const tip = $("#tip");
function tipHTML(n) {
  const col = n.region ? colOf(n) : "#d29a2e";
  const share = ratio(n.c, TOTAL);
  if (n.kind === "fn") {
    const st = n.m === 100 ? "matched" : n.m === -1 ? "not split yet" : n.m > 0 ? `${n.m} % similar, not matched` : "not matched";
    return `<div class="t-name">${esc(n.name)}</div><div class="t-path">${esc(n.parent.path)} &middot; ${hex(n.addr)}</div>
      <dl><dt>Size</dt><dd>${num(n.c)} bytes</dd><dt>Of the DOL</dt><dd>${pct(share)}</dd><dt>Status</dt><dd>${st}</dd></dl>`;
  }
  const rows = [
    ["Size", `${bytes(n.c)}`], ["Of the DOL", pct(share)],
    ["Matched", `${pct(ratio(n.cm, n.c))} (${bytes(n.cm)})`],
    ...(n.wm != null && n.wm > n.cm ? [["Weighted", pct(ratio(n.wm, n.c))]] : []),
    ["Functions", `${num(n.fm)} / ${num(n.f)}`],
  ];
  if (n.kind === "file") {
    rows.push(["Status", statusText(n.s)]);
    if (n.region === "game" && n.cat && PART[n.cat]) rows.push(["Category", `${PART[n.cat].name}${n.u ? " (unconfirmed)" : ""}`]);
    if (n.region === "game" && n.ev) rows.push(["Why", `${n.ev} (${n.conf} confidence)`]);
    if (n.d) rows.push(["Data", `${pct(ratio(n.dm, n.d))} of ${bytes(n.d)}`]);
    if (n.t) rows.push(["Not split yet", bytes(n.t)]);
    rows.push(["Address", hex(n.addr)]);
  } else if (n.kind === "lib" || n.kind === "region") {
    const files = n.kind === "lib" ? n.children : n.children.flatMap((l) => l.children);
    const m = files.filter((f) => f.s === "M").length, u = files.filter((f) => f.s === "U").length;
    rows.push(["Files", `${num(files.length)} (${m} Matching, ${u} not split)`]);
    const uc = files.filter((f) => f.u).length;
    if (uc) rows.push(["Unconfirmed", `${num(uc)} files placed on thin evidence`]);
    const q = questFor(n);
    if (q.cur) rows.push(["Milestone", `${q.cur.title} (${STATUS[q.cur.status].word})`]);
    if (q.next) rows.push(["Next", q.next.title]);
  }
  const sub = n.kind === "file" ? n.path : n.kind === "region" ? n.desc : n.kind === "lib" ? (n.desc || n.parent.name) : "";
  return `<div class="t-name">${n.region ? nodeGlyph(n) + " " : ""}${esc(n.name)}</div>${sub ? `<div class="t-path">${esc(sub)}</div>` : ""}
    <div class="bar"><i style="width:${ratio(n.cm, n.c)}%;background:${col}"></i></div>
    <dl>${rows.map(([k, v]) => `<dt>${k}</dt><dd>${esc(v)}</dd>`).join("")}</dl>`;
}
function place(ev) {
  const pad = 14, r = tip.getBoundingClientRect();
  let x = ev.clientX + pad, y = ev.clientY + pad;
  if (x + r.width > innerWidth - 8) x = ev.clientX - r.width - pad;
  if (y + r.height > innerHeight - 8) y = ev.clientY - r.height - pad;
  tip.style.left = `${Math.max(8, x)}px`; tip.style.top = `${Math.max(8, y)}px`;
}
function showTip(n, ev) {
  tip.classList.remove("sheet");
  tip.innerHTML = tipHTML(n);
  tip.hidden = false;
  place(ev);
}
function pinTip(n, ev) { showTip(n, ev); }
function hideTip() { if (!tip.classList.contains("sheet")) tip.hidden = true; }
function closeSheet() { tip.hidden = true; tip.classList.remove("sheet"); d3.selectAll(".tile.sel").classed("sel", false); }
function showSheet(n, d) {
  d3.selectAll(".tile.sel").classed("sel", false);
  d3.selectAll(".tile").filter((x) => x === d).classed("sel", true);
  tip.classList.add("sheet");
  tip.innerHTML = tipHTML(n) + `<div class="go">${n.kind !== "fn" ? `<button class="primary" type="button" data-go>Open</button>` : ""}<button type="button" data-close>Close</button></div>`;
  tip.hidden = false;
  const o = tip.querySelector("[data-go]");
  if (o) o.onclick = () => { closeSheet(); go(n.id); };
  tip.querySelector("[data-close]").onclick = closeSheet;
}

function initMap() {
  const svg = d3.select("#map-svg");
  const defs = svg.append("defs");
  const grads = [...Object.entries(REGION).map(([id, r]) => [`tg-${id}`, r.color]),
    ...Object.entries(PART).map(([k, p]) => [`tg-p-${k}`, p.color])];
  for (const [id, color] of grads) {
    const g = defs.append("linearGradient").attr("id", id).attr("x1", 0).attr("y1", 1).attr("x2", 0).attr("y2", 0);
    g.append("stop").attr("offset", 0).attr("stop-color", d3.color(color).darker(0.3));
    g.append("stop").attr("offset", 1).attr("stop-color", d3.color(color).brighter(0.5));
  }
  const measure = () => { const b = $("#map").getBoundingClientRect(); map.W = Math.round(b.width); map.H = Math.round(b.height); };
  measure();
  let last = map.W;
  new ResizeObserver(() => {
    measure();
    if (map.focus && Math.abs(map.W - last) > 2) { last = map.W; if (map.layer) map.layer.remove(); map.layer = null; map.layer = drawLayer(map.focus); }
  }).observe($("#map"));
  window.addEventListener("hashchange", navigate);
  document.addEventListener("keydown", (e) => {
    if (e.key === "Escape") { if (!tip.hidden && tip.classList.contains("sheet")) closeSheet(); else zoomOut(); }
  });
  svg.on("contextmenu", (e) => { e.preventDefault(); zoomOut(); });
  document.addEventListener("pointerdown", (e) => {
    if (tip.classList.contains("sheet") && !tip.contains(e.target) && !e.target.closest(".tile")) closeSheet();
  });
  drawLegend();
  document.querySelectorAll(".groupby").forEach((el) => {
    el.innerHTML = `<span class="gb-k" id="${el.id}-k" aria-label="Group the game code by">Group by</span>` + Object.entries(GROUPS).map(([k, g]) =>
      `<button type="button" data-g="${k}" aria-pressed="${k === GROUP}">${esc(g.word)}</button>`).join("");
    el.setAttribute("role", "group");
    el.setAttribute("aria-labelledby", `${el.id}-k`);
    el.querySelectorAll("button").forEach((b) => (b.onclick = () => setGroup(b.dataset.g)));
  });
}

function drawLegend() {
  $("#legend").innerHTML = gameGroups().map((l) => `<span>${partGlyph(l.part)} ${esc(PART[l.part].name)}</span>`).join("")
    + `<span>${glyph("jsystem")} Libraries</span>`
    + `<span><i class="fillkey"></i> matched share</span><span><i class="dashkey"></i> not split yet</span>`
    + `<span><i class="unconfkey">?</i> category unconfirmed</span>`;
}

// Switch the game code between its categories and the boot-path view. The map stays
// where it is when it can (the region or the whole DOL); inside the game code it goes
// to the game region. `nav` false: the caller navigates itself.
function setGroup(g, nav = true) {
  if (!GROUPS[g] || g === GROUP || !GROUPS[g].libs.length) return;
  GROUP = g;
  try { localStorage.setItem("fsa-group", g); } catch (e) { /* storage blocked */ }
  byId.get("game").children = GROUPS[g].libs;
  document.querySelectorAll(".groupby button").forEach((b) => b.setAttribute("aria-pressed", b.dataset.g === g));
  document.body.dataset.group = g;
  drawLegend();
  drawRemaining();
  drawGameParts();
  table.rows = tableRows(); renderTable();
  if (!nav) return;
  const f = map.focus;
  if (f && f.region === "game" && f.kind !== "region") go("game");
  else if (f) { if (map.layer) map.layer.remove(); map.layer = drawLayer(f); drawCrumbs(); }
}

/* ---------- overview: where the work is, the libraries, the game code ---------- */

function growOnView(host) {
  const io = new IntersectionObserver((es) => es.forEach((e) => {
    if (e.isIntersecting) { e.target.querySelectorAll("[data-w]").forEach((i) => (i.style.width = `${Math.max(+i.dataset.w, +i.dataset.w > 0 ? 0.8 : 0)}%`)); io.unobserve(e.target); }
  }), { threshold: 0.1 });
  io.observe(host);
}
function openOnClick(host, sel, fn) {
  host.querySelectorAll(sel).forEach((el) => {
    el.onclick = (e) => { if (!e.target.closest("a")) fn(el); };
    el.onkeydown = (e) => { if (e.key === "Enter" || e.key === " ") { e.preventDefault(); fn(el); } };
  });
}
const toMap = (id) => { go(id); $("#treemap").scrollIntoView({ behavior: REDUCED ? "auto" : "smooth" }); };

// Remaining (not yet byte-identical) code: a segment per game group in the current view, then the libraries.
function drawRemaining() {
  const host = $("#remaining");
  const L = S.libraries;
  const segs = [...gameGroups().map((n) => ({ name: PART[n.part].name, col: PART[n.part].color, left: n.c - n.cm, glyph: partGlyph(n.part) })),
    { name: "Libraries", col: "#d29a2e", left: L.c - L.cm, glyph: glyph("jsystem") }];
  const total = segs.reduce((a, x) => a + x.left, 0);
  const game = segs.slice(0, -1).reduce((a, x) => a + x.left, 0);
  host.innerHTML = `<div class="rw-k">Where the remaining work is <span>${bytes(total)} of code not yet byte-identical; the game code is ${pct(ratio(game, total))} of it</span></div>
    <div class="rw-bar" role="img" aria-label="${esc(segs.map((x) => `${x.name} ${pct(ratio(x.left, total))}`).join(", "))}">${segs.map((x) =>
      `<i style="flex:${x.left} 1 0;background:${x.col}" title="${esc(`${x.name}: ${bytes(x.left)} left (${pct(ratio(x.left, total))})`)}"></i>`).join("")}</div>
    <div class="rw-legend">${segs.map((x) => `<span>${x.glyph} ${esc(x.name)} <b>${pct(ratio(x.left, total))}</b></span>`).join("")}</div>`;
}

function drawLibraries() {
  const host = $("#libraries");
  const L = S.libraries;
  if (!L) { $("#libs-panel").hidden = true; return; }
  const rows = LIB_REGIONS.map((id) => byId.get(id)).filter(Boolean).map((r) => {
    const left = r.f - r.fm;
    return `<div class="lrow" data-id="${esc(r.id)}" role="button" tabindex="0">
      <span class="ln">${glyph(r.id, 11)} ${esc(r.name)}</span>
      <span class="track"><i data-w="${ratio(r.cm, r.c)}" style="background:${REGION[r.id].color}"></i></span>
      <span class="lv"><b>${pct(ratio(r.cm, r.c))}</b> &middot; ${num(r.fm)}/${num(r.f)} fn${left ? ` &middot; ${num(left)} left` : ""}</span></div>`;
  }).join("");
  host.innerHTML = `<div class="lsum">
      <div class="big">${pct(ratio(L.cm, L.c))}</div>
      <div class="lsum-d">of ${bytes(L.c)} of library code byte-identical &middot; ${num(L.fm)} / ${num(L.f)} functions &middot;
        ${num(L.matching)} / ${num(L.units)} files Matching${L.wm > L.cm ? ` <span class="wnote">weighted ${pct(ratio(L.wm, L.c))}</span>` : ""}</div>
      <div class="track big-track"><i data-w="${ratio(L.cm, L.c)}" style="background:var(--gold)"></i></div>
    </div><div class="lrows">${rows}</div>`;
  growOnView(host);
  openOnClick(host, ".lrow", (el) => toMap(el.dataset.id));
}

function drawGameParts() {
  const host = $("#parts");
  const g = byId.get("game");
  const parts = gameGroups();
  const gw = g.wm ?? g.cm;
  $("#game-sub").textContent = GROUP === "boot"
    ? "FSA's own code by boot-path phase (docs/BOOT_PATH.md): the two stretches the game runs from power-on to a live Shadow Battle, in that order, then the actors and everything else. Solid bars are byte-identical; weighted progress is shown per category only."
    : "FSA's own code in eight categories, by what each file is (docs/names/categories.tsv). Solid bars are byte-identical; the lighter extension is weighted progress (partial credit for structurally matched functions). A ? marks files placed on thin evidence.";
  $("#game-sum").innerHTML = `<b>${pct(ratio(g.cm, g.c))}</b> byte-identical &middot; <span class="wnote">weighted ${pct(ratio(gw, g.c))}</span> &middot; ${num(g.fm)} / ${num(g.f)} functions &middot; ${bytes(g.c)}`;
  host.innerHTML = parts.map((n) => {
    const p = PART[n.part], files = n.children, m = files.filter((f) => f.s === "M").length, wm = n.wm ?? n.cm;
    const uc = files.filter((f) => f.u).length;
    return `<div class="part" data-id="${esc(n.id)}" role="button" tabindex="0" style="--c:${p.color}">
      <div class="p-head"><span class="p-name">${partGlyph(n.part, 12)} ${esc(p.name)}</span><span class="p-pct">${pct(ratio(n.cm, n.c))}</span></div>
      <div class="p-desc">${esc(p.desc)}</div>
      <div class="p-bar" title="solid: byte-identical; light: weighted (partial credit for structurally matched functions)">
        <i class="w" data-w="${ratio(wm, n.c)}"></i><i class="h" data-w="${ratio(n.cm, n.c)}"></i></div>
      <div class="p-nums">${n.wm != null ? `<span>weighted <b>${pct(ratio(wm, n.c))}</b></span>` : ""}<span><b>${num(n.fm)}</b> / ${num(n.f)} functions</span>
        <span><b>${num(m)}</b> / ${num(files.length)} files Matching</span><span>${bytes(n.c)} &middot; ${pct(ratio(n.c, g.c))} of the game</span>
        ${uc ? `<span class="p-unconf" title="Files placed in this category on thin evidence (low confidence in docs/names/categories.tsv)"><i class="unconfkey">?</i> ${num(uc)} unconfirmed</span>` : ""}</div>
    </div>`;
  }).join("");
  growOnView(host);
  openOnClick(host, ".part", (el) => toMap(el.dataset.id));
}

/* ---------- history ---------- */

function drawHistory(hist) {
  const host = $("#history");
  if (!hist.length) { host.innerHTML = `<p class="note">No history yet.</p>`; return; }
  // two series: the libraries (the four library regions) and the game code
  const ids = ["libs", "game"];
  const SERIES = { libs: { color: "#d29a2e", name: "Libraries", glyph: glyph("jsystem") }, game: { color: REGION.game.color, name: "Game code", glyph: glyph("game") } };
  const pts = hist.map((e) => {
    const p = { date: new Date(e.date), e };
    const share = (id) => (e.regions[id] ? (100 * e.regions[id][0]) / e.code_total : 0);
    p.libs = LIB_REGIONS.reduce((a, id) => a + share(id), 0);
    p.game = share("game");
    p.total = (100 * e.code) / e.code_total;
    return p;
  });
  const W = host.clientWidth || 600, H = 240, m = { t: 12, r: 14, b: 26, l: 44 };
  const svg = d3.select(host).append("svg").attr("viewBox", `0 0 ${W} ${H}`).attr("height", H);
  if (pts.length < 2) {
    // a single commit so far: show its composition as one stacked bar
    const p = pts[pts.length - 1];
    const max = Math.max(10, Math.ceil(p.total / 5) * 5);
    const x = d3.scaleLinear().domain([0, max]).range([m.l, W - m.r]);
    svg.attr("viewBox", `0 0 ${W} 110`).attr("height", 110);
    let acc = 0;
    for (const id of ids) {
      const w = x(acc + p[id]) - x(acc);
      if (w > 0) svg.append("rect").attr("x", x(acc)).attr("y", 20).attr("height", 34).attr("rx", 3)
        .attr("fill", SERIES[id].color).attr("width", Math.max(w - 2, 1));
      acc += p[id];
    }
    svg.append("g").attr("class", "axis").attr("transform", "translate(0,62)").call(d3.axisBottom(x).ticks(5).tickFormat((d) => `${d} %`));
    host.insertAdjacentHTML("beforeend", `<p class="note">History starts at this commit (${pct(p.total)} of the DOL's code matched): `
      + ids.filter((id) => p[id] > 0).map((id) => `${SERIES[id].glyph} ${esc(SERIES[id].name)} ${pct(p[id])}`).join(" &nbsp; ")
      + `. Each merge to <code>matching</code> adds a point.</p>`);
    return;
  }
  const x = d3.scaleTime().domain(d3.extent(pts, (p) => p.date)).range([m.l, W - m.r]);
  const y = d3.scaleLinear().domain([0, Math.max(5, d3.max(pts, (p) => p.total) * 1.15)]).nice().range([H - m.b, m.t]);
  const stack = d3.stack().keys(ids)(pts);
  svg.append("g").attr("class", "grid").attr("transform", `translate(${m.l},0)`).call(d3.axisLeft(y).ticks(5).tickSize(-(W - m.l - m.r)).tickFormat("")).select(".domain").remove();
  const area = d3.area().curve(d3.curveMonotoneX).x((d) => x(d.data.date)).y0((d) => y(d[0])).y1((d) => y(d[1]));
  svg.append("g").selectAll("path").data(stack).join("path").attr("fill", (s) => SERIES[s.key].color).attr("fill-opacity", 0.85)
    .attr("stroke", "#0b0d1a").attr("stroke-width", 1).attr("d", area);
  svg.append("path").datum(pts).attr("fill", "none").attr("stroke", "#fff").attr("stroke-width", 2)
    .attr("d", d3.line().curve(d3.curveMonotoneX).x((p) => x(p.date)).y((p) => y(p.total)));
  svg.append("g").attr("class", "axis").attr("transform", `translate(0,${H - m.b})`).call(d3.axisBottom(x).ticks(Math.max(2, Math.floor(W / 110))));
  svg.append("g").attr("class", "axis").attr("transform", `translate(${m.l},0)`).call(d3.axisLeft(y).ticks(5).tickFormat((d) => `${d}%`));
  const cross = svg.append("line").attr("stroke", "rgba(255,255,255,.5)").attr("y1", m.t).attr("y2", H - m.b).style("display", "none");
  const dot = svg.append("circle").attr("r", 5).attr("fill", "#fff").attr("stroke", "#0b0d1a").attr("stroke-width", 2).style("display", "none");
  const bis = d3.bisector((p) => p.date).center;
  svg.append("rect").attr("x", m.l).attr("y", m.t).attr("width", W - m.l - m.r).attr("height", H - m.t - m.b).attr("fill", "transparent")
    .on("pointermove", (ev) => {
      const [mx] = d3.pointer(ev);
      const p = pts[bis(pts, x.invert(mx))];
      cross.style("display", null).attr("x1", x(p.date)).attr("x2", x(p.date));
      dot.style("display", null).attr("cx", x(p.date)).attr("cy", y(p.total));
      const c = p.e.commit ? `<a href="${REPO}/commit/${p.e.commit}">${p.e.commit.slice(0, 8)}</a>` : "this commit";
      tip.classList.remove("sheet");
      tip.innerHTML = `<div class="t-name">${pct(p.total)} of code matched</div>
        <div class="t-path">${esc(p.date.toISOString().slice(0, 16).replace("T", " "))} UTC &middot; ${c}</div>
        ${p.e.subject ? `<div style="margin-bottom:6px">${esc(p.e.subject)}</div>` : ""}
        <dl>${ids.slice().reverse().map((id) => `<dt>${SERIES[id].glyph} ${esc(SERIES[id].name)}</dt><dd>${pct(p[id])} of the DOL</dd>`).join("")}
        <dt>Functions</dt><dd>${num(p.e.fns)} / ${num(p.e.fns_total)}</dd></dl>`;
      tip.hidden = false; place(ev);
    })
    .on("pointerleave", () => { cross.style("display", "none"); dot.style("display", "none"); hideTip(); });
}

/* ---------- files table ---------- */

const table = { rows: [], sort: "c", dir: -1, q: "", regions: new Set(), status: "", limit: 120 };

function tableRows() {
  const rows = [];
  for (const r of ROOT.children) for (const l of r.children) for (const f of l.children) {
    rows.push({ node: f, n: f.name, p: f.path, lib: l.name, region: r.id, c: f.c, cm: f.cm, f: f.f, fm: f.fm, s: f.s,
      share: ratio(f.c, TOTAL), pct: f.c ? ratio(f.cm, f.c) : -1,
      hay: `${f.name} ${f.path} ${l.name} ${r.name}${f.u ? " unconfirmed" : ""}`.toLowerCase() });
  }
  return rows;
}

function initTable() {
  table.rows = tableRows();
  $("#chips").innerHTML = ROOT.children.map((r) => `<button class="chip" type="button" aria-pressed="false" data-r="${r.id}" style="--c:${REGION[r.id].color}">${glyph(r.id)} ${esc(REGION[r.id].short)}</button>`).join("");
  $("#chips").querySelectorAll(".chip").forEach((b) => (b.onclick = () => {
    const on = b.getAttribute("aria-pressed") !== "true";
    b.setAttribute("aria-pressed", on);
    on ? table.regions.add(b.dataset.r) : table.regions.delete(b.dataset.r);
    table.limit = 120; renderTable();
  }));
  let timer;
  $("#q").oninput = (e) => { clearTimeout(timer); timer = setTimeout(() => { table.q = e.target.value.trim().toLowerCase(); table.limit = 120; renderTable(); }, 120); };
  $("#status-filter").onchange = (e) => { table.status = e.target.value; table.limit = 120; renderTable(); };
  document.querySelectorAll("#files th").forEach((th) => (th.onclick = () => {
    const k = th.dataset.k;
    if (table.sort === k) table.dir = -table.dir; else { table.sort = k; table.dir = k === "n" || k === "lib" || k === "s" ? 1 : -1; }
    renderTable();
  }));
  $("#more").onclick = () => { table.limit += 400; renderTable(); };
  renderTable();
}

function renderTable() {
  const terms = table.q.split(/\s+/).filter(Boolean);
  let rows = table.rows.filter((r) => (!table.regions.size || table.regions.has(r.region))
    && (!table.status || r.s === table.status) && terms.every((t) => r.hay.includes(t)));
  const k = table.sort, dir = table.dir;
  rows.sort((a, b) => (typeof a[k] === "string" ? a[k].localeCompare(b[k]) : a[k] - b[k]) * dir || b.c - a.c);
  document.querySelectorAll("#files th").forEach((th) => {
    if (th.dataset.k === k) th.setAttribute("aria-sort", dir > 0 ? "ascending" : "descending"); else th.removeAttribute("aria-sort");
  });
  const shown = rows.slice(0, table.limit);
  $("#files tbody").innerHTML = shown.map((r) => {
    const col = colOf(r.node);
    return `<tr data-id="${esc(r.node.id)}"><td><div class="fn">${esc(r.n)}</div><div class="fp">${esc(r.p)}</div></td>
      <td class="hide-s"><span class="lb">${nodeGlyph(r.node)} ${esc(r.lib)}</span>${r.node.u ? ` <span class="unconf-tag" title="Category unconfirmed: placed on thin evidence (docs/names/categories.tsv)">unconfirmed</span>` : ""}</td>
      <td class="num">${bytes(r.c)}</td>
      <td class="num hide-s">${pct(r.share)}</td>
      <td class="num"><span class="mbar"><i style="width:${Math.max(0, r.pct)}%;background:${col}"></i></span>${r.c ? pct(r.pct) : "–"}</td>
      <td class="num hide-s">${num(r.fm)} / ${num(r.f)}</td>
      <td><span class="pill ${r.s}">${statusText(r.s)}</span></td></tr>`;
  }).join("");
  $("#count").textContent = `${num(shown.length)} of ${num(rows.length)} files`;
  $("#more").hidden = shown.length >= rows.length;
  $("#files tbody").querySelectorAll("tr").forEach((tr) => (tr.onclick = () => {
    go(tr.dataset.id); $("#treemap").scrollIntoView({ behavior: REDUCED ? "auto" : "smooth" });
  }));
}

/* ---------- honesty note ---------- */

function drawHonest() {
  const t = S.totals;
  const when = new Date(S.generated);
  $("#honest-body").innerHTML = `
    <p>Every number here comes from the project's <b>honest objdiff report</b> (<code>build/G4SE01/report_honest.json</code>,
      objdiff-cli with <code>functionRelocDiffs=all</code>). Every unit is diffed, even those flagged Matching, so a function
      counts only when its instructions <i>and</i> relocations match the original. Byte-pattern hits and objdiff's lenient default
      are never counted.</p>
    <p>The link is the final authority: a plain <code>ninja</code> links every Matching unit's own object into <code>main.dol</code>
      and checks its SHA-1 (<code>${esc(S.sha1)}</code>). ${t.sha1_ok ? `It matched for this snapshot, so all ${num(t.matching)} Matching units link byte-identical.` : `<b>It did not match for this snapshot.</b>`}</p>
    <p>The total is the whole DOL: ${num(t.code)} bytes of code in ${num(t.fns)} functions. Code dtk has not split into a source file yet
      (${num(t.files_unsplit)} files) is placed by the library map (<code>lib_map.tsv</code>) and the game-code file map (<code>tu_map.tsv</code>), so
      unstarted work shows at its true size.</p>
    <p>The game code is grouped into eight categories by what each file is, from <code>docs/names/categories.tsv</code>
      (<code>tools/categorize.py</code>, method in <code>docs/names/METHOD.md</code>): the class hierarchy (an <code>EnemyBase</code>
      subclass is an enemy), the actor the spawner builds and its kind in <code>actors.tsv</code>, strings and resource names, callers
      and the docs. A file placed on thin evidence is marked unconfirmed (?). The <b>Boot path</b> switch regroups the same files the
      way <code>docs/BOOT_PATH.md</code> orders the work: Dolphin recorded every instruction the game runs from power-on to a live
      Shadow Battle, and each file goes to the first stretch that runs it (boot to the main menu, or main menu to the battle); of the
      rest, <b>Actors</b> are the <code>act_*</code> files and the files whose classes the actor spawner constructs; <b>Everything else</b>
      is the remainder. Either grouping only sorts files: every number is the same honest count.</p>
    <p>Weighted progress is a second, clearly separate figure: the honest bytes plus, for each function
      <code>tools/structural.py</code> finds structurally matched (same instructions, registers or order differ), its size times its
      match percent. It shows work under way in big functions; the honest numbers never include it.</p>
    <p>Game code is compiled the way the original was: <code>-inline auto,deferred</code>, with each source's functions in descending
      address order (<code>docs/INLINING.md</code>). Names come from a reviewed manifest (<code>docs/names/names.tsv</code>); a name without
      enough evidence keeps its address-based placeholder.</p>
    <p>Snapshot ${esc(when.toISOString().slice(0, 16).replace("T", " "))} UTC, built on
      <a href="${REPO}/commit/${esc(S.base)}">${esc(S.base.slice(0, 8))}</a>. The data is regenerated locally with
      <code>tools/site_data.py</code> and committed with each merge; CI only publishes it.</p>`;
}

/* ---------- the quest (roadmap) ----------
   data/roadmap.json is docs/roadmap.yml with each milestone's live measure. A milestone's
   `covers` are treemap ids (`<region>` or `<region>/<library id>`): picking it lights those
   tiles (map.hl) and zooms there; the map in turn names the milestone of whatever it shows. */

const HUE = { green: "#3aae5c", red: "#ef5b52", blue: "#3d9cf0", purple: "#c35ee6", gold: "#d29a2e",
  // the eight game categories (tools/readme_assets.py HUE)
  leaf: "#4aa75c", crimson: "#c52607", rose: "#a96087", azure: "#508ae1", teal: "#00a8a1", violet: "#8c55b4", cyan: "#269ac2", olive: "#608c3d" };
const STATUS = {
  done: { word: "done" }, active: { word: "under way" }, next: { word: "next" }, later: { word: "later" },
};
let RM = null;
const MS = new Map(); // milestone id -> milestone (+ phase, pi, ord)
const quest = { phase: null };

const msFrac = (m) => (m.measure && m.measure.total ? m.measure.done / m.measure.total : null);
const hueOf = (ph) => HUE[ph.hue] || HUE.gold;

function measureText(m) {
  const x = m.measure;
  if (!x) return "";
  const f = x.kind === "code" || x.kind === "split" ? bytes : num;
  return `${pct(ratio(x.done, x.total))} ${x.label} · ${f(x.done)} / ${f(x.total)}`;
}

function phaseFrac(ph) {
  if (ph.status === "done") return 1;
  const ms = ph.milestones;
  return ms.reduce((a, m) => a + (m.status === "done" ? 1 : m.status === "active" ? msFrac(m) || 0 : 0), 0) / (ms.length || 1);
}

// A gem in the emblem's shape: filled when done, filled to its share while under way,
// an outline when ahead (dashed for "later").
let gemN = 0;
function gemSVG(status, col, w, h, frac = null) {
  const f = status === "done" ? 1 : status === "active" ? Math.max(0.15, frac ?? 0.5) : 0;
  const off = rhombusFill(f, h / 2) / h, k = `qg${gemN++}`, p = 1.5, cx = w / 2 + p, cy = h / 2 + p;
  return `<svg class="qgem ${status}" width="${w + 2 * p}" height="${h + 2 * p}" viewBox="0 0 ${w + 2 * p} ${h + 2 * p}" aria-hidden="true">
    <defs><linearGradient id="${k}" x1="0" y1="1" x2="0" y2="0"><stop offset="${off}" stop-color="${col}"/><stop offset="${off}" stop-color="${col}" stop-opacity="0.1"/></linearGradient></defs>
    <path d="M${cx} ${p}L${w + p} ${cy}L${cx} ${h + p}L${p} ${cy}Z" fill="url(#${k})" stroke="${col}" stroke-width="1.3"${status === "later" ? ' stroke-dasharray="2 2"' : ""}/>
    ${f > 0 ? `<path d="M${cx} ${p}L${w + p} ${cy}L${cx} ${cy}Z" fill="rgba(255,255,255,.14)"/>` : ""}</svg>`;
}

function initQuest(rm) {
  if (!rm || !rm.phases) { $("#quest-panel").hidden = true; $("#nownext").hidden = true; return; }
  RM = rm;
  rm.phases.forEach((ph, pi) => ph.milestones.forEach((m) => { m.phase = ph; m.pi = pi; m.ord = MS.size; MS.set(m.id, m); }));
  quest.phase = (rm.phases.find((p) => p.focus) || rm.phases.find((p) => p.status === "active") || rm.phases.find((p) => p.status === "next") || rm.phases[0]).id;
  $("#quest-legend").innerHTML = Object.keys(STATUS).map((s) => `<span>${gemSVG(s, "#d29a2e", 9, 12, 0.5)} ${STATUS[s].word}</span>`).join("");
  drawNowNext();
  drawPath();
  drawStage();
  $("#vows").innerHTML = (rm.policies || []).map((p) => `<li><b>${esc(p.title)}.</b> ${esc(p.text)}</li>`).join("");
}

// the focus phase's first milestone with this status, else the first anywhere
const firstWith = (status) => [...MS.values()].find((m) => m.status === status && m.phase.focus)
  || [...MS.values()].find((m) => m.status === status);

function drawNowNext() {
  const cards = [["Now", firstWith("active")], ["Next", firstWith("next")]].filter(([, m]) => m);
  const host = $("#nownext");
  host.innerHTML = cards.map(([k, m]) => {
    const col = hueOf(m.phase), f = msFrac(m);
    return `<button type="button" class="nn ${m.status}" data-ms="${esc(m.id)}" style="--c:${col}">
      <span class="nn-k">${gemSVG(m.status, col, 10, 13, f)} ${k} &middot; phase ${m.pi + 1}</span>
      <span class="nn-t">${esc(m.title)}</span>
      <span class="nn-d">${esc(m.desc)}</span>
      ${m.measure ? `<span class="nn-bar"><i style="width:${Math.max(1, 100 * f)}%"></i></span><span class="nn-v">${esc(measureText(m))}</span>` : ""}
    </button>`;
  }).join("");
  host.querySelectorAll(".nn").forEach((b) => (b.onclick = () => {
    pickMilestone(b.dataset.ms, true);
    $("#quest-panel").scrollIntoView({ behavior: REDUCED ? "auto" : "smooth" });
  }));
}

function drawPath() {
  const host = $("#path");
  host.style.setProperty("--n", RM.phases.length);
  host.innerHTML = RM.phases.map((ph, i) => {
    const col = hueOf(ph), done = ph.milestones.filter((m) => m.status === "done").length;
    return `<li class="station ${ph.status}${ph.id === quest.phase ? " cur" : ""}" style="--c:${col}">
      <button type="button" data-ph="${esc(ph.id)}" aria-pressed="${ph.id === quest.phase}">
        <span class="st-gem">${gemSVG(ph.status, col, 26, 36, phaseFrac(ph))}</span>
        <span class="st-txt"><span class="st-n">Phase ${i + 1} &middot; ${STATUS[ph.status].word}</span>
        <span class="st-t">${esc(ph.title)}</span>
        <span class="st-dots" title="${done} of ${ph.milestones.length} milestones done">${ph.milestones.map((m) => gemSVG(m.status, col, 6, 8, msFrac(m))).join("")}</span></span>
      </button></li>`;
  }).join("");
  host.querySelectorAll("button[data-ph]").forEach((b) => (b.onclick = () => {
    quest.phase = b.dataset.ph; drawPath(); drawStage();
    if (innerWidth <= 860) $("#stage").scrollIntoView({ behavior: REDUCED ? "auto" : "smooth", block: "nearest" });
  }));
}

function coverChip(id) {
  const n = byId.get(id);
  if (!n) return "";
  return `<span class="m-cov">${nodeGlyph(n, 9)} ${esc(n.name)}</span>`;
}

function drawStage() {
  if (!RM) return;
  const pi = RM.phases.findIndex((p) => p.id === quest.phase), ph = RM.phases[pi], col = hueOf(ph);
  const host = $("#stage");
  host.style.setProperty("--c", col);
  host.innerHTML = `<div class="stage-head"><h3>Phase ${pi + 1}: ${esc(ph.title)} <span class="m-st ${ph.status}">${STATUS[ph.status].word}</span></h3>
      <p>${esc(ph.summary)}</p></div>
    <ol class="ms">${ph.milestones.map((m) => {
      const f = msFrac(m), sel = map.ms === m.id, canMap = m.covers.length > 0;
      return `<li class="m ${m.status}${sel ? " sel" : ""}${canMap ? "" : " nomap"}" data-ms="${esc(m.id)}"${canMap ? ` tabindex="0" role="button" aria-pressed="${sel}"` : ""}>
        ${gemSVG(m.status, col, 13, 17, f)}
        <div class="m-body">
          <div class="m-t">${esc(m.title)} <span class="m-st ${m.status}">${STATUS[m.status].word}</span></div>
          <div class="m-d">${esc(m.desc)}</div>
          ${m.measure ? `<div class="m-bar"><i style="width:${f ? Math.max(0.8, 100 * f) : 0}%"></i></div><div class="m-v">${esc(measureText(m))}</div>` : ""}
          <div class="m-foot">${m.covers.map(coverChip).join("")}
            ${(m.landed || []).map((c) => `<a class="m-commit" href="${REPO}/commit/${esc(c)}">${esc(c)}</a>`).join("")}
            ${sel ? `<button type="button" class="m-go">See it in the map &darr;</button>` : ""}</div>
        </div></li>`;
    }).join("")}</ol>`;
  host.querySelectorAll(".m[data-ms]:not(.nomap)").forEach((el) => {
    el.onclick = (e) => {
      if (e.target.closest("a")) return;
      if (e.target.closest(".m-go")) { $("#treemap").scrollIntoView({ behavior: REDUCED ? "auto" : "smooth" }); return; }
      pickMilestone(el.dataset.ms);
    };
    el.onkeydown = (e) => { if ((e.key === "Enter" || e.key === " ") && e.target === el) { e.preventDefault(); pickMilestone(el.dataset.ms); } };
  });
}

// Common treemap ancestor of a milestone's covers: the library, its region, or the whole DOL.
function questTarget(m) {
  if (!m.covers.length) return null;
  if (m.covers.length === 1) return m.covers[0];
  const regions = new Set(m.covers.map((c) => c.split("/")[0]));
  return regions.size === 1 ? [...regions][0] : "";
}

// Pick (or, picked again, drop) a milestone: light its tiles and zoom the map to them.
function pickMilestone(id, keep = false) {
  const m = MS.get(id);
  if (!m) return;
  if (map.ms === id && !keep) { map.ms = null; go(map.focus ? map.focus.id : ""); return; }
  map.ms = id;
  const t = questTarget(m);
  go(t === null ? (map.focus ? map.focus.id : "") : t);
}

// Called from navigate() with the hash's ?m=: the single place the highlight is set.
function setQuest(id) {
  const m = id && MS.get(id);
  map.ms = m ? id : null;
  map.hl = m && m.covers.length ? new Set(m.covers) : null;
  if (m && quest.phase !== m.phase.id) { quest.phase = m.phase.id; drawPath(); }
  drawStage();
}

function inQuest(n) {
  for (; n; n = n.parent) if (map.hl.has(n.id)) return true;
  return false;
}
const hlClass = (d) => (map.hl && map.hlMix ? (d.hl ? " hl" : " dim") : "");

// The milestone a treemap node belongs to now, and the next one that will work on it.
function questFor(n) {
  if (!RM || !n) return {};
  if (n.kind === "root") return { cur: firstWith("active"), next: firstWith("next") };
  const ids = [];
  for (let a = n; a && a.kind !== "root"; a = a.parent) ids.push(a.id);
  const rank = (m) => (m.covers.includes(n.id) ? 0 : m.covers.some((c) => ids.includes(c)) ? 1 : 2);
  const list = [...MS.values()].filter((m) => m.covers.some((c) => ids.includes(c) || (n.kind === "region" && c.startsWith(n.id + "/"))));
  const act = list.filter((m) => m.status === "active").sort((a, b) => rank(a) - rank(b) || a.ord - b.ord)[0];
  const done = list.filter((m) => m.status === "done").sort((a, b) => rank(a) - rank(b) || b.ord - a.ord)[0];
  const next = list.filter((m) => m.status === "next" || m.status === "later").sort((a, b) => rank(a) - rank(b) || a.ord - b.ord)[0];
  return { cur: act || done, next };
}

function drawQuestline() {
  const el = $("#questline");
  if (!RM) { el.hidden = true; return; }
  const link = (m) => `<button type="button" class="ql-m" data-ms="${esc(m.id)}" style="--c:${hueOf(m.phase)}">${gemSVG(m.status, hueOf(m.phase), 8, 11, msFrac(m))} ${esc(m.title)}</button>`;
  if (map.ms) {
    const m = MS.get(map.ms);
    el.innerHTML = `<span class="ql-k">Milestone</span> ${link(m)} <span class="ql-s">${STATUS[m.status].word}${m.measure ? ` &middot; ${esc(measureText(m))}` : ""}</span>
      <button type="button" class="ql-x" aria-label="Clear the milestone highlight">&times; clear</button>`;
    el.querySelector(".ql-x").onclick = () => { map.ms = null; go(map.focus.id); };
  } else {
    const q = questFor(map.focus);
    el.innerHTML = q.cur || q.next
      ? `${q.cur ? `<span class="ql-k">${q.cur.status === "active" ? "Now" : "Done"}</span> ${link(q.cur)}` : ""}
         ${q.next ? `<span class="ql-k">Next</span> ${link(q.next)}` : ""}`
      : `<span class="ql-s">No milestone covers this part yet.</span>`;
  }
  el.querySelectorAll(".ql-m").forEach((b) => (b.onclick = () => {
    const m = MS.get(b.dataset.ms);
    if (map.ms === m.id) { quest.phase = m.phase.id; drawPath(); drawStage(); $("#quest-panel").scrollIntoView({ behavior: REDUCED ? "auto" : "smooth" }); }
    else pickMilestone(m.id);
  }));
}

/* ---------- boot ---------- */

async function main() {
  const [summary, hist, roadmap] = await Promise.all([
    fetch("data/summary.json").then((r) => r.json()),
    fetch("data/history.json").then((r) => r.json()).catch(() => []),
    fetch("data/roadmap.json").then((r) => r.json()).catch(() => null),
  ]);
  S = summary; TOTAL = summary.totals.code;
  buildTree(summary);
  document.body.dataset.group = GROUP;
  drawStats();
  drawEmblem();
  initQuest(roadmap);
  initMap();
  await navigate();
  // README tiles link to #treemap or a deep #/<region>/<lib>: land on the map once it is drawn
  if (location.hash === "#treemap" || location.hash.startsWith("#/")) $("#treemap").scrollIntoView();
  drawRemaining();
  drawLibraries();
  drawGameParts();
  drawHistory(hist.length ? hist : []);
  initTable();
  drawHonest();
}
main().catch((e) => {
  console.error(e);
  document.querySelector("main").insertAdjacentHTML("afterbegin", `<p class="panel">Could not load the progress data: ${esc(e.message)}</p>`);
});
