def is_prime_helper(n, div):
    if div * div > n:
        return True  # число простое
    if n % div == 0:
        return False  # число составное
    return is_prime_helper(n, div + 1)

def is_prime(n):
    if n <= 1:
        return False
    return is_prime_helper(n, 2)

n = int(input("Введите натуральное число больше 1: "))

if is_prime(n):
    print("YES")
else:
    print("NO")