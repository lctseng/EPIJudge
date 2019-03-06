#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include <algorithm>
#include <stack>
#include <vector>
using std::make_unique;
using std::move;
using std::vector;

unique_ptr<BinaryTreeNode<int>>
Clone(const unique_ptr<BinaryTreeNode<int>> &tree) {
  if (!tree)
    return nullptr;
  return make_unique<BinaryTreeNode<int>>(tree->data, Clone(tree->left),
                                          Clone(tree->right));
}

vector<unique_ptr<BinaryTreeNode<int>>> GenerateAllBinaryTreesHelper(int first,
                                                                     int last) {
  vector<unique_ptr<BinaryTreeNode<int>>> res;
  if (first > last) {
    res.push_back(nullptr);
  } else {
    // pick a root
    for (int i = first; i <= last; i++) {
      auto leftResults = GenerateAllBinaryTreesHelper(first, i - 1);
      auto rightResults = GenerateAllBinaryTreesHelper(i + 1, last);
      for (auto &leftPtr : leftResults) {
        for (auto &rightPtr : rightResults) {
          auto root = make_unique<BinaryTreeNode<int>>(i);
          root->left = Clone(leftPtr);
          root->right = Clone(rightPtr);
          res.push_back(move(root));
        }
      }
    }
  }
  return move(res);
}

vector<unique_ptr<BinaryTreeNode<int>>> GenerateAllBinaryTrees(int num_nodes) {
  return GenerateAllBinaryTreesHelper(1, num_nodes);
}
vector<int> SerializeStructure(const unique_ptr<BinaryTreeNode<int>> &tree) {
  vector<int> result;

  std::stack<BinaryTreeNode<int> *> stack;
  stack.push(tree.get());
  while (!stack.empty()) {
    auto p = stack.top();
    stack.pop();
    result.push_back(p != nullptr);
    if (p) {
      stack.push(p->left.get());
      stack.push(p->right.get());
    }
  }
  return result;
}

vector<vector<int>> GenerateAllBinaryTreesWrapper(TimedExecutor &executor,
                                                  int num_nodes) {
  auto result = executor.Run([&] { return GenerateAllBinaryTrees(num_nodes); });

  vector<vector<int>> serialized;
  for (auto &x : result) {
    serialized.push_back(SerializeStructure(x));
  }
  std::sort(begin(serialized), end(serialized));
  return serialized;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "num_nodes"};
  return GenericTestMain(args, "enumerate_trees.cc", "enumerate_trees.tsv",
                         &GenerateAllBinaryTreesWrapper, DefaultComparator{},
                         param_names);
}
