#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int GetMaxTrappedWater(const vector<int> &heights) {
  int left = 0, right = heights.size() - 1;
  int maxValue = 0;
  while (left < right) {
    if (heights[left] < heights[right]) {
      int current = heights[left] * (right - left);
      if (current > maxValue)
        maxValue = current;
      ++left;
    } else {
      int current = heights[right] * (right - left);
      if (current > maxValue)
        maxValue = current;
      --right;
    }
  }
  return maxValue;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "max_trapped_water.cc", "max_trapped_water.tsv",
                         &GetMaxTrappedWater, DefaultComparator{}, param_names);
}
