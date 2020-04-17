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
    para_index = {'title': 0, 'index':1, 'adc':2, 'angle':3}

    print("Handle data ...")
    f = open("data" + "/" + 'raw_data_' + file_marker + '.txt', "r")
    raw_data = f.readlines()
    f.close()
    data_length = len(raw_data)
    print("Data has ", data_length)

    # collect data
    index = np.zeros([data_length, 1], dtype=np.float32)
    adc = np.zeros([data_length, 1], dtype=np.float32)
    angle = np.zeros([data_length, 1], dtype=np.float32)

    cnt = 0
    for data in raw_data:
        data_list = data.split(',')
        if len(data_list) == length:
            adc[cnt] = float(data_list[para_index['adc']])
            angle[cnt] = float(data_list[para_index['angle']])
            cnt += 1
        else:
            print("Warning !!! List size not match !!!, ", cnt)
            print(data_list)

    ia = -(3.3 * adc / 65535.0 - 1.65) / 20 / 0.1

    print("max value: ", np.max(ia))
    print("min value: ", np.min(ia))

    plt.plot(angle)
    plt.show()

    plt.plot(ia)
    plt.show()
