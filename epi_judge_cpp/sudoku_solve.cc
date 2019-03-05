#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>
using std::begin;
using std::end;
using std::vector;

vector<vector<int>> colUsed, rowUsed, blockUsed;
// for blockUsed, index is i * 3 + j

// BE CAREFUL!
// block index computation! use division not modulo!
#define BLOCK_INDEX(i, j) ((i / 3) * 3 + (j / 3))

void AdvanceIndex(int &i, int &j) {
  if (++j == 9) {
    j = 0;
    ++i;
  }
}

bool SolveSudokuHelper(vector<vector<int>> &partial_assignment, int i, int j) {
  // ending check?
  if (i >= 9)
    return true;
  else {
    // i, j in range
    int nextI = i, nextJ = j;
    AdvanceIndex(nextI, nextJ);
    // only process the block is blank
    if (partial_assignment[i][j] == 0) {
      // pick a valid value for this block
      for (int n = 1; n <= 9; n++) {
        if (!rowUsed[i][n] && !colUsed[j][n] &&
            !blockUsed[BLOCK_INDEX(i, j)][n]) {
          // use
          ++rowUsed[i][n];
          ++colUsed[j][n];
          ++blockUsed[BLOCK_INDEX(i, j)][n];
          partial_assignment[i][j] = n;
          // forward
          if (SolveSudokuHelper(partial_assignment, nextI, nextJ)) {
            // force return if solved
            return true;
          }
          // restore
          --rowUsed[i][n];
          --colUsed[j][n];
          --blockUsed[BLOCK_INDEX(i, j)][n];
        }
      }
      partial_assignment[i][j] = 0;
      return false;
    } else {
      // just go next
      return SolveSudokuHelper(partial_assignment, nextI, nextJ);
    }
  }
}

bool SolveSudoku(vector<vector<int>> *partial_assignment) {
  // clear used info
  for (auto *used : {&colUsed, &rowUsed, &blockUsed}) {
    used->clear();
    for (int i = 0; i < 9; i++) {
      used->emplace_back(10, 0);
    }
  }
  // fill parital assignment
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      int val = (*partial_assignment)[i][j];
      if (val) {
        ++rowUsed[i][val];
        ++colUsed[j][val];
        ++blockUsed[BLOCK_INDEX(i, j)][val];
      }
    }
  }
  // solve!
  return SolveSudokuHelper(*partial_assignment, 0, 0);
}
vector<int> GatherColumn(const vector<vector<int>> &data, size_t i) {
  vector<int> result;
  for (auto &row : data) {
    result.push_back(row[i]);
  }
  return result;
}

vector<int> GatherSquareBlock(const vector<vector<int>> &data,
                              size_t block_size, size_t n) {
  vector<int> result;
  size_t block_x = n % block_size;
  size_t block_y = n / block_size;
  for (size_t i = block_x * block_size; i < (block_x + 1) * block_size; i++) {
    for (size_t j = block_y * block_size; j < (block_y + 1) * block_size; j++) {
      result.push_back(data[i][j]);
    }
  }

  return result;
}

void AssertUniqueSeq(const vector<int> &seq) {
  vector<bool> seen(seq.size(), false);
  for (auto &x : seq) {
    if (x == 0) {
      throw TestFailure("Cell left uninitialized");
    }
    if (x < 0 || x > seq.size()) {
      throw TestFailure("Cell value out of range");
    }
    if (seen[x - 1]) {
      throw TestFailure("Duplicate value in section");
    }
    seen[x - 1] = true;
  }
}

void SolveSudokuWrapper(TimedExecutor &executor,
                        const vector<vector<int>> &partial_assignment) {
  vector<vector<int>> solved = partial_assignment;

  executor.Run([&] { SolveSudoku(&solved); });

  if (!std::equal(begin(partial_assignment), end(partial_assignment),
                  begin(solved), end(solved), [](auto br, auto cr) {
                    return std::equal(begin(br), end(br), begin(cr), end(cr),
                                      [](int bcell, int ccell) {
                                        return bcell == 0 || bcell == ccell;
                                      });
                  }))
    throw TestFailure("Initial cell assignment has been changed");

  auto block_size = static_cast<size_t>(sqrt(solved.size()));

  for (size_t i = 0; i < solved.size(); i++) {
    AssertUniqueSeq(solved[i]);
    AssertUniqueSeq(GatherColumn(solved, i));
    AssertUniqueSeq(GatherSquareBlock(solved, block_size, i));
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "partial_assignment"};
  return GenericTestMain(args, "sudoku_solve.cc", "sudoku_solve.tsv",
                         &SolveSudokuWrapper, DefaultComparator{}, param_names);
}
