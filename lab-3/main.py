def is_prime(n):
    answ = True
    i = 2
    # PreCondition
    print("n >= 0: ", n >= 0)
    print("n == 10: ", n == 10)
    print("n < 3: ", n < 3)
    print("i < 15: ", i < 15)
    print("n + i == 0: ", n + i == 0)
    print("n + i < 4: ", n + i < 4)
    print("n + i >= 0: ", n + i >= 0)
    print("n - i < 0: ", n - i < 0)
    print("i - n < 0: ", i - n < 0)
    print("answ >= 0: ", answ >= 0)
    print("answ <= 1: ", answ <= 1)
    print("i > 0: ", i > 0)
    print("i < int(n ** 0.5) + 1: ", i < int(n**0.5)+1)
    print("---------------------")
    for t in range(2, int(n**0.5)+1):
        i = t
        if n % t == 0:
            answ = False
            break
    #PostCondition
    print("n >= 0: ", n >= 0)
    print("n == 10: ", n == 10)
    print("n < 3: ", n < 3)
    print("i < 15: ", i < 15)
    print("n + i == 0: ", n + i == 0)
    print("n + i < 4: ", n + i < 4)
    print("n + i >= 0: ", n + i >= 0)
    print("n - i < 0: ", n - i < 0)
    print("i - n < 0: ", i - n < 0)
    print("answ >= 0: ", answ >= 0)
    print("answ <= 1: ", answ <= 1)
    print("i > 0: ", i > 0)
    print("i < int(n ** 0.5) + 1: ", i < int(n**0.5)+1)
    print(i, n)

    return answ

n = int(input("Введите натуральное число больше 1: "))

if n <= 1:
    print("Введено некорректное значение")
else:
    if is_prime(n):
        print("YES")
    else:
        print("NO")