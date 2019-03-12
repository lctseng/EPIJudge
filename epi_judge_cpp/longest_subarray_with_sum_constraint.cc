#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::vector;

int FindLongestSubarrayLessEqualK(const vector<int> &A, int k) {
  vector<int> prefix;
  prefix.push_back(0);
  for (int val : A) {
    prefix.push_back(prefix.back() + val);
  }
  // try every start point
  int maxValue = 0;
  for (int i = 0; i < A.size() - maxValue; i++) {
    // erase impossible i: prev num is  negative
    if (i > 0 && A[i - 1] <= 0)
      continue;
    for (int j = A.size() - 1; j >= i + maxValue; j--) {
      if (prefix[j + 1] - prefix[i] <= k) {
        maxValue = max(maxValue, j - i + 1);
        break;
      }
    }
  }
  return maxValue;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(args, "longest_subarray_with_sum_constraint.cc",
                         "longest_subarray_with_sum_constraint.tsv",
                         &FindLongestSubarrayLessEqualK, DefaultComparator{},
                         param_names);
}
