import json # To read json file of Petri network
import numpy as np # To manipulate matrixes

def get_destination_matrix(network: dict, transitions: list, points: list):
    dest = np.zeros((points_len, points_len), dtype=np.int32)

    # Fill destination matrix
    for trans in transitions:
        for inp_ind in [points.index(i[0]) for i in network[trans]["inp"]]:
            for out_ind in [points.index(i[0]) for i in network[trans]["out"]]:
                dest[inp_ind, out_ind] = 1
    
    return dest

def is_vital(network: dict, transitions: list):
    for trans in transitions:
        if not len([points.index(i[0]) for i in network[trans]["inp"]]):
            return False
    
    return True

def print_destination_tree(start_label: np.ndarray, D: np.ndarray, network: dict, transitions: list, points: list, steps: int, cur_step = 0, cur_trans = ''):
    if cur_step >= steps:
        return

    if cur_step > 0:
        print('|' * (cur_step - 1) + '∟', end='')
    print(cur_trans, start_label, sep='')

    points_to = set([l for l_ind, l in enumerate(points) if start_label[l_ind] != 0])
    for trans_ind, trans in enumerate(transitions):
        new_label = start_label + D[trans_ind]
        if all(new_label >= 0) and len(network[trans]["inp"]) > 0 and set([i[0] for i in network[trans]["inp"]]).issubset(points_to):
            print_destination_tree(new_label, D, network, transitions, points, steps, cur_step + 1, trans)

# Reading Petri network
with open('.\RGR\graph.json', 'r') as f:
    petri = json.load(f)

# Some Global variables
points = petri["points"]
transitions = petri["transitions"]

points_len, transitions_len = len(points), len(transitions)

tree_steps_c = 5
start_tree_label = [1, 1, 0, 0, 0]

# Task 1: Change matrix
# Fill part's matrixes
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

# Print Change matrix
D = D_plus - D_minus

print('D+:\n', D_plus, sep='')
print('D-:\n', D_minus, sep='')
print('D:\n', D, sep='')

# Task 2: Destination tree
# Print destianation tree for every label
start_tree_label = np.array(start_tree_label, dtype=np.int32)

print(f"Destination tree of steps: {tree_steps_c}:")
print_destination_tree(start_tree_label, D, petri["network"], transitions, points, tree_steps_c)

# Task 3: analysis
# Destination matrix
dest = get_destination_matrix(petri["network"], transitions, points)
print("Destination matrix:\n", dest, sep='')

# Vitality
print("Vitality: ", is_vital(petri["network"], transitions), sep='')