#include "bst_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include <memory>
using std::unique_ptr;

int IsPossibleAncestorOf(const unique_ptr<BstNode<int>> &ancestor,
                         const unique_ptr<BstNode<int>> &target) {
  // search until reach the null
  BstNode<int> *current = ancestor.get();
  int distance = 0;
  while (current) {
    if (current->data > target->data) {
      current = current->left.get();
    } else if (current->data < target->data) {
      current = current->right.get();
    } else {
      return distance;
    }
    ++distance;
  }
  // not found
  return -1;
}

bool PairIncludesAncestorAndDescendantOfM(
    const unique_ptr<BstNode<int>> &possible_anc_or_desc_0,
    const unique_ptr<BstNode<int>> &possible_anc_or_desc_1,
    const unique_ptr<BstNode<int>> &middle) {
  // brute force try :P
  // is 0->M->1?
  if (IsPossibleAncestorOf(possible_anc_or_desc_0, middle) >= 0 &&
      IsPossibleAncestorOf(middle, possible_anc_or_desc_1) >= 1) {
    return true;
  }

  // is 1->M->0?
  if (IsPossibleAncestorOf(possible_anc_or_desc_1, middle) >= 0 &&
      IsPossibleAncestorOf(middle, possible_anc_or_desc_0) >= 1) {
    return true;
  }
  // otherwise
  return false;
}
bool PairIncludesAncestorAndDescendantOfMWrapper(
    TimedExecutor &executor, const unique_ptr<BstNode<int>> &tree,
    int possible_anc_or_desc_0, int possible_anc_or_desc_1, int middle) {
  auto &candidate0 = MustFindNode(tree, possible_anc_or_desc_0);
  auto &candidate1 = MustFindNode(tree, possible_anc_or_desc_1);
  auto &middle_node = MustFindNode(tree, middle);
  return executor.Run([&] {
    return PairIncludesAncestorAndDescendantOfM(candidate0, candidate1,
                                                middle_node);
  });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree",
                                       "possible_anc_or_desc_0",
                                       "possible_anc_or_desc_1", "middle"};
  return GenericTestMain(args, "descendant_and_ancestor_in_bst.cc",
                         "descendant_and_ancestor_in_bst.tsv",
                         &PairIncludesAncestorAndDescendantOfMWrapper,
                         DefaultComparator{}, param_names);
}
