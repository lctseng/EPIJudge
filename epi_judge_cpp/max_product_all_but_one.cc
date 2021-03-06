#include "test_framework/generic_test.h"
#include <climits>
#include <vector>
using std::accumulate;
using std::vector;

int ResultExceptIndex(const vector<int> &A, int index) {
  int res = 1;
  for (int i = 0; i < A.size(); i++) {
    if (i != index) {
      res *= A[i];
    }
  }
  return res;
}

int FindBiggestNMinusOneProduct(const vector<int> &A) {
  // count number of 0, neg
  int zeroCount = 0;
  int negCount = 0;
  for (int val : A) {
    if (val == 0)
      zeroCount++;
    else if (val < 0)
      negCount++;
  }
  // if 2+ zero: 0
  if (zeroCount >= 2)
    return 0;
  // if 1 zero
  if (zeroCount == 1) {
    if (negCount & 1)
      // odd neg: 0
      return 0;
    else {
      // locate 0
      int idxToSkip;
      for (int i = 0; i < A.size(); i++) {
        if (A[i] == 0) {
          idxToSkip = i;
          break;
        }
      }
      return ResultExceptIndex(A, idxToSkip);
    }
  }
  // if no zero
  if (negCount & 1) {
    // if odd neg, remove largest negative num
    int idxToSkip = -1;
    int currentMax = INT_MIN;
    for (int i = 0; i < A.size(); i++) {
      if (A[i] < 0 && A[i] > currentMax) {
        currentMax = A[i];
        idxToSkip = i;
      }
    }
    return ResultExceptIndex(A, idxToSkip);
  } else {
    if (negCount < A.size()) {
      // if even neg, remove smallest positive num
      int idxToSkip = -1;
      int currentMin = INT_MAX;
      for (int i = 0; i < A.size(); i++) {
        if (A[i] > 0 && A[i] < currentMin) {
          currentMin = A[i];
          idxToSkip = i;
        }
      }
      return ResultExceptIndex(A, idxToSkip);
    } else {
      // special case for even neg, if negCount == A.size(),
      // then remove smallest negative num
      int idxToSkip = -1;
      int currentMin = INT_MAX;
      for (int i = 0; i < A.size(); i++) {
        if (A[i] < currentMin) {
          currentMin = A[i];
          idxToSkip = i;
        }
      }
      return ResultExceptIndex(A, idxToSkip);
    }
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "max_product_all_but_one.cc", "max_product_all_but_one.tsv",
      &FindBiggestNMinusOneProduct, DefaultComparator{}, param_names);
}
