import pickle
import matplotlib.pyplot as plt
from mypickle import save2pickle, load_pickle
import os
import numpy as np

FIG_DIR = "figure"
def plot_data(file_marker, DELTA_T):
    try:
        os.mkdir(FIG_DIR)
    except:
        pass
    try:
        os.mkdir(FIG_DIR+"/"+file_marker)
    except:
        pass

    save_data = load_pickle("data/data_"+file_marker+'.pickle')

    ia = save_data["ia"]
    ib = save_data["ib"]
    angle = save_data["angle"]
    sele_dangle = save_data["sele_dangle"]
    cele_dangle = save_data["cele_dangle"]
    sangle_cum = save_data["sangle_cum"]
    cangle_cum = save_data["cangle_cum"]
    th_svpwm = save_data["th_svpwm"]
    ki_cum = save_data["ki_cum"]
    i_svpwm = save_data["i_svpwm"]
    th_er = save_data["th_er"]
    th_cum = save_data["th_cum"]
    pwma = save_data["pwma"]
    pwmb = save_data["pwmb"]
    power = save_data['power']

    # 轉換成度度量
    th_er = th_er*1.8/90

    t = np.linspace(0, ia.shape[0]-1, ia.shape[0]) * DELTA_T

    ### ==================================================================================== ###
    plt.figure(figsize=(20, 12))
    plt.plot(t, angle)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('angle coding', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('angle coding', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.savefig(FIG_DIR+"/"+file_marker + '/angle_' + file_marker + '.png', dpi=600)

    ### ==================================================================================== ###
    plt.figure(figsize=(20, 12))
    plt.plot(t, sele_dangle)
    plt.plot(t, cele_dangle)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('degree', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('command and sensor eletrical degree', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.legend(['sensor', 'command'], fontsize=20)
    plt.savefig(FIG_DIR+"/"+file_marker + '/scele_dangle_' + file_marker + '.png')

    ### ==================================================================================== ###
    fig = plt.figure(figsize=(20, 12))
    # first figure
    plt.subplot(211)
    plt.plot(t, sangle_cum)
    plt.plot(t, cangle_cum)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('degree', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('command and sensor machanical degree accumlation', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.legend(['sensor', 'command'], fontsize=20)
    # second figure
    plt.subplot(212)
    plt.plot(t, th_er)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1, 0))
    plt.ylabel('degree', fontsize=24, position=(0,1.05), rotation="horizontal")
    plt.title('theta error machanical angle', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    fig.align_labels()
    plt.savefig(FIG_DIR+"/"+file_marker + '/scanglecum_er_' + file_marker + '.png', dpi=600)

    ### ==================================================================================== ###

    plt.figure(figsize=(20,12))
    plt.plot(t, th_er)
    plt.grid(True)
    plt.xlabel('t', fontsize=26, position=(1, 0))
    plt.ylabel('degree', fontsize=26, position=(0,1.05), rotation="horizontal")
    plt.title('theta error machanical angle', fontsize=30)
    plt.xticks(fontsize=22)
    plt.yticks(fontsize=22)
    fig.align_labels()
    plt.savefig(FIG_DIR+"/"+file_marker + '/ther_' + file_marker + '.png')

    ### ==================================================================================== ###
    plt.figure(figsize=(20, 12))
    plt.plot(t, ki_cum)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('degree', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('th_svpwm minus command eletrical degree = ki*er_cum', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.savefig(FIG_DIR+"/"+file_marker + '/svpwm_cele_dangle_' + file_marker + '.png')

    ### ==================================================================================== ###
    err = th_svpwm-sele_dangle
    for i in range(err.shape[0]):
        if err[i] > 180:
            err[i] -= 360
        elif err[i] < -180:
            err[i] += 360
    plt.figure(figsize=(20, 12))
    plt.plot(t, err)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('degree', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('th_svpwm minus sele_dangle eletrical degree', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.savefig(FIG_DIR+"/"+file_marker + '/svpwm_s_' + file_marker + '.png')

    ### ==================================================================================== ###
    plt.figure(figsize=(20, 12))
    plt.plot(t, i_svpwm)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('A', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('i svpwm', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.savefig(FIG_DIR+"/"+file_marker + '/i_svpwm_' + file_marker + '.png')

    ### ==================================================================================== ###
    plt.figure(figsize=(20, 12))
    plt.plot(t, th_cum)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('degree', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('theta error accumulate', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20, position=(0,1), rotation="horizontal")
    plt.savefig(FIG_DIR+"/"+file_marker + '/th_cum_' + file_marker + '.png')

    ### ==================================================================================== ###
    fig = plt.figure(figsize=(20, 12))
    # first figure
    plt.subplot(211)
    plt.plot(t, pwma)
    plt.plot(t, pwmb)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('duty', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('Phase A B PWM', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20, position=(0,1), rotation="horizontal")
    plt.legend(['pwma', 'pwmb'], fontsize=20)
    # second figure
    plt.subplot(212)
    plt.plot(t, ia)
    plt.plot(t, ib)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('A', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('Phase A B current', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.legend(['ia', 'ib'], fontsize=20)
    fig.align_labels()
    plt.savefig(FIG_DIR+"/"+file_marker + '/current_pwm_' + file_marker + '.png', dpi=600)

    ### ==================================================================================== ###
    fig = plt.figure(figsize=(20, 12))
    # first figure
    plt.subplot(211)
    plt.plot(t, ia)
    plt.plot(t, ib)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('A', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('Phase A B current', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.legend(['ia', 'ib'], fontsize=20)
    # second figure
    plt.subplot(212)
    plt.plot(t, power)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('W', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('1A1B 2A2B Power', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    fig.align_labels()
    plt.savefig(FIG_DIR+"/"+file_marker + '/current_power_' + file_marker + '.png', dpi=600)

    ### ==================================================================================== ###
    report_str = ""
    report_str += "Data size is " +  str(ia.shape[0]) + "\n"
    report_str += "===== sensor ele angle =====" + "\n"
    report_str += "max: " + str(np.max(sele_dangle)) + "\n"
    report_str += "min: " + str(np.min(sele_dangle)) + "\n"
    report_str += "mean: " + str(np.mean(sele_dangle)) + "\n"
    report_str += "===== command ele angle =====" + "\n"
    report_str += "max: " + str(np.max(cele_dangle)) + "\n"
    report_str += "min: " + str(np.min(cele_dangle)) + "\n"
    report_str += "mean: " + str(np.mean(cele_dangle)) + "\n"
    report_str += "===== theta error (degree) =====" + "\n"
    report_str += "max: " + str(np.max(th_er)) + "\n"
    report_str += "min: " + str(np.min(th_er)) + "\n"
    report_str += "mean: " + str(np.mean(th_er)) + "\n"
    report_str += "std: " + str(np.std(th_er)) + "\n"
    report_str += "MSE: " + str(np.mean(np.square(th_er))) + "\n"
    report_str += "N_STEP: " + str(1.8/(np.max(th_er))) + ", " + str(1.8/(np.min(th_er))) + "\n"
    report_str += "===== consume power (watt) =====" + "\n"
    report_str += "max: " + str(np.max(power)) + "\n"
    report_str += "min: " + str(np.min(power)) + "\n"
    report_str += "mean: " + str(np.mean(power)) + "\n"
    report_str += "std: " + str(np.std(power)) + "\n"
    report_str += "sum: " + str(np.sum(np.abs(power))) + " J\n"

    print(report_str)

    f = open(FIG_DIR + "/" + file_marker + "/report_" + file_marker + ".txt", "w")
    f.write(report_str)
    f.close()

    ### ==================================================================================== ###
    ### ==================================================================================== ###
    ### ==================================================================================== ###
    ### Plot less data
    small_data_size = int(ia.shape[0] / 10)
    small_start = int(ia.shape[0] / 10)
    t_small = t[small_start:small_start+small_data_size]

    fig = plt.figure(figsize=(20, 12))
    # first figure
    plt.subplot(211)
    plt.plot(t_small, sangle_cum[small_start:small_start+small_data_size])
    plt.plot(t_small, cangle_cum[small_start:small_start+small_data_size])
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('degree', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('command and sensor machanical degree accumlation', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.legend(['sensor', 'command'], fontsize=20)
    # second figure
    plt.subplot(212)
    plt.plot(t_small, th_er[small_start:small_start+small_data_size])
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1, 0))
    plt.ylabel('degree', fontsize=24, position=(0,1.05), rotation="horizontal")
    plt.title('theta error machanical angle', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    fig.align_labels()
    plt.savefig(FIG_DIR+"/"+file_marker + '/sm_scanglecum_er_' + file_marker + '.png', dpi=600)


    fig = plt.figure(figsize=(20, 12))
    # first figure
    plt.subplot(211)
    plt.plot(t_small, pwma[small_start:small_start+small_data_size])
    plt.plot(t_small, pwmb[small_start:small_start+small_data_size])
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('duty', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('Phase A B PWM', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20, position=(0,1), rotation="horizontal")
    plt.legend(['pwma', 'pwmb'], fontsize=20)
    # second figure
    plt.subplot(212)
    plt.plot(t_small, ia[small_start:small_start+small_data_size])
    plt.plot(t_small, ib[small_start:small_start+small_data_size])
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('A', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('Phase A B current', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.legend(['ia', 'ib'], fontsize=20)
    fig.align_labels()
    plt.savefig(FIG_DIR+"/"+file_marker + '/sm_current_pwm_' + file_marker + '.png', dpi=600)

    fig = plt.figure(figsize=(20, 12))
    # first figure
    plt.subplot(211)
    plt.plot(t_small, ia[small_start:small_start+small_data_size])
    plt.plot(t_small, ib[small_start:small_start+small_data_size])
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('A', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('Phase A B current', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    plt.legend(['ia', 'ib'], fontsize=20)
    # second figure
    plt.subplot(212)
    plt.plot(t_small, power[small_start:small_start+small_data_size])
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1,0))
    plt.ylabel('W', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('1A1B 2A2B Power', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    fig.align_labels()
    plt.savefig(FIG_DIR+"/"+file_marker + '/sm_current_power_' + file_marker + '.png', dpi=600)
