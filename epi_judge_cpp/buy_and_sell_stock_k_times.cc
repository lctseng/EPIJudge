#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::min;
using std::vector;

// O(NK) time, O(K) space
double BuyAndSellStockKTimes(const vector<double> &prices, int K) {
  if (prices.empty() || K == 0)
    return 0;
  int N = prices.size();
  // BE CAREFUL! extremely large K
  if (K >= N / 2) {
    // buy all increasing edge
    // BE CAREFUL the type
    double sum = 0;
    for (int i = 1; i < N; i++) {
      double diff = prices[i] - prices[i - 1];
      if (diff > 0)
        sum += diff;
    }
    return sum;
  }
  // two-state dp
  // buy[i][k]: consider prices[0:i) and exactly done k buys, with final
  // operation is BUY init condition:
  // → buy[0][~] = INT_MIN
  // → sell[0][~] = 0

  vector<double> buy(K + 1, -std::numeric_limits<double>::infinity());
  vector<double> sell(K + 1, 0);
  // i from 1 to price.size(),k from 1 to K
  for (int i = 1; i <= N; i++) {
    for (int k = 1; k <= K; k++) {
      // do not update inplace, avoid interference
      double nextBuy = max(buy[k], sell[k - 1] - prices[i - 1]);
      double nextSell = max(sell[k], buy[k] + prices[i - 1]);
      buy[k] = nextBuy;
      sell[k] = nextSell;
    }
  }
  // buy[i][k] = max(buy[i-1][k], sell[i-1][k-1] - prices[i-1]);
  // sell[i][k] = max(sell[i-1][k], buy[i-1][k] + prices[i-1]);
  // answer is sell[prices.size()][K]
  return sell[K];
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
