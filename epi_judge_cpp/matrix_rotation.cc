#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

void RotateLayer(vector<vector<int>> &matrix, int start, int sideLen) {
  // inplace rotate!
  for (int j = 0; j < sideLen - 1; j++) {
    int currentI = 0, currentJ = j;
    // perform 4 times for each element
    int nextI, nextJ;
    int currentValue = matrix[currentI + start][currentJ + start];
    for (int t = 0; t < 4; t++) {
      // compute nextI, nextJ
      switch (t) {
      case 0:
        nextI = currentJ;
        nextJ = sideLen - 1;
        break;
      case 1:
        nextI = sideLen - 1;
        nextJ = sideLen - 1 - currentI;
        break;
      case 2:
        nextI = currentJ;
        nextJ = 0;
        break;
      case 3:
        nextI = 0;
        nextJ = sideLen - 1 - currentI;
        break;
      }
      // perform move
      int nextValue = matrix[nextI + start][nextJ + start];
      matrix[nextI + start][nextJ + start] = currentValue;
      // assign next to current
      currentI = nextI;
      currentJ = nextJ;
      currentValue = nextValue;
    }
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
