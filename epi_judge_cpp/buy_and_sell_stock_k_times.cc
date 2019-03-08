#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::min;
using std::vector;

double BuyAndSellStockKTimes(const vector<double> &prices, int k) {
  // edge case: empty
  if (k == 0 || prices.empty())
    return 0.0;
  if (k >= prices.size() / 2) {
    // simple sum
    double sum = 0;
    for (int i = 0; i < prices.size() - 1; i++) {
      double diff = prices[i + 1] - prices[i];
      if (diff > 0) {
        sum += diff;
      }
    }
    return sum;
  }
  // buy[i][j]: max profit end in buy in [0:i] days and max j times
  vector<double> buy(k + 1);
  vector<double> sell(k + 1);
  for (int i = 0; i <= k; i++) {
    buy[i] = -prices[0];
  }
  for (int i = 1; i < prices.size(); i++) {
    for (int j = 1; j <= k; j++) {
      double nextBuy = max(buy[j], sell[j - 1] - prices[i]);
      double nextSell = max(sell[j], buy[j] + prices[i]);
      buy[j] = nextBuy;
      sell[j] = nextSell;
    }
  }
  return sell[k];
}

double BuyAndSellStockKTimesPrecise(const vector<double> &prices, int k) {
  // edge case: empty
  if (k == 0 || prices.empty())
    return 0.0;
  // compute slops
  int slops = 0;
  int i = 0;
  double allSlopSum = 0;
  // printf("Prices: %lu\n", prices.size());
  while (i < prices.size()) {
    // find termination
    int j = i + 1; // j: end of slop (pass-over-1)
    while (j < prices.size() && prices[j - 1] <= prices[j])
      j++;
    if (j > i + 1) {
      allSlopSum += prices[j - 1] - prices[i];
      // printf("Slop: %lf -> %lf\n", prices[i], prices[j - 1]);
      slops++;
    }
    i = j;
  }
  if (k >= slops) {
    // simple case
    return allSlopSum;
  }
  // dp case
  vector<double> buy(k + 1, -prices[0]), sell(k + 1, 0);
  buy[0] = 0;
  for (int i = 1; i < prices.size(); i++) {
    for (int j = 1; j <= k; j++)
      if (prices[i - 1] >= prices[i] &&
          (i == prices.size() - 1 || prices[i + 1] >= prices[i])) {
        // vally
        buy[j] = max(buy[j], sell[j - 1] - prices[i]);
      } else if (prices[i - 1] <= prices[i] &&
                 (i == prices.size() - 1 || prices[i + 1] <= prices[i])) {
        // peak
        sell[j] = max(sell[j], buy[j] + prices[i]);
      }
  }
  // buy[i][k], consider prices[0:i] and max buy k times,
  // the max profit end with buy
  // if vally, buy[i][k] = max(buy[i-1][k], sell[i-1][k-1] - prices[i])
  // if peak, sell[i][k] = max(sell[i-1][k],  buy[i-1][k] + prices[i])
  return sell[k];
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices", "k"};
  return GenericTestMain(
      args, "buy_and_sell_stock_k_times.cc", "buy_and_sell_stock_k_times.tsv",
      &BuyAndSellStockKTimes, DefaultComparator{}, param_names);
}
