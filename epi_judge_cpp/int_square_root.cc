#include "test_framework/generic_test.h"

int SquareRoot(int k) {
  if (k <= 1)
    return k;
  int low = 1, high = k;
  while (low < high) {
    int mid = low + (high - low) / 2;
    if (mid <= k / mid && (mid + 1) > k / (mid + 1))
      return mid;
    else if (mid > k / mid) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }
  return low;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k"};
  return GenericTestMain(args, "int_square_root.cc", "int_square_root.tsv",
                         &SquareRoot, DefaultComparator{}, param_names);
}
