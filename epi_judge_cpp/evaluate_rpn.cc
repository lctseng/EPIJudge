#include "test_framework/generic_test.h"
#include <cstring>
#include <stack>
#include <string>
using std::stack;
using std::stoi;
using std::string;

int Evaluate(const string &expression) {
  if (expression.empty())
    return 0;
  // we need to tokenize
  stack<int> stk;
  // find next token
  size_t pos = 0;
  while (pos != string::npos) {
    // pos the position of next character
    // end: the position of next ','
    size_t end = expression.find(',', pos);
    string token;
    if (end == string::npos) {
      token = expression.substr(pos);
    } else {
      token = expression.substr(pos, end - pos);
    }
    if (token.length() == 1 && !isdigit(token[0])) {
      // operator
      int op2 = stk.top();
      stk.pop();
      int op1 = stk.top();
      stk.pop();
      int res;
      switch (token[0]) {
      case '+':
        res = op1 + op2;
        break;
      case '-':
        res = op1 - op2;
        break;
      case '*':
        res = op1 * op2;
        break;
      case '/':
        res = op1 / op2;
        break;
      }
      stk.push(res);
    } else {
      // operand
      stk.push(stoi(token));
    }
    pos = end;
    if (pos != string::npos) {
      ++pos; // skip the comma
    }
  }
  return stk.top();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"expression"};
  return GenericTestMain(args, "evaluate_rpn.cc", "evaluate_rpn.tsv", &Evaluate,
                         DefaultComparator{}, param_names);
}
