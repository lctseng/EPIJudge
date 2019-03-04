#include "test_framework/generic_test.h"
#include <unordered_set>
#include <vector>
using std::unordered_set;
using std::vector;

int LongestContainedRange(const vector<int> &A) {
  unordered_set<int> nums(A.begin(), A.end());
  int maxStreak = 0; // only 0 for empty array
  for (int n : A) {
    auto currentIt = nums.find(n);
    if (currentIt == nums.end())
      continue;
    nums.erase(currentIt); // prevent future searching
    int currentStreak = 1;
    // go up
    int currentNum = n;
    auto it = nums.begin();
    while ((it = nums.find(++currentNum)) != nums.end()) {
      currentStreak++;
      nums.erase(it);
    }
    // go down
    currentNum = n;
    while ((it = nums.find(--currentNum)) != nums.end()) {
      currentStreak++;
      nums.erase(it);
    }
    // update
    if (currentStreak > maxStreak)
      maxStreak = currentStreak;
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
