def counting_sort(arr, k):
    counters = [0] * (k + 1)
    res = [0] * len(arr)
    for i in range(len(arr)):
        if arr[i] < 0 or arr[i] > k:
            raise ValueError()
        counters[arr[i]] += 1
    for i in range(1, k + 1):
        counters[i] += counters[i - 1]
    for i in range(len(arr) - 1, -1, -1):
        res[counters[arr[i]] - 1] = arr[i]
        counters[arr[i]] -= 1
    return res

print("Input max elem val: ", end="")
k = int(input())
print("Input arr: ", end="")
arr = list(map(int, input().split()))
print(counting_sort(arr, k))
