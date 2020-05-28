import matplotlib.pyplot as plt
import os
import numpy as np

FIG_DIR = "figure"
RESULT_DIR = "result"
X_TICKS = ["N1", "N2", "N4", "N8", "N16", "N32"]

try:
    os.mkdir(FIG_DIR + "/" + RESULT_DIR)
except:
    pass

# Open loop
ol_n_eff = np.array([7.4, 24.59, 28.15, 26.69, 30.27, 30.67], dtype=np.float32)
ol_n_erstd = np.array([0.3329, 0.3318, 0.3348, 0.3316, 0.3295, 0.3315], dtype=np.float32)

# Close loop exitation angle
cl_n_eff = np.array([33.51, 30.07, 30.69, 31.88, 32.20, 32.58], dtype=np.float32)
cl_n_erstd = np.array([0.1238, 0.0806, 0.0485, 0.0311, 0.0257, 0.0233], dtype=np.float32)

# Close loop exitation angle and exitation current
cl_in_eff = np.array([55.71, 55.39, 56.28, 59.53, 80.87, 92.54], dtype=np.float32)
cl_in_erstd = np.array([0.1428, 0.0816, 0.0621, 0.0430, 0.0271, 0.0271], dtype=np.float32)

n_size = ol_n_eff.shape[0]
x_dot = np.linspace(0, n_size-1, n_size)


### ============================= Plot ============================= ##

plt.figure(figsize=(20, 12))
plt.plot(x_dot, ol_n_eff, linewidth=3)
plt.plot(x_dot, cl_n_eff, linewidth=3)
plt.plot(x_dot, cl_in_eff, linewidth=3)
plt.grid(True)
plt.xlabel('N_step', fontsize=24, position=(1,0))
plt.ylabel('%', fontsize=24, position=(0,1), rotation="horizontal")
plt.title('Efficiency', fontsize=28)
plt.xticks(x_dot, X_TICKS, fontsize=20)
plt.yticks(fontsize=20, position=(0,1), rotation="horizontal")
plt.legend(["Open loop", "Close loop (servo)", "Close loop (servo + current)"], fontsize=20)
plt.savefig(FIG_DIR + "/" + RESULT_DIR + '/efficiency' + '.png', dpi=300)


plt.figure(figsize=(20, 12))
plt.plot(x_dot, ol_n_erstd, linewidth=3)
plt.plot(x_dot, cl_n_erstd, linewidth=3)
plt.plot(x_dot, cl_in_erstd, linewidth=3)
plt.grid(True)
plt.xlabel('N_step', fontsize=24, position=(1,0))
plt.ylabel('angle (deg)', fontsize=24, position=(0,1.05), rotation="horizontal")
plt.title('Error STD (standard deviation)', fontsize=28)
plt.xticks(x_dot, X_TICKS, fontsize=20)
plt.yticks(fontsize=20, position=(0,1), rotation="horizontal")
plt.legend(["Open loop", "Close loop (servo)", "Close loop (servo + current)"], fontsize=20)
plt.savefig(FIG_DIR + "/" + RESULT_DIR + '/er_std' + '.png', dpi=300)


