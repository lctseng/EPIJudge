#include "test_framework/generic_test.h"
#include <algorithm>
#include <iterator>
#include <vector>
using std::vector;

vector<vector<int>> res;
vector<int> builder;
int boardSize;
vector<int> rowCount, dia45Count, dia135Count;

// n: remain number of queens
void NQueensHelper(int n) {
  if (n == 0) {
    res.push_back(builder);
  } else {
    // place a queen in (n-1)th col
    // try every row
    for (int i = 0; i < boardSize; i++) {
      if (!rowCount[i] && !dia45Count[n - 1 + i] &&
          !dia135Count[n - 1 - i + boardSize]) {
        // place here
        rowCount[i]++;
        dia45Count[n - 1 + i]++;
        dia135Count[n - 1 - i + boardSize]++;
        builder.push_back(i);
        NQueensHelper(n - 1);
        // restore
        rowCount[i]--;
        dia45Count[n - 1 + i]--;
        dia135Count[n - 1 - i + boardSize]--;
        builder.pop_back();
      }
    }
  }
}

vector<vector<int>> NQueens(int n) {
  if (n <= 0)
    return {};
  boardSize = n;
  res.clear();
  rowCount.assign(n, 0);
  dia45Count.assign(2 * n + 1, 0);
  dia135Count.assign(2 * n + 1, 0);
  NQueensHelper(n);
  return res;
}
bool Comp(vector<vector<int>> &a, vector<vector<int>> &b) {
  std::sort(std::begin(a), std::end(a));
  std::sort(std::begin(b), std::end(b));
  return a == b;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "n_queens.cc", "n_queens.tsv", &NQueens, &Comp,
                         param_names);
}
