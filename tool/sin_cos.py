import numpy as np
import matplotlib.pyplot as plt

kv = 8

n = np.linspace(0, 4*kv-1, 4*kv)
print(n)


Va = np.sin(n*np.pi / (2*kv))
Vc = np.cos(n*np.pi / (2*kv))

plt.figure()
plt.plot(Va)
plt.plot(Vc)
plt.plot(Va-Vc)
plt.grid(True)
plt.show()





