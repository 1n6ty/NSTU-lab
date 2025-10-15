import seaborn as sns
import matplotlib.pyplot as plt

import numpy as np

start_x = int(input("Start x: "))
end_x = int(input("End x: "))

with open('output.txt', 'r') as f:
    data = [float(i) for i in f.read().split(' ') if i]

sns.lineplot(y=data, x=np.linspace(start_x, end_x, len(data)))
plt.show()