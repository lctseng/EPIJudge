#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::min;
using std::size;
using std::vector;

vector<vector<int>> dp;
// dp[j][i] : from coins[i:j], the max revenue for first pick player

int MaximumRevenueHelper(const vector<int> &coins, int first, int last) {
  if (first > last)
    return 0;
  int &dpEntry = dp[last][first];
  if (dpEntry >= 0)
    return dpEntry;
  // when pick first
  // Revenue(first, last) = max(pickFirst, pickLast)
  // pickFirst = coins[first] + min[R(first+2, last), R(first + 1, last - 1)]
  // pickLast = coins[last] + min[R(first + 1, last - 1), R(first, last - 2)]
  return dpEntry = max(
             coins[first] +
                 min(MaximumRevenueHelper(coins, first + 2, last),
                     MaximumRevenueHelper(coins, first + 1, last - 1)),
             coins[last] + min(MaximumRevenueHelper(coins, first + 1, last - 1),
                               MaximumRevenueHelper(coins, first, last - 2)));
}

int MaximumRevenue(const vector<int> &coins) {
  if (coins.empty())
    return 0;
  dp.assign(size(coins), {});
  for (int i = 0; i < size(coins); i++) {
    dp[i].assign(size(coins), -1); // -1 means not computed
  }
  // fill edge cases: i==j => coins[i]
  for (int i = 0; i < size(coins); i++) {
    dp[i][i] = coins[i];
  }
  return MaximumRevenueHelper(coins, 0, size(coins) - 1);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"coins"};
  return GenericTestMain(args, "picking_up_coins.cc", "picking_up_coins.tsv",
                         &MaximumRevenue, DefaultComparator{}, param_names);
}
