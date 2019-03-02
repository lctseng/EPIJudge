#include "test_framework/generic_test.h"
bool IsPalindromeNumber(int x) {
  if (x < 0)
    return false;
  if (x < 10)
    return true;
  // extract two bit at
  int exp = 10;
  while (x / exp >= 10) {
    exp *= 10;
  }
  while (exp >= 10) {
    // check two digit
    int msd = x / exp;
    int lsd = x % 10;
    if (msd != lsd)
      return false;
    x %= exp;
    x /= 10;
    exp /= 100; // IMPORTANT!
  }
  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "is_number_palindromic.cc",
                         "is_number_palindromic.tsv", &IsPalindromeNumber,
                         DefaultComparator{}, param_names);
}
