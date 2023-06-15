class Queue:
    def __init__(self, cap: int):
        if cap <= 0:
            raise ValueError("Cap must be positive")

        self.q = [0] * cap
        self.cap = cap
        self.head = -1
        self.tail = 0

    def is_empty(self) -> bool:
        return self.head == -1

    def size(self) -> int:
        if self.is_empty():
            return 0
        elif self.tail > self.head:
            return self.tail - self.head
        else:
            return self.tail + (self.cap-self.head)

    def enqueue(self, val: int) -> None:
        if self.head == self.tail:
            raise ValueError("Queue full")

        self.q[self.tail] = val
        if self.head == -1:
            self.head = self.tail

        if self.tail == self.cap-1:
            self.tail = 0
        else:
            self.tail += 1

    def dequeue(self) -> int:
        if self.is_empty():
            raise ValueError("Queue empty")

        val = self.q[self.head]
        if self.head == self.cap-1:
            self.head = 0
        else:
            self.head += 1

        if self.head == self.tail:
            self.head = -1

        return val

    def print(self) -> None:
        if self.is_empty():
            print("Queue is empty", end='')
        elif self.tail > self.head:
            for i in range(self.head, self.tail):
                print(self.q[i], end=' ')
        else:
            for i in range(self.head, self.cap):
                print(self.q[i], end=' ')
            for i in range(0, self.tail):
                print(self.q[i], end=' ')
        print()


q = Queue(int(input("Input queue cap size: ")))
while True:
    action = input("Input action (enq/deq/siz/pr): ")
    if action == "enq":
        val = int(input("Input val to enqueue: "))
        q.enqueue(val)
    elif action == "deq":
        print(f"Dequeued val {q.dequeue()}")
    elif action == "siz":
        print(f"Size: {q.size()}")
    elif action == "pr":
        q.print()
    else:
        print("Terminating")
        break

