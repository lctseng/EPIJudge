#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include <string>
#include <vector>
using std::make_unique;
using std::move;
using std::string;
using std::vector;

// recursive
// return: the root of the tree, also update the index of used elements

unique_ptr<BinaryTreeNode<int>>
ReconstructPreorder(const vector<int *> &preorder, int &nextIndex) {
  // check out of index
  if (nextIndex >= preorder.size()) {
    return nullptr;
  } else {
    auto valPtr = preorder[nextIndex++];
    if (valPtr) {
      auto root = make_unique<BinaryTreeNode<int>>(*valPtr);
      root->left = move(ReconstructPreorder(preorder, nextIndex));
      root->right = move(ReconstructPreorder(preorder, nextIndex));
      return move(root);
    } else {
      return nullptr;
    }
  }
}

unique_ptr<BinaryTreeNode<int>>
ReconstructPreorder(const vector<int *> &preorder) {
  int nextIndex = 0;
  return move(ReconstructPreorder(preorder, nextIndex));
}
unique_ptr<BinaryTreeNode<int>>
ReconstructPreorderWrapper(TimedExecutor &executor,
                           const vector<string> &preorder) {
  vector<int> values;
  vector<int *> ptrs;
  values.reserve(preorder.size());
  for (auto &s : preorder) {
    if (s == "null") {
      ptrs.push_back(nullptr);
    } else {
      int i = std::stoi(s);
      values.push_back(i);
      ptrs.push_back(&values.back());
    }
  }

  return executor.Run([&] { return ReconstructPreorder(ptrs); });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "preorder"};
  return GenericTestMain(args, "tree_from_preorder_with_null.cc",
                         "tree_from_preorder_with_null.tsv",
                         &ReconstructPreorderWrapper, DefaultComparator{},
                         param_names);
}
