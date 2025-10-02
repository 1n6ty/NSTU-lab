import seaborn as sns
import matplotlib.pyplot as plt

import math

with open('output.txt', 'r') as f:
    nums = [float(i) for i in f.read().split(' ') if i]
    sns.histplot(data=nums, bins=int(math.log(len(nums), 2)) + 1)
plt.show()