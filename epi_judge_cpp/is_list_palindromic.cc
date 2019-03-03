#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> reverseList(shared_ptr<ListNode<int>> oldHead) {
  if (!oldHead || !oldHead->next)
    return oldHead;
  shared_ptr<ListNode<int>> prev = nullptr, current = oldHead;
  while (current) {
    auto next = current->next;
    current->next = prev;
    prev = current;
    current = next;
  }
  return prev;
}

bool IsLinkedListAPalindrome(shared_ptr<ListNode<int>> L) {
  if (!L || !L->next)
    return true; // edge case for 0 and 1 node
  // find the mid point, record 'checkEnd' and laterHead
  shared_ptr<ListNode<int>> checkEnd = nullptr, laterHead = nullptr;
  auto slow = L, fast = L;
  while (true) {
    fast = fast->next;
    if (!fast) {
      // odd terminated
      checkEnd = slow;
      laterHead = slow->next;
      break;
    }
    fast = fast->next;
    if (!fast) {
      // even terminate
      laterHead = checkEnd = slow->next;
      break;
    }
    slow = slow->next;
  }
  // reverse laterHead, get newHead
  auto newHead = reverseList(laterHead);
  // run check again L and newHead, until L meet checkEnd
  bool res = true;
  auto checkCurrent = L, checkNew = newHead;
  while (true) {
    if (checkCurrent->data != checkNew->data) {
      res = false;
      break;
    }
    if (checkCurrent->next == checkEnd)
      break; // finish all list
    checkCurrent = checkCurrent->next;
    checkNew = checkNew->next;
  }
  // reverse newHead back
  reverseList(newHead);
  return res;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "is_list_palindromic.cc",
                         "is_list_palindromic.tsv", &IsLinkedListAPalindrome,
                         DefaultComparator{}, param_names);
}
