#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

bool ExpHelper(const vector<int> &digits, int target, int index,
               int lastOperand, int value) {
  if (index == digits.size()) {
    // terminal
    return value + lastOperand == target;
  } else if (value > target) {
    // pruning
    return false;
  }
  // find next placement
  int curVal = 0;
  for (int i = index; i < digits.size(); i++) {
    curVal = curVal * 10 + digits[i];
    // try place operator after this?
    // place '+'
    if (ExpHelper(digits, target, i + 1, curVal, value + lastOperand)) {
      return true;
    }
    // place '*'
    // BE CAREFUL!
    // when '*' is the first operator,
    // we cannot multiply it with last operand! this will make it become 0
    int newLastOperand = index == 0 ? curVal : curVal * lastOperand;
    if (ExpHelper(digits, target, i + 1, newLastOperand, value)) {
      return true;
    }
  }
  return false;
}

// form a number start from index, pick a operator and combine with prev
bool ExpressionSynthesisHelper(const vector<int> &digits, int target, int index,
                               int lastOperand, char lastOp, int value) {
  if (index == digits.size())
    return value == target;
  // form current number
  int curVal = 0;
  for (int i = index; i < digits.size(); i++) {
    curVal = curVal * 10 + digits[i];
    // pick op
    // pick '+'
    // pruning: no longer possible if value > target
    // because this '+' will only make value increaing in the future
    if (value <= target) {
      if (ExpressionSynthesisHelper(digits, target, i + 1, curVal, '+',
                                    value + curVal)) {
        return true;
      }
    }

    // pick '*'
    // previous pruning may not work here,
    // since we can use '* 0' to cancel previous values
    if (lastOp == '+') {
      // reverse
      int newOperand = index == 0 ? curVal : lastOperand * curVal;
      int newValue = value - lastOperand + newOperand;
      if (ExpressionSynthesisHelper(digits, target, i + 1, newOperand, lastOp,
                                    newValue)) {
        return true;
      }
    }
  }
  return false;
}

bool ExpressionSynthesis(const vector<int> &digits, int target) {
  // TODO - you fill in here.
  if (digits.empty())
    return target == 0;
  return ExpressionSynthesisHelper(digits, target, 0, 0, '+', 0);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"digits", "target"};
  return GenericTestMain(args, "insert_operators_in_string.cc",
                         "insert_operators_in_string.tsv", &ExpressionSynthesis,
                         DefaultComparator{}, param_names);
}
