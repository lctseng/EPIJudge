#include "test_framework/generic_test.h"
#include <vector>
using std::vector;
vector<int> PlusOne(vector<int> A) {
  vector<int> res(A.begin(), A.end());
  res.back()++;
  for (int i = res.size() - 1; i > 0; i--) {
    if (res[i] >= 10) {
      res[i - 1]++;
      res[i] -= 10;
    } else {
      break;
    }
  }
  if (res[0] >= 10) {
    res[0] -= 10;
    res.insert(res.begin(), 1);
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "int_as_array_increment.cc",
                         "int_as_array_increment.tsv", &PlusOne,
                         DefaultComparator{}, param_names);
}
