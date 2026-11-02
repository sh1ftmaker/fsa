import sys,struct,re
from base import *
from collections import defaultdict
CT=[u32(a) for a in range(0x8045bd00,0x8045c640,4)]
SINITS=set(x for x in CT if x)
s2syms=sorted([s for s in syms if s['sec']=='.sdata2'],key=lambda s:s['addr'])
def dtype(s):
    m=re.search(r'data:(\w+)',s['attrs']); return m[1] if m else None
def natural(s):
    t=dtype(s)
    if t=='float' or t=='4byte': return 4
    if t=='double': return 8
    if t=='2byte': return 2
    if t=='byte': return 1
    if t=='string':
        b=rd(s['addr'],s['size']); return b.index(b'\0')+1 if b'\0' in b else s['size']
    return s['size']

R_DATA=json.load(open('refs.json'))['data']
byname_addr={s['name']:s['addr'] for s in syms}
def run(lo,hi,anchor_extra=()):
    G=[f for f in fns if lo<=f['addr']<hi]; N=len(G)
    idx={f['addr']:i for i,f in enumerate(G)}
    refby=defaultdict(set); outside=defaultdict(int)
    for f in fns:
        for a in f['refs']:
            if f['addr'] in idx: refby[a].add(idx[f['addr']])
            else: outside[a]+=1
    def cls(a):
        s=secof(a)
        if s=='.data':
            n=symaddr.get(a,{}).get('name','')
            return '.data.jt' if n.startswith('jumptable') else '.data'
        return s
    # anchors: cut after each sinit (k = index of first fn of next TU)
    anchors=set(idx[x]+1 for x in SINITS if x in idx and idx[x]+1<N)
    anchors|=set(anchor_extra)
    anc=sorted(anchors)
    import bisect
    def blockof(i): return bisect.bisect_right(anc,i)
    glob=set()
    for a,s in refby.items():
        if outside[a] or secof(a) is None: glob.add(a); continue
        if secof(a)=='.sdata2' and symaddr.get(a,{}).get('size')==4 and 0x80000000<=u32(a)<0x81800000: glob.add(a); continue
        if len({blockof(i) for i in s})>1: glob.add(a)
    covers={}
    for c in ['.sdata2','.rodata','.data','.data.jt','.bss','.sbss','.sdata','.text']:
        diff=[0]*(N+2)
        for a,s in refby.items():
            if a in glob or cls(a)!=c: continue
            r0,r1=min(s),max(s)
            if c=='.text':
                # static-function heuristic: callee must be in-region and callers within its block
                if a not in idx: continue
                r0=min(r0,idx[a]); r1=max(r1,idx[a])
            if r1>r0: diff[r0+1]+=1; diff[r1+1]-=1
        cv=[0]*(N+1);run_=0
        for k in range(N+1): run_+=diff[k]; cv[k]=run_
        covers[c]=cv
    HARD=['.sdata2','.rodata','.data.jt']
    SOFT=['.data','.bss','.sbss','.sdata']
    allowed=[all(covers[c][k]==0 for c in HARD) for k in range(N+1)]
    # order constraints: prefix max < suffix min for local extab / sdata2 / rodata
    def order_ok(vals):
        pm=[0]*(N+1); m=-1
        for i in range(N):
            if i in anchors: m=-1
            pm[i]=m
            if vals[i]: m=max(m,max(vals[i]))
        sm=[0]*(N+1); m=1<<40; sm[N]=m
        for i in range(N-1,-1,-1):
            if vals[i]: m=min(m,min(vals[i]))
            sm[i]=m
            if i in anchors: m=1<<40
        return [pm[k]<sm[k] for k in range(N+1)]
    EXT={u32(a):u32(a+8) for a in range(0x80011860,0x80021840,12) if u32(a)}
    ordc={'extab':order_ok([[EXT[f['addr']]] if f['addr'] in EXT else [] for f in G])}
    for c in ['.sdata2','.data.jt']:
        ordc[c]=order_ok([[a for a in f['refs'] if a not in glob and cls(a)==c] for f in G])
    for c,v in ordc.items():
        allowed=[x and y for x,y in zip(allowed,v)]
    R_ext=EXT
    soft=[sum(covers[c][k] for c in SOFT) for k in range(N+1)]
    # positive evidence intervals: (lo_k, hi_k, kind) meaning a cut must exist at some k in [lo_k,hi_k]
    ev=[]
    # sdata2 holes and dups
    loc2=[s for s in s2syms if s['addr'] in refby and s['addr'] not in glob]
    def refrange(addrs):
        r=set()
        for a in addrs: r|=refby[a]
        return r
    # holes
    for i,s in enumerate(s2syms[:-1]):
        nxt=s2syms[i+1]['addr']; end=s['addr']+natural(s)
        hole=nxt-end
        if hole<=0: continue
        if rd(end,hole)!=b'\0'*hole: continue
        nd=dtype(s2syms[i+1])=='double' or (s2syms[i+1]['size']==8 and dtype(s2syms[i+1]) is None)
        strong = (nxt%8==0 and not nd) or hole>=8
        if not strong: continue
        below=[x['addr'] for x in s2syms[max(0,i-200):i+1] if x['addr'] in refby and x['addr'] not in glob]
        above=[x['addr'] for x in s2syms[i+1:i+200] if x['addr'] in refby and x['addr'] not in glob]
        if not below or not above: continue
        a=max(refby[below[-1]]) ; b=min(refby[above[0]])
        # use all symbols nearby for tighter bound
        a=max(max(refby[x]) for x in below[-20:]); b=min(min(refby[x]) for x in above[:20])
        if b>a: ev.append((a+1,b,'s2hole' if strong else 's2hole_d',hex(nxt)))
    # duplicates in sdata2 (value+size), only nearby (within 0x400 bytes)
    for i,s in enumerate(loc2):
        v=rd(s['addr'],natural(s))
        for t in loc2[i+1:i+80]:
            if t['addr']-s['addr']>0x600: break
            if dtype(s) in ('float','double') and dtype(t)==dtype(s) and rd(t['addr'],natural(t))==v:
                a=max(refby[s['addr']]); b=min(refby[t['addr']])
                if b>a: ev.append((a+1,b,'s2dup',hex(t['addr'])))
                break
    # duplicate strings in .rodata/.data
    strs=[x for x in syms if x['sec'] in ('.rodata','.data') and dtype(x)=='string' and x['addr'] in refby and x['addr'] not in glob]
    strs.sort(key=lambda x:x['addr'])
    seen={}
    for x in strs:
        b=rd(x['addr'],natural(x))
        key=(x['sec'],b)
        if key in seen:
            p=seen[key]; a=max(refby[p]); bb=min(refby[x['addr']])
            if bb>a and bb-a<400: ev.append((a+1,bb,'strdup',hex(x['addr'])))
        seen[key]=x['addr']
    return dict(EXT=R_ext,ordc=ordc,cls=cls,G=G,N=N,anchors=anchors,allowed=allowed,soft=soft,ev=ev,refby=refby,glob=glob,covers=covers,idx=idx)

TAILMODE='A'
def resolve(R):
    G,N,allowed,soft=R['G'],R['N'],R['allowed'],R['soft']
    cuts={k:{'kind':{'sinit'},'conf':'high','alts':(k,k)} for k in R['anchors']}
    # weak tail: extend past functions after the sinit that belong to the sinit TU
    callers=defaultdict(set)
    for i,f in enumerate(G):
        for a in f['refs']:
            if a in R['idx']: callers[R['idx'][a]].add(i)
    dataholders=defaultdict(set)   # fn index -> data object addrs containing a pointer to it
    for dn,lst in R_DATA.items():
        da=byname_addr.get(dn)
        if da is None: continue
        for a in lst:
            if a in R['idx']: dataholders[R['idx'][a]].add(da)
    anyref=defaultdict(int)
    for f in fns:
        for a in f['refs']: anyref[a]+=1
    for dn,lst in R_DATA.items():
        for a in lst: anyref[a]+=1
    ks=sorted(cuts)
    for n,k in enumerate(ks):
        start=ks[n-1] if n else 0
        j=k
        while j<N and j-k<80 and (j+1 not in cuts):
            f=G[j]
            inTU=lambda S: any(start<=c<k for c in S)
            if (not allowed[j]) or (TAILMODE=='B' and (inTU(callers[j]) or any(inTU(R['refby'].get(d,())) for d in dataholders[j]) or (anyref[f['addr']]==0 and f['size']<=0x80))) or (TAILMODE=='C' and not callers[j] and any(inTU(R['refby'].get(d,())) for d in dataholders[j])):
                j+=1
            else: break
        if j!=k and j<N:
            cuts[j]=cuts.pop(k); cuts[j]['kind']={'sinit+tail'}; cuts[j]['tail']=j-k; cuts[j]['alts']=(j,j)
    for (a,b,kind,where) in sorted(R['ev']):
        if any(a<=k<=b for k in cuts):
            for k in cuts:
                if a<=k<=b: cuts[k]['kind'].add(kind)
            continue
        cand=[k for k in range(a,b+1) if 0<k<N and allowed[k]]
        if not cand: continue
        m=min(soft[k] for k in cand); c2=[k for k in cand if soft[k]==m]
        k=c2[0]
        conf='high' if len(cand)==1 else ('med' if len(c2)==1 or (G[c2[-1]]['addr']-G[c2[0]]['addr'])<0x200 else 'low')
        cuts[k]={'kind':{kind},'conf':conf,'alts':(cand[0],cand[-1])}
    return cuts
