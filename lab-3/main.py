def is_prime(n):
    for i in range(2, int(n**0.5)+1):
        if n % i == 0: return False
    return True

n = int(input("Введите натуральное число больше 1: "))

if n <= 1:
    print("Введено некорректное значение")
else:
    if is_prime(n):
        print("YES")
    else:
        print("NO")