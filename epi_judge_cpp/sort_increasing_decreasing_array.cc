#include "test_framework/generic_test.h"
#include <queue>
#include <vector>
using std::greater;
using std::priority_queue;
using std::vector;

struct Entry {
  int value;
  int index;
  int lastIndex; // last valid index
  int step;      // +1 or -1
};

bool operator>(const Entry &a, const Entry &b) { return a.value > b.value; }

vector<int> SortKIncreasingDecreasingArray(const vector<int> &A) {
  // scan for endpoint
  priority_queue<Entry, vector<Entry>, greater<Entry>> pq;
  bool incr = true;
  int start = 0;
  for (int i = 0; i < A.size(); i++) {
    // end of slop: at the array end, or dir mismatch
    if ((i == A.size() - 1) || (incr && A[i] > A[i + 1]) ||
        (!incr && A[i] < A[i + 1])) {
      if (start <= i) {
        if (incr) {
          pq.push({A[start], start, i, 1});
        } else {
          pq.push({A[i], i, start, -1});
        }
      }
      start = i + 1;
      incr = !incr;
    }
  }
  vector<int> res;
  while (pq.size()) {
    Entry ent = pq.top();
    pq.pop();
    res.push_back(ent.value);
    if (ent.index != ent.lastIndex) {
      // we can still advance
      int nextIndex = ent.index + ent.step;
      pq.push({A[nextIndex], nextIndex, ent.lastIndex, ent.step});
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "sort_increasing_decreasing_array.cc",
                         "sort_increasing_decreasing_array.tsv",
                         &SortKIncreasingDecreasingArray, DefaultComparator{},
                         param_names);
}
