import random

def random_search(arr, val):
    checked = set()
    while len(checked) < len(arr):
        i = random.randint(0, len(arr) - 1)
        while i in checked:
            i = random.randint(0, len(arr) - 1)
        
        if arr[i] == val:
            return i
        else:
            checked.add(i)

    return -1

arr = list(map(int, input().split()))
val = int(input())

print(random_search(arr, val))
