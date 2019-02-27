#include "queue"
#include "test_framework/generic_test.h"
#include <vector>
using std::priority_queue;
using std::vector;

struct Entry {
  int value;
  int from;
  int index;
};

bool operator>(const Entry &a, const Entry &b) { return a.value > b.value; }

vector<int> MergeSortedArrays(const vector<vector<int>> &sorted_arrays) {
  priority_queue<Entry, vector<Entry>, std::greater<Entry>> pq;
  vector<int> res;
  // push init entries
  for (int i = 0; i < sorted_arrays.size(); i++) {
    pq.push({sorted_arrays[i][0], i, 0});
  }
  while (pq.size()) {
    Entry ent = pq.top();
    pq.pop();
    res.push_back(ent.value);
    if (ent.index < sorted_arrays[ent.from].size() - 1) {
      // push next
      pq.push(
          {sorted_arrays[ent.from][ent.index + 1], ent.from, ent.index + 1});
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"sorted_arrays"};
  return GenericTestMain(args, "sorted_arrays_merge.cc",
                         "sorted_arrays_merge.tsv", &MergeSortedArrays,
                         DefaultComparator{}, param_names);
}
