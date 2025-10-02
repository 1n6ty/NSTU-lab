import seaborn as sns
import matplotlib.pyplot as plt

with open('output.txt', 'r') as f:
    sns.lineplot(data=[float(i) for i in f.read().split(' ') if i])
plt.show()