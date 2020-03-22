import numpy as np

collect = []

for sbr in range(2**12-1):
    for brfa in range(32):
        if sbr == 0 and brfa == 0:
            continue
        bps = 72e6/(16*(sbr + (brfa/32)))
        if abs(bps - 115200)/115200 < 0.0001:
            print(sbr, brfa, bps)





