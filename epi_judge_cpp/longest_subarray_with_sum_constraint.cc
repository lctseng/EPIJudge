#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::min;
using std::vector;

// Weird O(n) solution
int FindLongestSubarrayLessEqualK(const vector<int> &A, int k) {
  vector<int> prefix;
  prefix.push_back(0);
  for (int val : A) {
    prefix.push_back(prefix.back() + val);
  }
  vector<int> minPrefix(A.size() + 1);
  // minPrefix(i) = min(prefix(i), minPrefix(i+1)) for i < n
  // minPrefix(n) = prefix(n)
  minPrefix.back() = prefix.back();
  for (int i = A.size() - 1; i >= 0; i--) {
    minPrefix[i] = min(prefix[i], minPrefix[i + 1]);
  }
  // minPrefix(i): start from index 0, the min subarray sum using 'i' or beyond
  // as ending pointer (pass-over-1), called M(0, i)
  // minPrefix(i) - prefix(j): M(j, i)
  int a = 0, b = 1;
  int maxValue = 0;
  while (a < A.size() && b <= A.size()) {
    // try array [a..b]
    if (minPrefix[b] - prefix[a] <= k) {
      // we have candidate answer end with b (or beyond)
      // at least length is b - a
      if (b - a > maxValue)
        maxValue = b - a;
      // we can try larger b
      ++b;
    } else {
      // it is impossible to start with index a
      ++a;
      // no need to reset b when advancing a
      // since we are going to find the longest answer
      // the distance between a and be only increasing
      // reset b to a definitely cannot find better longer answer
    }
  }
  return maxValue;
}

// Worst case still O(n^2)
int FindLongestSubarrayLessEqualKHeuristic(const vector<int> &A, int k) {
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
