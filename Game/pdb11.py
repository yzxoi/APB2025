# generate_pdb11.py
import collections, itertools, json

JUMPS = [...]              # 33 种洞位 jump 列表，与你 C++ 中一致
SUBSETS = [L, R, C]        # 三个 11-idx 列表
PDB = [[255]*2048 for _ in range(3)]

for k, subset in enumerate(SUBSETS):
    start = (1<<11)-1                      # 终局：该子集全空
    q = collections.deque([(0, start)])    # BFS 逆向
    PDB[k][start] = 0

    while q:
        d, mask = q.popleft()
        board = expand(mask, subset)       # → 33-bit 位板
        for f,o,t in JUMPS:
            if board>>f&1 and board>>o&1 and not board>>t&1:
                b2 = board ^ (1<<f) ^ (1<<o) | (1<<t)
                mask2 = compress(b2, subset)   # → 11-bit
                if PDB[k][mask2] == 255:
                    PDB[k][mask2] = d+1
                    q.append((d+1, mask2))

# 写文件
with open("pdb11.h","w") as fp:
    fp.write("static const uint8_t PDB[3][2048] = {\n")
    for k in range(3):
        fp.write("  {")
        fp.write(",".join(map(str,PDB[k])))
        fp.write("},\n")
    fp.write("};\n")
