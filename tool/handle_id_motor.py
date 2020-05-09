import numpy as np
from datetime import datetime
from mypickle import save2pickle, load_pickle
import matplotlib.pyplot as plt
import argparse

if __name__ == "__main__":
    step_data = 1200     # stack size lㄎDㄋimit
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
    index = np.zeros([data_length, 200], dtype=np.float64)
    raw_adc = np.zeros([data_length, 200], dtype=np.float64)
    angle = np.zeros([step_data, 200], dtype=np.float64)   # full step = 200

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

    test_all = np.zeros([step_data-1, 3], dtype=np.float64)
    i_real = (3.3 * raw_adc[0:step_data, 0] / 65535.0 - 1.65) / 20 / 0.1
    angle_mach = angle*360/16383

    angle_init = angle_mach[0,0]
    angle_first = (angle_mach[:,0] - angle_init)
    angle_first = angle_first.reshape(-1, 1)

    print("max ", np.max(angle_first), " min-> ", np.min(angle_first))
    
    # moving average
    mv_avg_window = 4
    for i in range(mv_avg_window-1, angle_first.shape[0]):
        angle_first[i, 0] = np.mean(angle_first[i-mv_avg_window+1:i+1, 0])
        
    print(angle_first.shape)

    for i in range(1, i_real.shape[0]):
        test_all[i-1, 0] = (i_real[i] - i_real[i-1]) / delat_t
        test_all[i-1, 1] = i_real[i-1]
        # test_all[i-1, 2] = ((angle_first[i] - angle_first[i-1]) / delat_t * np.pi / 180) * np.sin(angle_first[i] * np.pi / 180)
        test_all[i-1, 2] = 3 * np.sin(angle_first[i] * np.pi / 180)
    
    plt.plot(test_all[:, 2])
    plt.title("all")

    mv_avg_window = 5
    for i in range(mv_avg_window-1, angle_first.shape[0]-1):
        test_all[i, 2] = np.mean(test_all[i-mv_avg_window+1:i+1, 2])

    plt.plot(test_all[:, 2])
    plt.show()

    print("mean: ", np.mean(test_all[:, 2]))

    plt.plot(i_real)
    plt.title("current (A)")
    plt.show()

    Y = np.ones([step_data-1, 1], dtype=np.float64)*5

    th_para = np.matmul(np.matmul(np.linalg.inv(np.matmul(test_all.T, test_all)), test_all.T), Y)
    print(th_para)
    print(np.max(i_real))

    A_bar = np.matmul(test_all, th_para)
    plt.plot(A_bar)
    plt.plot(Y)
    plt.show()

    
    f = open("data/motor_para_" + str(file_marker) + ".txt", "w")
    write_str = "Data size: " + str(step_data) + "\n"
    write_str += "Inductance: " + str(th_para[0][0] * 1000) + " mH" + "\n"
    write_str += "Resistor: " + str(th_para[1][0]) + " ohms" + "\n"
    f.write(write_str)
    f.close()


