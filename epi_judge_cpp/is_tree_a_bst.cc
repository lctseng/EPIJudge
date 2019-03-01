#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include <climits>
#include <memory>
using std::max;
using std::min;
using std::unique_ptr;

struct Status {
  bool isBST;
  int minValue;
  int maxValue;
};

Status IsBinaryTreeBSTHelper(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return {true, INT_MAX, INT_MIN};
  else if (!tree->left && !tree->right) {
    return {true, tree->data, tree->data};
  }
  auto leftStatus = IsBinaryTreeBSTHelper(tree->left);
  if (!leftStatus.isBST || tree->data < leftStatus.maxValue)
    return {false, 0, 0};
  auto rightStatus = IsBinaryTreeBSTHelper(tree->right);
  if (!rightStatus.isBST || tree->data > rightStatus.minValue)
    return {false, 0, 0};

  return {true, min(tree->data, leftStatus.minValue),
          max(tree->data, rightStatus.maxValue)};
}

bool IsBinaryTreeBST(const unique_ptr<BinaryTreeNode<int>> &tree) {
  return IsBinaryTreeBSTHelper(tree).isBST;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_a_bst.cc", "is_tree_a_bst.tsv",
                         &IsBinaryTreeBST, DefaultComparator{}, param_names);
}
