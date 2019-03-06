#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

//
void CombinationsHelper(vector<vector<int>> &res, vector<int> &builder, int n,
                        int k, int currentN) {
  if (k > 0 && currentN > n)
    return;
  if (k == 0) {
    res.push_back(builder);
  } else {
    // use current N
    builder.push_back(currentN);
    CombinationsHelper(res, builder, n, k - 1, currentN + 1);
    builder.pop_back();
    // dont use current N
    CombinationsHelper(res, builder, n, k, currentN + 1);
  }
}

vector<vector<int>> Combinations(int n, int k) {
  vector<vector<int>> res;
  vector<int> builder;
  CombinationsHelper(res, builder, n, k, 1);
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "k"};
  return GenericTestMain(
      args, "combinations.cc", "combinations.tsv", &Combinations,
      &UnorderedComparator<std::vector<std::vector<int>>>, param_names);
}
