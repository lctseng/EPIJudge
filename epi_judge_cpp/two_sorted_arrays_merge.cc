#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

void MergeTwoSortedArrays(vector<int> &A, int m, const vector<int> &B, int n) {
  // merged from tail to avoid any bulk moving
  int finalIndex = m + n - 1; // current index for merged A
  --m;                        // current index for A
  --n;                        // current index for B
  while (m >= 0 && n >= 0) {
    // pick larger
    if (A[m] >= B[n]) {
      A[finalIndex--] = A[m--];
    } else {
      A[finalIndex--] = B[n--];
    }
  }
  // one of the array is empty
  // fill remaining. dont need to fill for A
  while (n >= 0) {
    A[finalIndex--] = B[n--];
  }
}
vector<int> MergeTwoSortedArraysWrapper(vector<int> A, int m,
                                        const vector<int> &B, int n) {
  MergeTwoSortedArrays(A, m, B, n);
  return A;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "m", "B", "n"};
  return GenericTestMain(
      args, "two_sorted_arrays_merge.cc", "two_sorted_arrays_merge.tsv",
      &MergeTwoSortedArraysWrapper, DefaultComparator{}, param_names);
}
