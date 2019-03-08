#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::vector;

bool CanReachEnd(const vector<int> &max_advance_steps) {
  if (max_advance_steps.size() <= 1)
    return true;
  // maintin "max reachable"
  int i = 0;
  int maxReachable = 0;
  int target = max_advance_steps.size() - 1;
  while (i <= maxReachable) {
    // update max reachable according to current step
    maxReachable = max(maxReachable, i + max_advance_steps[i]);
    if (maxReachable >= target)
      return true;
    // advance
    i++;
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
