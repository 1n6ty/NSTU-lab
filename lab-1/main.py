s = [*input()]

left_ind, right_ind = 0, len(s) - 1

while(left_ind < right_ind):
    if s[left_ind] == 'b' and s[right_ind] == 'a':
        s[left_ind], s[right_ind] = s[right_ind], s[left_ind]

    if s[left_ind] != 'b':
        left_ind += 1
    if s[right_ind] != 'a':
        right_ind -= 1

print(''.join(s))