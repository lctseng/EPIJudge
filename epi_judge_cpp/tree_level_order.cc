#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include <memory>
#include <queue>
#include <vector>
using std::move;
using std::queue;
using std::unique_ptr;
using std::vector;

vector<vector<int>>
BinaryTreeDepthOrder(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return {};
  queue<const BinaryTreeNode<int> *> q;
  vector<vector<int>> res;
  vector<int> currentLevel;
  q.push(tree.get());
  q.push(NULL); // sentinels
  while (q.size()) {
    auto current = q.front();
    q.pop();
    if (current) {
      currentLevel.push_back(current->data);
      if (current->left) {
        q.push(current->left.get());
      }
      if (current->right) {
        q.push(current->right.get());
      }
    } else {
      // meet end
      res.push_back(move(currentLevel));
      currentLevel.clear();
      if (q.empty())
        break;
      q.push(NULL);
    }
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_level_order.cc", "tree_level_order.tsv",
                         &BinaryTreeDepthOrder, DefaultComparator{},
                         param_names);
}
