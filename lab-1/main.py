s = [*input()]

left_ind, right_ind = 0, len(s) - 1 # lefthand and righthand indexes

while(left_ind < right_ind): # Check if indexes are crossed
    if s[left_ind] == 'b' and s[right_ind] == 'a': # Swap symbols if condition is met
        s[left_ind], s[right_ind] = s[right_ind], s[left_ind]

    if s[left_ind] != 'b': # Search for 'b' from left
        left_ind += 1
    if s[right_ind] != 'a': # Search for 'a' from right
        right_ind -= 1

print(''.join(s))