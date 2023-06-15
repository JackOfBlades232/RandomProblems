class Deque:
    def __init__(self, cap: int):
        if cap <= 0:
            raise ValueError("Cap must be positive")

        self.q = [0] * cap
        self.cap = cap
        self.head = -1
        self.tail = -1

    def is_empty(self) -> bool:
        return self.head == -1 and self.tail == -1

    def is_full(self) -> bool:
        return (self.head == 0 and self.tail == self.cap - 1) or \
                self.head == self.tail + 1

    def size(self) -> int:
        if self.is_empty():
            return 0
        elif self.tail >= self.head:
            return self.tail - self.head + 1
        else:
            return self.tail + (self.cap-self.head) + 1

    def push_front(self, val: int) -> None:
        if self.is_full():
            raise ValueError("Deque full")
        elif self.is_empty():
            self.head = 0
            self.tail = 0
        else:
            self.head -= 1
            while self.head < 0:
                self.head += self.cap

        self.q[self.head] = val
    
    def push_back(self, val: int) -> None:
        if self.is_full():
            raise ValueError("Deque full")
        elif self.is_empty():
            self.head = 0
            self.tail = 0
        else:
            self.tail += 1
            while self.tail >= self.cap:
                self.tail -= self.cap

        self.q[self.tail] = val

    def pop_front(self) -> int:
        if self.is_empty():
            raise ValueError("Deque empty")

        val = self.q[self.head]
        if self.head == self.tail:
            self.head = -1
            self.tail = -1
        elif self.head == self.cap-1:
            self.head = 0
        else:
            self.head += 1

        return val

    def pop_back(self) -> int:
        if self.is_empty():
            raise ValueError("Deque empty")

        val = self.q[self.tail]
        if self.head == self.tail:
            self.head = -1
            self.tail = -1
        elif self.tail == 0:
            self.tail = self.cap-1
        else:
            self.tail -= 1

        return val

    def print(self) -> None:
        if self.is_empty():
            print("Deque is empty", end='')
        elif self.tail >= self.head:
            for i in range(self.head, self.tail+1):
                print(self.q[i], end=' ')
        else:
            for i in range(self.head, self.cap):
                print(self.q[i], end=' ')
            for i in range(0, self.tail+1):
                print(self.q[i], end=' ')
        print()


q = Deque(int(input("Input deque cap size: ")))
while True:
    action = input("Input action (psf/psb/ppf/ppb/siz/pr): ")
    if action == "psf":
        val = int(input("Input val to push front: "))
        q.push_front(val)
    elif action == "psb":
        val = int(input("Input val to push back: "))
        q.push_back(val)
    elif action == "ppf":
        print(f"Popped from front: {q.pop_front()}")
    elif action == "ppb":
        print(f"Popped from back: {q.pop_back()}")
    elif action == "siz":
        print(f"Size: {q.size()}")
    elif action == "pr":
        q.print()
    else:
        print("Terminating")
        break

