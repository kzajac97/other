from collections import Counter, defaultdict
from datetime import datetime

inFile = open("datad4.txt","r")
s = inFile.read()

guards = defaultdict(Counter)
for t, m in [l.split('] ') for l in sorted(s.splitlines()) if l]:
    t = datetime.strptime(t, '[%Y-%m-%d %H:%M')
    if '#' in m:     g = int(m.split('#')[1].split()[0])
    if 'falls' in m: start = t
    if 'wakes' in m:
        minutes = int((t - start).total_seconds() // 60)
        guards[g].update(Counter((start.minute+i)%60 for i in range(minutes)))

_, id = max((sum(c.values()), id) for id, c in guards.items())
part1 = id * guards[id].most_common()[0][0]
print(id)
print(guards[id].most_common()[0][0])

(_, minute), id = max((c.most_common()[0][::-1], id) for id, c in guards.items())
part2 = id * minute
print(id)
print(minute)