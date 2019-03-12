#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

// dp[c+1][d] = dp[c][d-1] + 1 + dp[c+1][d-1]
// dp[c][d] = dp[c-1][d-1] + 1 + dp[c][d-1]
int GetHeight(int cases, int drops) {
  vector<int> dp(cases + 1, 0); // d = 0
  for (int d = 1; d <= drops; d++) {
    for (int c = cases; c >= 1; c--) {
      dp[c] += dp[c - 1] + 1;
    }
  }
  return dp[cases];
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"cases", "drops"};
  return GenericTestMain(args, "max_safe_height.cc", "max_safe_height.tsv",
                         &GetHeight, DefaultComparator{}, param_names);
}
