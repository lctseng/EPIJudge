#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

void RookAttack(vector<vector<int>> *A_ptr) {
  vector<vector<int>> &A = *A_ptr;
  // accumulate to left to top border
  int m = A.size();
  if (!m)
    return;
  int n = A[0].size();
  if (!n)
    return;
  // BE CAREFUL!
  // Need record first row/col state
  // first col has rook and first row has rook must be recorded
  bool firstRowRook = false;
  bool firstColRook = false;
  // BE CAREFUL!
  // when recording, still consider first row/col !
  // because they still affect remaining blocks!
  for (int j = 0; j < n; j++) {
    if (!A[0][j]) {
      firstRowRook = true;
      break;
    }
  }
  for (int i = 0; i < m; i++) {
    if (!A[i][0]) {
      firstColRook = true;
      break;
    }
  }
  // use first row and first col as accumulation space
  // BE CAREFUL!
  // dont need to split into 2 pass!
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (!A[i][j]) {
        A[i][0] = A[0][j] = 0;
      }
    }
  }
  // distribute all except first row and first col
  for (int i = 1; i < m; i++) {
    if (!A[i][0]) {
      for (int j = 1; j < n; j++) {
        A[i][j] = 0;
      }
    }
  }
  for (int j = 1; j < n; j++) {
    if (!A[0][j]) {
      for (int i = 1; i < m; i++) {
        A[i][j] = 0;
      }
    }
  }
  // distribute first row and first col
  // BE CAREFUL!
  // remember to update first col/row
  if (firstRowRook) {
    for (int j = 0; j < n; j++) {
      A[0][j] = 0;
    }
  }
  if (firstColRook) {
    for (int i = 0; i < m; i++) {
      A[i][0] = 0;
    }
  }
}
vector<vector<int>> RookAttackWrapper(vector<vector<int>> A) {
  vector<vector<int>> a_copy = A;
  RookAttack(&a_copy);
  return a_copy;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "rook_attack.cc", "rook_attack.tsv",
                         &RookAttackWrapper, DefaultComparator{}, param_names);
}
