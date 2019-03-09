#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include <memory>
using std::unique_ptr;

bool HasPathSum(const unique_ptr<BinaryTreeNode<int>> &tree,
                int remaining_weight) {
  // empty tree
  if (!tree)
    return remaining_weight == 0;
  // leaf node
  else if (!tree->left && !tree->right)
    return remaining_weight == tree->data;
  // not a leaf node
  int nextRemain = remaining_weight - tree->data;
  if (tree->left && HasPathSum(tree->left, nextRemain)) {
    return true;
  }
  if (tree->right && HasPathSum(tree->right, nextRemain)) {
    return true;
  }
  return false;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "remaining_weight"};
  return GenericTestMain(args, "path_sum.cc", "path_sum.tsv", &HasPathSum,
                         DefaultComparator{}, param_names);
}
