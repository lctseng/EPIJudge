#include "binary_tree_with_parent_prototype.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

BinaryTreeNode<int> *LCA(const unique_ptr<BinaryTreeNode<int>> &node0,
                         const unique_ptr<BinaryTreeNode<int>> &node1) {
  // perform a listed list loop detection
  BinaryTreeNode<int> *headA = node0.get(), *headB = node1.get();
  if (!headA || !headB)
    return nullptr;
  BinaryTreeNode<int> *tailB = headB;
  while (tailB->parent) {
    tailB = tailB->parent;
  }
  tailB->parent = headB;
  // during traversal, if fast reach null, change to headB
  BinaryTreeNode<int> *slow = nullptr, *fast = nullptr, *finder = nullptr;
  while (true) {
    if (!slow) {
      slow = headA;
    } else {
      slow = slow->parent;
    }
    if (!fast) {
      fast = headA;
    } else {
      fast = fast->parent;
    }
    fast = fast->parent;
    if (fast == slow)
      break;
  }
  while (true) {
    slow = slow->parent;
    if (!finder) {
      finder = headA;
    } else {
      finder = finder->parent;
    }
    if (slow == finder) {
      // restore and return
      tailB->parent = nullptr;
      return slow;
    }
  }
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
