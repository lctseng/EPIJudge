#include "test_framework/generic_test.h"
#include <vector>
using std::accumulate;
using std::max;
using std::min;
using std::vector;

int MaxSubarraySumInCircular(const vector<int> &A) {
  // no wrap around case
  int maxNonWrap = 0;
  int curSum = 0;
  for (int val : A) {
    curSum = max(curSum + val, val);
    maxNonWrap = max(curSum, maxNonWrap);
  }
  int maxWrap = 0;
  if (A.size() > 2) {
    // wraping is only meaningful when size of A is greater than 2
    // in fact, just substract the min subarray, we got the circular max
    int minSum = 0;
    int curSum = 0;
    for (int i = 0; i < A.size(); i++) {
      curSum = min(curSum + A[i], A[i]);
      minSum = min(curSum, minSum);
    }
    maxWrap = max(maxWrap, accumulate(A.begin(), A.end(), 0) - minSum);

    /*
    // find 'no warp mininum subarray' along its start and end index
    int minSum = 0;
    int curSum = 0;
    int minStartIdx;
    int minEndIdx;
    int startIdx = 0;
    for (int i = 0; i < A.size(); i++) {
      if (curSum > 0) {
        curSum = A[i];
        startIdx = i;
      } else {
        curSum += A[i];
      }
      if (curSum < minSum) {
        minSum = curSum;
        minStartIdx = startIdx;
        minEndIdx = i;
      }
    }

    // find  conti sum
    // search first half, index less than minStartIndex
    curSum = A[0];
    int maxSum = curSum;
    for (int i = 1; i < minStartIdx; i++) {
      curSum += A[i];
      if (curSum > maxSum)
        maxSum = curSum;
    }
    // search second half
    curSum = A.back();
    for (int i = A.size() - 2; i >= minEndIdx; i--) {
      maxWrap = max(maxWrap, maxSum + curSum);
      curSum += A[i];
    }
    */
  }
  return max(maxNonWrap, maxWrap);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "maximum_subarray_in_circular_array.cc",
                         "maximum_subarray_in_circular_array.tsv",
                         &MaxSubarraySumInCircular, DefaultComparator{},
                         param_names);
}
