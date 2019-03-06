#include "test_framework/generic_test.h"
#include <climits>
#include <vector>
using std::min;
using std::vector;

int MinimumPathWeight(const vector<vector<int>> &triangle) {
  if (triangle.empty())
    return 0;
  int sz = triangle.back().size();
  vector<int> dp0(sz, INT_MAX);
  vector<int> dp1(sz, INT_MAX);
  vector<int> *pPrev = &dp0;
  vector<int> *pCurr = &dp1;
  // fill level 0
  dp0[0] = triangle[0][0];
  // dp[i]: min weight advance from dp[i-1] + dp[i]
  for (int l = 1; l < size(triangle); l++) {
    auto &level = triangle[l];
    vector<int> &curr = *pCurr;
    vector<int> &prev = *pPrev;
    for (int i = 0; i < size(level); i++) {
      int leftMin = i > 0 ? prev[i - 1] : INT_MAX;
      int rightMin = i < size(level) - 1 ? prev[i] : INT_MAX;
      // Must have one side to be non-max
      curr[i] = min(leftMin, rightMin) + level[i];
    }
    swap(pPrev, pCurr);
  }
  vector<int> &last = *pPrev;
  int minValue = last[0];
  for (int val : last) {
    if (val < minValue)
      minValue = val;
  }
  return minValue;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"triangle"};
  return GenericTestMain(args, "minimum_weight_path_in_a_triangle.cc",
                         "minimum_weight_path_in_a_triangle.tsv",
                         &MinimumPathWeight, DefaultComparator{}, param_names);
}
