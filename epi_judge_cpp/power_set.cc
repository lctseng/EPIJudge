#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

vector<vector<int>> GeneratePowerSet(const vector<int> &input_set) {
  vector<vector<int>> ans;
  ans.push_back({});
  // consider every number to use
  for (int n : input_set) {
    // use this number
    int curSize = ans.size();
    for (int i = 0; i < curSize; i++) {
      ans.push_back(ans[i]);
      ans.back().push_back(n);
    }
  }
  return ans;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"input_set"};
  return GenericTestMain(
      args, "power_set.cc", "power_set.tsv", &GeneratePowerSet,
      &UnorderedComparator<vector<vector<int>>>, param_names);
}
