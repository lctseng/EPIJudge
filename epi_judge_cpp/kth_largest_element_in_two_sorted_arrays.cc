#include "test_framework/generic_test.h"
#include <climits>
#include <vector>
using std::max;
using std::min;
using std::vector;

int FindKthInTwoSortedArrays(const vector<int> &A, const vector<int> &B,
                             int k) {
  int low = 0, high = min(k, (int)A.size());
  // find how many element got from A
  while (low <= high) {
    int numFromA = low + (high - low) / 2;
    int numFromB = k - numFromA;
    if (numFromB > B.size()) {
      // BE CAREFUL!
      // numFromB need to be meaningful
      // numFromA too small
      low = numFromA + 1;
      continue;
    }
    // now, numFromA is 0 to A.size(), numFromB is 0 to B.size()
    int preA = numFromA > 0 ? A[numFromA - 1] : INT_MIN;
    int postA = numFromA < A.size() ? A[numFromA] : INT_MAX;
    int preB = numFromB > 0 ? B[numFromB - 1] : INT_MIN;
    int postB = numFromB < B.size() ? B[numFromB] : INT_MAX;
    // check: A[numFromA - 1] >= B[numFromB]
    if (preA > postB) {
      // --> if not, numFromA too large
      high = numFromA - 1;
    }
    // check: B[numFromB - 1] >= A[numFromA]
    else if (preB > postA) {
      // --> if not, numFromA too small
      low = numFromA + 1;
    } else {
      // if all meet, larger of A[numFromA - 1], B[numFromB - 1] is answer
      return max(preA, preB);
    }
  }
  // Impossible to reach here
  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B", "k"};
  return GenericTestMain(args, "kth_largest_element_in_two_sorted_arrays.cc",
                         "kth_largest_element_in_two_sorted_arrays.tsv",
                         &FindKthInTwoSortedArrays, DefaultComparator{},
                         param_names);
}
