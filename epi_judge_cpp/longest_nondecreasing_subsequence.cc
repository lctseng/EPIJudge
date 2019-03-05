#include "test_framework/generic_test.h"
#include <map>
#include <vector>

using std::map;
using std::vector;

int LongestNondecreasingSubsequenceLength(const vector<int> &A) {
  if (A.empty())
    return 0;
  vector<int> lengthToIndex(A.size() + 1, -1);
  // LNS length to minial ending value index
  map<int, int> valueToLength;
  // mininum ending value with its length
  // setup init condition
  lengthToIndex[1] = 0;
  valueToLength[A[0]] = 1;
  int maxLength = 1;
  for (int i = 1; i < A.size(); i++) {
    // find concat: prev of upper_bound
    // if upper_bound is begin, this is a 1-length
    auto upperIt = valueToLength.upper_bound(A[i]);
    int newLen = 1;
    if (upperIt != valueToLength.begin()) {
      --upperIt;
      // concat from this
      newLen = upperIt->second + 1;
    }
    // check replace
    if (lengthToIndex[newLen] == -1 || A[i] < A[lengthToIndex[newLen]]) {
      // erase old entry if exists
      if (lengthToIndex[newLen] >= 0) {
        valueToLength.erase(A[lengthToIndex[newLen]]);
      }
      lengthToIndex[newLen] = i;
      // BE CAREFUL!
      // release old position if needed
      auto oldIt = valueToLength.find(A[i]);
      if (oldIt != valueToLength.end()) {
        lengthToIndex[oldIt->second] = -1;
      }
      valueToLength[A[i]] = newLen;
      if (newLen > maxLength)
        maxLength = newLen;
    }
  }
  // find max
  return maxLength;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "longest_nondecreasing_subsequence.cc",
                         "longest_nondecreasing_subsequence.tsv",
                         &LongestNondecreasingSubsequenceLength,
                         DefaultComparator{}, param_names);
}
