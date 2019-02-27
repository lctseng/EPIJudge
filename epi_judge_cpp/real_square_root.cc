#include "test_framework/generic_test.h"
#include <limits>
using std::max;
using std::numeric_limits;

int Compare(double a, double b) {
  double diff = (a - b) / max(abs(a), abs(b)); // THIS IS IMPORTANT!
  if (abs(diff) < numeric_limits<double>::epsilon()) {
    return 0;
  } else if (diff < -numeric_limits<double>::epsilon()) {
    return -1;
  } else {
    return 1;
  }
}

double SquareRoot(double x) {
  if (x == 0)
    return 0;
  double low = 0, high = x;
  if (high < 1)
    high = 1;
  while (low < high) {
    double mid = low + (high - low) / 2;
    int res = Compare(mid, x / mid);
    if (res == 0)
      return mid;
    else if (res == 1) {
      high = mid;
    } else {
      low = mid;
    }
  }
  return low;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "real_square_root.cc", "real_square_root.tsv",
                         &SquareRoot, DefaultComparator{}, param_names);
}
