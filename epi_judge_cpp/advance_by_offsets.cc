#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::vector;

bool CanReachEnd(const vector<int> &max_advance_steps) {
  if (max_advance_steps.size() <= 1)
    return true;
  // maintin "max reachable"
  // BE CAREFUL
  // the ending condition and definition of 'maxReachable'
  int maxReachable = 0;
  int target = max_advance_steps.size() - 1;
  for (int i = 0; i <= maxReachable; i++) {
    maxReachable = max(maxReachable, i + max_advance_steps[i]);
    if (maxReachable >= target)
      return true;
  }
  return maxReachable >= target;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"max_advance_steps"};
  return GenericTestMain(args, "advance_by_offsets.cc",
                         "advance_by_offsets.tsv", &CanReachEnd,
                         DefaultComparator{}, param_names);
}
