def accepts(word):
    word = '0' + word + '0'
    alphabet = {'a', 'x'}
    count_a = 0
    count_x = 0

    r = len(word) // 2
    l = len(word) // 2 + 1

    while(word[r] != '0' or word[l] != '0'):
        if (word[r] or word[l]) not in alphabet:
            return False
        
        if word[r] == 'a':
            count_a += 1
        if word[l] == 'a':
            count_a += 1
        if word[r] == 'x':
            count_x += 1
        if word[l] == 'x':
            count_x += 1

        r -= 1 if r > 0 else 0
        l += 1 if l < len(word) - 1 else 0

    if count_a == count_x:
        return True
    else: 
        return False 


word_to_test = input("Введите слово для проверки автоматом: ")

if accepts(word_to_test):
    print(f"Слово '{word_to_test}' принято автоматом.")
else:
    print(f"Слово '{word_to_test}' не принято автоматом.")
