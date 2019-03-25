#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include <climits>
#include <memory>
#include <queue>
using std::max;
using std::min;
using std::queue;
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

struct QueueEntry {
  BinaryTreeNode<int> *node;
  int upper;
  int lower;
};

bool IsBinaryTreeBSTIterative(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return true;
  queue<QueueEntry> q;
  q.push({tree.get(), INT_MAX, INT_MIN});
  while (q.size()) {
    auto ent = q.front();
    q.pop();
    if (ent.node->data > ent.upper || ent.node->data < ent.lower)
      return false;
    if (ent.node->left) {
      q.push({ent.node->left.get(), ent.node->data, ent.lower});
    }
    if (ent.node->right) {
      q.push({ent.node->right.get(), ent.upper, ent.node->data});
    }
  }
  return true;
}

// bool IsBinaryTreeBST(const unique_ptr<BinaryTreeNode<int>> &tree) {
//   // return IsBinaryTreeBSTHelper(tree).isBST;
//   return IsBinaryTreeBSTIterative(tree);
// }

// solve it recursively
bool IsBinaryTreeBSTRecursiveHelper(const unique_ptr<BinaryTreeNode<int>> &tree,
                                    int lower, int upper) {
  if (!tree)
    return true;
  if (tree->data < lower || tree->data > upper)
    return false;
  // current node fit
  return IsBinaryTreeBSTRecursiveHelper(tree->left, lower, tree->data) &&
         IsBinaryTreeBSTRecursiveHelper(tree->right, tree->data, upper);
}

bool IsBinaryTreeBST(const unique_ptr<BinaryTreeNode<int>> &tree) {
  return IsBinaryTreeBSTRecursiveHelper(tree, INT_MIN, INT_MAX);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "is_tree_a_bst.cc", "is_tree_a_bst.tsv",
                         &IsBinaryTreeBST, DefaultComparator{}, param_names);
}
