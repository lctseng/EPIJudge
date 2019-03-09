#include "test_framework/generic_test.h"
#include <unordered_set>
#include <vector>
using std::unordered_set;
using std::vector;

int LongestSubarrayWithDistinctEntries(const vector<int> &A) {
  // two pointer
  // j: end index of element that still meet the req, j is pass-over-1
  // that is, using A[j] will break the condition
  // i: smallest start index which meet the condition
  // keep a set of seen element
  int i = 0, j = 0;
  int maxValue = 0;
  unordered_set<int> seen;
  while (j < A.size()) {
    // advance J as possible
    // need check whether j is valid
    while (j < A.size() && !seen.count(A[j])) {
      // BE CAREFUL
      // update the seen!
      seen.insert(A[j]);
      ++j;
    }

    // record the length now
    int curLen = j - i;
    if (curLen > maxValue)
      maxValue = curLen;
    // break if j pass over the end,
    // since we only advance i when we need to consider next valid A[j]
    if (j >= A.size())
      break;
    // advance i and update seen, until find A[j]
    while (i < j && A[i] != A[j]) {
      // BE CAREFUL
      // update the seen!
      seen.erase(A[i]);
      ++i;
    }
    // A[i] now == A[j]
    // advance both i, j without changing the seen
    ++i;
    ++j;
    // even this reach the end, the max distance does not change
    // no need to do last update after the loop
  }
  // no last updated needed
  return maxValue;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "longest_subarray_with_distinct_values.cc",
                         "longest_subarray_with_distinct_values.tsv",
                         &LongestSubarrayWithDistinctEntries,
                         DefaultComparator{}, param_names);
}
