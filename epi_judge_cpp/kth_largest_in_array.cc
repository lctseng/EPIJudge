#include "test_framework/generic_test.h"
#include <vector>
using std::swap;
using std::vector;

// The numbering starts from one, i.e., if A = [3, 1, -1, 2] then
// FindKthLargest(1, A) returns 3, FindKthLargest(2, A) returns 2,
// FindKthLargest(3, A) returns 1, and FindKthLargest(4, A) returns -1.

int FindKthLargestWrapper(int k, vector<int> &A) {
  int begin = 0, end = A.size();
  while (begin < end - 1) {
    // partition around pivot
    int pivotIndex = begin + rand() % (end - begin);
    int pivot = A[pivotIndex];
    swap(A[pivotIndex], A[end - 1]);
    auto lessEqualIt = partition(A.begin() + begin, A.begin() + end - 1,
                                 [=](int n) { return n > pivot; });
    swap(*lessEqualIt, A[end - 1]);
    int newPivotIndex = lessEqualIt - A.begin(); // global index
    if (newPivotIndex == k - 1) {
      return A[newPivotIndex];
    } else if (newPivotIndex < k - 1) {
      // need a smaller number
      begin = newPivotIndex + 1;
    } else {
      // need a larger number
      end = newPivotIndex;
    }
  }
  // not reach here
  return A[begin];
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k", "A"};
  return GenericTestMain(args, "kth_largest_in_array.cc",
                         "kth_largest_in_array.tsv", &FindKthLargestWrapper,
                         DefaultComparator{}, param_names);
}
