#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int BinarySearchUnknownLength(const vector<int> &A, int k) {
  if (A.empty())
    return -1; // we do not handle the empty Array case
  // stage 1: find length using binary search
  // BE CAREFUL!
  // init low and high
  int high = 1, low = 0;
  bool found = false;
  while (!found) {
    // try access
    try {
      int a = A.at(high);
      // BE CAREFUL!!
      // the direction of shift!
      high <<= 1;
    } catch (...) {
      // too large, stop
      found = true;
    }
  }
  // stage 2: use that length to do binary search
  // Note that if the mid element access with exception, force go left
  while (low < high) {
    int mid = low + (high - low) / 2;
    try {
      int val = A.at(mid);
      if (val > k) {
        // too large
        high = mid;
      } else if (val < k) {
        // too small
        low = mid + 1;
      } else {
        // found
        return mid;
      }
    } catch (...) {
      // mid too large
      high = mid;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(
      args, "search_unknown_length_array.cc", "search_unknown_length_array.tsv",
      &BinarySearchUnknownLength, DefaultComparator{}, param_names);
}
