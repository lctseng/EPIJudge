#include "test_framework/generic_test.h"
#include <stack>
#include <vector>
using std::size;
using std::stack;
using std::vector;

int CalculateLargestRectangle(const vector<int> &heights) {
  // monostack with greedy
  if (heights.empty())
    return 0;
  // need to flush all when meet the end
  stack<int> stk; // monostack, according to height, store index
  int maxValue = 0;
  for (int i = 0; i <= size(heights); i++) {
    int currentH = i == size(heights) ? 0 : heights[i];
    int lastIndex = stk.size() ? stk.top() : -1;
    // pop all
    while (stk.size() && heights[stk.top()] > currentH) {
      int popIndex = stk.top();
      stk.pop();
      int prevIndex = stk.size() ? stk.top() : -1;
      int currentArea = heights[popIndex] * (lastIndex - prevIndex);
      if (currentArea > maxValue)
        maxValue = currentArea;
    }
    // push it
    stk.push(i);
  }
  return maxValue;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"heights"};
  return GenericTestMain(args, "largest_rectangle_under_skyline.cc",
                         "largest_rectangle_under_skyline.tsv",
                         &CalculateLargestRectangle, DefaultComparator{},
                         param_names);
}
