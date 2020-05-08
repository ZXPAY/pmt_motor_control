import numpy as np
from datetime import datetime
from mypickle import save2pickle, load_pickle
import matplotlib.pyplot as plt
import argparse

if __name__ == "__main__":
    step_data = 1200     # stack size limit
    delat_t = 1/(50000)
    now = datetime.now()
    default_file_marker = now.strftime("%m_%d_%H_%M_%S")
    parser = argparse.ArgumentParser()
    parser.add_argument("-mk", "--marker", help="Enter the marker about file, it will add behine the file", default=default_file_marker, type=str)
    args = parser.parse_args()

    file_marker = args.marker
    factor_length = 5
    para_index = {'title': 0, 'index':1, 'adc':2, 'angle':3}

    print("Handle data ...")
    f = open("data" + "/" + 'raw_data_' + file_marker + '.txt', "r")
    raw_data = f.readlines()
    f.close()
    data_length = len(raw_data)
    print("Data has ", data_length)

    # collect data
    index = np.zeros([data_length, 200], dtype=np.float32)
    raw_adc = np.zeros([data_length, 200], dtype=np.float32)
    angle = np.zeros([step_data, 200], dtype=np.float32)   # full step = 200

    num = 0
    cnt = 0


    for data in raw_data:
        data_list = data.split(',')
        if len(data_list) == factor_length:
            temp_adc = float(data_list[para_index['adc']])
            temp_angle = float(data_list[para_index['angle']])
            raw_adc[cnt, num] = temp_adc
            angle[cnt, num] = temp_angle
            cnt += 1
            if cnt == step_data:
                cnt = 0
                num += 1
        else:
            print("Warning !!! List size not match !!!, ", cnt)
            print(data_list)

    i = -(3.3 * raw_adc / 65535.0 - 1.65) / 20 / 0.1

    t = np.linspace(0, step_data*(delat_t), step_data)

    # print("max value: ", np.max(ia))
    # print("min value: ", np.min(ia))

    test_all = np.zeros([step_data-1, 2], dtype=np.float32)



    i_real = (3.3 * raw_adc[0:step_data, 0] / 65535.0 - 1.65) / 20 / 0.1
    angle_mach = angle*360/16383
    for i in range(1, i_real.shape[0]):
        test_all[i-1, 0] = (i_real[i] - i_real[i-1]) / delat_t
        test_all[i-1, 1] = i_real[i-1]


    plt.plot(i_real)
    plt.title("current (A)")
    plt.show()

    print(angle_mach.shape)
    plt.plot(angle_mach[:,0])
    plt.title("angle (degree)")
    plt.show()

    Y = np.ones([step_data-1, 1], dtype=np.float32)*5

    th_para = np.matmul(np.matmul(np.linalg.inv(np.matmul(test_all.T, test_all)), test_all.T), Y)
    print(th_para)
    print(np.max(i_real))

    A_bar = np.matmul(test_all, th_para)
    plt.plot(A_bar)
    plt.plot(Y)
    plt.show()

    f = open("data/motor_id_phaseA_" + str(file_marker) + ".txt", "w")
    write_str = "Data size: " + str(step_data) + "\n"
    write_str += "Inductance: " + str(th_para[0][0] * 1000) + " mH" + "\n"
    write_str += "Resistor: " + str(th_para[1][0]) + " ohms" + "\n"
    f.write(write_str)
    f.close()


