import numpy as np
import matplotlib.pyplot as plt

N = 10
MAX_SPEED = 3

add = np.array([3, 3, 3, 0, 5, 0 ,0 ,0 ,0 ,0], dtype=np.float32).reshape(-1, 1)
limit = np.zeros([N+1, 1], dtype=np.float32)
adj = np.zeros([N+1, 1], dtype=np.float32)

for i in range(1, N+1):
    adj[i] = add[i-1] + adj[i-1] - limit[i-1]
    if adj[i] > MAX_SPEED:
        limit[i] = MAX_SPEED
    else:
        limit[i] = adj[i]
print(add)
print(limit)
print(adj)

print("sum limit is ", np.sum(limit))

plt.plot(adj)
plt.plot(limit)
plt.legend(["adj", "limit"])
plt.show()


