from random import randint

def comp(x, y, asc):
    return x < y if asc else y < x; 

def partition(arr, l, r, asc):
    r_ind = randint(l, r-1);
    arr[r_ind], arr[r-1] = arr[r-1], arr[r_ind]
    x = arr[r-1]
    i = l-1
    j = l-1
    for k in range(l, r-1):
        if comp(arr[k], x, asc):
            i += 1
            j += 1
            if i == j or j == k:
                arr[i], arr[k] = arr[k], arr[i]
            else:
                arr[i], arr[j], arr[k] = arr[k], arr[i], arr[j]
        elif arr[k] == x:
            j += 1
            arr[j], arr[k] = arr[k], arr[j]
    arr[j+1], arr[r-1] = arr[r-1], arr[j+1]
    return i+1, j+2

def quicksort_step(arr, l, r, asc):
    if l < r-1:
        q, t = partition(arr, l, r, asc)
        quicksort_step(arr, l, q, asc)
        quicksort_step(arr, t, r, asc)

def quicksort(arr, asc=True):
    quicksort_step(arr, 0, len(arr), asc)

print("Asc? [y/n]", end=' ')
ans = input()
arr = list(map(int, input().split()))
quicksort(arr, asc=(ans != 'n'))
print(arr)
