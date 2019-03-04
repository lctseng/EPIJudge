#include "list_node.h"
#include "test_framework/generic_test.h"
#include <memory>
using std::make_shared;

shared_ptr<ListNode<int>> StableSortList(shared_ptr<ListNode<int>> L) {
  if (!L || !L->next)
    return L;
  // have two or more nodes
  // find mid point
  auto slow = L, fast = L;
  while (true) {
    fast = fast->next;
    if (!fast)
      break;
    fast = fast->next;
    if (!fast)
      break;
    slow = slow->next;
  }
  // slow is now mid point. the slow->next is new head
  auto leftHead = L, rightHead = slow->next;
  slow->next = nullptr;
  leftHead = StableSortList(leftHead);
  rightHead = StableSortList(rightHead);
  // merge !
  shared_ptr<ListNode<int>> dummyHead = make_shared<ListNode<int>>(0);
  shared_ptr<ListNode<int>> current = dummyHead;
  while (leftHead && rightHead) {
    if (leftHead->data <= rightHead->data) {
      current->next = leftHead;
      leftHead = leftHead->next;
    } else {
      current->next = rightHead;
      rightHead = rightHead->next;
    }
    current = current->next;
  }
  // concat remaining
  if (leftHead) {
    current->next = leftHead;
  } else if (rightHead) {
    current->next = rightHead;
  }
  return dummyHead->next;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "sort_list.cc", "sort_list.tsv", &StableSortList,
                         DefaultComparator{}, param_names);
}
