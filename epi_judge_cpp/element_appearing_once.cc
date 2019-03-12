#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int FindElementAppearsOnce(const vector<int> &A) {
  const int BIT_SIZE = 32;
  int bitCount[BIT_SIZE] = {0};
  for (int val : A) {
    for (int i = 0; i < BIT_SIZE; i++) {
      // BE CAREFUL!
      // must use != 0, cannot use >0 since it may be the sign bit!
      bitCount[i] += (val & (1 << i)) != 0;
    }
  }
  // reconstruct that num
  int res = 0;
  for (int i = 0; i < BIT_SIZE; i++) {
    if (bitCount[i] % 3) {
      res |= 1 << i;
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "element_appearing_once.cc",
                         "element_appearing_once.tsv", &FindElementAppearsOnce,
                         DefaultComparator{}, param_names);
}
