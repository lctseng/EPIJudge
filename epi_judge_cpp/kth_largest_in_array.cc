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
    // find pivot
    int sz = end - begin;
    int pivotIndex = begin + rand() % sz;
    int pivot = A[pivotIndex];
    swap(A[pivotIndex], A[begin]);
    auto largerIt = partition(A.begin() + begin + 1, A.begin() + end,
                              [=](int n) { return n <= pivot; });
    int numSmallEqual = largerIt - (A.begin() + begin);

    int numLarger = sz - numSmallEqual;
    if (numLarger == 0 && k > 1) {
      // we picked the max value and it is not we wanted (k != 1)
      k -= 1;
      begin += 1;
    } else if (numLarger == k - 1) {
      // the pivot is kth largest
      return pivot;
    } else if (numLarger > k - 1) {
      // we have too may large number. the answer only exists in those numbers
      // skip all small/equal numbers
      begin += numSmallEqual;
    } else if (numLarger == sz - 1) {
      // we picked the min value and numLarger != k-1
      // we discard this value
      begin++;
    } else {
      // we have not enough large numbers
      // we need to find extra (k - numLarger)-th largest number in the smaller
      // part
      k -= numLarger;
      end = begin + numSmallEqual;
    }
  }
  return A[begin];
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k", "A"};
  return GenericTestMain(args, "kth_largest_in_array.cc",
                         "kth_largest_in_array.tsv", &FindKthLargestWrapper,
                         DefaultComparator{}, param_names);
}
