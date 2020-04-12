import numpy as np
import matplotlib.pyplot as plt
import random

RANDOM_GAIN = 5
TEST_NUMS = 100
er = np.zeros([TEST_NUMS, 1], dtype=np.float32)
er_cum = np.zeros([TEST_NUMS, 1], dtype=np.float32)
i_svpwm = np.zeros([TEST_NUMS, 1], dtype=np.float32)
I_SVPWM_MEAN = 0.65

kp = I_SVPWM_MEAN * 0.01
ki = I_SVPWM_MEAN * 0.001

for i in range(TEST_NUMS):
    p = random.random()
    if p > 0.9:
        rand_er = 10
    elif p < 0.1:
        rand_er = -10
    else:
        rand_er = (random.random()-0.5)*RANDOM_GAIN
    er[i] = rand_er
    if i > 0:
        er_cum[i] += rand_er + er_cum[i-1]
    else:
        er_cum[i] = rand_er
    i_svpwm[i] = (-kp*rand_er - ki*er_cum[i]) / I_SVPWM_MEAN



plt.plot(er)
plt.show()
plt.plot(er_cum)
plt.show()

plt.plot(i_svpwm)
plt.show()

