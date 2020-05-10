import numpy as np
import os
from datetime import datetime
from mypickle import save2pickle, load_pickle
import matplotlib.pyplot as plt
import argparse

if __name__ == "__main__":
    delat_t = 1/10000
    FIG_DIR = "figure"
    now = datetime.now()
    default_file_marker = now.strftime("%m_%d_%H_%M_%S")
    default_len = 13
    parser = argparse.ArgumentParser()
    parser.add_argument("-mk", "--marker", help="Enter the marker about file, it will add behine the file", default=default_file_marker, type=str)
    parser.add_argument("-len", "--length", help="Comma \",\" split length, example \"1,2,3\" is three", default=default_len, type=int)
    args = parser.parse_args()

    file_marker = args.marker
    length = args.length
    para_index = {'title': 0, 'angle': 1}

    print("Handle data ...")
    f = open("data" + "/" + 'raw_data_' + file_marker + '.txt', "r")
    raw_data = f.readlines()
    f.close()
    data_length = len(raw_data)
    print("Data has ", data_length)

    # collect data
    angle = np.zeros([data_length, 1], dtype=np.float32)
    cnt = 0
    for data in raw_data:
        data_list = data.split(',')
        if len(data_list) == length:
            angle[cnt] = float(data_list[para_index['angle']])
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
    print("var: ", np.var(angle))
    print("std: ", np.std(angle))

    print("==================================")

    window = 8
    recur_num = 7  # feature = 3 + 1 = 4
    angle_recur = np.zeros([data_length-recur_num, recur_num+1], dtype=np.float64)
    angle_ans = np.ones([data_length-recur_num, 1], dtype=np.float64) * np.mean(angle)
    for i in range(recur_num, data_length):
        for r in range(recur_num+1):
            angle_recur[i-recur_num, r] = angle[i-recur_num+r]

    # Calculate parameters
    P = np.matmul(np.matmul(np.linalg.inv(np.matmul(angle_recur.T, angle_recur)), angle_recur.T), angle_ans)
    save2pickle("data/fir_" + file_marker + ".pickle", P)

    # Load parameters
    # P = load_pickle("data/fir_filter_8_2.pickle")

    print(P.shape)
    print(P)

    new_angle = np.matmul(angle_recur, P)

    try:
        os.mkdir(FIG_DIR+"/"+file_marker)
    except:
        pass

    plt.figure(figsize=(20,12))
    plt.plot(angle*360/16383)
    plt.plot(new_angle*360/16383)
    plt.plot(np.ones(angle.shape)*np.mean(angle)*360/16383, 'r')
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('degree', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('angle and after FIR filter angle', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20, position=(0,1), rotation="horizontal")
    plt.legend(['angle', 'FIR angle', 'mean'], fontsize=20)
    plt.savefig(FIG_DIR+"/"+file_marker + '/fir_angle_' + file_marker + '.png')

    print("===== new_angle =====")
    print("max value: ", np.max(new_angle))
    print("min value: ", np.min(new_angle))
    er_max = np.max(new_angle) - np.min(new_angle)
    print("er_max: ", er_max)
    print("er_max: ", er_max*360/16383, " degree")
    print("mean value: ", np.mean(new_angle))
    print("var: ", np.var(new_angle))
    print("std: ", np.std(new_angle))

    temp = "{"
    c = 0
    for feature in P:
        temp += str(feature[0])
        if c < recur_num:
            temp += ", "
        c += 1
    temp += "}"
    print(temp)

    try:
        os.mkdir("data/para")
    except:
        pass

    write_str = ""
    f = open("data/para/enc_fir_" + str(file_marker) + ".txt", "w")
    write_str += "========== angle =========="
    write_str += "max value: " + str(np.max(angle)) + "\n"
    write_str += "min value: " + str(np.min(angle)) + "\n"
    write_str += "mean value: " + str(np.mean(angle)) + "\n"
    write_str += "var value: " + str(np.var(angle)) + "\n"
    write_str += "std value: " + str(np.std(angle)) + "\n"
    write_str += "========== FIR angle =========="
    write_str += "max value: " + str(np.max(new_angle)) + "\n"
    write_str += "min value: " + str(np.min(new_angle)) + "\n"
    write_str += "er_max: " + str(er_max*360/16383) + " degree" + "\n"
    write_str += "mean value: " + str(np.mean(new_angle)) + "\n"
    write_str += "var value: " + str(np.var(new_angle)) + "\n"
    write_str += "std value: " + str(np.std(new_angle)) + "\n"
    write_str += "parameters: " + "\n"
    write_str += temp  
    f.write(write_str)
    f.close()

