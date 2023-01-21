# algorigthm for POT square matrices
from copy import deepcopy

def add_matrices(a, b):
    c = deepcopy(a);
    for i in range(0, len(a)):
        for j in range(0, len(a)):
            c[i][j] = a[i][j] + b[i][j]
    return c

def sub_matrices(a, b):
    c = deepcopy(a);
    for i in range(0, len(a)):
        for j in range(0, len(a)):
            c[i][j] = a[i][j] - b[i][j]
    return c

def multiply_square_matrices(a, b):
    if len(a) == 1:
        return a[0][0] * b[0][0]

    n = len(a)
    n_half = n // 2

