# 先執行rec_data, example
# python .\tool\rec_data.py -sz=100 -mk=dutytest -len=5

import numpy as np
from mypickle import save2pickle, load_pickle

para_index = {'title': 0, 'duty': 1, 'i1': 2, 'i2': 3, 'dummy':4}

f = open("data/raw_data_dutytest.txt")
raw_data = f.readlines()
f.close()
# duty 0, 25, 50, 75, 100
data_length = int(len(raw_data) / 5)

i1_d0 = np.zeros([data_length, 1], dtype=np.float32)
i1_d25 = np.zeros([data_length, 1], dtype=np.float32)
i1_d50 = np.zeros([data_length, 1], dtype=np.float32)
i1_d75 = np.zeros([data_length, 1], dtype=np.float32)
i1_d100 = np.zeros([data_length, 1], dtype=np.float32)

i2_d0 = np.zeros([data_length, 1], dtype=np.float32)
i2_d25 = np.zeros([data_length, 1], dtype=np.float32)
i2_d50 = np.zeros([data_length, 1], dtype=np.float32)
i2_d75 = np.zeros([data_length, 1], dtype=np.float32)
i2_d100 = np.zeros([data_length, 1], dtype=np.float32)

cnt = np.zeros([5, 1], dtype=np.int16)
for data_line in raw_data:
    data_list = data_line.split(",")
    if int(data_list[para_index["duty"]]) == 0:
        i1_d0[cnt[0]] = float(data_list[para_index["i1"]])
        i2_d0[cnt[0]] = float(data_list[para_index["i2"]])
        cnt[0] += 1
    elif int(data_list[para_index["duty"]]) == 25:
        i1_d25[cnt[1]] = float(data_list[para_index["i1"]])
        i2_d25[cnt[1]] = float(data_list[para_index["i2"]])
        cnt[1] += 1
    elif int(data_list[para_index["duty"]]) == 50:
        i1_d50[cnt[2]] = float(data_list[para_index["i1"]])
        i2_d50[cnt[2]] = float(data_list[para_index["i2"]])
        cnt[2] += 1
    elif int(data_list[para_index["duty"]]) == 75:
        i1_d75[cnt[3]] = float(data_list[para_index["i1"]])
        i2_d75[cnt[3]] = float(data_list[para_index["i2"]])
        cnt[3] += 1
    elif int(data_list[para_index["duty"]]) == 100:
        i1_d100[cnt[4]] = float(data_list[para_index["i1"]])
        i2_d100[cnt[4]] = float(data_list[para_index["i2"]])
        cnt[4] += 1

print(cnt)

print("i1 duty0   : ", np.mean(i1_d0))
print("i1 duty25  : ", np.mean(i1_d25))
print("i1 duty50  : ", np.mean(i1_d50))
print("i1 duty75  : ", np.mean(i1_d75))
print("i1 duty100 : ", np.mean(i1_d100))
print("i2 duty0   : ", np.mean(i2_d0))
print("i2 duty25  : ", np.mean(i2_d25))
print("i2 duty50  : ", np.mean(i2_d50))
print("i2 duty75  : ", np.mean(i2_d75))
print("i2 duty100 : ", np.mean(i2_d100))
