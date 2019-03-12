#include "test_framework/generic_test.h"
#include <deque>
#include <string>
#include <vector>
using std::deque;
using std::max;
using std::string;
using std::vector;

int LongestMatchingParentheses(const string &s) {
  int maxValue = 0;
  deque<int> levelStack;
  levelStack.push_back(0);
  for (char c : s) {
    if (c == '(') {
      // meet '(' : push next level
      levelStack.push_back(0);
    } else {
      // ')'
      // meet ')' : pop current and accumulate with lower
      if (levelStack.size() > 1) {
        int cur = levelStack.back();
        levelStack.pop_back();
        levelStack.back() += cur + 2;
      } else {
        // if going to pop level-0,
        // replace by recording the max value and reset level 0 to 0
        maxValue = max(maxValue, levelStack[0]);
        levelStack[0] = 0;
      }
    }
  }

  // finally, scan over the deque to find again the max value
  for (int val : levelStack) {
    if (val > maxValue)
      maxValue = val;
  }
  return maxValue;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "longest_substring_with_matching_parentheses.cc",
                         "longest_substring_with_matching_parentheses.tsv",
                         &LongestMatchingParentheses, DefaultComparator{},
                         param_names);
}
