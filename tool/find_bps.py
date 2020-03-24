import numpy as np

FREQ = 72e6
BAUDRATE = 115200
ERROR = 0.0001
collect = []

for sbr in range(2**12-1):
    for brfa in range(32):
        if sbr == 0 and brfa == 0:
            continue
        bps = FREQ/(16*(sbr + (brfa/32)))
        if abs(bps - BAUDRATE)/BAUDRATE < ERROR:
            print(sbr, brfa, bps, ' er: ', abs(bps - BAUDRATE)/BAUDRATE)





