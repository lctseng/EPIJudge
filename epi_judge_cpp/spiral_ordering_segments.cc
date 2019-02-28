#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

void OneLoop(vector<vector<int>> &square_matrix, vector<int> &res, int start,
             int sideLen) {
  if (sideLen == 1)
    res.push_back(square_matrix[start][start]);
  else {
    // go 4 sides, skip last one
    int maxIndex = start + sideLen - 1;
    for (int j = start; j < maxIndex; j++)
      res.push_back(square_matrix[start][j]);
    for (int i = start; i < maxIndex; i++)
      res.push_back(square_matrix[i][maxIndex]);
    for (int j = maxIndex; j > start; j--)
      res.push_back(square_matrix[maxIndex][j]);
    for (int i = maxIndex; i > start; i--)
      res.push_back(square_matrix[i][start]);
  }
}

vector<int> MatrixInSpiralOrder(vector<vector<int>> square_matrix) {
  vector<int> res;
  int start = 0;
  int sideLen = square_matrix.size();
  for (; sideLen > 0; sideLen -= 2) {
    OneLoop(square_matrix, res, start, sideLen);
    start++;
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"square_matrix"};
  return GenericTestMain(args, "spiral_ordering_segments.cc",
                         "spiral_ordering_segments.tsv", &MatrixInSpiralOrder,
                         DefaultComparator{}, param_names);
}
