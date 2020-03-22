import pickle
import matplotlib.pyplot as plt
from mypickle import save2pickle, load_pickle

i1 = load_pickle('i1.pickle')
i2 = load_pickle('i2.pickle')
sign1 = load_pickle('sign1.pickle')
sign2 = load_pickle('sign2.pickle')
angle = load_pickle('angle.pickle')

plt.figure(3)
plt.plot(sign1)
plt.plot(sign2)
plt.show()

plt.figure(1)
plt.plot(i1*sign1)
plt.plot(i2*sign2)
plt.grid(True)
plt.legend(['i1', 'i2'])
plt.show()

plt.figure(2)
plt.plot(angle/16383*360)
plt.grid(True)
plt.show()

