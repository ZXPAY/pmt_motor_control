import numpy as np
from datetime import datetime
from mypickle import save2pickle, load_pickle
from plot_data import plot_data
import argparse

if __name__ == "__main__":
    delat_t = 0.01
    now = datetime.now()
    default_file_marker = now.strftime("%m_%d_%H_%M_%S")
    parser = argparse.ArgumentParser()
    parser.add_argument("-mk", "--marker", help="Enter the marker about file, it will add behine the file", default=default_file_marker, type=str)
    args = parser.parse_args()

    file_marker = args.marker

    # define PWM PERIOD COUNT
    PERIOD_COUNT = 1000
    para_index = {'title': 0, 'i1': 1, 'i2': 2, 'angle': 3, 'sele_dangle': 4, 'cele_dangle': 5, 'th_svpwm': 6,
                    'i_svpwm': 7, 'th_er': 8, 'th_cum': 9, 'pwm1': 10, 'pwm2': 11}

    print("Handle data ...")
    f = open("data" + "/" + 'raw_data_' + file_marker + '.txt', "r")
    raw_data = f.readlines()
    f.close()
    data_length = len(raw_data)
    print("Data has ", data_length)

    # collect data
    i1 = np.zeros([data_length, 1], dtype=np.float32)
    i2 = np.zeros([data_length, 1], dtype=np.float32)
    angle = np.zeros([data_length, 1], dtype=np.float32)
    sele_dangle = np.zeros([data_length, 1], dtype=np.float32)
    cele_dangle = np.zeros([data_length, 1], dtype=np.float32)
    th_svpwm = np.zeros([data_length, 1], dtype=np.float32)
    i_svpwm = np.zeros([data_length, 1], dtype=np.float32)
    th_er = np.zeros([data_length, 1], dtype=np.float32)
    th_cum = np.zeros([data_length, 1], dtype=np.float32)
    pwm1 = np.zeros([data_length, 1], dtype=np.float32)
    pwm2 = np.zeros([data_length, 1], dtype=np.float32)

    cnt = 0
    for data in raw_data:
        data_list = data.split(',')
        if len(data_list) == 13:
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
        else:
            print("Warning !!! List size not match !!!, ", cnt)
            print(data_list)

    # Change PWM to duty 0~1
    pwm1 = pwm1 / PERIOD_COUNT
    pwm2 = pwm2 / PERIOD_COUNT

    # Handle current positive or negative by decide PWM duty
    # 20 is gain (AD8206), 0.1 is current sense resistor
    i1 = (3.3 * i1 / 65535.0 - 1.65) / 20 / 0.1
    i2 = (3.3 * i2 / 65535.0 - 1.65) / 20 / 0.1

    print("Saving data to pickle file ...")
    save2pickle("data"+'/i1_'+file_marker+'.pickle', i1)
    save2pickle("data"+'/i2_'+file_marker+'.pickle', i2)
    save2pickle("data"+'/angle_'+file_marker+'.pickle', angle)
    save2pickle("data"+'/sele_dangle_'+file_marker+'.pickle', sele_dangle)
    save2pickle("data"+'/cele_dangle_'+file_marker+'.pickle', cele_dangle)
    save2pickle("data"+'/th_svpwm_'+file_marker+'.pickle', th_svpwm)
    save2pickle("data"+'/i_svpwm_'+file_marker+'.pickle', i_svpwm)
    save2pickle("data"+'/th_er_'+file_marker+'.pickle', th_er)
    save2pickle("data"+'/th_cum_'+file_marker+'.pickle', th_cum)
    save2pickle("data"+'/pwm1_'+file_marker+'.pickle', pwm1)
    save2pickle("data"+'/pwm2_'+file_marker+'.pickle', pwm2)

    print("Saving figure ...")
    plot_data(file_marker, delat_t)
