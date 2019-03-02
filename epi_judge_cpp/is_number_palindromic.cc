#include "test_framework/generic_test.h"
bool IsPalindromeNumber(int x) {
  long long rev = 0;
  long long orig = x;
  while (x > 0) {
    int d = x % 10;
    x /= 10;
    rev = rev * 10 + d;
  }
  return orig == rev;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "is_number_palindromic.cc",
                         "is_number_palindromic.tsv", &IsPalindromeNumber,
                         DefaultComparator{}, param_names);
}
