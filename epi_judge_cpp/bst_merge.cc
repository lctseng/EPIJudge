#include "bst_prototype_shared_ptr.h"
#include "test_framework/generic_test.h"
#include <memory>
using std::make_shared;
using std::shared_ptr;

// Count the list length till end.
int CountLength(shared_ptr<BstNode<int>> L) {
  int length = 0;
  while (L) {
    ++length, L = L->right;
  }
  return length;
}

shared_ptr<BstNode<int>>
BuildBSTFromSortedDoublyListHelper(shared_ptr<BstNode<int>> *l_ptr,
                                   int length) {
  if (!length)
    return nullptr;
  else {
    int rightSize = (length - 1) / 2;
    int leftSize = length - 1 - rightSize;
    auto left = BuildBSTFromSortedDoublyListHelper(l_ptr, leftSize);
    shared_ptr<BstNode<int>> root = *l_ptr;
    *l_ptr = (*l_ptr)->right;
    auto right = BuildBSTFromSortedDoublyListHelper(l_ptr, rightSize);
    root->left = left;
    root->right = right;
    return root;
  }
}

shared_ptr<BstNode<int>>
BuildBSTFromSortedDoublyList(shared_ptr<BstNode<int>> l, int length) {
  return BuildBSTFromSortedDoublyListHelper(&l, length);
}

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

shared_ptr<BstNode<int>> MergeTwoSortedList(shared_ptr<BstNode<int>> listA,
                                            shared_ptr<BstNode<int>> listB) {
  // dummy head
  auto dummyHead = make_shared<BstNode<int>>(0);
  auto current = dummyHead;
  while (listA && listB) {
    // pick one
    if (listA->data < listB->data) {
      current->right = listA;
      listA = listA->right;
    } else {
      current->right = listB;
      listB = listB->right;
    }
    current = current->right;
  }
  if (listA) {
    current->right = listA;
  } else if (listB) {
    current->right = listB;
  }
  // attach remaining if needed
  return dummyHead->right;
}

shared_ptr<BstNode<int>> MergeTwoBSTs(shared_ptr<BstNode<int>> A,
                                      shared_ptr<BstNode<int>> B) {
  // convert two bst to doubly linked list
  auto listA = BSTToDoublyLinkedList(A);
  auto listB = BSTToDoublyLinkedList(B);

  // merge two list
  auto newHead = MergeTwoSortedList(listA, listB);
  int length = CountLength(newHead);
  // construct bst from it
  return BuildBSTFromSortedDoublyList(newHead, length);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "B"};
  return GenericTestMain(args, "bst_merge.cc", "bst_merge.tsv", &MergeTwoBSTs,
                         DefaultComparator{}, param_names);
}
