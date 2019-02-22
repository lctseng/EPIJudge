#include "test_framework/generic_test.h"
#include <vector>
using std::vector;
// Check if a partially filled matrix has any conflicts.
bool IsValidSudoku(const vector<vector<int>> &partial_assignment) {
  // check rows
  for (int i = 0; i < 9; i++) {
    int count[10] = {0};
    for (int n : partial_assignment[i]) {
      if (n > 0) {
        if (++count[n] > 1)
          return false;
      }
    }
  }
  // check cols
  for (int i = 0; i < 9; i++) {
    int count[10] = {0};
    for (int j = 0; j < 9; j++) {
      int n = partial_assignment[j][i];
      if (n > 0) {
        if (++count[n] > 1)
          return false;
      }
    }
  }
  // check 3x3 grid
  for (int oi = 0; oi < 9; oi += 3) {
    for (int oj = 0; oj < 9; oj += 3) {
      int count[10] = {0};
      for (int i = oi; i < oi + 3; i++) {
        for (int j = oj; j < oj + 3; j++) {
          int n = partial_assignment[i][j];
          if (n > 0) {
            if (++count[n] > 1)
              return false;
          }
        }
      }
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"partial_assignment"};
  return GenericTestMain(args, "is_valid_sudoku.cc", "is_valid_sudoku.tsv",
                         &IsValidSudoku, DefaultComparator{}, param_names);
}
