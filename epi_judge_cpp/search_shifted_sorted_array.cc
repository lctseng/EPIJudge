#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int SearchSmallest(const vector<int> &A) {
  int low = 0, high = A.size() - 1;
  while (low < high) {
    int mid = low + (high - low) / 2;
    if ((mid == 0 || A[mid - 1] > A[mid]) &&
        (mid == A.size() - 1 || A[mid + 1] > A[mid]))
      return mid;
    else if (A[mid] > A[high]) {
      // go right
      low = mid + 1;
    } else {
      // go left
      high = mid - 1;
    }
  }
  return low;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "search_shifted_sorted_array.cc",
                         "search_shifted_sorted_array.tsv", &SearchSmallest,
                         DefaultComparator{}, param_names);
}
