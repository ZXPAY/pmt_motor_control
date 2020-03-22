import matplotlib.pyplot as plt
import numpy as np
from mypickle import save2pickle, load_pickle


i1_max = load_pickle("i1_max.pickle")
i2_max = load_pickle("i2_max.pickle")


print("max i1: ", np.max(i1_max))
print("max i2: ", np.max(i2_max))
print("mean i1: ", np.mean(i1_max))
print("mean i2: ", np.mean(i2_max))



plt.plot(i1_max)
plt.plot(i2_max)
plt.title("max current", fontsize=28)
plt.grid(True)
plt.legend(["I1", "I2"], fontsize=16)
plt.xticks(fontsize=20)
plt.yticks(fontsize=20)
plt.ylabel("A", fontsize=24)
plt.xlabel("t", fontsize=24)
plt.show()



