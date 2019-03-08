#include "list_node.h"
#include "test_framework/generic_test.h"
#include <memory>
using std::shared_ptr;

int Length(shared_ptr<ListNode<int>> head) {
  int l = 0;
  while (head) {
    head = head->next;
    ++l;
  }
  return l;
}

shared_ptr<ListNode<int>> AdvanceByK(shared_ptr<ListNode<int>> head, int k) {
  while (k-- > 0) {
    head = head->next;
  }
  return head;
}

shared_ptr<ListNode<int>> CyclicallyRightShiftList(shared_ptr<ListNode<int>> L,
                                                   int k) {
  if (!L)
    return L;
  int len = Length(L);
  k %= len;
  if (k > 0) {
    // find new head
    auto newTail = AdvanceByK(L, len - k - 1);
    auto newHead = newTail->next;
    newTail->next = nullptr;
    // concat oldTail to L
    auto oldTail = newHead;
    while (oldTail->next) {
      oldTail = oldTail->next;
    }
    oldTail->next = L;
    return newHead;
  } else {
    return L;
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L", "k"};
  return GenericTestMain(
      args, "list_cyclic_right_shift.cc", "list_cyclic_right_shift.tsv",
      &CyclicallyRightShiftList, DefaultComparator{}, param_names);
}
