#include "test_framework/generic_test.h"
#include <vector>
using std::size;
using std::vector;

// A is passed by value argument, since we change it.
int FindFirstMissingPositive(vector<int> A) {
  // move every positive number to its position by num - 1
  // discard too large number
  int sz = size(A);
  for (int i = 0; i < sz; i++) {
    if (A[i] > 0 && A[i] != i + 1) {
      // a positive number is not on its position
      // circular moving it
      if (A[i] > sz) {
        // this number is totally no use
        A[i] = -1; // free its space :P
        continue;
      }
      int currentValue = A[i];
      A[i] = -1; // free its space
      // next number exists and need to preserve
      // next number must in the range and not equal to current
      while (A[currentValue - 1] > 0 && A[currentValue - 1] <= sz &&
             A[currentValue - 1] != currentValue) {
        int nextValue = A[currentValue - 1];
        A[currentValue - 1] = currentValue;
        currentValue = nextValue;
      }
      // destination space is usable
      A[currentValue - 1] = currentValue;
    }
  }
  // now, scan from 1 to sz
  for (int i = 0; i < sz; i++) {
    if (A[i] != i + 1) {
      return i + 1;
    }
  }
  return sz + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "first_missing_positive_entry.cc",
                         "first_missing_positive_entry.tsv",
                         &FindFirstMissingPositive, DefaultComparator{},
                         param_names);
}
