#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>
using std::hash;
using std::make_pair;
using std::pair;
using std::queue;
using std::string;
using std::vector;

int m, n;
const unsigned char kMask = 1 << 7;
const char kMarkedWhite = 'W' | (char)kMask;
const auto shifts = {make_pair(1, 0), make_pair(0, 1), make_pair(-1, 0),
                     make_pair(0, -1)};

void BFS(vector<vector<char>> &board, int startI, int startJ) {
  queue<pair<int, int>> q;
  q.push(make_pair(startI, startJ));
  board[startI][startJ] = kMarkedWhite;
  while (q.size()) {
    const auto &[i, j] = q.front();
    q.pop();
    // explore
    for (const auto &shift : shifts) {
      auto next = make_pair(i + shift.first, j + shift.second);
      if (next.first >= 0 && next.first < m && next.second >= 0 &&
          next.second < n) {
        if (board[next.first][next.second] == 'W') {
          board[next.first][next.second] = kMarkedWhite;
          q.push(next);
        }
      }
    }
  }
}

void FillSurroundedRegions(vector<vector<char>> *board_ptr) {
  m = board_ptr->size();
  if (!m)
    return;
  n = board_ptr->at(0).size();
  if (!n)
    return;
  // start from border, mark them as Marked
  for (int i = 0; i < m; i++) {
    for (auto j : {0, n - 1}) {
      char c = (*board_ptr)[i][j];
      if (c != 'B' && c != kMarkedWhite) {
        BFS(*board_ptr, i, j);
      }
    }
  }
  for (int j = 0; j < n; j++) {
    for (auto i : {0, m - 1}) {
      char c = (*board_ptr)[i][j];
      if (c != 'B' && c != kMarkedWhite) {
        BFS(*board_ptr, i, j);
      }
    }
  }
  // mark: white -> black
  // mark: markedW -> white
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      char &c = (*board_ptr)[i][j];
      if (c == 'W')
        c = 'B';
      else if (c == kMarkedWhite)
        c = 'W';
    }
  }
}
vector<vector<string>>
FillSurroundedRegionsWrapper(TimedExecutor &executor,
                             vector<vector<string>> board) {
  vector<vector<char>> char_vector;
  char_vector.resize(board.size());
  for (int i = 0; i < board.size(); i++) {
    for (const string &s : board[i]) {
      if (s.size() != 1) {
        throw std::runtime_error("String size is not 1");
      }
      char_vector[i].push_back(s[0]);
    }
  }

  executor.Run([&] { FillSurroundedRegions(&char_vector); });

  board.clear();
  board.resize(char_vector.size(), {});
  for (int i = 0; i < board.size(); i++) {
    for (char c : char_vector[i]) {
      board[i].emplace_back(1, c);
    }
  }

  return board;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "board"};
  return GenericTestMain(
      args, "matrix_enclosed_regions.cc", "matrix_enclosed_regions.tsv",
      &FillSurroundedRegionsWrapper, DefaultComparator{}, param_names);
}
