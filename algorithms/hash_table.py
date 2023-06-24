NIL = 0
DELETED = -1

class HashSet:
    def __init__(self, cap: int):
        if cap <= 0:
            raise ValueError("Cap must be positive")

        self.t = [NIL] * cap
        self.cap = cap

    def hash_func(self, n: int, i: int) -> int:
        return (n + i) % self.cap

    def insert(self, key: int) -> bool:
        if key <= 0:
            raise ValueError("Key must be positive")
        for i in range(0, self.cap-1):
            h = self.hash_func(key, i);
            if self.t[h] == key:
                return True
            elif self.t[h] == NIL or self.t[h] == DELETED:
                self.t[h] = key
                return True
        return False
    
    def delete(self, key: int) -> bool:
        if key <= 0:
            raise ValueError("Key must be positive")
        for i in range(0, self.cap-1):
            h = self.hash_func(key, i);
            if self.t[h] == key:
                self.t[h] = DELETED
                return True
            elif self.t[h] == NIL:
                return False

    def print(self) -> None:
        for i in range(0, self.cap):
            print(f"{i}: {self.t[i]}")

hs = HashSet(int(input("Input hash table size: ")))
while True:
    action = input("Input action (ins/del/pr): ")
    if action == "ins":
        key = int(input("Input key to insert: "))
        print("inserted" if hs.insert(key) else "table full")
    elif action == "del":
        key = int(input("Input key to delete: "))
        print("deleted" if hs.delete(key) else "not in table")
    elif action == "pr":
        hs.print()
    else:
        print("Terminating")
        break

