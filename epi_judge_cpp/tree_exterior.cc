#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <deque>
#include <vector>
using std::deque;
using std::move;
using std::vector;

bool IsLeaf(const unique_ptr<BinaryTreeNode<int>> &current) {
  if (!current)
    return false;
  return !current->left && !current->right;
}

void findLeftBoundary(deque<const unique_ptr<BinaryTreeNode<int>> *> &res,
                      const unique_ptr<BinaryTreeNode<int>> &current,
                      bool isBoundary) {
  if (!current)
    return;
  if (isBoundary && !IsLeaf(current)) {
    res.push_back(&current);
  }
  if (current->left) {
    findLeftBoundary(res, current->left, isBoundary);
  }
  findLeftBoundary(res, current->right, isBoundary && !current->left);
}

void findRightBoundary(deque<const unique_ptr<BinaryTreeNode<int>> *> &res,
                       const unique_ptr<BinaryTreeNode<int>> &current,
                       bool isBoundary) {
  if (!current)
    return;
  if (isBoundary && !IsLeaf(current)) {
    res.push_back(&current);
  }
  if (current->right) {
    findRightBoundary(res, current->right, isBoundary);
  }
  findRightBoundary(res, current->left, isBoundary && !current->right);
}

void findLeaves(deque<const unique_ptr<BinaryTreeNode<int>> *> &res,
                const unique_ptr<BinaryTreeNode<int>> &current) {
  if (!current)
    return;
  if (!current->left && !current->right) {
    res.push_back(&current);
  } else {
    // not a leaf if has at least 1 child
    findLeaves(res, current->left);
    findLeaves(res, current->right);
  }
}

// BE CAREFUL the duplicate handling
vector<const unique_ptr<BinaryTreeNode<int>> *>
ExteriorBinaryTree(const unique_ptr<BinaryTreeNode<int>> &tree) {
  // edge case: 0 or 1 nodes
  if (!tree)
    return {};
  if (IsLeaf(tree))
    return {&tree};
  // steps
  // find left boundary
  deque<const unique_ptr<BinaryTreeNode<int>> *> leftBoundary;
  findLeftBoundary(leftBoundary, tree->left, true);
  // find right boundary
  deque<const unique_ptr<BinaryTreeNode<int>> *> rightBoundary;
  findRightBoundary(rightBoundary, tree->right, true);
  // find leaves
  deque<const unique_ptr<BinaryTreeNode<int>> *> leaves;
  findLeaves(leaves, tree);
  // concat: root + left + leaves + right(reversed)
  vector<const unique_ptr<BinaryTreeNode<int>> *> res;
  res.push_back(&tree);
  res.insert(res.end(), leftBoundary.begin(), leftBoundary.end());
  res.insert(res.end(), leaves.begin(), leaves.end());
  res.insert(res.end(), rightBoundary.rbegin(), rightBoundary.rend());
  return res;
}

// bool IsLeaf(const unique_ptr<BinaryTreeNode<int>> &node) {
//   return !node->left && !node->right;
// }

// // BE CAREFUL!
// // orders MATTER!
// // for left: current->right->left
// void AddLeftBoundary(vector<const unique_ptr<BinaryTreeNode<int>> *> &res,
//                      const unique_ptr<BinaryTreeNode<int>> &tree,
//                      bool IsBoundary) {
//   if (!tree)
//     return;
//   // add if this is already marked boundary
//   // or it is a leaf node
//   if (IsBoundary || IsLeaf(tree)) {
//     res.push_back(&tree);
//   }

//   if (tree->left) {
//     AddLeftBoundary(res, tree->left,
//                     IsBoundary); // left child inherit the boundary condition
//   }
//   if (tree->right) {
//     // right child inherit the boundary condition, plus the left child is
//     empty AddLeftBoundary(res, tree->right, IsBoundary && !tree->left);
//   }
// }

// // for left: left->right->current
// void AddRightBoundary(vector<const unique_ptr<BinaryTreeNode<int>> *> &res,
//                       const unique_ptr<BinaryTreeNode<int>> &tree,
//                       bool IsBoundary) {
//   if (!tree)
//     return;
//   if (tree->left) {
//     // left child inherit the boundary condition, plus the right child is
//     empty AddRightBoundary(res, tree->left, IsBoundary && !tree->right);
//   }

//   if (tree->right) {
//     AddRightBoundary(res, tree->right,
//                      IsBoundary); // right child inherit the boundary
//                      condition
//   }

//   // add if this is already marked boundary
//   // or it is a leaf node
//   if (IsBoundary || IsLeaf(tree)) {
//     res.push_back(&tree);
//   }
// }

// vector<const unique_ptr<BinaryTreeNode<int>> *>
// ExteriorBinaryTree(const unique_ptr<BinaryTreeNode<int>> &tree) {
//   if (!tree)
//     return {};
//   // add root, handle left child and right child
//   vector<const unique_ptr<BinaryTreeNode<int>> *> res;
//   res.push_back(&tree);
//   AddLeftBoundary(res, tree->left, true);
//   AddRightBoundary(res, tree->right, true);

//   return move(res);
// }
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
