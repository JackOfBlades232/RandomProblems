from ctypes import ArgumentError

def parent(i):
    return (i+1) // 2 - 1

def left(i):
    return (i+1) * 2 - 1

def right(i):
    return (i+1) * 2

def min_heapify(arr, heap_size, i):
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

def heap_minimum(arr, heap_size):
    if heap_size < 1:
        raise ArgumentError()
    return arr[0]

def heap_extract_min(arr, heap_size):
    min = heap_minimum(arr, heap_size)
    heap_size -= 1
    arr[0] = arr[heap_size]
    min_heapify(arr, heap_size, 0)
    return min, heap_size

def heap_decrease_key(arr, heap_size, i, new_val):
    if heap_size <= i or new_val > arr[i]:
        raise ArgumentError();
    arr[i] = new_val
    while i > 0 and arr[i] < arr[parent(i)]:
        arr[i], arr[parent(i)] = arr[parent(i)], arr[i]
        i = parent(i)

def min_heap_insert(arr, heap_size, new_val):
    arr[heap_size] = new_val
    heap_size += 1
    heap_decrease_key(arr, heap_size, heap_size-1, new_val)
    return heap_size

def min_heap_delete(arr, heap_size, i):
    if heap_size < 1:
        raise ArgumentError()
    heap_decrease_key(arr, heap_size, i, arr[0]-1)
    _, heap_size = heap_extract_min(arr, heap_size)
    return heap_size

arr = [0] * 4096
heap_size = 0

while True:
    print("Input command (min/popmin/dec/ins/del/print):", end=" ")
    cmd = input()
    if cmd == "min":
        print(heap_minimum(arr, heap_size))
    elif cmd == "popmin":
        min, heap_size = heap_extract_min(arr, heap_size)
        print(min)
    elif cmd == "dec":
        print("Input index:", end=" ")
        idx = int(input())
        print("Input new val:", end=" ")
        new_val = int(input())
        heap_decrease_key(arr, heap_size, idx, new_val)
    elif cmd == "ins":
        if heap_size >= len(arr):
            arr += [0] * 4096
        print("Input new val:", end=" ")
        new_val = int(input())
        heap_size = min_heap_insert(arr, heap_size, new_val)
    elif cmd == "del":
        print("Input index:", end=" ")
        idx = int(input())
        heap_size = min_heap_delete(arr, heap_size, idx)
    elif cmd == "print":
        print(arr[:heap_size])
    else:
        break
