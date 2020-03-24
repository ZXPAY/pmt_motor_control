import numpy as np
from mypickle import save2pickle, load_pickle

para_index = {'title': 0, 'i1': 1, 'i2': 2, 'angle': 3, 'sele_dangle': 4, 'cele_dangle': 5, 'th_svpwm': 6,
                'i_svpwm': 7, 'th_er': 8, 'th_cum': 9, 'pwm1': 10, 'pwm2': 11}

def handle_data(DATA_DIR, file_name, pickle_last_nm):
    f = open(DATA_DIR + "/" + file_name, "r")
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


    save2pickle(DATA_DIR+'/i1'+pickle_last_nm+'.pickle', i1*3.3/65535/0.15)
    save2pickle(DATA_DIR+'/i2'+pickle_last_nm+'.pickle', i2*3.3/65535/0.15)
    save2pickle(DATA_DIR+'/angle'+pickle_last_nm+'.pickle', angle)
    save2pickle(DATA_DIR+'/sele_dangle'+pickle_last_nm+'.pickle', sele_dangle)
    save2pickle(DATA_DIR+'/cele_dangle'+pickle_last_nm+'.pickle', cele_dangle)
    save2pickle(DATA_DIR+'/th_svpwm'+pickle_last_nm+'.pickle', th_svpwm)
    save2pickle(DATA_DIR+'/i_svpwm'+pickle_last_nm+'.pickle', i_svpwm)
    save2pickle(DATA_DIR+'/th_er'+pickle_last_nm+'.pickle', th_er)
    save2pickle(DATA_DIR+'/th_cum'+pickle_last_nm+'.pickle', th_cum)
    save2pickle(DATA_DIR+'/pwm1'+pickle_last_nm+'.pickle', pwm1)
    save2pickle(DATA_DIR+'/pwm2'+pickle_last_nm+'.pickle', pwm2)




