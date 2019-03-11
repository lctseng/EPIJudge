#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

struct Subarray {
  int start = 0, end = 0;
  int Distance() const { return end - start + 1; }
};

// improve on best case
Subarray FindLongestIncreasingSubarray(const vector<int> &A) {
  Subarray res;
  // two stage
  // stage 1: compute as usual
  // stage 2: when find max len L, with ending index i
  //          try back track j in [i+1:i + L]
  //          if one j that is A[j-1] >= A[j], break
  // stage 3: skip all indices before j,
  //          make current L = i + L - j + 1 (by changing startIndex to j)
  //          i jump to i+L, back to stage 1
  int i = 0;
  int startIndex = 0;
  while (i < A.size()) {
    if (i < A.size() - 1 && A[i + 1] <= A[i]) {
      // stage 1: found a current len
      int currentD = i - startIndex + 1;
      if (currentD > res.Distance())
        res = {startIndex, i};
      // try backing
      // jump!
      int newI = i + res.Distance();
      if (newI < A.size()) {
        int j;
        for (j = newI; j > i; j--) {
          if (A[j - 1] >= A[j])
            break;
        }
        startIndex = j;
        i = newI;
      } else {
        // it is impossble to find better answer
        break;
      }
    } else {
      // number valid
      ++i;
    }
  }
  // last update
  if (i - startIndex > res.Distance()) {
    res.start = startIndex;
    res.end = i - 1;
  }
  return res;
}

Subarray FindLongestIncreasingSubarrayStandard(const vector<int> &A) {
  Subarray res;
  int startIndex = 0;
  for (int i = 0; i < A.size(); i++) {
    if (i == A.size() - 1 || A[i + 1] <= A[i]) {
      // found ans startIndex to i
      int curD = i - startIndex + 1;
      if (curD > res.Distance()) {
        res = {startIndex, i};
      }
      startIndex = i + 1;
    }
  }
  return res;
}
int FindLongestIncreasingSubarrayWrapper(const vector<int> &A) {
  Subarray result = FindLongestIncreasingSubarray(A);
  return result.end - result.start + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "longest_increasing_subarray.cc", "longest_increasing_subarray.tsv",
      &FindLongestIncreasingSubarrayWrapper, DefaultComparator{}, param_names);
}
