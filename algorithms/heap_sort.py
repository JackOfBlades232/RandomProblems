def parent(i):
    return (i+1) // 2 - 1

def left(i):
    return (i+1) * 2 - 1

def right(i):
    return (i+1) * 2

def min_heapify_rec(arr, heap_size, i):
    l = left(i)
    r = right(i)
    if l < heap_size and arr[l] < arr[i]:
        smallest = l
    else:
        smallest = i
    if r < heap_size and arr[r] < arr[smallest]:
        smallest = r
    if smallest != i:
        arr[i], arr[smallest] = arr[smallest], arr[i]
        min_heapify_rec(arr, heap_size, smallest)

def min_heapify_iter(arr, heap_size, i):
    smallest = i + 1
    while smallest != i:
        l = left(i)
        r = right(i)
        if l < heap_size and arr[l] < arr[i]:
            smallest = l
        else:
            smallest = i
        if r < heap_size and arr[r] < arr[smallest]:
            smallest = r
        if smallest != i:
            arr[i], arr[smallest] = arr[smallest], arr[i]

def build_min_heap(arr):
    for i in range(len(arr)//2 - 1, -1, -1):
        min_heapify_rec(arr, len(arr), i)

def heap_sort_desc(arr):
    build_min_heap(arr)
    heap_size = len(arr)
    for i in range(len(arr)-1, 0, -1):
        arr[0], arr[i] = arr[i], arr[0]
        heap_size -= 1
        min_heapify_rec(arr, heap_size, 0)

print("Input arr:")
array = list(map(int, input().split()))
heap_sort_desc(array)
print(array)
