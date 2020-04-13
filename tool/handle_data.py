import numpy as np
from datetime import datetime
from mypickle import save2pickle, load_pickle
from plot_data import plot_data
import argparse

if __name__ == "__main__":
    delat_t = 0.01
    now = datetime.now()
    default_file_marker = now.strftime("%m_%d_%H_%M_%S")
    parser = argparse.ArgumentParser()
    parser.add_argument("-mk", "--marker", help="Enter the marker about file, it will add behine the file", default=default_file_marker, type=str)
    args = parser.parse_args()

    file_marker = args.marker

    # define PWM PERIOD COUNT
    PERIOD_COUNT = 900
    # define each phase resistor
    ra = 7.29
    rb = 7.08
    para_index = {'title': 0, 'ia': 1, 'ib': 2, 'angle': 3, 'sele_dangle': 4, 'cele_dangle': 5, 'th_svpwm': 6,
                    'i_svpwm': 7, 'th_er': 8, 'th_cum': 9, 'pwma': 10, 'pwmb': 11}

    print("Handle data ...")
    f = open("data" + "/" + 'raw_data_' + file_marker + '.txt', "r")
    raw_data = f.readlines()
    f.close()
    data_length = len(raw_data)
    print("Data has ", data_length)

    # collect data
    ia = np.zeros([data_length, 1], dtype=np.float32)
    ib = np.zeros([data_length, 1], dtype=np.float32)
    angle = np.zeros([data_length, 1], dtype=np.float32)
    sele_dangle = np.zeros([data_length, 1], dtype=np.float32)
    cele_dangle = np.zeros([data_length, 1], dtype=np.float32)
    th_svpwm = np.zeros([data_length, 1], dtype=np.float32)
    ki_cum = np.zeros([data_length, 1], dtype=np.float32)
    i_svpwm = np.zeros([data_length, 1], dtype=np.float32)
    th_er = np.zeros([data_length, 1], dtype=np.float32)
    th_cum = np.zeros([data_length, 1], dtype=np.float32)
    pwma = np.zeros([data_length, 1], dtype=np.float32)
    pwmb = np.zeros([data_length, 1], dtype=np.float32)
    # Accumulate sele_dangle and cele_dangle
    sele_dangle_cum = np.zeros([data_length, 1], dtype=np.float32)
    cele_dangle_cum = np.zeros([data_length, 1], dtype=np.float32)
    save_data = {}

    cnt = 0
    c1 = 0
    c2 = 0
    conf_cnt1 = 0
    conf_cnt2 = 0
    dc = 0
    dc_cnt = 0
    rpc_fg = 0
    ds = 0
    ds_cnt = 0
    rps_fg = 0
    for data in raw_data:
        data_list = data.split(',')
        if len(data_list) == 13:
            ia[cnt] = float(data_list[para_index['ia']])
            ib[cnt] = float(data_list[para_index['ib']])
            angle[cnt] = float(data_list[para_index['angle']])
            sele_dangle[cnt] = float(data_list[para_index['sele_dangle']])
            cele_dangle[cnt] = float(data_list[para_index['cele_dangle']])
            if cnt > 0:
                dc = cele_dangle[cnt] - cele_dangle[cnt-1]
                ds = sele_dangle[cnt] - sele_dangle[cnt-1]
            if abs(dc) > 180 and dc_cnt < 1:
                # boundary ripple
                if rp_fg == 0:
                    c1 -= 1
                    rpc_fg = 1
                else:
                    c1 += 1
                    rpc_fg = 0
                print("RippleC", cnt, cele_dangle[cnt], cele_dangle[cnt-1])
            elif abs(dc) > 180 and dc_cnt > 1:
                c1 += 1
                rpc_fg = 0
                dc_cnt = 0

            if abs(ds) > 180 and ds_cnt < 1:
                # boundary ripple
                if rps_fg == 0:
                    c2 -= 1
                    rps_fg = 1
                else:
                    c2 += 1
                    rps_fg = 0
                print("RippleS", cnt, sele_dangle[cnt], sele_dangle[cnt-1])
            elif abs(ds) > 180 and ds_cnt > 1:
                c2 += 1
                rps_fg = 0
                ds_cnt = 0
            dc_cnt += 1
            ds_cnt += 1
            sele_dangle_cum[cnt] = sele_dangle[cnt] + 360*c2
            cele_dangle_cum[cnt] = cele_dangle[cnt] + 360*c1
            th_svpwm[cnt] = float(data_list[para_index['th_svpwm']])
            temp = th_svpwm[cnt] - cele_dangle[cnt]
            if temp > 180:
                temp  -= 360
            elif temp < -180:
                temp += 360
            ki_cum[cnt] = temp
            i_svpwm[cnt] = float(data_list[para_index['i_svpwm']])
            th_er[cnt] = float(data_list[para_index['th_er']])
            th_cum[cnt] = float(data_list[para_index['th_cum']])
            pwma[cnt] = float(data_list[para_index['pwma']])
            pwmb[cnt] = float(data_list[para_index['pwmb']])
            cnt += 1
        else:
            print("Warning !!! List size not match !!!, ", cnt)
            print(data_list)

    # Change PWM to duty 0~1
    pwma = pwma / PERIOD_COUNT
    pwmb = pwmb / PERIOD_COUNT

    # Handle current positive or negative by decide PWM duty
    # 20 is gain (AD8206), 0.1 is current sense resistor
    ia = -(3.3 * ia / 65535.0 - 1.65) / 20 / 0.1
    ib = -(3.3 * ib / 65535.0 - 1.65) / 20 / 0.1

    # Calculate the power
    power = (ia*ia*ra) + (ib*ib*rb)

    save_data["ia"] = ia
    save_data["ib"] = ib
    save_data["angle"] = angle
    save_data["sele_dangle"] = sele_dangle
    save_data["cele_dangle"] = cele_dangle
    save_data["sele_dangle_cum"] = sele_dangle_cum
    save_data["cele_dangle_cum"] = cele_dangle_cum
    save_data["th_svpwm"] = th_svpwm
    save_data["i_svpwm"] = i_svpwm
    save_data["th_svpwm"] = th_svpwm
    save_data["ki_cum"] = ki_cum
    save_data["th_er"] = th_er
    save_data["th_cum"] = th_cum
    save_data["pwma"] = pwma
    save_data["pwmb"] = pwmb
    save_data["power"] = power

    print("Saving data to pickle file ...")
    save2pickle("data"+'/data_'+file_marker+'.pickle', save_data)

    print("Saving figure ...")
    plot_data(file_marker, delat_t)
