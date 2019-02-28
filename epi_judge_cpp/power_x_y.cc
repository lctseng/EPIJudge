#include "test_framework/generic_test.h"
double Power(double x, int y) {
  bool isNeg = false;
  if (y < 0) {
    isNeg = true;
    y *= -1;
  }
  double res = 1.0;
  while (y > 0) {
    if (y & 1) {
      // odd
      --y;
      res *= x;
    } else {
      y >>= 1;
      x = x * x;
    }
  }
  return isNeg ? 1.0 / res : res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x", "y"};
  return GenericTestMain(args, "power_x_y.cc", "power_x_y.tsv", &Power,
                         DefaultComparator{}, param_names);
}
