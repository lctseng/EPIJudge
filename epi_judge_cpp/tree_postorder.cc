#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include <memory>
#include <stack>
#include <vector>
using std::stack;
using std::unique_ptr;
using std::vector;

struct Entry {
  // local variable
  int stage;
  const BinaryTreeNode<int> *node;
};

// We use stack and previous node pointer to simulate postorder traversal.
vector<int> PostorderTraversal(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return {};
  stack<Entry> stk;
  vector<int> res;
  stk.push({1, tree.get()});
  while (stk.size()) {
    // BE CAREFUL!
    // MUST use reference!
    auto &root = stk.top();
    if (root.stage == 2) {
      res.push_back(root.node->data);
      stk.pop();
    } else {
      root.stage = 2;
      if (root.node->right) {
        stk.push({1, root.node->right.get()});
      }
      if (root.node->left) {
        stk.push({1, root.node->left.get()});
      }
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_postorder.cc", "tree_postorder.tsv",
                         &PostorderTraversal, DefaultComparator{}, param_names);
}
