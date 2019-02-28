#include "test_framework/generic_test.h"
long long Reverse(int _x) {
  // TODO - you fill in here.
  bool isNeg = false;
  long long x = _x;
  if (x < 0) {
    x *= -1;
    isNeg = true;
  }
  long long res = 0;
  while (x > 0) {
    int d = x % 10;
    x /= 10;
    res = res * 10 + d;
  }
  if (isNeg)
    res *= -1;
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "reverse_digits.cc", "reverse_digits.tsv",
                         &Reverse, DefaultComparator{}, param_names);
}
