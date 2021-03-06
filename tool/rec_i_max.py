import serial
import numpy as np
import pickle

GRAB_DATA_SIZE = 1000

ser = serial.Serial("COM3", 115200)

para_index = {'title': 0, 'i1': 1, 'i2': 2}

# collect data
i1 = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
i2 = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)

cnt = 0
while cnt < GRAB_DATA_SIZE:
    try:
        raw_data = ser.readline()
        data_list = raw_data.decode('utf-8').split(',')
        # print(data_list)
        i1[cnt] = float(data_list[para_index['i1']])
        i2[cnt] = float(data_list[para_index['i2']])
        cnt += 1
    except Exception as e:
        print(data_list)

def save2pickle(file_name, data):
    with open(file_name, 'wb') as f:
        pickle.dump(data, f)

save2pickle('i1_max.pickle', i1*3.3/65535/0.15)
save2pickle('i2_max.pickle', i2*3.3/65535/0.15)

ser.close()




