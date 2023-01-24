# algorigthm for POT square matrices
from copy import deepcopy
import numpy as np

def add_matrices(a, b):
    return a + b

def sub_matrices(a, b):
    return a - b

def get_submatrix(m, half_size, i, j):
    if i == 0 and j == 0:
        return m[:half_size, :half_size]
    if i == 0 and j == 1:
        return m[:half_size, half_size:]
    if i == 1 and j == 0:
        return m[half_size:, :half_size]
    if i == 1 and j == 1:
        return m[half_size:, half_size:]

def multiply_square_matrices(a, b):
    if len(a) == 1:
        return np.array([[ a[0][0] * b[0][0] ]])

    n = len(a)
    n_half = n // 2
    
    a_11 = get_submatrix(a, n_half, 0, 0)
    a_12 = get_submatrix(a, n_half, 0, 1)
    a_21 = get_submatrix(a, n_half, 1, 0)
    a_22 = get_submatrix(a, n_half, 1, 1)
    
    b_11 = get_submatrix(b, n_half, 0, 0)
    b_12 = get_submatrix(b, n_half, 0, 1)
    b_21 = get_submatrix(b, n_half, 1, 0)
    b_22 = get_submatrix(b, n_half, 1, 1)

    s_1 = sub_matrices(b_12, b_22)
    s_2 = add_matrices(a_11, a_12)
    s_3 = add_matrices(a_21, a_22)
    s_4 = sub_matrices(b_21, b_11)
    s_5 = add_matrices(a_11, a_22)
    s_6 = add_matrices(b_11, b_22)
    s_7 = sub_matrices(a_12, a_22)
    s_8 = add_matrices(b_21, b_22)
    s_9 = sub_matrices(a_11, a_21)
    s_10 = add_matrices(b_11, b_12)

    p_1 = multiply_square_matrices(a_11, s_1)
    p_2 = multiply_square_matrices(s_2, b_22)
    p_3 = multiply_square_matrices(s_3, b_11)
    p_4 = multiply_square_matrices(a_22, s_4)
    p_5 = multiply_square_matrices(s_5, s_6)
    p_6 = multiply_square_matrices(s_7, s_8)
    p_7 = multiply_square_matrices(s_9, s_10)

    c_11 = add_matrices(sub_matrices(add_matrices(p_5, p_4), p_2), p_6);
    c_12 = add_matrices(p_1, p_2)
    c_21 = add_matrices(p_3, p_4)
    c_22 = sub_matrices(sub_matrices(add_matrices(p_5, p_1), p_3), p_7);

    res = np.copy(a)
    res[:n_half, :n_half] = c_11
    res[:n_half, n_half:] = c_12
    res[n_half:, :n_half] = c_21
    res[n_half:, n_half:] = c_22

    return res

def is_pot(n):
    if n == 0:
        return True
    elif n < 0:
        return False

    while n > 1:
        if n % 2 == 1:
            return False
        n //= 2

    return True


print("Input n:")
n = int(input())
if not is_pot(n):
    print("n must be a power of 2")
    quit()

a = [[0] * n] * n
b = deepcopy(a)

print("Input first matrix:")
for i in range(n):
    a[i] = list(map(int, input().split()))
    if len(a[i]) != n:
        print("Matrix row must be of len n")
        quit()

print("Input second matrix:")
for i in range(n):
    b[i] = list(map(int, input().split()))
    if len(b[i]) != n:
        print("Matrix row must be of len n")
        quit()

c = multiply_square_matrices(np.array(a), np.array(b))
print("\nProduct: ")
for i in range(n):
    print(c[i])
