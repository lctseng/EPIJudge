#include "test_framework/generic_test.h"
#include <unordered_set>
#include <vector>
using std::unordered_set;
using std::vector;

int LongestContainedRange(const vector<int> &A) {
  unordered_set<int> nums(A.begin(), A.end());
  int maxStreak = 1;
  for (int n : A) {
    // check n is a head: no n-1 exists
    if (!nums.count(n - 1)) {
      // count current streak
      int y = 1;
      while (nums.count(n + y)) {
        y++;
      }
      if (y > maxStreak)
        maxStreak = y;
    }
  }
  return maxStreak;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "longest_contained_interval.cc", "longest_contained_interval.tsv",
      &LongestContainedRange, DefaultComparator{}, param_names);
}
