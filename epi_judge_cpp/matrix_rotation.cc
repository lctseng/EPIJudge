#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

void RotateLayer(vector<vector<int>> &matrix, int start, int sideLen) {
  // inplace rotate!
  for (int j = 0; j < sideLen - 1; j++) {
    // perform 4-way exchange
    // be careful! all matrix access need to +start
    // gather 4 to-go element
    int temp1 = matrix[start][start + j];
    int temp2 = matrix[start + j][start + sideLen - 1];
    int temp3 = matrix[start + sideLen - 1][start + sideLen - 1 - j];
    int temp4 = matrix[start + sideLen - 1 - j][start];
    matrix[start + j][start + sideLen - 1] = temp1;
    matrix[start + sideLen - 1][start + sideLen - 1 - j] = temp2;
    matrix[start + sideLen - 1 - j][start] = temp3;
    matrix[start][start + j] = temp4;
  }
}

void RotateMatrix(vector<vector<int>> *square_matrix_ptr) {
  auto &square_matrix = *square_matrix_ptr;
  int sideLen = square_matrix.size();
  int start = 0;
  // rotate layer by layer
  for (; sideLen > 0; sideLen -= 2) {
    RotateLayer(square_matrix, start, sideLen);
    ++start;
  }
}
vector<vector<int>> RotateMatrixWrapper(vector<vector<int>> square_matrix) {
  RotateMatrix(&square_matrix);
  return square_matrix;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"square_matrix"};
  return GenericTestMain(args, "matrix_rotation.cc", "matrix_rotation.tsv",
                         &RotateMatrixWrapper, DefaultComparator{},
                         param_names);
}
