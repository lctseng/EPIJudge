#include "binary_tree_with_parent_prototype.h"
#include "test_framework/generic_test.h"
#include <vector>
using std::vector;

vector<int> InorderTraversal(const unique_ptr<BinaryTreeNode<int>> &tree) {
  vector<int> res;

  const BinaryTreeNode<int> *current = tree.get();
  const BinaryTreeNode<int> *prev = nullptr;

  while (current) {
    // edge case: this is leaf
    if (!current->left && !current->right) {
      res.push_back(current->data);
      prev = current;
      current = current->parent;
    }
    // if come from parent: go left
    else if (prev == current->parent) {
      if (current->left) {
        prev = current;
        current = current->left.get();
      } else {
        // no left to go, visit current and go right
        res.push_back(current->data);
        prev = current;
        current = current->right.get();
      }
    }
    // if come from left: print self and go right
    else if (prev == current->left.get()) {
      res.push_back(current->data);
      if (current->right) {
        prev = current;
        current = current->right.get();
      } else {
        // no right: turn BACK now
        prev = current;
        current = current->parent;
      }

    }
    // if come from right: go parent
    else if (prev == current->right.get()) {
      prev = current;
      current = current->parent;
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_with_parent_inorder.cc",
                         "tree_with_parent_inorder.tsv", &InorderTraversal,
                         DefaultComparator{}, param_names);
}
