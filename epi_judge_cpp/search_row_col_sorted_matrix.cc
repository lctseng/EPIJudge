#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

bool MatrixSearch(const vector<vector<int>> &A, int x) {
  int m = A.size();
  if (!m)
    return false;
  int n = A[0].size();
  if (!n)
    return false;
  // check left-bottom angle
  int minCol = 0;
  int maxRow = m - 1;
  while (maxRow >= 0 && minCol < n) {
    if (A[maxRow][minCol] == x)
      return true;
    else if (x > A[maxRow][minCol]) {
      ++minCol;
    } else {
      --maxRow;
    }
  }
  return false;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "x"};
  return GenericTestMain(args, "search_row_col_sorted_matrix.cc",
                         "search_row_col_sorted_matrix.tsv", &MatrixSearch,
                         DefaultComparator{}, param_names);
}
