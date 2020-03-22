import serial
import os
import numpy as np
from mypickle import save2pickle, load_pickle

GRAB_DATA_SIZE = 1000
DATA_DIR = 'data'
try:
    os.mkdir(DATA_DIR)
except:
    pass
ser = serial.Serial("COM3", 115200)

para_index = {'title': 0, 'i1': 1, 'i2': 2, 'angle': 3, 'sele_dangle': 4, 'cele_dangle': 5, 'th_svpwm': 6,
                'i_svpwm': 7, 'th_er': 8, 'th_cum': 9, 'pwm1': 10, 'pwm2': 11}

# collect data
i1 = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
i2 = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
angle = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
sele_dangle = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
cele_dangle = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
th_svpwm = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
i_svpwm = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
th_er = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
th_cum = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
pwm1 = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)
pwm2 = np.zeros([GRAB_DATA_SIZE, 1], dtype=np.float32)

cnt = 0
ser.flush()
while cnt < GRAB_DATA_SIZE:
    try:
        raw_data = ser.readline()
        data_list = raw_data.decode('utf-8').split(',')
        # print(raw_data)
        i1[cnt] = float(data_list[para_index['i1']])
        i2[cnt] = float(data_list[para_index['i2']])
        angle[cnt] = float(data_list[para_index['angle']])
        sele_dangle[cnt] = float(data_list[para_index['sele_dangle']])
        cele_dangle[cnt] = float(data_list[para_index['cele_dangle']])
        th_svpwm[cnt] = float(data_list[para_index['th_svpwm']])
        i_svpwm[cnt] = float(data_list[para_index['i_svpwm']])
        th_er[cnt] = float(data_list[para_index['th_er']])
        th_cum[cnt] = float(data_list[para_index['th_cum']])
        pwm1[cnt] = float(data_list[para_index['pwm1']])
        pwm2[cnt] = float(data_list[para_index['pwm2']])
        cnt += 1    
        if cnt % 100 == 0:
            print("waiting ... ", cnt)
    except Exception as e:
        print(data_list)
        # print(e)

save2pickle(DATA_DIR+'/i1.pickle', i1*3.3/65535/0.15)
save2pickle(DATA_DIR+'/i2.pickle', i2*3.3/65535/0.15)
save2pickle(DATA_DIR+'/angle.pickle', angle)
save2pickle(DATA_DIR+'/sele_dangle.pickle', sele_dangle)
save2pickle(DATA_DIR+'/cele_dangle.pickle', cele_dangle)
save2pickle(DATA_DIR+'/th_svpwm.pickle', th_svpwm)
save2pickle(DATA_DIR+'/i_svpwm.pickle', i_svpwm)
save2pickle(DATA_DIR+'/th_er.pickle', th_er)
save2pickle(DATA_DIR+'/th_cum.pickle', th_cum)
save2pickle(DATA_DIR+'/pwm1.pickle', pwm1)
save2pickle(DATA_DIR+'/pwm2.pickle', pwm2)

ser.close()




