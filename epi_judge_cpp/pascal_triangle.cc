#include "test_framework/generic_test.h"
#include <vector>
using std::move;
using std::vector;
vector<vector<int>> GeneratePascalTriangle(int num_rows) {
  if (num_rows == 0)
    return {};
  vector<vector<int>> res;
  // base case
  res.emplace_back(1, 1);
  for (int i = 2; i <= num_rows; i++) {
    // create this layer
    vector<int> current;
    auto &prev = res.back();
    // gen same as prev layer
    for (int j = 0; j < prev.size(); j++) {
      current.emplace_back(prev[j] + (j > 0 ? prev[j - 1] : 0));
    }
    // final extra element
    current.emplace_back(1);
    // store!
    res.emplace_back(move(current));
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_rows"};
  return GenericTestMain(args, "pascal_triangle.cc", "pascal_triangle.tsv",
                         &GeneratePascalTriangle, DefaultComparator{},
                         param_names);
}
