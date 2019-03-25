#include "test_framework/generic_test.h"
#include <queue>
#include <vector>
using std::make_pair;
using std::pair;
using std::priority_queue;
using std::swap;
using std::vector;

vector<int> KLargestInBinaryHeap(const vector<int> &A, int k) {
  if (k == 0)
    return {};
  // A is already sorted in heap
  // assume A.size() >= k
  priority_queue<pair<int, int>> pq;
  pq.push(make_pair(A[0], 0));
  vector<int> res;
  while (k-- > 0) {
    auto current = pq.top();
    pq.pop();
    res.push_back(current.first);
    int leftChild = current.second * 2 + 1;
    int rightChild = current.second * 2 + 2;
    if (leftChild < A.size()) {
      pq.push(make_pair(A[leftChild], leftChild));
    }
    if (rightChild < A.size()) {
      pq.push(make_pair(A[rightChild], rightChild));
    }
  }
  return res;
}

// struct Entry {
//   int value;
//   int index;
// };

// bool operator<(const Entry &a, const Entry &b) { return a.value < b.value; }

// vector<int> KLargestInBinaryHeap(const vector<int> &A, int k) {
//   if (A.empty())
//     return {};
//   priority_queue<Entry> maxHeap;
//   // dont scan over the Array A
//   // A is already a max-heap
//   // find K values
//   vector<int> res;
//   maxHeap.push({A[0], 0});
//   int sz = A.size();
//   for (int i = 0; i < k; i++) {
//     auto parent = maxHeap.top();
//     maxHeap.pop();
//     res.push_back(parent.value);
//     // add two children to heap
//     // BE CAREFUL: boundary check
//     int candidate = parent.index * 2 + 1;
//     if (candidate < sz)
//       maxHeap.push({A[candidate], candidate});
//     candidate = parent.index * 2 + 2;
//     if (candidate < sz)
//       maxHeap.push({A[candidate], candidate});
//   }
//   return res;
// }

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(args, "k_largest_in_heap.cc", "k_largest_in_heap.tsv",
                         &KLargestInBinaryHeap,
                         &UnorderedComparator<std::vector<int>>, param_names);
}
