#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include <memory>
#include <stack>
#include <vector>
using std::stack;
using std::unique_ptr;
using std::vector;

void pushAll(stack<BinaryTreeNode<int> *> &stk, BinaryTreeNode<int> *root) {
  BinaryTreeNode<int> *current = root;
  while (current) {
    stk.push(current);
    current = current->left.get();
  }
}

vector<int> InorderTraversal(unique_ptr<BinaryTreeNode<int>> &tree) {
  stack<BinaryTreeNode<int> *> stk;
  pushAll(stk, tree.get());
  vector<int> result;
  while (stk.size()) {
    auto current = stk.top();
    stk.pop();
    result.push_back(current->data);
    pushAll(stk, current->right.get());
  }
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_inorder.cc", "tree_inorder.tsv",
                         &InorderTraversal, DefaultComparator{}, param_names);
}
