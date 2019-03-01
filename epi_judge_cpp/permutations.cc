#include "test_framework/generic_test.h"
#include <vector>
using std::swap;
using std::vector;

vector<vector<int>> res;

void PermutationsHelper(vector<int> &A, int begin) {
  if (begin == A.size()) {
    res.push_back(A);
  } else {
    // for position A[begin], choose one to being head
    for (int i = begin; i < A.size(); i++) {
      swap(A[i], A[begin]);
      PermutationsHelper(A, begin + 1);
      // restore
      swap(A[i], A[begin]);
    }
  }
}

// inplace perm
vector<vector<int>> Permutations(vector<int> A) {
  res.clear();
  PermutationsHelper(A, 0);
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(
      args, "permutations.cc", "permutations.tsv", &Permutations,
      &UnorderedComparator<vector<vector<int>>>, param_names);
}
