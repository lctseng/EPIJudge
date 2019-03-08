#include "test_framework/generic_test.h"
#include <vector>
using std::greater;
using std::lower_bound;
using std::reverse;
using std::size;
using std::swap;
using std::vector;

vector<int> NextPermutation(vector<int> perm) {
  // find first inverting point: O(n)
  int i = size(perm) - 2;
  for (; i >= 0; i--) {
    if (perm[i] < perm[i + 1])
      break;
  }
  // if not found, return {}
  if (i < 0)
    return {};
  // after inverting point, find first number > inverting point
  // using lower_bound with greater + go prev
  // O(logn)
  auto it =
      lower_bound(perm.begin() + i + 1, perm.end(), perm[i], greater<int>());
  --it;
  // swap that two
  swap(perm[i], *it);
  // reverse all after inverting point: O(n)
  reverse(perm.begin() + i + 1, perm.end());
  return perm;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"perm"};
  return GenericTestMain(args, "next_permutation.cc", "next_permutation.tsv",
                         &NextPermutation, DefaultComparator{}, param_names);
}
