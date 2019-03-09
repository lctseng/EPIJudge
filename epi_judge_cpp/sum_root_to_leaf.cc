#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

int sum;
int builder;

void SumRootToLeafHelper(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return;
  else if (!tree->left && !tree->right) {
    // leaf node
    sum += (builder << 1) | tree->data;
  } else {
    // non leaf
    builder = (builder << 1) | tree->data;
    if (tree->left) {
      SumRootToLeafHelper(tree->left);
    }
    if (tree->right) {
      SumRootToLeafHelper(tree->right);
    }
    // restore
    builder >>= 1;
  }
}

int SumRootToLeaf(const unique_ptr<BinaryTreeNode<int>> &tree) {
  sum = 0;
  builder = 0;
  SumRootToLeafHelper(tree);
  return sum;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "sum_root_to_leaf.cc", "sum_root_to_leaf.tsv",
                         &SumRootToLeaf, DefaultComparator{}, param_names);
}
