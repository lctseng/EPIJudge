#include "test_framework/generic_test.h"
#include <tuple>
#include <unordered_map>
#include <vector>
using std::get;
using std::hash;
using std::make_tuple;
using std::size;
using std::tuple;
using std::unordered_map;
using std::vector;

template <> struct std::hash<tuple<int, int, int>> {
  size_t operator()(const tuple<int, int, int> &t) const {
    return (std::hash<int>{}(get<0>(t)) << 2) ^
           (std::hash<int>{}(get<1>(t)) << 1) ^ (std::hash<int>{}(get<2>(t)));
  }
};

int m, n;
unordered_map<tuple<int, int, int>, bool> cache;

bool IsPatternContainedInGridHelper(const vector<vector<int>> &grid,
                                    const vector<int> &pattern, int i, int j,
                                    int k) {
  if (k == pattern.size())
    return true;
  if (i < 0 || i >= m || j < 0 || j >= n)
    return false;

  auto key = make_tuple(i, j, k);
  auto it = cache.find(key);
  if (it != cache.end()) {
    return it->second;
  } else {
    // compute new entry
    if (grid[i][j] == pattern[k]) {
      // try explore nearby
      return cache[key] =
                 IsPatternContainedInGridHelper(grid, pattern, i + 1, j,
                                                k + 1) ||
                 IsPatternContainedInGridHelper(grid, pattern, i, j + 1,
                                                k + 1) ||
                 IsPatternContainedInGridHelper(grid, pattern, i - 1, j,
                                                k + 1) ||
                 IsPatternContainedInGridHelper(grid, pattern, i, j - 1, k + 1);
    } else {
      // not exist
      return false;
    }
  }
}

bool IsPatternContainedInGrid(const vector<vector<int>> &grid,
                              const vector<int> &pattern) {
  // dp: i, j, k
  // start from grip[i][j], contains pattern[k:]
  if (pattern.empty())
    return true; // empty pattern always true
  m = grid.size();
  if (!m)
    return false;
  n = grid[0].size();
  if (!n)
    return false;
  // non-empty grid and non-empty pattern
  cache.clear();
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (IsPatternContainedInGridHelper(grid, pattern, i, j, 0)) {
        return true;
      }
    }
  }
  return false;
}

// Prev backup
// typedef tuple<int, int, int> Entry;

// template <> struct hash<Entry> {
//   size_t operator()(const Entry &ent) const {
//     return (hash<int>{}(get<0>(ent)) << 2) ^ (hash<int>{}(get<1>(ent)) << 1)
//     ^
//            (hash<int>{}(get<2>(ent)));
//   }
// };

// int m, n;

// bool IsPatternContainedInGridHelper(unordered_map<Entry, bool> &cache,
//                                     const vector<vector<int>> &grid,
//                                     const vector<int> &pattern, int i, int j,
//                                     int index) {
//   // grid range check
//   if (i < 0 || i >= m || j < 0 || j >= n)
//     return false;
//   // index is gauranteed to in the range
//   if (pattern[index] != grid[i][j])
//     return false;
//   auto cacheIt = cache.find({i, j, index});
//   if (cacheIt != cache.end())
//     return cacheIt->second;
//   // this block matched, search 4-dir
//   int nextIndex = index + 1;
//   if (nextIndex < size(pattern)) {
//     // need recursive search. cache the result
//     return cache[{i, j, index}] =
//                IsPatternContainedInGridHelper(cache, grid, pattern, i - 1, j,
//                                               nextIndex) ||
//                IsPatternContainedInGridHelper(cache, grid, pattern, i + 1, j,
//                                               nextIndex) ||
//                IsPatternContainedInGridHelper(cache, grid, pattern, i, j - 1,
//                                               nextIndex) ||
//                IsPatternContainedInGridHelper(cache, grid, pattern, i, j + 1,
//                                               nextIndex);
//   } else {
//     // next index not exist and we have matched current entry
//     // Yay!
//     return true;
//   }
// }

// bool IsPatternContainedInGrid(const vector<vector<int>> &grid,
//                               const vector<int> &pattern) {
//   m = size(grid);
//   if (pattern.empty())
//     return true;
//   if (!m)
//     return false;
//   n = size(grid[0]);
//   if (!n)
//     return false;
//   unordered_map<Entry, bool> cache;
//   // try every start point
//   for (int i = 0; i < m; i++) {
//     for (int j = 0; j < n; j++) {
//       if (IsPatternContainedInGridHelper(cache, grid, pattern, i, j, 0)) {
//         return true;
//       }
//     }
//   }
//   return false;
// }

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"grid", "pattern"};
  return GenericTestMain(args, "is_string_in_matrix.cc",
                         "is_string_in_matrix.tsv", &IsPatternContainedInGrid,
                         DefaultComparator{}, param_names);
}
