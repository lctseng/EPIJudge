#include "test_framework/generic_test.h"
#include <queue>
#include <vector>
using std::greater;
using std::priority_queue;
using std::vector;
vector<int>
SortApproximatelySortedData(vector<int>::const_iterator sequence_begin,
                            const vector<int>::const_iterator &sequence_end,
                            int k) {
  // maintain a pqueue with size k, after having K element, after the K + 1
  // insert, the top is the global mininal
  priority_queue<int, vector<int>, greater<int>> pq;
  vector<int> res;
  // push  k elements
  for (int i = 0; i < k && sequence_begin != sequence_end; i++) {
    pq.push(*(sequence_begin++));
  }
  // from until be meet the end, push 1 and pop 1
  while (sequence_begin != sequence_end) {
    pq.push(*(sequence_begin++));
    res.push_back(pq.top());
    pq.pop();
  }
  // finally, pop all remaining elements
  while (pq.size()) {
    res.push_back(pq.top());
    pq.pop();
  }
  return res;
}
vector<int> SortApproximatelySortedDataWrapper(const vector<int> &sequence,
                                               int k) {
  return SortApproximatelySortedData(cbegin(sequence), cend(sequence), k);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sequence", "k"};
  return GenericTestMain(
      args, "sort_almost_sorted_array.cc", "sort_almost_sorted_array.tsv",
      &SortApproximatelySortedDataWrapper, DefaultComparator{}, param_names);
}
