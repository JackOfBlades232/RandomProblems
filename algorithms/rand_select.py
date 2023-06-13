import random
from copy import deepcopy

def rand_partition(arr, l, r):
    i = random.randint(l, r-1)
    arr[r-1], arr[i] = arr[i], arr[r-1]
    x = arr[r-1]
    i = l-1
    for k in range(l, r-1):
        if arr[k] < x:
            i += 1
            arr[i], arr[k] = arr[k], arr[i]
        elif arr[k] == x:
            i += 1
            arr[i], arr[k] = arr[k], arr[i]
    arr[i+1], arr[r-1] = arr[r-1], arr[i+1]
    return i+1

def rand_select(arr, i):
    l = 0
    r = len(arr)
    arr_c = deepcopy(arr)
    while l < r-1:
        q = rand_partition(arr_c, l, r)
        if i == q:
            return arr_c[q]
        elif i < q:
            r = q
        else:
            l = q+1
    return arr_c[l]

arr = list(map(int, input().split()))
i = int(input())
print(rand_select(arr, i))
