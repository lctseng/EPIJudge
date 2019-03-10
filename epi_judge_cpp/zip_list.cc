#include "list_node.h"
#include "test_framework/generic_test.h"
#include <memory>
using std::make_shared;
using std::shared_ptr;

shared_ptr<ListNode<int>> ReverseList(shared_ptr<ListNode<int>> current) {
  shared_ptr<ListNode<int>> prev = nullptr;
  while (current) {
    auto next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  return prev;
}

shared_ptr<ListNode<int>> ReverseListCopy(ListNode<int> *head) {
  shared_ptr<ListNode<int>> newPrev = nullptr;
  while (head) {
    auto newCurrent = make_shared<ListNode<int>>(head->data);
    newCurrent->next = newPrev;
    newPrev = newCurrent;
    head = head->next.get();
  }
  return newPrev;
}

shared_ptr<ListNode<int>>
ZippingLinkedList(const shared_ptr<ListNode<int>> &L) {
  if (!L)
    return nullptr;
  if (!L->next)
    return make_shared<ListNode<int>>(L->data);
  // use slow/fast to find split point
  // fast go first, then the slow->next is the new start point
  const ListNode<int> *slow = L.get(), *fast = L.get();
  auto newHead = make_shared<ListNode<int>>(slow->data);
  auto newCurrent = newHead;
  while (true) {
    fast = fast->next.get();
    if (!fast) {
      break;
    }
    fast = fast->next.get();
    if (!fast) {
      break;
    }
    // meanwhile, copy the slow list
    slow = slow->next.get();
    newCurrent->next = make_shared<ListNode<int>>(slow->data);
    newCurrent = newCurrent->next;
  }
  // reverse latter part (use copy)
  auto *latterHead = slow->next.get();
  auto reversedHead = ReverseListCopy(latterHead);
  // merge! (when odd, first part is larger than second part)
  newCurrent = newHead;
  while (newCurrent && reversedHead) {
    // insert this between newCurrent and newCurrent->next
    auto nextNext = newCurrent->next;
    newCurrent->next = reversedHead;
    reversedHead = reversedHead->next;
    newCurrent->next->next = nextNext;
    newCurrent = nextNext;
  }
  return newHead;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "zip_list.cc", "zip_list.tsv",
                         &ZippingLinkedList, DefaultComparator{}, param_names);
}
