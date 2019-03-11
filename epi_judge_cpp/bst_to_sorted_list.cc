#include "bst_prototype_shared_ptr.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include <memory>
#include <vector>
using std::shared_ptr;

struct Result {
  shared_ptr<BstNode<int>> head;
  shared_ptr<BstNode<int>> tail;
};

// convert the given BST to doubly linked list, returning its head and tail
Result BSTToDoublyLinkedListHelper(const shared_ptr<BstNode<int>> &tree) {
  if (!tree)
    return {nullptr, nullptr};
  else if (!tree->left && !tree->right) {
    // speed up edge case
    // leaf node
    return {tree, tree};
  } else {
    auto leftRes = BSTToDoublyLinkedListHelper(tree->left);
    auto rightRes = BSTToDoublyLinkedListHelper(tree->right);
    // concat: left tail -> this -> right head
    auto newHead = leftRes.head;
    if (!newHead) {
      // left head is null: left is nullptr!
      newHead = tree;
    } else {
      leftRes.tail->right = tree;
      tree->left = leftRes.tail;
    }
    auto newTail = rightRes.tail;
    if (!newTail) {
      newTail = tree;
    } else {
      rightRes.head->left = tree;
      tree->right = rightRes.head;
    }
    return {newHead, newTail};
  }
}

shared_ptr<BstNode<int>>
BSTToDoublyLinkedList(const shared_ptr<BstNode<int>> &tree) {
  return BSTToDoublyLinkedListHelper(tree).head;
}
std::vector<int>
BSTToDoublyLinkedListWrapper(TimedExecutor &executor,
                             const std::shared_ptr<BstNode<int>> &tree) {
  auto list = executor.Run([&] { return BSTToDoublyLinkedList(tree); });

  if (list && list->left) {
    throw TestFailure(
        "Function must return the head of the list. Left link must be null");
  }
  std::vector<int> v;
  while (list) {
    v.push_back(list->data);
    if (list->right && list->right->left != list) {
      throw TestFailure("List is ill-formed");
    }
    list = list->right;
  }
  return v;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "tree"};
  return GenericTestMain(
      args, "bst_to_sorted_list.cc", "bst_to_sorted_list.tsv",
      &BSTToDoublyLinkedListWrapper, DefaultComparator{}, param_names);
}
