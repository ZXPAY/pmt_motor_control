import serial
import os
import numpy as np
from datetime import datetime
from mypickle import save2pickle, load_pickle
from handle_data import handle_data
from plot_data import plot_data

delat_t = 0.01
now = datetime.now()
# date_time = now.strftime("%m_%d_%H_%M_%S")
date_time = "0"
raw_data_file_name = 'raw_data_' + date_time + '.txt'

GRAB_DATA_SIZE = 1000
DATA_DIR = 'data'
try:
    os.mkdir(DATA_DIR)
except:
    pass
ser = serial.Serial("COM21", 115200)


cnt = 0
temp_str = ''
ser.flush()

f = open(DATA_DIR + "/" + raw_data_file_name, "w")
f.close()
f = open(DATA_DIR + "/" + raw_data_file_name, "ab")
while cnt < (GRAB_DATA_SIZE):
    raw_data = ser.readline()
    if len(raw_data.decode("utf-8").split(",")) == 13:
        f.write(raw_data)

        cnt += 1
        if cnt % 100 == 0:
            print("waiting ... ", cnt)

f.close()
ser.close()


# Handle the data
print("Handle data ...")
handle_data(DATA_DIR, raw_data_file_name, date_time)
plot_data(DATA_DIR, date_time, delat_t)


