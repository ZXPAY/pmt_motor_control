import numpy as np
from datetime import datetime
from mypickle import save2pickle, load_pickle
import matplotlib.pyplot as plt
import argparse

if __name__ == "__main__":
    step_data = 1024
    delat_t = 4*10e-6
    now = datetime.now()
    default_file_marker = now.strftime("%m_%d_%H_%M_%S")
    default_len = 13
    parser = argparse.ArgumentParser()
    parser.add_argument("-mk", "--marker", help="Enter the marker about file, it will add behine the file", default=default_file_marker, type=str)
    parser.add_argument("-len", "--length", help="Comma \",\" split length, example \"1,2,3\" is three", default=default_len, type=int)
    args = parser.parse_args()

    file_marker = args.marker
    length = args.length
    para_index = {'title': 0, 'index':1, 'adc':2, 'angle':3}

    print("Handle data ...")
    f = open("data" + "/" + 'raw_data_' + file_marker + '.txt', "r")
    raw_data = f.readlines()
    f.close()
    data_length = len(raw_data)
    print("Data has ", data_length)

    # collect data
    index = np.zeros([data_length, 1], dtype=np.float32)
    raw_adc = np.zeros([data_length, 1], dtype=np.float32)
    adc_a = np.zeros([step_data, 100], dtype=np.float32)
    adc_b = np.zeros([step_data, 100], dtype=np.float32)
    angle_a = np.zeros([step_data, 100], dtype=np.float32)
    angle_b = np.zeros([step_data, 100], dtype=np.float32)

    cnt = 0
    cnta = 0
    cntb = 0
    cnt2 = 0
    cnt3 = 0
    tog_fg = 1

    for data in raw_data:
        data_list = data.split(',')
        if len(data_list) == length:
            temp_adc = float(data_list[para_index['adc']])
            temp_angle = float(data_list[para_index['angle']])
            raw_adc[cnt, 0] = temp_adc
            # 放棄第一筆
            if cnt2 > 0:
                if tog_fg==0:
                    adc_a[cnt3, cnta] = temp_adc
                    angle_a[cnt3, cnta] = temp_angle
                    if cnt3 == step_data - 1:
                        cnta += 1
                else:
                    adc_b[cnt3, cntb] = temp_adc
                    angle_b[cnt3, cntb] = temp_angle
                    if cnt3 == step_data - 1:
                        cntb += 1
                cnt3 += 1
            cnt += 1
            if cnt >= step_data*(1+cnt2):
                cnt2 += 1
                cnt3 = 0
                if tog_fg==1:
                    tog_fg = 0
                else:
                    tog_fg = 1
                # print(cnt2)
        else:
            print("Warning !!! List size not match !!!, ", cnt)
            print(data_list)

    ia = -(3.3 * adc_a / 65535.0 - 1.65 - 0.026) / 20 / 0.1
    ib = -(3.3 * adc_b / 65535.0 - 1.65 - 0.0266) / 20 / 0.1

    t = np.linspace(0, step_data*(delat_t), step_data)

    # print("max value: ", np.max(ia))
    # print("min value: ", np.min(ia))

    test_all = np.zeros([step_data-1, 2], dtype=np.float32)
    test = (3.3 * raw_adc[0:step_data, 0] / 65535.0 - 1.65 - 0.026) / 20 / 0.1
    for i in range(1, test.shape[0]):
        test_all[i-1, 0] = (test[i] - test[i-1]) / delat_t
        test_all[i-1, 1] = test[i-1]

    # test_all[:, 2] = -10*np.sin(np.linspace(0, 90, step_data-1)*np.pi/180)
    # test_all[:, 3] = 10*np.cos(np.linspace(0, 90, step_data-1)*np.pi/180)

    plt.plot(raw_adc)
    plt.show()

    plt.plot(test)
    plt.show()

    Y = np.ones([step_data-1, 1], dtype=np.float32)*5

    th_para = np.matmul(np.matmul(np.linalg.inv(np.matmul(test_all.T, test_all)), test_all.T), Y)
    print(th_para)
    print(np.max(test))

    A_bar = np.matmul(test_all, th_para)
    plt.plot(A_bar)
    plt.plot(Y)
    plt.show()
