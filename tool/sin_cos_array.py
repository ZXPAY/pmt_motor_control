import numpy as np

DOTS = 12

a = np.linspace(0, DOTS-1, DOTS)


d_sin = np.sin(2*np.pi*a/DOTS)
d_cos = np.cos(2*np.pi*a/DOTS)

print(d_sin)
print(d_cos)

print(d_sin[0])
print(d_sin[3])
print(d_cos[0])




