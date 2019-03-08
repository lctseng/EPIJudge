#include "test_framework/generic_test.h"
#include <deque>
#include <vector>
using std::deque;
using std::max;
using std::vector;

#define INF (1.0 / 0.0)

double BuyAndSellStockTwice(const vector<double> &prices) {
  // find split point
  deque<double> forwardProfit(1, 0.0);
  // forward[i]: profit for buy and sell stock within [0:i) days
  // using single scan
  double low, high;
  low = INF;
  high = -INF;
  for (int i = 0; i < prices.size(); i++) {
    if (prices[i] < low) {
      low = high = prices[i];
    } else if (prices[i] > high)
      high = prices[i];
    forwardProfit.push_back(max(forwardProfit.back(), high - low));
  }
  // backward profit within [i:]
  deque<double> backwardProfit(1, 0.0);
  // profit within []
  low = INF;
  high = -INF;
  for (int i = prices.size() - 1; i >= 0; i--) {
    if (prices[i] > high) {
      low = high = prices[i];
    } else if (prices[i] < low) {
      low = prices[i];
    }
    backwardProfit.push_front(max(backwardProfit.back(), high - low));
  }
  // find max point
  double maxValue = 0.0;
  for (int i = 0; i <= prices.size(); i++) {
    double curr = forwardProfit[i] + backwardProfit[i];
    if (curr > maxValue)
      maxValue = curr;
  }
  return maxValue;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock_twice.cc",
                         "buy_and_sell_stock_twice.tsv", &BuyAndSellStockTwice,
                         DefaultComparator{}, param_names);
}
