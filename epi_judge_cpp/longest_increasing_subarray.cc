#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

struct Subarray {
  int start = 0, end = 0;
  int Distance() const { return end - start; }
};

Subarray FindLongestIncreasingSubarray(const vector<int> &A) {
  Subarray res;
  int startIndex = 0;
  for (int i = 0; i < A.size(); i++) {
    if (i == A.size() - 1 || A[i + 1] <= A[i]) {
      // found ans startIndex to i
      int curD = i - startIndex;
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
