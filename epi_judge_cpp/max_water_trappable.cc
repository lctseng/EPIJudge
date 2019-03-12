#include "test_framework/generic_test.h"
#include <vector>
using std::max;
using std::vector;

int CalculateTrappingWater(const vector<int> &heights) {
  int total = 0;
  int leftMax = 0, rightMax = 0;
  int left = 0, right = heights.size() - 1;
  while (left <= right) {
    if (heights[left] < heights[right]) {
      // left forward
      total += max(0, leftMax - heights[left]);
      leftMax = max(leftMax, heights[left]);
      left++;
    } else {
      // right forward
      total += max(0, rightMax - heights[right]);
      rightMax = max(rightMax, heights[right]);
      right--;
    }
  }
  return total;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "max_water_trappable.cc",
                         "max_water_trappable.tsv", &CalculateTrappingWater,
                         DefaultComparator{}, param_names);
}
