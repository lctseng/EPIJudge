#include "test_framework/generic_test.h"
#include <queue>
#include <vector>
using std::greater;
using std::priority_queue;
using std::vector;

vector<int> KLargestInBinaryHeap(const vector<int> &A, int k) {
  priority_queue<int, vector<int>, greater<int>> minHeap;
  // insert k  values
  for (int i = 0; i < k; i++) {
    minHeap.push(A[i]);
  }
  for (int i = k; i < A.size(); i++) {
    // insert next value
    if (A[i] > minHeap.top()) {
      minHeap.push(A[i]);
      minHeap.pop();
    }
  }
  // dump the pq
  vector<int> res(minHeap.size());
  for (int i = res.size() - 1; i >= 0; i--) {
    res[i] = minHeap.top();
    minHeap.pop();
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(args, "k_largest_in_heap.cc", "k_largest_in_heap.tsv",
                         &KLargestInBinaryHeap,
                         &UnorderedComparator<std::vector<int>>, param_names);
}
