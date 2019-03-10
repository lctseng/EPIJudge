#include "list_node.h"
#include "test_framework/generic_test.h"
#include <memory>
using std::make_shared;
using std::shared_ptr;
using std::unique_ptr;

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
  // use slow/fast to find split point
  // fast go first, then the slow->next is the new start point
  ListNode<int> *slow = L.get(), *fast = L.get();
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
  }
  // reverse latter part (use copy)
  auto latterHead = slow->next;
  slow->next = nullptr;
  auto reversedHead = ReverseList(latterHead);
  // merge! (when odd, first part is larger than second part)
  auto newCurrent = L;
  while (newCurrent && reversedHead) {
    // insert this between newCurrent and newCurrent->next
    auto nextNext = newCurrent->next;
    newCurrent->next = reversedHead;
    reversedHead = reversedHead->next;
    newCurrent->next->next = nextNext;
    newCurrent = nextNext;
  }
  return L;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "zip_list.cc", "zip_list.tsv",
                         &ZippingLinkedList, DefaultComparator{}, param_names);
}
