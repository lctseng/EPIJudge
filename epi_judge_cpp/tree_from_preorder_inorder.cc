#include "binary_tree_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include <vector>
using std::make_unique;
using std::move;
using std::vector;

unique_ptr<BinaryTreeNode<int>> build(const vector<int> &preorder,
                                      const vector<int> &inorder, int pBegin,
                                      int iBegin, int size) {
  if (size <= 0)
    return move(unique_ptr<BinaryTreeNode<int>>{nullptr});
  else if (size == 1) {
    return move(make_unique<BinaryTreeNode<int>>(preorder[pBegin]));
  } else {
    auto root = make_unique<BinaryTreeNode<int>>(preorder[pBegin]);
    // find the root from inorder
    int iRoot;
    for (int i = 0; i < size; i++) {
      if (inorder[iBegin + i] == root->data) {
        iRoot = iBegin + i;
        break;
      }
    }
    int leftSize = iRoot - iBegin;
    int rightSize = size - leftSize - 1;
    root->left = move(build(preorder, inorder, pBegin + 1, iBegin, leftSize));
    root->right = move(
        build(preorder, inorder, pBegin + leftSize + 1, iRoot + 1, rightSize));
    return move(root);
  }
}

unique_ptr<BinaryTreeNode<int>>
BinaryTreeFromPreorderInorder(const vector<int> &preorder,
                              const vector<int> &inorder) {
  return move(build(preorder, inorder, 0, 0, preorder.size()));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"preorder", "inorder"};
  return GenericTestMain(
      args, "tree_from_preorder_inorder.cc", "tree_from_preorder_inorder.tsv",
      &BinaryTreeFromPreorderInorder, DefaultComparator{}, param_names);
}
