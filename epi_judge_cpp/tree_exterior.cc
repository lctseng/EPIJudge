#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <vector>
using std::move;
using std::vector;

bool IsLeaf(const unique_ptr<BinaryTreeNode<int>> &node) {
  return !node->left && !node->right;
}

// BE CAREFUL!
// orders MATTER!
// for left: current->right->left
void AddLeftBoundary(vector<const unique_ptr<BinaryTreeNode<int>> *> &res,
                     const unique_ptr<BinaryTreeNode<int>> &tree,
                     bool IsBoundary) {
  if (!tree)
    return;
  // add if this is already marked boundary
  // or it is a leaf node
  if (IsBoundary || IsLeaf(tree)) {
    res.push_back(&tree);
  }

  if (tree->left) {
    AddLeftBoundary(res, tree->left,
                    IsBoundary); // left child inherit the boundary condition
  }
  if (tree->right) {
    // right child inherit the boundary condition, plus the left child is empty
    AddLeftBoundary(res, tree->right, IsBoundary && !tree->left);
  }
}

// for left: left->right->current
void AddRightBoundary(vector<const unique_ptr<BinaryTreeNode<int>> *> &res,
                      const unique_ptr<BinaryTreeNode<int>> &tree,
                      bool IsBoundary) {
  if (!tree)
    return;
  if (tree->left) {
    // left child inherit the boundary condition, plus the right child is empty
    AddRightBoundary(res, tree->left, IsBoundary && !tree->right);
  }

  if (tree->right) {
    AddRightBoundary(res, tree->right,
                     IsBoundary); // right child inherit the boundary condition
  }

  // add if this is already marked boundary
  // or it is a leaf node
  if (IsBoundary || IsLeaf(tree)) {
    res.push_back(&tree);
  }
}

vector<const unique_ptr<BinaryTreeNode<int>> *>
ExteriorBinaryTree(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return {};
  // add root, handle left child and right child
  vector<const unique_ptr<BinaryTreeNode<int>> *> res;
  res.push_back(&tree);
  AddLeftBoundary(res, tree->left, true);
  AddRightBoundary(res, tree->right, true);

  return move(res);
}
vector<int>
CreateOutputVector(const vector<const unique_ptr<BinaryTreeNode<int>> *> &L) {
  if (std::find(std::begin(L), std::end(L), nullptr) != std::end(L)) {
    throw TestFailure("Resulting list contains nullptr");
  }
  std::vector<int> output;
  for (const auto *l : L) {
    output.push_back((*l)->data);
  }
  return output;
}

vector<int>
ExteriorBinaryTreeWrapper(TimedExecutor &executor,
                          const unique_ptr<BinaryTreeNode<int>> &tree) {
  auto result = executor.Run([&] { return ExteriorBinaryTree(tree); });

  return CreateOutputVector(result);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree"};
  return GenericTestMain(args, "tree_exterior.cc", "tree_exterior.tsv",
                         &ExteriorBinaryTreeWrapper, DefaultComparator{},
                         param_names);
}
