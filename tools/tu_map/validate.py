from pipeline import *
sp=sorted([(s,e,n) for n,sec,s,e in load_splits() if sec=='.text'])
LO,HI=0x80021840,0x800F39EC
R=run(LO,HI)
cuts=resolve(R)
G=R['G'];idx=R['idx']
# truth: boundaries between adjacent contiguous known splits
truth=set()
for (s1,e1,n1),(s2,e2,n2) in zip(sp,sp[1:]):
    if e1==s2 and s2 in idx: truth.add(idx[s2])
# known non-boundaries: cuts strictly inside a known split
inside=set()
for s,e,n in sp:
    for f in G:
        if s<f['addr']<e: inside.add(idx[f['addr']])
pred=set(cuts)
print('truth boundaries',len(truth),'found',len(truth&pred))
fp=pred&inside
print('pred cuts',len(pred),'inside known TU (false pos)',len(fp))
from collections import Counter
print('FP kinds',Counter(tuple(sorted(cuts[k]['kind'])) for k in fp))
print('TP kinds',Counter(tuple(sorted(cuts[k]['kind'])) for k in truth&pred))
print('allowed@truth',sum(R['allowed'][k] for k in truth),'allowed inside',sum(R['allowed'][k] for k in inside),'of',len(inside))
for k in sorted(fp)[:10]: print(hex(G[k]['addr']),cuts[k])
print('--- cover at truth by section')
for c in R['covers']:
    print(c, sum(1 for k in truth if R['covers'][c][k]>0), 'inside zero-cover', sum(1 for k in inside if R['covers'][c][k]==0))
# weak tail check: JASTrack
rb=R['refby']
names={s['addr']:s['name'] for s in syms}
for k in sorted(truth):
    if R['covers']['.sdata2'][k]>0:
        print('truth cut',hex(G[k]['addr']))
        for a,s in rb.items():
            if secof(a)=='.sdata2' and a not in R['glob'] and min(s)<k<=max(s):
                print('   ',hex(a),rd(a,4).hex(),[ (hex(G[i]['addr']),G[i]['name']) for i in sorted(s)])
print('--- interval-level precision')
# known TU membership per function index
mem={}
for s,e,n in sp:
    for f in G:
        if s<=f['addr']<e: mem[idx[f['addr']]]=n
res=Counter()
bad=[]
for k,c in cuts.items():
    a,b=c['alts']
    # does any cut in [a,b] separate two different-known-TU or unknown functions?
    ok=False; inside_only=True
    for j in range(a,b+1):
        l=mem.get(j-1); r=mem.get(j)
        if l is None or r is None or l!=r: ok=True; break
    res[(c['conf'],ok)]+=1
    if not ok: bad.append((hex(G[k]['addr']),mem.get(k),c))
print(sorted(res.items()))
for b in bad: print(b)
