#include "test_framework/generic_test.h"
#include <vector>
using std::move;
using std::vector;

vector<int> builder;
vector<vector<int>> res;

void CombinationsHelper(int n, int k) {
  if (n < k)
    return;
  if (k == 0) {
    res.push_back(builder);
  } else {
    // use n
    builder.push_back(n);
    CombinationsHelper(n - 1, k - 1);
    builder.pop_back();
    // not use n
    CombinationsHelper(n - 1, k);
  }
}

vector<vector<int>> Combinations(int n, int k) {
  if (k == 0)
    return {{}};
  res.clear();
  builder.clear();
  CombinationsHelper(n, k);
  return move(res);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "k"};
  return GenericTestMain(
      args, "combinations.cc", "combinations.tsv", &Combinations,
      &UnorderedComparator<std::vector<std::vector<int>>>, param_names);
}
