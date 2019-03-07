#include "test_framework/generic_test.h"

// BE CAREFUL!
// sizeof return is bytes not bits!
#define BIT_LONG (sizeof(unsigned long long) * 8)

unsigned long long ClosestIntSameBitCount(unsigned long long x) {
  // if start with 1
  // find closest 0, change it and its prev 1
  // if start with 0
  // find closest 1, move it down
  for (int i = 1; i < BIT_LONG; i++) {
    int bit = 1 << i;
    if ((x & 1) == !(x & bit)) {
      // found
      x ^= bit;
      x ^= bit >> 1;
      break;
    }
  }
  return x;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"x"};
  return GenericTestMain(args, "closest_int_same_weight.cc",
                         "closest_int_same_weight.tsv", &ClosestIntSameBitCount,
                         DefaultComparator{}, param_names);
}
