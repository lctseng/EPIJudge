#include "test_framework/generic_test.h"
#include <vector>
using std::swap;
using std::vector;

int NumberOfWays(int n, int m) {
  if (n < m)
    swap(n, m);
  // n >= m
  vector<int> dp(m + 1, 1); // answer for i = 1
  dp[0] = 0;                // m = 0
  for (int i = 2; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      dp[j] += dp[j - 1];
    }
  }
  return dp.back();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "m"};
  return GenericTestMain(args, "number_of_traversals_matrix.cc",
                         "number_of_traversals_matrix.tsv", &NumberOfWays,
                         DefaultComparator{}, param_names);
}
