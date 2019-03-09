#include "test_framework/generic_test.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
using std::max;
using std::unordered_map;
using std::unordered_set;
using std::vector;

int LongestSubarrayWithDistinctEntries(const vector<int> &A) {
  // single pass
  // like two pointer, but use the hash map to fast forward i
  unordered_map<int, int> mostRecentIndex;
  int startIndex = 0;
  int maxValue = 0;
  for (int i = 0; i < A.size(); i++) {
    auto it = mostRecentIndex.find(A[i]);
    if (it != mostRecentIndex.end()) {
      // existed
      // BE CAREFUL!!
      // check the index still valid?
      // if already later than start index, that value treat as invalid
      if (it->second >= startIndex) {
        // compute len here
        maxValue = max(maxValue, i - startIndex);
        // move start index to next of the previous existing index
        startIndex = it->second + 1;
      }
      // update current entry's most recent index
      it->second = i;
    } else {
      // not appear, go forward!
      mostRecentIndex[A[i]] = i;
    }
  }
  // last update
  return max(maxValue, (int)A.size() - startIndex);
}

int LongestSubarrayWithDistinctEntriesTwoPointer(const vector<int> &A) {
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
