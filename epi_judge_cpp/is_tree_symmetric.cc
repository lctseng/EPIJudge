#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

// test if two tree is symmertic
bool IsSymmetric(BinaryTreeNode<int> *left, BinaryTreeNode<int> *right) {
  if (!left && !right)
    return true;
  else if (!left && right)
    return false;
  else if (left && !right)
    return false;
  // otherwise, two tree are non-empty
  else if (left->data != right->data)
    return false;
  return IsSymmetric(left->right.get(), right->left.get()) &&
         IsSymmetric(left->left.get(), right->right.get());
}

bool IsSymmetricWrapper(const unique_ptr<BinaryTreeNode<int>> &tree) {
  return !tree || IsSymmetric(tree->left.get(), tree->right.get());
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_symmetric.cc", "is_tree_symmetric.tsv",
                         &IsSymmetricWrapper, DefaultComparator{}, param_names);
}
