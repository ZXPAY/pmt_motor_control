import numpy as np
from mypickle import save2pickle, load_pickle

# define PWM PERIOD COUNT
PERIOD_COUNT = 2000
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

    # Change PWM to duty 0~1
    pwm1 = pwm1 / PERIOD_COUNT
    pwm2 = pwm2 / PERIOD_COUNT

    # Handle current positive or negative by decide PWM duty
    i1 = i1 * (((pwm1 < 0.5) * -1) + (pwm1 > 0.5))
    i2 = i2 * (((pwm2 < 0.5) * -1) + (pwm2 > 0.5))

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


if __name__ == "__main__":
    DATA_DIR = "data"
    pickle_last_nm = "_4"

    i1 = load_pickle(DATA_DIR+'/i1'+pickle_last_nm+'.pickle')
    i2 = load_pickle(DATA_DIR+'/i2'+pickle_last_nm+'.pickle')
    pwm1 = load_pickle(DATA_DIR+'/pwm1'+pickle_last_nm+'.pickle')
    pwm2 = load_pickle(DATA_DIR+'/pwm2'+pickle_last_nm+'.pickle')

    i1a = (((pwm1 < 0.5) * -1) + (pwm1 >= 0.5))
    i2a = (((pwm2 < 0.5) * -1) + (pwm2 >= 0.5))
    print(np.sum(i1a==0))

    import matplotlib.pyplot as plt
    plt.plot(i1*i1a)
    plt.plot(i2*i2a)
    plt.legend(['i1', 'i2'])
    # plt.plot(pwm1/np.max(pwm1))
    plt.show()

