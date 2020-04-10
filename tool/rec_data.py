import serial
import os
import numpy as np
from datetime import datetime
from mypickle import save2pickle, load_pickle
import argparse

if __name__ == "__main__":
    now = datetime.now()
    default_file_marker = now.strftime("%m_%d_%H_%M_%S")
    default_len = 13

    parser = argparse.ArgumentParser()
    parser.add_argument("-sz", "--size", help="Receive data size you want", default=1000, type=int)
    parser.add_argument("-mk", "--marker", help="Enter the marker about file, it will add behine the file", default=default_file_marker, type=str)
    parser.add_argument("-len", "--length", help="Comma \",\" split length, example \"1,2,3\" is three", default=default_len, type=int)
    args = parser.parse_args()

    file_marker = args.marker
    GRAB_DATA_SIZE = args.size
    COMMA_DATA_LEN = args.length
    raw_data_file_name = 'raw_data_' + file_marker + '.txt'
    print('Write to file: ' + raw_data_file_name)
    print('Grab data size: ' + str(GRAB_DATA_SIZE))

    DATA_DIR = 'data'
    try:
        os.mkdir(DATA_DIR)
    except:
        pass
    ser = serial.Serial("COM6", 115200)


    cnt = 0
    temp_str = ''
    ser.flush()

    # Erase
    f = open(DATA_DIR + "/" + raw_data_file_name, "w")
    f.close()
    # Open
    f = open(DATA_DIR + "/" + raw_data_file_name, "ab")
    while cnt < (GRAB_DATA_SIZE):
        raw_data = ser.readline()
        # print(raw_data)
        try:
            if len(raw_data.decode("utf-8").split(",")) == COMMA_DATA_LEN:
                # print(raw_data)
                f.write(raw_data[0:-1])

                cnt += 1
                if cnt % 100 == 0:
                    print("waiting ... ", cnt)
        except Exception as e:
            print(raw_data)
            print(e)

    f.close()
    ser.close()



