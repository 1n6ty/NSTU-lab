def is_prime(n, d=2):
    if d < int(n**0.5) + 1:
        return (n % d == 1) and is_prime(n, d + 1)
    else:
        return True

n = int(input("Введите натуральное число больше 1: "))

if n <= 1:
    print("Введено некорректное значение")
else:
    if is_prime(n):
        print("YES")
    else:
        print("NO")