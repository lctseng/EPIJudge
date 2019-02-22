#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::vector;
#define INF (1.0 / 0.0)
double BuyAndSellStockOnce(const vector<double> &prices) {
  // find max diff
  double low = INF;
  double high = -INF;
  double maxDiff = 0;
  for (double price : prices) {
    if (price < low) {
      low = high = price;
    } else if (price > high) {
      high = price;
    }
    maxDiff = max(maxDiff, high - low);
  }
  return maxDiff;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock.cc",
                         "buy_and_sell_stock.tsv", &BuyAndSellStockOnce,
                         DefaultComparator{}, param_names);
}
