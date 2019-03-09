#include "binary_tree_with_parent_prototype.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"

BinaryTreeNode<int> *
FindSuccessor(const unique_ptr<BinaryTreeNode<int>> &node) {
  if (!node)
    return nullptr;
  if (node->right) {
    BinaryTreeNode<int> *curr = node->right.get();
    while (curr->left) {
      curr = curr->left.get();
    }
    return curr;
  } else {
    // BE CAREFUL
    // the case when right subtree is not exist
    // DO NOT just return the parent (or parent's parent)
    //    A
    //   /
    //  B
    //    \
    //     C
    //      \
    //       D
    // in this case, the successor of D is A, not C nor B
    BinaryTreeNode<int> *curr = node.get();
    BinaryTreeNode<int> *parent = node->parent;
    while (parent && curr == parent->right.get()) {
      curr = parent;
      parent = parent->parent;
    }
    return parent;
  }
}
int FindSuccessorWrapper(const unique_ptr<BinaryTreeNode<int>> &tree,
                         int node_idx) {
  auto result = FindSuccessor(MustFindNode(tree, node_idx));
  return result ? result->data : -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "node_idx"};
  return GenericTestMain(args, "successor_in_tree.cc", "successor_in_tree.tsv",
                         &FindSuccessorWrapper, DefaultComparator{},
                         param_names);
}
