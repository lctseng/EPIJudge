#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

int SearchFirstOfK(const vector<int> &A, int k) {
  int begin = 0, end = A.size();
  while (begin < end) {
    int mid = begin + (end - begin) / 2;
    if (A[mid] == k && (mid == 0 || A[mid - 1] != k)) {
      return mid;
    } else if (A[mid] < k) {
      // go right
      begin = mid + 1;
    } else {
      // go left
      end = mid;
    }
  }
  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "k"};
  return GenericTestMain(args, "search_first_key.cc", "search_first_key.tsv",
                         &SearchFirstOfK, DefaultComparator{}, param_names);
}
