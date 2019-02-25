#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
class Queue {
public:
  Queue(size_t _capacity)
      : capacity(_capacity + 1), begin(0), end(0), data(new int[capacity]) {}
  ~Queue() { delete[] data; }
  void Enqueue(int x) {
    if (Size() + 1 == capacity) {
      DoubleCapacity();
    }
    data[end] = x;
    end = (end + 1) % capacity;
    return;
  }
  int Dequeue() {
    int v = data[begin];
    begin = (begin + 1) % capacity;
    return v;
  }
  int Size() const {
    int diff = end - begin;
    if (diff < 0)
      diff += capacity;
    return diff;
  }

private:
  void DoubleCapacity() {
    int newCapacity = capacity * 2;
    int *newData = new int[newCapacity];
    // copy old data to new data
    int oldSize = Size();
    for (int i = 0; i < oldSize; i++) {
      newData[i] = data[(begin + i) % capacity];
    }
    delete[] data;
    begin = 0;
    end = oldSize;
    data = newData;
    capacity = newCapacity;
  }

  int capacity;
  int begin, end;
  int *data;
};
struct QueueOp {
  enum { kConstruct, kDequeue, kEnqueue, kSize } op;
  int argument;

  QueueOp(const std::string &op_string, int arg) : argument(arg) {
    if (op_string == "Queue") {
      op = kConstruct;
    } else if (op_string == "dequeue") {
      op = kDequeue;
    } else if (op_string == "enqueue") {
      op = kEnqueue;
    } else if (op_string == "size") {
      op = kSize;
    } else {
      throw std::runtime_error("Unsupported queue operation: " + op_string);
    }
  }

  void execute(Queue &q) const {
    switch (op) {
    case kConstruct:
      // Hack to bypass deleted assign operator
      q.~Queue();
      new (&q) Queue(argument);
      break;
    case kDequeue: {
      int result = q.Dequeue();
      if (result != argument) {
        throw TestFailure("Dequeue: expected " + std::to_string(argument) +
                          ", got " + std::to_string(result));
      }
    } break;
    case kEnqueue:
      q.Enqueue(argument);
      break;
    case kSize: {
      int s = q.Size();
      if (s != argument) {
        throw TestFailure("Size: expected " + std::to_string(argument) +
                          ", got " + std::to_string(s));
      }
    } break;
    }
  }
};

template <>
struct SerializationTraits<QueueOp> : UserSerTraits<QueueOp, std::string, int> {
};

void QueueTester(const std::vector<QueueOp> &ops) {
  Queue q(0);
  for (auto &op : ops) {
    op.execute(q);
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"ops"};
  return GenericTestMain(args, "circular_queue.cc", "circular_queue.tsv",
                         &QueueTester, DefaultComparator{}, param_names);
}
