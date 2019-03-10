#include "test_framework/generic_test.h"
#include <deque>
#include <stack>
#include <vector>
using std::deque;
using std::max;
using std::min;
using std::stack;
using std::vector;

int CalculateLargestSquare(const vector<int> &heights) {
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
      int sideLen = min(heights[popIndex], (lastIndex - prevIndex));
      int currentArea = sideLen * sideLen;
      if (currentArea > maxValue)
        maxValue = currentArea;
    }
    // push it
    stk.push(i);
  }
  return maxValue;
}

int MaxSquareSubmatrix(const vector<deque<bool>> &A) {
  // model this problem using max rectangle under skyline
  int n = A.size();
  if (!n)
    return 0;
  int m = A[0].size();
  if (!m)
    return 0;
  vector<int> heights(m, 0);
  int maxValue = 0;
  for (int i = 0; i < n; i++) {
    // update height first
    for (int j = 0; j < m; j++) {
      if (A[i][j]) {
        heights[j] += 1;
      } else {
        heights[j] = 0;
      }
    }
    // apply previous algo
    maxValue = max(maxValue, CalculateLargestSquare(heights));
  }
  return maxValue;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "max_square_submatrix.cc",
                         "max_square_submatrix.tsv", &MaxSquareSubmatrix,
                         DefaultComparator{}, param_names);
}
