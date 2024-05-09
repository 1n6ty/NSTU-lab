import json # To read json file of Petri network
import numpy as np # To manipulate matrixes

def print_destination_tree(start_label: np.ndarray, network: dict, transitions: list, points: list, steps: int, cur_step = 0, cur_trans = ''):
    if cur_step >= steps:
        return

    if cur_step > 0:
        print('|' * (cur_step - 1) + '∟', end='')
    print(cur_trans, start_label, sep='')

    points_to, labels_in = [l for l_ind, l in enumerate(points) if start_label[l_ind] != 0], [l for l in start_label if l != 0]
    for trans in transitions:
        cur_label = np.zeros(len(points), dtype=np.int32)
        for i in network[trans]["inp"]: # Count every transition entry if it is in start_label 
            if i[0] in points_to:
                i_ind = points_to.index(i[0])
                cur_label += np.array([(labels_in[i_ind] if i[0] == j else 0) for j in points], dtype=np.int32)

        cur_label_sum = np.sum(cur_label)
        if cur_label_sum > 0:
            out_label = np.zeros(len(points), dtype=np.int32) # Count every transition out
            for out in network[trans]["out"]:
                out_label += np.array([int(out[0] == j) for j in points], dtype=np.int32)
            # Recursively print out-labels 
            print_destination_tree(start_label - cur_label + out_label * cur_label_sum, network, transitions, points, steps, cur_step + 1, trans)

# Reading Petri network
with open('graph.json', 'r') as f:
    petri = json.load(f)

# Some Global variables
points = petri["points"]
transitions = petri["transitions"]

points_len, transitions_len = len(points), len(transitions)

tree_steps_c = 5
start_tree_label = [1, 0, 0, 0, 0]

# Task 1: Destination matrix
# Fill destination matrixes
D_minus, D_plus = np.zeros((transitions_len, points_len), dtype=np.int32), np.zeros((transitions_len, points_len), dtype=np.int32)

for trans_ind, trans in enumerate(transitions):
    # For input matrix
    for point in petri["network"][trans]["inp"]:
        point_ind = points.index(point[0])
        D_minus[trans_ind, point_ind] = 1
    # For output matrix
    for point in petri["network"][trans]["out"]:
        point_ind = points.index(point[0])
        D_plus[trans_ind, point_ind] = 1

# Print Destination matrix
print('D+:\n', D_plus)
print('D-:\n', D_minus)
print('D:\n', D_plus - D_minus)

# Task 2: Destination tree
# Print destianation tree for every label
start_tree_label = np.array(start_tree_label, dtype=np.int32)

print(f"Destination tree of steps: {tree_steps_c}:")
print_destination_tree(start_tree_label, petri["network"], transitions, points, tree_steps_c)
