#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include <stack>
#include <vector>
using std::stack;
using std::vector;

vector<int> PreorderTraversal(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return {};
  stack<BinaryTreeNode<int> *> stk;
  vector<int> res;
  stk.push(tree.get());
  while (stk.size()) {
    // handle this
    auto *curr = stk.top();
    stk.pop();
    res.push_back(curr->data);
    if (curr->right) {
      stk.push(curr->right.get());
    }
    if (curr->left) {
      stk.push(curr->left.get());
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_preorder.cc", "tree_preorder.tsv",
                         &PreorderTraversal, DefaultComparator{}, param_names);
}
