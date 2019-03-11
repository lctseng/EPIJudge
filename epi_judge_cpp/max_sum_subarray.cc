#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::vector;

int FindMaximumSubarray(const vector<int> &A) {
  int maxNonWrap = 0;
  int curSum = 0;
  for (int val : A) {
    curSum = max(curSum + val, val);
    maxNonWrap = max(curSum, maxNonWrap);
  }
  return maxNonWrap;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "max_sum_subarray.cc", "max_sum_subarray.tsv",
                         &FindMaximumSubarray, DefaultComparator{},
                         param_names);
}
