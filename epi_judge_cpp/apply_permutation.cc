#include "test_framework/generic_test.h"
#include <vector>
using std::vector;
void ApplyPermutation(vector<int> *perm_ptr, vector<int> *A_ptr) {
  for (int srcIndex = 0; srcIndex < perm_ptr->size(); srcIndex++) {
    int currentVal = A_ptr->at(srcIndex);
    int currentIndex = srcIndex;
    int nextIndex, nextValue;
    while ((nextIndex = perm_ptr->at(currentIndex)) >= 0) {
      // save next value if valid
      nextValue = A_ptr->at(nextIndex);
      // go!
      A_ptr->at(nextIndex) = currentVal;
      // mark current is used
      perm_ptr->at(currentIndex) -= perm_ptr->size();
      currentVal = nextValue;
      currentIndex = nextIndex;
    }
  }
  // restore perm
  for (int &n : *perm_ptr) {
    n += perm_ptr->size();
  }
}
vector<int> ApplyPermutationWrapper(vector<int> perm, vector<int> A) {
  ApplyPermutation(&perm, &A);
  return A;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"perm", "A"};
  return GenericTestMain(args, "apply_permutation.cc", "apply_permutation.tsv",
                         &ApplyPermutationWrapper, DefaultComparator{},
                         param_names);
}
