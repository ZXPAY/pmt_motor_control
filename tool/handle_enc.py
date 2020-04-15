import numpy as np
from datetime import datetime
from mypickle import save2pickle, load_pickle
import matplotlib.pyplot as plt
import argparse

if __name__ == "__main__":
    delat_t = 0.01
    now = datetime.now()
    default_file_marker = now.strftime("%m_%d_%H_%M_%S")
    default_len = 13
    parser = argparse.ArgumentParser()
    parser.add_argument("-mk", "--marker", help="Enter the marker about file, it will add behine the file", default=default_file_marker, type=str)
    parser.add_argument("-len", "--length", help="Comma \",\" split length, example \"1,2,3\" is three", default=default_len, type=int)
    args = parser.parse_args()

    file_marker = args.marker
    length = args.length
    para_index = {'title': 0, 'angle': 1, 'angle_com': 2, 'diagnostic': 3}

    print("Handle data ...")
    f = open("data" + "/" + 'raw_data_' + file_marker + '.txt', "r")
    raw_data = f.readlines()
    f.close()
    data_length = len(raw_data)
    print("Data has ", data_length)

    # collect data
    angle = np.zeros([data_length, 1], dtype=np.float32)
    angle_com = np.zeros([data_length, 1], dtype=np.float32)
    cnt = 0
    for data in raw_data:
        data_list = data.split(',')
        if len(data_list) == length:
            angle[cnt] = float(data_list[para_index['angle']])
            angle_com[cnt] = float(data_list[para_index['angle_com']])
            cnt += 1
        else:
            print("Warning !!! List size not match !!!, ", cnt)
            print(data_list)

    print("max value: ", np.max(angle))
    print("min value: ", np.min(angle))
    er_max = np.max(angle) - np.min(angle)
    print("er_max: ", er_max)
    print("er_max: ", er_max*360/16383, " degree")
    print("mean value: ", np.mean(angle))

    print("==================================")

    print("max value: ", np.max(angle_com))
    print("min value: ", np.min(angle_com))
    er_max = np.max(angle_com) - np.min(angle_com)
    print("er_max: ", er_max)
    print("er_max: ", er_max*360/16383, " degree")
    print("mean value: ", np.mean(angle_com))


    plt.plot(angle)
    plt.plot(angle_com)
    plt.show()
