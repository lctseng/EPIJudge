#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

using std::max;

struct Entry {
  bool balanced;
  int height;
};

Entry IsBalancedRec(const BinaryTreeNode<int> *current) {
  if (!current) {
    return {true, 0};
  }
  auto leftRes = IsBalancedRec(current->left.get());
  if (!leftRes.balanced)
    return {false, 0};
  auto rightRes = IsBalancedRec(current->right.get());
  if (!rightRes.balanced)
    return {false, 0};
  // both subtree is balanced, check height diff
  int diff = abs(leftRes.height - rightRes.height);
  if (diff > 1)
    return {false, 0};
  return {true, max(leftRes.height, rightRes.height) + 1};
}

bool IsBalanced(const unique_ptr<BinaryTreeNode<int>> &tree) {
  const BinaryTreeNode<int> *current = tree.get();
  return IsBalancedRec(current).balanced;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_balanced.cc", "is_tree_balanced.tsv",
                         &IsBalanced, DefaultComparator{}, param_names);
}
