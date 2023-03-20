def comp(x, y, asc):
    return x < y if asc else y < x; 

def partition(arr, l, r, asc):
    x = arr[r-1]
    i = l-1
    for k in range(l, r-1):
        if comp(arr[k], x, asc):
            i += 1
            arr[i], arr[k] = arr[k], arr[i]
        elif arr[k] == x:
            i += 1
            arr[i], arr[k] = arr[k], arr[i]
    arr[i+1], arr[r-1] = arr[r-1], arr[i+1]
    return i+1

def quicksort_step(arr, l, r, asc):
    if l < r-1:
        q = partition(arr, l, r, asc)
        quicksort_step(arr, l, q, asc)
        quicksort_step(arr, q, r, asc)

def quicksort(arr, asc=True):
    quicksort_step(arr, 0, len(arr), asc)

print("Asc? [y/n]", end=' ')
ans = input()
arr = list(map(int, input().split()))
quicksort(arr, asc=(ans != 'n'))
print(arr)
