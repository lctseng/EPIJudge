#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

// advance to next new number
void advance(const vector<int> &A, int &index) {
  ++index;
  while (index < A.size() && A[index - 1] == A[index]) {
    ++index;
  }
}

vector<int> IntersectTwoSortedArrays(const vector<int> &A,
                                     const vector<int> &B) {
  vector<int> res;
  // two  pointers
  int i = 0, j = 0;
  while (i < A.size() && j < B.size()) {
    if (A[i] == B[j]) {
      res.push_back(A[i]);
      advance(A, i);
      advance(B, j);
    } else if (A[i] < B[j])
      advance(A, i);
    else
      advance(B, j);
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(
      args, "intersect_sorted_arrays.cc", "intersect_sorted_arrays.tsv",
      &IntersectTwoSortedArrays, DefaultComparator{}, param_names);
}
