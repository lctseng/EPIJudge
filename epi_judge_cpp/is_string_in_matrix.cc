#include "test_framework/generic_test.h"
#include <unordered_set>
#include <vector>
using std::hash;
using std::size;
using std::unordered_set;
using std::vector;

struct Entry {
  int i, j;
};

struct hashFunc {
  size_t operator()(const Entry &ent) const {
    return (hash<int>{}(ent.i) << 1) ^ hash<int>{}(ent.j);
  }
};

bool operator==(const Entry &a, const Entry &b) {
  return a.i == b.i && a.j == b.j;
}

bool IsPatternContainedInGrid(const vector<vector<int>> &grid,
                              const vector<int> &pattern) {
  int m = size(grid);
  if (pattern.empty())
    return true;
  if (!m)
    return false;
  int n = size(grid[0]);
  if (!n)
    return false;
  unordered_set<Entry, hashFunc> current;
  // find all start point
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (grid[i][j] == pattern[0]) {
        current.insert({i, j});
      }
    }
  }
  // printf("num of init entry: %lu\n", q.size());
  // extend size(pattern) - 1 times. if still not empty, we are done
  int patternSize = size(pattern);
  for (int index = 1; index < patternSize; index++) {
    if (current.empty())
      return false;
    // extend every element currently
    unordered_set<Entry, hashFunc> next;
    for (auto &ent : current) {
      int i = ent.i, j = ent.j;
      // check exist pattern[index] in 4 dir?
      if (i > 0 && grid[i - 1][j] == pattern[index]) {
        next.insert({i - 1, j});
      }
      if (i < m - 1 && grid[i + 1][j] == pattern[index]) {
        next.insert({i + 1, j});
      }
      if (j > 0 && grid[i][j - 1] == pattern[index]) {
        next.insert({i, j - 1});
      }
      if (j < n - 1 && grid[i][j + 1] == pattern[index]) {
        next.insert({i, j + 1});
      }
    }
    current = std::move(next);
  }
  return !current.empty();
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"grid", "pattern"};
  return GenericTestMain(args, "is_string_in_matrix.cc",
                         "is_string_in_matrix.tsv", &IsPatternContainedInGrid,
                         DefaultComparator{}, param_names);
}
