#include "test_framework/generic_test.h"
#include <stack>
#include <string>
using std::stack;
using std::string;
bool IsWellFormed(const string &s) {
  stack<char> stk;
  for (char c : s) {
    switch (c) {
    case '(':
    case '[':
    case '{':
      stk.push(c);
      break;
    case ')':
      if (stk.empty() || stk.top() != '(')
        return false;
      stk.pop();
      break;
    case ']':
      if (stk.empty() || stk.top() != '[')
        return false;
      stk.pop();
      break;
    case '}':
      if (stk.empty() || stk.top() != '{')
        return false;
      stk.pop();
      break;
    }
  }
  return stk.empty();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "is_valid_parenthesization.cc",
                         "is_valid_parenthesization.tsv", &IsWellFormed,
                         DefaultComparator{}, param_names);
}
