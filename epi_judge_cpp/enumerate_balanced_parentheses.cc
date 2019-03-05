#include "test_framework/generic_test.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

void GenerateBalancedParenthesesHelper(vector<string> &res, string &builder,
                                       int left, int right) {
  if (!left && !right) {
    // we have done
    res.push_back(builder);
  } else {
    if (left > 0) {
      // add a left
      builder.append(1, '(');
      GenerateBalancedParenthesesHelper(res, builder, left - 1, right + 1);
      // restore
      builder.pop_back();
    }
    if (right > 0) {
      // add a right
      builder.append(1, ')');
      GenerateBalancedParenthesesHelper(res, builder, left, right - 1);
      // restore
      builder.pop_back();
    }
  }
}

vector<string> GenerateBalancedParentheses(int num_pairs) {
  string builder;
  builder.reserve(num_pairs * 2);
  vector<string> res;
  GenerateBalancedParenthesesHelper(res, builder, num_pairs, 0);
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_pairs"};
  return GenericTestMain(args, "enumerate_balanced_parentheses.cc",
                         "enumerate_balanced_parentheses.tsv",
                         &GenerateBalancedParentheses,
                         &UnorderedComparator<vector<string>>, param_names);
}
