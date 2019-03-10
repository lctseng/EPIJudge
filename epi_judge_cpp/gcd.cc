#include "test_framework/generic_test.h"
using std::swap;

long long Gcd(long long x, long long y) {
  long long res = 1;
  if (x < y)
    swap(x, y);
  // BE CAREFUL!
  // the terminal condition is y > 0, same as usual GCD
  while (y > 0) {
    if ((x & 1) == 0 && (y & 1) == 0) {
      // both even
      x >>= 1;
      y >>= 1;
      res <<= 1;
    } else if ((x & 1) == 0) {
      // x even only
      // 2 is not part of GCD
      x >>= 1;
    } else if ((y & 1) == 0) {
      y >>= 1;
    } else {
      // both odd
      x -= y;
    }
    if (x < y)
      swap(x, y);
  }
  return x * res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  return GenericTestMain(args, "gcd.cc", "gcd.tsv", &Gcd, DefaultComparator{},
                         param_names);
}
