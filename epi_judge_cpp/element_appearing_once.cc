#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int FindElementAppearsOnce(const vector<int> &A) {
  const int BIT_SIZE = 32;
  int res = 0;
  for (int i = 0; i < BIT_SIZE; i++) {
    int mask = 1 << i;
    int count = 0;
    for (int val : A) {
      if (val & mask) {
        ++count;
      }
    }
    if (count % 3) {
      res |= mask;
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
