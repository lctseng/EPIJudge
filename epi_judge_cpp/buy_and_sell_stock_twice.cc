#include "test_framework/generic_test.h"
#include <deque>
#include <vector>
using std::deque;
using std::max;
using std::vector;

#define INF (1.0 / 0.0)

double BuyAndSellStockTwice(const vector<double> &prices) {
  if (prices.empty())
    return 0;
  // like DP in k-times
  int k = 2;
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

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock_twice.cc",
                         "buy_and_sell_stock_twice.tsv", &BuyAndSellStockTwice,
                         DefaultComparator{}, param_names);
}
