#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int NumberOfWaysToTop(int top, int maximum_step) {
  vector<int> dp(top + 1, 0);
  dp[0] = 1;
  for (int i = 1; i <= top; i++) {
    // go to i-th level
    for (int step = 1; step <= maximum_step; step++) {
      if (i - step < 0)
        break;
      dp[i] += dp[i - step];
    }
  }
  return dp[top];
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"top", "maximum_step"};
  return GenericTestMain(args, "number_of_traversals_staircase.cc",
                         "number_of_traversals_staircase.tsv",
                         &NumberOfWaysToTop, DefaultComparator{}, param_names);
}
