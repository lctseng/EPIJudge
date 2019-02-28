#include "binary_tree_with_parent_prototype.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::abs;
using std::swap;

int getDepth(BinaryTreeNode<int> *node) {
  int count = 0;
  while (node) {
    node = node->parent;
    ++count;
  }
  return count;
}

BinaryTreeNode<int> *LCA(const unique_ptr<BinaryTreeNode<int>> &node0,
                         const unique_ptr<BinaryTreeNode<int>> &node1) {
  // perform a listed list loop detection
  int depth0 = getDepth(node0.get()), depth1 = getDepth(node1.get());
  BinaryTreeNode<int> *deepNode = node0.get(), *swallowNode = node1.get();
  if (depth1 > depth0) {
    swap(deepNode, swallowNode);
  }
  int depthDiff = abs(depth1 - depth0);
  // go from depth node first
  while (depthDiff-- > 0) {
    deepNode = deepNode->parent;
  }
  // go together
  while (deepNode != swallowNode) {
    deepNode = deepNode->parent;
    swallowNode = swallowNode->parent;
  }
  return deepNode;
}
int LcaWrapper(TimedExecutor &executor,
               const unique_ptr<BinaryTreeNode<int>> &tree, int key0,
               int key1) {
  const unique_ptr<BinaryTreeNode<int>> &node0 = MustFindNode(tree, key0);
  const unique_ptr<BinaryTreeNode<int>> &node1 = MustFindNode(tree, key1);

  auto result = executor.Run([&] { return LCA(node0, node1); });

  if (!result) {
    throw TestFailure("Result can not be nullptr");
  }
  return result->data;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree", "key0", "key1"};
  return GenericTestMain(args, "lowest_common_ancestor_with_parent.cc",
                         "lowest_common_ancestor.tsv", &LcaWrapper,
                         DefaultComparator{}, param_names);
}
