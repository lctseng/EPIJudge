#include "test_framework/generic_test.h"
#include <set>
#include <vector>
using std::greater;
using std::multiset;
using std::nth_element;
using std::prev;
using std::vector;

int FindKthLargestUnknownLength(vector<int>::const_iterator stream_begin,
                                const vector<int>::const_iterator &stream_end,
                                int k) {
  // keep currently kth largest in the array
  vector<int> buffer;
  while (stream_begin != stream_end) {
    buffer.push_back(*(stream_begin++));
    if (buffer.size() == 2 * k) {
      nth_element(buffer.begin(), buffer.begin() + (k - 1), buffer.end(),
                  greater<int>());
      buffer.resize(k);
    }
  }
  // last update
  nth_element(buffer.begin(), buffer.begin() + (k - 1), buffer.end(),
              greater<int>());
  return buffer[k - 1];
}

int FindKthLargestUnknownLengthHeap(
    vector<int>::const_iterator stream_begin,
    const vector<int>::const_iterator &stream_end, int k) {
  multiset<int> candidates;
  while (stream_begin != stream_end) {
    int cur = *(stream_begin++);
    // ensure first K element in the heap
    // or it must larger than existing minumun
    if (candidates.size() + 1 <= k || *candidates.begin() < cur) {
      candidates.insert(cur);
    }
    if (candidates.size() > k) {
      candidates.erase(candidates.begin());
    }
  }
  return *candidates.begin();
}
int FindKthLargestUnknownLengthWrapper(const vector<int> &stream, int k) {
  return FindKthLargestUnknownLength(stream.cbegin(), stream.cend(), k);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"stream", "k"};
  return GenericTestMain(args, "kth_largest_element_in_long_array.cc",
                         "kth_largest_element_in_long_array.tsv",
                         &FindKthLargestUnknownLengthWrapper,
                         DefaultComparator{}, param_names);
}
