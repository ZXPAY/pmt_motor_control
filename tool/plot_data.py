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
    # s_cycles = save_data['s_cycles']
    # s_length = save_data['s_length']
    # c_total = save_data['c_total']

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
    plt.savefig(FIG_DIR+"/"+file_marker + '/angle_' + file_marker + '.png')

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
    plt.plot(t, th_er*1.8/90)
    plt.grid(True)
    plt.xlabel('t', fontsize=24, position=(1, 0))
    plt.ylabel('degree', fontsize=24, position=(0,1), rotation="horizontal")
    plt.title('theta error machanical angle', fontsize=28)
    plt.xticks(fontsize=20)
    plt.yticks(fontsize=20)
    fig.align_labels()
    plt.savefig(FIG_DIR+"/"+file_marker + '/scanglecum_er_' + file_marker + '.png')

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
    plt.savefig(FIG_DIR+"/"+file_marker + '/current_pwm_' + file_marker + '.png')

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
    plt.savefig(FIG_DIR+"/"+file_marker + '/current_power_' + file_marker + '.png')

    ### ==================================================================================== ###
    print("Data size is ", ia.shape[0])
    print("===== sensor ele angle =====")
    print("max: ", np.max(sele_dangle))
    print("min: ", np.min(sele_dangle))
    print("mean: ", np.mean(sele_dangle))

    print("===== command ele angle =====")
    print("max: ", np.max(cele_dangle))
    print("min: ", np.min(cele_dangle))
    print("mean: ", np.mean(cele_dangle))

    print("===== theta error =====")
    print("max: ", np.max(th_er))
    print("min: ", np.min(th_er))
    print("mean: ", np.mean(th_er))
    print("machanical angle res: ", np.max(th_er)*1.8/90, np.min(th_er)*1.8/90)
    print("N_STEP: ", 1.8/(np.max(th_er)*1.8/90), 1.8/(np.min(th_er)*1.8/90))
    print("mse er: ", np.mean(np.square(th_er)))

    print("===== theta cum =====")
    print("max: ", np.max(th_cum))
    print("min: ", np.min(th_cum))
    print("mean: ", np.mean(th_cum))

    print("===== consume power =====")
    print("power mean is ", np.mean(power))



