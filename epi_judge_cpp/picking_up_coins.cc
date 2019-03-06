#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::size;
using std::vector;

vector<vector<int>> dp;
vector<int> prefixSum;
// dp[j][i] : from coins[i:j], the max revenue for first pick player

int MaximumRevenueHelper(const vector<int> &coins, int first, int last) {
  if (first > last)
    return 0;
  int &dpEntry = dp[last][first];
  if (dpEntry >= 0)
    return dpEntry;
  // when pick first
  int totalGain = prefixSum[last + 1] - prefixSum[first];
  int pickFirstTheirGain = MaximumRevenueHelper(coins, first + 1, last);
  int pickFirstMyGain = totalGain - pickFirstTheirGain;
  int pickLastTheirGain = MaximumRevenueHelper(coins, first, last - 1);
  int pickLastMyGain = totalGain - pickLastTheirGain;
  return dpEntry = max(pickFirstMyGain, pickLastMyGain);
}

int MaximumRevenue(const vector<int> &coins) {
  if (coins.empty())
    return 0;
  prefixSum.clear();
  dp.assign(size(coins), {});
  for (int i = 0; i < size(coins); i++) {
    dp[i].assign(size(coins), -1); // -1 means not computed
  }
  // fill edge cases: i==j => coins[i]
  prefixSum.push_back(0);
  for (int i = 0; i < size(coins); i++) {
    dp[i][i] = coins[i];
    prefixSum.push_back(prefixSum.back() + coins[i]);
  }
  return MaximumRevenueHelper(coins, 0, size(coins) - 1);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"coins"};
  return GenericTestMain(args, "picking_up_coins.cc", "picking_up_coins.tsv",
                         &MaximumRevenue, DefaultComparator{}, param_names);
}
