import numpy as np
import random
import matplotlib.pyplot as plt

th_mach_init = 0
mach_ang = 0
ele_ang = 0
er = 0

def sele_angle(enc_value):
    global th_mach_init
    global mach_ang
    global ele_ang
    global er
    temp = enc_value - th_mach_init
    if temp < 0:
        temp += 16384
    th_delta = temp - mach_ang
    mach_ang = temp

    if abs(th_delta) > 8197:
        if th_delta < 0:
            th_delta += 16384
        else:
            th_delta -= 16384
    ele_ang += th_delta
    if ele_ang > 327:
        ele_ang -= 328
        er += 32
        if er > 50:
            ele_ang += 1
            er -= 100
    elif ele_ang < 1:
        ele_ang += 328
        er -= 68
        if er < -50:
            ele_ang -= 1
            er += 100

    if mach_ang == th_mach_init:
        ele_ang = 0
        er = 0


if __name__ == "__main__":
    NUM = 16384
    ele_ang_mat = np.zeros([NUM*2, 1], dtype=np.float32)
    ans_mat = np.zeros([NUM*2, 1], dtype=np.float32)
    er_mat = np.zeros([NUM*2, 1], dtype=np.float32)
    er_cnt = np.zeros([NUM*2, 1], dtype=np.float32)
    i = 0
    for enc_test in range(0, NUM):
        # enc_test = enc_test + random.randint(-5, 5)
        if enc_test < 0:
            enc_test = 0
        elif enc_test > 16383:
            enc_test = 16383
        sele_angle(enc_test)
        ele_ang_mat[i] = ele_ang
        ans_mat[i] = enc_test % 327.68
        er_mat[i] = ele_ang_mat[i] - ans_mat[i]
        er_cnt[i] = er
        i += 1
    for enc_test in range(NUM-1, 0, -1):
        # enc_test = enc_test + random.randint(-5, 5)
        if enc_test < 0:
            enc_test = 0
        elif enc_test > 16383:
            enc_test = 16383
        sele_angle(enc_test)
        ele_ang_mat[i] = ele_ang
        ans_mat[i] = enc_test % 327.68
        er_mat[i] = ele_ang_mat[i] - ans_mat[i]
        er_cnt[i] = er
        i += 1

    print(np.sum(np.abs(er_cnt) > 50))
    plt.plot(er_cnt)
    plt.show()
    print(np.sum(ele_ang_mat<0))
    print((np.sum(np.abs(ans_mat)) - np.sum(np.abs(ele_ang_mat))) / ans_mat.shape[0])
    print(np.mean(er_mat))


