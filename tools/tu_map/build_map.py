from pipeline import *
import re
from collections import Counter
END=0x8045bd00
R=run(GAME0,END); cuts=resolve(R)
G,N,idx,refby,glob,cls,EXT=R['G'],R['N'],R['idx'],R['refby'],R['glob'],R['cls'],R['EXT']
anyref=Counter()
for f in fns:
    for a in f['refs']: anyref[a]+=1
for dn,lst in R_DATA.items():
    for a in lst: anyref[a]+=1
for addr,why in [(0x804280C0,'thp_split'),(0x8042E780,'thp_split')]:
    cuts[idx[addr]]={'kind':{why},'conf':'low','alts':(idx[addr],idx[addr])}
ks=[0]+sorted(cuts)+[N]
allowed=R['allowed']
sinitset=SINITS
RANK={'high':3,'med':2,'low':1}
def cutconf(k):
    if k==0: return 'high'   # boundary with known JSystem split
    if k==N: return 'high'   # end of .text
    return cuts[k]['conf']
def cutdesc(k):
    if k in (0,N): return 'section-edge'
    c=cuts[k]; d='+'.join(sorted(c['kind']))
    if c['alts'][0]!=c['alts'][1]: d+=f"(range {G[c['alts'][0]]['addr']:08X}-{G[c['alts'][1]]['addr']:08X})"
    return d
def sstr(a,sz):
    b=rd(a,min(sz,200)) or b''
    b=b.split(b'\0')[0]
    try: return b.decode('ascii')
    except: return None
rows=[]
for t,(a,b) in enumerate(zip(ks,ks[1:])):
    fs=G[a:b]; start=fs[0]['addr']; end=G[b]['addr'] if b<N else END
    refs=set(x for f in fs for x in f['refs'])
    loc=[x for x in refs if x not in glob]
    ranges={}
    for c in ['.sdata2','.rodata','.data','.bss','.sbss','.sdata','.sbss2']:
        xs=[x for x in loc if secof(x)==c and x in symaddr]
        if xs:
            lo=min(xs); hi=max(xs); hi_end=hi+max(symaddr[hi]['size'],1)
            ranges[c]=f"{lo:08X}-{hi_end:08X}"
    ex=[EXT[f['addr']] for f in fs if f['addr'] in EXT]
    if ex: ranges['extab']=f"{min(ex):08X}-{max(ex):08X}+"
    # internal checks
    s2=[x for x in loc if secof(x)=='.sdata2' and x in symaddr]
    vals=Counter((dtype(symaddr[x]),rd(x,natural(symaddr[x]))) for x in s2 if dtype(symaddr[x]) in ('float','double'))
    dups=sum(v-1 for v in vals.values() if v>1)
    has_sinit=any(f['addr'] in sinitset for f in fs)
    extab_frac=len(ex)/len(fs)
    # strings
    hints=[]
    for x in sorted(refs):
        s=symaddr.get(x)
        if not s or s['sec'] not in ('.rodata','.data','.sdata2','.sdata'): continue
        if re.search(r'data:string',s['attrs']):
            v=sstr(x,s['size'])
            if v and len(v)>=4: hints.append(v)
    def score(h):
        if re.search(r'\.(cpp|c)$',h): return 0
        if re.search(r'\.(arc|szs|rarc)$',h,re.I): return 1
        if re.search(r'\.(bmd|bdl|bck|btk|brk|bpk|btp|bti|bin|csv|blo|bmg|thp|jpc|jpa)$',h,re.I): return 2
        if re.match(r'^[A-Za-z_][\w ]+$',h) and len(h)<40: return 3
        return 4
    hints=sorted(set(hints),key=lambda h:(score(h),h))
    name=f"tu_{start:08X}"
    if hints and score(hints[0])<=2:
        base=re.sub(r'.*/','',hints[0]); base=re.sub(r'\.(arc|szs|bmd|bdl|bck|btk|brk|bpk|btp|bti|bin|csv|blo|bmg|rarc|jpc)$','',base,flags=re.I)
        name = base if score(hints[0])==0 else f"tu_{start:08X}_{re.sub(r'[^A-Za-z0-9_]','_',base)}"
    named=[f['name'] for f in fs if not f['name'].startswith(('fn_','dtor_'))]
    
    # confidence
    cs,ce=cutconf(a),cutconf(b)
    conf=min(cs,ce,key=lambda c:RANK[c])
    flags=[]
    if has_sinit: flags.append('has_sinit')
    if len(s2)>=8 and dups==0: flags.append(f'nodup({len(s2)}k)')
    if dups: flags.append(f'DUPS={dups}'); conf=min(conf,'low',key=lambda c:RANK[c])
    if extab_frac==0: flags.append('no_extab(C?)')
    elif extab_frac<0.5: flags.append(f'extab{int(extab_frac*100)}%')
    size=end-start
    verified = len(s2)>=8 and dups==0
    free=sum(1 for k in range(a+1,b) if allowed[k])
    if free: flags.append(f'free_cuts={free}')
    if size>0x4000 and not verified: conf=min(conf,'med',key=lambda c:RANK[c]); flags.append('large_unverified')
    if conf=='high' and not verified and free>0 and len(fs)>8: conf='med'; flags.append('unverified_single_TU')
    if any('tail' in k for k in cutdesc(b).split('+')): flags.append('weak_tail_end')
    if start==0x804280C0: name='THP_lib_or_player_804280C0'
    ev=f"start:{cutdesc(a)}; end:{cutdesc(b)}; "+','.join(flags)
    rows.append(dict(start=start,end=end,size=size,n=len(fs),ev=ev,conf=conf,name=name,hints='|'.join(hints[:4]),ranges=ranges))
import json
json.dump(rows,open('rows.json','w'))
with open('tu_map.tsv','w') as o:
    cols=['start','end','size','n_functions','evidence','confidence','suggested_name','sdata2','rodata','data','bss','sbss','sdata','extab','string_hints']
    o.write('\t'.join(cols)+'\n')
    for r in rows:
        rg=r['ranges']
        o.write('\t'.join([f"0x{r['start']:08X}",f"0x{r['end']:08X}",f"0x{r['size']:X}",str(r['n']),r['ev'],r['conf'],r['name']]+[rg.get(c,'') for c in ['.sdata2','.rodata','.data','.bss','.sbss','.sdata','extab']]+[re.sub(r'[\t\r\n]',' ',r['hints'])[:200]])+'\n')
tot=END-GAME0
cc=Counter()
for r in rows: cc[r['conf']]+=r['size']
print('TUs',len(rows),'coverage',{k:f"{v/tot*100:.1f}%" for k,v in cc.items()}, Counter(r['conf'] for r in rows))
